#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <Poco/Data/MySQL/MySQLException.h>
#include "crow_all.h"
#include "Repository.h"

using namespace std;

int main(int argc, char * argv[])
{
    crow::SimpleApp app;
    // app.loglevel(crow::LogLevel::Warning);

    CROW_ROUTE(app, "/")
    ([](){
        return "Hello world!";
    });

    CROW_ROUTE(app, "/health")
    ([]{
        crow::json::wvalue x;
        x["message"] = "Hello, World!";
        return x;
    });

    CROW_ROUTE(app, "/tenant/<int>").methods("GET"_method)
    ([](const crow::request& req, int id){ // compile time input type check
        Component::Repository db = Component::Repository();
        crow::json::wvalue x;
        try{
            Tenant tenant = { id };
            db.popById(tenant);
            x["tenant_id"] = tenant.id;
            x["name"] = tenant.name;
        } catch (Poco::Data::MySQL::ConnectionException& e) {
            cout << e.what() << endl;
        } catch (Poco::Data::MySQL::StatementException& e) {
            cout << e.what() << endl;
        }
        // return x;
        return crow::response{x};
    });

    CROW_ROUTE(app, "/city/<string>").methods("GET"_method)
    ([](const crow::request& req, string uuid){ // compile time input type check
        Component::Repository db = Component::Repository();
        crow::json::wvalue x;
        try{
            City city = { uuid };
            db.popById(city);
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

    CROW_ROUTE(app, "/html").methods("GET"_method)
    ([](){
        return crow::response {
        "<h1>hello world<h1>            \
        <div>                           \
            <p>accessible document<p>   \
        <div>"
        };
    });
    app.port(8080).multithreaded().run();

    return 0;
}
