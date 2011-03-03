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

#ifndef PROTECTEDFILE_P_H
#define PROTECTEDFILE_P_H

#include <QtCore/QFile>
#include <QtCore/QSharedPointer>

#include <sys/stat.h>
#include <unistd.h>

#ifdef MAEMO
namespace aegis {
class p_file;
}
#else
namespace mssf {
class p_file;
}
#endif

namespace MssfQt
{

class MssfStorage;
class MssfStoragePrivate;

class ProtectedFilePrivate
{
    friend class MssfStoragePrivate;

public:
    ~ProtectedFilePrivate();

    bool open(QFile::Permissions flags);

    QByteArray read(quint64 at, quintptr len);

    qptrdiff write(quint64 at, QByteArray &data);

    bool trunc(quint64 at);

    void close();

    bool isOpen();

    bool status(struct stat *st);

    QByteArray digest();

    QString name();

    QSharedPointer<MssfStorage> owner();

    bool rename(QString newName);

    bool chmod(QFile::Permissions flags);

    bool chown(uid_t uid, gid_t gid);

    bool utime(const QDateTime &accessTime, const QDateTime &modifiedTime);

private:

#ifdef MAEMO
    /*!
      * \brief Constructor private to allow only to reference a file within the storage area
      * \param file The file that is being accessed
      */
    ProtectedFilePrivate(aegis::p_file *file);
    //! The protected file that is wrapped
    aegis::p_file *file;
#else
    /*!
      * \brief Constructor private to allow only to reference a file within the storage area
      * \param file The file that is being accessed
      */
    ProtectedFilePrivate(mssf::p_file *file);
    //! The protected file that is wrapped
    mssf::p_file *file;
#endif
    //! A pointer to the owner of this protected file.
    QSharedPointer<MssfStorage> ownerPointer;
};

} //namespace MssfQt

#endif // PROTECTEDFILE_P_H
