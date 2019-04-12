//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//
//
//     mail.c
//     Biff functions for MaCoPiX (POP3 /APOP and local spool)
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

#ifdef USE_BIFF

#ifdef USE_WIN32
#include <windows.h>
#endif

#include <dirent.h>

#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif

#include <signal.h>
#include "codeconv.h"

#define BUFFSIZE 200
#define BUFFSIZE5 1000
#define HEADER_MAX 256
#define MIME_BUFFSIZE 8192


void biff_init();

gint ResetMailChecker();
gboolean MailChecker();

void mail_check();
void mail_check_mbox();
void mail_check_qmail();
void mail_check_pop3();
#ifdef USE_WIN32
unsigned __stdcall get_pop3();
#else
int get_pop3();
#endif
gchar * fs_get_mbox();
gchar * fs_get_procmail();
gchar * fs_get_qmail();
void fs_get_pop3();
void pop3_error();
#ifndef USE_WIN32
void pop3_signal();
void pop3_data_read();
#endif
char* strbuf();


void make_fs_max();
static void close_biff();
static void mailer_start();

void strip_last_ret();


extern typMascot *Mascot;
extern gboolean flag_balloon;


// global argument
#ifndef USE_WIN32
int pop3_fd[2];
int pop3_pid=0;
#endif

gchar *pop_froms;
time_t former_newest;




void biff_init(typMascot *mascot)
{
  pop_debug_print("\nBiff init\n");

  mascot->mail.size=0;
  mascot->mail.last_check=0;
  mascot->mail.count=0;
  mascot->mail.fetched_count = 0;    // POP3 fetched fs this access
  mascot->mail.displayed_count = 0;  // POP3 displayed fs
  mascot->mail.spam_count = 0;       // SPAM count
  mascot->mail.status = NO_MAIL;
  mascot->mail.pop3_fs_status = POP3_OK_NORMAL;
  if(mascot->mail.last_f) g_free(mascot->mail.last_f);
  mascot->mail.last_f= NULL;
  if(mascot->mail.last_s) g_free(mascot->mail.last_s);
  mascot->mail.last_s=NULL;
  if(pop_froms) g_free(pop_froms);
  pop_froms=strbuf(NULL);
#ifdef __GTK_TOOLTIP_H__
  //gtk_widget_set_tooltip_text(mascot->mail.e_draw,
  //gtk_widget_set_tooltip_text(mascot->mail.e_draw,
  gtk_widget_set_tooltip_text(mascot->biff_pix,
			      _("Initializing Biff..."));
#else
  gtk_tooltips_set_tip(GTK_TOOLTIPS(mascot->mail.tooltips),
		       //mascot->mail.e_draw,
		       mascot->biff_pix,
		       _("Initializing Biff..."),
		       NULL);
#endif

  if(mascot->mail.tooltips_fl){
#ifdef __GTK_TOOLTIP_H__
    //gtk_widget_set_tooltip_text(mascot->mail.e_draw,NULL);
    gtk_widget_set_tooltip_text(mascot->biff_pix,NULL);
#else
    gtk_tooltips_enable(mascot->mail.tooltips);
#endif
  }
  else{
#ifdef __GTK_TOOLTIP_H__
    //gtk_widget_set_tooltip_text(mascot->mail.e_draw,NULL);
    gtk_widget_set_tooltip_text(mascot->biff_pix,NULL);
#else
    gtk_tooltips_disable(mascot->mail.tooltips);
#endif
  }
  mascot->mail.pop_readed=FALSE;

  if((mascot->mail.type==MAIL_POP3)||(mascot->mail.type==MAIL_APOP)){
    if(mascot->mail.pop_pass==NULL){
      if((mascot->mail.pop_server!=NULL)&&(mascot->mail.pop_id!=NULL)){
	create_pop_pass_dialog();
	// Get Pop Pass from GUI
      }
    }
  }

  if(mascot->mail.ssl_cert_skip)
    mascot->mail.ssl_cert_res=SSL_CERT_ACCEPT;
  else mascot->mail.ssl_cert_res=SSL_CERT_NONE;
  if(mascot->mail.ssl_sub) g_free(mascot->mail.ssl_sub);
  mascot->mail.ssl_sub=NULL;
  if(mascot->mail.ssl_iss) g_free(mascot->mail.ssl_iss);
  mascot->mail.ssl_iss=NULL;
  mascot->mail.ssl_verify=0;
}    


gint SetMailChecker(gpointer gdata){
  typMascot *mascot;

  mascot=(typMascot *)gdata;

  if(mascot->mail.proc_id>0){
    g_source_remove((guint)mascot->mail.proc_id);
    fflush(stdin);
    fflush(stdout);
  }
  gtk_widget_unmap(mascot->biff_pix);

  if(mascot->mail.flag){
    biff_init(mascot);
    MailChecker(mascot);
    mascot->mail.proc_id
      =(gint)g_timeout_add((guint32)(mascot->mail.interval*1000),
			   (GSourceFunc)MailChecker,
			   (gpointer)mascot);

    pop_debug_print("proc_id=%d\n",mascot->mail.proc_id);
  }
}

gint ResetMailChecker(gpointer gdata){
  typMascot *mascot;

  mascot=(typMascot *)gdata;

  if(mascot->mail.pop_child_fl){
    return (0);
  }

  if(mascot->mail.proc_id>0){
    g_source_remove((guint)mascot->mail.proc_id);
    fflush(stdin);
    fflush(stdout);
  }
  gtk_widget_unmap(mascot->biff_pix);

  if(mascot->mail.flag){
    MailChecker(mascot);
    mascot->mail.proc_id
      =(gint)g_timeout_add((guint32)(mascot->mail.interval*1000),
			   (GSourceFunc)MailChecker,
			   (gpointer)mascot);

    pop_debug_print("proc_id=%d\n",mascot->mail.proc_id);
  }

  return (0);
}


gboolean MailChecker(gpointer gdata){
  typMascot *mascot;

  mascot=(typMascot *)gdata;

  if(mascot->flag_menu) return(TRUE);

  gdk_flush();

  ext_play(mascot,mascot->mail.polling);  
  mail_check(mascot);

  gdk_flush();

  if((mascot->mail.type!=MAIL_POP3)&&(mascot->mail.type!=MAIL_APOP))
    display_biff_balloon(mascot);

  gdk_flush();
  return(TRUE);
}


void make_biff_pix(typMascot *mascot){
  
  mascot->biff_pix = gtk_window_new(GTK_WINDOW_POPUP);
  gtk_window_set_accept_focus(GTK_WINDOW(mascot->biff_pix),FALSE);
  gtk_widget_set_app_paintable(mascot->biff_pix, TRUE);
  gtk_widget_set_events(GTK_WIDGET (mascot->biff_pix), 
			GDK_FOCUS_CHANGE_MASK | 
			GDK_BUTTON_MOTION_MASK | 
			GDK_BUTTON_RELEASE_MASK | 
			GDK_BUTTON_PRESS_MASK | 
			GDK_EXPOSURE_MASK);
  gtk_widget_realize(mascot->biff_pix);
  gtk_window_set_resizable(GTK_WINDOW(mascot->biff_pix),TRUE);
  gdk_window_set_decorations(gtk_widget_get_window(mascot->biff_pix), 0);
#ifndef USE_WIN32
  /* gdk_window_set_override_redirect is not implemented (for warning) */
  gdk_window_set_override_redirect(gtk_widget_get_window(mascot->biff_pix),TRUE);
#endif

  my_signal_connect(mascot->biff_pix, "configure_event",
		    dw_configure_biff_pix, (gpointer)mascot);
  my_signal_connect(mascot->biff_pix, "expose_event",
		    dw_expose_biff_pix, (gpointer)mascot);
  my_signal_connect(mascot->biff_pix, "button_press_event",
		    biff_drag_begin, (gpointer)mascot);
  my_signal_connect(mascot->biff_pix, "button_release_event",
		    biff_drag_end, (gpointer)mascot);
  my_signal_connect(mascot->biff_pix, "motion_notify_event",
		    biff_window_motion, (gpointer)mascot);

  gtk_window_resize (GTK_WINDOW(mascot->biff_pix), 1, 1);
}


