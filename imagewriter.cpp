#include "imagewriter.h"
#include "image.h"

IImageProcessor::Ptr ImageWriter::Create( const std::string& filePrefix )
{
    return Ptr( new ImageWriter( filePrefix ) );
}

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
