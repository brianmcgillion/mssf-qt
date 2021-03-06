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

#include "credentialsmanager.h"
#include "credentialsutils.h"

#include <string.h>
#include <errno.h>

#include <QtCore/QString>

#ifdef MAEMO
#include <sys/creds.h>
#else
#include <creds.h>
#endif

using namespace MssfQt;

static bool setLastError(const QString &errorStr, QString *returnString)
{
    return CredentialsUtils::setLastError(errorStr, returnString);
}

/*!
  * \brief Determine if a given credential list contains the required credential
  * \param creds The credentials structure to search, (this will be freed upon return)
  * \param credential The credential that the socket must possess.
  * \param access The access type requested to the object.
  * \param errorString An empty string that will be populated with the most recent error if this method returns false.
  * \returns true if the creds has the required credential, false otherwise.
  */
static bool hasCredential(creds_t creds, const QString &credential, const QString &access, QString *errorString)
{
    creds_type_t type;
    creds_value_t value;
    char strErrArray[256];

    if ((type = creds_str2creds(credential.toUtf8().constData(), &value)) == CREDS_BAD)
    {
        creds_free(creds);
        return setLastError(QString("Invalid credential string (%1), errno (%2) : %3")
                            .arg(credential).arg(errno).arg(
                                strerror_r(errno, strErrArray, sizeof(strErrArray))),
                            errorString);
    }

    bool hasCredential = creds_have_access(creds, type, value, access.toUtf8().constData());
    if (!hasCredential)
        setLastError("Entity does not possess required credential", errorString);

    creds_free(creds);

    return hasCredential;
}

CredentialsManager::CredentialsManager()
{
}

CredentialsManager::~CredentialsManager()
{
}

bool CredentialsManager::hasProcessCredential(pid_t clientPID, const QString &credential, const QString &access, QString *errorString)
{
    creds_t creds;
    char strErrArray[256];

    if ((creds = creds_gettask(clientPID)) == NULL)
    {
        return setLastError(QString("Failed to find credentials for process (%1), errno (%2) : %3")
                            .arg((int)clientPID).arg(errno).arg(
                                strerror_r(errno, strErrArray, sizeof(strErrArray))),
                            errorString);
    }

    return hasCredential(creds, credential, access, errorString);
}

bool CredentialsManager::hasProcessCredential(pid_t clientPID, const QString &credential, QString *errorString)
{
    return hasProcessCredential(clientPID, credential, QString(), errorString);
}

bool CredentialsManager::hasSocketCredential(int socketId, const QString &credential, const QString &access, QString *errorString)
{
    creds_t creds;
    char strErrArray[256];

    if ((creds = creds_getpeer(socketId)) == NULL)
    {
        return setLastError(QString("Failed to find credentials for socket (%1), errno (%2) : %3")
                            .arg(socketId).arg(errno).arg(
                                strerror_r(errno, strErrArray, sizeof(strErrArray))),
                            errorString);
    }

    return hasCredential(creds, credential, access, errorString);
}

bool CredentialsManager::hasSocketCredential(int socketId, const QString &credential, QString *errorString)
{
    return hasSocketCredential(socketId, credential, QString(), errorString);
}
