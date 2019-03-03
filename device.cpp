#include "device.h"

#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <iostream>

Device::Device( const std::string& device )
: m_fd(-1)
{
    struct stat st;

    if( stat(device.c_str(), &st) == -1 ) 
    {
        throw std::runtime_error("stat() failed");
    }

    if (!S_ISCHR(st.st_mode)) 
    {
        throw std::runtime_error("Invalid device");
    }

    m_fd = open(device.c_str(), O_RDWR | O_NONBLOCK, 0);
    if( m_fd == -1 ) 
    {
        throw std::runtime_error("open()");
    }
}

Device::~Device()
{
    if( m_fd == -1 ) 
    {
        return;
    }
    close( m_fd );
    m_fd = -1;
}

void Device::IoctlImpl( unsigned long request, void* param )
{
    int r = 0;
    do 
    {
        r = ioctl( m_fd, request, param );
    }
    while( r == -1 && errno == EINTR );

    if( r == -1 )
    {
        throw std::runtime_error("Device: ioctl() failed");
    }
}