void display_biff_balloon(typMascot *mascot)
{
  gchar *text_tip=NULL;

  if(mascot->mail.tooltips_fl){

    if(mascot->mail.count!=0){
      if((mascot->mail.type==MAIL_POP3)||(mascot->mail.type==MAIL_APOP)){
	if(mascot->mail.spam_count!=0){
	  if((mascot->mail.last_f!=NULL)&&(mascot->mail.last_s!=NULL))
	    text_tip=g_strdup_printf(_("You have %d mails (%d SPAMs).\n [Latest Mail]\n  From: %s\n  Subject: %s"),
				     mascot->mail.count,mascot->mail.spam_count,
				     mascot->mail.last_f,mascot->mail.last_s);
	}
	else{
	  if((mascot->mail.last_f!=NULL)&&(mascot->mail.last_s!=NULL))
	    text_tip=g_strdup_printf(_("You have %d mails.\n [Latest Mail]\n  From: %s\n  Subject: %s"),
				     mascot->mail.count,
				     mascot->mail.last_f,mascot->mail.last_s);
	}
	if(text_tip){
#ifdef __GTK_TOOLTIP_H__
	  gtk_widget_set_tooltip_text(mascot->biff_pix,
				      //gtk_widget_set_tooltip_text(mascot->mail.e_draw,
				      text_tip);
#else
	  gtk_tooltips_set_tip(GTK_TOOLTIPS(mascot->mail.tooltips),
			       mascot->biff_pix,
			       //mascot->mail.e_draw,
			       text_tip,NULL);
#endif
	  g_free(text_tip);
	}
      }
      else{
	text_tip=g_strdup_printf(_("You have %d mails."),mascot->mail.count);
#ifdef __GTK_TOOLTIP_H__
	//	gtk_widget_set_tooltip_text(mascot->mail.e_draw,
	gtk_widget_set_tooltip_text(mascot->biff_pix,
				    text_tip);
#else
	gtk_tooltips_set_tip(GTK_TOOLTIPS(mascot->mail.tooltips),
			     //mascot->mail.e_draw,
			     mascot->biff_pix,
			     text_tip,NULL);
#endif
	g_free(text_tip);
      }
    }
    else{
#ifdef __GTK_TOOLTIP_H__
      //gtk_widget_set_tooltip_text(mascot->mail.e_draw,
      gtk_widget_set_tooltip_text(mascot->biff_pix,
				  _("You have no mails."));
#else
      gtk_tooltips_set_tip(GTK_TOOLTIPS(mascot->mail.tooltips),
			   //mascot->mail.e_draw,
			   mascot->biff_pix,
			   _("You have no mails."),
			   NULL);
#endif
    }
  }

  if(mascot->mail.status == POP3_ERROR){
    gtk_widget_unmap(mascot->biff_pix);

    if(flag_balloon==FALSE){
      mascot->balseq=0;
      mascot->bal_mode=BALLOON_POPERROR;
      DoBalloon(mascot); 
      flag_balloon=TRUE;
      mascot->mail.status = NO_MAIL;
    }
  }
  else if(mascot->mail.status == POP3_SSL_CERT){
    gtk_widget_unmap(mascot->biff_pix);
    
#ifdef USE_SSL
    mascot->mail.ssl_cert_res=ssl_manager_verify_cert(mascot);
    if(mascot->mail.ssl_cert_res==SSL_CERT_ACCEPT)
      ResetMailChecker((gpointer)mascot);
#endif
  }
  else if(mascot->mail.status != NO_MAIL){
    MoveBiffPix(mascot,mascot->x,mascot->y);
    gtk_widget_map(mascot->biff_pix);
  }
  else{
    gtk_widget_unmap(mascot->biff_pix);
  }
  if(mascot->mail.status==NEW_MAIL){
    sound_play(mascot,mascot->mail.sound);

    if((mascot->mail.word)&&(flag_balloon==FALSE)) {
      mascot->balseq=0;
      mascot->bal_mode=BALLOON_MAIL;
      DoBalloon(mascot); 
      flag_balloon=TRUE;
      if((mascot->mail.type==MAIL_POP3)||(mascot->mail.type==MAIL_APOP)){
	mascot->mail.status=KEEP_NEW_MAIL;
      }
    }
  }
}


void remap_biff_pix(typMascot *mascot){
  if(!mascot->mail.flag) return;
  if(mascot->mail.status == (POP3_ERROR|POP3_SSL_CERT)){
    gtk_widget_unmap(mascot->biff_pix);
  }
  else if(mascot->mail.status != NO_MAIL){
    MoveBiffPix(mascot,mascot->x,mascot->y);
    gtk_widget_map(mascot->biff_pix);
  }
  else{
    gtk_widget_unmap(mascot->biff_pix);
  }
}

void mail_check(typMascot *mascot){
#ifndef USE_WIN32
  static struct sigaction act;
#endif

  switch(mascot->mail.type){     
  case MAIL_LOCAL:
  case MAIL_PROCMAIL:
    mail_check_mbox(mascot);
    break;
  case MAIL_POP3: 
  case MAIL_APOP: 
#ifndef USE_WIN32
    act.sa_handler=pop3_signal;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    if(sigaction(SIGUSR1, &act,NULL)==-1)
      fprintf(stderr,"sigaction error\n");
#endif
    mail_check_pop3(mascot);
    break;
  case MAIL_QMAIL:
    mail_check_qmail(mascot);
    break;
  }

}


void mail_check_mbox(typMascot *mascot){
    struct stat t;
    DIR *dp;
    struct dirent *entry;
    time_t newest_time=0;
    int filenum=0;
    int mc=0;
    FILE *fp;
    char buf[BUFFSIZE];
    int wo_spam;

    if (!stat(mascot->mail.file, &t)){

      if(t.st_size == 0){ // メイルファイルサイズがゼロ
	mascot->mail.status = NO_MAIL;
	mascot->mail.count = 0;
      }
      else{
	wo_spam = mascot->mail.count - mascot->mail.spam_count;

	if (t.st_size < mascot->mail.size){
	  // mailbox smaller in size; some mails have been deleted
	  mascot->mail.status = OLD_MAIL;
	}
	else if (t.st_atime > t.st_mtime){ 
	  // mailbox read after most recent write
	  mascot->mail.status = OLD_MAIL;
	}
	else if (t.st_size > mascot->mail.size){
	  // mailbox modified after most recent read, and larger in size
	  // this implies the arrival of some new mails
	  mascot->mail.status = NEW_MAIL;
	  // get mail count
	  if((fp=fopen(mascot->mail.file,"r"))!= NULL){
	    do{
	      if(fgets(buf,BUFFSIZE-1,fp)==NULL) break;
	      if(strncmp(buf,"From ",5)==0){
		mc++;
	      }
	    } while (1);
	    fclose(fp);
	    mascot->mail.count = mc;
	  }
	}
	else if ((t.st_size == mascot->mail.size)&&
		 ((mascot->mail.status == NEW_MAIL)||
		  (mascot->mail.status ==KEEP_NEW_MAIL))){
	  mascot->mail.status=KEEP_NEW_MAIL;
	}
	else{
	  mascot->mail.status = NO_MAIL;          // no such mailbox
	  mascot->mail.count = 0;
	}

	if( mc == mascot->mail.spam_count){
	  mascot->mail.status = NO_MAIL; // SPAMだけのとき
	}
	else if((mc - mascot->mail.spam_count) == wo_spam){
	  mascot->mail.status = KEEP_NEW_MAIL; // 増えたのがSPAMだけだったとき
	}
      }
    }
    else{ // メイルファイルが存在しないとき
      mascot->mail.status = NO_MAIL;  
      mascot->mail.count = 0;
    }
    mascot->mail.size = t.st_size;
}


void mail_check_qmail(typMascot *mascot){
    struct stat t;
    DIR *dp;
    struct dirent *entry;
    char tmp[256];
    time_t newest_time=0;
    int filenum=0;
    int wo_spam;

    if ((dp=opendir(mascot->mail.file))==NULL){
      mascot->mail.status = NO_MAIL;  
      mascot->mail.count = 0;
      return;
    }
    
    while((entry=readdir(dp))!=NULL){
      if(entry->d_name[0]!='.'){
	sprintf(tmp,"%s/%s",mascot->mail.file,entry->d_name);
	if (!stat(tmp, &t)){
	  filenum++;
	  if (t.st_mtime>newest_time){ 
	    newest_time=t.st_mtime;
	  }
	}
      }
    }
	
    closedir(dp);
	
    if(filenum==0){
      mascot->mail.status = NO_MAIL;
      mascot->mail.count = 0;
    }
    else{
      wo_spam = mascot->mail.count - mascot->mail.spam_count;

      if(newest_time==former_newest){
	mascot->mail.status = KEEP_NEW_MAIL;  
      }
      else if(newest_time>former_newest){
	mascot->mail.status = NEW_MAIL;  
      }
      else{
	mascot->mail.status = OLD_MAIL;  
      }

      if( filenum == mascot->mail.spam_count){
	mascot->mail.status = NO_MAIL; // SPAMだけのとき
      }
      else if((filenum - mascot->mail.spam_count) == wo_spam){
	mascot->mail.status = KEEP_NEW_MAIL; // 増えたのがSPAMだけだったとき
      }
    }
    former_newest=newest_time;
    mascot->mail.count = filenum;
}


