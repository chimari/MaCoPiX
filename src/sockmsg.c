//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//
//
//      sockmsg.c
//      Management of Socket message function
//
//                            Copyright 2002-2008  K.Chimari
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

#include "main.h"

#ifdef USE_SOCKMSG

#include <glib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <netdb.h>
#include <ctype.h>

gchar *cut_spc(gchar * obj_name);
static gchar *get_socket_name(void);
static gchar *get_duet_name(gchar *mascotname);
static gint fd_open_unix(const gchar *path);
static gint fd_connect_unix(const gchar *path);
static gint fd_accept(gint sock);
static gint fd_close(gint fd);
static gboolean io_callback(GIOChannel * source,
			    GIOCondition condition, gpointer data);
static gboolean io_callback_sv(GIOChannel * source,
			       GIOCondition condition, gpointer data);
gchar* set_typing_msg();
gchar* my_strbuf();


gchar *cut_spc(gchar * obj_name){
  gchar *tgt_name=NULL, *ret_name, *c;
  gint  i_bak,i;
                                                                               
  tgt_name=g_strdup(obj_name);
  i_bak=strlen(tgt_name)-1;
  while((tgt_name[i_bak]==0x20)
        ||(tgt_name[i_bak]==0x0A)
        ||(tgt_name[i_bak]==0x0D)
        ||(tgt_name[i_bak]==0x09)){
    //tgt_name[i_bak]=(char)NULL;
    tgt_name[i_bak]=0x00;
    i_bak--;
  }
                                                                                
  c=tgt_name;
  i=0;
  while((tgt_name[i]==0x20)||(tgt_name[i]==0x09)){
    c++;
    i++;
  }

  return(tgt_name);
}


static gchar *get_socket_name(void)
{
  static gchar *filename = NULL;
  gchar *dirname = NULL;

  dirname = g_strdup_printf("%s%cmacopix-%d",
			    g_get_tmp_dir(), G_DIR_SEPARATOR, getuid());
  if (access(dirname, F_OK) != 0) {
    mkdir(dirname,(S_IRWXU|S_IRGRP|S_IROTH|S_IXGRP|S_IXOTH));
  }
  g_free(dirname);

  if (filename == NULL) {
    filename = g_strdup_printf("%s%cmacopix-%d%cmacopix",
			       g_get_tmp_dir(), G_DIR_SEPARATOR,
			       getuid(), G_DIR_SEPARATOR);
  }
  
  return filename;
}

static gchar *get_duet_name(gchar *mascotname)
{
  static gchar *filename = NULL;
  gchar *dirname = NULL;

  dirname = g_strdup_printf("%s%cmacopix-%d",
			    g_get_tmp_dir(), G_DIR_SEPARATOR, getuid());
  if (access(dirname, F_OK) != 0) {
    mkdir(dirname,(S_IRWXU|S_IRGRP|S_IROTH|S_IRGRP|S_IROTH));
  }
  g_free(dirname);
  
  filename = g_strdup_printf("%s%cmacopix-%d%cmacopix-%s",
			     g_get_tmp_dir(), 
			     G_DIR_SEPARATOR,
			     getuid(),
			     G_DIR_SEPARATOR,
			     mascotname);
  
  return filename;
}

static gint fd_open_unix(const gchar *path)
{
	gint sock;
	struct sockaddr_un addr;

	sock = socket(PF_UNIX, SOCK_STREAM, 0);

	if (sock < 0) {
		perror("sock_open_unix(): socket");
		return -1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);

	if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind");
		close(sock);
		return -1;
	}

	if (listen(sock, 1) < 0) {
		perror("listen");
		close(sock);
		return -1;		
	}

	return sock;
}

static gint fd_connect_unix(const gchar *path)
{
	gint sock;
	struct sockaddr_un addr;

	sock = socket(PF_UNIX, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("sock_connect_unix(): socket");
		return -1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);

	if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		close(sock);
		return -1;
	}

	return sock;
}

static gint fd_accept(gint sock)
{
	struct sockaddr_in caddr;
	gint caddr_len;

	caddr_len = sizeof(caddr);
	return accept(sock, (struct sockaddr *)&caddr, &caddr_len);
}

static gint fd_close(gint fd)
{
	return close(fd);
}

