//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//
//
//      pop.c  
//      Management of pop3(apop) access to mail server
//          originated from POP3 library (see blow)
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


/*******************************************************************************
 * POP3 Library.
 * COPYRIGHT(C) 1999-2001 TKS
 * 
 * $Revision: 1.8 $
 * $Date: 2001/02/07 14:34:41 $
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 ******************************************************************************/
#include "main.h"

#ifdef USE_BIFF

// #include <config.h>
#include <stdlib.h>
#include <sys/types.h>
#ifdef USE_WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif
#include <unistd.h>
#include <errno.h>
#include <strings.h>
#include <stdio.h>

#include "libpop.h"
#include "main.h"

#ifdef USE_SSL
#include<fcntl.h>
#  include "ssl.h"
SSL *ssl;
#endif

extern typMascot *Mascot;

/*==========================================================================*/
/* Function Prototype                                                       */
/*==========================================================================*/
int popReadLine(char *ptr, int size, guint ssl_mode);
int popWriteLine(char *ptr, guint ssl_mode);

//from Sylpheed
gint sock_gets();
gint sock_write();
gint sock_write_all();
gint fd_recv();
gint fd_write();
gint fd_write_all();
gint fd_gets();
#ifdef USE_SSL
gint ssl_read();
gint ssl_peek();
gint ssl_gets();
gint ssl_write();
gint ssl_write_all();
static gint set_nonblocking_mode(gint fd, gboolean nonblock);
#endif
static gint fd_check_io();

/*==========================================================================*/
/* Internal Function Prototype                                              */
/*==========================================================================*/
static int popIsOK(char *ptr);
static int popLineCommand(char *ptr, guint ssl_mode);
static int popApopGetKey(char *ptr, char *apop_key);
static int popApop(char *user, char *passwd, char *apop_key, guint ssl_mode);

extern  char * MD5Digest (unsigned char *s);

/*==========================================================================*/
/* Global Variables                                                         */
/* Can not use application.                                                 */
/*==========================================================================*/
static int  Sockfd;                                   /* POP3 server socket */
#ifndef USE_WIN32
//static FILE *Rfp;                                     /* Read file pointer  */
//static FILE *Wfp;                                     /* Write file pointer */
#endif


/*==========================================================================*/
/* Read data (one line) from POP3 server                                    */
/*==========================================================================*/
int popReadLine(char *ptr, int size,  guint ssl_mode)
{
  gint ret;

  ret = sock_gets(Sockfd, ptr, size, ssl_mode);

  pop_debug_print("Recv() %s\n", ptr);

#ifdef USE_WIN32
  if (ret == SOCKET_ERROR) {
    return(errno);
  }
#endif

  return(0);
  
	 /*
#ifdef USE_WIN32
  gint ret;
  
#ifdef USE_SSL
  if(ssl_flag){
    ret = ssl_gets(ssl, ptr, size);
  }
  else{
    ret = fd_gets(Sockfd, ptr, size);
  }
#else
  ret = fd_gets(Sockfd, ptr, size);
#endif
  pop_debug_print("Recv() %s\n", ptr);

#ifdef USE_WIN32
  if (ret == SOCKET_ERROR) {
    return(errno);
  }
#endif

  return(0);
#else
  char *funcret;

  funcret = fgets(ptr, size, Rfp);
  if( funcret == NULL ){
    return(errno);
  }
  return(0);
#endif
	 */
}


/*============================================================================*/
/* Send data (one line) to POP3 server                                        */
/*============================================================================*/
int popWriteLine(char *ptr, guint ssl_mode)
{
  gint ret;

  ret = sock_write_all(Sockfd, ptr, strlen(ptr), ssl_mode);

  pop_debug_print("Send() %s",ptr);

  if (ret < 0 ) {
    return(errno);
  }

  return(0);

  /*
#ifdef USE_WIN32
  gint ret, err;
  gint i;

  i=0;
  do{
    ret=send(Sockfd, ptr, strlen(ptr), 0);
   
    if(ret==SOCKET_ERROR){
      pop_debug_print("Send() Error TimeOut...  %d\n",WSAGetLastError());
      Sleep(1000);
    }
    i++;
  }while((ret==SOCKET_ERROR)&&(i<10));

  if(ret==SOCKET_ERROR){
    return(errno);
  }


  i=0;
  if(!strstr(ptr, "\r\n")){
    do{
      ret = send(Sockfd, "\r\n", strlen("\r\n"), 0);
      if( ret==SOCKET_ERROR ){
	pop_debug_print("Send() Error TimeOut...  %d\n",WSAGetLastError());
	Sleep(1000);
      }
      i++;
    }while((ret==SOCKET_ERROR)&&(i<10));
  }

  if(ret==SOCKET_ERROR){
    return(errno);
  }

  return(0);
#else
  int funcret;

  funcret = fputs(ptr, Wfp);
  if( funcret < 0 ){
    return(errno);
  }
  if(!strstr(ptr, "\r\n")){
    funcret = fputs("\r\n", Wfp);
    if( funcret < 0 ){
      return(errno);
    }
  }
  fflush(Wfp);
  return(0);
#endif
  */
}



