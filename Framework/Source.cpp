#include <iostream>
#include "TcpServer/TcpServer.h"
#include <boost/beast/http/parser.hpp>
#include <boost/beast/http/string_body.hpp>

int main()
{
    try
    {
        boost::asio::io_service io_service;
        TcpServer s(io_service, 5555);
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    
	return 0; 
}