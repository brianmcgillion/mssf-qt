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
    mssf_crypto_finish();
}

bool MssfCrypto::initialize()
{
    return mssf_crypto_init();
}

QString MssfCrypto::lastError()
{
    return QLatin1String(mssf_crypto_last_error_str());
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
    //First sign the data
    if (mssf_crypto_sign(data.data(), data.length(), token, &signature) != mssf_crypto_ok)
        return false;

    //Then create a string Representation of it
    char *signatureAsString = NULL;
    mssf_crypto_signature_to_string(&signature, (format == base64 ? mssf_as_base64 : mssf_as_hexstring), token, &signatureAsString);

    signatureOut.clear();
    signatureOut.append(signatureAsString);

    mssf_crypto_free(signatureAsString);
    return true;
}

bool MssfCrypto::verifySignature(const QByteArray &signature, const QByteArray &data, MssfCrypto::SystemMode *createdMode)
{
    mssf_signature_t binarySig;
    char *tokenName = NULL;

    if (mssf_crypto_string_to_signature(signature.data(), &binarySig, &tokenName) != mssf_crypto_ok)
    {
        mssf_crypto_free(tokenName);
        return false;
    }

    mssf_system_mode_t mode;
    if (mssf_crypto_verify(&binarySig, tokenName, data.data(), data.length(), &mode) != mssf_crypto_ok)
    {
        mssf_crypto_free(tokenName);
        return false;
    }

    *createdMode = (mode == mssf_system_open ? SystemOpen : SystemProtected);
    mssf_crypto_free(tokenName);
    return true;
}

QByteArray MssfCrypto::encryptData(const QByteArray &clearText, const char *token)
{
    if (clearText.length() <= 0)
        return QByteArray();

    RAWDATA_PTR cipherText = NULL;
    size_t cipherLength = 0;

    if (mssf_crypto_encrypt(clearText.data(), clearText.length(), token, &cipherText, &cipherLength) != mssf_crypto_ok)
    {
        mssf_crypto_free(cipherText);
        return false;
    }

    QByteArray encrypted((char *)cipherText, cipherLength);
    mssf_crypto_free(cipherText);
    return encrypted;
}

QByteArray MssfCrypto::decryptData(const QByteArray &cipherText, const char *token)
{
    if (cipherText.length() <= 0)
        return QByteArray();

    RAWDATA_PTR clearText = NULL;
    size_t length = 0;

    if (mssf_crypto_decrypt(cipherText.data(), cipherText.length(), token, &clearText, &length) != mssf_crypto_ok)
    {
        mssf_crypto_free(clearText);
        return false;
    }

    QByteArray decrypted((char *)clearText, length);
    mssf_crypto_free(clearText);
    return decrypted;
}

QByteArray MssfCrypto::random(quintptr size)
{
    RAWDATA_PTR bytes = NULL;
    size_t length = mssf_crypto_random(&bytes, size);

    QByteArray data((char *)bytes, length);
    mssf_crypto_free(bytes);
    return data;
}

bool MssfCrypto::verifyMssffs(const QString &dir, MssfCrypto::SystemMode *mode)
{
    return verifyMssffs(dir.toUtf8().constData(), mode);
}

bool MssfCrypto::verifyMssffs(const char *dir, MssfCrypto::SystemMode *mode)
{
    mssf_system_mode_t cmode;
    if (mssf_crypto_verify_mssffs(dir, &cmode) != mssf_crypto_ok)
        return false;

    *mode = (cmode == mssf_system_open ? SystemOpen : SystemProtected);
    return true;
}
