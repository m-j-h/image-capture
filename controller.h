#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>
#include "iimageprocessor.h"
#include "camera.h"

class Controller
{
public:
    Controller( const std::string& cameraDevice,
                const std::string& filePrefix );
    ~Controller();

    void Run();

private:
    void CaptureImage();

    Camera                  m_camera;
    IImageProcessor::Ptr    m_imageProcessor;
};

#endif // CONTROLLER_H
