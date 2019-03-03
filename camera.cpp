#include "camera.h"
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <linux/videodev2.h>
#include <sys/mman.h>

namespace
{

template <typename T>
void Clear( T& object )
{
    ::memset( (void*)(&object), 0x0, sizeof(T) );
}

}

Camera::Camera( const std::string& device )
:   m_device( device ),
    m_buffers( nullptr ),
    m_nBuffers( 0 )
{
    struct v4l2_capability cap;
    m_device.Ioctl( VIDIOC_QUERYCAP, &cap );
    if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
    {
        throw std::runtime_error("Device is not a video capture device");
    }

    if(!(cap.capabilities & V4L2_CAP_STREAMING)) 
    {
        throw std::runtime_error("Device does not support streaming i/o");
    }

    /* Select video input, video standard and tune here. */
    struct v4l2_cropcap cropcap;
    Clear(cropcap);
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    m_device.Ioctl( VIDIOC_CROPCAP, &cropcap );

    struct v4l2_format fmt;
    Clear(fmt);
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = 640;
    fmt.fmt.pix.height      = 480;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field       = V4L2_FIELD_NONE; // INTERLACED;
    m_device.Ioctl( VIDIOC_S_FMT, &fmt );

    /* Buggy driver paranoia. */
    unsigned int min = fmt.fmt.pix.width * 2;
    if (fmt.fmt.pix.bytesperline < min)
        fmt.fmt.pix.bytesperline = min;
    min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
    if (fmt.fmt.pix.sizeimage < min)
        fmt.fmt.pix.sizeimage = min;

    InitMemMap();
}

Camera::~Camera()
{
    for (unsigned int i = 0; i < m_nBuffers; ++i)
    {
        munmap( m_buffers[i].start, m_buffers[i].length);
    }
    free(m_buffers);
    m_buffers = nullptr;
}

void Camera::CaptureImage( const IImageProcessor::Ptr& imageProcessor )
{
    Start();
    CaptureLoop( imageProcessor );
    Stop();
}

void Camera::InitMemMap()
{
    struct v4l2_requestbuffers req;

    Clear(req);
    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    m_device.Ioctl( VIDIOC_REQBUFS, &req );
    if(req.count < 2) 
    {
        throw std::runtime_error("Insufficient buffer memory");
    }

    m_buffers = (Buffer*)calloc(req.count, sizeof(*m_buffers));
    if(!m_buffers) 
    {
        throw std::runtime_error("Failed to calloc() buffer memory");
    }

    for (m_nBuffers = 0; m_nBuffers < req.count; ++m_nBuffers)
    {
        struct v4l2_buffer buf;
        Clear(buf);
        buf.type    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory  = V4L2_MEMORY_MMAP;
        buf.index   = m_nBuffers;

        m_device.Ioctl( VIDIOC_QUERYBUF, &buf );

        m_buffers[m_nBuffers].length = buf.length;
        m_buffers[m_nBuffers].start =
                mmap(   NULL,   // start anywhere
                        buf.length,
                        PROT_READ | PROT_WRITE, // required
                        MAP_SHARED,     // recommended
                        m_device.Fd(), 
                        buf.m.offset);

        if( m_buffers[m_nBuffers].start == MAP_FAILED )
        {
            throw std::runtime_error("mmap() failed");
        }
    }
}

void Camera::Start()
{
    for (unsigned int i = 0; i < m_nBuffers; ++i)
    {
            struct v4l2_buffer buf;
            Clear(buf);

            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_MMAP;
            buf.index = i;

            m_device.Ioctl( VIDIOC_QBUF, &buf );
    }

    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    m_device.Ioctl( VIDIOC_STREAMON, &type );
}

void Camera::CaptureLoop( const IImageProcessor::Ptr& imageProcessor )
{
    unsigned int count = 1;
    while (count-- > 0) 
    {
        for (;;) 
        {
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(m_device.Fd(), &fds);

            /* Timeout. */
            struct timeval tv;
            tv.tv_sec = 5;
            tv.tv_usec = 0;

            int r = select(m_device.Fd() + 1, &fds, NULL, NULL, &tv);
            if( r == -1 ) 
            {
                if( EINTR == errno )
                {
                    continue;
                }
                throw std::runtime_error("select() failed");
            }

            if(0 == r) 
            {
                throw std::runtime_error("select timeout");
            }
            ReadFrame( imageProcessor );
            return;
        }
    }
}

void Camera::ReadFrame( const IImageProcessor::Ptr& imageProcessor )
{
    struct v4l2_buffer buf;
    unsigned int i;

    Clear(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

    m_device.Ioctl( VIDIOC_DQBUF, &buf);
    Image image = CreateImage(m_buffers[buf.index].start, buf.bytesused);
    m_device.Ioctl( VIDIOC_QBUF, &buf);

    imageProcessor->ProcessImage( image );
}

Image Camera::CreateImage( const void *p, int size )
{
    return Image( (unsigned char*)(p), size );
}

void Camera::Stop()
{
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    m_device.Ioctl( VIDIOC_STREAMOFF, &type);
}

void Camera::Log( const std::string& text )
{
    std::cerr << "LOG: " << text << std::endl;
}
