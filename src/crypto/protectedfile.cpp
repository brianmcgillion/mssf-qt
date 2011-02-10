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


#include "protectedfile.h"
#include "mssfstorage.h"

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

ProtectedFile::ProtectedFile(p_file *file)
    : file(file),
      ownerPointer(NULL)
{
}

ProtectedFile::~ProtectedFile()
{
    delete file; file = NULL;
}

bool ProtectedFile::open(QFile::Permissions flags)
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
    QByteArray bytes(len, 0);
    file->p_read(at, bytes.data(), len);
    return bytes;
}

qptrdiff ProtectedFile::write(quint64 at, QByteArray &data)
{
    return file->p_write(at, (void *)data.constData(), data.length());
}

bool ProtectedFile::trunc(quint64 at)
{
    return (file->p_trunc(at) == 0 ? true : false);
}

void ProtectedFile::close()
{
    file->p_close();
}

bool ProtectedFile::isOpen()
{
    return file->is_open();
}

bool ProtectedFile::status(struct stat *st)
{
    return (file->p_stat(st) == 0 ? true : false);
}

QByteArray ProtectedFile::digest()
{
    return QByteArray(file->digest());
}

QString ProtectedFile::name()
{
    return QLatin1String(file->name());
}

QSharedPointer<MssfStorage> ProtectedFile::owner()
{
    if (!ownerPointer)
        ownerPointer = QSharedPointer<MssfStorage>(new MssfStorage(file->owner()));
    return ownerPointer;
}

bool ProtectedFile::rename(QString newName)
{
    return (file->p_rename(newName.toUtf8().constData()) == 0 ? true : false);
}

bool ProtectedFile::chmod(QFile::Permissions flags)
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
    return (file->p_chown(uid, gid) == 0 ? true : false);
}

bool ProtectedFile::utime(const QDateTime &accessTime, const QDateTime &modifiedTime)
{
    struct utimbuf bufTime;
    bufTime.actime = accessTime.toTime_t();
    bufTime.modtime = modifiedTime.toTime_t();
    return (file->p_utime(&bufTime) == 0 ? true : false);
}
