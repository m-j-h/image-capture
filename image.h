#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>

class Image
{
public:
    Image();
    Image( void* data, int size );
    ~Image();

    void Save( const std::string& filename );

private:
    std::vector<unsigned char> m_data;
};

#endif // IMAGE_H
