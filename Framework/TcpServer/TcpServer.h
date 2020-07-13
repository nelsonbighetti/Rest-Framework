#pragma once 

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "TcpSession.h"


class TcpServer
{
public:
    TcpServer(boost::asio::io_service& io_service, short port);

private:
    void StartAccept();

    void HandleAccept(TcpSession* new_session,
        const boost::system::error_code& error);

    boost::asio::io_service& io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
};