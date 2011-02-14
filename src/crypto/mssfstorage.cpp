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

#include "mssfstorage.h"
#include "mssfstorage_p.h"
#include "protectedfile.h"
#include "protectedfile_p.h"

#include <QtCore/QVector>
#include <QtCore/QRegExp>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QByteArray>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#ifdef MAEMO
//use the V1 libraries for maemo
#include <aegis_storage.h>
#include <aegis_crypto.h>
using namespace aegis;
#else
//use the V2 libraries for MeeGo
#include <mssf_storage.h>
#include <mssf_crypto.h>
using namespace mssf;
#endif

using namespace MssfQt;

//Convert to wrapped types
static storage::visibility_t visConverter(MssfStorage::Visibility vis)
{
    if (vis == MssfStorage::global_vis)
        return storage::vis_global;
    else if (vis == MssfStorage::private_vis)
        return storage::vis_private;
    else
        return storage::vis_shared;
}

static storage::protection_t protConverter(MssfStorage::Protection prot)
{
    if (prot == MssfStorage::Encrypted)
        return storage::prot_encrypted;
    else
        return storage::prot_signed;
}

/*!
  * \brief Convert a Vector to a string list.
  * \param list The list that is to be converted
  * \param mask A mask to filter the list with, if mask = QString() then all files found will be returned.
  * \returns The list of file names.
  */
QStringList vectorToStringList(const storage::stringlist &list, const QString &mask)
{
    QList<const char *> fullList = QList<const char *>::fromVector(QVector<const char *>::fromStdVector(list));

    QStringList resultList;
    QRegExp rx(mask);
    rx.setPatternSyntax(QRegExp::Wildcard);

    foreach(const char *stdString, fullList)
    {
        QString file = QString::fromUtf8(stdString);
        if (mask.isEmpty() || rx.exactMatch(file))
            resultList.append(file);
    }

    return resultList;
}

MssfStorage::MssfStorage(const QString &name, const QString &owner, MssfStorage::Visibility vis, MssfStorage::Protection prot)
    : d_ptr(new MssfStoragePrivate(name, owner, vis, prot))
{
}

MssfStorage::MssfStorage(MssfStoragePrivate *other)
    : d_ptr(other)
{
}

MssfStoragePrivate::MssfStoragePrivate(const QString &name, const QString &owner, MssfStorage::Visibility vis, MssfStorage::Protection prot)
    : store(new storage(name.toUtf8().constData(), owner.toUtf8().constData(), visConverter(vis), protConverter(prot)))
{
}

MssfStoragePrivate::MssfStoragePrivate(storage *store)
    : store(store)
{
}

MssfStorage::~MssfStorage()
{
}

MssfStoragePrivate::~MssfStoragePrivate()
{
    delete store; store = NULL;
}

QString MssfStorage::storageRoot()
{
    return MssfStoragePrivate::storageRoot();
}

QString MssfStoragePrivate::storageRoot()
{
    return QLatin1String(storage_root());
}

QString MssfStorage::name() const
{
    return d_ptr->name();
}

QString MssfStoragePrivate::name() const
{
    return QLatin1String(store->name());
}

QString MssfStorage::filename() const
{
    return d_ptr->filename();
}

QString MssfStoragePrivate::filename() const
{
    return QLatin1String(store->filename());
}

QString MssfStorage::lastError()
{
    return d_ptr->lastError();
}

QString MssfStoragePrivate::lastError()
{
    char err[256];
    strerror_r(errno, err, 256);
    return QLatin1String(err);
}

MssfStorage::Visibility MssfStorage::visibility() const
{
   return d_ptr->visibility();
}

MssfStorage::Visibility MssfStoragePrivate::visibility() const
{
    storage::visibility_t vis = store->visibility();
    if (vis == storage::vis_global)
        return MssfStorage::global_vis;
    else if (vis == storage::vis_private)
        return MssfStorage::private_vis;
    else
        return MssfStorage::shared_vis;
}

MssfStorage::Protection MssfStorage::protection() const
{
    return d_ptr->protection();
}

MssfStorage::Protection MssfStoragePrivate::protection() const
{
    return (store->protection() == storage::prot_encrypted ? MssfStorage::Encrypted : MssfStorage::Signed);
}

int MssfStorage::numFiles() const
{
    return d_ptr->numFiles();
}

int MssfStoragePrivate::numFiles() const
{
    return store->nbrof_files();
}

int MssfStorage::numLinks() const
{
    return d_ptr->numLinks();
}

int MssfStoragePrivate::numLinks() const
{
    return store->nbrof_links();
}

bool MssfStorage::removeAllFiles()
{
    return d_ptr->removeAllFiles();
}

bool MssfStoragePrivate::removeAllFiles()
{
    return store->remove_all_files();
}

QStringList MssfStorage::getFiles(const QString &mask)
{
    return d_ptr->getFiles(mask);
}

QStringList MssfStoragePrivate::getFiles(const QString &mask)
{
    storage::stringlist list;
    size_t total = store->get_files(list);
    if (total <= 0)
        return QStringList(); // no files in store.

    QStringList returnList = vectorToStringList(list, mask);
    // free the memory for the tmp list
    store->release(list);
    return returnList;
}

