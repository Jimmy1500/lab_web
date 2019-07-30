#include "Repository.h"

SessionPool * Component::Repository::pool = nullptr;
size_t Component::Repository::poolRefCount = 0;

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
    if (poolRefCount && poolRefCount < maxSessions) {
        Poco::Data::MySQL::Connector::registerConnector();
        poolRefCount++;
        cout << "Connector " << poolRefCount << " -> Database (" << Poco::Data::MySQL::Connector::KEY << ") registed!" << endl;
    } else if (!pool) {
        Poco::Data::MySQL::Connector::registerConnector();
        pool = new SessionPool(Poco::Data::MySQL::Connector::KEY, connectionString, minSessions, maxSessions, idleTime);
        poolRefCount++;
        cout << "Connector " << poolRefCount << " -> Database (" << Poco::Data::MySQL::Connector::KEY << ") created!" << endl;
    }
}

Component::Repository::~Repository() {
    if (poolRefCount) {
        Poco::Data::MySQL::Connector::unregisterConnector();
        poolRefCount--;
        if (!poolRefCount && pool) {
            delete pool;
            pool = nullptr;
            cout << "Connector " << (poolRefCount+1) << " -> Database (" << Poco::Data::MySQL::Connector::KEY << ") destoryed!" << endl;
        } else {
            cout << "Connector " << (poolRefCount+1) << " -> Database (" << Poco::Data::MySQL::Connector::KEY << ") unregistered!" << endl;
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

void Component::Repository::popTenants(std::vector<int> & ids, std::vector<string> & names) {
    Session session(this->pool->get());
    Poco::Data::Statement select(session);
    select << "SELECT * FROM tenant", into(ids), into(names), now; //  iterate over result set one row at a time
    if (ids.size() != names.size()){
        throw Poco::Data::MySQL::StatementException("query returns results unmatched in size");
    }
}

void Component::Repository::popById(Tenant & tenant) {
    Session session(this->pool->get());
    Poco::Data::Statement select(session);
    select << "SELECT * FROM tenant WHERE tenant_id = ?", use(tenant.id), into(tenant.id), into(tenant.name), range(0, 1); //  iterate over result set one row at a time
    while (!select.done())
    {
        select.execute();
        cout << tenant.id << " " << tenant.name << endl;
    }
}

void Component::Repository::scanCities() {
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
        cout << city.id << " "
                << city.name << " "
                << city.name_native << " "
                << city.latitude << " "
                << city.longitude << " "
                << city.county << " "
                << city.transportation_region << " "
                << city.position_region << " "
                << city.country_code << " "
                << endl;
    }
}

void Component::Repository::popById(City & city) {
    Session session(this->pool->get());
    Poco::Data::Statement select(session);
    select << "SELECT * FROM city WHERE city_id = ?",
           use(city.id),
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
        cout << city.id << " "
                << city.name << " "
                << city.name_native << " "
                << city.latitude << " "
                << city.longitude << " "
                << city.county << " "
                << city.transportation_region << " "
                << city.position_region << " "
                << city.country_code << " "
                << endl;
    }
}
