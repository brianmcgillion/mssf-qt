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

#include "credentialsutils.h"

#ifdef MAEMO
#include <sys/creds.h>
#else
#include <creds.h>
#endif

#include <errno.h>
#include <limits.h>

namespace MssfQt
{

CredentialsUtils::CredentialsUtils()
{
}

CredentialsUtils::~CredentialsUtils()
{
}

quint32 CredentialsUtils::stringToCreds(const QString &name, QString *errorString)
{
    creds_type_t type;
    creds_value_t value;

    type = creds_str2creds(name.toUtf8().constData(), &value);

    // if something is wrong - reset value ("value The credential value (NULL possible)")
    if (type == CREDS_BAD)
    {
        char strErrArray[256];
        setLastError(QString(QLatin1String("Invalid credential string (%1), errno (%2) : %3"))
                     .arg(name).arg(errno).arg(strerror_r(errno, strErrArray, sizeof(strErrArray))),
                     errorString);
        value = UINT_MAX;
    }

    return value;
}

bool CredentialsUtils::setLastError(const QString &errorStr, QString *returnString)
{
    if (!returnString)
        return false;

    returnString->clear();
    returnString->append(errorStr);
    return false;
}

} // namespace MssfQt
