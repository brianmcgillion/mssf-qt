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

#include "mssfcrypto.h"

#include <string>

#include <QtCore/QLatin1String>
#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <QtCore/QtGlobal>

#ifdef MAEMO
#include <aegis_crypto.h>
#define mssf_application_id aegis_application_id
#define mssf_application_id_of_bin aegis_application_id_of_bin
#define mssf_as_base64 aegis_as_base64
#define mssf_as_hexstring aegis_as_hexstring
#define mssf_crypto_decrypt aegis_crypto_decrypt
#define mssf_crypto_encrypt aegis_crypto_encrypt
#define mssf_crypto_finish aegis_crypto_finish
#define mssf_crypto_free aegis_crypto_free
#define mssf_crypto_init aegis_crypto_init
#define mssf_crypto_last_error_str aegis_crypto_last_error_str
#define mssf_crypto_ok aegis_crypto_ok
#define mssf_crypto_random aegis_crypto_random
#define mssf_crypto_sign aegis_crypto_sign
#define mssf_crypto_string_to_signature aegis_crypto_string_to_signature
#define mssf_crypto_verify_mssffs aegis_crypto_verify_aegisfs
#define mssf_current_mode aegis_current_mode
#define mssf_signature_t aegis_signature_t
#define mssf_sysinvariant_t aegis_sysinvariant_t
#define mssf_system_mode_t aegis_system_mode_t
#define mssf_system_open aegis_system_open
#define mssf_system_protected aegis_system_protected
#define mssf_crypto_signature_to_string aegis_crypto_signature_to_string
#define mssf_crypto_verify aegis_crypto_verify
#define mssf_system_invariant aegis_system_invariant
#else
#include <mssf_crypto.h>
#endif

using namespace MssfQt;

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
    char *ID = NULL;
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
    if (!pathName)
        return QString();

    char *ID = NULL;
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

bool MssfQt::MssfCrypto::signDataAppended(const QByteArray &data, const char *token, QByteArray &dataAndsignatureOut, MssfCrypto::SignatureFormat format)
{
    QByteArray signature;

    if (!signData(data, token, signature, format))
        return false;

    dataAndsignatureOut.clear();
    dataAndsignatureOut.append(data);
    dataAndsignatureOut.append(Separator);
    dataAndsignatureOut.append(signature);
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

    if (createdMode)
        *createdMode = (mode == mssf_system_open ? SystemOpen : SystemProtected);

    mssf_crypto_free(tokenName);
    return true;
}

bool MssfQt::MssfCrypto::verifySignatureAndSplit(const QByteArray &dataAndSignature, QByteArray &dataOut, MssfCrypto::SystemMode *createdMode)
{
    int sepPosition = dataAndSignature.lastIndexOf(Separator);
    QByteArray data = dataAndSignature.left(sepPosition);
    QByteArray signature = dataAndSignature.mid(sepPosition + 1).data();

    if (!verifySignature(signature, data, createdMode))
        return false;

    dataOut.clear();
    dataOut.append(data);
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
        return QByteArray();
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
        return QByteArray();
    }

    QByteArray decrypted((char *)clearText, length);
    mssf_crypto_free(clearText);
    return decrypted;
}

QByteArray MssfCrypto::random(quintptr size)
{
    QByteArray bytes(size, 0);
    mssf_crypto_random(bytes.data(), size);
    return bytes;
}

QString MssfCrypto::systemInvariant(MssfCrypto::SystemInvariant invariant)
{
    //TODO fix this when more invariants are added
    Q_UNUSED(invariant)
    return QLatin1String(mssf_system_invariant(sysinvariant_imei));
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

    if (mode)
        *mode = (cmode == mssf_system_open ? SystemOpen : SystemProtected);
    return true;
}
