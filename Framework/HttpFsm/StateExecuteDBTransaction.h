#pragma once
#include <PgConnection/PgConnectionWrapper.h>
#include "HttpFsmStateIFace.h"


class ExecuteDBTransaction : public HttpFsmStateIFace
{
	StateType type;
public:

	ExecuteDBTransaction()
	{
		type = StateType::ExecuteDBTransaction_state;
	}

	~ExecuteDBTransaction()
	{

	}

	StateType getType() 
	{ 
		return type; 
	}

	void execute(HttpFsmContext* context) 
	{
		auto sql_query = context->getSqlQuery();
	
		std::string req = context->getSqlQuery();
		auto db_resp = context->getDbConnectionWrapper()->ConnectAndExecute(req);
		HttpResponse http_resp; 
		if (db_resp.success)
		{
			http_resp.code = ResponseCodeType::OK_200;

			std::stringstream json;
			json << "{";

			int row_counter = 0;
			for (auto row : db_resp.result)
			{
				if (row_counter)
					json << ",";

				json << "{";
				int column_counter = 0; 
				for (auto column : row)
				{
					if (column_counter)
						json << ","; 
					json << "'" << column << "'";
					column_counter++;
				}
				json << "}";

				row_counter++; 
			}
			json << "}"; 
			http_resp.payload = json.str();
		}
		else
		{
			http_resp.code = ResponseCodeType::Bad_Gateway_502;
		}

		context->updateResponse(http_resp);
		context->pushNextState(context->GetStateByType(StateType::SendHttpResponse_state));
	}
};
