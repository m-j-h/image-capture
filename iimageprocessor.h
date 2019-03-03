#ifndef IIMAGEPROCESSOR_H
#define IIMAGEPROCESSOR_H

#include <memory>

class Image;

class IImageProcessor
{
public:
    using Ptr = std::shared_ptr<IImageProcessor>;
    
    virtual ~IImageProcessor() {}
    virtual void ProcessImage( Image& image ) = 0;
};

#endif // IIMAGEPROCESSOR_H
