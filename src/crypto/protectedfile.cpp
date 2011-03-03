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


#include "protectedfile.h"
#include "protectedfile_p.h"
#include "mssfstorage.h"
#include "mssfstorage_p.h"


#include <QtCore/QByteArray>
#include <QtCore/QDateTime>

#include <utime.h>

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

ProtectedFile::ProtectedFile(ProtectedFilePrivate *other)
    : d_ptr(other)
{
}

ProtectedFilePrivate::ProtectedFilePrivate(p_file *file)
    : file(file),
      ownerPointer(NULL)
{
}

ProtectedFile::~ProtectedFile()
{
}

ProtectedFilePrivate::~ProtectedFilePrivate()
{
    delete file; file = NULL;
}

bool ProtectedFile::open(QFile::Permissions flags)
{
    return d_ptr->open(flags);
}

bool ProtectedFilePrivate::open(QFile::Permissions flags)
{
    //make the owner the same as user
    if (flags.testFlag(QFile::ReadOwner))
        flags |= QFile::ReadUser;
    if (flags.testFlag(QFile::WriteOwner))
        flags |= QFile::WriteUser;
    if (flags.testFlag(QFile::ExeOwner))
        flags |= QFile::ExeUser;

    return file->p_open(flags);
}

QByteArray ProtectedFile::read(quint64 at, quintptr len)
{
    return d_ptr->read(at, len);
}

QByteArray ProtectedFilePrivate::read(quint64 at, quintptr len)
{
    QByteArray bytes(len, 0);
    file->p_read(at, bytes.data(), len);
    return bytes;
}

qptrdiff ProtectedFile::write(quint64 at, QByteArray &data)
{
    return d_ptr->write(at, data);
}

qptrdiff ProtectedFilePrivate::write(quint64 at, QByteArray &data)
{
    return file->p_write(at, (void *)data.constData(), data.length());
}

bool ProtectedFile::trunc(quint64 at)
{
    return d_ptr->trunc(at);
}

bool ProtectedFilePrivate::trunc(quint64 at)
{
    return (file->p_trunc(at) == 0 ? true : false);
}

void ProtectedFile::close()
{
    d_ptr->close();
}

void ProtectedFilePrivate::close()
{
    file->p_close();
}

bool ProtectedFile::isOpen()
{
    return d_ptr->isOpen();
}

bool ProtectedFilePrivate::isOpen()
{
    return file->is_open();
}

bool ProtectedFile::status(struct stat *st)
{
    return d_ptr->status(st);
}

bool ProtectedFilePrivate::status(struct stat *st)
{
    return (file->p_stat(st) == 0 ? true : false);
}

QByteArray ProtectedFile::digest()
{
    return d_ptr->digest();
}

QByteArray ProtectedFilePrivate::digest()
{
    return QByteArray(file->digest());
}

QString ProtectedFile::name()
{
    return d_ptr->name();
}

QString ProtectedFilePrivate::name()
{
    return QLatin1String(file->name());
}

QSharedPointer<MssfStorage> ProtectedFile::owner()
{
    return d_ptr->owner();
}

QSharedPointer<MssfStorage> ProtectedFilePrivate::owner()
{
    if (!ownerPointer)
        ownerPointer = QSharedPointer<MssfStorage>(new MssfStorage(new MssfStoragePrivate(file->owner())));
    return ownerPointer;
}

bool ProtectedFile::rename(QString newName)
{
    return d_ptr->rename(newName);
}

bool ProtectedFilePrivate::rename(QString newName)
{
    return (file->p_rename(newName.toUtf8().constData()) == 0 ? true : false);
}

bool ProtectedFile::chmod(QFile::Permissions flags)
{
    return d_ptr->chmod(flags);
}

bool ProtectedFilePrivate::chmod(QFile::Permissions flags)
{
    //make the owner the same as user
    if (flags.testFlag(QFile::ReadOwner))
        flags |= QFile::ReadUser;
    if (flags.testFlag(QFile::WriteOwner))
        flags |= QFile::WriteUser;
    if (flags.testFlag(QFile::ExeOwner))
        flags |= QFile::ExeUser;

    return (file->p_chmod(flags) == 0 ? true : false);
}

bool ProtectedFile::chown(uid_t uid, gid_t gid)
{
    return d_ptr->chown(uid, gid);
}

bool ProtectedFilePrivate::chown(uid_t uid, gid_t gid)
{
    return (file->p_chown(uid, gid) == 0 ? true : false);
}

bool ProtectedFile::utime(const QDateTime &accessTime, const QDateTime &modifiedTime)
{
    return d_ptr->utime(accessTime, modifiedTime);
}

bool ProtectedFilePrivate::utime(const QDateTime &accessTime, const QDateTime &modifiedTime)
{
    struct utimbuf bufTime;
    bufTime.actime = accessTime.toTime_t();
    bufTime.modtime = modifiedTime.toTime_t();
    return (file->p_utime(&bufTime) == 0 ? true : false);
}
