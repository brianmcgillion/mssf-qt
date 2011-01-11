/*
 * This file is part of MSSF
 *
 * Copyright (C) 2011 Nokia Corporation
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
 */

#include "dbuscontextaccessmanager.h"
#include "credentialsmanager.h"

#include <QDBusMessage>
#include <QDBusConnection>
#include <QDBusConnectionInterface>


DBusContextAccessManager::DBusContextAccessManager()
{
}

DBusContextAccessManager::~DBusContextAccessManager()
{

}

bool DBusContextAccessManager::hasClientCredential(const QDBusContext &context, const QString &credential, QString *errorString)
{
    return CredentialsManager::hasProcessCredential(getClientPID(context), credential, errorString);
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
