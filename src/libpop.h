/*******************************************************************************
 * POP3 Library Header File.
 * COPYRIGHT(C) 1999-2001 TKS
 * 
 * $Revision: 1.3 $
 * $Date: 2001/01/31 16:03:54 $
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
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02111-1301, USA.
 *
 ******************************************************************************/

#ifndef _LIBPOP_H_
#define _LIBPOP_H_

#define POP_FIELD_VALUE_LENGTH    1024
#define POP_OK_STR   "+OK"
#define POP_ERR_STR  "-ERR"
#define POP_OK 0
#define POP_ERROR 1
#define POP_INVALID 2
#define POP_MAX_LINE 1024
#define POP_MAX_CMD 512
#define POP_MAX_UIDL 32

#define MD5_MAX_LEN 33


typedef struct {
  int no;
  int size;
} PopList;


int popUser(char *user, guint ssl_mode);
int popPasswd(char *passwd, guint ssl_mode);
int popDelete(int no, guint ssl_mode);
int popRset(guint ssl_mode);
int popStat(int *mail, long *size, guint ssl_mode);
int popUidl(int no, char *uniqid, int size, guint ssl_mode);
int popList(PopList **list, int *size, guint ssl_mode);
int popRetr(int no, FILE *fp, guint ssl_mode);
int popTop(int no,int line,  FILE *fp, guint ssl_mode);
#ifdef USE_SSL
int popSTLS(guint ssl_mode);
#endif
int popNoop(guint ssl_mode);
int popQuit(guint ssl_mode);
int popConnect(char *server, 
	       int port, 
	       char *apop_key, 
	       guint ssl_mode, 
	       gboolean ssl_nonblock, 
	       gint ssl_cert_res,
	       gchar **subject,
	       gchar **issue,
	       glong *verify);
int popLogin(char *user, char *pass, char *apop_key, guint ssl_mode);
int popCheckMailNum(int *number, char *uidl, int *num, guint ssl_mode);
int popGetMail(int number, FILE *fp, char *uidl, guint ssl_mode);
int popClose();
int popDisconnect(guint ssl_mode);

#endif /* _LIBPOP_H_ */