//from Sylpheed/libsylph-2.4.0
gint sock_gets(gint fd, gchar *buf, gint len, guint ssl_mode)
{
#ifdef USE_SSL
  if(ssl_mode!=SSL_NONE){
    if (ssl)
      return ssl_gets(buf, len);
    else
      g_return_val_if_fail(ssl != NULL, -1);
  }
#endif
  return fd_gets(fd, buf, len);
}

gint sock_write(gint fd, const gchar *buf, gint len, guint ssl_mode)
{
#ifdef USE_SSL

  if(ssl_mode!=SSL_NONE){
    if (ssl)
      return ssl_write(buf, len);
    else
      g_return_val_if_fail(ssl != NULL, -1);
  }
#endif
  return fd_write(fd, buf, len);
}

gint fd_recv(gint fd, gchar *buf, gint len, gint flags)
{
  gint ret;
  
  if (fd_check_io(fd, G_IO_IN) < 0)
    return -1;

  ret = recv(fd, buf, len, flags);
#ifdef USE_WIN32
  if (ret == SOCKET_ERROR) {
    pop_debug_print("Recv() Error TimeOut...  %d\n",WSAGetLastError());
  }
#endif
  return ret;
}

gint fd_write(gint fd, const gchar *buf, gint len)
{
#ifdef USE_WIN32
  gint ret;
#endif
  if (fd_check_io(fd, G_IO_OUT) < 0)
    return -1;
  
#ifdef USE_WIN32
  ret = send(fd, buf, len, 0);
  if (ret == SOCKET_ERROR) {
    gint err;
    err = WSAGetLastError();
    switch (err) {
    case WSAEWOULDBLOCK:
      errno = EAGAIN;
      break;
    default:
      pop_debug_print("last error = %d\n", err);
      errno = 0;
      break;
    }
    if (err != WSAEWOULDBLOCK)
      g_warning("fd_write() failed with %d (errno = %d)\n",
		err, errno);
  }
  return ret;
#else
  return write(fd, buf, len);
#endif
}


gint fd_gets(gint fd, gchar *buf, gint len)
{
  gchar *newline, *bp = buf;
  gint n;
  
  if (--len < 1)
    return -1;
  do {
    if ((n = fd_recv(fd, bp, len, MSG_PEEK)) <= 0)
      return -1;
    if ((newline = memchr(bp, '\n', n)) != NULL)
      n = newline - bp + 1;
    if ((n = fd_recv(fd, bp, n, 0)) < 0)
      return -1;
    bp += n;
    len -= n;
  } while (!newline && len);
  
  *bp = '\0';
  return bp - buf;
}


#ifdef USE_SSL
gint ssl_gets(gchar *buf, gint len)
{
	gchar *newline, *bp = buf;
	gint n;

	if (--len < 1)
		return -1;
	do {
		if ((n = ssl_peek(bp, len)) <= 0)
			return -1;
		if ((newline = memchr(bp, '\n', n)) != NULL)
			n = newline - bp + 1;
		if ((n = ssl_read(bp, n)) < 0)
			return -1;
		bp += n;
		len -= n;
	} while (!newline && len);

	*bp = '\0';
	return bp - buf;
}
#endif

