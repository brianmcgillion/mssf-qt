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

#include "credentialsutils.h"
#include "credentialsif.h"

#ifdef MAEMO
#include <sys/creds.h>
#else
#include <creds.h>
#endif

#include <errno.h>
#include <limits.h>

#include <QtCore/QList>
#include <QtCore/QVector>
#include <QtCore/QScopedPointer>
#include <QtDBus/QDBusPendingReply>
#include <QtCore/QDebug>

namespace MssfQt
{

CredentialsUtils::CredentialsUtils()
{
}

CredentialsUtils::~CredentialsUtils()
{
}

bool CredentialsUtils::setLastError(const QString &errorStr, QString *returnString)
{
    if (!returnString)
        return false;

    returnString->clear();
    returnString->append(errorStr);
    return false;
}

CredentialsUtils::Credential CredentialsUtils::stringToCreds(const QString &name, QString *errorString)
{
    Credential cred;
    creds_type_t type;
    creds_value_t value;

    type = creds_str2creds(name.toUtf8().constData(), &value);

    // if something is wrong - reset value ("value The credential value (NULL possible)")
    if (type == CREDS_BAD)
    {
        char strErrArray[256];
        setLastError(QString(QLatin1String("Invalid credential string (%1), errno (%2) : %3"))
                     .arg(name).arg(errno).arg(strerror_r(errno, strErrArray, sizeof(strErrArray))),
                     errorString);
    }
    //qDebug("Type %x value %ld", type, value);

    cred.first = type;
    cred.second = value;

    return cred;
}

QString CredentialsUtils::credsToString(const Credential &cred, QString *errorStr)
{
    Q_UNUSED(errorStr)

    char str[512];
    creds_creds2str(cred.first, cred.second, str, sizeof(str));

    return QString::fromAscii(str);
}

QList<CredentialsUtils::Credential> CredentialsUtils::getClientCredentialsList(const QDBusContext &context, QString *errorString)
{
    QString serviceName = context.message().service();

    QScopedPointer<Internal::CredentialsIf> credsIf(new Internal::CredentialsIf());
    QDBusPendingReply<QList<quint32> > reply;
    reply = credsIf->getConnectionCredentials(serviceName);
    reply.waitForFinished();
    if (!reply.isValid())
    {
        CredentialsUtils::setLastError(reply.error().message(), errorString);
        return QList<CredentialsUtils::Credential>();
    }

    QList<quint32> result = reply.value();
    QVector<quint32> resultVector = result.toVector();

    creds_t creds = creds_import(resultVector.constData(), resultVector.count());
    if (creds == 0)
    {
        CredentialsUtils::setLastError(QLatin1String("Unsuccesful credentials import"), errorString);
        return QList<CredentialsUtils::Credential>();
    }

    QList<CredentialsUtils::Credential> list;
    int i = 0;
    while (i >= 0)
    {
        creds_type_t type;
        creds_value_t value;
        type = creds_list(creds, i, &value);
        i++;
        if (type == CREDS_BAD)
            break;

        Credential add;
        add.first = type;
        add.second = value;
        list.append(add);

        //qDebug("cred %x type %ld value", add.first, add.second);
    }
    //qDebug("%d items", list.count());

    creds_free(creds);

    return list;
}

} // namespace MssfQt
