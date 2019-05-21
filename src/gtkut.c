// gtkut.c    modified Gtk+ functions
//                              Mar 2019  Akito Tajitsu

#include "main.h"

void gtkut_tree_view_column_set_markup(GtkTreeViewColumn *column,
				       gchar *markup_str){
  GtkWidget *label;
  label=gtkut_label_new(markup_str);
  gtk_tree_view_column_set_widget(column, label);
  gtk_widget_show(label);
}

GtkWidget* gtkut_label_new(gchar *markup_str){
  GtkWidget *w;
  
  w=gtk_label_new(NULL);
  gtk_label_set_markup(GTK_LABEL(w), markup_str);
  return(w);
}


GtkWidget* gtkut_frame_new(gchar *str){
  GtkWidget *w;

  if(str){
    w=gtk_frame_new(" ");
    gtk_label_set_markup(GTK_LABEL(gtk_frame_get_label_widget(GTK_FRAME(w))),
			 str);
  }
  else{
    w=gtk_frame_new(NULL);
  }
  return(w);
}

void gtkut_frame_set_label(GtkFrame *frame, gchar *str){
  gchar *tmp;
  
  tmp=g_strdup_printf("<b>%s</b>", str);
  gtk_label_set_markup(GTK_LABEL(gtk_frame_get_label_widget(frame)),
		       tmp);
  g_free(tmp);
}

GtkWidget* gtkut_hbox_new(gboolean homogeneous, gint spacing){
  GtkWidget *w;
#ifdef USE_GTK3
  w = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, spacing);
  gtk_box_set_homogeneous(GTK_BOX(w), homogeneous);
#else
  w = gtk_hbox_new(homogeneous, spacing);
#endif
  return(w);
}

GtkWidget* gtkut_vbox_new(gboolean homogeneous, gint spacing){
  GtkWidget *w;
#ifdef USE_GTK3
  w = gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);
  gtk_box_set_homogeneous(GTK_BOX(w), homogeneous);
#else
  w = gtk_vbox_new(homogeneous, spacing);
#endif
  return(w);
}

GtkWidget* gtkut_table_new(gint col, gint row, gboolean homogeneous, gint col_spacing, gint row_spacing, gint border_width){
  GtkWidget *w;
#ifdef USE_GTK3
  w = gtk_grid_new();
  gtk_grid_set_column_spacing (GTK_GRID (w), col_spacing);
  gtk_grid_set_row_spacing (GTK_GRID (w), row_spacing);
#else
  w = gtk_table_new(col, row, homogeneous);
  gtk_table_set_col_spacings (GTK_TABLE (w), col_spacing);
  gtk_table_set_row_spacings (GTK_TABLE (w), row_spacing);
#endif
  gtk_container_set_border_width (GTK_CONTAINER (w), border_width);
  return(w);
}

void gtkut_table_attach(GtkWidget *table,
			GtkWidget *child,
			guint left_attach,
			guint right_attach,
			guint top_attach,
			guint bottom_attach,
			GtkAttachOptions xoptions,
			GtkAttachOptions yoptions,
			guint xpadding,
			guint ypadding){
#ifdef USE_GTK3      
  gtk_grid_attach(GTK_GRID(table), child,
		  left_attach, top_attach, right_attach-left_attach, bottom_attach-top_attach);
  if(xoptions == GTK_SHRINK){
    gtk_widget_set_hexpand(child,FALSE);
  }
  else{
    gtk_widget_set_hexpand(child,TRUE);
  }
  
  if(yoptions == GTK_SHRINK){
    gtk_widget_set_vexpand(child,FALSE);
  }
  else{
    gtk_widget_set_vexpand(child,TRUE);
  }
#else
  gtk_table_attach(GTK_TABLE(table), child,
		   left_attach, right_attach, top_attach, bottom_attach,
		   xoptions, yoptions, xpadding, ypadding);
#endif
}

void gtkut_table_attach_defaults(GtkWidget *table,
				 GtkWidget *child,
				 guint left_attach,
				 guint right_attach,
				 guint top_attach,
				 guint bottom_attach){
#ifdef USE_GTK3      
  gtk_widget_set_hexpand(child,TRUE);
  gtk_widget_set_vexpand(child,TRUE);
  gtk_grid_attach(GTK_GRID(table), child,
		  left_attach, top_attach, right_attach-left_attach, bottom_attach-top_attach);
#else
  gtk_table_attach_defaults(GTK_TABLE(table), child,
			    left_attach, right_attach, top_attach, bottom_attach);
#endif
}