#ifdef USE_SSL
#ifdef USE_GNUTLS
char *SSL_mybuf = NULL;
int SSL_mybuf_size=0;
int SSL_myread(SSL *ssl, void *buf, int num) {
  int ret=0,i;
  if (SSL_mybuf==NULL) {
    ret = SSL_read(ssl,buf,num);
    return ret;
  } else {
    if (SSL_mybuf_size <= num) {
      memcpy(buf,SSL_mybuf,SSL_mybuf_size);
      ret = SSL_mybuf_size;
      free(SSL_mybuf);
      SSL_mybuf=NULL;
      SSL_mybuf_size=0;
    } else {
      memcpy(buf,SSL_mybuf,num);
      ret = num;
      memmove(SSL_mybuf,&(SSL_mybuf[num]),SSL_mybuf_size-num);
      SSL_mybuf_size -= num;
      SSL_mybuf = realloc(SSL_mybuf,SSL_mybuf_size);
    }
    return ret;
  }
}

int SSL_mypeek(SSL *ssl,void *buf,int num) {
  int tsize;
  if (SSL_mybuf == NULL) {
    SSL_mybuf_size = SSL_read(ssl,buf,num);
    SSL_mybuf = (char *)malloc(sizeof(char)*SSL_mybuf_size);
    memcpy(SSL_mybuf,buf,SSL_mybuf_size);
    return SSL_mybuf_size;
  } else if (SSL_mybuf != NULL && SSL_mybuf_size >= num) {
    memcpy(buf,SSL_mybuf,num);
    return num;
  } else if (SSL_mybuf != NULL && SSL_mybuf_size < num && SSL_pending(ssl)>0) {
    tsize = SSL_read(ssl,buf,num-SSL_mybuf_size);
    if(tsize>0){
      SSL_mybuf = realloc(SSL_mybuf,SSL_mybuf_size + tsize);
      memcpy(&(SSL_mybuf[SSL_mybuf_size]),buf,tsize);
      SSL_mybuf_size += tsize;
    }
    memcpy(buf,SSL_mybuf,SSL_mybuf_size);
    return SSL_mybuf_size;
  } else {
    memcpy(buf,SSL_mybuf,SSL_mybuf_size);
    return SSL_mybuf_size;
  }
  return 0;
}  

int SSL_mypending(SSL *ssl) {
  if (SSL_mybuf==NULL) {
    SSL_mybuf = (char *)malloc(1);
    SSL_mybuf_size = SSL_read(ssl,SSL_mybuf,1);
    if (SSL_mybuf_size<=0) {
      free(SSL_mybuf);
      SSL_mybuf = NULL;
    }
    return 1;
  } else {
    return SSL_mybuf_size;
  }
}
#endif //USE_GNUTLS

