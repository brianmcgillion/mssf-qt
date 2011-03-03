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

#ifndef MSSFSTORAGE_H
#define MSSFSTORAGE_H

#include "mssf-qt_global.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <QtCore/QScopedPointer>
#include <QtCore/QString>

class QStringList;
class QByteArray;

namespace MssfQt
{

class ProtectedFile;
class MssfStoragePrivate;

class MSSFQTSHARED_EXPORT MssfStorage
{
    friend class ProtectedFilePrivate;

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

    /*!
      * \brief The storage directory
      *
      * Returns the path to the directory where all protected storage data is stored.
      */
    static QString storageRoot();

    /*!
      * \brief Logical name of the storage
      * \returns The name that was used when the store was opened.
      */
    QString name() const;

    /*!
      * \brief Location of the store
      * \returns The actual pathname of the store index file.
      *
      * The pathname of the index file is subject to changes and no assumptions should be made about it.
      */
    QString filename() const;

    /*!
      * \brief Return the visibility of the store
      * \returns The value given in the constructor
      */
    Visibility visibility() const;

    /*!
      * \brief Return the protection type of the store
      * \returns The value given in the constructor
      */
    Protection protection() const;

    /*!
      * \brief Return the last error that was seen.
      * This method should be called directly after an error condition before calling any other methods
      * to ensure an accurate response.
      */
    QString lastError();

    /*!
      * \brief How many files the storage contains
      * \returns The number of files and links in the store.
      */
    int numFiles() const;

    /*!
      * \brief How many links the storage contains
      * \return The number of links in the store.
      */
    int numLinks() const;

    /*!
      * \brief Erase the storage permanently. If the storage is encrypted, the member files will also
      * be erase. Use cautiously.
      * \return true if the command could be completed, false otherwise call \ref lastError() to see reason.
      */
    bool removeAllFiles();

    /*!
      * \brief Get a list of the files in the store
      * \param mask An optional mask by which to only return files matching the criteria.
      * \returns the list of files or QStringList() if none exist.
      */
    QStringList getFiles(const QString &mask = QString());

    /*!
      * \brief Get a list of the actual files that contain the content in the store, including the index file.
      * \returns the list of files or QStringList() if none exist.
      */
    QStringList getUFiles();

    /*!
      * \brief Check if the store contains the given file
      * \param pathname The name of the file to test
      * \returns true, if the file exists, false otherwise.
      */
    bool containsFile(const QString &pathname);

    /*!
      * \brief Check if the store contains the given link
      * \param pathname The name of a symbolic link
      * \returns true if the link exists in the store
      *
      * Links are an internal feature of a store and do not map into actual symbolic links in the
      * filesystem.  A link can be added into a store with \ref addLink, adding a file that is a
      * symbolic link will result in a regular file member.
      */
    bool containsLink(const QString &pathname);

    /*!
      * \brief Add a link to an existing file into the store
      * \param pathname The name of the link.
      * \param to The file where the link points to.
      *
      * The file identified by to must already be a member in the store, otherwise no link is added.
      */
    void addLink(const QString &pathname, const QString &to);

    /*!
      * \brief Add an existing file into the store.
      * \param pathname The name of the file. Relative pathnames are automatically converted to absolute.
      *
      * Existing files can only be added to signed stores.
      * New files are added to encrypted stores by \ref putFile
      */
    void addFile(const QString &pathname);

    /*!
      * \brief Write a file to the filesystem. Encrypt if needed.
      * \param pathname The name of the file to write. If the file does not yet exist in the store, it's added.
      * \param data The data to be written and optionally encrypted
      * \returns true on success, false otherwise.
      */
    bool putFile(const QString &pathname, const QByteArray &data);

    /*!
      * \brief Remove a file from the store
      * \param pathname The name of the file
      *
      * In a signed store this does not affect the actual file,
      * which will remain where it is. Only its hash is removed from the store.
      *
      * NOP if the file does not exist in the store.
      */
    void removeFile(const QString &pathname);

    /*!
      * \brief Remove a link from the store
      * \param pathname The name of the link
      *
      * NOP if the link doesn't exist in the store.
      */
    void removeLink(const QString &pathname);

    /*!
      * \brief Rename a file or link to something else
      * \param pathname The name of the file or link
      * \param to the new name
      *
      * NOP if the given file or link doesn't exist
      */
    void rename(const QString &pathname, const QString &to);

    /*!
      * \brief Get the name of the file a link points to.
      * \param pathname The name of the link.
      * \returns The full pathname of the file, if it exists, QString() otherwise.
      */
    QString readLink(const QString &pathname);

    /*!
      * \brief Check that a file matches the checksum stored \sa commit
      * \param pathname The name of the file
      * \returns true, if the checksum matches, false otherwise.
      *
      * Using this function will easily result in a race condition,
      * if the file is opened and read later. It is safer
      * to use \ref getFile or the \re#include "mssfstorage_p.h"f member function to
      * return a p_open instance which allows reading the
      * file contents without a race.
      */
    bool verifyFile(const QString &pathname);

    /*!
      * \brief Read an entire file into memory. Verification and decryption are performed automatically.
      * \param pathname The name of the file
      * \returns The buffer where the file contents are copied. Decryption is done
      * automatically if needed. The parameter needs not to have any value at entry.
      *
      * This function is safe to use but requires the while file to be in memory at once. Use the
      * \ref member function and \ref ProtectedFile::read to read data in smaller pieces if necessary.
      */
    QByteArray getFile(const QString &pathname);

    /*!
      * \brief Check if the content in a buffer matches the hash recorded for a file.
      * \param pathname The name of the file
      * \param data The data to verify. If the store is encrypted this should be the plaintext data.
      * \returns true, if the hashes match, false otherwise.
      */
    bool verifyContent(const QString &pathname, const QByteArray &data);

    /*!
      * \brief Seal a store
      *
      * Save a new copy of the store index file. Unless this call is made the changes will not be
      * recorded and the store may become unusable if its files have been changed.
     */
    void commit();

    /*!
      * \brief Create a protected handle to a file in the store
      * \param pathname The name of the file
      * \return A protected handle of type \ref ProtectedFile or NULL if some error occured.
      *
      * With the protected handle the file can be read from and written to in pieces like a
      * regular file, with no race conditions.
      *
      * If the file didn't exist, it is created automatically, so this function cannot be used to
      * test the existence of a file, use the \ref containsFile or \ref containsLink functions
      * for that in stead.
      *
      */
    ProtectedFile* member(const QString &pathname);

    /*!
      * \brief Get the status of a member file
      * \param pathname The name of the file
      * \param stbuf status buffer
      * \return true on success, false otherwise.
      * \todo should a converter be created for QFileInfo
      */
    bool statFile(const QString &pathname, struct stat *stbuf);

//    static int iterate_storage_names(storage::visibility_t of_visibility,
//                             storage::protection_t of_protection,
//                             const char* matching_names,
//                             mssf_callback* cb_func,
//                             void* ctx);

private:
    /*!
      * \brief Overlaoded Constructor
      */
    MssfStorage(MssfStoragePrivate *other);
    //! internal private implementation
    QScopedPointer<MssfStoragePrivate> d_ptr;
};

} //namespace MssfQt

#endif // MSSFSTORAGE_H
