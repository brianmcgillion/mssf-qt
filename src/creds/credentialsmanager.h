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

#ifndef CREDENTIALSMANAGER_H
#define CREDENTIALSMANAGER_H

#include "mssf-qt_global.h"

#include <unistd.h>
#include <sys/types.h>

class QString;

namespace MssfQt
{

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
      * \brief Determine if a process has the required credential and the correct access type.
      * \param clientPID The process id of the process, if 0 then the credentials of the current process will be checked.
      * \param credential The credential that the process must possess.
      * \param access The access type requested to the object.
      * \param errorString An empty string that will be populated with the most recent error if this method returns false.
      * \returns true if the client has the required credential, false otherwise.
      *
      * Examples of access_type valid strings: "r", "w", "rw", "a", "ra", "wa", "x", "rwx", and etc.
      * If an empty string is supplied, then access is assumed to be "rw".
      */
    static bool hasProcessCredential(pid_t clientPID, const QString &credential, const QString &access, QString *errorString = NULL);

    /*!
      * \brief Determine if a process has the required credentials, overloaded for convenience.
      * \param clientPID The process id of the process, if 0 then the credentials of the current process will be checked.
      * \param credential The credential that the process must possess.
      * \param errorString An empty string that will be populated with the most recent error if this method returns false.
      * \returns true if the client has the required credential, false otherwise.
      *
      * In Mssf V2 this method assumes checking 'rw' access with the token, in V1 possession of the token is sufficient to grant all access.
      */
    static bool hasProcessCredential(pid_t clientPID, const QString &credential, QString *errorString = NULL);

    /*!
      * \brief Determine if a socket has the required credentials.
      * \param socketId The id of the socket.
      * \param credential The credential that the socket must possess.
      * \param access The access type requested to the object.
      * \param errorString An empty string that will be populated with the most recent error if this method returns false.
      * \returns true if the client has the required credential, false otherwise.
      *
      * Examples of access_type valid strings: "r", "w", "rw", "a", "ra", "wa", "x", "rwx", and etc.
      * If an empty string is supplied, then access is assumed to be "rw".
      */
    static bool hasSocketCredential(int socketId, const QString &credential, const QString &access, QString *errorString = NULL);

    /*!
      * \brief Determine if a socket has the required credentials.
      * \param socketId The id of the socket.
      * \param credential The credential that the socket must possess.
      * \param errorString An empty string that will be populated with the most recent error if this method returns false.
      * \returns true if the client has the required credential, false otherwise.
      *
      * In Mssf V2 this method assumes checking 'rw' access with the token, in V1 possession of the token is sufficient to grant all access.
      */
    static bool hasSocketCredential(int socketId, const QString &credential, QString *errorString = NULL);

};

} //namespace MssfQt

#endif // CREDENTIALSMANAGER_H
