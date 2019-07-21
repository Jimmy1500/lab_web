#include "Connection.h"

using Component::Data::Connection;


Connection * Connection::pInstance = nullptr;

Connection::Connection() : credentials(host + user + password + db + port + compress + autoReconnect)
{
    Poco::Data::MySQL::Connector::registerConnector();
    cout << "MySQL connection established: " << this->credentials << endl;
}

Connection::~Connection()
{
    cout << "MySQL connection closed: " << this->credentials << endl;
}

Connection & Connection::getInstance() {
    if (!pInstance) {
        pInstance = new Connection();
    }
    return *pInstance;
}

void Connection::close() {
    if (pInstance) {
        delete pInstance;
    }
}

Session Connection::getSession() {
    return Poco::Data::SessionFactory::instance().create(Poco::Data::MySQL::Connector::KEY, credentials);
}
