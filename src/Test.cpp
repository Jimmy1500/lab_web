#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include "crow_all.h"

#include <Poco/Data/MySQL/MySQLException.h>

#include "Repository.h"

using namespace std;

int main(int argc, char * argv[])
{
    crow::SimpleApp app;
    Component::Repository repo = Component::Repository();
    // app.loglevel(crow::LogLevel::Warning);

    CROW_ROUTE(app, "/")
    ([]() {
        return "Hello world!";
    });

    CROW_ROUTE(app, "/health")
    ([]{
        crow::json::wvalue x;
        x["message"] = "Hello, World!";
        return x;
    });

    CROW_ROUTE(app, "/tenant/<int>").methods("GET"_method)
    ([&repo](const crow::request& req, int id){ // compile time input type check
        crow::json::wvalue x;
        try{
            Tenant tenant = { id };
            repo.popById(tenant);
            x["tenant_id"] = tenant.id;
            x["name"] = tenant.name;
        } catch (Poco::Data::MySQL::ConnectionException& e) {
            cout << e.what() << endl;
        } catch (Poco::Data::MySQL::StatementException& e) {
            cout << e.what() << endl;
        }
        return crow::response{x};
    });

    CROW_ROUTE(app, "/city/<string>").methods("GET"_method)
    ([&repo](const crow::request& req, string uuid){ // compile time input type check
        crow::json::wvalue x;
        try{
            City city = { uuid };
            repo.popById(city);
            x["city_id"] = city.id;
            x["tenant_id"] = city.tenant_id;
            x["city_name"] = city.name;
            x["city_native_name"] = city.name_native;
            x["latitude"] = city.name_native;
            x["longitude"] = city.longitude;
            x["county"] = city.county;
            x["transportation_region"] = city.transportation_region;
            x["position_region"] = city.position_region;
            x["country_code"] = city.country_code;
        } catch (Poco::Data::MySQL::ConnectionException& e) {
            cout << e.what() << endl;
        } catch (Poco::Data::MySQL::StatementException& e) {
            cout << e.what() << endl;
        }
        return crow::response{x};
    });

    app.port(8080).multithreaded().run();

    return 0;
}
