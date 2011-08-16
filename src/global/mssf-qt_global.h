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

/**
  \mainpage The MSSF-Qt documentation

  This library contains Qt wrappers for some <a href="http://meego.gitorious.org/meego-platform-security">Mobile Simplified Security Framework</a> components:
    \li <a href="../../maemosec-certman-doc/html/index.html">certman</a> - accessing certificate stores (\ref MssfQt::MssfCertman "MssfCertman"),
    \li <a href="../../libcreds2-doc/html/index.html">libcreds2</a> - credentials handling library (\ref MssfQt::CredentialsManager "CredentialsManager" and \ref MssfQt::DBusContextAccessManager "DBusContextAccessManager")
    \li <a href="../../aegis-crypto-doc/index.html">crypto</a> - basic cryptographic functions (\ref MssfQt::MssfCrypto "MssfCrypto", \ref MssfQt::MssfStorage "MssfStorage").

*/

#ifndef MSSFQT_GLOBAL_H
#define MSSFQT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MSSFQT_LIBRARY)
#  define MSSFQTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MSSFQTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MSSF_GLOBAL_H
