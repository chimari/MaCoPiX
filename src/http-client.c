//    MaCoPiX : Mascot Constructive Pilot for X
//      http-client.c : Access to HTTP
//                                           2019.5 K. Chimari

#include "main.h"

#ifdef USE_WIN32
#include <ws2tcpip.h>
#define BUF_LEN 65535             /* バッファのサイズ */
#else
#define BUF_LEN 65535             /* バッファのサイズ */
#endif

#define BUFFSIZE 65535

void check_msg_from_parent();
static gint fd_check_io();
//gint fd_recv();
//gint fd_gets();
char *read_line();
void read_response();
gint fd_write();
void write_to_server();
#ifdef USE_SSL
void write_to_SSLserver();
#endif
void error();
void PortReq();
//int sftp_c();
//int sftp_get_c();
//int ftp_c();

#ifdef USE_WIN32
unsigned __stdcall http_c_nonssl();
#ifdef USE_SSL
unsigned __stdcall http_c_ssl();
#endif
#else
int http_c_nonssl();
#ifdef USE_SSL
int http_c_ssl();
#endif
#endif

void unchunk();


#ifdef USE_SSL
gint ssl_gets();
gint ssl_read();
gint ssl_peek();
gint ssl_write();
#endif

gboolean progress_timeout();
#ifndef USE_WIN32
void httpdl_signal();
#endif

void dl_mascot_list();

glong get_file_size();
void write_dlsz();
void unlink_dlsz();
glong get_dlsz();

static void cancel_http();

#ifdef POP_DEBUG
gboolean debug_flg=TRUE;
#else
gboolean debug_flg=FALSE;
#endif

void check_msg_from_parent(){
#ifdef USE_WIN32
  MSG msg;
  
  PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);

  if(msg.message==WM_QUIT) {
    fprintf(stderr,"Terminated from parent.\n");
    gtk_main_quit();
    _endthreadex(0);
  }
#endif
}

