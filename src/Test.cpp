#include <string>
#include <thread>
#include <chrono>
#include <iostream>

#include <Poco/Data/MySQL/MySQLException.h>

#include "Repository.h"

using namespace std;
using Component::Repository;

int main()
{
    try
    {
        Repository repo1 = Repository();
        Repository repo2 = Repository();

        Tenant tenant = { 5, "Germany" };
        repo1.insert(tenant);
        repo1.pop(tenant);
        repo1.popById(tenant);

        City city = { "a594e0a9-92f5-11e9-9d02-e0d55e84a5b0", 0, "city_name", "city_name_native", 0.0, 0.0, "cnty", "transportation_region", "position_region", "cntry" };
        // repo2.pop(city);
        repo2.popById(city);
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

    this_thread::sleep_for (chrono::milliseconds(250));
    return 0;
}
