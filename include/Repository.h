#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <Poco/Data/Time.h>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/MySQL/MySQLException.h>

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;
using namespace Poco::Data::Keywords;
using Poco::Data::Time;
using Poco::Data::Session;
using Poco::Data::SessionPool;
using Poco::Data::SessionFactory;

struct Tenant
{
    int         id;
    string      name;
};

struct City
{
    string      id;
    int         tenant_id;
    string      name;
    string      name_native;
    double      latitude;
    double      longitude;
    string      county;
    string      transportation_region;
    string      position_region;
    string      country_code;
};

struct CityPairDistance
{
    string      id;
    int         tenant_id;
    string      city_from_id;
    string      city_to_id;
    double      distance;
    Time        inserted_date;
    Time        updated_date;
};

namespace Component {
    enum Flag {
        DBSESSIONPOOL=1,
        NUM_FLAGS
    };

    class Repository {
        private:
            static unique_ptr<SessionPool> pool;
            static size_t mask;
        public:
            Repository();
            ~Repository();

            static inline void Init(string & dbType, string & connectionString, size_t minSessions, size_t maxSessions, size_t idleTime){
                if (!IsDirty(DBSESSIONPOOL)) {
                    pool = make_unique<Poco::Data::SessionPool>(dbType, connectionString, minSessions, maxSessions, idleTime);
                    cout << "Database session pool created!" << endl;
                    MarkDirty(DBSESSIONPOOL);
                }
            }

            static inline void Release() {
                if (IsDirty(DBSESSIONPOOL)) {
                    pool.reset();
                }
                cout << "Database session pool destroyed!" << endl;
            }

            static inline void MarkDirty(size_t flag){ mask |= flag; }
            static inline void MarkDirtyAll(){ size_t i; for (i=DBSESSIONPOOL; i<NUM_FLAGS; i<<=1u){ mask |= i; } }
            static inline void ClearDirty(size_t flag){ mask &= ~flag; }
            static inline void ClearDirtyAll(){ mask=0; }
            static inline size_t IsDirty(size_t flag){ return mask & flag; }

            void CreateTenant();
            void Insert(Tenant &);
            void PopById(int, Tenant &);
            void PopById(string &, City &);
            void PopById(string &, CityPairDistance &);

            void PopTenants(std::vector<int> &, std::vector<string> &);
            void PopAll(std::vector<Tenant> &);
            void PopAll(std::vector<City> &);
            void PopAll(std::vector<CityPairDistance> &);

    };
}
#endif