static gint fd_check_io(gint fd, GIOCondition cond)
{
	struct timeval timeout;
	fd_set fds;
	guint io_timeout=60;

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

/*
gint fd_recv(gint fd, gchar *buf, gint len, gint flags)
{
  gint ret;
  
  if (fd_check_io(fd, G_IO_IN) < 0)
    return -1;

  ret = recv(fd, buf, len, flags);
#ifdef USE_WIN32
  if (ret == SOCKET_ERROR) {
    fprintf(stderr,"Recv() Error TimeOut...  %d\n",WSAGetLastError());
  }
#endif
  return ret;
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
*/

/*--------------------------------------------------
 * ソケットから1行読み込む
 */
char *read_line(int socket, char *p){
    char *org_p = p;

    while (1){
        if ( read(socket, p, 1) == 0 ) break;
        if ( *p == '\n' ) break;
        p++;
    }
    *(++p) = '\0';
    return org_p;
}


/*--------------------------------------------------
 * レスポンスを取得する。^\d\d\d- ならもう1行取得
 */
void read_response(int socket, char *p){
  int ret;
    do { 
      //read_line(socket, p);
    ret=fd_gets(socket,p,BUF_LEN);
        if ( debug_flg ){
	  fprintf(stderr, "<-- %s", p);fflush(stderr);
        }
    } while ( isdigit(p[0]) &&
	      isdigit(p[1]) && 
	      isdigit(p[2]) &&
	      p[3]=='-' );

}

/*
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
      fprintf(stderr,"last error = %d\n", err);
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
*/

/*--------------------------------------------------
 * 指定されたソケット socket に文字列 p を送信。
 * 文字列 p の終端は \0 で terminate されている
 * 必要がある
 */

void write_to_server(int socket, char *p){
    if ( debug_flg ){
        fprintf(stderr, "--> %s", p);fflush(stderr);
    }
    
    fd_write(socket, p, strlen(p));
}

#ifdef USE_SSL
void write_to_SSLserver(SSL *ssl, char *p){
  if ( debug_flg ){
    fprintf(stderr, "[SSL] <-- %s", p);fflush(stderr);
  }
  
  ssl_write(ssl, p, strlen(p));
}
#endif

void error( char *message ){
  fprintf(stderr, "%s\n", message);
    exit(1);
}

void PortReq(char *IPaddr , int *i1 , int *i2 , int *i3 , int *i4 , int *i5 , int *i6)
{
  int j ;
  char *ip ;
  IPaddr = IPaddr + 3 ;

  //printf("aaa %s \n",IPaddr);

  while( isdigit(*IPaddr) == 0 ) { IPaddr++ ; }

  ip = strtok(IPaddr,",");
  *i1 = atoi(ip) ;

  ip = strtok(NULL,",");
  *i2 = atoi(ip) ;

  ip = strtok(NULL,",");
  *i3 = atoi(ip) ;

  ip = strtok(NULL,",");
  *i4 = atoi(ip) ;

  ip = strtok(NULL,",");
  *i5 = atoi(ip) ;

  ip = strtok(NULL,",");

  j = 0 ;
  while ( isdigit(*(ip +j)) != 0 ) { j += 1 ; }
  ip[j] = '\0' ;
  *i6 = atoi(ip) ;
}



int get_mascot_list(typMascot *mascot){
#ifdef USE_WIN32
  DWORD dwErrorNumber;

  mascot->hThread_http = (HANDLE)_beginthreadex(NULL,0,
						http_c_nonssl,
						(LPVOID)mascot,
						0,
						&mascot->dwThreadID_http);
  if (mascot->hThread_http == NULL) {
    dwErrorNumber = GetLastError();
    fprintf(stderr,"_beginthreadex() error(%ld).\n", dwErrorNumber);
  }
  else{
    CloseHandle(mascot->hThread_http);
  }
  mascot->dwThreadID_http=0;
#else

  waitpid(http_pid,0,WNOHANG);

  if( (http_pid = fork()) <0){
    fprintf(stderr,"fork error\n");
  }
  else if(http_pid ==0) {
    http_c_nonssl(mascot);
    kill(getppid(), SIGHTTPDL);  //calling dss_signal
    _exit(1);
  }
#endif

  return 0;
}

void unchunk(gchar *dss_tmp){
  FILE *fp_read, *fp_write;
  gchar *unchunk_tmp;
  gchar cbuf[BUFFSIZE];
  gchar *dbuf=NULL;
  gchar *cpp;
  gchar *chunkptr, *endptr;
  long chunk_size;
  gint i, read_size=0, crlf_size=0;
  
  if ( debug_flg ){
    fprintf(stderr, "Decoding chunked file \"%s\".\n", dss_tmp);fflush(stderr);
  }
  
  fp_read=fopen(dss_tmp,"r");
  unchunk_tmp=g_strconcat(dss_tmp,"_unchunked",NULL);
  fp_write=fopen(unchunk_tmp,"wb");
  
  while(!feof(fp_read)){
    if(fgets(cbuf,BUFFSIZE-1,fp_read)){
      cpp=cbuf;
      
      read_size=strlen(cpp);
      for(i=read_size;i>=0;i--){
	if(isalnum(cpp[i])){
	  crlf_size=read_size-i-1;
	  break;
	}
	else{
	  cpp[i]='\0';
	}
      }
      chunkptr=g_strdup_printf("0x%s",cpp);
      chunk_size=strtol(chunkptr, &endptr, 0);
      g_free(chunkptr);
      
      if(chunk_size==0) break;
      
      if((dbuf = (gchar *)g_malloc(sizeof(gchar)*(chunk_size+crlf_size+1)))==NULL){
	fprintf(stderr, "!!! Memory allocation error in unchunk() \"%s\".\n", dss_tmp);
	fflush(stderr);
	break;
      }
      if(fread(dbuf,1, chunk_size+crlf_size, fp_read)){
	fwrite( dbuf , chunk_size , 1 , fp_write ); 
	if(dbuf) g_free(dbuf);
      }
      else{
	break;
      }
    }
  }
  
  fclose(fp_read);
  fclose(fp_write);
  
  unlink(dss_tmp);
  
  rename(unchunk_tmp,dss_tmp);
  
  g_free(unchunk_tmp);
}

/*
#ifdef USE_SSL
 gint ssl_gets(SSL *ssl, gchar *buf, gint len)
{
  gchar *newline, *bp = buf;
  gint n;
  gint i;
  
  if (--len < 1)
    return -1;
  do {
    if ((n = ssl_peek(ssl, bp, len)) <= 0)
	return -1;
    if ((newline = memchr(bp, '\n', n)) != NULL)
      n = newline - bp + 1;
    if ((n = ssl_read(ssl, bp, n)) < 0)
      return -1;
    bp += n;
    len -= n;
  } while (!newline && len);
  
  *bp = '\0';
  return bp - buf;
}
#endif

#ifdef USE_SSL
 gint ssl_read(SSL *ssl, gchar *buf, gint len)
{
	gint err, ret;

	if (SSL_pending(ssl) == 0) {
		if (fd_check_io(SSL_get_rfd(ssl), G_IO_IN) < 0)
			return -1;
	}

	ret = SSL_read(ssl, buf, len);

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
*/
 
/* peek at the socket data without actually reading it */
 /*
#ifdef USE_SSL
gint ssl_peek(SSL *ssl, gchar *buf, gint len)
{
	gint err, ret;

	if (SSL_pending(ssl) == 0) {
		if (fd_check_io(SSL_get_rfd(ssl), G_IO_IN) < 0)
			return -1;
	}

	ret = SSL_peek(ssl, buf, len);

	switch ((err = SSL_get_error(ssl, ret))) {
	case SSL_ERROR_NONE:
		return ret;
	case SSL_ERROR_WANT_READ:
	case SSL_ERROR_WANT_WRITE:
		errno = EAGAIN;
		return -1;
	case SSL_ERROR_ZERO_RETURN:
		return 0;
	case SSL_ERROR_SYSCALL:
	  // End of file
	  //printf("SSL_ERROR_SYSCALL ret=%d  %d\n",ret,(gint)strlen(buf));
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
gint ssl_write(SSL *ssl, const gchar *buf, gint len)
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
*/


#ifdef USE_WIN32
unsigned __stdcall http_c_nonssl(LPVOID lpvPipe)
#else
int http_c_nonssl(typMascot *mascot)
#endif
{
#ifdef USE_WIN32
  typMascot *mascot=(typMascot *) lpvPipe;
#endif
  int command_socket;           /* コマンド用ソケット */
  int size;
  
  char send_mesg[BUF_LEN];          /* サーバに送るメッセージ */
  char buf[BUF_LEN+1];
  
  FILE *fp_write;
  FILE *fp_read;

  struct addrinfo hints, *res;
  struct in_addr addr;
  int err;

  gboolean chunked_flag=FALSE;
  gchar *cp;

  gchar *rand16=NULL;
  gint plen;

  check_msg_from_parent();
   
  /* ホストの情報 (IP アドレスなど) を取得 */
  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_family = AF_INET;

  if ((err = getaddrinfo(mascot->http_host, "http", &hints, &res)) !=0){
    fprintf(stderr, "Bad hostname [%s]\n", mascot->http_host);
#ifdef USE_WIN32
    gtk_main_quit();
    _endthreadex(0);
#endif
    return(MACOPIX_HTTP_ERROR_GETHOST);
  }

  check_msg_from_parent();
   
  /* ソケット生成 */
  if( (command_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0){
    fprintf(stderr, "Failed to create a new socket.\n");
#ifdef USE_WIN32
    gtk_main_quit();
    _endthreadex(0);
#endif
    return(MACOPIX_HTTP_ERROR_SOCKET);
  }
  
  check_msg_from_parent();
   
  /* サーバに接続 */
  if( connect(command_socket, res->ai_addr, res->ai_addrlen) == -1){
    fprintf(stderr, "Failed to connect to %s .\n", mascot->http_host);
#ifdef USE_WIN32
    gtk_main_quit();
    _endthreadex(0);
#endif
    return(MACOPIX_HTTP_ERROR_CONNECT);
  }
  
  check_msg_from_parent();
   
  // AddrInfoの解放
  freeaddrinfo(res);

  // HTTP/1.1 ではchunked対策が必要
  sprintf(send_mesg, "GET %s HTTP/1.1\r\n", mascot->http_path);
  write_to_server(command_socket, send_mesg);

  sprintf(send_mesg, "Accept: text/plain,text/html,application/x-gzip\r\n");
  write_to_server(command_socket, send_mesg);

  sprintf(send_mesg, "Accept-Encoding: gzip\r\n");
  write_to_server(command_socket, send_mesg);

  sprintf(send_mesg, "User-Agent: Mozilla/5.0\r\n");
  write_to_server(command_socket, send_mesg);

  sprintf(send_mesg, "Host: %s\r\n", mascot->http_host);
  write_to_server(command_socket, send_mesg);

  sprintf(send_mesg, "Connection: close\r\n");
  write_to_server(command_socket, send_mesg);

  sprintf(send_mesg, "\r\n");
  write_to_server(command_socket, send_mesg);

  if((fp_write=fopen(mascot->http_dlfile,"wb"))==NULL){
    fprintf(stderr," File Write Error  \"%s\" \n", mascot->http_dlfile);
    return(MACOPIX_HTTP_ERROR_TEMPFILE);
  }

  unlink_dlsz(mascot);
  
  while((size = fd_gets(command_socket,buf,BUF_LEN)) > 2 ){
    // header lines
    if(debug_flg){
      fprintf(stderr,"--> Header: %s", buf);
    }
    if(NULL != (cp = strstr(buf, "Transfer-Encoding: chunked"))){
      chunked_flag=TRUE;
    }
    if(strncmp(buf,"Content-Length: ",strlen("Content-Length: "))==0){
      cp = buf + strlen("Content-Length: ");
      mascot->http_dlsz=atol(cp);
    }
  }
  
  write_dlsz(mascot);
  
  do{ // data read
    size = recv(command_socket,buf,BUF_LEN, 0);
    fwrite( &buf , size , 1 , fp_write ); 
  }while(size>0);
      
  fclose(fp_write);

  check_msg_from_parent();

  if(chunked_flag) unchunk(mascot->http_dlfile);

#ifndef USE_WIN32
    if((chmod(mascot->http_dlfile,(S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |S_IROTH | S_IWOTH ))) != 0){
    g_print("Cannot Chmod Temporary File %s!  Please check!!!\n",mascot->http_dlfile);
  }
#endif
  
#ifdef USE_WIN32
  closesocket(command_socket);
  gtk_main_quit();
  _endthreadex(0);
#else
  close(command_socket);

  return 0;
#endif
}

#ifdef USE_SSL
#ifdef USE_WIN32
unsigned __stdcall http_c_ssl(LPVOID lpvPipe)
#else
int http_c_ssl(typMascot *mascot)
#endif
{
#ifdef USE_WIN32
  typMascot *mascot=(typMascot *) lpvPipe; 
#endif
  int command_socket;           /* コマンド用ソケット */
  int size;

  char send_mesg[BUF_LEN];          /* サーバに送るメッセージ */
  char buf[BUF_LEN+1];
  
  FILE *fp_write;
  FILE *fp_read;

  struct addrinfo hints, *res;
  struct addrinfo dhints, *dres;
  struct in_addr addr;
  int err, ret;

  gboolean chunked_flag=FALSE;
  gchar *cp;

  gchar *rand16=NULL;
  gint plen;

  SSL *ssl;
  SSL_CTX *ctx;

   
  check_msg_from_parent();

  /* ホストの情報 (IP アドレスなど) を取得 */
  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_family = AF_INET;

  if ((err = getaddrinfo(mascot->http_host, "https", &hints, &res)) !=0){
    fprintf(stderr, "Bad hostname [%s]\n", mascot->http_host);
#ifdef USE_WIN32
    gtk_main_quit();
    _endthreadex(0);
#endif
    return(MACOPIX_HTTP_ERROR_GETHOST);
  }

  check_msg_from_parent();

    /* ソケット生成 */
  if( (command_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0){
    fprintf(stderr, "Failed to create a new socket.\n");
#ifdef USE_WIN32
    gtk_main_quit();
    _endthreadex(0);
#endif
    return(MACOPIX_HTTP_ERROR_SOCKET);
  }

  check_msg_from_parent();
  
  /* サーバに接続 */
  if( connect(command_socket, res->ai_addr, res->ai_addrlen) == -1){
    fprintf(stderr, "Failed to connect to %s .\n", mascot->http_host);
#ifdef USE_WIN32
    gtk_main_quit();
    _endthreadex(0);
#endif
    return(MACOPIX_HTTP_ERROR_CONNECT);
  }

  check_msg_from_parent();

  SSL_load_error_strings();
  SSL_library_init();

  ctx = SSL_CTX_new(SSLv23_client_method());
  ssl = SSL_new(ctx);
  err = SSL_set_fd(ssl, command_socket);
  while((ret=SSL_connect(ssl))!=1){
    err=SSL_get_error(ssl, ret);
    if( (err==SSL_ERROR_WANT_READ)||(err==SSL_ERROR_WANT_WRITE) ){
      g_usleep(100000);
      g_warning("SSL_connect(): try again\n");
      continue;
    }
    g_warning("SSL_connect() failed with error %d, ret=%d (%s)\n",
	      err, ret, ERR_error_string(ERR_get_error(), NULL));
#ifdef USE_WIN32
    gtk_main_quit();
    _endthreadex(0);
#endif
    return (MACOPIX_HTTP_ERROR_CONNECT);
  }

  check_msg_from_parent();
  
  // AddrInfoの解放
  freeaddrinfo(res);

  // HTTP/1.1 ではchunked対策が必要
  sprintf(send_mesg, "GET %s HTTP/1.1\r\n", mascot->http_path);
  write_to_SSLserver(ssl, send_mesg);

  sprintf(send_mesg, "Accept: application/xml, application/json\r\n");
  write_to_SSLserver(ssl, send_mesg);

  sprintf(send_mesg, "User-Agent: Mozilla/5.0\r\n");
  write_to_SSLserver(ssl, send_mesg);

  sprintf(send_mesg, "Host: %s\r\n", mascot->http_host);
  write_to_SSLserver(ssl, send_mesg);

  sprintf(send_mesg, "Connection: close\r\n");
  write_to_SSLserver(ssl, send_mesg);

  sprintf(send_mesg, "\r\n");
  write_to_SSLserver(ssl, send_mesg);

  if((fp_write=fopen(mascot->http_dlfile,"wb"))==NULL){
    fprintf(stderr," File Write Error  \"%s\" \n", mascot->http_dlfile);
    return(MACOPIX_HTTP_ERROR_TEMPFILE);
  }

  while((size = ssl_gets(ssl, buf, BUF_LEN)) > 2 ){
    // header lines
    if(debug_flg){
      fprintf(stderr,"[SSL] --> Header: %s", buf);
    }
    if(NULL != (cp = strstr(buf, "Transfer-Encoding: chunked"))){
      chunked_flag=TRUE;
      }
  }
  do{ // data read
    size = SSL_read(ssl, buf, BUF_LEN);
    fwrite( &buf , size , 1 , fp_write ); 
  }while(size >0);
      
  fclose(fp_write);

  check_msg_from_parent();

  if(chunked_flag) unchunk(mascot->http_dlfile);

#ifndef USE_WIN32
    if((chmod(mascot->http_dlfile,(S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |S_IROTH | S_IWOTH ))) != 0){
    g_print("Cannot Chmod Temporary File %s!  Please check!!!\n",mascot->http_dlfile);
  }
#endif

  SSL_shutdown(ssl);
  SSL_free(ssl);
  SSL_CTX_free(ctx);
  ERR_free_strings();
  
#ifdef USE_WIN32
  closesocket(command_socket);
  gtk_main_quit();
  _endthreadex(0);
#else
  close(command_socket);

  return 0;
#endif
}
#endif  //USE_SSL



gboolean progress_timeout( gpointer data ){
  typMascot *mascot=(typMascot *)data;
  glong sz=-1;
  gchar *tmp;
  gdouble frac;

  if(!mascot->http_ok){
    return(FALSE);
  }

  if(gtk_widget_get_realized(mascot->pbar)){
    sz=get_file_size(mascot->http_dlfile);

    if(sz>0){  // After Downloading Started to get current dlsz
      if(mascot->http_dlsz<0){
	mascot->http_dlsz=get_dlsz(mascot);
      }
    }

    if(mascot->http_dlsz>0){
      frac=(gdouble)sz/(gdouble)mascot->http_dlsz;
      gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(mascot->pbar),
				    frac);

      if(sz>1024*1024){
	tmp=g_strdup_printf(_("%d%% Downloaded (%.2lf / %.2lf MB)"),
			    (gint)(frac*100.),
			    (gdouble)sz/1024./1024.,
			    (gdouble)mascot->http_dlsz/1024./1024.);
      }
      else if(sz>1024){
	tmp=g_strdup_printf(_("%d%% Downloaded (%ld / %ld kB)"),
			    (gint)(frac*100.),
			    sz/1024,
			    mascot->http_dlsz/1024);
      }
      else{
	tmp=g_strdup_printf(_("%d%% Downloaded (%ld / %ld bytes)"),
			    (gint)(frac*100.),
			    sz, mascot->http_dlsz);
      }
    }
    else{
      gtk_progress_bar_pulse(GTK_PROGRESS_BAR(mascot->pbar));

      if(sz>1024*1024){
	tmp=g_strdup_printf(_("Downloaded %.2lf MB"),
			    (gdouble)sz/1024./1024.);
      }
      else if(sz>1024){
	tmp=g_strdup_printf(_("Downloaded %ld kB"), sz/1024);
      }
      else if (sz>0){
	tmp=g_strdup_printf(_("Downloaded %ld bytes"), sz);
      }
      else{
	tmp=g_strdup_printf(_("Waiting for HTTP server response ..."));
      }
    }
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(mascot->pbar),
			      tmp);
    g_free(tmp);
  }
  
  return TRUE;
}


#ifndef USE_WIN32
void httpdl_signal(int sig){
  pid_t child_pid=0;

  gtk_main_quit();

  do{
    int child_ret;
    child_pid=waitpid(http_pid, &child_ret,WNOHANG);
  } while((child_pid>0)||(child_pid!=-1));
}
#endif

void popup_dl_mascot_list(GtkWidget *w, gpointer gdata){
  typMascot *mascot = (typMascot *)gdata;
  
  dl_mascot_list(mascot, TRUE);
}

void smenu_dl_mascot_list(GtkWidget *w, gpointer gdata){
  typMascot *mascot = (typMascot *)gdata;
  
  dl_mascot_list(mascot, FALSE);
  
  gtk_main_quit();
}

void dl_mascot_list(typMascot *mascot,  gboolean flag_popup){
  GtkWidget *dialog, *vbox, *label, *button, *bar;
  gint timer=-1;
#ifndef USE_WIN32
  static struct sigaction act;
#endif
  gchar *tmp;

  mascot->http_ok=TRUE;
  
  if(mascot->http_host) g_free(mascot->http_host);
  mascot->http_host=g_strdup(HTTP_MASCOT_HOST);

  if(mascot->http_path) g_free(mascot->http_path);
  mascot->http_path=g_strconcat(HTTP_MASCOT_PATH, HTTP_MASCOT_FILE, NULL);

  if(mascot->http_dlfile) g_free(mascot->http_dlfile);
#ifdef USE_WIN32
  mascot->http_dlfile=g_strconcat(g_get_tmp_dir(), G_DIR_SEPARATOR_S,
				  HTTP_MASCOT_FILE, NULL);
#else
  mascot->http_dlfile=g_strdup_printf("%s%s%s-%d",
				      g_get_tmp_dir(), G_DIR_SEPARATOR_S,
				      HTTP_MASCOT_FILE,   getuid());
#endif

  if(access(mascot->http_dlfile, F_OK)==0) unlink(mascot->http_dlfile);

  dialog = gtk_dialog_new();
  
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(dialog),5);
  gtk_container_set_border_width(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),5);
  gtk_window_set_title(GTK_WINDOW(dialog),_("MaCoPiX : Downloading Official Mascot List"));
  gtk_window_set_decorated(GTK_WINDOW(dialog),TRUE);

  label=gtk_label_new(_("Downloading the latest version of the official mascot list ..."));
