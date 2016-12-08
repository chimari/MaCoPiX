//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//
//
//      ssl.h  
//      Heade for SSL (OpenSSL) management
//          originated from Libsylph 2.4.0 (see blow)
//
//                            Copyright 2002-2007  K.Chimari
//                                     http://rosegray.sakura.ne.jp/
//
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
// 
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
//

/*
 * LibSylph -- E-Mail client library
 * Copyright (C) 1999-2006 Hiroyuki Yamamoto
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __SSL_H__
#define __SSL_H__

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#ifdef USE_SSL

#include <glib.h>

#ifdef USE_OPENSSL
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

#ifdef USE_GNUTLS
#include <gnutls/openssl.h>
#endif



typedef enum {
	SSL_METHOD_SSLv23,
	SSL_METHOD_TLSv1
} SSLMethod;





void ssl_init				(void);
void ssl_done				(void);
SSL *ssl_init_socket		(gint fd,
				 char *server, 
				 gint ssl_cert_res,
				 gchar **subject,
				 gchar **issue,
				 glong *verify);
SSL *ssl_init_socket_with_method	(gint fd, 
					 char *server,  
					 gint ssl_cert_res,
					 gchar **subject,
					 gchar **issue,
					 glong *verify,
					 SSLMethod method);
void ssl_done_socket			(SSL *ssl);

#endif /* USE_SSL */

#endif /* __SSL_H__ */
