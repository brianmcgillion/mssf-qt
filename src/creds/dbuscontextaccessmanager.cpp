/*
 * This file is part of MSSF
 *
 * Copyright (C) 2011 Brian McGillion, Denis Mingulov
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
 * Authors: Brian McGillion <brian.mcgillion@symbio.com>,
 *          Denis Mingulov <denis.mingulov@symbio.com>
 *
 * This is a wrapper library to provide a Qt API.  All rights for the wrapped
 * libraries remain with their original authors.
 */

#include "dbuscontextaccessmanager.h"
#include "credentialsmanager.h"
#include "credentialsif.h"
#include "credentialsutils.h"

#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusContext>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QSet>
#include <QtCore/QScopedPointer>
#include <QtCore/QDebug>

using namespace MssfQt;

DBusContextAccessManager::DBusContextAccessManager()
{
}

DBusContextAccessManager::~DBusContextAccessManager()
{
}

bool DBusContextAccessManager::hasClientCredential(const QDBusContext &context, const QString &credential, const QString &access, QString *errorString)
{
    return hasClientCredentials(context, QStringList() << credential, access, errorString);
}

bool DBusContextAccessManager::hasClientCredential(const QDBusContext &context, const QString &credential, QString *errorString)
{
    return hasClientCredential(context, credential, QString(), errorString);
}

bool DBusContextAccessManager::hasClientCredentials(const QDBusContext &context, const QStringList &credentialList, const QString &access, QString *errorString)
{
    //! \todo implement support for \a access later
    Q_UNUSED(access);

    if (errorString)
        errorString->clear();

    QSet<CredentialsUtils::Credential> creds;

    foreach(const QString &str, credentialList)
    {
        CredentialsUtils::Credential id = CredentialsUtils::stringToCreds(str, errorString);
        if (errorString && !errorString->isEmpty())
            return false;

        creds.insert(id);
    }

    QSet<CredentialsUtils::Credential> list = CredentialsUtils::getClientCredentialsList(context, errorString).toSet();
    if (errorString && !errorString->isEmpty())
        return false;

    bool exist = list.contains(creds);
    qDebug("creds %d items, list %d items, exits %d", creds.count(), list.count(), (int)exist);

    peerCredentials(context);

    return exist;
}

bool DBusContextAccessManager::hasClientCredentials(const QDBusContext &context, const QStringList &credentialList, QString *errorString)
{
    return hasClientCredentials(context, credentialList, QString(), errorString);
}

quint32 *DBusContextAccessManager::getClientCredentials(const QDBusContext &context)
{
    QString serviceName = context.message().service();

    QScopedPointer<Internal::CredentialsIf> credsIf(new Internal::CredentialsIf());
    QDBusPendingReply<QList<quint32> > reply;
    reply = credsIf->getConnectionCredentials(serviceName);
    reply.waitForFinished();
    if (!reply.isValid())
        return 0;

    QList<quint32> list = reply.value();
    // add separator
    list.append(UINT_MAX);

    quint32 *result = new quint32[list.count()];
    for(int i = 0; i < list.count(); i++)
        result[i] = list[i];

    return result;
}

QStringList DBusContextAccessManager::peerCredentials(const QDBusContext &context, QString *errorString)
{
    if (errorString)
        errorString->clear();

    QList<CredentialsUtils::Credential> list = CredentialsUtils::getClientCredentialsList(context, errorString);

    // if error - do nothing, just return
    if (errorString && !errorString->isEmpty())
        return QStringList();

    QStringList result;

    foreach(const CredentialsUtils::Credential &cred, list)
    {
        QString str = CredentialsUtils::credsToString(cred, errorString);
        if (errorString && !errorString->isEmpty())
            continue;

        result.append(str);
    }

    qDebug() << result;

    return result;
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
