//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//
//
//      sslmanager.c  
//      Cpnfirmation window for SSL Certification
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


#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#ifdef USE_SSL

#include "main.h"
#include "ssl.h"

#ifdef USE_WIN32
#include <windows.h>
#include <gdk/gdkwin32.h>
#endif

extern typMascot *Mascot;

extern GtkWidget* gtkut_button_new_from_stock();

extern void my_signal_connect();
extern gboolean my_main_iteration();


static void cc_get_toggle();
static void cc_true ();

gint ssl_manager_verify_cert	(typMascot *mascot)
{
  GtkWidget *main;
  GtkWidget *button;
  GtkWidget *label;
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *check;
  gchar *message;
  gchar *subject, *issuer;
  gboolean result;
  
#ifdef USE_GNUTLS
  if (mascot->mail.ssl_verify == /*X509_V_OK*/0)
#else
  if (mascot->mail.ssl_verify == X509_V_OK)
#endif
   return SSL_CERT_ACCEPT;
  
  while (my_main_iteration(FALSE));

  mascot->flag_menu=TRUE;

  result=FALSE;

  if(mascot->mail.ssl_sub){
    subject = g_strdup(mascot->mail.ssl_sub);
  }
  else{
    subject = g_strdup(_("(unknown)"));
  }

  if(mascot->mail.ssl_iss){
    issuer = g_strdup(mascot->mail.ssl_iss);
  }
  else{
    issuer = g_strdup(_("(unknown)"));
  }


  /*
  subject = X509_NAME_oneline(X509_get_subject_name(server_cert),
  		      NULL, 0);
  issuer = X509_NAME_oneline(X509_get_issuer_name(server_cert), NULL, 0);
  
  message = g_strdup_printf
    (_("The SSL certificate of %s cannot be verified by the following reason:\n"
       "  %s\n\n"
       "Server certificate:\n"
       "  Subject: %s\n"
       "  Issuer: %s\n\n"
       "Do you accept this certificate?"),
     hostname, X509_verify_cert_error_string(verify_result),
     subject ? subject : "(unknown)",
     issuer ? issuer : "(unknown)");
  g_free(issuer);
  g_free(subject);
  */

  message = g_strdup_printf
    (_("The SSL certificate of host \"%s\" cannot verified by the following reason:\n"
       "  %s\n\n"
       "Server certificate:\n"
       "  Subject: %s\n"
       "  Issuer: %s\n\n"
       "Do you accept this certificate?"),
     mascot->mail.pop_server,
#ifdef USE_GNUTLS
     /*X509_verify_cert_error_string(mascot->mail.ssl_verify)*/"FIXME",
#else
     X509_verify_cert_error_string(mascot->mail.ssl_verify),
#endif
     subject,issuer);
  g_free(issuer);
  g_free(subject);


#ifdef USE_GTK2  
  main = gtk_window_new(GTK_WINDOW_TOPLEVEL);
#else
  main = gtk_window_new(GTK_WINDOW_DIALOG);
#endif

  gtk_window_set_position(GTK_WINDOW(main), GTK_WIN_POS_CENTER);
  gtk_window_set_modal(GTK_WINDOW(main), TRUE);

  gtk_window_set_title(GTK_WINDOW(main),
		       _("SSL certificate verify failed"));

  gtk_widget_realize(main);

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add (GTK_CONTAINER (main), vbox);

  label = gtk_label_new(message);
  g_free(message);
  gtk_box_pack_start(GTK_BOX(vbox),label,FALSE,FALSE,0);

  label = gtk_label_new("");
  gtk_box_pack_start(GTK_BOX(vbox),label,FALSE,FALSE,0);

  check = gtk_check_button_new_with_label(_("Skip this verification after this."));
  gtk_box_pack_start(GTK_BOX(vbox), check,FALSE, FALSE, 0);
  if(mascot->mail.ssl_cert_skip){
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
  }
  my_signal_connect (check, "toggled",cc_get_toggle,
		     &mascot->mail.ssl_cert_skip);


  hbox = gtk_hbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,0);

  label = gtk_label_new("");
  gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,TRUE,0);
  
#ifdef __GTK_STOCK_H__
  button=gtkut_button_new_from_stock(_("OK"),GTK_STOCK_OK);
#else
  button=gtk_button_new_with_label(_("OK"));
#endif
  gtk_box_pack_start(GTK_BOX(hbox),button,FALSE,FALSE,0);
  my_signal_connect(button,"clicked",cc_true, &result);

#ifdef __GTK_STOCK_H__
  button=gtkut_button_new_from_stock(_("Cancel"),GTK_STOCK_CANCEL);
#else
  button=gtk_button_new_with_label(_("Cancel"));
#endif
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

  mascot->flag_menu=FALSE;
  if(result){
    return SSL_CERT_ACCEPT;
  }
  mascot->mail.ssl_cert_skip=FALSE;
  return SSL_CERT_DENY;
}

static void cc_true (GtkWidget * widget, gboolean * gdata)
{
  *gdata=TRUE;
  gtk_main_quit();
}

static void cc_get_toggle (GtkWidget * widget, gboolean * gdata)
{
  *gdata=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
}


#endif /* USE_SSL */