#ifdef USE_GTK3
  gtk_widget_set_halign (label, GTK_ALIGN_START);
  gtk_widget_set_valign (label, GTK_ALIGN_CENTER);
#else
  gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
#endif
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     label,TRUE,TRUE,0);
  gtk_widget_show(label);
  
  mascot->http_dlsz=-1;
  mascot->pbar=gtk_progress_bar_new();
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     mascot->pbar,TRUE,TRUE,0);
  gtk_progress_bar_pulse(GTK_PROGRESS_BAR(mascot->pbar));
#ifdef USE_GTK3
  gtk_orientable_set_orientation (GTK_ORIENTABLE (mascot->pbar), 
				  GTK_ORIENTATION_HORIZONTAL);
  css_change_pbar_height(mascot->pbar,15);
  gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(mascot->pbar), TRUE);
#else
  gtk_progress_bar_set_orientation (GTK_PROGRESS_BAR (mascot->pbar), 
				    GTK_PROGRESS_RIGHT_TO_LEFT);
#endif
  gtk_progress_bar_set_pulse_step(GTK_PROGRESS_BAR(mascot->pbar),0.05);
  gtk_widget_show(mascot->pbar);
  
#ifdef USE_GTK3
  bar = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
#else
  bar = gtk_hseparator_new();
