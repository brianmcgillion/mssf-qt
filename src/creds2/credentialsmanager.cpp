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

#include "credentialsmanager.h"

#include <string.h>
#include <errno.h>

CredentialsManager::CredentialsManager()
{
}

CredentialsManager::~CredentialsManager()
{
}

bool CredentialsManager::hasProcessCredential(pid_t clientPID, const QString &credential, QString *errorString)
{
    creds_t creds;
    char strErrArray[256];

    if ((creds = creds_gettask(clientPID)) == NULL)
    {
        return CredentialsManager::setLastError(QString("Failed to find credentials for process (%1), errno (%2) : %3")
                                                .arg((int)clientPID).arg(errno).arg(
                                                    strerror_r(errno, strErrArray, sizeof(strErrArray))),
                                                errorString);
    }

    return CredentialsManager::hasCredential(creds, credential, errorString);
}

bool CredentialsManager::hasSocketCredential(int socketId, const QString &credential, QString *errorString)
{
    creds_t creds;
    char strErrArray[256];

    if ((creds = creds_getpeer(socketId)) == NULL)
    {
        return CredentialsManager::setLastError(QString("Failed to find credentials for socket (%1), errno (%2) : %3")
                                                .arg(socketId).arg(errno).arg(
                                                    strerror_r(errno, strErrArray, sizeof(strErrArray))),
                                                errorString);
    }

    return CredentialsManager::hasCredential(creds, credential, errorString);
}

bool CredentialsManager::hasCredential(creds_t creds, const QString &credential, QString *errorString)
{
    creds_type_t type;
    creds_value_t value;
    char strErrArray[256];

    if ((type = creds_str2creds(credential.toUtf8().data(), &value)) == CREDS_BAD)
    {
        creds_free(creds);
        return CredentialsManager::setLastError(QString("Invalid credential string (%1), errno (%2) : %3")
                                                .arg(credential).arg(errno).arg(
                                                    strerror_r(errno, strErrArray, sizeof(strErrArray))),
                                                errorString);
    }

    bool hasCredential = creds_have_p(creds, type, value);
    if (!hasCredential)
        CredentialsManager::setLastError("Entity does not possess required credential", errorString);

    creds_free(creds);

    return hasCredential;
}

bool CredentialsManager::setLastError(const QString &errorStr, QString *returnString)
{
    if (!returnString)
        return false;

    returnString->clear();
    returnString->append(errorStr);
    return false;
}
