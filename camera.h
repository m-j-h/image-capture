#ifndef CAMERA_H
#define CAMERA_H

#include <string>
#include <vector>

#include "image.h"
#include "device.h"

class Camera
{
public:
    explicit Camera( const std::string& device );
    ~Camera();

    Image CaptureImage();

private:
    struct Buffer 
    {
        void*   start;
        size_t  length;
    };

    void InitMemMap();
    void Start();
    Image CaptureLoop();
    Image ReadFrame();
    Image CreateImage( const void *p, int size );
    void Stop();
    void Log( const std::string& text );

    Device          m_device;
    Buffer*         m_buffers;
    unsigned int    m_nBuffers;
};

#endif // CAMERA_H
