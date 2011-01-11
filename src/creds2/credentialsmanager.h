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

#ifndef CREDENTIALSMANAGER_H
#define CREDENTIALSMANAGER_H

#include "mssf-qt_global.h"

#include <QtCore/QString>

#include <unistd.h>
#include <sys/types.h>
#include <sys/creds.h>

class MSSFQTSHARED_EXPORT CredentialsManager
{
public:

    /*!
      * \brief Constructor
      */
    CredentialsManager();

    /*!
      * \brief Destructor
      */
    virtual ~CredentialsManager();

    /*!
      * \brief Determine if a process has the required credentials.
      * \param clientPID The process id of the process, if 0 then the credentials of the current process will be checked.
      * \param credential The credential that the process must possess.
      * \param errorString An empty string that will be populated with the most recent error if this method returns false.
      * \returns true if the client has the required credential, false otherwise.
      */
    static bool hasProcessCredential(pid_t clientPID, const QString &credential, QString *errorString = NULL);

    /*!
      * \brief Determine if a socket has the required credentials.
      * \param socketId The id of the socket.
      * \param credential The credential that the socket must possess.
      * \param errorString An empty string that will be populated with the most recent error if this method returns false.
      * \returns true if the client has the required credential, false otherwise.
      */
    static bool hasSocketCredential(int socketId, const QString &credential, QString *errorString = NULL);

private:

    /*!
      * \brief Determine if a given credential list contains the required credential
      * \param creds The credentials structure to search, (this will be freed upon return)
      * \param credential The credential that the socket must possess.
      * \param errorString An empty string that will be populated with the most recent error if this method returns false.
      * \returns true if the creds has the required credential, false otherwise.
      */
    static bool hasCredential(creds_t creds, const QString &credential, QString *errorString);

    /*!
      * \brief Set the error string for the applciation.
      * \param errorStr The latest error string.
      * \param returnString The error string to be returned
      * \returns false This is a convenience so that we can just "return setLastError("Bla")"
      */
    static bool setLastError(const QString &errorStr, QString *returnString);
};

#endif // CREDENTIALSMANAGER_H
