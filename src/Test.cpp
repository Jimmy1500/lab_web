#include <string>
#include <thread>
#include <chrono>
#include <iostream>

#include <Poco/Data/MySQL/MySQLException.h>

#include "Connection.h"
#include "Repository.h"

using namespace std;
using Component::Data::Connection;
using Component::Data::Repository;

int main()
{
    try
    {
        Repository db = Repository();

        Tenant tenant = { 0, "Germany" };
        db.insert(tenant);
        db.pop(tenant);

        City city = { "uuid", 0, "city_name", "city_name_native", 0.0, 0.0, "cnty", "transportation_region", "position_region", "cntry" };
        db.pop(city);
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

    this_thread::sleep_for (chrono::milliseconds(500));
    return 0;
}
