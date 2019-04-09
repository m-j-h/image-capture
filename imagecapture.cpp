#include <cstdlib>
#include <iostream>

#include "camera.h"
#include "image.h"

int main( int argc, char** argv )
{
    if( argc < 2 || argc > 3 )
    {
        std::cerr << "usage: imagecapture <filename> [<device>]" << std::endl;
        return EXIT_FAILURE;
    }
    const std::string filename( argv[1] );
    const std::string device = ( argc == 3 ) ? std::string(argv[2]) : std::string("/dev/video0");
 
    try
    {
        Camera camera( device );
        auto image = camera.CaptureImage();
        image.Save( filename );
    }
    catch( std::exception& ex )
    {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
