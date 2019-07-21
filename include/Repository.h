#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "Connection.h"

namespace Component {
    namespace Data {
        class Repository {
            private:
                Poco::Data::Session session;
            public:
                Repository();
                Repository(Poco::Data::Session &);
                ~Repository();

                void initTenant();

                void insert(Tenant & tenent);
                void pop(Tenant & tenent);
                void popById(Tenant & tenent);

                void insert(City & city);
                void pop(City & city);
                void popById(City & city);
        };
    }
}
#endif
