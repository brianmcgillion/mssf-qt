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

#ifndef MSSFSTORAGE_P_H
#define MSSFSTORAGE_P_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef MAEMO
namespace aegis {
class storage;
}
#else
namespace mssf {
class storage;
}
#endif

class QString;
class QStringList;
class QByteArray;

namespace MssfQt
{

class ProtectedFile;
class MssfStorage;

class MssfStoragePrivate
{
    friend class ProtectedFilePrivate;

public:

    MssfStoragePrivate(const QString &name, const QString &owner, MssfStorage::Visibility vis, MssfStorage::Protection prot);

    ~MssfStoragePrivate();

    static QString storageRoot();

    QString name() const;

    QString filename() const;

    MssfStorage::Visibility visibility() const;

    MssfStorage::Protection protection() const;

    QString lastError();

    int numFiles() const;

    int numLinks() const;

    bool removeAllFiles();

    QStringList getFiles(const QString &mask = QString());

    QStringList getUFiles();

    bool containsFile(const QString &pathname);

    bool containsLink(const QString &pathname);

    void addLink(const QString &pathname, const QString &to);

    void addFile(const QString &pathname);

    bool putFile(const QString &pathname, const QByteArray &data);

    void removeFile(const QString &pathname);

    void removeLink(const QString &pathname);

    void rename(const QString &pathname, const QString &to);

    QString readLink(const QString &pathname);

    bool verifyFile(const QString &pathname);

    QByteArray getFile(const QString &pathname);

    bool verifyContent(const QString &pathname, const QByteArray &data);

    void commit();

    ProtectedFile* member(const QString &pathname);

    bool statFile(const QString &pathname, struct stat *stbuf);

    //    static int iterate_storage_names(storage::visibility_t of_visibility,
    //                             storage::protection_t of_protection,
    //                             const char* matching_names,
    //                             mssf_callback* cb_func,
    //                             void* ctx);

private:

    //static MssfStorage* createStorage()
    //! The storage class that is being wrapped.
#ifdef MAEMO
    MssfStoragePrivate(aegis::storage *store);
    aegis::storage *store;
#else
    MssfStoragePrivate(mssf::storage *store);
    mssf::storage *store;
#endif
};

} //namespace MssfQt

#endif // MSSFSTORAGE_P_H
