#include "Repository.h"

SessionPool * Component::Repository::pool = nullptr;
size_t Component::Repository::poolRefCount = 0;

Component::Repository::Repository(size_t minSessions, size_t maxSessions, size_t idleTime) :
    connectionString(host + user + password + db + port + compress + autoReconnect)
{
    Poco::Data::MySQL::Connector::registerConnector(); 
    if (poolRefCount) {
        poolRefCount++;
        cout << "Connection " << poolRefCount << " -> " << Poco::Data::MySQL::Connector::KEY << " registed with: " << connectionString << endl;
    } else if (!pool) {
        pool = new SessionPool(Poco::Data::MySQL::Connector::KEY, connectionString, minSessions, maxSessions, idleTime);
        poolRefCount++;
        cout << "Connection " << poolRefCount << " -> " << Poco::Data::MySQL::Connector::KEY << " established with: " << connectionString << endl;
    }
}

Component::Repository::~Repository() {
    Poco::Data::MySQL::Connector::unregisterConnector(); 
    if (poolRefCount) {
        poolRefCount--;
        if (!poolRefCount && pool) {
            delete pool;
            pool = nullptr;
            cout << "Connection " << (poolRefCount+1) << " -> " << Poco::Data::MySQL::Connector::KEY << " disestablished with: " << connectionString << endl;
        } else {
            cout << "Connection " << (poolRefCount+1) << " -> " << Poco::Data::MySQL::Connector::KEY << " unregistered with: " << connectionString << endl;
        }
    }
}

Session Component::Repository::getSession() {
    // return Poco::Data::SessionFactory::instance().create(Poco::Data::MySQL::Connector::KEY, connectionString);
    // return Session(Poco::Data::MySQL::Connector::KEY, connectionString);
    // return Session("MySQL", connectionString);
    return this->pool->get(); // Not thread safe, probably need to mutex lock it
}

void Component::Repository::initTenant() {
    Session session(getSession());
    session << "DROP TABLE IF EXISTS tenant", now;
    session << "CREATE TABLE tenant (tenant_id int(30), tenant_name VARCHAR(45), inserted_date DATETIME, updated_data DATETIME)", now;
}

void Component::Repository::insert(Tenant & tenant) {
    Session session(getSession());
    Poco::Data::Statement insert(session);
    insert << "INSERT INTO tenant (tenant_name) (SELECT ? WHERE NOT EXISTS (SELECT * FROM tenant WHERE tenant_name = ?))", use(tenant.name), use(tenant.name);
    insert.execute();
}

void Component::Repository::popAll(std::vector<Tenant> & tenants) {
    std::vector<size_t> ids;
    std::vector<string> names;

    Session session(getSession());
    Poco::Data::Statement select(session);
    select << "SELECT * FROM tenant", into(ids), into(names), now; //  iterate over result set one row at a time
    if (ids.size() != names.size()){
        throw Poco::Data::MySQL::StatementException("query returns results unmatched in size");
    }
    size_t i;
    for ( i = 0; i < ids.size(); ++i ) {
        tenants.push_back( { ids.at(i), names.at(i) } );
        cout << ids.at(i) << " " << names.at(i) << endl;
    }
}

void Component::Repository::popById(Tenant & tenant) {
    Session session(this->getSession());
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
    Session session(getSession());
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
    Session session(getSession());
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
