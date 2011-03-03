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

#include "mssfcertman.h"

#ifdef MAEMO
#include <aegis_certman.h>
#define mssf_certman_key_id_to_str aegis_certman_key_id_to_str
#define mssf_certman_str_to_key_id aegis_certman_str_to_key_id
#define mssf_certman_open aegis_certman_open
#define mssf_certman_collect aegis_certman_collect
#define mssf_certman_str_to_key_id aegis_certman_str_to_key_id
#define mssf_certman_iterate_domains aegis_certman_iterate_domains
#define mssf_certman_set_credentials aegis_certman_str_to_key_id
#define mssf_certman_open_domain aegis_certman_open_domain
#define mssf_certman_iterate_certs aegis_certman_iterate_certs
#define mssf_certman_load_cert aegis_certman_load_cert
#define mssf_certman_add_cert aegis_certman_add_cert
#define mssf_certman_add_certs aegis_certman_add_certs
#define mssf_certman_rm_cert aegis_certman_rm_cert
#define mssf_certman_nbrof_certs aegis_certman_nbrof_certs
#define mssf_certman_close_domain aegis_certman_close_domain
#define mssf_certman_get_key_id aegis_certman_get_key_id
#define mssf_certman_store_key aegis_certman_store_key
#define mssf_certman_retrieve_key aegis_certman_retrieve_key
#define mssf_certman_iterate_keys aegis_certman_iterate_keys
#define mssf_certman_get_nickname aegis_certman_get_nickname
#define mssf_certman_has_private_key aegis_certman_has_private_key
#define mssf_certman_has_private_key_by_id aegis_certman_has_private_key_by_id
#define mssf_certman_gui_check_certificate aegis_certman_gui_check_certificate
#define mssf_certman_gui_get_private_key aegis_certman_gui_get_private_key
#else
#include <mssf_certman.h>
#endif

using namespace MssfQt;

MssfCertman::MssfCertman()
{
}
