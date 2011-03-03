/*
 * This file is part of MSSF
 *
 * Copyright (C) 2011 Brian McGillion
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
 * Author: Brian McGillion <brian.mcgillion@symbio.com>
 *
 * This is a wrapper library to provide a Qt API.  All rights for the wrapped
 * libraries remain with their original authors.
 */

#include "dbuscontextaccessmanager.h"
#include "credentialsmanager.h"

#include <QDBusMessage>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusContext>
#include <QtCore/QString>

using namespace MssfQt;

DBusContextAccessManager::DBusContextAccessManager()
{
}

DBusContextAccessManager::~DBusContextAccessManager()
{

}

bool DBusContextAccessManager::hasClientCredential(const QDBusContext &context, const QString &credential, const QString &access, QString *errorString)
{
    return CredentialsManager::hasProcessCredential(getClientPID(context), credential, access, errorString);
}

bool DBusContextAccessManager::hasClientCredential(const QDBusContext &context, const QString &credential, QString *errorString)
{
    return CredentialsManager::hasProcessCredential(getClientPID(context), credential, errorString);
}

quint32 *DBusContextAccessManager::getClientCredentials(const QDBusContext &context)
{
    QString serviceName = context.message().service();
    QDBusReply<quint32 *> reply;
    reply = context.connection().interface()->call(QLatin1String("GetConnectionUnixCredentials"), serviceName);
    if (!context.connection().interface()->lastError().isValid())
        return 0;

    return reply.value();
}

pid_t DBusContextAccessManager::getClientPID(const QDBusContext &context)
{
    QString serviceName = context.message().service();
    return context.connection().interface()->servicePid(serviceName).value();
}

uid_t DBusContextAccessManager::getClientUID(const QDBusContext &context)
{
    QString serviceName = context.message().service();
    return context.connection().interface()->serviceUid(serviceName).value();
}

Q_DECLARE_METATYPE(quint32 *)
