#include <sstream>

#include "HttpFsmContext.h"
#include "HttpFsmStatesCreator.h"

#include "StateParseHttpReq.h"
#include "StateFillDbRequest.h"
#include "StateExecuteDBTransaction.h"
#include "StateSendHttpResponse.h"


void HttpFsmContext::CreateStates()
{
	states_map = HttpFsmStatesCreator::CreateStatesInstances< ParseHttpReq, FillDbRequest, ExecuteDBTransaction, SendHttpResponse >();
}

HttpFsmContext::HttpFsmContext(const std::string& request) : requestString(request)
{
    CreateStates();
    states_queue.push_back(GetStateByType(StateType::ParseHttpReq_state));
    isCustomConnectionWrapper = false; 
    connection_wrapper = new PgConnectionWrapper();
    connection_wrapper->init(DbConfigSingleton::Instance().GetPostgresQLServerConfig());
}

HttpFsmContext::HttpFsmContext(const std::string& request, DbInterface* customDbConnectionWrapper) : requestString(request)
{
    CreateStates();
    states_queue.push_back(GetStateByType(StateType::ParseHttpReq_state));
    isCustomConnectionWrapper = true;
    connection_wrapper = customDbConnectionWrapper;
}

HttpFsmContext::~HttpFsmContext()
{
    if(!isCustomConnectionWrapper)
        delete connection_wrapper;

    for (auto state : states_map)
        delete state.second;
}

HttpFsmStateIFace* HttpFsmContext::GetStateByType(StateType type)
{
    return states_map[type];
}

void HttpFsmContext::pushNextState(HttpFsmStateIFace* next_state)
{
    states_queue.push_back(next_state);
}

void HttpFsmContext::execute()
{
    while (states_queue.size())
    {
        states_queue.front()->execute(this);
        states_queue.pop_front();
    }
}

DbInterface* HttpFsmContext::getDbConnectionWrapper()
{
    return connection_wrapper;
}

void HttpFsmContext::updateSqlQuery(const std::string& sql_query_new)
{
    sql_query = sql_query_new;
}

std::string& HttpFsmContext::getSqlQuery()
{
    return sql_query;
}

void HttpFsmContext::updateRequest(const HttpRequest& request_new)
{
    request = request_new;
}

HttpRequest& HttpFsmContext::getRequest()
{
    return request;
}

void HttpFsmContext::updateResponse(const HttpResponse& response_new)
{
    response = response_new;
}

HttpResponse& HttpFsmContext::getResponse()
{
    return response; 
}

std::string& HttpFsmContext::getRequestString()
{
    return requestString;
}

void HttpFsmContext::updateResponseString(const std::string& response_string_new)
{
    responseString = response_string_new;
}

std::string HttpFsmContext::getResponseString()
{
    return responseString;
}