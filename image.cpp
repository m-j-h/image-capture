#include "image.h"
#include <cstring>
#include <fstream>

Image::Image()
: m_data()
{}

Image::Image( void* data, int size )
: m_data()
{
    m_data.resize( size );
    ::memcpy( m_data.data(), data, size );
}

Image::~Image()
{}

void Image::Save( const std::string& filename )
{
    std::ofstream file( filename.c_str(), std::ios::binary | std::ios::out );
    char* data = (char*)( m_data.data() );
    file.write( const_cast<const char*>(data), m_data.size() );
    //fwrite( const_cast<const signed char*>(data), m_data.size(), 1, stdout);
    //fflush(stdout);
}