void mail_check_pop3(typMascot *mascot){
#ifdef USE_WIN32
  DWORD dwErrorNumber;
  SECURITY_ATTRIBUTES SecAttrib;    // セキュリティ属性
  //DWORD dwThreadID;
  unsigned int dwThreadID;
  HANDLE hPipe;
  HANDLE hThread;
  BOOL fConnected; 

  if(mascot->mail.pop_child_fl){
    return ;
  }
  mascot->mail.pop_child_fl=TRUE;

  /*hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)get_pop3,
    (LPVOID)mascot, 0, &dwThreadID);*/
  pop_debug_print("Begin POP Thread.\n");
  hThread = (HANDLE)_beginthreadex(NULL,0,
				   get_pop3,
				   (LPVOID)mascot, 0, &dwThreadID);
				   
  if (hThread == NULL) {
    dwErrorNumber = GetLastError();
    fprintf(stderr,"_beginthreadex() error(%ld).\n", dwErrorNumber);
    //ExitProcess(dwErrorNumber);
    pop3_error(mascot);
    mascot->mail.pop_child_fl=FALSE;
  }
  else{
    CloseHandle(hThread);
  }


#else
  int status = 0;
  char *buff = NULL;
  char buf[BUFFSIZE];

  if(mascot->mail.pop_child_fl){
    return ;
  }
  mascot->mail.pop_child_fl=TRUE;

  if(pipe(pop3_fd)==-1) {
    fprintf(stderr,"pipe open error\n");
  }
  waitpid(pop3_pid,0,WNOHANG);

  if( (pop3_pid = fork()) <0){
    fprintf(stderr,"fork error\n");
  }
  else if(pop3_pid ==0) {
    pop_debug_print("child\n");

    status = get_pop3(mascot);

    pop_debug_print("get pop3 end\n");
    pop_debug_print("pop3 fs status = %d\n", mascot->mail.pop3_fs_status);

    if(buff) g_free(buff);
    buff = strbuf(NULL);  // stand by buff
    sprintf(buf,"%d\n",mascot->mail.status); // Write mail status
    buff = strbuf(buf);
    if(status == POP3_SSL_CERT){
      if(mascot->mail.ssl_sub!=NULL){
	sprintf(buf,"%s\n",mascot->mail.ssl_sub);  // SSL subject
      }
      else{
	sprintf(buf,"\n");  
      }
      buff = strbuf(buf);

      if(mascot->mail.ssl_iss!=NULL){
	sprintf(buf,"%s\n",mascot->mail.ssl_iss);  // SSL issuer
      }
      else{
	sprintf(buf,"\n");  
      }
      buff = strbuf(buf);

      sprintf(buf,"%ld\n",mascot->mail.ssl_verify);  // SSL verify_result
      buff = strbuf(buf);
    }
    if((status != POP3_ERROR)&&(status != POP3_SSL_CERT)){          // POP status is GOOD?
      sprintf(buf,"%d\n",mascot->mail.count);  // write total mail count
      buff = strbuf(buf);
      sprintf(buf,"%d\n",mascot->mail.fetched_count);  // write fetched mail count
      buff = strbuf(buf);
      sprintf(buf,"%d\n",mascot->mail.pop3_fs_status);  // write POP status
      buff = strbuf(buf);
      sprintf(buf,"%d\n",mascot->mail.spam_count);  // write SPAM count
      buff = strbuf(buf);
      if(mascot->mail.last_f!=NULL){
	sprintf(buf,"%s\n",mascot->mail.last_f);  // Last_from
      }
      else{
	sprintf(buf,"\n");  
      }
      buff = strbuf(buf);
      if(mascot->mail.last_s!=NULL){
	sprintf(buf,"%s\n",mascot->mail.last_s);  // Last_subject
      }
      else{
	sprintf(buf,"\n");  
      }
      buff = strbuf(buf);

      buff = strbuf(pop_froms);                // write fs data
    }
    if(close(STDOUT_FILENO)==-1) fprintf(stderr,"pipe close error\n");
    if(dup2(pop3_fd[1],STDOUT_FILENO)==-1)
       fprintf(stderr,"pipe duplicate error\n");
    if(close(pop3_fd[0])==-1) fprintf(stderr,"pipe close error\n");

    pop_debug_print("pipe open/close end\n");
    pop_debug_print("buff size = %d\n", (int)strlen(buff));

    printf ("%s",buff);

    pop_debug_print("child end\n");

    fflush(stdout);
    fflush(stdin);
    fflush(stderr);
    kill(getppid(), SIGUSR1);  //calling pop3_data_read
     _exit(-1);
  }
  else{

    pop_debug_print("Parent: --------------------------------\n");
    pop_debug_print("Parent: forked a child with pid = %d\n", (int)pop3_pid);
    pop_debug_print("Parent: mail status = %d\n", mascot->mail.status);

  }
#endif //USE_WIN32
}

