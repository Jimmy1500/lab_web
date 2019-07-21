#ifndef TENANTREPOSITORY_H
#define TENANTREPOSITORY_H

#include "Connection.h"

namespace Component {
    namespace Data {
        class TenantRepository : public Connection {
            private:
                Poco::Data::Session Session;
            public:
                TenantRepository();
                void Insert(Tenant & tenent);
                void Pop(Tenant & tenent);
        };
    }
}
#endif
