#pragma once
#include <map>
#include <string>

enum RequestType
{
	GET_req
};

enum ResponseCodeType
{
	OK_200, 
	No_Content_204, 
	Not_Implemented_501, 
	Bad_Gateway_502
};

struct HttpRequest
{
	RequestType request_type; 
	std::map<std::string, std::string> parameters; 
	std::string address; 
};

struct HttpResponse
{
	ResponseCodeType code;
	std::string payload; 
};