#ifdef USE_WIN32
unsigned __stdcall get_pop3(LPVOID lpvPipe)
{
  typMascot *mascot;
  signed int ret;
  int num;
  long size;
  gchar *apop_key;
  int wo_spam;
  int pre_mail_count;

  mascot = (typMascot *)lpvPipe;

  pre_mail_count = mascot->mail.count;  // store mail count

  pop_debug_print("Conncet %s : %d\n", mascot->mail.pop_server, mascot->mail.pop_port);

  apop_key=g_malloc0(sizeof(gchar)*BUFFSIZE);
  memset(apop_key, '\0', BUFFSIZE);
  ret = popConnect(mascot->mail.pop_server, mascot->mail.pop_port, apop_key, 
		   mascot->mail.ssl_mode, mascot->mail.ssl_nonblock, 
		   mascot->mail.ssl_cert_res,
		   &mascot->mail.ssl_sub, &mascot->mail.ssl_iss,
		   &mascot->mail.ssl_verify);

  if( ret != 0 ){
    if(apop_key) g_free(apop_key);
    if(ret==-(30+4)){
      pop_debug_print("popConnect() [ret=%d] :  Certification is required\n", ret);
      mascot->mail.status = POP3_SSL_CERT;
    }
    else{
      fprintf(stderr, "ERR: popConnect() [ret=%d]\n", ret);
      mascot->mail.status = POP3_ERROR;
      pop3_error(mascot);
    }
    mascot->mail.pop_child_fl=FALSE;
    mascot->mail.pop_readed=TRUE;
    _endthreadex(0);
    //ExitThread(0);
    //return(POP3_ERROR);
  }

  pop_debug_print("POP LOGIN %s %s %s\n",mascot->mail.pop_id, mascot->mail.pop_pass,
	 apop_key);
  ret = popLogin(mascot->mail.pop_id, mascot->mail.pop_pass, apop_key, mascot->mail.ssl_mode);
  if( ret != 0 ){
    if(apop_key) g_free(apop_key);
    fprintf(stderr, "ERR: popLogin() [ret=%d]\n", ret);
    mascot->mail.status = POP3_ERROR;
    popQuit(mascot->mail.ssl_mode);
    popClose();
    pop3_error(mascot);
    mascot->mail.pop_child_fl=FALSE;
    mascot->mail.pop_readed=TRUE;
    _endthreadex(0);
    //ExitThread(0);
    //return(POP3_ERROR);
  }

  pop_debug_print("POP STAT\n");

  ret = popStat(&num, &size, mascot->mail.ssl_mode);
  if( ret != 0 ){
    if(apop_key) g_free(apop_key);
    fprintf(stderr, "ERR: popStat() [ret=%d]\n", ret);
    mascot->mail.status = POP3_ERROR;
    popQuit(mascot->mail.ssl_mode);
    popClose();
    pop3_error(mascot);
    mascot->mail.pop_child_fl=FALSE;
    mascot->mail.pop_readed=TRUE;
    _endthreadex(0);
    //ExitThread(0);
    //return(POP3_ERROR);
  }

  pop_debug_print("mail: %d, size: %ld\n", num, size);

  if(num == 0){  // メール無し
    mascot->mail.status = NO_MAIL;
    mascot->mail.fetched_count = 0;
    mascot->mail.spam_count = 0;
    mascot->mail.pop3_fs_status = POP3_OK_NORMAL;
    if(mascot->mail.last_f) g_free(mascot->mail.last_f);
    mascot->mail.last_f = NULL;
    if(mascot->mail.last_s) g_free(mascot->mail.last_s);
    mascot->mail.last_s =  NULL;
    if(pop_froms) g_free(pop_froms);
    pop_froms = strbuf(NULL);
  }
  else{
    wo_spam = mascot->mail.count - mascot->mail.spam_count;

    if(num == mascot->mail.count){  // メール増減なし
      mascot->mail.status = KEEP_NEW_MAIL;
      mascot->mail.fetched_count = 0;
      mascot->mail.pop3_fs_status = POP3_OK_NORMAL;
    }
    else if(num > mascot->mail.count){  // メール増えた
      mascot->mail.status = NEW_MAIL;
      fs_get_pop3(num,mascot); // ここでSPAMの数, pop_fromsが更新される
      // for WIN32 fs_get_pop3 should not initialize pop_froms
    }
    else{  // メール数が減っていたらすべて新規メールと見なす
      mascot->mail.status = NEW_MAIL;      
      if(mascot->mail.last_f) g_free(mascot->mail.last_f);
      mascot->mail.last_f = NULL;
      if(mascot->mail.last_f) g_free(mascot->mail.last_s);
      mascot->mail.last_s =  NULL;
      if(pop_froms) g_free(pop_froms);
      pop_froms = strbuf(NULL);
      mascot->mail.count = 0;
      mascot->mail.spam_count = 0;
      fs_get_pop3(num,mascot);
    }

    if( num == mascot->mail.spam_count){
      mascot->mail.status = NO_MAIL; // SPAMだけのとき
      if(mascot->mail.last_f) g_free(mascot->mail.last_f);
      mascot->mail.last_f = NULL;
      if(mascot->mail.last_s) g_free(mascot->mail.last_s);
      mascot->mail.last_s =  NULL;
      if(pop_froms) g_free(pop_froms);
      pop_froms = strbuf(NULL);
    }
    else if((num - mascot->mail.spam_count) == wo_spam){
      mascot->mail.status = KEEP_NEW_MAIL; // 増えたのがSPAMだけだったとき
    }
  }


  pop_debug_print("data read end\n");

  popQuit(mascot->mail.ssl_mode);
  popClose();

  pop_debug_print("pop quit\n");

  mascot->mail.count = num;

  /*************** from pop3 data read *********/

  // Should be in parent process in UNIX ver

  pop_debug_print("POP Thread: pop3 data read\n");

  if( mascot->mail.status == POP3_ERROR ){  // POP3 status is ERR
    pop3_error(mascot);
  }
#ifdef USE_SSL
  else if( mascot->mail.status == POP3_SSL_CERT){  // SSL Certification
    //mascot->mail.ssl_cert_res=ssl_manager_verify_cert(mascot);
  }
#endif
  else{
    if(mascot->mail.last_f!=NULL){
      if(g_utf8_validate(mascot->mail.last_f,-1,NULL)){
	strip_last_ret(mascot->mail.last_f);
      }
      else{
	mascot->mail.last_f = g_strdup(_("(Decode Error)"));
      }
    }

    if(mascot->mail.last_s!=NULL){
      if(g_utf8_validate(mascot->mail.last_s,-1,NULL)){
	strip_last_ret(mascot->mail.last_s);
      }
      else{
	mascot->mail.last_s = g_strdup(_("(Decode Error)"));
      }
    }
  }

  if(mascot->mail.count == 0){
    mascot->mail.displayed_count = 0;
  }
  else {
    if(mascot->mail.pop3_fs_status == POP3_OK_NORMAL){
      if(pre_mail_count > mascot->mail.count){  //メール減った
	mascot->mail.displayed_count = mascot->mail.fetched_count; 
      } 
      else { // added or no changed
	mascot->mail.displayed_count += mascot->mail.fetched_count; 
      }

    }
    else if(mascot->mail.pop3_fs_status == POP3_OK_FS_OVER){
      gchar buf[BUFFSIZE], *tmp_froms;
      //ここは無条件で fs clear してよい
      mascot->mail.displayed_count = mascot->mail.fetched_count; 
      
      sprintf(buf,_(" \n     ***** %d mails are skipped *****\n \n"),
	      mascot->mail.count - mascot->mail.displayed_count);

      tmp_froms=strdup(pop_froms);
      if(pop_froms) g_free(pop_froms);
      pop_froms=strbuf(NULL);
      pop_froms=strbuf(buf);
      pop_froms=strbuf(tmp_froms);
      if(tmp_froms) g_free(tmp_froms);
    }
  }

  mascot->mail.pop_child_fl=FALSE;
  mascot->mail.pop_readed=TRUE;
  
  pop_debug_print("POP Thread: status = %d\n",mascot->mail.status);
  pop_debug_print("POP Thread: pop3 fs status = %d\n",mascot->mail.pop3_fs_status);
  pop_debug_print("POP Thread: count = %d\n",mascot->mail.count);
  pop_debug_print("POP Thread: fetched count = %d\n",mascot->mail.fetched_count);
  pop_debug_print("POP Thread: disped count = %d\n",mascot->mail.displayed_count);
  pop_debug_print("POP Thread: spam count = %d\n",mascot->mail.spam_count);
  if(mascot->mail.last_f!=NULL)
    pop_debug_print("Last From:       %s\n",mascot->mail.last_f);
  if(mascot->mail.last_s!=NULL)
    pop_debug_print("Last Subject:    %s\n",mascot->mail.last_s);
  if(pop_froms!=NULL)
    pop_debug_print("data2 = %s\n",pop_froms);

  pop_debug_print("POP Thread: Done\n");


  /*************** end of  pop3 data read *********/


  if(apop_key) g_free(apop_key);
  _endthreadex(0);
  //ExitThread(0);
  //return(mascot->mail.status);
}
#else
int get_pop3(typMascot *mascot)
{
  signed int ret;
  int num;
  long size;
  gchar *apop_key;
  int wo_spam;

  pop_debug_print("Conncet %s : %d\n", mascot->mail.pop_server, mascot->mail.pop_port);

  apop_key=g_malloc0(sizeof(gchar)*BUFFSIZE);
  memset(apop_key, '\0', BUFFSIZE);
  ret = popConnect(mascot->mail.pop_server, mascot->mail.pop_port, apop_key, 
		   mascot->mail.ssl_mode, mascot->mail.ssl_nonblock, 
		   mascot->mail.ssl_cert_res,
		   &mascot->mail.ssl_sub, &mascot->mail.ssl_iss,
		   &mascot->mail.ssl_verify);
  if( ret != 0 ){
    if(apop_key) g_free(apop_key);
    if(ret==-(30+4)){
      pop_debug_print("popConnect() [ret=%d] :  Certification is required\n", ret);
      mascot->mail.status = POP3_SSL_CERT;
      return(POP3_SSL_CERT);
    }
    else{
      fprintf(stderr, "ERR: popConnect() [ret=%d]\n", ret);
      mascot->mail.status = POP3_ERROR;
      return(POP3_ERROR);
    }
  }

  pop_debug_print("POP LOGIN %s %s %s\n",mascot->mail.pop_id, mascot->mail.pop_pass,
	 apop_key);

  ret = popLogin(mascot->mail.pop_id, mascot->mail.pop_pass, apop_key, mascot->mail.ssl_mode);
  if( ret != 0 ){
    fprintf(stderr, "ERR: popLogin() [ret=%d]\n", ret);
    mascot->mail.status = POP3_ERROR;
    popQuit(mascot->mail.ssl_mode);
    popClose();
    if(apop_key) g_free(apop_key);
    return(POP3_ERROR);
  }
  
  pop_debug_print("POP STAT\n");

  ret = popStat(&num, &size, mascot->mail.ssl_mode);
  if( ret != 0 ){
    fprintf(stderr, "ERR: popStat() [ret=%d]\n", ret);
    mascot->mail.status = POP3_ERROR;
    popQuit(mascot->mail.ssl_mode);
    popClose();
    if(apop_key) g_free(apop_key);
    return(POP3_ERROR);
  }

  pop_debug_print("mail: %d, size: %ld\n", num, size);

  if(num == 0){  // メール無し
    mascot->mail.status = NO_MAIL;
    mascot->mail.fetched_count = 0;
    mascot->mail.spam_count = 0;
    mascot->mail.pop3_fs_status = POP3_OK_NORMAL;
    if(mascot->mail.last_f) g_free(mascot->mail.last_f);
    mascot->mail.last_f = NULL;
    if(mascot->mail.last_s) g_free(mascot->mail.last_s);
    mascot->mail.last_s =  NULL;
    if(pop_froms) g_free(pop_froms);
    pop_froms = strbuf(NULL);
  }
  else{
    wo_spam = mascot->mail.count - mascot->mail.spam_count;

    if(num == mascot->mail.count){  // メール増減なし
      mascot->mail.status = KEEP_NEW_MAIL;
      mascot->mail.fetched_count = 0;
      mascot->mail.pop3_fs_status = POP3_OK_NORMAL;
      if(pop_froms) g_free(pop_froms);  // 前の fs をクリア
      pop_froms = strbuf(NULL);
    }
    else if(num > mascot->mail.count){  // メール増えた
      mascot->mail.status = NEW_MAIL;
      fs_get_pop3(num,mascot); // ここでSPAMの数が更新される
    }
    else{  // メール数が減っていたらすべて新規メールと見なす
      mascot->mail.status = NEW_MAIL;      
      if(mascot->mail.last_f) g_free(mascot->mail.last_f);
      mascot->mail.last_f = NULL;
      if(mascot->mail.last_s) g_free(mascot->mail.last_s);
      mascot->mail.last_s =  NULL;
      if(pop_froms) g_free(pop_froms);
      pop_froms = strbuf(NULL);
      mascot->mail.count = 0;
      mascot->mail.spam_count = 0;
      fs_get_pop3(num,mascot);
    }

    if( num == mascot->mail.spam_count){
      mascot->mail.status = NO_MAIL; // SPAMだけのとき
    }
    else if((num - mascot->mail.spam_count) == wo_spam){
      mascot->mail.status = KEEP_NEW_MAIL; // 増えたのがSPAMだけだったとき
    }
  }

  pop_debug_print("data read end\n");

  popQuit(mascot->mail.ssl_mode);
  popClose();
  if(apop_key) g_free(apop_key);

  pop_debug_print("pop quit\n");

  mascot->mail.count = num;
  return(mascot->mail.status);
}
#endif //USE_WIN32

void pop3_error(typMascot *mascot){
  if( mascot->mail.status < 0){  // POP3 status is ERR
    fprintf(stderr,"POP3 error\n");
    mascot->mail.count = 0;
    mascot->mail.spam_count = 0;
    mascot->mail.fetched_count = 0;
    mascot->mail.displayed_count = 0;
    mascot->mail.status = POP3_ERROR;
    if(mascot->mail.last_f) g_free(mascot->mail.last_f);
    mascot->mail.last_f = NULL;
    if(mascot->mail.last_s) g_free(mascot->mail.last_s);
    mascot->mail.last_s =  NULL;
    if(pop_froms) g_free(pop_froms);
    pop_froms = strbuf(NULL);
  }
}

#ifndef USE_WIN32
void pop3_signal(int sig){

  pop_debug_print("get child end\n");

  pop3_data_read(Mascot);
}

