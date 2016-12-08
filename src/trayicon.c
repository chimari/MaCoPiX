//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//
//
//      trayicon.c
//      Management of Trayicon w/Gtk+ >2.10.0
//         originated from Sylpheed-2.3.1  *** see below copyright ***
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


/*
 * Sylpheed -- a GTK+ based, lightweight, and fast e-mail client
 * Copyright (C) 1999-2006 Hiroyuki Yamamoto
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


#include "main.h"


//#if GTK_CHECK_VERSION(2, 10, 0)
#ifdef __GTK_STATUS_ICON_H__

#include "macopix_icon.h"



extern void AllRandomChangeMascotMenu();
extern void create_config_dialog();
extern void MenuSaveAll();
extern void quit_all();
extern void my_signal_connect();

void trayicon_create();
static void trayicon_popup_menu();
void trayicon_show();
void trayicon_hyde();
void trayicon_destroy();
void trayicon_set_tooltip();

extern GtkWidget *win_main;
// extern GtkWidget *PopupMenu;

static GtkWidget *trayicon_menu=NULL;
static gboolean on_notify=FALSE;
static gboolean default_tooltip=FALSE;

void trayicon_create(typMascot *mascot){
  GdkPixbuf *icon;
  GtkWidget *popup_button, *bar;
#ifdef __GTK_STOCK_H__
  GtkWidget *image;
#endif

  icon = gdk_pixbuf_new_from_inline(sizeof(macopix_icon), macopix_icon, 
				    FALSE, NULL);
  mascot->tray_icon = gtk_status_icon_new_from_pixbuf(icon);

  g_signal_connect(G_OBJECT(mascot->tray_icon), "popup-menu",
  		   G_CALLBACK(trayicon_popup_menu), win_main);
  
  if (!trayicon_menu) {
    trayicon_menu = gtk_menu_new();
    gtk_widget_show(trayicon_menu);
    
#ifdef __GTK_STOCK_H__
    image=gtk_image_new_from_stock (GTK_STOCK_REFRESH, GTK_ICON_SIZE_MENU);
    popup_button =gtk_image_menu_item_new_with_label (_("Mascot Random Change"));
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(popup_button),image);
#else
    popup_button =gtk_menu_item_new_with_label (_("Mascot Random Change"));
#endif
    gtk_widget_show (popup_button);
    gtk_container_add (GTK_CONTAINER (trayicon_menu), popup_button);
    my_signal_connect (popup_button, "activate",
                       AllRandomChangeMascotMenu,
		       NULL);
    
    bar =gtk_menu_item_new();
    gtk_widget_show (bar);
    gtk_container_add (GTK_CONTAINER (trayicon_menu), bar);
    
#ifdef __GTK_STOCK_H__
    image=gtk_image_new_from_stock (GTK_STOCK_PROPERTIES, GTK_ICON_SIZE_MENU);
    popup_button =gtk_image_menu_item_new_with_label (_("Config"));
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(popup_button),image);
#else
    popup_button =gtk_menu_item_new_with_label (_("Config"));
#endif
    gtk_widget_show (popup_button);
    gtk_container_add (GTK_CONTAINER (trayicon_menu), popup_button);
    my_signal_connect (popup_button, "activate",
		       create_config_dialog,
		       NULL);
                        
#ifdef __GTK_STOCK_H__
    image=gtk_image_new_from_stock (GTK_STOCK_SAVE, GTK_ICON_SIZE_MENU);
    popup_button =gtk_image_menu_item_new_with_label (_("Save All"));
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(popup_button),image);
#else
    popup_button =gtk_menu_item_new_with_label (_("Save All"));
#endif
    gtk_widget_show (popup_button);
    gtk_container_add (GTK_CONTAINER (trayicon_menu), popup_button);
    my_signal_connect (popup_button, "activate",
		       MenuSaveAll,
		       NULL);
    
    bar =gtk_menu_item_new();
    gtk_widget_show (bar);
    gtk_container_add (GTK_CONTAINER (trayicon_menu), bar);
    
#ifdef __GTK_STOCK_H__
    image=gtk_image_new_from_stock (GTK_STOCK_QUIT, GTK_ICON_SIZE_MENU);
    popup_button =gtk_image_menu_item_new_with_label (_("Exit"));
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(popup_button),image);
#else
    popup_button =gtk_menu_item_new_with_label (_("Exit"));
#endif
    gtk_widget_show (popup_button);
    gtk_container_add (GTK_CONTAINER (trayicon_menu), popup_button);
    my_signal_connect (popup_button, "activate",
		       quit_all,
		       NULL);
  }
  
  on_notify = FALSE;
  default_tooltip = FALSE;
  trayicon_set_tooltip(mascot);
  
}

static void trayicon_popup_menu(GtkStatusIcon *status_icon, 
				guint button,
                                guint activate_time, 
				gpointer gdata)
{
  gtk_menu_popup (GTK_MENU(trayicon_menu), NULL, NULL, NULL, NULL, 
		  button, activate_time);
}



void trayicon_show(typMascot  *mascot)
{
  gtk_status_icon_set_visible(mascot->tray_icon, TRUE);
};

void trayicon_hide(typMascot  *mascot)
{
	gtk_status_icon_set_visible(mascot->tray_icon, FALSE);
}

void trayicon_destroy(typMascot  *mascot)
{
	g_object_unref(mascot->tray_icon);
	mascot->tray_icon = NULL;
}

void trayicon_set_tooltip(typMascot *mascot)
{
  gchar *text;

  if (mascot->name) {
    default_tooltip = FALSE;
    text=g_strdup_printf("MaCoPiX : %s",mascot->name);
    gtk_status_icon_set_tooltip(mascot->tray_icon, text);
    g_free(text);
  } else if (!default_tooltip) {
    default_tooltip = TRUE;
    gtk_status_icon_set_tooltip(mascot->tray_icon,
				"MaCoPiX");
  }
}

#endif
