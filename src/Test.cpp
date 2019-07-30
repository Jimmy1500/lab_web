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

    CROW_ROUTE(app, "/tenant").methods("GET"_method)
    ([](){
        Component::Repository db = Component::Repository();
        crow::json::wvalue x;
        try{
            std::vector<Tenant> tenants;
            db.popAll(tenants);
            size_t i = 0;
            for (Tenant const & tenant : tenants) {
                x[i]["tenant_id"] = tenant.id;
                x[i]["name"] = tenant.name;
                ++i;
            }
        } catch (Poco::Data::MySQL::ConnectionException& e) {
            cout << e.what() << endl;
        } catch (Poco::Data::MySQL::StatementException& e) {
            cout << e.what() << endl;
        }
        return crow::response{x};
    });

    CROW_ROUTE(app, "/tenant/<int>").methods("GET"_method)
    ([](const crow::request& req, int id){ // compile time input type check
        Component::Repository db = Component::Repository();
        crow::json::wvalue x;
        try{
            Tenant tenant;
            db.popById(id, tenant);
            x["tenant_id"] = tenant.id;
            x["name"] = tenant.name;
        } catch (Poco::Data::MySQL::ConnectionException& e) {
            cout << e.what() << endl;
        } catch (Poco::Data::MySQL::StatementException& e) {
            cout << e.what() << endl;
        }
        return crow::response{x};
    });

    CROW_ROUTE(app, "/city").methods("GET"_method)
    ([](){
        Component::Repository db = Component::Repository();
        crow::json::wvalue x;
        try{
            std::vector<City> cities;
            db.popAll(cities);
            size_t i = 0;
            for (City const & city : cities) {
                x[i]["city_id"] = city.id;
                x[i]["tenant_id"] = city.tenant_id;
                x[i]["city_name"] = city.name;
                x[i]["city_native_name"] = city.name_native;
                x[i]["latitude"] = city.name_native;
                x[i]["longitude"] = city.longitude;
                x[i]["county"] = city.county;
                x[i]["transportation_region"] = city.transportation_region;
                x[i]["position_region"] = city.position_region;
                x[i]["country_code"] = city.country_code;
                ++i;
            }
        } catch (Poco::Data::MySQL::ConnectionException& e) {
            cout << e.what() << endl;
        } catch (Poco::Data::MySQL::StatementException& e) {
            cout << e.what() << endl;
        }
        return crow::response{x};
    });

    CROW_ROUTE(app, "/city/<string>").methods("GET"_method)
    ([](const crow::request& req, string id){ // compile time input type check
        Component::Repository db = Component::Repository();
        crow::json::wvalue x;
        try{
            City city;
            db.popById(id, city);
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
