#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <thread>
#include <boost/asio.hpp>

class Controller
{
public:
    Controller();
    ~Controller();

    void Run();

private:
    std::thread             m_thread;
    boost::asio::io_service m_service;
};

#endif // CONTROLLER_H
