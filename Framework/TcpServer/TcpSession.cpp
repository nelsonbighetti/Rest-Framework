#pragma once
#include "TcpSession.h"
#include <PgConnection/PgConnectionTypes.h>
#include <HttpFsm/HttpFsmContext.h>
#include <Windows.h>
#include <iostream>

TcpSession::TcpSession(boost::asio::io_service& io_service)
    : socket_(io_service)
{
}

boost::asio::ip::tcp::socket& TcpSession::Socket()
{
    return socket_;
}

void TcpSession::Start()
{
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        boost::bind(&TcpSession::HandleRead, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void TcpSession::HandleRead(const boost::system::error_code& error,
    size_t bytes_transferred)
{
    std::string request(data_);
    auto fsm = HttpFsmContext(request);
    fsm.execute();
    response_data_ = fsm.getResponseString();
    boost::asio::async_write(socket_,
        boost::asio::buffer(response_data_.c_str(), response_data_.length()),
        boost::bind(&TcpSession::HandleWrite, this,
            boost::asio::placeholders::error));
}

void TcpSession::HandleWrite(const boost::system::error_code& error)
{
    delete this; 
}
