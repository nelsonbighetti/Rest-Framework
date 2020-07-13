#include "gtest/gtest.h"

#include <iostream>
#include <TcpServer/TcpServer.h>
#include <boost/beast/http/parser.hpp>
#include <boost/beast/http/string_body.hpp>

#include <PgConnection/PgConnectionWrapper.h>
#include <HttpFsm/HttpFsmContext.h>

class PgConnectionMock : public DbInterface
{
    PgResponse response;
    std::string sqlRequestFromFsm; 

public:
    PgConnectionMock() {}

    void setResponse(PgResponse resp)
    {
        response = resp;
    }

    std::string getSqlRequestFromFsm()
    {
        return sqlRequestFromFsm;
    }

    void init(const PgHostConfig& host)
    {

    }

    PgResponse ConnectAndExecute(const std::string& request)
    {
        sqlRequestFromFsm = request; 
        return response;
    }
};

struct testData
{
    std::string HttpRequestString;
    std::string HttpResponseString;
    std::string SqlRequest;
    PgResponse SqlResponse;
};

struct HttpFsmResults
{
    std::string sqlRequest; 
    std::string httpResponse; 
};

testData GETReqWithoutArgs()
{
    std::stringstream http_request_ss;
    http_request_ss << "GET /mydb/demo HTTP/1.1\r\n" <<
        "User-Agent: PostmanRuntime/7.26.1\r\n" <<
        "Accept: */*\r\n";

    std::stringstream http_response_ss;
    http_response_ss << "HTTP/1.1 200 OK\r\nVersion: HTTP/1.1\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: 14\r\n\r\n{{'1','Name'}}";

    PgResponse db_response;
    db_response.success = true;
    db_response.result.push_back({ std::string("1"), std::string("Name") });

    testData data;
    data.HttpRequestString = http_request_ss.str();
    data.HttpResponseString = http_response_ss.str();
    data.SqlRequest = "SELECT * FROM demo";
    data.SqlResponse = db_response;

    return data;
}

testData GETReqWith1Arg()
{
    std::stringstream http_request_ss;
    http_request_ss << "GET /mydb/demo?id=4 HTTP/1.1\r\n" <<
        "User-Agent: PostmanRuntime/7.26.1\r\n" <<
        "Accept: */*\r\n";

    std::stringstream http_response_ss;
    http_response_ss << "HTTP/1.1 200 OK\r\nVersion: HTTP/1.1\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: 14\r\n\r\n{{'4','Test'}}";

    PgResponse db_response;
    db_response.success = true;
    db_response.result.push_back({ std::string("4"), std::string("Test") });

    testData data;
    data.HttpRequestString = http_request_ss.str();
    data.HttpResponseString = http_response_ss.str();
    data.SqlRequest = "SELECT * FROM demo WHERE id='4'";
    data.SqlResponse = db_response;

    return data;
}

testData GETReqWith2Args()
{
    std::stringstream http_request_ss;
    http_request_ss << "GET /mydb/demo?id=4&name=Ivan HTTP/1.1\r\n" <<
        "User-Agent: PostmanRuntime/7.26.1\r\n" <<
        "Accept: */*\r\n";

    std::stringstream http_response_ss;
    http_response_ss << "HTTP/1.1 200 OK\r\nVersion: HTTP/1.1\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: 14\r\n\r\n{{'4','Ivan'}}";

    PgResponse db_response;
    db_response.success = true;
    db_response.result.push_back({ std::string("4"), std::string("Ivan") });

    testData data;
    data.HttpRequestString = http_request_ss.str();
    data.HttpResponseString = http_response_ss.str();
    data.SqlRequest = "SELECT * FROM demo WHERE id='4' AND name='Ivan'";
    data.SqlResponse = db_response;

    return data;
}


