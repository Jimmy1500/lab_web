#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/MySQL/MySQLException.h>

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;
using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::SessionPool;
using Poco::Data::SessionFactory;

struct Tenant
{
    int         id;
    string      name;
};

struct City
{
    string      id;
    int         tenant_id;
    string      name;
    string      name_native;
    double      latitude;
    double      longitude;
    string      county;
    string      transportation_region;
    string      position_region;
    string      country_code;
};

namespace Component {
    class Repository {
        private:
            string host="127.0.0.1";
            string user;
            string password;
            string db;
            string port="3306";
            string compress="true";
            string autoReconnect="true";
            string connectionString;

            static SessionPool * pool;
            static size_t poolRefCount;
        public:
            Repository(size_t minSessions = 1, size_t maxSessions = 32, size_t idleTime = 60);
            ~Repository();

            void initTenant();

            void insert(Tenant & tenent);
            void popTenants(std::vector<int> &, std::vector<string> &);
            void popById(Tenant & tenent);

            void scanCities();
            void popById(City & city);
    };
}
#endif
