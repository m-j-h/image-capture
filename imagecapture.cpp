#include <cstdlib>
#include <iostream>

#include "image.h"
#include "camera.h"
#include "imagewriter.h"

int main( int argc, char** argv )
{
    if( argc != 2 )
    {
        return EXIT_FAILURE;
    }
    const std::string filePrefix( argv[1] );

    try
    {
        Camera camera( "/dev/video0" );
        IImageProcessor::Ptr imageWriter( new ImageWriter( filePrefix ) );
        camera.CaptureImage( imageWriter );
    }
    catch( std::exception& ex )
    {
        std::cerr << ex.what() << std::endl;
    }
    return EXIT_SUCCESS;
}
