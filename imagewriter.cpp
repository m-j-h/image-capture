#include "imagewriter.h"
#include "image.h"

ImageWriter::ImageWriter( const std::string& filePrefix )
:   m_filePrefix( filePrefix ),
    m_count( 0L )
{}

ImageWriter::~ImageWriter()
{}

void ImageWriter::ProcessImage( Image& image )
{
    const std::string filename = m_filePrefix + "_" + std::to_string( m_count++ ) + ".img";
    image.Save( filename );
}
