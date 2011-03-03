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

#ifndef MSSFCERTMAN_H
#define MSSFCERTMAN_H

#include "mssf-qt_global.h"

namespace MssfQt
{

class MSSFQTSHARED_EXPORT MssfCertman
{

public:

    /*!
      * \enum DomainType
      * \brief Define the visibility of the domain.
      */
    enum DomainType {
        DomainPrivate,     /*!< DomainPrivate  - Private certificate domains can only be accessed by
                                one application, the one that has created them and owns them.  Store
                                is globally signed and cannot be modified locally. */
        DomainPublic       /*!< DomainPublic  - Common certificate domain, accessible by all applications */
    };

    MssfCertman();
};

} //namespace MssfQt
#endif // MSSFCERTMAN_H