QStringList MssfStorage::getUFiles()
{
    return d_ptr->getUFiles();
}

QStringList MssfStoragePrivate::getUFiles()
{
    storage::stringlist list;
    size_t total = store->get_ufiles(list);
    if (total <= 0)
        return QStringList(); // no files in store.

    QStringList returnList = vectorToStringList(list, QString());
    // free the memory for the tmp list
    store->release(list);
    return returnList;
}

bool MssfStorage::containsFile(const QString &pathname)
{
    return d_ptr->containsFile(pathname);
}

bool MssfStoragePrivate::containsFile(const QString &pathname)
{
    return store->contains_file(pathname.toUtf8().constData());
}

bool MssfStorage::containsLink(const QString &pathname)
{
    return d_ptr->containsLink(pathname);
}

bool MssfStoragePrivate::containsLink(const QString &pathname)
{
    return store->contains_link(pathname.toUtf8().constData());
}

void MssfStorage::addFile(const QString &pathname)
{
    d_ptr->addFile(pathname);
}

void MssfStoragePrivate::addFile(const QString &pathname)
{
    store->add_file(pathname.toUtf8().constData());
}

void MssfStorage::removeFile(const QString &pathname)
{
    d_ptr->removeFile(pathname);
}

void MssfStoragePrivate::removeFile(const QString &pathname)
{
    store->remove_file(pathname.toUtf8().constData());
}

void MssfStorage::addLink(const QString &pathname, const QString &to)
{
    d_ptr->addLink(pathname, to);
}

void MssfStoragePrivate::addLink(const QString &pathname, const QString &to)
{
    store->add_link(pathname.toUtf8().constData(), to.toUtf8().constData());
}

void MssfStorage::removeLink(const QString &pathname)
{
    d_ptr->removeLink(pathname);
}

void MssfStoragePrivate::removeLink(const QString &pathname)
{
    store->remove_link(pathname.toUtf8().constData());
}

void MssfStorage::rename(const QString &pathname, const QString &to)
{
    d_ptr->rename(pathname, to);
}

void MssfStoragePrivate::rename(const QString &pathname, const QString &to)
{
    store->rename(pathname.toUtf8().constData(), to.toUtf8().constData());
}

QString MssfStorage::readLink(const QString &pathname)
{
    return d_ptr->readLink(pathname);
}

QString MssfStoragePrivate::readLink(const QString &pathname)
{
    std::string pointsTo;
    store->read_link(pathname.toUtf8().constData(), pointsTo);
    return QString::fromStdString(pointsTo);
}

bool MssfStorage::verifyFile(const QString &pathname)
{
    return d_ptr->verifyFile(pathname);
}

bool MssfStoragePrivate::verifyFile(const QString &pathname)
{
    return store->verify_file(pathname.toUtf8().constData());
}

bool MssfStorage::verifyContent(const QString &pathname, const QByteArray &data)
{
    return d_ptr->verifyContent(pathname, data);
}

bool MssfStoragePrivate::verifyContent(const QString &pathname, const QByteArray &data)
{
    return store->verify_content(pathname.toUtf8().constData(), (uchar *)data.constData(), data.size());
}

QByteArray MssfStorage::getFile(const QString &pathname)
{
    return d_ptr->getFile(pathname);
}

QByteArray MssfStoragePrivate::getFile(const QString &pathname)
{
    RAWDATA_PTR storedData = NULL;
    size_t length = 0;

    if (store->get_file(pathname.toUtf8().constData(), &storedData, &length) != 0)
    {
        store->release_buffer(storedData);
        return QByteArray();
    }

    QByteArray retrievedData((char *)storedData, length);
    //clean up
    store->release_buffer(storedData);
    return retrievedData;
}

bool MssfStorage::putFile(const QString &pathname, const QByteArray &data)
{
    return d_ptr->putFile(pathname, data);
}

bool MssfStoragePrivate::putFile(const QString &pathname, const QByteArray &data)
{
    return (store->put_file(pathname.toUtf8().constData(), (void *)data.constData(), data.size()) == 0 ? true : false);
}

void MssfStorage::commit()
{
    d_ptr->commit();
}

void MssfStoragePrivate::commit()
{
    store->commit();
}

ProtectedFile* MssfStorage::member(const QString &pathname)
{
   return d_ptr->member(pathname);
}

ProtectedFile* MssfStoragePrivate::member(const QString &pathname)
{
    p_file *file = store->member(pathname.toUtf8().constData());
    if (!file)
        return NULL;
    return new ProtectedFile(new ProtectedFilePrivate(file));
}

bool MssfStorage::statFile(const QString &pathname, struct stat *stbuf)
{
    return d_ptr->statFile(pathname, stbuf);
}

bool MssfStoragePrivate::statFile(const QString &pathname, struct stat *stbuf)
{
    return (store->stat_file(pathname.toUtf8().constData(), stbuf) == 0 ? true : false);
}
