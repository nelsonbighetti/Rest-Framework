#pragma once
#include <boost/bind.hpp>
#include <boost/asio.hpp>


class TcpSession
{
public:
    TcpSession(boost::asio::io_service& io_service);

    boost::asio::ip::tcp::socket& Socket();

    void Start(); 

private:
    void HandleRead(const boost::system::error_code& error,
        size_t bytes_transferred);

    void HandleWrite(const boost::system::error_code& error);

    boost::asio::ip::tcp::socket socket_;

    enum { max_length = 4096 };

    char data_[max_length];

    std::string response_data_;
};