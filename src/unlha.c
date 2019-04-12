//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//
//
//      unlzh.c
//      Using UNLHA32.DLL for mascots installation (for Windows)
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

#ifdef USE_UNLHA32
#ifdef USE_WIN32
#include <windows.h>
#include "UNLHA32.H"
#endif  // USE_WIN32

gchar* unlha_menu();
void unlha_fail();
void menu_fail();

gchar* unlha_menu(gchar *lzhfile)
{
  HARC hArc;
  INDIVIDUALINFO ii;
  char szCmdLine[2048],buf[2048];
  gchar *lzhdir;
  gchar *menufile;
  
  
  hArc=UnlhaOpenArchive(NULL, lzhfile, M_ERROR_MESSAGE_ON);
  if (hArc == NULL) { 
    unlha_fail(lzhfile);
    exit(1);
  }

  if(UnlhaFindFirst(hArc, MENU_EXTENSION, &ii)!=0){
    menu_fail(lzhfile);
    exit(1);
  }

  UnlhaCloseArchive(hArc);
  
  //lzhdir=g_strconcat(my_dirname(lzhfile),"/",NULL);
  GetTempPath(2048, buf);
  if(access(buf,W_OK)==0){
    lzhdir=g_strdup(buf);
  }
  else{
    lzhdir=g_strconcat(my_dirname(lzhfile),G_DIR_SEPARATOR_S,NULL);
  }
  menufile=g_strconcat(lzhdir,ii.szFileName,NULL);

  strcpy(szCmdLine, "e ");
  strcat(szCmdLine, "\"");
  strcat(szCmdLine, lzhfile);
  strcat(szCmdLine, "\"");
  strcat(szCmdLine, " -x1a1 -c ");
  strcat(szCmdLine, "\"");
  strcat(szCmdLine, lzhdir);
  strcat(szCmdLine, "\"");
  strcat(szCmdLine, " *.*");

  Unlha(NULL, szCmdLine, NULL, 0);

  g_free(lzhdir);

  return(menufile);
} 


void unlha_fail(gchar* lzhfile){
#ifdef GTK_MSG
  popup_message(POPUP_TIMEOUT*2,
		_("Error: File cannot be opened."),
		" ",
		to_utf8(lzhfile),
		NULL);
#else
  g_print (_("Cannot Open %s\n"),to_utf8(lzhfile));
#endif
}

void menu_fail(gchar* lzhfile){
#ifdef GTK_MSG
  popup_message(POPUP_TIMEOUT*2,
		_("Error: A Menu File is not found in"),
		" ",
		to_utf8(lzhfile),
		NULL);
#else
  g_print (_("A Menu Files is not found in %s\n"),to_utf8(lzhfile));
#endif
}
#endif  // USE_UNLHA32