#endif
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     bar,FALSE, FALSE, 0);

  label=gtk_label_new(_("Checking the latest version of the official mascot list ..."));
#ifdef USE_GTK3
  gtk_widget_set_halign (label, GTK_ALIGN_START);
  gtk_widget_set_valign (label, GTK_ALIGN_CENTER);
#else
  gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
#endif
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     label,FALSE,FALSE,0);

#ifdef USE_GTK3
  button=gtkut_button_new_from_icon_name(_("Cancel"),"process-stop");
#else
  button=gtkut_button_new_from_stock(_("Cancel"),GTK_STOCK_CANCEL);
#endif
  gtk_dialog_add_action_widget(GTK_DIALOG(dialog),button,GTK_RESPONSE_CANCEL);
  my_signal_connect(button,"pressed",
		    cancel_http, 
		    (gpointer)mascot);

  gtk_widget_show_all(dialog);

  timer=g_timeout_add(100, 
		      (GSourceFunc)progress_timeout,
		      (gpointer)mascot);
  
#ifndef USE_WIN32
  act.sa_handler=httpdl_signal;
  sigemptyset(&act.sa_mask);
  act.sa_flags=0;
  if(sigaction(SIGHTTPDL, &act, NULL)==-1){
    fprintf(stderr,"Error in sigaction (SIGHSKYMON1).\n");
  }
