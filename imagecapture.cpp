#include <cstdlib>
#include <iostream>

#include "image.h"
#include "camera.h"
#include "imagewriter.h"
#include "controller.h"

int main( int argc, char** argv )
{
    if( argc != 2 )
    {
        return EXIT_FAILURE;
    }
    const std::string filePrefix( argv[1] );

    try
    {
        Controller controller( "/dev/video0", "image" );
        controller.Run();
    }
    catch( std::exception& ex )
    {
        std::cerr << ex.what() << std::endl;
    }
    return EXIT_SUCCESS;
}
