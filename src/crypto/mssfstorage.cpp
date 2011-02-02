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

#include <mssf_storage.h>

//Convert to wrapped types
mssf::storage::visibility_t visConverter(MssfStorage::Visibility vis)
{
    if (vis == MssfStorage::global_vis)
        return mssf::storage::vis_global;
    else if (vis == MssfStorage::private_vis)
        return mssf::storage::vis_private;
    else
        return mssf::storage::vis_shared;
}

mssf::storage::protection_t protConverter(MssfStorage::Protection prot)
{
    if (prot == MssfStorage::Encrypted)
        return mssf::storage::prot_encrypted;
    else
        return mssf::storage::prot_signed;
}

MssfStorage::MssfStorage(const QString &name, const QString &owner, MssfStorage::Visibility vis, MssfStorage::Protection prot)
    : store(new mssf::storage(name.toUtf8().constData(), owner.toUtf8().constData(), visConverter(vis), protConverter(prot)))
{
}

MssfStorage::~MssfStorage()
{
    delete store; store = NULL;
}