void pop3_data_read(typMascot *mascot)
{
  FILE *fp;
  char buf[BUFFSIZE];
  int pre_mail_count;
  gint i_step;
  pid_t child_pid=0;
  
  if(mascot->mail.pop_child_fl){
    pre_mail_count = mascot->mail.count;  // store mail count

    pop_debug_print("Parent: pop3 data read\n");

    if(close(pop3_fd[1])==-1) fprintf(stderr,"pipe close error\n");
    pop_debug_print("Parent: pop3_fd[1] closed\n");
    if( (fp = fdopen( pop3_fd[0], "r" )) == NULL ){
      fprintf(stderr,"pipe open error\n");    
    }
    else{
      if(fgets( buf,BUFFSIZE-1,fp )==NULL) exit(1);  // read mail status
      mascot->mail.status = atoi(buf);
      if( mascot->mail.status == POP3_ERROR){  // POP3 status is ERR
	pop3_error(mascot);
      }
#ifdef USE_SSL
      else if( mascot->mail.status == POP3_SSL_CERT){  // SSL Certification
	if(mascot->mail.ssl_sub) g_free(mascot->mail.ssl_sub);
	if(fgets( buf,BUFFSIZE-1,fp )==NULL) exit(1);
	if(strlen(buf)>=2){
	  mascot->mail.ssl_sub = g_strdup(buf);
	  strip_last_ret(mascot->mail.ssl_sub);
	}
	else{
	  mascot->mail.ssl_sub = NULL;
	}

	if(mascot->mail.ssl_iss) g_free(mascot->mail.ssl_iss);
	if(fgets( buf,BUFFSIZE-1,fp )==NULL) exit(1);
	if(strlen(buf)>=2){
	  mascot->mail.ssl_iss = g_strdup(buf);
	  strip_last_ret(mascot->mail.ssl_iss);
	}
	else{
	  mascot->mail.ssl_iss = NULL;
	}
	
	if(fgets( buf,BUFFSIZE-1,fp )==NULL) exit(1);  // SSL verify_result
	mascot->mail.ssl_verify = atoi(buf);
      }
#endif
      else{
	if(fgets( buf,BUFFSIZE-1,fp )==NULL) exit(1);  // read total mail count
	mascot->mail.count = atoi(buf);
	if(fgets( buf,BUFFSIZE-1,fp )==NULL) exit(1);  // read fetched mail count
	mascot->mail.fetched_count = atoi(buf);
	if(fgets( buf,BUFFSIZE-1,fp )==NULL) exit(1);  // read pop status
	mascot->mail.pop3_fs_status = atoi(buf);
	if(fgets( buf,BUFFSIZE-1,fp )==NULL) exit(1);  // read SPAM count
	mascot->mail.spam_count = atoi(buf);

	if(fgets( buf,BUFFSIZE-1,fp )==NULL) exit(1);  // Last From
	if(mascot->mail.last_f) g_free(mascot->mail.last_f);  
	if(strlen(buf)>=2){
	  if(g_utf8_validate(buf,-1,NULL)){
	    mascot->mail.last_f = g_strdup(buf);
	    strip_last_ret(mascot->mail.last_f);
	  }
	  else{
	    mascot->mail.last_f = g_strdup(_("(Decode Error)"));
	  }
	}
	else{
	  mascot->mail.last_f = NULL;
	}
	if(fgets( buf,BUFFSIZE-1,fp )==NULL) exit(1);  // Last Sub
	if(mascot->mail.last_s) g_free(mascot->mail.last_s);  
	if(strlen(buf)>=2){
	  if(g_utf8_validate(buf,-1,NULL)){
	    mascot->mail.last_s = g_strdup(buf);
	    strip_last_ret(mascot->mail.last_s);
	  }
	  else{
	    mascot->mail.last_s = g_strdup(_("(Decode Error)"));
	  }
	}
	else{
	  mascot->mail.last_s = NULL;
	}

	if(mascot->mail.count == 0){
	  if(pop_froms) g_free(pop_froms);  // fs data clear
 	  pop_froms = strbuf(NULL);
	  mascot->mail.displayed_count = 0;
	}
	else {
	  if(mascot->mail.pop3_fs_status == POP3_OK_NORMAL){
	    if(pre_mail_count > mascot->mail.count){  //メール減った
	      // メール減った場合は全部取り直しなので fs clear
	      mascot->mail.displayed_count = mascot->mail.fetched_count; 
	      if(pop_froms) g_free(pop_froms);
	      pop_froms = strbuf(NULL);
	    } 
	    else {
	      mascot->mail.displayed_count += mascot->mail.fetched_count; 
	    }

	    while( fgets( buf,BUFFSIZE-1,fp ) != NULL ){
	      if(strlen(buf)<5){
		pop_debug_print("Parent: abnormal prms?\n");
		break;
	      }
	      pop_froms = strbuf( buf );
	    }
	  }
	  else if(mascot->mail.pop3_fs_status == POP3_OK_FS_OVER){
	    //ここは無条件で fs clear してよい
	    mascot->mail.displayed_count = mascot->mail.fetched_count; 

	    if(pop_froms) g_free(pop_froms);    // fs data clear
	    pop_froms = strbuf(NULL);
	    sprintf(buf,_(" \n     ***** %d mails are skipped *****\n \n"),
		    mascot->mail.count - mascot->mail.displayed_count);
	    pop_froms = strbuf( buf );
	    while( fgets( buf,BUFFSIZE-1,fp ) != NULL ){
	      if(strlen(buf)<5){
		pop_debug_print("Parent: abnormal prms?\n");
		break;
	      }
	      pop_froms = strbuf( buf );
	    }
	  }
	}
      }
      if(close(pop3_fd[0])==-1) fprintf(stderr,"pipe close error\n");
      fclose( fp );
    }
    //if(close(pop3_fd[0])==-1) fprintf(stderr,"pipe close error\n");

    //gdk_flush();
    //display_biff_balloon(mascot);
    //gdk_flush();
    mascot->mail.pop_child_fl=FALSE;
    mascot->mail.pop_readed=TRUE;


    pop_debug_print("Parent: status = %d\n",mascot->mail.status);
    pop_debug_print("Parent: pop3 fs status = %d\n",mascot->mail.pop3_fs_status);
    pop_debug_print("Parent: count = %d\n",mascot->mail.count);
    pop_debug_print("Parent: fetched count = %d\n",mascot->mail.fetched_count);
    pop_debug_print("Parent: disped count = %d\n",mascot->mail.displayed_count);
    pop_debug_print("Parent: spam count = %d\n",mascot->mail.spam_count);
    if(mascot->mail.last_f!=NULL)
      pop_debug_print("Last From:       %s\n",mascot->mail.last_f);
    if(mascot->mail.last_s!=NULL)
      pop_debug_print("Last Subject:    %s\n",mascot->mail.last_s);
    if(pop_froms!=NULL)
      pop_debug_print("data2 = %s\n",pop_froms);

    do{
      int child_ret;
      child_pid=waitpid(pop3_pid, &child_ret,WNOHANG);
    } while((child_pid>0)||(child_pid!=-1));
  }
}
#endif

// メールボックスから From: Subject: を取得
gchar * fs_get_mbox(typMascot *mascot){
  FILE *fp;
  char buf[BUFFSIZE*16 ],bufs[BUFFSIZE5*16];
  gchar *froms=NULL;
  char *p;
  gboolean ed_fl=FALSE, spam_flag;
  fpos_t pos;
  int i; 
  char *f, *s, *sdel;
  
  froms=strbuf(NULL);
  mascot->mail.spam_count=0;
  
  if((fp=fopen(mascot->mail.file,"r"))!= NULL){
    do{
      if(fgets(buf,BUFFSIZE-1,fp)==NULL) break;
      if(strncmp(buf,"From ",5)==0){  // １つのメールの始まり
	f=s=NULL;
	spam_flag = FALSE;
	for(;;){
	  if((p=fgets(buf,BUFFSIZE-1,fp))==NULL
	     ||buf[0]=='\n'||buf[0]=='\0')
	    break;
	  if(g_ascii_strncasecmp(buf,"from:",5)==0)
	    {
	      strcpy(bufs,buf);
	      ///***for(i=0;i<10;i++){
	      while(1){
		fgetpos(fp,&pos);
		if((p=fgets(buf,BUFFSIZE-1,fp))==NULL
		   ||buf[0]=='\n'||buf[0]=='\0'){
		  ed_fl=TRUE;
/* tnaka start */
		  if( strlen(bufs)) {
		    f = g_strdup(bufs);
		    fsetpos(fp,&pos);
		  }
/* tnaka end */
		  break;
		}
		if(bufs[strlen(bufs)-1]!='\n'){
		  strcat(bufs,buf); /* 一行が200byte以上 */
		}

		else if((buf[0]==' ') || (buf[0]=='\t')){
		  bufs[strlen(bufs)-1]='\0';
		  sdel = &buf[1];     /* \t を上書き */
		  strcat(bufs,sdel); /* 複数行 */
		}
		else{
		  f=g_strdup(bufs);
		  fsetpos(fp,&pos);
		  break;
		}
	      }
	      if(ed_fl) break;
	    }
	  else if(g_ascii_strncasecmp(buf,"subject:",8)==0)
	    {
	      strcpy(bufs,buf);
	      ///***for(i=0;i<10;i++){
	      while(1){
		fgetpos(fp,&pos);
		if((p=fgets(buf,BUFFSIZE-1,fp))==NULL
		   ||buf[0]=='\n'||buf[0]=='\0'){
		  ed_fl=TRUE;
/* tnaka start */
		  if( strlen(bufs)) {
		    s = g_strdup(bufs);
		    fsetpos(fp,&pos);
		  }
/* tnaka end */
		  break;
		}
		if(bufs[strlen(bufs)-1]!='\n'){
		  strcat(bufs,buf); /* 一行が200byte以上 */
		}
		else if((buf[0]==' ') || (buf[0]=='\t')){
		  bufs[strlen(bufs)-1]='\0';
		  sdel = &buf[1];     /* \t を上書き */
		  strcat(bufs,sdel); /* 複数行 */
		}
		else{
		  s=g_strdup(bufs);
		  fsetpos(fp,&pos);
		  break;
		}
	      }
	      if(ed_fl) break;
	    } 
	  else if(strncmp(buf, mascot->mail.spam_mark, 
			  strlen(mascot->mail.spam_mark))==0){
	    spam_flag = TRUE;
	    mascot->mail.spam_count++;
	  }
	}
	
	//
	// From: Subject:
	// の順に表示させる
	//
	if ( (!spam_flag) || (!mascot->mail.spam_check) ){
	  if (f) {
	    conv_unmime_header_overwrite(f);
	    froms=strbuf(f);
	    g_free(f);
	  }
	  else{
	    froms=strbuf("From: (no From: in original)\n");
	  }
	  if (s) {
	    froms=strbuf(" ");
	    conv_unmime_header_overwrite(s);
	    froms=strbuf(s);
	    g_free(s);
	  }
	  else{
	    froms=strbuf(" Subject: (no Subject: in original)\n");
	  }
	}
ed_fl = FALSE;  /* tnaka */
      }
      while (p != NULL && buf[0] != '\n' && buf[0] != '\0')
	p = fgets(buf, BUFFSIZE - 1, fp);
    } while (1);
    fclose(fp);
  }
  return(froms);
}