static gboolean io_callback(GIOChannel * source,
		  GIOCondition condition, gpointer data)
{
  gchar buf[2048],tmp[10];
  gint fd, readsize;
  typMascot *mascot = data;
  gint step,mode;
  
  fd = fd_accept(g_io_channel_unix_get_fd(source));
  
  readsize = read(fd, buf, 2047);
  buf[readsize] = '\0';
  
  fd_close(fd);
  
  // Expiration
  strncpy(tmp,buf,8);
  mascot->sockmsg_expire=(gint)(atoi(tmp)/INTERVAL);
  if(mascot->sockmsg_expire<=0){
    mascot->sockmsg_expire=mascot->sockmsg_expire_def/INTERVAL;
  }
  
  // Stepping or Lump Sum?
  strncpy(tmp,buf+8,2);
  mode=(gint)atoi(tmp);
  if(mode<0){
    mode=mascot->sockmsg_type;
  }
  
  // Stepping Interval
  strncpy(tmp,buf+8+2,4);
  step=(gint)atoi(tmp);
  if(step<0){
    step=mascot->sockmsg_step;
  }
  
  
  mascot->balseq=0;
  mascot->bal_mode=BALLOON_SOCKMSG;
  if (mascot->sockmsg != NULL)
    g_free(mascot->sockmsg);
  if(mode==SOCK_STEPPING){
    mascot->sockmsg = set_typing_msg(buf+8+2+4,step);
  }
  else{
    mascot->sockmsg = g_strdup(buf+8+2+4);
  }
  DoBalloon(mascot);
  flag_balloon=TRUE;

  return(TRUE);
}

static gboolean io_callback_sv(GIOChannel * source,
		  GIOCondition condition, gpointer data)
{
	gchar buf[2048],tmp[8];
	gint fd, readsize;
	typMascot *mascot = data;
	gint i_ptn,i_frm;
	gint mode;

	fd = fd_accept(g_io_channel_unix_get_fd(source));

	readsize = read(fd, buf, 2047);
	buf[readsize] = '\0';

	fd_close(fd);
	
	if(readsize<4) return(FALSE);
	
	strncpy(tmp,buf,2);
	mode=atoi(tmp);
	strncpy(tmp,buf+2,2);
	i_ptn=atoi(tmp);

	if(mode==DUET_CLICK){
	  if (mascot->sockmsg != NULL)
	    g_free(mascot->sockmsg);
	  mascot->sockmsg=g_strdup(buf+2+2);
	}
	
	
	// クリックアニメ
	if((mode==DUET_CLICK)&&(mascot->duet_use_click)){
	  if(mascot->frame_pix[i_ptn][0]!=-1){
	    for(i_frm=0;i_frm<mascot->frame_num[mascot->anime_ptn];i_frm++){
	      // ブロックループのキャンセル
	      mascot->frame_loop[mascot->anime_ptn][i_frm].seq=0;
	    }
	    //mascot->anime_ptn=weight_click(mascot);
	    mascot->anime_ptn=i_ptn;
	    mascot->anime_frm=-1;
	    mascot->anime_seq=-1;
	    mascot->anime_seqend=
	      RANDOM(mascot->frame_max[mascot->anime_ptn][mascot->anime_frm]
		     -mascot->frame_min[mascot->anime_ptn][mascot->anime_frm]+1)
	      +mascot->frame_min[mascot->anime_ptn][mascot->anime_frm];
	    
	    sound_play(mascot,mascot->click_sound[mascot->anime_ptn]);

	    if(mascot->sockmsg){
	      if(strlen(mascot->sockmsg)>0){
		mascot->balseq=0;
		mascot->bal_mode=BALLOON_DUET;
		DoBalloon(mascot); 
		flag_balloon=TRUE;
	      }
	    }
	  }
	}
	else if (mascot->duet_use_random){
	  if(mascot->anime_ptn==0){ // ランダムアニメ
	    if(mascot->frame_pix[i_ptn][0]!=-1){
	      mascot->anime_ptn=i_ptn;
	      mascot->anime_frm=0;
	    }
	  }
	}
	return(TRUE);
}


gint sock = -1;
gint io_watch = -1;

SockMsgInitResult sockmsg_init() {
	gchar *sockname;
	SockMsgInitResult result;

	sockname = get_socket_name();

	sock = fd_connect_unix(sockname);
	if (sock < 0) {
		unlink(sockname);
		sock = fd_open_unix(sockname);
		if (sock < 0)
			result = SOCKMSG_ERROR;
		else
			result = SOCKMSG_CREATED;
	} else {
		result = SOCKMSG_OPENED;
	}

	return result;
}

gint duet_sv = -1;
gint io_watch_sv = -1;

SockMsgInitResult duet_sv_init(gchar *mascotname) {
  gchar *sockname;
  SockMsgInitResult result;
  
  sockname = get_duet_name(mascotname);
  
  duet_sv = fd_connect_unix(sockname);
  if (duet_sv < 0) {
    unlink(sockname);
    duet_sv = fd_open_unix(sockname);
    if (duet_sv < 0){
      //fd_close(duet_sv);
      result = SOCKMSG_ERROR;
    }
    else
      result = SOCKMSG_CREATED;
  } else {
    // Already Opened / Closeして再オープンする。
    fd_close(duet_sv);
    unlink(sockname);
    duet_sv = fd_open_unix(sockname);
    result = SOCKMSG_OPENED;
  }
  
  return result;
}

