#include <string>
#include <thread>
#include <chrono>
#include <iostream>

#include <Poco/Data/MySQL/MySQLException.h>

#include "Repository.h"

using namespace std;

int main(int argc, char * argv[])
{
    try
    {
        Component::Repository repo1 = Component::Repository();
        Component::Repository repo2 = Component::Repository();

        Tenant tenant = { 5, "Germany" };
        std::vector<Tenant> tenants;
        City city = { "a594e0a9-92f5-11e9-9d02-e0d55e84a5b0", 0, "", "", 0.0, 0.0, "", "", "", "" };

        repo1.insert(tenant);
        repo1.popAll(tenants);
        repo1.popById(tenant);

        repo2.scanCities();
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

    // this_thread::sleep_for (chrono::milliseconds(250));
    return 0;
}
