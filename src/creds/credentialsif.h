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

#ifndef CREDENTIALSIF_H
#define CREDENTIALSIF_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtDBus/QtDBus>

namespace MssfQt
{

namespace Internal
{

/*!
  * \file credentialsif.h
  * \class CredentialsIf
  * \brief Implementation of a mechanism to allow get credentials over DBus.
  */

class CredentialsIf : public QDBusAbstractInterface
{
    Q_OBJECT

public:

    /*!
      * \brief Constructor
      * \param parent The parent object of this.
      */
    CredentialsIf(QObject *parent = 0);

    /*!
      * \brief Destructor
      */
    ~CredentialsIf();

public Q_SLOTS: // METHODS

    /*!
      * \brief Get DBus connection credentials
      * \param service Service name to be checked
      * \returns List of credentials
      */
    QDBusPendingReply<QList<quint32> > getConnectionCredentials(const QString &service);

};

} // namespace Internal

} // namespace MssfQt

Q_DECLARE_METATYPE(QList<quint32>)

#endif // CREDENTIALSIF_H
