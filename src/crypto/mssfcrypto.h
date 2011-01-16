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

#include <unistd.h>

#include <QtCore/QString>
#include <QtCore/QByteArray>

class MssfCrypto
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
        SystemOpen,      /*!< SystemOpen      - Integrity cannot be guaranteed */
        SystemProtected  /*!< SystemProtected - Integrity should be guaranteed */
    };

    /*!
     * \enum SignatureFormat
     * \brief The format to use for the data signatures.
     */
    enum SignatureFormat {
        hexString,      /*!< hexString      - Use a hex string, characters 0-9, a-f */
        base64          /*!< base64         - Use base 64 encoding. */
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
};

#endif // MSSFCRYPTO_H
