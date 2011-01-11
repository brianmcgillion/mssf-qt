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

#ifndef LIBCREDSQT_H
#define LIBCREDSQT_H

#include "mssf-qt_global.h"

#include <QDBusContext>
#include <QtCore/QString>

#include <unistd.h>
#include <sys/types.h>

class MSSFQTSHARED_EXPORT DBusContextAccessManager
{

public:

    /*!
      * \brief Constructor
      */
    DBusContextAccessManager();

    /*!
      * \brief Destructor
      */
    virtual ~DBusContextAccessManager();

    /*!
      * \brief Determine if a dbus client has the required credentials.
      * \param context The local DBus context that is handling the client connection.
      * \param credential The credential that the client must possess.
      * \param errorString An empty string that will be populated with the most recent error if this method returns false.
      * \returns true if the client has the required credential, false otherwise.
      * \sa QDBusContext
      */
    static bool hasClientCredential(const QDBusContext &context, const QString &credential, QString *errorString = NULL);

    /*!
      * \brief Determine the calling process's PID based on the DBus context.
      * \param context The QDBusContext that is handling the client request.
      * \returns The process id of the client DBus connection.
      * \sa QDBusContext
      */
    static pid_t getClientPID(const QDBusContext &context);

    /*!
      * \brief Determine the user ID of the calling (client) process.
      * \param context The QDBusContext that is handling the client request.
      * \returns The user id of the client DBus connection.
      * \sa QDBusContext
      */
    static uid_t getClientUID(const QDBusContext &context);
};

#endif // LIBCREDSQT_H
