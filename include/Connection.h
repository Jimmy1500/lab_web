#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <string>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionFactory.h>

#include <iostream>
#include <thread>
#include <chrono>

using namespace std;
using namespace Poco::Data::Keywords;
using Poco::Data::Session;
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
    namespace Data {
        class Connection {
            private:
                string host="host=127.0.0.1;";
                string user="user=jding;";
                string password="password=jding;";
                string db="db=optitrade_emea;";
                string port="port=3306;";
                string compress="compress=true;";
                string autoReconnect="auto-connect=true;";
                string credentials;

                static Connection * pInstance;

                Connection();
                ~Connection();
            public:
                static Connection * getInstance();
                static void close();

                Poco::Data::Session getSession();
        };
    }
}

#endif //CONNECTION_H
