#pragma once
#include "HttpAuxiliary.h"
#include "HttpFsmStateIFace.h"


class ParseHttpReq : public HttpFsmStateIFace
{
	StateType type;

public:
	ParseHttpReq()
	{
		type = StateType::ParseHttpReq_state;
	}
	~ParseHttpReq()
	{
	}
	StateType getType() 
	{ 
		return type; 
	}
	void execute(HttpFsmContext* context)
	{
		if (context->getRequestString().length() > 0)
		{
			HttpRequest request; 

			std::vector<std::string> tokens = SplitString(context->getRequestString(), "\r\n");
			if (tokens[0].substr(0, 3) == "GET")
			{
				request.request_type = RequestType::GET_req;
				tokens[0] = tokens[0].substr(4, tokens[0].length() - 3);

				auto request_address_and_parameters = SplitString(tokens[0].substr(0, tokens[0].length() - 9), "?&");
				request.address = request_address_and_parameters[0];
				request_address_and_parameters.erase(request_address_and_parameters.begin());

				std::map<std::string, std::string> parameters;
				if (request_address_and_parameters.size())
				{
					for (auto p : request_address_and_parameters)
					{
						auto parameter_tokenized = SplitString(p, "=");
						request.parameters[parameter_tokenized[0]] = parameter_tokenized[1];
					}
				}

				context->updateRequest(request);
				context->pushNextState(context->GetStateByType(StateType::FillDBRequest_state));
			}
			else
			{
				HttpResponse response; 
				response.code = ResponseCodeType::Not_Implemented_501;
				context->updateResponse(response);
				context->pushNextState(context->GetStateByType(StateType::SendHttpResponse_state));
			}
		}
		else
		{
			HttpResponse response;
			response.code = ResponseCodeType::No_Content_204;
			context->updateResponse(response);
			context->pushNextState(context->GetStateByType(StateType::SendHttpResponse_state));
		}
	}
};