// MH + Procmail から
gchar *  fs_get_procmail(typMascot  *mascot){
    FILE *fp,*fp_folder;
    gchar buf[BUFFSIZE],tmp[10],bufs[BUFFSIZE5];
    gchar *folder_file,folder_tmp[BUFFSIZE];
    gchar *froms=NULL, *p;
    gboolean ed_fl=FALSE;
    fpos_t pos;
    int i;
    gchar *f, *s, *sdel;


    froms=strbuf(NULL);

    if((fp=fopen(mascot->mail.file,"r"))==NULL){
	return;
    }
    
    while(!feof(fp)){
	f=s=NULL;
	if(fgets(buf,BUFFSIZE-1,fp)==NULL) break;
	if(strncmp(buf,"  Folder:",9)==0){
	    sscanf(buf,"%s%s",tmp,folder_tmp);
	    folder_file=g_strconcat(set_mhdir(),G_DIR_SEPARATOR,
				    folder_tmp,NULL);

	    if((fp_folder=fopen(folder_file,"r"))!=NULL){
		while(!feof(fp_folder)){
		    if((p=fgets(buf,BUFFSIZE-1,fp_folder))==NULL
		       ||buf[0]=='\n'||buf[0]=='\0')
			break;
		    if(g_ascii_strncasecmp(buf,"from:",5)==0)
		    {
			strcpy(bufs,buf);
			///***for(i=0;i<10;i++){
			while(1){
			    fgetpos(fp_folder,&pos);
			    if((p=fgets(buf,BUFFSIZE-1,fp_folder))==NULL
			       ||buf[0]=='\n'||buf[0]=='\0'){
				ed_fl=TRUE;
				break;
			    }
			    if(bufs[strlen(bufs)-1]!='\n'){
				strcat(bufs,buf); /* 一行が200byte以上 */
			    }
			    else if((buf[0]==' ') || (buf[0]=='\t')){
				bufs[strlen(bufs)-1]='\0';
				sdel = &buf[1];     /* \t を上書き */
				strcat(bufs,sdel); /* 複数行 */
			    }
			    else{
			        f=g_strdup(bufs);
				fsetpos(fp_folder,&pos);
				break;
			    }
			}
			if(ed_fl) break;
		    }
		    else if(g_ascii_strncasecmp(buf,"subject:",8)==0)
		    {
			strcpy(bufs,buf);
			///***for(i=0;i<10;i++){
			while(1){
			    fgetpos(fp_folder,&pos);
			    if((p=fgets(buf,BUFFSIZE-1,fp_folder))==NULL
			       ||buf[0]=='\n'||buf[0]=='\0'){
				ed_fl=TRUE;
				break;
			    }
			    if(bufs[strlen(bufs)-1]!='\n'){
				strcat(bufs,buf); /* 一行が200byte以上 */
			    }
			    else if((buf[0]==' ') || (buf[0]=='\t')){
				bufs[strlen(bufs)-1]='\0';
				sdel = &buf[1];     /* \t を上書き */
				strcat(bufs,sdel); /* 複数行 */
			    }
			    else{
			        s=g_strdup(bufs);
				fsetpos(fp_folder,&pos);
				break;
			    }
			}
			if(ed_fl) break;
		    } 
		}
		fclose(fp_folder);
		//
		// From: Subject:
		// の順に表示させる
		//
		if (f) {
		  conv_unmime_header_overwrite(f);
		  froms=strbuf(f);
		  g_free(f);
		}
		else{
		    froms=strbuf("From: (no From: in original)\n");
		}
		if (s) {
		  froms=strbuf(" ");
		  conv_unmime_header_overwrite(s);
		  froms=strbuf(s);
		  g_free(s);
		}
		else{
		    froms=strbuf(" Subject: (no Subject: in original)\n");
		}
	    }
	}
	
    }
    fclose(fp);

    if(folder_file) g_free(folder_file);

    return(froms);
}	


// Qmail から
gchar * fs_get_qmail(typMascot *mascot){
    FILE *fp_folder;
    gchar buf[BUFFSIZE],tmp[10],bufs[BUFFSIZE5];
    gchar folder_file[BUFFSIZE],folder_tmp[BUFFSIZE];
    gchar *froms=NULL, *p;
    gboolean ed_fl=FALSE, spam_flag;
    fpos_t pos;
    int i;
    DIR *dp;
    struct dirent *entry;

    gchar *f, *s, *sdel;

    mascot->mail.spam_count=0;
    froms=strbuf(NULL);

    if ((dp=opendir(mascot->mail.file))==NULL){
	return;
    }	
    

    while((entry=readdir(dp))!=NULL){
	f=s=NULL;
	if(entry->d_name[0]!='.'){
	    sprintf(folder_file,"%s/%s",mascot->mail.file,entry->d_name);

	    if((fp_folder=fopen(folder_file,"r"))!=NULL){
	        spam_flag = FALSE;
                while(!feof(fp_folder)){
		  if((p=fgets(buf,BUFFSIZE-1,fp_folder))==NULL
		     ||buf[0]=='\n'||buf[0]=='\0')
		    break;

		  if(strncmp(buf, mascot->mail.spam_mark, 
			     strlen(mascot->mail.spam_mark))==0){
		    spam_flag = TRUE;
		    mascot->mail.spam_count++;
		  }
		}
		if( (spam_flag) && (mascot->mail.spam_check) ){
		  fclose(fp_folder);
		  continue;
		}

		rewind(fp_folder);
		while(!feof(fp_folder)){
		    if((p=fgets(buf,BUFFSIZE-1,fp_folder))==NULL
		       ||buf[0]=='\n'||buf[0]=='\0')
			break;
		    if(g_ascii_strncasecmp(buf,"from:",5)==0)
                    {
			strcpy(bufs,buf);
			///***for(i=0;i<10;i++){
			while(1){
			    fgetpos(fp_folder,&pos);
			    if((p=fgets(buf,BUFFSIZE-1,fp_folder))==NULL
			       ||buf[0]=='\n'||buf[0]=='\0'){
				ed_fl=TRUE;
				break;
			    }
			    if(bufs[strlen(bufs)-1]!='\n'){
			      strcat(bufs,buf); // 一行が200byte以上
			    }
			    else if((buf[0]==' ') || (buf[0]=='\t')){
				bufs[strlen(bufs)-1]='\0';
				sdel = &buf[1];     // \t を上書き
				strcat(bufs,sdel); // 複数行
			    }
			    else{
			        f=g_strdup(bufs);
				fsetpos(fp_folder,&pos);
				break;
			    }
			}
			if(ed_fl) break;
		    }
		    else if(g_ascii_strncasecmp(buf,"subject:",8)==0)
                    {
			strcpy(bufs,buf);
			///***for(i=0;i<10;i++){
			while(1){
			    fgetpos(fp_folder,&pos);
			    if((p=fgets(buf,BUFFSIZE-1,fp_folder))==NULL
			       ||buf[0]=='\n'||buf[0]=='\0'){
				ed_fl=TRUE;
				break;
			    }
			    if(bufs[strlen(bufs)-1]!='\n'){
				strcat(bufs,buf); /* 一行が200byte以上 */
			    }
			    else if((buf[0]==' ') || (buf[0]=='\t')){
				bufs[strlen(bufs)-1]='\0';
				sdel = &buf[1];     /* \t を上書き */
				strcat(bufs,sdel); /* 複数行 */
			    }
			    else{
				s=g_strdup(bufs);
				fsetpos(fp_folder,&pos);
				break;
			    }
			}
			if(ed_fl) break;
		    }
		}
		fclose(fp_folder);
		//
		// From: Subject:
		// の順に表示させる
		//
		if (f) {
		  conv_unmime_header_overwrite(f);
		  froms=strbuf(f);
		  g_free(f);
		}
		else{
		    froms=strbuf("From: (no From: in original)\n");
		}
		if (s) {
		    froms=strbuf(" ");
		    conv_unmime_header_overwrite(s);
		    froms=strbuf(s);
		    g_free(s);
		}
		else{
		    froms=strbuf(" Subject: (no Subject: in original)\n");
		}

	    }
	}
	
    }
    closedir(dp);

    return(froms);
}


