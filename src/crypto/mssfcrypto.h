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

#ifndef MSSFCRYPTO_H
#define MSSFCRYPTO_H

#include "mssf-qt_global.h"

#include <unistd.h>

#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <QtCore/QtGlobal>

class MSSFQTSHARED_EXPORT MssfCrypto
{

public:

    /*!
     * \enum SystemMode
     * \brief The mode the system is in.
     *
     * "Open" means that that the integrity cannot be guaranteed,
     * as for instance the kernel may have been modified.
     * "Protected" means that the system integrity is guaranteed.
     */
    enum SystemMode {
        SystemOpen,         /*!< SystemOpen      - Integrity cannot be guaranteed */
        SystemProtected     /*!< SystemProtected - Integrity should be guaranteed */
    };

    /*!
     * \enum SignatureFormat
     * \brief The format to use for the data signatures.
     */
    enum SignatureFormat {
        hexString,          /*!< hexString      - Use a hex string, characters 0-9, a-f */
        base64              /*!< base64         - Use base 64 encoding. */
    };

    /*!
      * \enum mssf_sysinvariant_t
      * \brief Supported system invariants
      * These are system wide constant values that are not supposed to change uncontrollably.
      */
    enum SystemInvariant {
        sysIMEI             /*!< IMEI           - The IMEI code of the device. */
    };

    /*!
      * \brief Default constructor.
      */
    MssfCrypto();

    /*!
      * \brief Destructor
      */
    ~MssfCrypto();

    /*!
      * \brief Initialize the underlying crypto framework.
      * \returns true on success, false otherwise.
      * Although not strictly required to call this method, it would be best to explicetidly call
      * this method to catch any errors as early as possible.
      */
    bool initialize();

    /*!
      * \brief Return the last error that occured.
      * On Error this method should be called before any other methods to ensure the
      * correct error is returned
      */
    QString lastError();

    /*!
      * \brief Determine the name of a process that is running with a give process ID.
      * \param pid The process ID of the process in question.
      * \returns The name of the process if it exists, or QString() otherwise.
      */
    QString processName(pid_t pid);

    /*!
      * \brief Determine the name of the current process.
      * \returns The name of the process, or QString() if it can't be determined.
      * This is an overloaded method provided for convenience.
      */
    QString processName();

    /*!
      * \brief Return the application ID of a particular application.
      * \param pid The process ID of the application to query.
      * \returns The application ID if it exists, QString() on error.
      * The application ID differs from teh process name in that it is the unque name that is
      * created by the MSSF framework based on teh package that the binary belongs to.
      */
    QString applicationId(pid_t pid);

    /*!
      * \brief Return the application ID of a particular application.
      * \param pathName The path name of the binary to query.
      * \returns The application ID if it exists, QString() on error.
      * This is an overloaded method provided for convenience.
      */
    QString applicationId(const QString &pathName);

    /*!
      * \brief Return the application ID of a particular application.
      * \param pathName The path name of the binary to query.
      * \returns The application ID if it exists, QString() on error.
      * This is an overloaded method provided for convenience.
      */
    QString applicationId(const char *pathName);

    /*!
     * \brief In what mode the system seems to be in
     * \return The current mode.
     *
     * Notice that this function cannot be fully trusted, as it
     * is possible to fool the system to believe that it is in
     * protected mode even if it is not by a custom kernel and
     * by emulating BB5 functions, or simply be replacing this
     * very function. It should be used only as a guideline.
     */
    SystemMode currentMode();

    /*!
      * \brief Sign some data with a token.
      * \param data The data that is to be signed
      * \param token The NULL terminated name of the token to use, use NULL to specify the current APPLICATION ID.
      * \param signatureOut The resulting signature if successful, QByteArray() otherwise.
      * \param format The encoding format to use for the signature
      * \returns true on success, false otherwise.
      * \sa MssfCrypto::SignatureFormat
      */
    bool signData(const QByteArray &data, const char *token, QByteArray &signatureOut, MssfCrypto::SignatureFormat format = base64);

    /*!
      * \brief Verify That a signature is valid for a given piece of data.
      * \param signature The signature to verify.
      * \param data The data that the signature is computed over.
      * \param createdMode Used to determine the mode in which the signature was created. \sa MssfCrypto::SystemMode
      * \returns true on success, false otherwise.  If the created mode is Open then true can only be used as a guide line.
      */
    bool verifySignature(const QByteArray &signature, const QByteArray &data, MssfCrypto::SystemMode *createdMode);

    /*!
      * \brief Encrypt the clear text and use the optional token for reference.
      * \param data The origional message that is to be encrypted.
      * \param token The optional token to use for referencing the encrypted data.
      * \returns The encrypted data on success or an empty QByteArray() on failure.
      */
    QByteArray encryptData(const QByteArray &data, const char *token);

    /*!
      * \brief decrypt a given message using an optional token as reference
      * \param data The encrypted message that is to be decoded.
      * \param token An optional token to use as a reference
      * \returns The deciphered data on success, an empty QByteArray() on failure.
      */
    QByteArray decryptData(const QByteArray &data, const char *token);

    /*!
      * \brief Create an array of random data.
      * \param size The number of random bytes required.
      * \returns The random array.
      */
    QByteArray random(quintptr size);

    /*!
      * \brief Query the value of a given system invariant
      * \returns The value of the invariant.
      */
    QString systemInvariant(MssfCrypto::SystemInvariant invariant);

    /*!
     * \brief Verify that the given directory is an MSSFFS mountpoint
     * \param dir Name of the directory
     * \param mode On ouput, the variable tells in which mode
     *             the security framework is on. If the mode
     *             is open, take the result with a grain of salt.
     * \returns true on success, false otherwise
     *
     * Use this function to verify that a directory is mounted by
     * the MSSFFS cryptofilesystem. If the result is positive and
     * the returned mode is protected, you should be good to go.
     * Otherwise you might want to reconsider whether you want to
     * store sensitive data in this directory or rely on the
     * integrity of its contents.
     *
     * This function can be used for all types of mssffs mountpoints
     * regardless of their type and access rights.
     */
    bool verifyMssffs(const QString &dir, MssfCrypto::SystemMode *mode);

    /*!
     * \brief Verify that the given directory is an MSSFFS mountpoint
     * \param dir Name of the directory
     * \param mode On ouput, the variable tells in which mode
     *             the security framework is on. If the mode
     *             is open, take the result with a grain of salt.
     * \returns true on success, false otherwise
     * This overladed method is provided for convenience.
     */
    bool verifyMssffs(const char *dir, MssfCrypto::SystemMode *mode);
};

#endif // MSSFCRYPTO_H
