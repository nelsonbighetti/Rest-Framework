#ifndef PGCONNECTION_H
#define PGCONNECTION_H

#include <memory>
#include <mutex>
#include <libpq-fe.h>

struct PgHostConfig; 


class PGConnection
{
public:
    PGConnection(const PgHostConfig& host_config);

    std::shared_ptr<PGconn> connection() const;

private:
    void establish_connection();

    std::string m_dbhost;
    int         m_dbport;
    std::string m_dbname;
    std::string m_dbuser;
    std::string m_dbpass;

    std::shared_ptr<PGconn>  m_connection;
};


#endif //PGCONNECTION_H

