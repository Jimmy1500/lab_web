#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include <Poco/Data/MySQL/MySQLException.h>
#include "crow_all.h"
#include "Repository.h"

using namespace std;
using namespace rapidjson;

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
        StringBuffer s;
        Writer<StringBuffer> writer(s);
        writer.StartObject();
        writer.Key("hello");
        writer.String("world");
        writer.Key("t");
        writer.Bool(true);
        writer.Key("f");
        writer.Bool(false);
        writer.Key("n");
        writer.Null();
        writer.Key("i");
        writer.Uint(123);
        writer.Key("pi");
        writer.Double(3.1416);
        writer.Key("a");
        writer.StartArray();
        for (unsigned i = 0; i < 4; i++)
            writer.Uint(i);
        writer.EndArray();
        writer.EndObject();
        cout << s.GetString() <<endl;
        crow::json::wvalue x(crow::json::load(s.GetString(), strlen(s.GetString())));

        return crow::response(200, x);
    });

    CROW_ROUTE(app, "/tenant").methods("GET"_method)
    ([](){
        crow::json::wvalue x;
        try{
            std::vector<Tenant> tenants;
            Component::Repository db = Component::Repository();
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
        crow::json::wvalue x;
        try{
            Tenant tenant;
            Component::Repository db = Component::Repository();
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
        crow::json::wvalue x;
        try{
            std::vector<City> cities;
            Component::Repository db = Component::Repository();
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
        crow::json::wvalue x;
        try{
            City city;
            Component::Repository db = Component::Repository();
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

    CROW_ROUTE(app, "/city_pair_distance").methods("GET"_method)
    ([](){
        StringBuffer s;
        try{
            std::vector<CityPairDistance> cityPairDistances;
            Component::Repository db = Component::Repository();
            db.popAll(cityPairDistances);

            size_t i = 0;
            // PrettyWriter<StringBuffer> writer(s);
            Writer<StringBuffer> writer(s);
            writer.StartObject();
            writer.Key("success");
            writer.StartArray();
            for (CityPairDistance const & cityPairDistance : cityPairDistances) {
                writer.StartObject();
                writer.Key("id");
                writer.String(cityPairDistance.id.c_str());
                writer.Key("tenant_id");
                writer.Uint(cityPairDistance.tenant_id);
                writer.Key("city_from_id");
                writer.String(cityPairDistance.city_from_id.c_str());
                writer.Key("city_to_id");
                writer.String(cityPairDistance.city_to_id.c_str());
                writer.Key("distance");
                writer.Double(cityPairDistance.distance);

                int hh = cityPairDistance.inserted_date.hour();
                int mm = cityPairDistance.inserted_date.minute();
                int ss = cityPairDistance.inserted_date.second();
                string hhMMss = to_string(hh) + "::" + to_string(mm) + "::" + to_string(ss);
                writer.Key("inserted time");
                writer.String(hhMMss.c_str());

                hh = cityPairDistance.updated_date.hour();
                mm = cityPairDistance.updated_date.minute();
                ss = cityPairDistance.updated_date.second();
                hhMMss = to_string(hh) + "::" + to_string(mm) + "::" + to_string(ss);
                writer.Key("updated time");
                writer.String(hhMMss.c_str());
                writer.EndObject();
            }
            writer.EndArray();
            writer.EndObject();
        } catch (Poco::Data::MySQL::ConnectionException& e) {
            cout << e.what() << endl;
        } catch (Poco::Data::MySQL::StatementException& e) {
            cout << e.what() << endl;
        }
        // crow::json::wvalue x();
        // return crow::response{crow::json::load(s.GetString(), strlen(s.GetString()))};
        return crow::response{s.GetString()};
    });

    CROW_ROUTE(app, "/city_pair_distance/<string>").methods("GET"_method)
    ([](const crow::request& req, string id){
        Component::Repository db = Component::Repository();
        crow::json::wvalue x;
        try{
            CityPairDistance cityPairDistance;
            db.popById(id, cityPairDistance);
            x["city_pair_distance_id"] = cityPairDistance.id;
            x["tenant_id"] = cityPairDistance.tenant_id;
            x["city_from_id"] = cityPairDistance.city_from_id;
            x["city_to_id"] = cityPairDistance.city_to_id;
            x["distance"] = cityPairDistance.distance;

            int hour = cityPairDistance.inserted_date.hour();
            int minute = cityPairDistance.inserted_date.minute();
            int second = cityPairDistance.inserted_date.second();
            x["inserted_date"] = std::to_string(hour) + ":" + std::to_string(minute) + ":" + std::to_string(second);
            hour = cityPairDistance.updated_date.hour();
            minute = cityPairDistance.updated_date.minute();
            second = cityPairDistance.updated_date.second();
            x["updated_date"] = std::to_string(hour) + ":" + std::to_string(minute) + ":" + std::to_string(second);
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