#endif
  
  gtk_window_set_modal(GTK_WINDOW(dialog),TRUE);
  
  get_mascot_list(mascot);
  gtk_main();
  
  gtk_window_set_modal(GTK_WINDOW(dialog),FALSE);
  if(timer!=-1) g_source_remove(timer);

  if(GTK_IS_WIDGET(dialog)) gtk_widget_destroy(dialog);

  if(access(mascot->http_dlfile, F_OK)==0){
    create_dl_smenu_dialog(mascot, flag_popup);
  }
  else{
    tmp=g_strconcat(mascot->http_host,
		    mascot->http_path,
		    NULL);
    popup_message(NULL,
#ifdef USE_GTK3
		  "dialog-error", 
#else
		  GTK_STOCK_DIALOG_ERROR,
#endif
		  -1,
		  _("Error : Failed to download the mascot archive file (tar.gz)."),
		  " ",
		  tmp,
		  NULL);
    g_free(tmp);
  }
}



void dl_mascot_tgz(typMascot *mascot){
  GtkWidget *dialog, *vbox, *label, *button, *bar;
  gint timer=-1;
#ifndef USE_WIN32
  static struct sigaction act;
#endif

  dialog = gtk_dialog_new();

  mascot->http_ok=TRUE;
  
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(dialog),5);
  gtk_container_set_border_width(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),5);
  gtk_window_set_title(GTK_WINDOW(dialog),_("MaCoPiX : Downloading Official Mascot tar.gz file"));
  gtk_window_set_decorated(GTK_WINDOW(dialog),TRUE);

  label=gtkut_label_new(_("Downloading an official mascot ..."));
  gtkut_pos(label, POS_START, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     label,TRUE,TRUE,0);
  gtk_widget_show(label);

  mascot->http_dlsz=-1;
  mascot->pbar=gtk_progress_bar_new();
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     mascot->pbar,TRUE,TRUE,0);
  gtk_progress_bar_pulse(GTK_PROGRESS_BAR(mascot->pbar));
