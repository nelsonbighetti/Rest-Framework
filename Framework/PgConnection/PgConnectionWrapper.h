#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include "PgBackend.h"
#include "PgConnectionTypes.h"

class DbInterface
{
public:
    virtual void init(const PgHostConfig& host) = 0;
    virtual PgResponse ConnectAndExecute(const std::string& request) = 0;
};

class PgConnectionWrapper : public DbInterface
{
    std::shared_ptr<PGBackend> pgbackend_ptr;

public:

    PgConnectionWrapper(){}

    void init(const PgHostConfig& host)
    {
        pgbackend_ptr = std::make_shared<PGBackend>(host);
    }

    PgResponse ConnectAndExecute(const std::string& request)
    {
        auto conn = pgbackend_ptr->connection();
        PgResponse response;

        PQsendQuery(conn->connection().get(), request.c_str());

        while (auto res_ = PQgetResult(conn->connection().get()))
        {
            if (PQresultStatus(res_) == PGRES_TUPLES_OK && PQntuples(res_))
            {
                auto ID = PQgetvalue(res_, 0, 0);
                int nFields = PQnfields(res_);
                for (int i = 0; i < PQntuples(res_); i++)
                {
                    std::vector<std::string> row;
                    for (int j = 0; j < nFields; j++)
                        row.push_back(std::string(PQgetvalue(res_, i, j)));
                    response.result.push_back(row);
                }
                response.success = true;
            }

            if (PQresultStatus(res_) == PGRES_FATAL_ERROR)
            {
                response.success = false;
                std::cout << PQresultErrorMessage(res_) << std::endl;
                break;
            }

            PQclear(res_);
        }

        pgbackend_ptr->freeConnection(conn);
        return response;
    }
};
