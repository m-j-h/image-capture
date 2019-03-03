#include "controller.h"
#include "imagewriter.h"

Controller::Controller( const std::string& cameraDevice,
                        const std::string& filePrefix )
:   m_camera( cameraDevice ),
    m_imageProcessor( ImageWriter::Create( filePrefix ) )
{}

Controller::~Controller()
{}

void Controller::Run()
{
    CaptureImage();
}

void Controller::CaptureImage()
{
    m_camera.CaptureImage( m_imageProcessor );
}
