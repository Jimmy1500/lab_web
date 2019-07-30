#include "Repository.h"

SessionPool * Component::Repository::pool = nullptr;
size_t Component::Repository::connectorCount = 0;
size_t Component::Repository::maxPoolSize = 0;

Component::Repository::Repository(size_t minSessions, size_t maxSessions, size_t idleTime) :
    user(getenv("DB_USERNAME")),
    password(getenv("DB_PASSWORD")),
    db(getenv("DB_DATABASE")),
    connectionString(
            "host=" + host +
            ";user=" + user + ";password=" + password + ";db=" + db +
            ";port=" + port + ";compress=" + compress + ";auto-reconnect=" + autoReconnect + ";"
            )
{
    if (connectorCount && connectorCount < Component::Repository::maxPoolSize) {
        Poco::Data::MySQL::Connector::registerConnector();
        connectorCount++;
        cout << "Connector " << connectorCount << " -> Database (" << Poco::Data::MySQL::Connector::KEY << ") registed!" << endl;
    } else if (!pool) {
        Poco::Data::MySQL::Connector::registerConnector();
        pool = new SessionPool(Poco::Data::MySQL::Connector::KEY, connectionString, minSessions, maxSessions, idleTime);
        Component::Repository::maxPoolSize = maxSessions;
        connectorCount++;
        cout << "Connector " << connectorCount << " -> Database (" << Poco::Data::MySQL::Connector::KEY << ") created!" << endl;
    }
}

Component::Repository::~Repository() {
    if (connectorCount) {
        Poco::Data::MySQL::Connector::unregisterConnector();
        connectorCount--;
        if (!connectorCount && pool) {
            delete pool;
            pool = nullptr;
            Component::Repository::maxPoolSize = 0;
            cout << "Connector " << (connectorCount+1) << " -> Database (" << Poco::Data::MySQL::Connector::KEY << ") destroyed!" << endl;
        } else {
            cout << "Connector " << (connectorCount+1) << " -> Database (" << Poco::Data::MySQL::Connector::KEY << ") unregistered!" << endl;
        }
    }
}

void Component::Repository::initTenant() {
    Session session(this->pool->get());
    session << "DROP TABLE IF EXISTS tenant", now;
    session << "CREATE TABLE tenant (tenant_id int(30), tenant_name VARCHAR(45), inserted_date DATETIME, updated_data DATETIME)", now;
}

void Component::Repository::insert(Tenant & tenant) {
    Session session(this->pool->get());
    Poco::Data::Statement insert(session);
    insert << "INSERT INTO tenant (tenant_name) (SELECT ? WHERE NOT EXISTS (SELECT * FROM tenant WHERE tenant_name = ?))", use(tenant.name), use(tenant.name);
    insert.execute();
}

void Component::Repository::popById(int id, Tenant & tenant) {
    Session session(this->pool->get());
    Poco::Data::Statement select(session);
    select << "SELECT * FROM tenant WHERE tenant_id = ?", use(id), into(tenant.id), into(tenant.name), range(0, 1); //  iterate over result set one row at a time
    while (!select.done())
    {
        select.execute();
        cout << tenant.id << " " << tenant.name << endl;
    }
}

void Component::Repository::popById(string & id, City & city) {
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

void Component::Repository::popById(string & id, CityPairDistance & cityPairDistance) {
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

void Component::Repository::popTenants(std::vector<int> & ids, std::vector<string> & names) {
    Session session(this->pool->get());
    Poco::Data::Statement select(session);
    select << "SELECT * FROM tenant", into(ids), into(names), now;
    if (ids.size() != names.size()){
        throw Poco::Data::MySQL::StatementException("query returns results unmatched in size");
    }
}

void Component::Repository::popAll(std::vector<Tenant> & tenants) {
    Tenant tenant;
    Session session(this->pool->get());
    Poco::Data::Statement select(session);
    select << "SELECT * FROM tenant", into(tenant.id), into(tenant.name), range(0,1);
    while (!select.done()) {
        select.execute();
        tenants.push_back(tenant);
    }
}

void Component::Repository::popAll(std::vector<City> & cities) {
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

void Component::Repository::popAll(std::vector<CityPairDistance> & cityPairDistances) {
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
        cout << cityPairDistance.id << endl;
        cout << cityPairDistance.tenant_id << endl;
        cout << cityPairDistance.city_from_id << endl;
        cout << cityPairDistance.city_to_id << endl;
        cout << cityPairDistance.distance << endl;
    }
}