gint duet_cl = -1;

SockMsgInitResult duet_cl_init(gchar *mascotname) {
  gchar *sockname;
  SockMsgInitResult result;
  
  sockname = get_duet_name(mascotname);
  
  duet_cl = fd_connect_unix(sockname);
  if (duet_cl < 0) {
    //fd_close(duet_cl);
    result = SOCKMSG_ERROR;
  } else {
    result = SOCKMSG_OPENED;
  }
  
  return result;
}

void sockmsg_set_mascot(typMascot *mascot)
{
	GIOChannel *io;

	io = g_io_channel_unix_new(sock);
	io_watch = g_io_add_watch(io, G_IO_IN | G_IO_ERR | G_IO_HUP | G_IO_PRI, io_callback, mascot);
	g_io_channel_unref(io);
}

void duet_set_mascot(typMascot *mascot)
{
	GIOChannel *io;

	io = g_io_channel_unix_new(duet_sv);
	io_watch_sv = g_io_add_watch(io, G_IO_IN | G_IO_ERR | G_IO_HUP | G_IO_PRI, io_callback_sv, mascot);
	g_io_channel_unref(io);
}

void sockmsg_send_msg(gchar *msg)
{
        msg = g_locale_to_utf8(msg,-1,NULL,NULL,NULL);
        if(!msg)
    		_("(Invalid Character Code)");

	if (write(sock, msg, strlen(msg)) <0){
	  printf("Error : sockmsg_send_msg();\n");
	  return;
	}
}

void duet_send_msg(gchar *msg)
{
        msg = g_locale_to_utf8(msg,-1,NULL,NULL,NULL);
        if(!msg)
    		_("(Invalid Character Code)");

	if (write(duet_cl, msg, strlen(msg)) <0){
	  printf("Error : duet_send_msg();\n");
	  return;
	}
	
}

void sockmsg_done()
{
	gchar *sockname;

	if (io_watch != -1)
		g_source_remove(io_watch);
        fd_close(sock);

        sockname = get_socket_name();
        unlink(sockname);
}

void duet_sv_done(gchar *mascotname, gboolean flag_close)
{
	gchar *sockname;

	if (io_watch_sv != -1)
		g_source_remove(io_watch_sv);
        fd_close(duet_sv);

	if(flag_close) {
	  sockname = get_duet_name(mascotname);
	  unlink(sockname);
	}
}

void duet_cl_done(gchar *mascotname, gboolean flag_close)
{
	gchar *sockname;

        fd_close(duet_cl);

	if(flag_close) {
	  sockname = get_duet_name(mascotname);
	  unlink(sockname);
	}
}

gchar* set_typing_msg(gchar* input, gint interval){
  gchar *p, *p1, *ret;
  gint i,j,num;
  
  p = input;
  ret=my_strbuf(NULL);

  while (*p) {
    gunichar ch, ch1;
    gchar tmp[64];

    ch= g_utf8_get_char (p);
    
    if (ch == '%'){
      num=0;
      i=0;
      p1=p;
      for(;;){
	p1=g_utf8_next_char (p1);
	ch1= g_utf8_get_char (p1);
	if((ch1 == 'c')||(ch1 == 'a')||(ch1 == 'n')||(ch1 == 'p')){
	  i++;
	  break;
	}
	else if(isdigit(ch1)){
	  i++;
	}
	else{
	  i=0;
	  break;
	}
      }

      if(i!=0){
	ret=my_strbuf("%");
	for(j=0;j<i;j++){
	  p = g_utf8_next_char (p);
	  g_utf8_strncpy(tmp,p,1);
	  ret=my_strbuf(tmp);
	}
      }
      else{
	sprintf(tmp,"%%%%%%%dc",interval);
	ret=my_strbuf(tmp);
      }
    }
    else{
      g_utf8_strncpy(tmp,p,1);
      ret=my_strbuf(tmp);
      sprintf(tmp,"%%%dc",interval);
      ret=my_strbuf(tmp);
    }
    
    
    p = g_utf8_next_char (p);
  }
  //fprintf(stderr,"%s\n",ret);
  //fflush(stderr);

  return(ret);
}

gchar* my_strbuf(gchar *p)
{
  static gchar    *start = NULL;
  static gchar    *next;
  static int      len, len2, l;
  
  if (p == NULL) {
    len = len2 = 4096;
    start = next = (gchar *) g_malloc(len + 1);
    start[0] = '\0';
  } else if (start != NULL) {
    l = strlen(p);
    while (len - l < 0) {
      gchar           *p;
      len2 += 256;
      len += 256;
      p = (gchar *) g_realloc(start, len2 + 1);
      start = p;
      next = p + len2 - len;
                }
    strcpy(next,p);
    len -= l;
    next += l;
  }
  return start;
}

#endif  // USE_SOCKMSG

