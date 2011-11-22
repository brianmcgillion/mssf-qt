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

#ifndef CREDENTIALSUTILS_H
#define CREDENTIALSUTILS_H

#include <QtCore/QString>
#include <QtCore/QPair>

QT_FORWARD_DECLARE_CLASS(QDBusContext)

namespace MssfQt
{

class CredentialsUtils
{
public:
    CredentialsUtils();
    virtual ~CredentialsUtils();

    typedef QPair<int, long> Credential;

    /*!
      * \brief Convert credential string \a name to a value
      * \param name Credential name
      * \param errorStr The latest error string.
      * \returns Credential value
      */
    static Credential stringToCreds(const QString &name, QString *errorStr = 0);

    /*!
      * \brief Convert credential to a string
      * \param cred Credential
      * \param errorStr The latest error string.
      * \returns Credential name
      */
    static QString credsToString(const Credential &cred, QString *errorStr = 0);

    /*!
      * \brief Set the error string for the applciation.
      * \param errorStr The latest error string.
      * \param returnString The error string to be returned
      * \returns false This is a convenience so that we can just "return setLastError("Bla")"
      */
    static bool setLastError(const QString &errorStr, QString *returnString);

    /*!
      * \brief Determine the calling process's Credentials based on the DBus context.
      * \param context The QDBusContext that is handling the client request.
      * \param errorString An empty string that will be populated with the most recent error if needed
      * \returns The credentials list of the client DBus connection.
      * \sa QDBusContext
      */
    static QList<Credential> getClientCredentialsList(const QDBusContext &context, QString *errorString = 0);

    /*!
      * \overload getClientCredentialsList(const QDBusContext &context, QString *errorString = 0)
      * \brief Determine the calling process's Credentials based on the DBus context.
      * \param message The DBus message that has been sent by client.
      * \param errorString An empty string that will be populated with the most recent error if needed
      * \returns The credentials list of the client DBus connection.
      * \sa QDBusMessage
      */
    static QList<Credential> getClientCredentialsList(const QDBusMessage &message, QString *errorString = 0);
};

} // namespace MssfQt

#endif // CREDENTIALSUTILS_H