#ifdef USE_GTK3
GtkWidget* gtkut_image_menu_item_new_with_label(GtkWidget *icon,
						const gchar *txt){
  GtkWidget *box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6);
  GtkWidget *label = gtk_label_new (txt);
  GtkWidget *menu_item = gtk_menu_item_new ();

  gtk_container_add (GTK_CONTAINER (box), icon);
  gtk_container_add (GTK_CONTAINER (box), label);

  gtk_container_add (GTK_CONTAINER (menu_item), box);
  
  gtk_widget_show_all (menu_item);
  return(menu_item);
}
#endif


GtkWidget* gtkut_button_new_with_icon(gchar *txt,
				      const gchar *stock_or_icon_name){
  GtkWidget *w;
#ifdef USE_GTK3
  w=gtkut_button_new_from_icon_name(txt, stock_or_icon_name);
#else
  w=gtkut_button_new_from_stock(txt, stock_or_icon_name);
#endif
  return(w);
}

#ifdef USE_GTK3
GtkWidget* gtkut_button_new_from_icon_name(gchar *txt,
					   const gchar *stock)
#else
GtkWidget* gtkut_button_new_from_stock(gchar *txt,
				       const gchar *stock)
#endif
{
  GtkWidget *button;
  GtkWidget *box;
  GtkWidget *image;
  GtkWidget *label;
  GtkWidget *box2;
  
  box2=gtkut_hbox_new(TRUE,0);

  box=gtkut_hbox_new(FALSE,0);
  gtk_box_pack_start(GTK_BOX(box2),box, FALSE,FALSE,0);

  gtk_container_set_border_width(GTK_CONTAINER(box),0);
  
  if(txt){
#ifdef USE_GTK3
    image=gtk_image_new_from_icon_name (stock, GTK_ICON_SIZE_BUTTON);
#else
    image=gtk_image_new_from_stock (stock, GTK_ICON_SIZE_BUTTON);
#endif
    gtk_box_pack_start(GTK_BOX(box),image, FALSE,FALSE,2);
  }
  else{
#ifdef USE_GTK3
    image=gtk_image_new_from_icon_name (stock, GTK_ICON_SIZE_MENU);
#else
    image=gtk_image_new_from_stock (stock, GTK_ICON_SIZE_MENU);
#endif
    gtk_box_pack_start(GTK_BOX(box),image, FALSE,FALSE,0);
  }
  gtk_widget_show(image);
  
  if(txt){
    label=gtk_label_new (txt);
    gtk_box_pack_start(GTK_BOX(box),label, FALSE,FALSE,2);
    gtk_widget_show(label);
  }

  button=gtk_button_new();
  gtk_container_add(GTK_CONTAINER(button),box2);

  gtk_widget_show(button);
  return(button);
}

GtkWidget* gtkut_toggle_button_new_with_icon(gchar *txt,
					     const gchar *stock_or_icon_name)
{
  GtkWidget* w;
#ifdef USE_GTK3
  w = gtkut_toggle_button_new_from_icon_name(txt, stock_or_icon_name);
#else
  w = gtkut_toggle_button_new_from_stock(txt, stock_or_icon_name);
#endif
  return(w);
}

#ifdef USE_GTK3
GtkWidget* gtkut_toggle_button_new_from_icon_name(gchar *txt,
						   const gchar *stock)
#else
GtkWidget* gtkut_toggle_button_new_from_stock(gchar *txt,
					      const gchar *stock)
#endif
{
  GtkWidget *button;
  GtkWidget *box;
  GtkWidget *image;
  GtkWidget *label;
  GtkWidget *box2;
  
  box2=gtkut_hbox_new(TRUE,0);

  box=gtkut_hbox_new(FALSE,0);
  gtk_box_pack_start(GTK_BOX(box2),box, FALSE,FALSE,0);

  gtk_container_set_border_width(GTK_CONTAINER(box),0);
  
  if(txt){
#ifdef USE_GTK3
    image=gtk_image_new_from_icon_name (stock, GTK_ICON_SIZE_BUTTON);
#else
    image=gtk_image_new_from_stock (stock, GTK_ICON_SIZE_BUTTON);
#endif
    gtk_box_pack_start(GTK_BOX(box),image, FALSE,FALSE,2);
  }
  else{
#ifdef USE_GTK3
    image=gtk_image_new_from_icon_name (stock, GTK_ICON_SIZE_MENU);
#else
    image=gtk_image_new_from_stock (stock, GTK_ICON_SIZE_MENU);
#endif
    gtk_box_pack_start(GTK_BOX(box),image, FALSE,FALSE,0);
  }
  gtk_widget_show(image);

  if(txt){
    label=gtk_label_new (txt);
    gtk_box_pack_start(GTK_BOX(box),label, FALSE,FALSE,2);
    gtk_widget_show(label);
  }

  button=gtk_toggle_button_new();
  gtk_container_add(GTK_CONTAINER(button),box2);
  
  gtk_widget_show(button);
  return(button);
}

