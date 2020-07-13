#ifndef PGBACKEND_H
#define PGBACKEND_H

#include <memory>
#include <mutex>
#include <string>
#include <queue>
#include <condition_variable>
#include <libpq-fe.h>
#include "PgConnection.h"


class PGBackend
{
public:
    PGBackend(const PgHostConfig& host_config);
    std::shared_ptr<PGConnection> connection();
    void freeConnection(std::shared_ptr<PGConnection>);

private:
    void createPool(const PgHostConfig& host_config);
    
    std::mutex m_mutex;
    std::condition_variable m_condition;
    std::queue<std::shared_ptr<PGConnection>> m_pool;

    const int POOL = 10;
};

#endif //PGBACKEND_H
