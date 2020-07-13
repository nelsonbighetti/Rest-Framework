#pragma once
#include "HttpFsmContext.h"
#include <iostream>

// Forward declaration to resolve circular dependency/include
class HttpFsmContext;

enum StateType
{
    ParseHttpReq_state,
    FillDBRequest_state, 
    ExecuteDBTransaction_state,
    SendHttpResponse_state
};

class HttpFsmStateIFace
{
public:
    virtual StateType getType() = 0;
    virtual void execute(HttpFsmContext* context) = 0; 
    virtual ~HttpFsmStateIFace(){}
};