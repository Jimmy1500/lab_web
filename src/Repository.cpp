#include "Repository.h"

using Component::Data::Repository;

Repository::Repository() :
    session(Component::Data::Connection::getInstance()->getSession())
{
}

Repository::Repository(Poco::Data::Session & session) :
    session(session)
{
}

Repository::~Repository() {
    Component::Data::Connection::close();
}

void Repository::initTenant() {
    session << "DROP TABLE IF EXISTS tenant", now;
    session << "CREATE TABLE tenant (tenant_id int(30), tenant_name VARCHAR(45), inserted_date DATETIME, updated_data DATETIME)", now;
}

void Repository::insert(Tenant & tenant) {
    Poco::Data::Statement insert(session);
    insert << "INSERT INTO tenant (tenant_name) (SELECT ? WHERE NOT EXISTS (SELECT * FROM tenant WHERE tenant_name = ?))", use(tenant.name), use(tenant.name);
    insert.execute();
}

void Repository::pop(Tenant & tenant) {
    Poco::Data::Statement select(session);
    select << "SELECT * FROM tenant", into(tenant.id), into(tenant.name), range(0, 1); //  iterate over result set one row at a time
    while (!select.done())
    {
        select.execute();
        cout << tenant.id << " " << tenant.name << endl;
    }
}


void Repository::pop(City & city) {
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
