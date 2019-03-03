#ifndef CAMERA_H
#define CAMERA_H

#include <string>
#include <vector>

#include "image.h"
#include "device.h"
#include "iimageprocessor.h"

class Camera
{
public:
    explicit Camera( const std::string& device );
    ~Camera();

    void AddImageProcessor( const IImageProcessor::Ptr& imageProcessor ); 
    void CaptureImage();

private:
    struct Buffer 
    {
        void*   start;
        size_t  length;
    };
    using ImageProcessors = std::vector<IImageProcessor::Ptr>;

    void InitMemMap();
    void Start();
    void CaptureLoop();
    void ReadFrame();
    Image WriteFrame(const void *p, int size);
    void Stop();
    void Log( const std::string& text );
    void ProcessImage( Image& image );

    Device          m_device;
    Buffer*         m_buffers;
    unsigned int    m_nBuffers;
    ImageProcessors m_imageProcessors;
};

#endif // CAMERA_H
