#pragma once
#include <string>
#include <deque>
#include <map>
#include "HttpFsmStateIFace.h"
#include "HttpRequestResponse.h"

class DbInterface;

class HttpFsmContext
{
    //PgHostConfig PgServerConfig; 
    bool isCustomConnectionWrapper; 
    DbInterface* connection_wrapper;
    HttpRequest request;
    HttpResponse response;
    std::string requestString; 
    std::string responseString;
    std::map< StateType, HttpFsmStateIFace* > states_map; 
    std::deque< HttpFsmStateIFace* > states_queue;
    std::string sql_query; 
    void CreateStates(); 

public: 
    HttpFsmContext(const std::string& request);
    HttpFsmContext(const std::string& request, DbInterface* customDbConnectionWrapper);

    ~HttpFsmContext();
    HttpFsmStateIFace* GetStateByType(StateType type);
    void pushNextState(HttpFsmStateIFace* next_state);
    void execute();

    DbInterface* getDbConnectionWrapper();

    void updateSqlQuery(const std::string& sql_query_new);
    std::string& getSqlQuery();

    void updateRequest(const HttpRequest& request_new);
    HttpRequest& getRequest();

    void updateResponse(const HttpResponse& response_new);
    HttpResponse& getResponse(); 

    std::string& getRequestString();

    void updateResponseString(const std::string& response_string_new);
    std::string getResponseString();
};