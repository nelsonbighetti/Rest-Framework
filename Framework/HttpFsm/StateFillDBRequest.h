#pragma once
#include "HttpAuxiliary.h"
#include "HttpFsmStateIFace.h"

class FillDbRequest : public HttpFsmStateIFace
{
	StateType type;
public:
	FillDbRequest()
	{
		type = StateType::FillDBRequest_state;

	}
	~FillDbRequest()
	{

	}
	StateType getType() 
	{ 
		return type; 
	}

	void execute(HttpFsmContext* context)
	{
		auto request = context->getRequest();
		auto tokens = SplitString(request.address, "\/");

		std::stringstream sql_query;
		sql_query << "SELECT * FROM " << tokens[2];

		if (request.parameters.size())
		{
			sql_query << " WHERE "; 
			int counter = 0;
			for (auto p : request.parameters)
			{
				if (counter)
					sql_query << " AND "; 
				sql_query << p.first;
				sql_query << "='" << p.second << "'";
				counter++; 
			}
		}
		
		context->updateSqlQuery(sql_query.str());
		context->pushNextState(context->GetStateByType(StateType::ExecuteDBTransaction_state));
	}
};