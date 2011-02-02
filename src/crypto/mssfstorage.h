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

#ifndef MSSFSTORAGE_H
#define MSSFSTORAGE_H

#include "mssf-qt_global.h"

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QByteArray>

namespace mssf {
    class storage;
}

class MSSFQTSHARED_EXPORT MssfStorage
{
public:

    /*!
      * \enum Visibility
      * \brief The visibility of a store. Defined when a storage is created and cannot be
      * changed afterwards.
      */
    enum Visibility {
        global_vis,     /*!< global_vis  - Store is globally signed and cannot be modified locally. */
        shared_vis,     /*!< shared_vis  - The caller must have a resource token to modify a signed store or read an encrypted store */
        private_vis     /*!< private_vis - The store is owned by a single application and can only be modified by it or read by it. */
    };

    /*!
      * \enum Protection
      * \brief The protection method of a storage. Defined when a storage is created and
      * cannot be changed afterwards.
      */
    enum Protection {
        Signed,         /*!< Signed    - Integrity is protected by signatures. */
        Encrypted       /*!< Encrypted - Confidentiality is protected by encryption */
    };

    /*!
      * \brief Create a storage object
      * \param name The name of the storage area.
      * \param owner The name of the token that is required to access the store.
      * \param vis The visibility of the storage area. \sa MssfStorage::Visibility
      * \param prot The Protection that is being used for the storage area. \sa MssfStorage::Protection
      */
    MssfStorage(const QString &name, const QString &owner, MssfStorage::Visibility vis, MssfStorage::Protection prot);

    /*!
      * \brief Destructor
      */
    ~MssfStorage();

    QString name() const;
    QString filename() const;

    Visibility visibility() const;
    Protection protection() const;

    int numFiles() const;
    int numLinks() const;

    bool removeAllFiles();

    quintptr getFiles(QStringList &names);
    quintptr getFiles(QStringList &names);
    bool containsFile(const QString &pathname);

    bool containsLink(const QString &pathname);
    void addFile(const QString &pathname);
    void removeFile(const QString &pathname);
    void addLink(const QString &pathname, const QString &to_file);

    void removeLink(const QString &pathname);
    void rename(const QString &pathname, const QString &to_this);
    QString readLink(const QString &pathname);
    bool verifyFile(const QString &pathname);
    bool verifyContent(const QString &pathname, const QByteArray &data);
    QByteArray getFile(const QString &pathname);
    bool putFile(const QString &pathname, const QByteArray &data);
    void commit();


    static int iterate_storage_names(storage::visibility_t of_visibility,
                             storage::protection_t of_protection,
                             const char* matching_names,
                             mssf_callback* cb_func,
                             void* ctx);



    //TODO 1) is this required
    //TODO 2) if required then stat has to be manually parsed
    //int stat_file(const char* pathname, struct stat *stbuf);

private:
    //! The storage class that is being wrapped.
    mssf::storage *store;
};

#endif // MSSFSTORAGE_H
