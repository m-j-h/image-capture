#ifndef DEVICE_H
#define DEVICE_H

#include <string>

class Device
{
public:
    explicit Device( const std::string& device );
    ~Device();

    template <typename T>
    void Ioctl( unsigned long request, T* param )
    {
        IoctlImpl( request, (void*)(param) );
    }

    int Fd() { return m_fd; }

private:
    void IoctlImpl( unsigned long request, void* param );
    void Log( const std::string& text );
    int m_fd;
};

#endif // DEVICE_H
