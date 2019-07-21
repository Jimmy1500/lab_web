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
                void insert(Tenant & tenent);
                void insert(City & city);
                void pop(Tenant & tenent);
                void pop(City & city);
        };
    }
}
#endif
