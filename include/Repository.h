#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <Poco/Data/Time.h>
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
using Poco::Data::Time;
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

struct CityPairDistance
{
    string      id;
    int         tenant_id;
    string      city_from_id;
    string      city_to_id;
    double      distance;
    Time        inserted_date;
    Time        updated_date;
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
            static size_t maxPoolSize;
            static size_t connectorCount;
        public:
            Repository(size_t minSessions = 1, size_t maxSessions = 32, size_t idleTime = 60);
            ~Repository();

            void initTenant();
            void insert(Tenant & );
            void popById(int, Tenant &);
            void popById(string &, City &);
            void popById(string &, CityPairDistance &);

            void popTenants(std::vector<int> &, std::vector<string> &);
            void popAll(std::vector<Tenant> &);
            void popAll(std::vector<City> &);
            void popAll(std::vector<CityPairDistance> &);

    };
}
#endif
