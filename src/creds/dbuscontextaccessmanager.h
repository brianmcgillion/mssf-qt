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

#ifndef LIBCREDSQT_H
#define LIBCREDSQT_H

#include "mssf-qt_global.h"

#include <unistd.h>
#include <sys/types.h>

#include <QtCore/QList>

QT_FORWARD_DECLARE_CLASS(QDBusContext)
QT_FORWARD_DECLARE_CLASS(QString)
QT_FORWARD_DECLARE_CLASS(QStringList)

namespace MssfQt
{

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
      * \brief Determine if a dbus client has the required credentials and access rights.
      * \param context The local DBus context that is handling the client connection.
      * \param credential The credential that the client must possess.
      * \param access The access type requested to the object.
      * \param errorString An empty string that will be populated with the most recent error if this method returns false.
      * \returns true if the client has the required credential, false otherwise.
      * \sa QDBusContext
      *
      * Examples of access_type valid strings: "r", "w", "rw", "a", "ra", "wa", "x", "rwx", and etc.
      * If an empty string is supplied, then access is assumed to be "rw".
      */
    static bool hasClientCredential(const QDBusContext &context, const QString &credential, const QString &access, QString *errorString = NULL);

    /*!
      * \brief Determine if a dbus client has the required credentials.
      * \param context The local DBus context that is handling the client connection.
      * \param credential The credential that the client must possess.
      * \param errorString An empty string that will be populated with the most recent error if this method returns false.
      * \returns true if the client has the required credential, false otherwise.
      * \sa QDBusContext
      *
      * In Mssf V2 this method assumes checking 'rw' access with the token, in V1 possession of the token is sufficient to grant all access.
      */
    static bool hasClientCredential(const QDBusContext &context, const QString &credential, QString *errorString = NULL);

    /*!
      * \brief Determine the calling process's Credentials based on the DBus context.
      * \param context The QDBusContext that is handling the client request.
      * \returns The pointer to credentials of the client DBus connection or 0 if something wrong happened. The array is allocated by new
      *          and has to be deleted. UINT_MAX is the mark of the end in the list.
      * \warning The result must be deleted
      * \sa QDBusContext
      */
    static quint32 *getClientCredentials(const QDBusContext &context);

    /*!
      * \brief Determine the calling process's Credentials based on the DBus context.
      * \param context The QDBusContext that is handling the client request.
      * \param errorString An empty string that will be populated with the most recent error if needed
      * \returns The credentials list of the client DBus connection.
      * \sa QDBusContext
      */
    static QList<quint32> getClientCredentialsList(const QDBusContext &context, QString *errorString = 0);

    /*!
      * \brief Determine if a dbus client has the required credentials and access rights.
      * \param context The local DBus context that is handling the client connection.
      * \param credentialList The credential list that the client must possess.
      * \param access The access type requested to the object.
      * \param errorString An empty string that will be populated with the most recent error if this method returns false.
      * \returns true if the client has the required credential, false otherwise.
      * \sa QDBusContext
      *
      * Examples of access_type valid strings: "r", "w", "rw", "a", "ra", "wa", "x", "rwx", and etc.
      * If an empty string is supplied, then access is assumed to be "rw".
      */
    static bool hasClientCredentials(const QDBusContext &context, const QStringList &credentialList, const QString &access, QString *errorString = 0);

    /*!
      * \brief Determine if a dbus client has the required credentials.
      * \param context The local DBus context that is handling the client connection.
      * \param credentialList The credential list that the client must possess.
      * \param errorString An empty string that will be populated with the most recent error if this method returns false.
      * \returns true if the client has the required credential, false otherwise.
      * \sa QDBusContext
      *
      * In Mssf V2 this method assumes checking 'rw' access with the token, in V1 possession of the token is sufficient to grant all access.
      */
    static bool hasClientCredentials(const QDBusContext &context, const QStringList &credentialList, QString *errorString = 0);

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

} //namespace MssfQt

#endif // LIBCREDSQT_H
