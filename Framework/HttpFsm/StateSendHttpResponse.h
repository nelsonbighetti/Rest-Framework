#pragma once
#include "HttpFsmStateIFace.h"

class SendHttpResponse : public HttpFsmStateIFace
{
	StateType type;
public:
	SendHttpResponse()
	{
		type = StateType::SendHttpResponse_state;
	}
	~SendHttpResponse()
	{
	}
	StateType getType() 
	{
		return type; 
	}
	void execute(HttpFsmContext* context) 
	{
		auto resp = context->getResponse();
		std::stringstream response;
		if (resp.code == ResponseCodeType::OK_200)
		{
			response << "HTTP/1.1 " << "200 OK" << "\r\n"
				<< "Version: HTTP/1.1\r\n"
				<< "Content-Type: text/html; charset=utf-8\r\n"
				<< "Content-Length: " << resp.payload.length()
				<< "\r\n\r\n"
				<< resp.payload;
		}
		else if (resp.code == ResponseCodeType::No_Content_204)
		{
			response << "HTTP/1.1 " << "204 No Content" << "\r\n"
				<< "Version: HTTP/1.1\r\n"
				<< "Content-Type: text/html; charset=utf-8\r\n"
				<< "\r\n\r\n";
		}
		else if (resp.code == ResponseCodeType::Not_Implemented_501)
		{
			response << "HTTP/1.1 " << "501 Not Implemented" << "\r\n"
				<< "Version: HTTP/1.1\r\n"
				<< "Content-Type: text/html; charset=utf-8\r\n"
				<< "\r\n\r\n";
		}
		else if (resp.code == ResponseCodeType::Bad_Gateway_502)
		{
			response << "HTTP/1.1 " << "502 Bad Gateway" << "\r\n"
				<< "Version: HTTP/1.1\r\n"
				<< "Content-Type: text/html; charset=utf-8\r\n"
				<< "\r\n\r\n";
		}
		context->updateResponseString(response.str());
	}
};

