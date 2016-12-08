//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//
//
//      ssl.c  
//      Management of SSL (OpenSSL) for POP3 access
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

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#ifdef USE_SSL

#include "main.h"

#include<unistd.h>
#include "ssl.h"

#ifdef USE_WIN32
#define usleep(x) g_usleep(x)
#endif

static SSL_CTX *ssl_ctx_SSLv23 = NULL;
static SSL_CTX *ssl_ctx_TLSv1 = NULL;

static GSList *trust_list = NULL;
static GSList *reject_list = NULL;

extern void pop_debug_print(const gchar *format, ...);
#ifdef USE_WIN32
extern gchar *get_win_home();
#endif
extern gchar *get_rc_dir();

gboolean is_dir_exist(const gchar *dir);


static gchar *find_certs_file(const gchar *certs_dir)
{
	gchar *certs_file;

#define LOOK_FOR(crt)							   \
{									   \
	certs_file = g_strconcat(certs_dir, G_DIR_SEPARATOR_S, crt, NULL); \
	pop_debug_print("looking for %s\n", certs_file);			   \
	if (access(certs_file,F_OK)==0)					   \
		return certs_file;					   \
	if(certs_file) g_free(certs_file);				\
}

	if (certs_dir) {
		LOOK_FOR("ca-certificates.crt");
		LOOK_FOR("ca-bundle.crt");
		LOOK_FOR("ca-root.crt");
		LOOK_FOR("certs.crt");
	}

#undef LOOK_FOR

	return NULL;
}

void ssl_init(void)
{
	gchar *certs_file = NULL, *certs_dir;

	SSL_library_init();
	SSL_load_error_strings();

	certs_dir = g_strconcat(get_rc_dir(), G_DIR_SEPARATOR_S, "certs", NULL);
	if (!is_dir_exist(certs_dir)) {
	 pop_debug_print("ssl_init(): %s doesn't exist, or not a directory.\n",
			  certs_dir);
	 if(certs_dir) g_free(certs_dir);
#ifdef USE_WIN32
		certs_dir = g_strconcat(get_win_home(), G_DIR_SEPARATOR_S
					"etc" G_DIR_SEPARATOR_S
					"ssl" G_DIR_SEPARATOR_S "certs", NULL);
#else
		certs_dir = g_strdup("/etc/ssl/certs");
#endif
		if (!is_dir_exist(certs_dir)) {
		  	pop_debug_print("ssl_init(): %s doesn't exist, or not a directory.\n",
		  	    certs_dir);
			if(certs_dir) g_free(certs_dir);
			certs_dir = NULL;
		}
	}
	if (certs_dir)
		pop_debug_print("ssl_init(): certs dir %s found.\n", certs_dir);

	certs_file = find_certs_file(get_rc_dir());

	if (certs_dir && !certs_file)
		certs_file = find_certs_file(certs_dir);

	if (!certs_file) {
#ifdef USE_WIN32
  	        certs_dir = g_strconcat(get_win_home(),
					G_DIR_SEPARATOR_S "etc"
					G_DIR_SEPARATOR_S "ssl", NULL);
		certs_file = find_certs_file(certs_dir);
		if(certs_dir) g_free(certs_dir);
		certs_dir = NULL;
		if (!certs_file) {
			certs_dir = g_strconcat(get_win_home(),
						G_DIR_SEPARATOR_S "etc", NULL);
			certs_file = find_certs_file(certs_dir);
			if(certs_dir) g_free(certs_dir);
			certs_dir = NULL;
		}
#else
		certs_file = find_certs_file("/etc/ssl");
		if (!certs_file)
			certs_file = find_certs_file("/etc");
#endif
	}

	if (certs_file)
		pop_debug_print("ssl_init(): certs file %s found.\n", certs_file);

	ssl_ctx_SSLv23 = SSL_CTX_new(SSLv23_client_method());
	if (ssl_ctx_SSLv23 == NULL) {
		pop_debug_print("SSLv23 not available\n");
	} else {
		pop_debug_print("SSLv23 available\n");
		if ((certs_file || certs_dir) 
#ifdef USE_OPENSSL
		    &&
		    !SSL_CTX_load_verify_locations(ssl_ctx_SSLv23, certs_file, certs_dir)
#endif
		    )
			g_warning("SSLv23 SSL_CTX_load_verify_locations failed.\n");
	}

	ssl_ctx_TLSv1 = SSL_CTX_new(TLSv1_client_method());
	if (ssl_ctx_TLSv1 == NULL) {
		pop_debug_print("TLSv1 not available\n");
	} else {
		pop_debug_print("TLSv1 available\n");
		if ((certs_file || certs_dir) 
#ifdef USE_OPENSSL
		    &&
		    !SSL_CTX_load_verify_locations(ssl_ctx_TLSv1, certs_file, certs_dir)
#endif
		    )
			g_warning("TLSv1 SSL_CTX_load_verify_locations failed.\n");
	}

	if(certs_dir) g_free(certs_dir);
}

