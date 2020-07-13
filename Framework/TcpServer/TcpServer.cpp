#include "TcpServer.h"

TcpServer::TcpServer(boost::asio::io_service& io_service, short port)
    : io_service_(io_service),
    acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
    StartAccept();
}

void TcpServer::StartAccept()
{
    TcpSession* new_session = new TcpSession(io_service_);
    acceptor_.async_accept(new_session->Socket(),
        boost::bind(&TcpServer::HandleAccept, this, new_session,
            boost::asio::placeholders::error));
}

void TcpServer::HandleAccept(TcpSession* new_session,
    const boost::system::error_code& error)
{
    new_session->Start();

    StartAccept();
}