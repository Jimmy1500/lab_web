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
using namespace Poco::Data::MySQL;
using Poco::Data::Session;
using Poco::Data::SessionFactory;
using Poco::Data::Statement;

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
    string      trans_region;
    string      posit_region;
    string      country_code;
};

int main()
{
    // register SQLite connector
    Poco::Data::MySQL::Connector::registerConnector();
    try
    {
        // create a session
        string str = "host=127.0.0.1;user=jding;password=jding;compress=true;db=optitrade_emea;port=3306;auto-reconnect=true";
        Poco::Data::Session session(Poco::Data::SessionFactory::instance().create(Poco::Data::MySQL::Connector::KEY, str));

        // session << "DROP TABLE IF EXISTS tenant", now;
        // session << "CREATE TABLE tenant (tenant_id int(30), tenant_name VARCHAR(45), inserted_date DATETIME, updated_data DATETIME)", now;

        Tenant tenant = { 0, "Germany" };

        Statement insert_tenant(session);
        insert_tenant << "INSERT INTO tenant (tenant_name) (SELECT ? WHERE NOT EXISTS (SELECT * FROM tenant WHERE tenant_name = ?))", use(tenant.name), use(tenant.name);
        insert_tenant.execute();

        Statement select_tenant(session);
        select_tenant << "SELECT * FROM tenant", into(tenant.id), into(tenant.name), range(0, 1); //  iterate over result set one row at a time
        while (!select_tenant.done())
        {
            select_tenant.execute();
            cout << tenant.id << " "
                << tenant.name
                << endl;
        }

        City city = { "uuid", 0, "city_name", "city_name_native", 0.0, 0.0, "cnty", "transregion", "posregion", "cntry" };
        Statement select_city(session);
        select_city << "SELECT * FROM city",
                    into(city.id), into(city.tenant_id), into(city.name), into(city.name_native), into(city.latitude), into(city.longitude), into(city.county),
                    into(city.trans_region), into(city.posit_region), into(city.country_code), range(0, 1);
        while (!select_city.done())
        {
            select_city.execute();
            cout << city.id << " "
                << city.name << " "
                << city.name_native << " "
                << city.latitude << " "
                << city.longitude << " "
                << city.county << " "
                << city.trans_region << " "
                << city.posit_region << " "
                << city.country_code << " "
                << endl;
        }
    }
    catch (Poco::Data::MySQL::ConnectionException& e)
    {
        cout << "Connection Exception: " << e.what() << endl;
        return -1;
    }
    catch(Poco::Data::MySQL::StatementException& e)
    {
        cout << "Statement Exception: " << e.what() << endl;
        return -1;
    }

    this_thread::sleep_for (chrono::seconds(1));
    return 0;
}
