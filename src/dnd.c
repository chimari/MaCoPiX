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
                           guint             time,
			   gpointer          gdata)
{
  typMascot *mascot = (typMascot *)gdata;
  gchar *file, *file0, **files;
  gint i;
  gboolean flag_install, installing;
  
  installing=FALSE;

  files = g_strsplit ((const gchar *)selection_data, "\r\n", 0);
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
	    if(create_dnd_confirm_window(mascot, file0)){
	      mascot->menu_file=unlha_menu(file);
	      mascot->installed_menu_dir=my_dirname(mascot->menu_file);
	      installing=TRUE;
	    }
#endif
	  }
	  else if(strcmp(file+strlen(file)-strlen(TAR_EXTENSION)+1,
			 TAR_EXTENSION+1)==0){
#if defined(USE_GTAR) || defined(USE_TAR32)
	    if(create_dnd_confirm_window(mascot, file0)){
	      mascot->menu_file=untar_menu(mascot,file);
	      mascot->installed_menu_dir=my_dirname(mascot->menu_file);
	      installing=TRUE;
	    }
#endif
	  }
	}
	
	if(installing){
	  flag_install=mascot->flag_install;
	  
	  mascot->flag_install=TRUE;
	  mascot->flag_common=FALSE;
	  mascot->flag_ow=TRUE;
	  mascot->flag_ow_ini=TRUE;
	  
	  gtk_widget_destroy(mascot->PopupMenu);
	  ReadMenu(mascot,0,NULL);
	  mascot->PopupMenu=make_popup_menu(mascot);
	  
	  mascot->flag_menu=FALSE;
	  mascot->flag_install=flag_install;
	  mascot->flag_common=FALSE;
	  mascot->flag_ow=FALSE;
	  mascot->flag_ow_ini=FALSE;
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
				 guint             time,
				 gpointer          gdata)
{
  signal_drag_data_received(widget, context, x, y, selection_data,
			    info, time, gdata);

  gtk_main_quit();
}


gboolean create_dnd_confirm_window (typMascot *mascot, gchar *file)
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

  mascot->flag_menu=TRUE;
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

  vbox = gtkut_vbox_new(FALSE, 0);
  gtk_container_add (GTK_CONTAINER (main), vbox);
  
  label = gtkut_label_new(message);
  g_free(message);
  gtk_box_pack_start(GTK_BOX(vbox),label,FALSE,FALSE,0);
  

  hbox = gtkut_hbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,0);

  label = gtkut_label_new("");
  gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,TRUE,0);
  
  button=gtkut_button_new_with_icon(_("OK"),
#ifdef USE_GTK3
				    "emblem-default"
#else
				    GTK_STOCK_OK
#endif
				    );
  gtk_box_pack_start(GTK_BOX(hbox),button,FALSE,FALSE,0);
  my_signal_connect(button,"clicked",cc_true, &result);

  button=gtkut_button_new_with_icon(_("Cancel"),
#ifdef USE_GTK3
				    "process-stop"
#else
				    GTK_STOCK_CANCEL
#endif
				    );
  gtk_box_pack_start(GTK_BOX(hbox),button,FALSE,FALSE,0);
  my_signal_connect(button,"clicked",gtk_main_quit, NULL);

  gtk_widget_show_all(main);
#ifdef USE_WIN32
  {
    HWND   hWnd;
    
    SetWindowPos(GDK_WINDOW_HWND(gtk_widget_get_window(main)),HWND_TOPMOST,0,0,0,0,
		 SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOSIZE);
  }
#else
  gdk_window_raise(gtk_widget_get_window(main));
#endif
  gtk_main();

  gtk_widget_destroy(main);

  mascot->flag_menu=FALSE;

  return (result);
}

static void cc_true (GtkWidget * widget, gboolean * gdata)
{
  *gdata=TRUE;
  gtk_main_quit();
}

