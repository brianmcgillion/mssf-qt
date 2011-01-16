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

#include "mssfcrypto.h"

#include <mssf_crypto.h>
#include <string>

#include <QtCore/QLatin1String>

MssfCrypto::MssfCrypto()
{
}

MssfCrypto::~MssfCrypto()
{
}

QString MssfCrypto::processName(pid_t pid)
{
    std::string name;

    if (process_name_of_pid(pid, name) == 0)
        return QString();

    return QString::fromStdString(name);
}

QString MssfCrypto::processName()
{
    std::string name;

    if (!process_name(name))
        return QString();

    return QString::fromStdString(name);
}

QString MssfCrypto::applicationId(pid_t pid)
{
    char *ID;
    mssf_application_id(pid, &ID);

    QLatin1String appID(ID);
    mssf_crypto_free(ID);
    return appID;
}

QString MssfCrypto::applicationId(const QString &pathName)
{
    return applicationId(pathName.toUtf8().constData());
}

QString MssfCrypto::applicationId(const char *pathName)
{
    char *ID;
    mssf_application_id_of_bin(pathName, &ID);

    QLatin1String appID(ID);
    mssf_crypto_free(ID);
    return appID;
}

MssfCrypto::SystemMode MssfCrypto::currentMode()
{
    return (mssf_current_mode() == mssf_system_open ? SystemOpen : SystemProtected);
}

bool MssfCrypto::signData(const QByteArray &data, const char *token, QByteArray &signatureOut, MssfCrypto::SignatureFormat format)
{
    mssf_signature_t signature;
    signatureOut.clear();

    //First sign the data
    if (mssf_crypto_sign(data.data(), data.length(), token, &signature) != mssf_crypto_ok)
        return false;

    //Then create a string Representation of if
    char *signatureAsString = NULL;
    mssf_crypto_signature_to_string(&signature, (format == base64 ? mssf_as_base64 : mssf_as_hexstring), token, &signatureAsString);

    signatureOut.append(signatureAsString);
    mssf_crypto_free(signatureAsString);

    return true;
}

QString MssfCrypto::lastError()
{
    return QLatin1String(mssf_crypto_last_error_str());
}