// pop3 server から From: Subject: を取得
void fs_get_pop3(int num, typMascot *mascot){

  int funcret;
  char buffer_header[HEADER_MAX][POP_MAX_LINE], bufs[BUFFSIZE5];
  char buffer[POP_MAX_LINE];
  int mail_cnt, header, header_line, i, j;
  int mail_cnt_start;
  char *f, *s, *sdel, tmp_fs[256];
  int disp=0;
  gboolean spam_flag;
#ifndef USE_WIN32
  char *tmp_froms;
  
  tmp_froms=strbuf(NULL); 
#endif

  pop_debug_print("fs read num = %d mail_count = %d\n", 
	  num, mascot->mail.count); 

  if((num - mascot->mail.count) > mascot->mail.pop_max_fs){  //overflow
    mail_cnt_start = num - mascot->mail.pop_max_fs +1;
    mascot->mail.fetched_count = mascot->mail.pop_max_fs;
    mascot->mail.pop3_fs_status = POP3_OK_FS_OVER;
  }
  else {    // under
    mail_cnt_start = mascot->mail.count +1;
    mascot->mail.fetched_count = num - mascot->mail.count;
    mascot->mail.pop3_fs_status = POP3_OK_NORMAL;
  }

  pop_debug_print("fs read num = %d mail_cnt_start = %d\n", 
	  num, mail_cnt_start); 
  pop_debug_print("fs mail_fetched = %d\n", mascot->mail.fetched_count); 

  for(mail_cnt = mail_cnt_start; mail_cnt <= num; mail_cnt++){

    sprintf(buffer, "TOP %d %d\r\n", mail_cnt, 0);  // get header only

    funcret = popWriteLine(buffer, mascot->mail.ssl_mode);
    if( funcret != 0 ){
      fprintf(stderr,"write err = %d\n",funcret);
#ifndef USE_WIN32
      mascot->mail.status=POP3_ERROR;
#endif
      return;
    }
    for(header=0; header < HEADER_MAX; header++){
      funcret = popReadLine(buffer_header[header], POP_MAX_LINE, mascot->mail.ssl_mode);
      //pop_debug_print("%d %d   %s\n",mail_cnt,header,buffer_header[header]);
      if( funcret != 0 ){
	fprintf(stderr,"read err\n");
#ifndef USE_WIN32
      mascot->mail.status=POP3_ERROR;
#endif
	return;
      }
      if( strcmp(buffer_header[header], ".\r\n") == 0 )      break;
      buffer_header[header][strlen(buffer_header[header])-2]=' ';  // \r delete
    }
    header_line = header-1;

    spam_flag = FALSE;
    if(mascot->mail.spam_check){
      for(header=0; header < header_line; header++){
	if(strncmp(buffer_header[header], mascot->mail.spam_mark, 
		   strlen(mascot->mail.spam_mark))==0){
	  spam_flag = TRUE;
	  mascot->mail.spam_count++;
	  
	  pop_debug_print("SPAM detected = %d\n", mascot->mail.spam_count); 
	}
      }
    }
    if( (spam_flag) && (mascot->mail.spam_check) )  continue;

    f=s=NULL;
    for(header=0; header < header_line; header++){
      if(g_ascii_strncasecmp(buffer_header[header],"from:",5)==0)
      {
	strcpy(bufs, buffer_header[header]);
	//bufs=g_strconcat(buffer_header[header],NULL);
	///***for(i=1;i<10;i++){
	i=1;
	while(1){
	  if((i+header) > header_line) break;
	  if((buffer_header[i+header][0]==' ') || (buffer_header[i+header][0]=='\t')){
	    bufs[strlen(bufs)-2]='\0';
	    sdel = &buffer_header[i+header][1];     /* \t を上書き */
	    strcat(bufs,sdel); /* 複数行 */
	    //bufs=g_strconcat(bufs,sdel,NULL); /* 複数行 */
	    if(f) g_free(f);
	    f=g_strdup(bufs);
	  }
	  else{
	    f=g_strdup(bufs);
	    break;
	  }
	  i++;
	}
      }
      else if(g_ascii_strncasecmp(buffer_header[header],"subject:",8)==0)
      {
	strcpy(bufs, buffer_header[header]);
	//bufs=g_strconcat(buffer_header[header],NULL);
	///***for(i=1;i<10;i++){
	//s=g_strdup(bufs);
	j=1;
	while(1){
	  if((j+header) > header_line) break;
	  if((buffer_header[j+header][0]==' ') || (buffer_header[j+header][0]=='\t')){
	    bufs[strlen(bufs)-2]='\0';
	    sdel = &buffer_header[j+header][1];     // \t を上書き
	    strcat(bufs,sdel); // 複数行
	    //bufs=g_strconcat(bufs,sdel,NULL); // 複数行
	    if(s) g_free(s);
	    s=g_strdup(bufs);
	  }
	  else{
	    s=g_strdup(bufs);
	    break;
	  }
	  j++;
	}
	pop_debug_print("??? ss=%s\n",s);
      }
    }
    


    if (f) {
      conv_unmime_header_overwrite(f);
      {
	gint n;
	for(n=4;n>1;n--){
	  if((f[strlen(f)-n]==0x0D)&&(f[strlen(f)-n+1]==0x0A)){
	    f[strlen(f)-n]=0x20;
	    f[strlen(f)-n+1]=0x20;
	  }
	}
      }
#ifdef USE_WIN32
      pop_froms=strbuf(f);
#else
      tmp_froms=strbuf(f);
#endif
      if(mascot->mail.last_f) g_free(mascot->mail.last_f);
      mascot->mail.last_f=g_strdup(f+strlen("From: "));
      strip_last_ret(mascot->mail.last_f);
      if(f) g_free(f);
    }
    else{
#ifdef USE_WIN32
      pop_froms=strbuf("From: (no From: in original)\n");
#else
      tmp_froms=strbuf("From: (no From: in original)\n");
#endif
    }
    if (s) {
#ifdef USE_WIN32
      pop_froms=strbuf(" ");
#else
      tmp_froms=strbuf(" ");
#endif
      conv_unmime_header_overwrite(s);
      {
	gint n;
	for(n=4;n>1;n--){
	  if((s[strlen(s)-n]==0x0D)&&(s[strlen(s)-n+1]==0x0A)){
	    s[strlen(s)-n]=0x20;
	    s[strlen(s)-n+1]=0x20;
	  }
	}
      }
      pop_debug_print("s=%s\n",s);

#ifdef USE_WIN32
      pop_froms=strbuf(s);
#else
      tmp_froms=strbuf(s);
#endif
      if(mascot->mail.last_s) g_free(mascot->mail.last_s);
      mascot->mail.last_s=g_strdup(s+strlen("Subject: "));
      strip_last_ret(mascot->mail.last_s);
      if(s) g_free(s);
    }
    else{
      pop_debug_print("s=!!!! No Subject !!!!\n");
#ifdef USE_WIN32
      pop_froms=strbuf(" Subject: (no Subject: in original)\n");
#else
      tmp_froms=strbuf(" Subject: (no Subject: in original)\n");
#endif
    }
  }
#ifndef USE_WIN32
  if(pop_froms) g_free(pop_froms);
  pop_froms=strbuf(NULL);
  pop_froms=strbuf(tmp_froms);
  if(tmp_froms) g_free(tmp_froms);
#endif
  pop_debug_print("fs_get_pop3 end\n");
}

//
char* strbuf(char *p)
{
  static char    *start = NULL;
  static char    *next;
  static int      len, len2, l;
  
  if (p == NULL) {
    len = len2 = 4096;
    start = next = (char *) g_malloc(len + 1);
    start[0] = '\0';
  } else if (start != NULL) {
    l = strlen(p);
    while (len - l < 0) {
      char           *p;
      len2 += 256;
      len += 256;
      p = (char *) g_realloc(start, len2 + 1);
      start = p;
      next = p + len2 - len;
                }
    strcpy(next,p);
    len -= l;
    next += l;
  }
  return start;
}



void make_fs_max(GtkWidget *widget, typMascot *mascot){
  GtkWidget *label;
  gchar tmp_fs_max[256];
  gchar tmp_fs[8];

  if(mascot->mail.displayed_count != mascot->mail.count){
    if(mascot->mail.spam_count!=0){
      sprintf(tmp_fs_max,
	      _("Displaying the newest %d mails out of %ds. [%d SPAMs are excluded.]"),
	      mascot->mail.displayed_count,
	      mascot->mail.count, mascot->mail.spam_count);
    }
    else{
      sprintf(tmp_fs_max,
	      _("Displaying the newest %d mails out of %ds."),
	      mascot->mail.displayed_count,
	      mascot->mail.count);
    }
  }
  else{
    if(mascot->mail.spam_count!=0){
      sprintf(tmp_fs_max,_("You have %d new mails.  [%d SPAMs are excluded.]"),
	      mascot->mail.count, mascot->mail.spam_count);
    }
    else{
      sprintf(tmp_fs_max,_("You have %d new mails."),
	      mascot->mail.count);
    }
  }
  label=gtk_label_new(tmp_fs_max);
  gtk_misc_set_alignment (GTK_MISC (label), 1.0, 0);
  gtk_table_attach (GTK_TABLE (widget), label, 0, 5, 1, 2,
		    GTK_FILL, GTK_SHRINK , 0, 0);
}


