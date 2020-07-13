#include <string>
#include <vector>

struct PgHostConfig
{
    std::string host;
    int         port;
    std::string name;
    std::string user;
    std::string pass;
};

struct PgResponse
{
    bool success;
    std::vector<std::vector<std::string>> result;
};

class DbConfigSingleton
{
private:
    DbConfigSingleton() {}
    DbConfigSingleton(const DbConfigSingleton&);
    DbConfigSingleton& operator=(DbConfigSingleton&);
public:
    static DbConfigSingleton& Instance()
    {
        static DbConfigSingleton inst;
        return inst;
    }

    PgHostConfig GetPostgresQLServerConfig()
    {
        return { "192.168.1.39",5432,"mydb","myuser","mypass" };
    }
};