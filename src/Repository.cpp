#include "Repository.h"

unique_ptr<SessionPool> Component::Repository::pool = nullptr;
size_t Component::Repository::mask = 0;

Component::Repository::Repository()
{
    Poco::Data::MySQL::Connector::registerConnector();
    cout << "Connector -> Database (" << Poco::Data::MySQL::Connector::KEY << ") registered!" << endl;
}

Component::Repository::~Repository() {
    Poco::Data::MySQL::Connector::unregisterConnector();
    cout << "Connector -> Database (" << Poco::Data::MySQL::Connector::KEY << ") unregistered!" << endl;
}

void Component::Repository::CreateTenant() {
    Session session(this->pool->get());
    session << "DROP TABLE IF EXISTS tenant", now;
    session << "CREATE TABLE tenant (tenant_id int(30), tenant_name VARCHAR(45), inserted_date DATETIME, updated_data DATETIME)", now;
}

void Component::Repository::Insert(Tenant & tenant) {
    Session session(this->pool->get());
    Poco::Data::Statement insert(session);
    insert << "INSERT INTO tenant (tenant_name) (SELECT ? WHERE NOT EXISTS (SELECT * FROM tenant WHERE tenant_name = ?))", use(tenant.name), use(tenant.name);
    insert.execute();
}

void Component::Repository::PopById(int id, Tenant & tenant) {
    Session session(this->pool->get());
    Poco::Data::Statement select(session);
    select << "SELECT * FROM tenant WHERE tenant_id = ?", use(id), into(tenant.id), into(tenant.name), range(0, 1); //  iterate over result set one row at a time
    while (!select.done())
    {
        select.execute();
        cout << tenant.id << " " << tenant.name << endl;
    }
}

void Component::Repository::PopById(string & id, City & city) {
    Session session(this->pool->get());
    Poco::Data::Statement select(session);
    select << "SELECT * FROM city WHERE city_id = ?",
           use(id),
           into(city.id),
           into(city.tenant_id),
           into(city.name),
           into(city.name_native),
           into(city.latitude),
           into(city.longitude),
           into(city.county),
           into(city.transportation_region),
           into(city.position_region),
           into(city.country_code), now;
}

void Component::Repository::PopById(string & id, CityPairDistance & cityPairDistance) {
    Session session(this->pool->get());
    Poco::Data::Statement select(session);
    select << "SELECT * FROM city_pair_distance WHERE city_pair_distance_id = ?",
           use(id),
           into(cityPairDistance.id),
           into(cityPairDistance.tenant_id),
           into(cityPairDistance.city_from_id),
           into(cityPairDistance.city_to_id),
           into(cityPairDistance.distance),
           into(cityPairDistance.inserted_date),
           into(cityPairDistance.updated_date), now;
}

void Component::Repository::PopTenants(std::vector<int> & ids, std::vector<string> & names) {
    Session session(this->pool->get());
    Poco::Data::Statement select(session);
    select << "SELECT * FROM tenant", into(ids), into(names), now;
    if (ids.size() != names.size()){
        throw Poco::Data::MySQL::StatementException("query returns results unmatched in size");
    }
}

void Component::Repository::PopAll(std::vector<Tenant> & tenants) {
    Tenant tenant;
    Session session(this->pool->get());
    Poco::Data::Statement select(session);
    select << "SELECT * FROM tenant", into(tenant.id), into(tenant.name), range(0,1);
    while (!select.done()) {
        select.execute();
        tenants.push_back(tenant);
    }
}

void Component::Repository::PopAll(std::vector<City> & cities) {
    City city;
    Session session(this->pool->get());
    Poco::Data::Statement select(session);
    select << "SELECT * FROM city",
           into(city.id),
           into(city.tenant_id),
           into(city.name),
           into(city.name_native),
           into(city.latitude),
           into(city.longitude),
           into(city.county),
           into(city.transportation_region),
           into(city.position_region),
           into(city.country_code), range(0, 1);

    while (!select.done())
    {
        select.execute();
        cities.push_back(city);
    }
}

void Component::Repository::PopAll(std::vector<CityPairDistance> & cityPairDistances) {
    CityPairDistance cityPairDistance;
    Session session(this->pool->get());
    Poco::Data::Statement select(session);
    select << "SELECT * FROM city_pair_distance",
           into(cityPairDistance.id),
           into(cityPairDistance.tenant_id),
           into(cityPairDistance.city_from_id),
           into(cityPairDistance.city_to_id),
           into(cityPairDistance.distance),
           into(cityPairDistance.inserted_date),
           into(cityPairDistance.updated_date), range(0, 1);

    while (!select.done())
    {
        select.execute();
        cityPairDistances.push_back(cityPairDistance);
    }
}
