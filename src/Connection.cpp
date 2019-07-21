#include "Connection.h"

using Component::Data::Connection;

Connection::Connection() :
    ConnStr(Host + User + Password + Db + Port + Compress + AutoReconnect)
{
    Poco::Data::MySQL::Connector::registerConnector();
    cout << "Mysql database connector registered: " << this->ConnStr << endl;
}

Session Connection::GetSession() {
    return Poco::Data::SessionFactory::instance().create(Poco::Data::MySQL::Connector::KEY, ConnStr);
}
