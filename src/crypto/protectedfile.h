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

#ifndef PROTECTEDFILE_H
#define PROTECTEDFILE_H

#include "mssf-qt_global.h"

#include <QtCore/QFile>
#include <QtCore/QSharedPointer>
#include <QtCore/QScopedPointer>

#include <sys/types.h>
#include <sys/stat.h>

class QByteArray;
class QDateTime;

namespace MssfQt
{

class MssfStorage;
class MssfStoragePrivate;
class ProtectedFilePrivate;

class MSSFQTSHARED_EXPORT ProtectedFile
{
    friend class MssfStoragePrivate;

public:

    /*!
      * \brief Destructor
      */
    ~ProtectedFile();

    /*!
      * \brief Open file
      * \param flags \sa QFile::Permissions
      * \returns true if the file could be opened/created
      *
      * Opening of a file may fail because of the protection was violated.
      * For instance the file contents didn't match its stored hash (it has been tampered) or the
      * caller doesn't have the required credentials to open the file for writing or to decrypt and
      * encrypted file. In all these cases the errno will be set to EACCES.
      */
    bool open(QFile::Permissions flags);

    /*!
      * \brief Read data from a file
      * \param at The offset from which to read
      * \param len The number of bytes to read
      * \returns The read data, it may be less that len if EOF was encountered.
      */
    QByteArray read(quint64 at, quintptr len);

    /*!
      * \brief Write data to a file
      * \param at The offset to which to write
      * \param data The data to write
      * \returns The number of bytes actually written. Can be smaller than the given len if the
      *  filesystem is full.
      */
    qptrdiff write(quint64 at, QByteArray &data);

    /*!
      * \brief Truncate the file
      * \param at The new size of the file
      * \returns true on success, false otherwise. Use errno to find out the actual error condition.
      */
    bool trunc(quint64 at);

    /*!
      * \brief Close the file
      *
      * The contents of the file are flushed to the disk.
      */
    void close();

    /*!
      * \brief Is the file currently open
      * \returns true if it is, false otherwise
      *
      * Attempts to read or write into a non-open file return and error and errno is set to EBADF.
      */
    bool isOpen();

    /*!
      * \brief Get file status
      * \param st (in,out) A pointer to the status buffer
      * \returns true on success, false otherwise. Errno is set.
      * \todo Can this be replaced with a QFileInfo instead of stat ?
      */
    bool status(struct stat *st);

    /*!
      * \brief Get the digest of the file
      * \returns A base64 encoded hash computed of the current contents of the file,
      * terminated with a NULL.
      */
    QByteArray digest();

    /*!
      * \brief The name shown to outside
      * \returns The public name, not necessarily the place where the actual contents are stored.
      */
    QString name();

    /*!
      * \brief Return a pointer to the owning pstore
      * \returns A reference to the pstore this file belongs to
      */
    QSharedPointer<MssfStorage> owner();

    /*!
      * \brief Rename the file
      * \param newName The new name
      * \returns true on success, false otherwise. Errno is set.
      */
    bool rename(QString newName);

    /*!
      * \brief Change mode flags
      * \param flags new value of mode flags
      * \return true on success, false otherwise.
      */
    bool chmod(QFile::Permissions flags);

    /*!
      * \brief Change ownership
      * \param uid new file owner uid
      * \param gid new group owner gid
      * \return true on success, false otherwise
      */
    bool chown(uid_t uid, gid_t gid);

    /*!
      * \brief Change access times
      * \param accessTime The last time the file was accessed
      * \param modifiedTime The last time the file was modified.
      * \return true on success, false otherwise
      */
    bool utime(const QDateTime &accessTime, const QDateTime &modifiedTime);

private:

    /*!
      * \brief Constructor
      * \param other The private implementation that is wrapped.
      */
    ProtectedFile(ProtectedFilePrivate *other);
    //! A pointer to the owner of this protected file.
    QScopedPointer<ProtectedFilePrivate> d_ptr;
};

} //MssfQt

#endif // PROTECTEDFILE_H