#ifdef USE_GTK3
  gtk_orientable_set_orientation (GTK_ORIENTABLE (mascot->pbar), 
				  GTK_ORIENTATION_HORIZONTAL);
  css_change_pbar_height(mascot->pbar,15);
  gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(mascot->pbar), TRUE);
#else
  gtk_progress_bar_set_orientation (GTK_PROGRESS_BAR (mascot->pbar), 
				    GTK_PROGRESS_RIGHT_TO_LEFT);
#endif
  gtk_progress_bar_set_pulse_step(GTK_PROGRESS_BAR(mascot->pbar),0.05);
  gtk_widget_show(mascot->pbar);
  
#ifdef USE_GTK3
  bar = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
#else
  bar = gtk_hseparator_new();
#endif
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     bar,FALSE, FALSE, 0);

  label=gtk_label_new(_("Downloading ..."));
#ifdef USE_GTK3
  gtk_widget_set_halign (label, GTK_ALIGN_START);
  gtk_widget_set_valign (label, GTK_ALIGN_CENTER);
#else
  gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
#endif
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     label,FALSE,FALSE,0);

#ifdef USE_GTK3
  button=gtkut_button_new_from_icon_name(_("Cancel"),"process-stop");
#else
  button=gtkut_button_new_from_stock(_("Cancel"),GTK_STOCK_CANCEL);
