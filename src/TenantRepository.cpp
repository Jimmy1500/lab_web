#include "TenantRepository.h"

using Component::Data::TenantRepository;

TenantRepository::TenantRepository() :
    Connection(),
    Session(this->GetSession())
{

}

// TenantRepository::TenantRepository(Session & session) : Session(session) { }
void TenantRepository::Insert(Tenant & tenant) {
    Poco::Data::Statement insert(this->Session);
    insert<< "INSERT INTO tenant (tenant_name) (SELECT ? WHERE NOT EXISTS (SELECT * FROM tenant WHERE tenant_name = ?))", use(tenant.name), use(tenant.name);
    insert.execute();
}

void TenantRepository::Pop(Tenant & tenant) {
    Poco::Data::Statement select(this->Session);
    select<< "SELECT * FROM tenant", into(tenant.id), into(tenant.name), range(0, 1); //  iterate over result set one row at a time
    while (!select.done())
    {
        select.execute();
        cout << tenant.id << " " << tenant.name << endl;
    }
}
