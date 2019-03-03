#ifndef IMAGEWRITER_H
#define IMAGEWRITER_H

#include <string>
#include "iimageprocessor.h"

class ImageWriter : public IImageProcessor
{
public:
    explicit ImageWriter( const std::string& filePrefix );
    virtual ~ImageWriter();

    virtual void ProcessImage( Image& image );

private:
    std::string     m_filePrefix;
    unsigned long   m_count;
};

#endif // IMAGEWRITER_H