testData InvalidMethod()
{
    std::stringstream http_request_ss;
    http_request_ss << "PATCH /mydb/demo?id=4&name=Ivan HTTP/1.1\r\n" <<
        "User-Agent: PostmanRuntime/7.26.1\r\n" <<
        "Accept: */*\r\n";

    std::stringstream http_response_ss;
    http_response_ss << "HTTP/1.1 501 Not Implemented\r\nVersion: HTTP/1.1\r\nContent-Type: text/html; charset=utf-8\r\n\r\n\r\n";

    PgResponse db_response;
    db_response.success = false;

    testData data;
    data.HttpRequestString = http_request_ss.str();
    data.HttpResponseString = http_response_ss.str();
    data.SqlRequest = "";
    data.SqlResponse = db_response;

    return data;
}

testData DBConnectionError()
{
    std::stringstream http_request_ss;
    http_request_ss << "GET /mydb/demo?id=4 HTTP/1.1\r\n" <<
        "User-Agent: PostmanRuntime/7.26.1\r\n" <<
        "Accept: */*\r\n";

    std::stringstream http_response_ss;
    http_response_ss << "HTTP/1.1 " << "502 Bad Gateway" << "\r\n"
        << "Version: HTTP/1.1\r\n"
        << "Content-Type: text/html; charset=utf-8\r\n"
        << "\r\n\r\n";

    PgResponse db_response;
    db_response.success = false;

    testData data;
    data.HttpRequestString = http_request_ss.str();
    data.HttpResponseString = http_response_ss.str();
    data.SqlRequest = "SELECT * FROM demo WHERE id='4'";
    data.SqlResponse = db_response;

    return data;
}




HttpFsmResults HttpFsmTestImpl(testData data)
{
    PgConnectionMock* mock = new PgConnectionMock();
    
    mock->setResponse(data.SqlResponse);

    auto fsm = HttpFsmContext(data.HttpRequestString, mock);
    fsm.execute();

    HttpFsmResults result; 
    result.sqlRequest = mock->getSqlRequestFromFsm();
    result.httpResponse = fsm.getResponseString();
    return result;
}

TEST(HttpFsmTest, GETReqWithoutArgs) 
{
    auto parameters = GETReqWithoutArgs();
    auto result = HttpFsmTestImpl(parameters);

    EXPECT_EQ(true, parameters.HttpResponseString == result.httpResponse);
    EXPECT_EQ(true, parameters.SqlRequest == result.sqlRequest);
    EXPECT_TRUE(true);
}

TEST(HttpFsmTest, GETReqWith1Arg)
{
    auto parameters = GETReqWith1Arg();
    auto result = HttpFsmTestImpl(parameters);

    EXPECT_EQ(true, parameters.HttpResponseString == result.httpResponse);
    EXPECT_EQ(true, parameters.SqlRequest == result.sqlRequest);
    EXPECT_TRUE(true);
}

TEST(HttpFsmTest, GETReqWith2Args)
{
    auto parameters = GETReqWith2Args();
    auto result = HttpFsmTestImpl(parameters);

    EXPECT_EQ(true, parameters.HttpResponseString == result.httpResponse);
    EXPECT_EQ(true, parameters.SqlRequest == result.sqlRequest);
    EXPECT_TRUE(true);
}

TEST(HttpFsmTest, InvalidMethod)
{
    auto parameters = InvalidMethod();
    auto result = HttpFsmTestImpl(parameters);

    EXPECT_EQ(true, parameters.HttpResponseString == result.httpResponse);
    EXPECT_EQ(true, parameters.SqlRequest == result.sqlRequest);
    EXPECT_TRUE(true);
}

TEST(HttpFsmTest, DBConnectionError)
{
    auto parameters = DBConnectionError();
    auto result = HttpFsmTestImpl(parameters);

    EXPECT_EQ(true, parameters.HttpResponseString == result.httpResponse);
    EXPECT_EQ(true, parameters.SqlRequest == result.sqlRequest);
    EXPECT_TRUE(true);
}