#endif
  gtk_dialog_add_action_widget(GTK_DIALOG(dialog),button,GTK_RESPONSE_CANCEL);
  my_signal_connect(button,"pressed",
		    cancel_http, 
		    (gpointer)mascot);
  
  gtk_widget_show_all(dialog);

  timer=g_timeout_add(100, 
		      (GSourceFunc)progress_timeout,
		      (gpointer)mascot);
  
#ifndef USE_WIN32
  act.sa_handler=httpdl_signal;
  sigemptyset(&act.sa_mask);
  act.sa_flags=0;
  if(sigaction(SIGHTTPDL, &act, NULL)==-1){
    fprintf(stderr,"Error in sigaction (SIGHSKYMON1).\n");
  }
#endif
  
  gtk_window_set_modal(GTK_WINDOW(dialog),TRUE);

  get_mascot_list(mascot);
  gtk_main();
  
  gtk_window_set_modal(GTK_WINDOW(dialog),FALSE);
  if(timer!=-1) g_source_remove(timer);

  if(GTK_IS_WIDGET(dialog)) gtk_widget_destroy(dialog);
}


glong get_file_size(gchar *fname)
{
  FILE *fp;
  long sz;

  fp = fopen( fname, "rb" );
  if( fp == NULL ){
    return -1;
  }

  fseek( fp, 0, SEEK_END );
  sz = ftell( fp );

  fclose( fp );
  return sz;
}


