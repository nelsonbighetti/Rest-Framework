#include <string>

#include "PgConnection.h"
#include "PgConnectionTypes.h"

PGConnection::PGConnection(const PgHostConfig& host_config) :
    m_dbhost(host_config.host),
    m_dbport(host_config.port),
    m_dbname(host_config.name),
    m_dbuser(host_config.user),
    m_dbpass(host_config.pass)
{

    m_connection.reset( PQsetdbLogin(m_dbhost.c_str(), std::to_string(m_dbport).c_str(), nullptr, nullptr, m_dbname.c_str(), m_dbuser.c_str(), m_dbpass.c_str()), &PQfinish );

    if (PQstatus( m_connection.get() ) != CONNECTION_OK && PQsetnonblocking(m_connection.get(), 1) != 0 )
    {
       throw std::runtime_error( PQerrorMessage( m_connection.get() ) );
    }
}


std::shared_ptr<PGconn> PGConnection::connection() const
{
    return m_connection;
}

