//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//
//
//      untar.c
///     Using TAR32.DLL for Win32
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


#include "main.h"


#ifdef USE_TAR32
#ifdef USE_WIN32
#include <windows.h>
#include "tar32api.h"
#endif  // USE_WIN32
#else
#include <dirent.h>
#endif //USE_TAR32

#define BUFFSIZE 2048

gchar* untar_menu();
void untar_fail();
void menu_fail();

#ifdef USE_TAR32
gchar* untar_menu(typMascot *mascot, gchar *tarfile)
{
  HARC hArc;
  INDIVIDUALINFO ii;
  char szCmdLine[2048],buf[2048];
  gchar *tardir;
  gchar *menufile, *buf2;
  gboolean eflag;

  hArc=TarOpenArchive(NULL,tarfile, M_ERROR_MESSAGE_ON);
  if (hArc == NULL) { 
    untar_fail(tarfile);
    exit(1);
  }

  eflag=FALSE;

  if(TarFindFirst(hArc, "", &ii)!=-1){
    do {
      if(strncmp(ii.szFileName+(strlen(ii.szFileName)-strlen(MENU_EXTENSION)+1),
		 MENU_EXTENSION+1,strlen(MENU_EXTENSION)-1)==0){
	eflag=TRUE;
	break;
      }
    }while (TarFindNext(hArc, &ii)!=-1);
  }
  else{
    menu_fail(tarfile);
    exit(1);
  }
  
  if(eflag==FALSE){
    menu_fail(tarfile);
    exit(1);
  }

  TarCloseArchive(hArc);
  
  //tardir=g_strconcat(my_dirname(tarfile),"\\",NULL);
 
  GetTempPath(2048, buf);
  if(access(buf,W_OK)==0){
    tardir=g_strdup(buf);
  }
  else{
    tardir=g_strconcat(my_dirname(tarfile),"\\",NULL);
  }

  menufile=g_strconcat(tardir,ii.szFileName,NULL);
  g_strdelimit(menufile,"/",G_DIR_SEPARATOR);

  strcpy(szCmdLine, "-zxf ");
  strcat(szCmdLine, "\"");
  strcat(szCmdLine, tarfile);
  strcat(szCmdLine, "\"");
  strcat(szCmdLine, " -o ");
  strcat(szCmdLine, "\"");
  strcat(szCmdLine, tardir);
  strcat(szCmdLine, "\"");

  Tar(NULL, szCmdLine, NULL, 0);

  g_free(tardir);

  return(menufile);
} 
#endif

#ifdef USE_GTAR
gchar* untar_menu(typMascot *mascot, gchar *tarfile)
{
  char szCmdLine[2048];
  gchar *tardir, *tardir2;
  gchar *arcdir;
  gchar *menufile;
  gchar *cp;
  struct dirent *entry;
  struct stat statbuf;
  DIR *dirp;
  gint ret;


  tardir = g_strdup_printf("%s%smacopix-%d",
			   g_get_tmp_dir(), G_DIR_SEPARATOR_S,
			   getuid());
  
  if (access(tardir, F_OK) != 0) {
    mkdir(tardir,(S_IRWXU|S_IRGRP|S_IROTH|S_IXGRP|S_IXOTH));
  }

  sprintf(szCmdLine,mascot->tar_command,
	  tarfile,
	  tardir);

  ret=system(szCmdLine);

  arcdir = g_strndup(my_basename(tarfile),
		     strlen(my_basename(tarfile))-strlen(TAR_EXTENSION)+1);
  
  tardir2 = g_strconcat(tardir,G_DIR_SEPARATOR_S, arcdir, NULL);
  if((dirp = opendir(tardir2)) != NULL){
    while((entry = readdir(dirp))!=NULL){
      cp=entry->d_name;
      cp+=strlen(entry->d_name)-strlen(MENU_EXTENSION)+1;
      if(!strcmp(MENU_EXTENSION+1,cp)){
	menufile=g_strconcat(tardir2,
			     G_DIR_SEPARATOR_S,
			     entry->d_name,
			     NULL);
	closedir(dirp);
	g_free(tardir);
	g_free(tardir2);
	g_free(arcdir);
	return(menufile);
      }
    }
  }
  
  menu_fail(tarfile);
  exit(1);
}
#endif


void untar_fail(gchar* tarfile){
  popup_message(Mascot->win_main,
#ifdef USE_GTK3
		"dialog-error", 
#else
		GTK_STOCK_DIALOG_ERROR,
#endif
		
		-1,
		_("Error: File cannot be opened."),
		" ",
		to_utf8(tarfile),
		NULL);
}

void menu_fail(gchar* tarfile){
  popup_message(Mascot->win_main,
#ifdef USE_GTK3
		"dialog-error", 
#else
		GTK_STOCK_DIALOG_ERROR,
#endif
		
		-1,
		_("Error: A Menu File is not found in"),
		" ",
		to_utf8(tarfile),
		NULL);
}