void write_dlsz(typMascot *mascot){
  FILE *fp;
  gchar *tmp_file;

#ifdef USE_WIN32
  tmp_file=g_strconcat(g_get_tmp_dir(), G_DIR_SEPARATOR_S,
				  HTTP_DLSZ_FILE, NULL);
#else
  tmp_file=g_strdup_printf("%s%s%s-%d",
			   g_get_tmp_dir(), G_DIR_SEPARATOR_S,
			   HTTP_DLSZ_FILE, getuid());
#endif
  
  if((fp=fopen(tmp_file,"w"))==NULL){
    fprintf(stderr," File Write Error  \"%s\" \n", tmp_file);
    g_free(tmp_file);
    return;
  }

  fprintf(fp, "%ld\n",mascot->http_dlsz);
  fclose(fp);
  
  g_free(tmp_file);
  return;
}


void unlink_dlsz(typMascot *mascot){
  gchar *tmp_file;

  mascot->http_dlsz=0;
  
#ifdef USE_WIN32
  tmp_file=g_strconcat(g_get_tmp_dir(), G_DIR_SEPARATOR_S,
				  HTTP_DLSZ_FILE, NULL);
#else
  tmp_file=g_strdup_printf("%s%s%s-%d",
			   g_get_tmp_dir(), G_DIR_SEPARATOR_S,
			   HTTP_DLSZ_FILE, getuid());
#endif
  
  if(access(tmp_file, F_OK)==0){
    unlink(tmp_file);
  }

  g_free(tmp_file);
  return;
}


glong get_dlsz(typMascot *mascot){
  FILE *fp;
  gchar *tmp_file;
  glong sz=0;
  gchar buf[10];
  

#ifdef USE_WIN32
  tmp_file=g_strconcat(g_get_tmp_dir(), G_DIR_SEPARATOR_S,
				  HTTP_DLSZ_FILE, NULL);
#else
  tmp_file=g_strdup_printf("%s%s%s-%d",
			   g_get_tmp_dir(), G_DIR_SEPARATOR_S,
			   HTTP_DLSZ_FILE, getuid());
#endif
  
  if((fp=fopen(tmp_file,"r"))==NULL){
    g_free(tmp_file);
    return(-1);
  }

  if(fgets(buf,10-1,fp)){
    sz = atol(buf);
  }
  fclose(fp);

  unlink(tmp_file);
  
  g_free(tmp_file);
  return (sz);
}


static void cancel_http(GtkWidget *w, gpointer gdata){
  typMascot *mascot = (typMascot *)gdata;
  pid_t child_pid=0;

  mascot->http_ok=FALSE;

#ifdef USE_WIN32
  if(mascot->dwThreadID_http){
    PostThreadMessage(mascot->dwThreadID_http, WM_QUIT, 0, 0);
    WaitForSingleObject(mascot->hThread_http, INFINITE);
    CloseHandle(mascot->hThread_http);
    gtk_main_quit();
  }
  else{
    gtk_main_quit();
  }
#else
  if(http_pid){
    kill(http_pid, SIGKILL);
    gtk_main_quit();

    do{
      int child_ret;
      child_pid=waitpid(http_pid, &child_ret,WNOHANG);
    } while((child_pid>0)||(child_pid!=-1));
 
    http_pid=0;
  }
  else{
    gtk_main_quit();
  }
#endif

  unlink_dlsz(mascot);
  if(access(mascot->http_dlfile, F_OK)==0) unlink(mascot->http_dlfile);
}
