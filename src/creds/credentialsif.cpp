/*
 * This file is part of MSSF
 *
 * Copyright (C) 2011 Denis Mingulov
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 * Author: Denis Mingulov <denis.mingulov@symbio.com>
 *
 * This is a wrapper library to provide a Qt API.  All rights for the wrapped
 * libraries remain with their original authors.
 */

#include "credentialsif.h"

namespace MssfQt
{

namespace Internal
{

namespace Constants
{
    //! DBus service
    static const QLatin1String DBusService("org.freedesktop.DBus");
    //! DBus path
    static const QLatin1String DBusPath("/org/freedesktop/DBus");
    //! DBus interface
    const char *const DBusInterface = "com.meego.DBus.Creds";
}

CredentialsIf::CredentialsIf(QObject *parent)
    : QDBusAbstractInterface(Constants::DBusService, Constants::DBusPath, Constants::DBusInterface,
          QDBusConnection::systemBus(), parent)
{
    // register uint list for DBus
    qDBusRegisterMetaType<QList<quint32> >();
}

CredentialsIf::~CredentialsIf()
{
}

QDBusPendingReply<QList<quint32> > CredentialsIf::getConnectionCredentials(const QString &service)
{
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(service);
    return asyncCallWithArgumentList(QLatin1String("GetConnectionCredentials"), argumentList);
}

} // namespace Internal

} // namespace MssfQt