GtkWidget* gtkut_button_new_from_pixbuf(gchar *txt,
				       GdkPixbuf *pixbuf){
  GtkWidget *button;
  GtkWidget *box;
  GtkWidget *image;
  GtkWidget *label;
  GtkWidget *box2;
  GdkPixbuf *pixbuf2;
  gint w,h;
  
  box2=gtkut_hbox_new(TRUE,0);

  box=gtkut_hbox_new(FALSE,0);
  gtk_box_pack_start(GTK_BOX(box2),box, FALSE,FALSE,0);

  gtk_container_set_border_width(GTK_CONTAINER(box),0);

  
  if(txt){
    gtk_icon_size_lookup(GTK_ICON_SIZE_BUTTON,&w,&h);
    pixbuf2=gdk_pixbuf_scale_simple(pixbuf,w,h,GDK_INTERP_BILINEAR);
    image=gtk_image_new_from_pixbuf (pixbuf2);
    gtk_box_pack_start(GTK_BOX(box),image, FALSE,FALSE,2);
  }
  else{
    gtk_icon_size_lookup(GTK_ICON_SIZE_MENU,&w,&h);
    pixbuf2=gdk_pixbuf_scale_simple(pixbuf,w,h,GDK_INTERP_BILINEAR);
    image=gtk_image_new_from_pixbuf (pixbuf2);
    gtk_box_pack_start(GTK_BOX(box),image, FALSE,FALSE,0);
  }
  gtk_widget_show(image);
  g_object_unref(pixbuf2);

  if(txt){
    label=gtk_label_new (txt);
    gtk_box_pack_start(GTK_BOX(box),label, FALSE,FALSE,2);
    gtk_widget_show(label);
  }

  button=gtk_button_new();
  gtk_container_add(GTK_CONTAINER(button),box2);

  gtk_widget_show(button);
  return(button);
}

 
GtkWidget* gtkut_toggle_button_new_from_pixbuf(gchar *txt,
					       GdkPixbuf *pixbuf){
  GtkWidget *button;
  GtkWidget *box;
  GtkWidget *image;
  GtkWidget *label;
  GtkWidget *box2;
  GdkPixbuf *pixbuf2;
  gint w,h;
  
  box2=gtkut_hbox_new(TRUE,0);

  box=gtkut_hbox_new(FALSE,0);
  gtk_box_pack_start(GTK_BOX(box2),box, FALSE,FALSE,0);

  gtk_container_set_border_width(GTK_CONTAINER(box),0);

  
  if(txt){
    gtk_icon_size_lookup(GTK_ICON_SIZE_BUTTON,&w,&h);
    pixbuf2=gdk_pixbuf_scale_simple(pixbuf,w,h,GDK_INTERP_BILINEAR);
    image=gtk_image_new_from_pixbuf (pixbuf2);
    gtk_box_pack_start(GTK_BOX(box),image, FALSE,FALSE,2);
  }
  else{
    gtk_icon_size_lookup(GTK_ICON_SIZE_MENU,&w,&h);
    pixbuf2=gdk_pixbuf_scale_simple(pixbuf,w,h,GDK_INTERP_BILINEAR);
    image=gtk_image_new_from_pixbuf (pixbuf2);
    gtk_box_pack_start(GTK_BOX(box),image, FALSE,FALSE,0);
  }
  gtk_widget_show(image);

  g_object_unref(pixbuf2);

  if(txt){
    label=gtk_label_new (txt);
    gtk_box_pack_start(GTK_BOX(box),label, FALSE,FALSE,2);
    gtk_widget_show(label);
  }

  button=gtk_toggle_button_new();
  gtk_container_add(GTK_CONTAINER(button),box2);
  
   gtk_widget_show(button);
  return(button);
}


