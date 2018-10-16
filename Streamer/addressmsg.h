#ifndef _ADDRESS_MSG_H
#define _ADDRESS_MSG_H

#include <QString>

struct AddressMsg
{
    QString ip;
    QString port;

    AddressMsg()
    {
    }

    AddressMsg(const QString& ip, const QString& port)
        : ip(ip)
        , port(port)
    {
    }
};

#endif // _ADDRESS_MSG_H