gint ssl_read(gchar *buf, gint len)
{
	gint err, ret;

#ifdef USE_GNUTLS
	if (SSL_mypending(ssl) == 0) {
	  if (fd_check_io(/*SSL_get_rfd(ssl)*/ssl->rfd, G_IO_IN) < 0)
#else
	if (SSL_pending(ssl) == 0) {
		if (fd_check_io(SSL_get_rfd(ssl), G_IO_IN) < 0)
#endif //USE_GNUTLS
			return -1;
	}

#ifdef USE_GNUTLS
	ret = SSL_myread(ssl, buf, len);
#else
	ret = SSL_read(ssl, buf, len);
#endif //USE_GNUTLS

	switch ((err = SSL_get_error(ssl, ret))) {
	case SSL_ERROR_NONE:
		return ret;
	case SSL_ERROR_WANT_READ:
	case SSL_ERROR_WANT_WRITE:
		errno = EAGAIN;
		return -1;
	case SSL_ERROR_ZERO_RETURN:
		return 0;
	default:
		g_warning("SSL_read() returned error %d, ret = %d\n", err, ret);
		if (ret == 0)
			return 0;
		return -1;
	}
}
#endif

/* peek at the socket data without actually reading it */
#ifdef USE_SSL
gint ssl_peek(gchar *buf, gint len)
{
	gint err, ret;

#ifdef USE_GNUTLS
	if (SSL_mypending(ssl) == 0) {
	  if (fd_check_io(/*SSL_get_rfd(ssl)*/ssl->rfd, G_IO_IN) < 0)
#else
	if (SSL_pending(ssl) == 0) {
		if (fd_check_io(SSL_get_rfd(ssl), G_IO_IN) < 0)
#endif //USE_GNUTLS

			return -1;
	}

#ifdef USE_GNUTLS
	ret = SSL_mypeek(ssl, buf, len);
#else
	ret = SSL_peek(ssl, buf, len);
#endif //USE_GNUTLS

	switch ((err = SSL_get_error(ssl, ret))) {
	case SSL_ERROR_NONE:
		return ret;
	case SSL_ERROR_WANT_READ:
	case SSL_ERROR_WANT_WRITE:
		errno = EAGAIN;
		return -1;
	case SSL_ERROR_ZERO_RETURN:
		return 0;
	default:
		g_warning("SSL_peek() returned error %d, ret = %d\n", err, ret);
		if (ret == 0)
			return 0;
		return -1;
	}
}
#endif

#ifdef USE_SSL
gint ssl_write(const gchar *buf, gint len)
{
	gint ret;

	ret = SSL_write(ssl, buf, len);

	switch (SSL_get_error(ssl, ret)) {
	case SSL_ERROR_NONE:
		return ret;
	case SSL_ERROR_WANT_READ:
	case SSL_ERROR_WANT_WRITE:
		errno = EAGAIN;
		return -1;
	default:
		return -1;
	}
}
#endif

gint sock_write_all(gint fd, const gchar *buf, gint len, guint ssl_mode)
{
#ifdef USE_SSL

  if(ssl_mode!=SSL_NONE){
    if (ssl)
      return ssl_write_all(buf, len);
    else
      g_return_val_if_fail(ssl != NULL, -1);
  }

#endif
  return fd_write_all(fd, buf, len);
}

gint fd_write_all(gint fd, const gchar *buf, gint len)
{
	gint n, wrlen = 0;

	while (len) {
		n = fd_write(fd, buf, len);
		if (n <= 0)
			return -1;
		len -= n;
		wrlen += n;
		buf += n;
	}

	return wrlen;
}

#ifdef USE_SSL
gint ssl_write_all(const gchar *buf, gint len)
{
	gint n, wrlen = 0;

	while (len) {
	  n = ssl_write(buf, len);
	  if (n <= 0)
	    return -1;
	  len -= n;
	  wrlen += n;
	  buf += n;
	}

	return wrlen;
}
#endif



static gint fd_check_io(gint fd, GIOCondition cond)
{
	struct timeval timeout;
	fd_set fds;
	guint io_timeout=60;
	//SockInfo *sock;

	//sock = sock_find_from_fd(fd);
	//if (sock && !SOCK_IS_CHECK_IO(sock->flags))
	//	return 0;

	timeout.tv_sec  = io_timeout;
	timeout.tv_usec = 0;

	FD_ZERO(&fds);
	FD_SET(fd, &fds);

	if (cond == G_IO_IN) {
		select(fd + 1, &fds, NULL, NULL,
		       io_timeout > 0 ? &timeout : NULL);
	} else {
		select(fd + 1, NULL, &fds, NULL,
		       io_timeout > 0 ? &timeout : NULL);
	}

	if (FD_ISSET(fd, &fds)) {
		return 0;
	} else {
		g_warning("Socket IO timeout\n");
		return -1;
	}
}

#ifdef USE_SSL
static gint set_nonblocking_mode(gint fd, gboolean nonblock)
{
#ifdef USE_WIN32
	gulong val = nonblock ? 1 : 0;

	if (!nonblock)
		WSAEventSelect(fd, NULL, 0);
	if (ioctlsocket(fd, FIONBIO, &val) == SOCKET_ERROR) {
		g_warning("set_nonblocking_mode(): ioctlsocket() failed: %ld\n",
			  WSAGetLastError());
		return -1;
	}

	pop_debug_print("set nonblocking mode to %d\n", nonblock);

	return 0;
#else
	gint flags;

	flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0) {
		perror("fcntl");
		return -1;
	}

	if (nonblock)
		flags |= O_NONBLOCK;
	else
		flags &= ~O_NONBLOCK;

	return fcntl(fd, F_SETFL, flags);
#endif
}
#endif
//from Sylpheed/libsylph-2.4.0






/*============================================================================*/
/* POP3 server reply check                                                    */
/* +OK   : POP_OK returned                                                    */
/* +ERR  : POP_ERROR returned                                                 */
/* other : POP_INVALID returned                                               */
/*============================================================================*/
static int
popIsOK(char *ptr)
{
  int ret = POP_INVALID;

  if( strncmp(ptr, POP_OK_STR, strlen(POP_OK_STR)) == 0 ){
    ret = POP_OK;
  }
  else if( strncmp(ptr, POP_ERR_STR, strlen(POP_ERR_STR)) == 0 ){
    ret = POP_ERROR;
  }

  return(ret);
}



/*============================================================================*/
/* Get APOP Key                                                               */
/*============================================================================*/
static int 
popApopGetKey(char *ptr, char *apop_key)
{

  int  i;
  char *pos;

  if ((pos = (char *)strrchr(ptr, '<'))==NULL) {
    return 1;
  } 

  for (i = 0; i < 128; i++) {
    apop_key[i] = *pos;
    if (*pos == '>') break;
    pos++;
  }

  return 0;
}



/*============================================================================*/
/* POP Line Command                                                           */
/*============================================================================*/
static int
popLineCommand(char *ptr, guint ssl_mode)
{
  int funcret;
  char buffer[POP_MAX_LINE];
  int status;

  funcret = popWriteLine(ptr, ssl_mode);
  if( funcret != 0 ){
    return(1);
  }

  funcret = popReadLine(buffer, POP_MAX_LINE, ssl_mode);
  if( funcret != 0 ){
    return(2);
  }

  status = popIsOK(buffer);
  if( status != POP_OK ){
    return(3);
  }

  return(0);
}


/*============================================================================*/
/* APOP command                                                               */
/*============================================================================*/
static int
popApop(char *user, char *passwd, char *apop_key, guint ssl_mode)
{
  char buffer[POP_MAX_CMD];
  char md5_passwd[MD5_MAX_LEN];

  strcat(apop_key, passwd);

  /*MD5Data(apop_key, strlen(apop_key), md5_passwd);*/
  strcpy(md5_passwd, MD5Digest((unsigned char *)apop_key));

  sprintf(buffer, "APOP %s %s\r\n", user, md5_passwd);
  return( popLineCommand(buffer, ssl_mode) );
}




/*============================================================================*/
/* POP USER command                                                           */
/*============================================================================*/
int
popUser(char *user, guint ssl_mode)
{
  char buffer[POP_MAX_CMD];

  sprintf(buffer, "USER %s\r\n", user);
  return( popLineCommand(buffer,ssl_mode) );
}



/*============================================================================*/
/* POP PASS command                                                           */
/*============================================================================*/
int
popPasswd(char *passwd, guint ssl_mode)
{
  char buffer[POP_MAX_CMD];

  sprintf(buffer, "PASS %s\r\n", passwd);
  return( popLineCommand(buffer, ssl_mode) );
}



/*============================================================================*/
/* POP DELE command                                                           */
/*============================================================================*/
int
popDelete(int no, guint ssl_mode)
{
  int funcret;
  char buffer[POP_MAX_CMD];

  sprintf(buffer, "DELE %d\r\n", no);
  return( popLineCommand(buffer, ssl_mode) );
}



/*============================================================================*/
/* POP RSET command                                                           */
/*============================================================================*/
int
popRset(guint ssl_mode)
{
  int funcret;

  funcret = popLineCommand("RSET\r\n", ssl_mode);
  return(funcret);
}



/*============================================================================*/
/* POP STAT command                                                           */
/*============================================================================*/
int
popStat(int *mail, long *size, guint ssl_mode)
{
  int funcret;
  char buffer[POP_MAX_LINE];
  char p[32];
  char dummy[32];
  char dummy_mail[32];
  char dummy_size[32];
  int status;

  funcret = popWriteLine("STAT\r\n",ssl_mode);
  if( funcret != 0 ){
    return(1);
  }

  funcret = popReadLine(buffer, POP_MAX_LINE,ssl_mode);
  if( funcret != 0 ){
    return(2);
  }

  status = popIsOK(buffer);
  if( status != POP_OK ){
    return(3);
  }

  /* "+OK <Number> <Size>" */
  sscanf(buffer, "%s %s %s", dummy, dummy_mail, dummy_size);
  *mail = atoi(dummy_mail);
  *size = atol(dummy_size);

  return(0);
}



/*============================================================================*/
/* POP UIDL command                                                           */
/*============================================================================*/
int
popUidl(int no, char *uniqid, int size, guint ssl_mode)
{
  int funcret;
  char buffer[POP_MAX_LINE];
  char dummy[32];
  int  dummyno;
  int status;

  if( size >= POP_MAX_UIDL+1 ){
    return(4);
  }

  sprintf(buffer, "UIDL %d\r\n", no);
  funcret = popWriteLine(buffer,ssl_mode);
  if( funcret != 0 ){
    return(1);
  }

  funcret = popReadLine(buffer, POP_MAX_LINE,ssl_mode);
  if( funcret != 0 ){
    return(2);
  }

  status = popIsOK(buffer);
  if( status != POP_OK ){
    return(3);
  }

  /* "+OK <Number> <uniq-id(32byte)>" */
  sscanf(buffer, "%s %d %s", dummy, &dummyno, uniqid);

  return(0);
}



/*============================================================================*/
/* POP LIST command                                                           */
/*============================================================================*/
int
popList(PopList **list, int *size, guint ssl_mode)
{
  int funcret;
  char buffer[POP_MAX_LINE];
  int status;
  int cnt;

  funcret = popWriteLine("LIST\r\n",ssl_mode);
  if( funcret != 0 ){
    return(funcret);
  }

  memset(list, 0, sizeof(PopList)*(*size));
  memset(buffer, 0, POP_MAX_LINE);
  cnt = 0;
  while( 1 ){
    funcret = popReadLine(buffer, POP_MAX_LINE,ssl_mode);
    if( funcret != 0 ){
      return(funcret);
    }
    if( cnt >= *size ){
      break;
    }
    if( strcmp(buffer, ".\r\n") != 0 ){
      break;
    }
    sscanf(buffer, "%d %d", &((*list+cnt)->no), &((*list+cnt)->size));
    cnt++;
  }

  *size = cnt;

  return(0);
}



/*============================================================================*/
/* POP RETR command                                                           */
/*============================================================================*/
int
popRetr(int no, FILE *fp, guint ssl_mode)
{
  int funcret;
  char buffer[POP_MAX_LINE];
  int status;
  int cnt;

  sprintf(buffer, "RETR %d\r\n", no);
  funcret = popWriteLine(buffer,ssl_mode);
  if( funcret != 0 ){
    return(funcret);
  }

  while( 1 ){
    funcret = popReadLine(buffer, POP_MAX_LINE,ssl_mode);
    if( funcret != 0 ){
      return(funcret);
    }
    if( strcmp(buffer, ".\r\n") == 0 ){
      break;
    }
    if( fputs(buffer, fp) == EOF ){
      return(4);
    }
    fflush(fp);
  }

  return(0);
}



/*============================================================================*/
/* POP TOP command                                                            */
/*============================================================================*/
int
popTop(int no, int line,  FILE *fp, guint ssl_mode)
{
  int funcret;
  char buffer[POP_MAX_LINE];
  int status;
  int cnt;

  sprintf(buffer, "TOP %d %d\r\n", no, line);
  funcret = popWriteLine(buffer,ssl_mode);
  if( funcret != 0 ){
    return(funcret);
  }

  while( 1 ){
    funcret = popReadLine(buffer, POP_MAX_LINE,ssl_mode);
    if( funcret != 0 ){
      return(funcret);
    }
    if( strcmp(buffer, ".\r\n") == 0 ){
      break;
    }
    if( fputs(buffer, fp) == EOF ){
      return(4);
    }
  }

  return(0);
}

/*============================================================================*/
/* POP NOOP command                                                           */
/*============================================================================*/
int
popNoop(guint ssl_mode)
{
  int funcret;

  funcret = popLineCommand("NOOP\r\n", ssl_mode);
  return(funcret);
}


/*============================================================================*/
/* POP STLS command                                                           */
/*============================================================================*/
#ifdef USE_SSL
int
popSTLS(guint ssl_mode)
{
  int funcret;

  funcret = popLineCommand("STLS\r\n", ssl_mode);
  return(funcret);
}
#endif


/*============================================================================*/
/* POP QUIT command                                                           */
/*============================================================================*/
int
popQuit(guint ssl_mode)
{
  int funcret;

  funcret = popWriteLine("QUIT\r\n",ssl_mode);
  return(funcret);
}


/*============================================================================*/
/* Connect POP server                                                         */
/*============================================================================*/
int
popConnect(char *server, int port, char *apop_key, guint ssl_mode, gboolean ssl_nonblock,
	   gint ssl_cert_res, gchar **subject, gchar **issue, glong *verify)
{
  int    funcret;
  struct hostent *host;
  struct sockaddr_in pop_addr;
  char   buffer[POP_MAX_LINE];
  int    status;
  int    true;
  gulong nonblock=0;

  /*--------------------------------------------------------------------------*/
  /* Get hostent data from server name                                        */
  /*--------------------------------------------------------------------------*/
  host = gethostbyname(server);
  if( host == NULL ){
    return(1);
  }

  /*--------------------------------------------------------------------------*/
  /* Create socket                                                            */
  /*--------------------------------------------------------------------------*/
  Sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if( Sockfd == -1 ){
    return(2);
  }

#ifdef USE_WIN32
  {
    long x=0;
    setsockopt(Sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&x, sizeof(x));
    setsockopt(Sockfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&x, sizeof(x));
  }
  
  /*  if(ioctlsocket(Sockfd, FIONBIO, &nonblock) == SOCKET_ERROR){
    fprintf(stderr,"ioctlsocket() : failed.\n");
    fflush(stderr);
    return(2);
  }
  else{
    fprintf(stderr,"ioctlsocket() : succeeded.\n");
    fflush(stderr);
    }*/
#endif

  /*--------------------------------------------------------------------------*/
  /* Connect POP server                                                       */
  /*--------------------------------------------------------------------------*/
  pop_addr.sin_family = AF_INET;
  pop_addr.sin_port = htons(port);
  pop_addr.sin_addr.s_addr = *(long *)(host->h_addr_list[0]);

  funcret = connect(Sockfd, (struct sockaddr *)&pop_addr, sizeof(struct sockaddr_in));
  if( funcret == -1 ){
    return(3);
  }

  /*--------------------------------------------------------------------------*/
  /* Associate FILE pointer                                                   */
  /*--------------------------------------------------------------------------*/
#ifndef USE_WIN32
  //  Rfp = (FILE *)fdopen(Sockfd, "r");
  //  Wfp = (FILE *)fdopen(Sockfd, "w");
  //  if( Rfp == NULL || Wfp == NULL ){
  //    popClose();
  //    return(4);
  //}
#endif


#ifdef USE_SSL
  if(ssl_mode==SSL_TUNNEL){
    set_nonblocking_mode(Sockfd, FALSE);
    ssl=ssl_init_socket(Sockfd, server, ssl_cert_res, subject, issue, verify);
    if (!ssl) {
      if(ssl_cert_res==SSL_CERT_NONE){
	popClose();

	pop_debug_print ("Initializing SSL certification\n");
	pop_debug_print ("SSL Cert Sub : %s\n",*subject);
	pop_debug_print ("SSL Cert Iss : %s\n",*issue);
	return(-(30+4));
      }
      else{
	g_warning("can't initialize SSL.");
	popClose();
	return(30+4);
      }
    }
#ifdef USE_GNUTLS
    set_nonblocking_mode(Sockfd, /*ssl_nonblock*/FALSE);
#else
    set_nonblocking_mode(Sockfd, ssl_nonblock);
#endif //USE_GNUTLS
  }
#endif


  /*--------------------------------------------------------------------------*/
  /* Read initial message from pop server.                                    */
  /*--------------------------------------------------------------------------*/
  funcret = popReadLine(buffer, POP_MAX_LINE, ssl_mode);
  if( funcret != 0 ){
    return(5);
  }

  /*--------------------------------------------------------------------------*/
  /* Confirm initial message.                                                 */
  /*--------------------------------------------------------------------------*/
  status = popIsOK(buffer);
  if( status != POP_OK ){
    return(6);
  }

  /*--------------------------------------------------------------------------*/
  /* SSL STARTTLS message.                                                    */
  /*--------------------------------------------------------------------------*/
#ifdef USE_SSL
  if(ssl_mode==SSL_STARTTLS){
    funcret = popSTLS(ssl_mode);
    if( funcret != 0 ){
      return(6);
    }
    {
      //gboolean nb_mode;
      //nb_mode = sock_is_nonblocking_mode(session->sock);
      set_nonblocking_mode(Sockfd, FALSE);
      
      ssl=ssl_init_socket_with_method(Sockfd, server, ssl_cert_res, 
				      subject, issue, verify, SSL_METHOD_TLSv1);
      if(!ssl){
	//g_warning("can't start TLS session.\n");
	//if (nb_mode)
	//  sock_set_nonblocking_mode(session->sock, TRUE);
	if(ssl_cert_res==SSL_CERT_NONE){
	  popClose();
	  
	  pop_debug_print ("SSL Cert Sub : %s\n",*subject);
	  pop_debug_print ("SSL Cert Iss : %s\n",*issue);
	  return(-(30+4));
	}
	else{
	  popClose();
	  return (30+6);
	}
      }
      
      set_nonblocking_mode(Sockfd, ssl_nonblock);
    }

  }

#endif


  /*--------------------------------------------------------------------------*/
  /* Get apop auth key.                                                       */
  /*--------------------------------------------------------------------------*/
  if(Mascot->mail.type==MAIL_APOP){
    funcret = popApopGetKey(buffer, apop_key);
    if( funcret != 0 ){
      return(7);
    }
  }

 END_FUNC:
  return(0);
}



/*============================================================================*/
/* Login pop server                                                           */
/*============================================================================*/
int
popLogin(char *user, char *pass, char *apop_key, guint ssl_mode)
{
  int funcret;

  if(Mascot->mail.type==MAIL_APOP){
  /*------------------------------------------------------------------------*/
  /* Try to APOP login                                                      */
  /*------------------------------------------------------------------------*/
    funcret = popApop(user, pass, apop_key, ssl_mode);
    if( funcret == 0 ){
      return(0);
    }
  }
  else if(Mascot->mail.type==MAIL_POP3){
  /*------------------------------------------------------------------------*/
  /* Try to Normal Login                                                    */
  /*------------------------------------------------------------------------*/
    funcret = popUser(user,ssl_mode);
    if( funcret != 0 ){
      return(funcret+10);
    }
    funcret = popPasswd(pass,ssl_mode);
    if( funcret != 0 ){
      return(funcret+20);
    }
  }

  return(0);
}



/*============================================================================*/
/* Check mail number                                                          */
/*============================================================================*/
int
popCheckMailNum(int *number, char *uidl, int *num, guint ssl_mode)
{
  int  funcret;
  int  s, e;
  long size;
  char chk_uidl[POP_MAX_UIDL+1];

  /*------------------------------------------------------------------------*/
  /* Get Total Mail Number                                                  */
  /*------------------------------------------------------------------------*/
  funcret = popStat(&e, &size, ssl_mode);
  if( funcret != 0 ){
    return(funcret+10);
  }

  if( e == 0 || uidl == NULL ){
    *number = *num = e;
    return(0);
  }

  /*------------------------------------------------------------------------*/
  /* Check UIDL                                                             */
  /*------------------------------------------------------------------------*/
  for( s = e; s > 0; s-- ){
    funcret = popUidl(s, chk_uidl, POP_MAX_UIDL, ssl_mode);
    if( funcret == 2 ){ /* -ERR Humm. May be not support UIDL. */
      s = e;
      break;
    }
    else if( funcret != 0 ){
      return(funcret+20);
    }
    if( strcmp(uidl, chk_uidl) == 0 ){
      break;
    }
  }
  if( s == 0 ){
    *number = e;
  }
  else {
    *number = e - s;
  }

  *num = e;
  funcret = popUidl(e, uidl, POP_MAX_UIDL, ssl_mode);

  return(0);
}



/*============================================================================*/
/* Get mail                                                                   */
/*============================================================================*/
int
popGetMail(int number, FILE *fp, char *uidl, guint ssl_mode)
{
  int funcret;
  char chk_uidl[POP_MAX_UIDL+1];

  funcret = popRetr(number, fp, ssl_mode);
  if( funcret != 0 ){
    return(funcret + 10);
  }

  if( uidl != NULL ){
    funcret = popUidl(number, chk_uidl, POP_MAX_UIDL, ssl_mode);
    if( funcret == 2 ){ /* -ERR Humm. May be not support UIDL. */
      uidl[0] = '\0';
    }
    else if( funcret != 0 ){
      return(funcret+20);
    }
    else {
      strcpy(uidl, chk_uidl);
    }
  }

  return(0);
}



/*============================================================================*/
/* Disconnect POP3 server                                                     */
/*============================================================================*/
int
popClose()
{
#ifndef USE_WIN32
  //fclose(Rfp);
  //fclose(Wfp);
#endif
  shutdown(Sockfd, 0);
#ifdef USE_WIN32
  closesocket(Sockfd);
#else
  close(Sockfd);
#endif
#ifdef USE_SSL
  ssl_done_socket(ssl);
#endif
  return(0);
} 



/*============================================================================*/
/* Disconnect POP server                                                      */
/*============================================================================*/
int
popDisconnect(guint ssl_mode)
{
  (void)popQuit(ssl_mode);
  (void)popClose();
  return(0);
}

#endif // USE_BIFF