void ssl_done(void)
{
	GSList *cur;

	pop_debug_print("SSL done : in\n");

	for (cur = trust_list; cur != NULL; cur = cur->next)
		X509_free((X509 *)cur->data);
	g_slist_free(trust_list);
	trust_list = NULL;
	for (cur = reject_list; cur != NULL; cur = cur->next)
		X509_free((X509 *)cur->data);
	g_slist_free(reject_list);
	reject_list = NULL;

	if (ssl_ctx_SSLv23) {
		SSL_CTX_free(ssl_ctx_SSLv23);
		ssl_ctx_SSLv23 = NULL;
	}

	if (ssl_ctx_TLSv1) {
		SSL_CTX_free(ssl_ctx_TLSv1);
		ssl_ctx_TLSv1 = NULL;
	}
	pop_debug_print("SSL done : out\n");
}

SSL *ssl_init_socket(gint fd, char *hostname, gint ssl_cert_res,
		     gchar **subject, gchar **issue, glong *verify)
{
  return ssl_init_socket_with_method(fd, hostname, ssl_cert_res, 
				     subject, issue, verify, SSL_METHOD_SSLv23);
}

static gint x509_cmp_func(gconstpointer a, gconstpointer b)
{
	const X509 *xa = a;
	const X509 *xb = b;

	return X509_cmp(xa, xb);
}

#ifdef USE_GNUTLS
X509* X509_dup(const X509* cert) {
	X509* ret;
	ret = (X509*)malloc(sizeof(X509));
	ret->size = cert->size;
	ret->data = (unsigned char *)malloc(sizeof(unsigned char)*ret->size);
	memcpy(ret->data,cert->data,ret->size);
	return ret;
}

int X509_cmp(const X509* cert1,const X509* cert2) {
	if (cert1->size != cert2->size) {
		return (cert1->size!=cert2->size);
	}
	return memcmp(cert1->data,cert2->data,cert1->size);
}
#endif //USE_GNUTLS