// メイル着信リストの生成
void create_biff_dialog(typMascot *mascot)
{
  GtkWidget *biff_main;
  GtkWidget *biff_tbl;
  GtkWidget *biff_text;
  GtkWidget *button;
  GtkWidget *biff_scroll;
  gchar *tmp_froms=NULL;
  gchar *tmp;
  gchar *fp_1, *fp_2;
  gchar *p;
  gchar *buf_unmime;
  gchar *err_msg;
  GtkTextBuffer *text_buffer;
  GtkTextIter start_iter, end_iter;
  GtkTextMark *end_mark;
  

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  while (my_main_iteration(FALSE));

  mascot->flag_menu=TRUE;

  
  biff_main = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  text_buffer = gtk_text_buffer_new(NULL);

  gtk_widget_set_usize (biff_main, mascot->mail.win_width, 
  			mascot->mail.win_height);
  if(mascot->mail.type==MAIL_POP3|MAIL_APOP){
    tmp=g_strconcat(_("MaCoPiX : Arrived mail list"),"  [",
		    mascot->mail.pop_server,"]",NULL);
  }
  else{
    tmp=g_strdup(_("MaCoPiX : Arrived mail list"));
  }
  gtk_window_set_title(GTK_WINDOW(biff_main), tmp);
  g_free(tmp);
  gtk_widget_realize(biff_main);
  my_signal_connect(biff_main,"destroy",close_biff, GTK_WIDGET(biff_main));
  gtk_container_set_border_width (GTK_CONTAINER (biff_main), 5);
  
  // 6x3のテーブル
  biff_tbl = gtk_table_new (6, 3, FALSE);
  gtk_container_add (GTK_CONTAINER (biff_main), biff_tbl);

  biff_scroll = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(biff_scroll),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  biff_text = gtk_text_view_new_with_buffer (text_buffer);
  gtk_text_view_set_editable (GTK_TEXT_VIEW (biff_text), FALSE);
  gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (biff_text), FALSE);

  gtk_container_add(GTK_CONTAINER(biff_scroll), biff_text);

  gtk_table_attach_defaults (GTK_TABLE (biff_tbl), biff_scroll, 0, 5, 0, 1);


  switch(mascot->mail.type){
  case MAIL_LOCAL: 
    tmp_froms=fs_get_mbox(mascot);
    break;
  case MAIL_POP3: 
  case MAIL_APOP: 
    tmp_froms=g_strdup(pop_froms);
    make_fs_max(biff_tbl, mascot);
    break;
  case MAIL_QMAIL:
    tmp_froms=fs_get_qmail(mascot);
    break;
  case MAIL_PROCMAIL:
    tmp_froms=fs_get_procmail(mascot);
    break;
  }


  gtk_text_buffer_create_tag (text_buffer, "underline",
			      "underline", PANGO_UNDERLINE_SINGLE, NULL);
  gtk_text_buffer_create_tag (text_buffer, "big_gap_after_line",
			      "pixels_below_lines", 5, NULL);
  gtk_text_buffer_create_tag (text_buffer, "heading",
			      "weight", PANGO_WEIGHT_BOLD,
			      //"size", 15 * PANGO_SCALE,
			      NULL);
  gtk_text_buffer_get_start_iter(text_buffer, &start_iter);

  if((strlen(tmp_froms)<5)||(mascot->mail.status == NO_MAIL)){
    err_msg=g_strdup(_("   === Failed to get From/Subject list of arrived mails. ==="));
    g_locale_to_utf8(err_msg,-1,NULL,NULL,NULL);
    gtk_text_buffer_insert (text_buffer, &start_iter, err_msg,-1);
    g_free(err_msg);
  }
  else{
    p=(gchar *)strtok(tmp_froms,"\n");
    do{
      buf_unmime=g_strdup(p);

      if(g_ascii_strncasecmp(buf_unmime," subject:",9)==0){
	gtk_text_buffer_insert_with_tags_by_name (text_buffer, &start_iter,
						  "Subject: ", -1,
						  "heading",
						  "big_gap_after_line",
						  NULL);
	if(g_utf8_validate(buf_unmime+9,-1,NULL)){
	  gtk_text_buffer_insert_with_tags_by_name (text_buffer, &start_iter,
	  					    buf_unmime+9, -1,
	  					    "underline",
	  					    "big_gap_after_line",
	  					    NULL);
	}
	else
	  gtk_text_buffer_insert (text_buffer, &start_iter, 
				  "(invalid/no subject in original)", -1);
      }
      else if(g_ascii_strncasecmp(buf_unmime,"from:",5)==0){
	gtk_text_buffer_insert_with_tags_by_name (text_buffer, &start_iter,
						  "From: ", -1,
						  "heading",
						  NULL);
	if(g_utf8_validate(buf_unmime+5,-1,NULL))
	  gtk_text_buffer_insert (text_buffer, &start_iter, buf_unmime+5, -1);
	else
	  gtk_text_buffer_insert (text_buffer, &start_iter, 
				  "(invalid/no from in original)", -1);
	
      }
      else{
	if(g_utf8_validate(buf_unmime,-1,NULL))
	  gtk_text_buffer_insert (text_buffer, &start_iter, buf_unmime, -1);
      }
      gtk_text_buffer_insert (text_buffer, &start_iter, "\n", -1);
      if(buf_unmime) g_free(buf_unmime);
    }while((p=(gchar *)strtok(NULL,"\n"))!=NULL);
  }

  gtk_text_buffer_get_end_iter(text_buffer, &end_iter);
  gtk_text_buffer_place_cursor(text_buffer, &end_iter);
  end_mark= gtk_text_buffer_create_mark(text_buffer, "end", &end_iter, FALSE);
  gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(biff_text),
			       end_mark, 0.0, FALSE, 0.0, 0.0);
  gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(biff_text),
			       &end_iter,0.0, FALSE,0.0, 0.0); 

  
  button=gtkut_button_new_from_stock(_("Start Mailer"),GTK_STOCK_EXECUTE);
  gtk_table_attach(GTK_TABLE(biff_tbl), button, 0, 1, 2, 3,
		   GTK_FILL,GTK_SHRINK,0,0);
  my_signal_connect(button,"clicked",mailer_start, GTK_WIDGET(biff_main));


  button=gtkut_button_new_from_stock(_("Close"),GTK_STOCK_CLOSE);
  gtk_table_attach(GTK_TABLE(biff_tbl), button, 4, 5, 2, 3,
		   GTK_FILL,GTK_SHRINK,0,0);
  my_signal_connect(button,"clicked",close_biff, GTK_WIDGET(biff_main));
  
  gtk_widget_show_all(biff_main);
  
  gdk_flush();
}


static void close_biff(GtkWidget *w, GtkWidget *dialog)
{
  //gdk_pointer_ungrab(GDK_CURRENT_TIME);

  //Mascot->mail.win_width=dialog->allocation.width;
  //Mascot->mail.win_height=dialog->allocation.height;

  while (my_main_iteration(FALSE));
  gtk_widget_destroy(GTK_WIDGET(dialog));
  while (my_main_iteration(FALSE));

  Mascot->flag_menu=FALSE;
  gdk_flush();
}


// Biff winからのメイラ起動
static void mailer_start(GtkWidget *w, GtkWidget *dialog)
{
  while (my_main_iteration(FALSE));
  gtk_widget_destroy(GTK_WIDGET(dialog));
  while (my_main_iteration(FALSE));
 
  Mascot->flag_menu=FALSE;

  ext_play(Mascot,Mascot->mail.mailer);
  gdk_flush();
}


gchar* set_mhdir(){
  FILE *fp;
  gchar *c=NULL,buf[256],*mhd=NULL, *tmp;
    
  c=g_strconcat(g_get_home_dir(),PROCMAILRC,NULL);

  if((fp=fopen(c,"r"))!=NULL){
    while(!feof(fp)){
      if(fgets(buf,256-1,fp)==NULL) break;
      if(strncmp(buf,"MAILDIR=",8)==0){
	tmp=buf+8;
	if(strncmp(tmp,"$HOME",5)==0){
	  mhd=g_strconcat(g_get_home_dir(),tmp+5,NULL);
	}
	else{
	  mhd=tmp;
	}
	break;
      }
    }
  }
    
  if(mhd==NULL){
    mhd=g_strconcat(g_get_home_dir(),MH_MAIL_DIR,NULL);
  }
  if(mhd[strlen(mhd)-1]=='\n') mhd[strlen(mhd)-1]='\0';

  if(c) g_free(c);
  
  return mhd;
}

void mail_arg_init(){
  pop_froms=NULL;
  former_newest=0;
}

void strip_last_ret(gchar *p){
  if(p[strlen(p)-1]=='\n') p[strlen(p)-1]='\0';
}


#ifndef USE_WIN32
void kill_pop3(){
  pid_t child_pid=0;

  if(pop3_pid) kill(pop3_pid,SIGKILL);

  do{
    int child_ret;
    child_pid=waitpid(pop3_pid, &child_ret,WNOHANG);
  } while(child_pid>0);
}
#endif

#endif // USE_BIFF