void gtkut_pos(GtkWidget *w, gint hpos, gint vpos){
#ifdef USE_GTK3
  switch(hpos){
  case POS_START:
    gtk_widget_set_halign(w, GTK_ALIGN_START);
    break;
  case POS_CENTER:
    gtk_widget_set_halign(w, GTK_ALIGN_CENTER);
    break;
  case POS_END:
    gtk_widget_set_halign(w, GTK_ALIGN_END);
    break;
  }
  switch(vpos){
  case POS_START:
    gtk_widget_set_valign(w, GTK_ALIGN_START);
    break;
  case POS_CENTER:
    gtk_widget_set_valign(w, GTK_ALIGN_CENTER);
    break;
  case POS_END:
    gtk_widget_set_valign(w, GTK_ALIGN_END);
    break;
  }
#else
  gdouble h, v;

  switch(hpos){
  case POS_START:
    h=0.0;
    break;
  case POS_CENTER:
    h=0.5;
    break;
  case POS_END:
    h=1.0;
    break;
  }
  switch(vpos){
  case POS_START:
    v-0.0;
    break;
  case POS_CENTER:
    v=0.5;
    break;
  case POS_END:
    v=1.0;
    break;
  }
  
  gtk_misc_set_alignment (GTK_MISC (w), h, v);
#endif
}


GtkWidget * gtkut_menu_item_new_with_icon(const gchar *stock_or_icon_name,
					  GtkIconSize size,
					  const gchar *txt)
{
  GtkWidget *image, *menu_item;
#ifdef USE_GTK3
  image=gtk_image_new_from_icon_name (stock_or_icon_name, size);
  menu_item =gtkut_image_menu_item_new_with_label (image, txt);
#else
  image=gtk_image_new_from_stock (stock_or_icon_name, size);
  menu_item =gtk_image_menu_item_new_with_label (txt);
  gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_item),image);
#endif

  return(menu_item);
}


void gdkut_flush(typMascot *mascot){
#ifdef USE_GTK3
  gdk_display_flush(gtk_widget_get_display(mascot->win_main));
#else
  gdk_flush();
#endif
}


GtkWidget * gtkut_hscale_new(GtkAdjustment *adj){
  GtkWidget *w;
  
#ifdef USE_GTK3
  w =  gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, GTK_ADJUSTMENT(adj));
  gtk_widget_set_hexpand(w,TRUE);
#else
  w =  gtk_hscale_new (GTK_ADJUSTMENT(adj));
#endif

  return(w);
}


GtkWidget * gtkut_arrow_new(MyArrowDirect direct){
  GtkWidget *w;
#ifdef USE_GTK3
  GtkWidget *image;

  w=gtkut_hbox_new(FALSE,0);
  gtk_container_set_border_width(GTK_CONTAINER(w),0);
  
  switch(direct){
  case MY_ARROW_LEFT:
    image = gtk_image_new_from_icon_name ("go-previous", GTK_ICON_SIZE_MENU);
    break;
  case MY_ARROW_RIGHT:
    image = gtk_image_new_from_icon_name ("go-next", GTK_ICON_SIZE_MENU);
    break;
  case MY_ARROW_UP:
    image = gtk_image_new_from_icon_name ("go-up", GTK_ICON_SIZE_MENU);
    break;
  case MY_ARROW_DOWN:
    image = gtk_image_new_from_icon_name ("go-down", GTK_ICON_SIZE_MENU);
    break;
  }
  gtk_widget_show(image);
  gtk_box_pack_start(GTK_BOX(w),image, FALSE,FALSE,2);
  
#else
  switch(direct){
  case MY_ARROW_LEFT:
    w = gtk_arrow_new(GTK_ARROW_LEFT,GTK_SHADOW_IN);
    break;
  case MY_ARROW_RIGHT:
    w = gtk_arrow_new(GTK_ARROW_RIGHT,GTK_SHADOW_IN);
    break;
  case MY_ARROW_UP:
    w = gtk_arrow_new(GTK_ARROW_UP,GTK_SHADOW_IN);
    break;
  case MY_ARROW_DOWN:
    w = gtk_arrow_new(GTK_ARROW_DOWN,GTK_SHADOW_IN);
    break;
  }  
#endif

  return(w);
}


#ifdef USE_GTK3
void my_cairo_set_source_rgba(cairo_t *cr, GdkRGBA *col, gdouble alpha){
  cairo_set_source_rgba (cr, 
			 col->red,
			 col->green,
			 col->blue,
			 col->alpha);
}
#else
void my_cairo_set_source_rgba(cairo_t *cr, GdkColor *col, gdouble alpha){
  cairo_set_source_rgba (cr, 
			 (gdouble)col->red/(gdouble)0xFFFF,
			 (gdouble)col->green/(gdouble)0xFFFF,
			 (gdouble)col->blue/(gdouble)0xFFFF,
			 (gdouble)alpha);
}
#endif
