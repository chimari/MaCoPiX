//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//
//
//     alpha.c
//     Using Alpha Blending Windows only in MS Windows
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

#ifdef USE_WIN32
#include <windows.h>
#include <gdk/gdkwin32.h>
#endif

gboolean create_dnd_confirm_window();
static void cc_true();

void
signal_drag_data_received (GtkWidget        *widget,
                           GdkDragContext   *context,
                           gint              x,
                           gint              y,
                           GtkSelectionData *selection_data,
                           guint             info,
                           guint             time)
{
  gchar *file, *file0, **files;
  gint i;
  gboolean flag_install, installing;
  
  installing=FALSE;

  files = g_strsplit ((const gchar *)selection_data->data, "\r\n", 0);
  for (i = 0; files[i] && *files[i] != '\0'; i++)
    {
      //g_print ("%s", files[i]);
      file0 = g_filename_from_uri (files[i], NULL, NULL);
      file = to_locale(file0);
      if (file){
	if(access(file,F_OK)==0){
	  if(strcmp(file+strlen(file)-strlen(LZH_EXTENSION)+1,
		    LZH_EXTENSION+1)==0){
#ifdef USE_LHA32
	    if(create_dnd_confirm_window(file0)){
	      Mascot->menu_file=unlha_menu(file);
	      Mascot->installed_menu_dir=my_dirname(Mascot->menu_file);
	      installing=TRUE;
	    }
#endif
	  }
	  else if(strcmp(file+strlen(file)-strlen(TAR_EXTENSION)+1,
			 TAR_EXTENSION+1)==0){
#if defined(USE_GTAR) || defined(USE_TAR32)
	    if(create_dnd_confirm_window(file0)){
	      Mascot->menu_file=untar_menu(Mascot,file);
	      Mascot->installed_menu_dir=my_dirname(Mascot->menu_file);
	      installing=TRUE;
	    }
#endif
	  }
	}
	
	if(installing){
	  flag_install=Mascot->flag_install;
	  
	  Mascot->flag_install=TRUE;
	  Mascot->flag_common=FALSE;
	  Mascot->flag_ow=TRUE;
	  Mascot->flag_ow_ini=TRUE;
	  
	  gtk_widget_destroy(Mascot->PopupMenu);
	  ReadMenu(Mascot,0,NULL);
	  Mascot->PopupMenu=make_popup_menu();
	  
	  Mascot->flag_menu=FALSE;
	  Mascot->flag_install=flag_install;
	  Mascot->flag_common=FALSE;
	  Mascot->flag_ow=FALSE;
	  Mascot->flag_ow_ini=FALSE;
	}
      
	//g_print (" -> %s", file);
	if(file) g_free(file);
	if(file0) g_free(file0);
      }
      //g_print ("\n");
    }
  g_strfreev (files);
}


void
signal_drag_data_received_smenu (GtkWidget        *widget,
				 GdkDragContext   *context,
				 gint              x,
				 gint              y,
				 GtkSelectionData *selection_data,
				 guint             info,
				 guint             time)
{
  signal_drag_data_received(widget, context, x, y, selection_data,
			    info, time);

  gtk_main_quit();
}


gboolean create_dnd_confirm_window (gchar *file)
{
  GtkWidget *main;
  GtkWidget *button;
  GtkWidget *label;
  GtkWidget *vbox;
  GtkWidget *hbox;
  gchar *message;
  gchar *userdir;
  gboolean result;
  
  //if (verify_result == X509_V_OK)
  // return SSL_CERT_ACCEPT;
  
  while (my_main_iteration(FALSE));

  Mascot->flag_menu=TRUE;
  result=FALSE;

#ifdef USE_WIN32
  userdir = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,USER_DIR, NULL);
#else
  userdir = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,USER_DIR, NULL);
#endif

  message = g_strdup_printf
    (_("Do you want to install the mascot menu archive,\n"
       "     \"%s\"\n"
       "to your system [\"%s\"]? \n"
       
       "(Files are automatically overwritten.)"),
     file,userdir);
  g_free(userdir);

  main = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  gtk_window_set_position(GTK_WINDOW(main), GTK_WIN_POS_CENTER);
  gtk_window_set_modal(GTK_WINDOW(main), TRUE);
  
  gtk_window_set_title(GTK_WINDOW(main),
		       _("MaCoPiX : Installing Mascot Menu File"));
  
  gtk_widget_realize(main);

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add (GTK_CONTAINER (main), vbox);
  
  label = gtk_label_new(message);
  g_free(message);
  gtk_box_pack_start(GTK_BOX(vbox),label,FALSE,FALSE,0);
  

  hbox = gtk_hbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,0);

  label = gtk_label_new("");
  gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,TRUE,0);
  
  button=gtkut_button_new_from_stock(_("OK"),GTK_STOCK_OK);
  gtk_box_pack_start(GTK_BOX(hbox),button,FALSE,FALSE,0);
  my_signal_connect(button,"clicked",cc_true, &result);

  button=gtkut_button_new_from_stock(_("Cancel"),GTK_STOCK_CANCEL);
  gtk_box_pack_start(GTK_BOX(hbox),button,FALSE,FALSE,0);
  my_signal_connect(button,"clicked",gtk_main_quit, NULL);

  gtk_widget_show_all(main);
#ifdef USE_WIN32
  {
    HWND   hWnd;
    
    SetWindowPos(GDK_WINDOW_HWND(main->window),HWND_TOPMOST,0,0,0,0,
		 SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOSIZE);
  }
#else
  gdk_window_raise(main->window);
#endif
  gdk_flush();

  gtk_main();

  gtk_widget_destroy(main);
  gdk_flush();

  Mascot->flag_menu=FALSE;

  return (result);
}

static void cc_true (GtkWidget * widget, gboolean * gdata)
{
  *gdata=TRUE;
  gtk_main_quit();
}