SSL *ssl_init_socket_with_method(gint fd, char *hostname,  gint ssl_cert_res, 
				 gchar **subject, gchar **issue, 
				 glong *verify, SSLMethod method)
{
	X509 *server_cert;
	gint err, ret;
	SSL *ssl;

	switch (method) {
	case SSL_METHOD_SSLv23:
		if (!ssl_ctx_SSLv23) {
			g_warning("SSL method not available\n");
			return(NULL);
		}
		ssl = SSL_new(ssl_ctx_SSLv23);
		break;
	case SSL_METHOD_TLSv1:
		if (!ssl_ctx_TLSv1) {
			g_warning("SSL method not available\n");
			return(NULL);
		}
		ssl = SSL_new(ssl_ctx_TLSv1);
		break;
	default:
	  g_warning("Unknown SSL method *PROGRAM BUG*\n");
	  return(NULL);
	  break;
	}

	if (ssl == NULL) {
		g_warning("Error creating ssl context\n");
		return(NULL);
	}

	SSL_set_fd(ssl, fd);
	while ((ret = SSL_connect(ssl)) != 1) {
		err = SSL_get_error(ssl, ret);
		if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
			usleep(100000);
			g_warning("SSL_connect(): try again\n");
			continue;
		}
		g_warning("SSL_connect() failed with error %d, ret = %d (%s)\n",
			  err, ret, ERR_error_string(ERR_get_error(), NULL));
		ssl_done_socket(ssl);
		return(NULL);
	}

	/* Get the cipher */

	pop_debug_print("SSL connection using %s\n",
		    SSL_get_cipher(ssl));

	/* Get server's certificate (note: beware of dynamic allocation) */

	if ((server_cert = SSL_get_peer_certificate(ssl)) != NULL) {
		gchar *str;
		glong verify_result;
		gchar *str_issuer, *str_subject;
		
		str_issuer=NULL;
		str_subject=NULL;

		pop_debug_print("Server certificate:\n");

		if ((str = g_strdup(X509_NAME_oneline(X509_get_subject_name(server_cert), 0, 0))) != NULL) {
			pop_debug_print("  Subject: %s\n", str);
			if(ssl_cert_res==SSL_CERT_NONE)
			  str_subject=g_strdup(str);
			if(str) g_free(str);
		}

		if ((str = g_strdup(X509_NAME_oneline(X509_get_issuer_name(server_cert), 0, 0))) != NULL) {
			pop_debug_print("  Issuer: %s\n", str);
			if(ssl_cert_res==SSL_CERT_NONE)
			  str_issuer=g_strdup(str);
			if(str) g_free(str);
		}

#ifdef USE_GNUTLS
		verify_result = 0 /*SSL_get_verify_result(ssl)*/;
		if (verify_result == 0 /*X509_V_OK*/) {
#else //OPEN_SSL
		verify_result = SSL_get_verify_result(ssl);
		if (verify_result == X509_V_OK) {
#endif
			pop_debug_print("SSL verify OK\n");
			X509_free(server_cert);
			return(ssl);
		} else if (g_slist_find_custom(trust_list, server_cert,
					     x509_cmp_func)) {
		        pop_debug_print("SSL certificate of %s previously accepted\n", hostname);
			X509_free(server_cert);
			return(ssl);
		} else if (g_slist_find_custom(reject_list, server_cert,
					       x509_cmp_func)) {
		  	pop_debug_print("SSL certificate of %s previously rejected\n", hostname);
			X509_free(server_cert);
			ssl_done_socket(ssl);
			return(NULL);
		}

			pop_debug_print("%s: SSL certificate verify failed (%ld: %s)\n",
				  hostname, verify_result,
#ifdef USE_GNUTLS
			/*X509_verify_cert_error_string(verify_result)*/"FIXME");
#else
			X509_verify_cert_error_string(verify_result));
#endif

		{
			gint res;

			//res = ssl_manager_verify_cert(fd, hostname,
			//			      server_cert, verify_result);
			// It is difficult to call GUI 
			//   from forked background job (UNIX ver)
			// So, basical temp accept should  be applied.
			switch(ssl_cert_res){
			case SSL_CERT_NONE:
#ifdef USE_GNUTLS
			  if(verify_result == /*X509_V_OK*/0){
#else
			  if(verify_result == X509_V_OK){
#endif
			    res=0;
			  }
			  else{
			    if(*subject) g_free(*subject);
			    *subject = g_strdup(str_subject);
			    if(str_subject) g_free(str_subject);
			    
			    if(*issue) g_free(*issue);
			    *issue = g_strdup(str_issuer);
			    if(str_issuer) g_free(str_issuer);

			    *verify=verify_result;

			    X509_free(server_cert);
			    ssl_done_socket(ssl);
			    return(NULL);
			  }
			  break;
			case SSL_CERT_DENY:
			  res=-1;
			  break;
			case SSL_CERT_ACCEPT:
			  res=0;
			  break;
			}

			/* 0: accept 1: temporarily accept -1: reject */
			if (res < 0) {
			  pop_debug_print("SSL certificate of %s rejected\n",
			  		   hostname);
#if 0
				reject_list = g_slist_prepend
					(reject_list, X509_dup(server_cert));
#endif
				X509_free(server_cert);
				ssl_done_socket(ssl);
				return(NULL);
			} else if (res > 0) {
			  	pop_debug_print("Temporarily accept SSL certificate of %s\n", hostname);
				trust_list = g_slist_prepend
					(trust_list, X509_dup(server_cert));
			} else {
			        pop_debug_print("Permanently accept SSL certificate of %s\n", hostname);
				/* TODO: save server cert */
				trust_list = g_slist_prepend
					(trust_list, X509_dup(server_cert));
			}
		}

		X509_free(server_cert);
	} else {
	  g_warning("%s: couldn't get SSL certificate\n",
	  		  hostname);
	  ssl_done_socket(ssl);
	  return(NULL);
	}

	return(ssl);
}

void ssl_done_socket(SSL *ssl)
{
	if (ssl) {
		SSL_free(ssl);
	}
}

gboolean is_dir_exist(const gchar *dir)
{
	if (dir == NULL)
		return FALSE;

#ifdef USE_GTK2
	return g_file_test(dir, G_FILE_TEST_IS_DIR);
#else
	if(access(dir,F_OK)) return(TRUE);
	else return(FALSE);
#endif
}

#endif /* USE_SSL */

