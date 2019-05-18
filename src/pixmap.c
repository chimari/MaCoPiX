//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//
//
//      pixmap.c  
//      Management of Image loading & rendering for MaCoPiX
//       (using gdk-pixbuf)
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



// *** GLOBAL ARGUMENT ***

#ifdef USE_WIN32
void GdkWinChangeAlpha();
void GdkWinChangeAlphaFG();
#endif

//Callback受け渡し用抱き合わせ構造体 デフォルトフォント変更用
typedef struct{
  gchar *fn;
  GtkFontSelectionDialog *fsd;
}confDefFont;




// Prototype of Functions in this file
#ifdef USE_BIFF
#endif

void create_default_font_selection_dialog();
void ChangeDefFontname();
static void close_child_dialog();

#ifdef USE_GTK3
cairo_region_t * get_cairo_region_from_pixbuf();
#endif

gboolean flag_img_cairo_go=FALSE;


gboolean TestLoadPixmaps(typMascot *mascot, gchar *filename, gint i_pix)
{
  GdkPixbuf *pixbuf = NULL,*pixbuf2 = NULL;
  gint w=0,h=0;
  gint ipstyle;
  gchar *tmp_open;
  cairo_t *cr;
#ifdef USE_GTK3
  cairo_surface_t *surface;
  cairo_region_t *region;
#ifdef FG_DRAW
  cairo_surface_t *surface_sdw;
  cairo_region_t  *region_sdw;
#endif
#endif

  tmp_open=to_utf8(filename);
  pixbuf = gdk_pixbuf_new_from_file(tmp_open, NULL);
  g_free(tmp_open);
  if(!pixbuf) return(FALSE);

  w = gdk_pixbuf_get_width(pixbuf)*((gdouble)(mascot->magnify)/100);
  h = gdk_pixbuf_get_height(pixbuf)*((gdouble)(mascot->magnify)/100);

#ifdef USE_WIN32
  if(mascot->move==MOVE_FOCUS){
    if(mascot->yoff>0){
      mascot->sdw_height
	=mascot->yoff*(gdouble)(mascot->magnify)/100+mascot->sdw_y;
    }
    else{
      mascot->sdw_height=0;
    }
  }
  else{
    mascot->sdw_height=h;
  }
#endif

  switch(mascot->ip_style){
  case MAG_IP_NEAREST:
    ipstyle=GDK_INTERP_NEAREST;
    break;
    
  case MAG_IP_TILES:
    ipstyle=GDK_INTERP_TILES;
    break;
    
  case MAG_IP_BILINEAR:
    ipstyle=GDK_INTERP_BILINEAR;
    break;
    
  case MAG_IP_HYPER:
    ipstyle=GDK_INTERP_HYPER;
    break;
    
  default:
    ipstyle=GDK_INTERP_BILINEAR;
  }

#ifdef USE_GTK3  ///////////////  GTK3  //////////////////////
  if(mascot->sprites[i_pix].pixbuf) {
    g_object_unref(G_OBJECT(mascot->sprites[i_pix].pixbuf));
    mascot->sprites[i_pix].pixbuf=NULL;
  }
#ifdef FG_DRAW
  if(mascot->sprites[i_pix].pixbuf_sdw) {
    g_object_unref(G_OBJECT(mascot->sprites[i_pix].pixbuf_sdw));
    mascot->sprites[i_pix].pixbuf_sdw=NULL;
  }
#endif  
#else       ///////////////  GTK2  //////////////////////
  if(mascot->sprites[i_pix].pixmap) {
    g_object_unref(G_OBJECT(mascot->sprites[i_pix].pixmap));
    mascot->sprites[i_pix].pixmap=NULL;
  }
  if(mascot->sprites[i_pix].mask) {
    g_object_unref(G_OBJECT(mascot->sprites[i_pix].mask));
    mascot->sprites[i_pix].mask=NULL;
  }
#ifdef FG_DRAW
  if(mascot->sprites[i_pix].pixmap_sdw) {
    g_object_unref(G_OBJECT(mascot->sprites[i_pix].pixmap_sdw));
    mascot->sprites[i_pix].pixmap_sdw=NULL;
  }
  if(mascot->sprites[i_pix].mask_sdw) {
    g_object_unref(G_OBJECT(mascot->sprites[i_pix].mask_sdw));
    mascot->sprites[i_pix].mask_sdw=NULL;
  }
#endif  
#endif  ////////////////////////////////////////////////

  pixbuf2=gdk_pixbuf_scale_simple(pixbuf,w,h,ipstyle);

#ifdef USE_GTK3  ///////////////  GTK3  //////////////////////
  if(flag_img_cairo_go){
    if(mascot->sdw_flag){
      w= w + mascot->sdw_x_int;
      h= h + mascot->sdw_y_int;
    }

    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
    cr = cairo_create(surface);

    cairo_set_source_rgba(cr, 0, 0, 0, 0);
    cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
    cairo_rectangle(cr, 0, 0, w, h);
    cairo_fill(cr);
    cairo_paint(cr);
      
    if(mascot->sdw_flag){
      // Shadow
      cairo_save(cr);
      
      if(mascot->sdw_x<0){
	gdk_cairo_set_source_pixbuf(cr, pixbuf2, 0, mascot->sdw_y);
      }
      else{
	gdk_cairo_set_source_pixbuf(cr, pixbuf2,mascot->sdw_x, mascot->sdw_y);
      }
      cairo_paint(cr);
      region = gdk_cairo_region_create_from_surface(surface);
      
      cairo_set_source_rgba(cr, 0, 0, 0, 0);
      cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
      cairo_rectangle(cr, 0, 0, w, h);
      cairo_fill(cr);
      cairo_paint(cr);

      cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
      gdk_cairo_region(cr, region);
      cairo_set_source_rgba(cr, 0, 0, 0, 1);
      cairo_clip(cr);
      cairo_paint_with_alpha (cr, (gdouble)(mascot->sdw_alpha)/100);
      cairo_region_destroy(region);

      cairo_restore(cr);
      
      // Remove shadow above title bar
      if(mascot->move==MOVE_FOCUS){
	cairo_save(cr);
      
	cairo_set_operator (cr, CAIRO_OPERATOR_CLEAR);
	cairo_move_to(cr,0.0,0.0);
	cairo_rectangle(cr, 0, 0, w, 
		      h-mascot->yoff*(gdouble)(mascot->magnify)/100-mascot->sdw_y_int);
	cairo_fill(cr);
	cairo_restore(cr);
      }      
      
    }

    // Mascot
    cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
    cairo_move_to(cr,0.0,0.0);
    if((mascot->sdw_flag)&&(mascot->sdw_x<0)){
      gdk_cairo_set_source_pixbuf(cr,pixbuf2,-mascot->sdw_x,0);
    }
    else{
      gdk_cairo_set_source_pixbuf(cr,pixbuf2,0,0);
    }
    cairo_paint_with_alpha (cr, (gdouble)mascot->alpha_main/100);
    cairo_destroy (cr);

    mascot->sprites[i_pix].pixbuf = gdk_pixbuf_get_from_surface(surface, 0, 0, w, h);
    cairo_surface_destroy (surface);
  }
  else{// for Win32 and non-composited Gtk+2.8 or later
#ifdef FG_DRAW
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
    cr = cairo_create(surface);

    cairo_set_source_rgba(cr, 0, 0, 0, 0);
    cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
    cairo_rectangle(cr, 0, 0, w, h);
    cairo_fill(cr);
    cairo_paint(cr);
    
    gdk_cairo_set_source_pixbuf(cr, pixbuf2, 0, 0);
    cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
    cairo_paint(cr);
    region = gdk_cairo_region_create_from_surface(surface);
    cairo_destroy(cr);
    
    mascot->sprites[i_pix].pixbuf = gdk_pixbuf_get_from_surface(surface, 0, 0, w, h);
    cairo_region_destroy(region);
    cairo_surface_destroy(surface);
    
    if(mascot->sdw_flag){
      gint h_sdw, h0;
      
      if(mascot->move==MOVE_FOCUS){
	if(mascot->sdw_height>0){
	  h_sdw=mascot->sdw_height;
	  h0=h-h_sdw;
	}
	else{
	  h_sdw=h;
	  h0=0;
	}
	surface_sdw = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h_sdw);
	cr = cairo_create(surface_sdw);
	  
	cairo_set_source_rgba(cr, 0, 0, 0, 0);
	cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
	cairo_rectangle(cr, 0, 0, w, h_sdw);
	cairo_fill(cr);
	cairo_paint(cr);

	  
	// Shadow
	gdk_cairo_set_source_pixbuf(cr, pixbuf2, 0, h0);
	cairo_paint(cr);
	region_sdw = gdk_cairo_region_create_from_surface(surface_sdw);
      
	cairo_set_source_rgba(cr, 0, 0, 0, 0);
	cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
	cairo_rectangle(cr, 0, 0, w, h_sdw);
	cairo_fill(cr);
	cairo_paint(cr);
	  
	cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
	gdk_cairo_region(cr, region_sdw);
	cairo_set_source_rgba(cr, 1, 1, 1, 1);
	cairo_paint(cr);
	cairo_destroy(cr);
	
	mascot->sprites[i_pix].pixbuf_sdw = gdk_pixbuf_get_from_surface(surface_sdw, 0, 0, w, h_sdw);
	cairo_region_destroy(region_sdw);
	cairo_surface_destroy(surface_sdw);
      }
    }
#else
    mascot->sprites[i_pix].pixbuf = gdk_pixbuf_copy(pixbuf2);
#endif   
  }
  
#else   ///////////////  GTK2  /////////////////
  if(flag_img_cairo_go){
    GdkBitmap *mask=NULL;
    
    if(mascot->sdw_flag){
      w= w + mascot->sdw_x_int;
      h= h + mascot->sdw_y_int;
      
      gdk_pixbuf_render_pixmap_and_mask(pixbuf, NULL, &mask, 0x01);
    }

    gdk_pixbuf_render_pixmap_and_mask(pixbuf2, NULL,
				      &mascot->sprites[i_pix].mask, 0xc0);
    
    mascot->sprites[i_pix].pixmap = gdk_pixmap_new(gtk_widget_get_window(mascot->win_main),
						   w,
						   h,
						   -1);
    
    cr = gdk_cairo_create(mascot->sprites[i_pix].pixmap);
    cairo_set_source_rgba (cr, 0, 0, 0, 0);
    cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint (cr);

    if(mascot->sdw_flag){
      // Shadow
      cairo_save(cr);
      cairo_scale (cr, (gdouble)(mascot->magnify)/100, 
		   (gdouble)(mascot->magnify)/100);
      if(mascot->sdw_x<0){
	gdk_cairo_set_source_pixmap(cr,mask,0, mascot->sdw_y);
      }
      else{
	gdk_cairo_set_source_pixmap(cr,mask,mascot->sdw_x, mascot->sdw_y);
      }
      cairo_paint_with_alpha (cr, (gdouble)(mascot->sdw_alpha)/100);
      
      cairo_restore(cr);

      // Remove shadow above title bar
      if(mascot->move==MOVE_FOCUS){
	cairo_save(cr);
      
	cairo_set_operator (cr, CAIRO_OPERATOR_CLEAR);
	cairo_move_to(cr,0.0,0.0);
	cairo_rectangle(cr, 0, 0, w, 
		      h-mascot->yoff*(gdouble)(mascot->magnify)/100-mascot->sdw_y_int);
	cairo_fill(cr);
	cairo_restore(cr);
      }
    }

    // Mascot
    cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
    cairo_move_to(cr,0.0,0.0);
    if((mascot->sdw_flag)&&(mascot->sdw_x<0)){
      gdk_cairo_set_source_pixbuf(cr,pixbuf2,-mascot->sdw_x,0);
    }
    else{
      gdk_cairo_set_source_pixbuf(cr,pixbuf2,0,0);
    }
    
    cairo_paint_with_alpha (cr, (gdouble)mascot->alpha_main/100);
    //cairo_paint (cr);
    
    cairo_destroy (cr);
    
    if(mascot->sdw_flag){
      if(mask) g_object_unref(G_OBJECT(mask));
    }
  }
  else{// for Win32 and non-composited Gtk+2.8 or later
    gdk_pixbuf_render_pixmap_and_mask(pixbuf2, &mascot->sprites[i_pix].pixmap,
				      &mascot->sprites[i_pix].mask, 0xc0);
#ifdef FG_DRAW
    if(mascot->sdw_flag){
      
      if(mascot->move==MOVE_FOCUS){
	if(mascot->sdw_height>0){
	  
	  mascot->sprites[i_pix].pixmap_sdw
	    =gdk_pixmap_new(gtk_widget_get_window(mascot->win_sdw),
			    w, mascot->sdw_height,
			    -1);
	  
	  mascot->sprites[i_pix].mask_sdw
	    = gdk_pixmap_new(gtk_widget_get_window(mascot->win_sdw),
			     w, mascot->sdw_height,
			     1); // Depth =1 (Bitmap)
	  
	  gdk_draw_drawable(mascot->sprites[i_pix].pixmap_sdw,
			    mascot->win_sdw->style->fg_gc[GTK_WIDGET_STATE(mascot->win_sdw)],
			    mascot->sprites[i_pix].mask,
			    0, h-mascot->sdw_height,
			    0,0,
			    w,mascot->sdw_height);
	  
	  gdk_draw_drawable(mascot->sprites[i_pix].mask_sdw,
			    mascot->win_sdw->style->fg_gc[GTK_WIDGET_STATE(mascot->win_sdw)],
			    mascot->sprites[i_pix].mask,
			    0, h-mascot->sdw_height,
			    0,0,
			    w,mascot->sdw_height);  
	}
      }
      else{
	mascot->sprites[i_pix].pixmap_sdw
	  =gdk_pixmap_new(gtk_widget_get_window(mascot->win_sdw),w,h,-1);
	
	mascot->sprites[i_pix].mask_sdw
	  = gdk_pixmap_new(gtk_widget_get_window(mascot->win_sdw), w,h,1); // Depth =1 (Bitmap)
	
	gdk_draw_drawable(mascot->sprites[i_pix].pixmap_sdw,
			  mascot->win_sdw->style->fg_gc[GTK_WIDGET_STATE(mascot->win_sdw)],
			  mascot->sprites[i_pix].mask,
			  0,0,
			  0,0,
			  w,h);
	
	gdk_draw_drawable(mascot->sprites[i_pix].mask_sdw,
			  mascot->win_sdw->style->fg_gc[GTK_WIDGET_STATE(mascot->win_sdw)],
			  mascot->sprites[i_pix].mask,
			  0,0,
			  0,0,
			  w,h);
      }
    }
#endif   
  }
#endif    // USE_GTK3 /////////////////////////////////////////////////
  gtk_widget_queue_draw(mascot->dw_main);
  
  g_object_unref(G_OBJECT(pixbuf));
  g_object_unref(G_OBJECT(pixbuf2));

  while (my_main_iteration(FALSE));
  DrawMascot(mascot,i_pix);

  return(TRUE);
  
}


void LoadPixmaps(typMascot *mascot){
  int i=0;
  //GdkImlibImage* im = NULL;
  GdkPixbuf *pixbuf = NULL, *pixbuf2 = NULL;
  gint w=0,h=0;
  gint ipstyle;
  gchar *tmp_open;
  cairo_t *cr;
#ifdef USE_GTK3
  cairo_surface_t *surface;
  cairo_region_t *region;
#ifdef FG_DRAW
  cairo_surface_t *surface_sdw;
  cairo_region_t  *region_sdw;
#endif
#endif


#ifdef USE_WIN32
  GdkWinChangeAlpha(gtk_widget_get_window(mascot->win_main), mascot->alpha_main);
#ifdef USE_BIFF
  GdkWinChangeAlpha(gtk_widget_get_window(mascot->biff_pix), mascot->alpha_biff);
#endif
#endif
#ifdef FG_DRAW
  GdkWinChangeAlpha(gtk_widget_get_window(mascot->win_sdw),  mascot->sdw_alpha);
  if((mascot->flag_clkfg)&&(mascot->alpha_clk!=100)){
    GdkWinChangeAlphaFG(gtk_widget_get_window(mascot->clock_fg), mascot->colclkbg);
  }
  GdkWinChangeAlpha(gtk_widget_get_window(mascot->clock_main), mascot->alpha_clk);
  if((mascot->flag_balfg)&&(mascot->alpha_bal!=100)){
    GdkWinChangeAlphaFG(gtk_widget_get_window(mascot->balloon_fg), mascot->colbalbg);
  }
  GdkWinChangeAlpha(gtk_widget_get_window(mascot->balloon_main), mascot->alpha_bal);
#endif

  while(mascot->sprites[i].filename){
#ifdef USE_GTK3
    if (mascot->sprites[i].pixbuf){
      g_object_unref(G_OBJECT(mascot->sprites[i].pixbuf));
      mascot->sprites[i].pixbuf=NULL;
    }
#ifdef FG_DRAW
    if (mascot->sprites[i].pixbuf_sdw){
      g_object_unref(G_OBJECT(mascot->sprites[i].pixbuf_sdw));
      mascot->sprites[i].pixbuf_sdw=NULL;
    }
#endif
#else   //////////  GTK2  /////////    
    if (mascot->sprites[i].pixmap){
      g_object_unref(G_OBJECT(mascot->sprites[i].pixmap));
      mascot->sprites[i].pixmap=NULL;
    }
    if (mascot->sprites[i].mask){
      g_object_unref(G_OBJECT(mascot->sprites[i].mask));
      mascot->sprites[i].mask=NULL;
    }
    
#ifdef FG_DRAW
    if (mascot->sprites[i].pixmap_sdw){
      g_object_unref(G_OBJECT(mascot->sprites[i].pixmap_sdw));
      mascot->sprites[i].pixmap_sdw=NULL;
    }
    if (mascot->sprites[i].mask_sdw){
      g_object_unref(G_OBJECT(mascot->sprites[i].mask_sdw));
      mascot->sprites[i].mask_sdw=NULL;
    }
#endif
#endif  // USE_GTK3

    //im = gdk_imlib_load_image(mascot->sprites[i].filename);
    tmp_open=to_utf8(mascot->sprites[i].filename);
    pixbuf = gdk_pixbuf_new_from_file(tmp_open, NULL);
    // if(im==NULL){
    if(pixbuf==NULL){
      g_print (_("Cannot Load Image %s\n"), tmp_open);
      g_free(tmp_open);
      exit(1);
    }
    g_free(tmp_open);

    w = gdk_pixbuf_get_width(pixbuf)*((gdouble)(mascot->magnify)/100);
    h = gdk_pixbuf_get_height(pixbuf)*((gdouble)(mascot->magnify)/100);

#ifdef USE_WIN32
    if(mascot->move==MOVE_FOCUS){
      if(mascot->yoff>0){
	mascot->sdw_height
	  =mascot->yoff*(gdouble)(mascot->magnify)/100+mascot->sdw_y;
      }
      else{
	mascot->sdw_height=0;
      }
    }
    else{
      mascot->sdw_height=h;
    }
#endif
    
    switch(mascot->ip_style){
    case MAG_IP_NEAREST:
      ipstyle=GDK_INTERP_NEAREST;
      break;

    case MAG_IP_TILES:
      ipstyle=GDK_INTERP_TILES;
      break;

    case MAG_IP_BILINEAR:
      ipstyle=GDK_INTERP_BILINEAR;
      break;

    case MAG_IP_HYPER:
      ipstyle=GDK_INTERP_HYPER;
      break;

    default:
      ipstyle=GDK_INTERP_BILINEAR;
    }
    
    pixbuf2=gdk_pixbuf_scale_simple(pixbuf,w,h,ipstyle);

#ifdef USE_GTK3   //////////////////  GTK3  ///////////////////
    if(flag_img_cairo_go){
      if(mascot->sdw_flag){
	w= w + mascot->sdw_x_int;
	h= h + mascot->sdw_y_int;
      }
      
      surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
      cr = cairo_create(surface);
      
      // Clear BG
      cairo_set_source_rgba(cr, 0, 0, 0, 0);
      cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
      cairo_rectangle(cr, 0, 0, w, h);
      cairo_fill(cr);
      cairo_paint(cr);


      if((mascot->sdw_flag)){
	// Shadow
	cairo_save(cr);
	if(mascot->sdw_x<0){
	  gdk_cairo_set_source_pixbuf(cr, pixbuf2, 0, mascot->sdw_y);
	}
	else{
	  gdk_cairo_set_source_pixbuf(cr, pixbuf2, mascot->sdw_x, mascot->sdw_y);
	}
	cairo_paint(cr);
	region = gdk_cairo_region_create_from_surface(surface);

	cairo_set_source_rgba(cr, 0, 0, 0, 0);
	cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
	cairo_rectangle(cr, 0, 0, w, h);
	cairo_fill(cr);
	cairo_paint(cr);

	cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
	gdk_cairo_region(cr, region);
	cairo_set_source_rgba(cr, 0, 0, 0, 1.0);
	cairo_clip(cr);
	cairo_paint_with_alpha (cr, (gdouble)(mascot->sdw_alpha)/100);
	cairo_region_destroy(region);

	cairo_restore(cr);
	
	// Remove shadow above title bar
	if(mascot->move==MOVE_FOCUS){
	  cairo_save(cr);
	  
	  cairo_set_operator (cr, CAIRO_OPERATOR_CLEAR);
	  cairo_move_to(cr,0.0,0.0);
	  cairo_rectangle(cr, 0, 0, w, 
			  h-mascot->yoff*(gdouble)(mascot->magnify)/100-mascot->sdw_y_int);
	  cairo_fill(cr);
	  cairo_restore(cr);
	}
      }

      // Mascot
      cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
      cairo_move_to(cr,0.0,0.0);
      if((mascot->sdw_flag)&&(mascot->sdw_x<0)){
	gdk_cairo_set_source_pixbuf(cr,pixbuf2,-mascot->sdw_x,0);
      }
      else{
	gdk_cairo_set_source_pixbuf(cr,pixbuf2,0,0);
      }
      
      cairo_paint_with_alpha (cr, (gdouble)mascot->alpha_main/100);
      //cairo_paint (cr);

      cairo_destroy (cr);
      mascot->sprites[i].pixbuf = gdk_pixbuf_get_from_surface(surface, 0, 0, w, h);
    }
    else{// for Win32 and non-composited Gtk+2.8 or later
#ifdef FG_DRAW
      surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
      cr = cairo_create(surface);

      cairo_set_source_rgba(cr, 0, 0, 0, 0);
      cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
      cairo_rectangle(cr, 0, 0, w, h);
      cairo_fill(cr);
      cairo_paint(cr);
      
      gdk_cairo_set_source_pixbuf(cr, pixbuf2, 0, 0);
      cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
      cairo_paint(cr);
      region = gdk_cairo_region_create_from_surface(surface);
      cairo_destroy(cr);
      
      mascot->sprites[i].pixbuf = gdk_pixbuf_get_from_surface(surface, 0, 0, w, h);
      cairo_region_destroy(region);
      cairo_surface_destroy(surface);
      
      if(mascot->sdw_flag){
	gint h_sdw, h0;
	
	if(mascot->move==MOVE_FOCUS){
	  if(mascot->sdw_height>0){
	    h_sdw=mascot->sdw_height;
	    h0=h-h_sdw;
	  }
	  else{
	    h_sdw=h;
	    h0=0;
	  }
	  surface_sdw = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h_sdw);
	  cr = cairo_create(surface_sdw);
	  
	  cairo_set_source_rgba(cr, 0, 0, 0, 0);
	  cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
	  cairo_rectangle(cr, 0, 0, w, h_sdw);
	  cairo_fill(cr);
	  cairo_paint(cr);
	  
	  
	  // Shadow
	  gdk_cairo_set_source_pixbuf(cr, pixbuf2, 0, h0);
	  cairo_paint(cr);
	  region_sdw = gdk_cairo_region_create_from_surface(surface_sdw);
	  
	  cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
	  cairo_set_source_rgba(cr, 0, 0, 0, 1);
	  gdk_cairo_region(cr, region_sdw);
	  cairo_clip(cr);
	  cairo_paint(cr);
	  
	  cairo_destroy(cr);
	  
	  mascot->sprites[i].pixbuf_sdw = gdk_pixbuf_get_from_surface(surface_sdw, 0, 0, w, h_sdw);
	  cairo_region_destroy(region_sdw);
	  cairo_surface_destroy(surface_sdw);
	}
      }
#else
      mascot->sprites[i].pixbuf = gdk_pixbuf_copy(pixbuf2);
#endif   
    }

#else  /////////////  GTK2  //////////////    
    if(flag_img_cairo_go){
      GdkBitmap *mask=NULL;
      
      if(mascot->sdw_flag){
	w= w + mascot->sdw_x_int;
	h= h + mascot->sdw_y_int;
	
	gdk_pixbuf_render_pixmap_and_mask(pixbuf, NULL, &mask, 0x01);
      }
      
      gdk_pixbuf_render_pixmap_and_mask(pixbuf2, NULL,
					&mascot->sprites[i].mask, 0xc0);
      
      mascot->sprites[i].pixmap = gdk_pixmap_new(gtk_widget_get_window(mascot->win_main),
					 w,
					 h,
					 -1);
      
      // Clear BG
      cr = gdk_cairo_create(mascot->sprites[i].pixmap);
      cairo_set_source_rgba (cr, 0, 0, 0, 0);
      cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
      cairo_paint (cr);


      if((mascot->sdw_flag)){
	// Shadow
	cairo_save(cr);
	cairo_scale (cr, (gdouble)(mascot->magnify)/100, 
		     (gdouble)(mascot->magnify)/100);
	if(mascot->sdw_x<0){
	  gdk_cairo_set_source_pixmap(cr,mask,0, mascot->sdw_y);
	}
	else{
	  gdk_cairo_set_source_pixmap(cr,mask,mascot->sdw_x, mascot->sdw_y);
	}
	cairo_paint_with_alpha (cr, (gdouble)(mascot->sdw_alpha)/100);
	cairo_restore(cr);

	// Remove shadow above title bar
	if(mascot->move==MOVE_FOCUS){
	  cairo_save(cr);
	  
	  cairo_set_operator (cr, CAIRO_OPERATOR_CLEAR);
	  cairo_move_to(cr,0.0,0.0);
	  cairo_rectangle(cr, 0, 0, w, 
			  h-mascot->yoff*(gdouble)(mascot->magnify)/100-mascot->sdw_y_int);
	  cairo_fill(cr);
	  cairo_restore(cr);
	}
      }

      // Mascot
      cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
      cairo_move_to(cr,0.0,0.0);
      if((mascot->sdw_flag)&&(mascot->sdw_x<0)){
	gdk_cairo_set_source_pixbuf(cr,pixbuf2,-mascot->sdw_x,0);
      }
      else{
	gdk_cairo_set_source_pixbuf(cr,pixbuf2,0,0);
      }
      
      cairo_paint_with_alpha (cr, (gdouble)mascot->alpha_main/100);
      //cairo_paint (cr);

      cairo_destroy (cr);

      if(mascot->sdw_flag){
	if(mask) g_object_unref(G_OBJECT(mask));
      }
    }
    else{// for Win32 and non-composited Gtk+2.8 or later
      gdk_pixbuf_render_pixmap_and_mask(pixbuf2, &mascot->sprites[i].pixmap,
					&mascot->sprites[i].mask, 0xc0); 
#ifdef FG_DRAW
      if(mascot->sdw_flag){
	
	if(mascot->move==MOVE_FOCUS){
	  if(mascot->sdw_height>0){
	    
	    mascot->sprites[i].pixmap_sdw
	      =gdk_pixmap_new(gtk_widget_get_window(mascot->win_sdw),
			      w, mascot->sdw_height,
			      -1);
	    
	    mascot->sprites[i].mask_sdw
	      = gdk_pixmap_new(gtk_widget_get_window(mascot->win_sdw),
			       w, mascot->sdw_height,
			       1); // Depth =1 (Bitmap)
	    
	    
	    gdk_draw_drawable(mascot->sprites[i].pixmap_sdw,
			      mascot->win_sdw->style->fg_gc[GTK_WIDGET_STATE(mascot->win_sdw)],
			      mascot->sprites[i].mask,
			      0, h-mascot->sdw_height,
			      0,0,
			      w,mascot->sdw_height);
	    
	    gdk_draw_drawable(mascot->sprites[i].mask_sdw,
			      mascot->win_sdw->style->fg_gc[GTK_WIDGET_STATE(mascot->win_sdw)],
			      mascot->sprites[i].mask,
			      0, h-mascot->sdw_height,
			      0,0,
			      w,mascot->sdw_height);
	  }
	}
	else{
	  mascot->sprites[i].pixmap_sdw
	    =gdk_pixmap_new(gtk_widget_get_window(mascot->win_sdw),w,h,-1);
	  
	  mascot->sprites[i].mask_sdw
	    = gdk_pixmap_new(gtk_widget_get_window(mascot->win_sdw), w,h,1); // Depth =1 (Bitmap)
	  
	  gdk_draw_drawable(mascot->sprites[i].pixmap_sdw,
			    mascot->win_sdw->style->fg_gc[GTK_WIDGET_STATE(mascot->win_sdw)],
			    mascot->sprites[i].mask,
			    0,0,
			    0,0,
			    w,h);
	  
	  gdk_draw_drawable(mascot->sprites[i].mask_sdw,
			    mascot->win_sdw->style->fg_gc[GTK_WIDGET_STATE(mascot->win_sdw)],
			    mascot->sprites[i].mask,
			    0,0,
			    0,0,
			    w,h);
	}
      }
#endif   
    }
#endif // USE_GTK3  ////////////////////////////////////////
    
    g_object_unref(G_OBJECT(pixbuf));
    g_object_unref(G_OBJECT(pixbuf2));
    i++;
  }

  mascot->width=w;
  mascot->height=h;

  gtk_window_resize (GTK_WINDOW(mascot->win_main), w, h);
  gtk_widget_set_size_request (mascot->dw_main, w, h);

  DrawMascot0(mascot);
  dw_init_main(mascot->dw_main, "configure_event",(gpointer)mascot);


#ifdef FG_DRAW
  if((mascot->sdw_flag)&&(mascot->sdw_height>0)){
    gtk_window_resize (GTK_WINDOW(mascot->win_sdw), w, mascot->sdw_height);
    gtk_widget_set_size_request (mascot->dw_sdw, w, mascot->sdw_height);
  }
#endif
  
  // フォントのロード
  if (mascot->fontclk!=NULL){
    pango_font_description_free(mascot->fontclk);
    mascot->fontclk=NULL;
  }

  if(mascot->fontname_clk){
    mascot->fontclk=pango_font_description_from_string(mascot->fontname_clk);
  }
  if(mascot->fontclk==NULL){
    mascot->fontclk=pango_font_description_from_string(mascot->deffontname_clk);
    while(mascot->fontclk==NULL){
      //create_default_font_selection_dialog(mascot,INIT_DEF_FONT_CLK);
      if(mascot->deffontname_clk) g_free(mascot->deffontname_clk);
      mascot->deffontname_clk=g_strdup(FONT_CLK);
      mascot->fontclk=pango_font_description_from_string(mascot->deffontname_clk);
    }
    mascot->fontname_clk=mascot->deffontname_clk;
  }

  {
    if(mascot->fontclk_pc.family) g_free(mascot->fontclk_pc.family);
    mascot->fontclk_pc.family
      =g_strdup(pango_font_description_get_family(mascot->fontclk));
    mascot->fontclk_pc.pointsize
      =(gdouble)pango_font_description_get_size(mascot->fontclk)/PANGO_SCALE;
    if(pango_font_description_get_weight (mascot->fontclk) 
       > PANGO_WEIGHT_SEMIBOLD){
      mascot->fontclk_pc.weight=CAIRO_FONT_WEIGHT_BOLD;
    }
    else{
      mascot->fontclk_pc.weight=CAIRO_FONT_WEIGHT_NORMAL;
    }
    switch (pango_font_description_get_style (mascot->fontclk)) {
    case (PANGO_STYLE_NORMAL):  
      mascot->fontclk_pc.slant = CAIRO_FONT_SLANT_NORMAL; break;
    case (PANGO_STYLE_OBLIQUE): 
      mascot->fontclk_pc.slant = CAIRO_FONT_SLANT_OBLIQUE; break;
    case (PANGO_STYLE_ITALIC):  
      mascot->fontclk_pc.slant = CAIRO_FONT_SLANT_ITALIC; break;
    }
  }

  if (mascot->fontbal!=NULL){
    pango_font_description_free(mascot->fontbal);
    mascot->fontbal=NULL;
  }
  
  if(mascot->fontname_bal){
    mascot->fontbal=pango_font_description_from_string(mascot->fontname_bal);
  }
  if(mascot->fontbal==NULL){
    mascot->fontbal=pango_font_description_from_string(mascot->deffontname_bal);
    while(mascot->fontbal==NULL){
      //create_default_font_selection_dialog(mascot,INIT_DEF_FONT_BAL);
      if(mascot->deffontname_bal) g_free(mascot->deffontname_bal);
      mascot->deffontname_bal=g_strdup(FONT_BAL);
      mascot->fontbal=pango_font_description_from_string(mascot->deffontname_bal);
    }
    mascot->fontname_bal=mascot->deffontname_bal;
  }

  {
    if(mascot->fontbal_pc.family) g_free(mascot->fontbal_pc.family);
    mascot->fontbal_pc.family
      =g_strdup(pango_font_description_get_family(mascot->fontbal));
    mascot->fontbal_pc.pointsize
      =(gdouble)pango_font_description_get_size(mascot->fontbal)/PANGO_SCALE;
    if(pango_font_description_get_weight (mascot->fontbal) 
       > PANGO_WEIGHT_SEMIBOLD){
      mascot->fontbal_pc.weight=CAIRO_FONT_WEIGHT_BOLD;
    }
    else{
      mascot->fontbal_pc.weight=CAIRO_FONT_WEIGHT_NORMAL;
    }
    switch (pango_font_description_get_style (mascot->fontbal)) {
    case (PANGO_STYLE_NORMAL):  
      mascot->fontbal_pc.slant = CAIRO_FONT_SLANT_NORMAL; break;
    case (PANGO_STYLE_OBLIQUE): 
      mascot->fontbal_pc.slant = CAIRO_FONT_SLANT_OBLIQUE; break;
    case (PANGO_STYLE_ITALIC):  
      mascot->fontbal_pc.slant = CAIRO_FONT_SLANT_ITALIC; break;
    }
  }

#ifdef USE_GTK3
  css_change_font(mascot->dw_balloon,mascot->fontbal_pc);
  css_change_font(mascot->dw_clock,mascot->fontclk_pc);
#ifdef FG_DRAW
  css_change_font(mascot->dw_clkfg,mascot->fontclk_pc);
  css_change_font(mascot->dw_balfg,mascot->fontbal_pc);
#endif
#else
  gtk_widget_modify_font(mascot->clock_main,mascot->fontclk);
  gtk_widget_modify_font(mascot->balloon_main,mascot->fontbal);
#endif
  mascot->fontname_clk=pango_font_description_to_string(mascot->fontclk);
  mascot->fontname_bal=pango_font_description_to_string(mascot->fontbal);

  
  mascot->nPixmap = i;
  mascot->drag=FALSE;
  if(!mascot->digit)  strcpy(mascot->digit,"00:00:00");

  for(i=mascot->nPixmap;i<MAX_PIXMAP;i++){
    if(mascot->sprites[i].filename){
      g_free(mascot->sprites[i].filename);
    }
    mascot->sprites[i].filename=NULL;
#ifdef USE_GTK3
    if(mascot->sprites[i].pixbuf){
      g_object_unref(G_OBJECT(mascot->sprites[i].pixbuf));
    }
    mascot->sprites[i].pixbuf=NULL;
#else
    if(mascot->sprites[i].pixmap){
      g_object_unref(G_OBJECT(mascot->sprites[i].pixmap));
    }
    mascot->sprites[i].pixmap=NULL;
    if(mascot->sprites[i].mask){
      g_object_unref(G_OBJECT(mascot->sprites[i].mask));
    }
    mascot->sprites[i].mask=NULL;
#endif
  }
  
  // 初回フレーム用マスク切出し

  /*
#ifdef USE_GTK3   /////////////  GTK3  ////////////////
#ifdef USE_WIN32
  if(mascot->sdw_flag){
    gdk_window_shape_combine_region(gtk_widget_get_window(mascot->win_sdw),
				    region_sdw, 0, 0 );
    cairo_region_destroy(region_sdw);
    cairo_surface_destroy(surface_sdw);
  }
  gdk_window_shape_combine_region(gtk_widget_get_window(mascot->win_main),
				  region, 0, 0 );
  cairo_region_destroy(region);
  cairo_surface_destroy(surface);
#else
  region = get_cairo_region_from_pixbuf(mascot->sprites[mascot->frame_pix[0][0]].pixbuf,
					mascot->width, mascot->height, 0, 0);
  if(flag_img_cairo_go){
    gdk_window_input_shape_combine_region(gtk_widget_get_window(mascot->win_main),
					  region, 0, 0 );
    gdk_window_set_cursor(gtk_widget_get_window(mascot->win_main),
			  mascot->cursor.normal);
  }
  else{
    gdk_window_shape_combine_region(gtk_widget_get_window(mascot->win_main),
					  region, 0, 0 );
  }

  cairo_region_destroy(region);
  cairo_surface_destroy(surface);
#endif

#else  /////////////  GTK2  ////////////////
  if(flag_img_cairo_go){
    gdk_window_input_shape_combine_mask
      ( gtk_widget_get_window(mascot->win_main), 
  	mascot->sprites[mascot->frame_pix[0][0]].mask,
 	0, 0 ); 
    gdk_window_set_cursor(gtk_widget_get_window(mascot->win_main),
			  mascot->cursor.normal);
  }
  else{
    gdk_window_shape_combine_mask
      ( gtk_widget_get_window(mascot->win_main), 
  	mascot->sprites[mascot->frame_pix[0][0]].mask,
 	0, 0 ); 
  }
#endif  // USE_GTK3
  */

  while (my_main_iteration(FALSE));
  
#ifdef USE_GTK_STATUS_ICON
  trayicon_set_tooltip(mascot);
#endif

}


#ifdef USE_BIFF
void LoadBiffPixmap(GtkWidget *widget, typMascot *mascot){
  gchar *fontset=NULL;
  int i=0;
  //GdkImlibImage* im = NULL;
  GdkPixbuf *pixbuf = NULL, *pixbuf2 = NULL;
  gint ipstyle;
  gint w=0,h=0;
  gchar *tmp_open;
  cairo_t *cr;
#ifdef USE_GTK3
  cairo_surface_t *surface;
  cairo_region_t *region;
#endif

  switch(mascot->ip_style){
  case MAG_IP_NEAREST:
    ipstyle=GDK_INTERP_NEAREST;
    break;
    
  case MAG_IP_TILES:
    ipstyle=GDK_INTERP_TILES;
    break;
    
  case MAG_IP_BILINEAR:
    ipstyle=GDK_INTERP_BILINEAR;
    break;
    
  case MAG_IP_HYPER:
    ipstyle=GDK_INTERP_HYPER;
    break;
    
  default:
    ipstyle=GDK_INTERP_BILINEAR;
  }


  // メイル用pixmap
#ifdef USE_GTK3
  if (mascot->mail.pixbuf){
    g_object_unref(G_OBJECT(mascot->mail.pixbuf));
    mascot->mail.pixbuf=NULL;
  }
#else /////// GTK2 /////////  
  if (mascot->mail.pixmap){
    g_object_unref(G_OBJECT(mascot->mail.pixmap));
    mascot->mail.pixmap=NULL;
  }
  if (mascot->mail.mask){
    g_object_unref(G_OBJECT(mascot->mail.mask));
    mascot->mail.mask=NULL;
  }
#endif
  
  if(mascot->mail.pix_file){
    tmp_open=to_utf8(mascot->mail.pix_file);
    pixbuf = gdk_pixbuf_new_from_file(tmp_open, NULL);
    g_free(tmp_open);
  }
  else{
    if(mascot->mail.pix_pos==MAIL_PIX_LEFT){
      pixbuf = gdk_pixbuf_new_from_resource ("/icons/def_biff.png", NULL);
    }
    else{
      pixbuf = gdk_pixbuf_new_from_resource ("/icons/def_biff_r.png", NULL);
    }
  }
  w = gdk_pixbuf_get_width(pixbuf)*((gdouble)(mascot->magnify)/100);
  h = gdk_pixbuf_get_height(pixbuf)*((gdouble)(mascot->magnify)/100);

  pixbuf2=gdk_pixbuf_scale_simple(pixbuf,w,h,ipstyle);

# ifdef USE_GTK3 ////////////////  GTK3  /////////////////  
  if(flag_img_cairo_go){
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
    cr = cairo_create(surface);
    
    cairo_set_source_rgba(cr, 0, 0, 0, 0);
    cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
    cairo_rectangle(cr, 0, 0, w, h);
    cairo_fill(cr);
    cairo_paint(cr);
    
    cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
    gdk_cairo_set_source_pixbuf(cr,pixbuf2,0,0);
    
    cairo_paint_with_alpha (cr, (gdouble)mascot->alpha_biff/100);
    
    cairo_destroy (cr);

    mascot->mail.pixbuf = gdk_pixbuf_get_from_surface(surface, 0, 0, w, h);
    cairo_surface_destroy (surface);
  }
  else{
    mascot->mail.pixbuf = gdk_pixbuf_copy(pixbuf2);
  }
  
# else ////////////////  GTK2  /////////////////  
  if(flag_img_cairo_go){
    mascot->mail.pixmap = gdk_pixmap_new(gtk_widget_get_window(mascot->biff_pix),
					 w,
					 h,
					 -1);
    cr = gdk_cairo_create(mascot->mail.pixmap);
    cairo_set_source_rgba (cr, 1, 1, 1, 0);
    cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint (cr);
    
    cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
    gdk_cairo_set_source_pixbuf(cr,pixbuf2,0,0);
    
    cairo_paint_with_alpha (cr, (gdouble)mascot->alpha_biff/100);
    
    cairo_destroy (cr);

    gdk_pixbuf_render_pixmap_and_mask(pixbuf2, NULL,
				      &mascot->mail.mask, 0xc0);
  }
  else{
    gdk_pixbuf_render_pixmap_and_mask(pixbuf2, &mascot->mail.pixmap,
				      &mascot->mail.mask, 0xc0);
  }
#endif
  
  g_object_unref(G_OBJECT(pixbuf));
  g_object_unref(G_OBJECT(pixbuf2));

  // 初回フレーム用マスク切出し
  
  gtk_widget_realize(mascot->dw_biff);
  gtk_window_resize (GTK_WINDOW(widget), w, h);
  gtk_widget_set_size_request (mascot->dw_biff, w, h);

#ifdef USE_GTK3  ///////////////  GTK3  //////////////
  gtk_widget_queue_draw(mascot->dw_biff);

  region = get_cairo_region_from_pixbuf(mascot->mail.pixbuf, w, h, 0, 0);
  
  if(flag_img_cairo_go){
    gdk_window_input_shape_combine_region( gtk_widget_get_window(widget),
					   region, 0, 0 ); 
    gdk_window_set_cursor(gtk_widget_get_window(widget),
			  mascot->cursor.biff);
  }
  else{
    gdk_window_shape_combine_region(gtk_widget_get_window(widget),
				    region, 0, 0 ); 
  }
#else  ///////////////  GTK2  //////////////
  {
    GtkAllocation *allocation=g_new(GtkAllocation, 1);
    GtkStyle *style=gtk_widget_get_style(mascot->dw_biff);
    gtk_widget_get_allocation(mascot->dw_biff,allocation);
    
    gdk_draw_drawable(gtk_widget_get_window(mascot->dw_biff),
		      mascot->gc_main,
		      mascot->mail.pixmap,
		      0,0,0,0,
		      allocation->width,
		      allocation->height);
    g_free(allocation);
  }

  if(flag_img_cairo_go){
    gdk_window_input_shape_combine_mask( widget->window, 
					 mascot->mail.mask,
					 0, 0 ); 
    gdk_window_set_cursor(widget->window,mascot->cursor.biff);
  }
  else{
    gdk_window_shape_combine_mask( widget->window, 
				   mascot->mail.mask,
				   0, 0 ); 
  }
#endif
  
}
#endif

// マスコットを変換した際にバッファを満たしておく
gint DrawMascot0(typMascot *mascot)
{
#ifdef USE_GTK3  ////////////////////// GTK3 ////////////////////////////////////
  if (pixbuf_main) {
    g_object_unref(G_OBJECT(pixbuf_main));
  }
  pixbuf_main=gdk_pixbuf_copy(mascot->sprites[mascot->frame_pix[0][0]].pixbuf);

#ifdef FG_DRAW
  if(mascot->sdw_flag){
    if(mascot->sdw_height>0){
      if (pixbuf_sdw) {
	g_object_unref(G_OBJECT(pixbuf_sdw));
      }
      pixbuf_sdw=gdk_pixbuf_copy(mascot->sprites[mascot->frame_pix[0][0]].pixbuf_sdw);
      gtk_widget_queue_draw(mascot->dw_sdw);
    }
  }
      
#endif
  gtk_widget_queue_draw(mascot->dw_main);
	
#else     ////////////////////// GTK2 ////////////////////////////////////
  if (pixmap_main) {
    g_object_unref(G_OBJECT(pixmap_main));
  } 
   
  pixmap_main = gdk_pixmap_new(gtk_widget_get_window(mascot->win_main),
			       mascot->width,
			       mascot->height,
			       -1);
#ifdef FG_DRAW
  if(mascot->sdw_flag){
    if(mascot->sdw_height>0){
      if (pixmap_sdw) {
	g_object_unref(G_OBJECT(pixmap_sdw));
      } 

      pixmap_sdw = gdk_pixmap_new(gtk_widget_get_window(mascot->win_sdw),
				  mascot->width,
				  mascot->sdw_height,
				  -1);
    }
  }
#endif
#endif // USE_GTK3    

  return(0);
}


// Buffer中へのマスコットの書き込み
gint DrawMascot(typMascot *mascot, gint i_pix)
{
#ifdef USE_GTK3  
  cairo_region_t *region;
#endif

#ifdef USE_GTK3
  if (pixbuf_main) {
    g_object_unref(G_OBJECT(pixbuf_main));
  }

  //pixbuf_main = gdk_pixbuf_copy(mascot->sprites[mascot->frame_pix[mascot->anime_ptn][mascot->anime_frm]].pixbuf);
  pixbuf_main = gdk_pixbuf_copy(mascot->sprites[i_pix].pixbuf);
#ifdef FG_DRAW
  if((mascot->sdw_flag)&&(mascot->sdw_height>0)){
    if (pixbuf_sdw) {
      g_object_unref(G_OBJECT(pixbuf_sdw));
    }
    
    //pixbuf_sdw = gdk_pixbuf_copy(mascot->sprites[mascot->frame_pix[mascot->anime_ptn][mascot->anime_frm]].pixbuf_sdw);
    pixbuf_sdw = gdk_pixbuf_copy(mascot->sprites[i_pix].pixbuf_sdw);
  }

#endif
  region = get_cairo_region_from_pixbuf(pixbuf_main,
					mascot->width, mascot->height,
					0,0);
  
  if(flag_img_cairo_go){
    gdk_window_input_shape_combine_region(gtk_widget_get_window(mascot->win_main),
					  region, 0, 0 );
  }
  else{
    gdk_window_shape_combine_region(gtk_widget_get_window(mascot->win_main),
				    region, 0, 0 );
  }

  cairo_region_destroy(region);

#ifdef FG_DRAW
  if((mascot->sdw_flag)&&(mascot->sdw_height>0)){
    region = get_cairo_region_from_pixbuf(pixbuf_sdw,
					  mascot->width, mascot->sdw_height,
					  0,0);
    
    if(flag_img_cairo_go){
      gdk_window_input_shape_combine_region(gtk_widget_get_window(mascot->win_sdw),
					    region, 0, 0 );
    }
    else{
      gdk_window_shape_combine_region(gtk_widget_get_window(mascot->win_sdw),
				      region, 0, 0 );
    }
    
    cairo_region_destroy(region);
    while (my_main_iteration(FALSE));

    gtk_widget_queue_draw(mascot->dw_sdw);
  }
#endif
  while (my_main_iteration(FALSE));
  //printf("DrawMascot  i_pix=%d\n",i_pix);
  gtk_widget_queue_draw(mascot->dw_main);
  
#else  ///////////////////////// USE_GTK2 ///////////////////////////////
  if (pixmap_main) {
    g_object_unref(G_OBJECT(pixmap_main));
  } 
  
  pixmap_main = gdk_pixmap_new(gtk_widget_get_window(mascot->win_main),
			       mascot->width,
			       mascot->height,
			       -1);
  
#ifdef FG_DRAW
  if((mascot->sdw_flag)&&(mascot->sdw_height>0)){
    if (pixmap_sdw) {
      g_object_unref(G_OBJECT(pixmap_sdw));
    } 
    
    pixmap_sdw = gdk_pixmap_new(gtk_widget_get_window(mascot->win_sdw),
				mascot->width,
				mascot->sdw_height,
				-1);
  }
#endif
  gdk_draw_drawable(pixmap_main,
		    mascot->gc_main,
		    //mascot->sprites[mascot->frame_pix[mascot->anime_ptn][mascot->anime_frm]].pixmap,
		    mascot->sprites[i_pix].pixmap,
		    0, 0,
		    0, 0,
		    mascot->width, mascot->height);
  
#ifdef FG_DRAW
  if((mascot->sdw_flag)&&(mascot->sdw_height>0)){
    gdk_draw_drawable(pixmap_sdw,
		      mascot->gc_main,
		      mascot->sprites[i_pix].pixmap_sdw,
		      //mascot->sprites[mascot->frame_pix[mascot->anime_ptn][mascot->anime_frm]].pixmap_sdw,
		      0, 0,
		      0, 0,
		      mascot->width, mascot->sdw_height);
  }
#endif

  if(flag_img_cairo_go){
    gdk_window_input_shape_combine_mask
      ( gtk_widget_get_window(mascot->win_main),
	//mascot->sprites[mascot->frame_pix[mascot->anime_ptn][mascot->anime_frm]].mask,
	mascot->sprites[i_pix].mask,
	0, 0 );
    gdk_window_set_cursor(gtk_widget_get_window(mascot->win_main),
			  mascot->cursor.normal);
  }
  else{
    gdk_window_shape_combine_mask
      ( gtk_widget_get_window(mascot->win_main),
	//mascot->sprites[mascot->frame_pix[mascot->anime_ptn][mascot->anime_frm]].mask,
	mascot->sprites[i_pix].mask,
	0, 0 );
#ifdef FG_DRAW
    if((mascot->sdw_flag)&&(mascot->sdw_height>0)){
      gdk_window_shape_combine_mask
	( gtk_widget_get_window(mascot->win_sdw),
	  // mascot->sprites[mascot->frame_pix[mascot->anime_ptn][mascot->anime_frm]].mask_sdw,
	  mascot->sprites[i_pix].mask_sdw,
	  0, 0 );
    }
#endif
  }
  
  {
    GtkStyle *style=gtk_widget_get_style(mascot->dw_main);
    
    gdk_draw_drawable(gtk_widget_get_window(mascot->dw_main),
		      style->fg_gc[GTK_WIDGET_STATE(mascot->dw_main)],
		      pixmap_main,
		      0,0,0,0,
		      mascot->width, mascot->height);
  }
  
#ifdef FG_DRAW
  if((mascot->sdw_flag)&&(mascot->sdw_height>0)){
    GtkStyle *style=gtk_widget_get_style(mascot->dw_sdw);
    
    gdk_draw_drawable(gtk_widget_get_window(mascot->dw_sdw),
		      style->fg_gc[GTK_WIDGET_STATE(mascot->win_sdw)],
		      pixmap_sdw,
		      0,0,0,0,
		      mascot->width, mascot->sdw_height);
  }
#endif  
#endif // USE_GTK3
  
  if(mascot->raise_kwin) raise_all();

  return(0);
} 

// Buffer中へのマスコットの書き込み
gint DrawMascotWithDigit(typMascot *mascot){
  gint clk_width,clk_height;
  //GdkGC *gc;
  gint as=0;
  PangoLayout *pango_text;
  cairo_t *cr;
  cairo_text_extents_t extents;
  gdouble ampmsize=0;
#ifdef USE_GTK3
  cairo_surface_t *surface;
  cairo_region_t *region;
#endif
  
#ifdef USE_GTK3  //////////////  GTK3  //////////////
  if (pixbuf_main) {
    g_object_unref(G_OBJECT(pixbuf_main));
  } 
  
  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
				       mascot->width, mascot->height);
  
  cr = cairo_create(surface);
  
  cairo_set_source_rgba(cr, 0, 0, 0, 0);
  cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
  cairo_rectangle(cr, 0, 0, mascot->width, mascot->height);
  cairo_fill(cr);
  cairo_paint(cr);
  
  gdk_cairo_set_source_pixbuf(cr,
			      mascot->sprites[mascot->frame_pix[mascot->anime_ptn][mascot->anime_frm]].pixbuf,
			      0, 0);
  cairo_paint(cr);
  
  if(flag_img_cairo_go){
#ifdef USE_PANGOCAIRO
    pango_text=gtk_widget_create_pango_layout(mascot->clock_main,
					      mascot->digit);
    pango_layout_get_pixel_size(pango_text,&clk_width,&clk_height);
#endif
    cairo_select_font_face (cr, 
			    mascot->fontclk_pc.family,
			    mascot->fontclk_pc.slant,
			    mascot->fontclk_pc.weight);

    cairo_set_font_size (cr, mascot->fontclk_pc.pointsize*96.0/72.0);
#ifndef USE_PANGOCAIRO
    cairo_text_extents (cr, mascot->digit, &extents);
    clk_width=(gint)(extents.x_advance);
    clk_height=(gint)(-extents.y_bearing);
#endif

    cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
    
    if(mascot->flag_clksd){
      cairo_set_source_rgba (cr, 
			     (gdouble)mascot->colclksd->red/0xFFFF,
			     (gdouble)mascot->colclksd->green/0xFFFF,
			     (gdouble)mascot->colclksd->blue/0xFFFF,
			     (gdouble)mascot->alpclksd/100.0); /* transparent */
#ifdef USE_PANGOCAIRO
      cairo_move_to(cr,
		    (gdouble)(mascot->clktext_x+mascot->clksd_x),
		    (gdouble)(mascot->clktext_y+mascot->clksd_y));
      pango_cairo_show_layout(cr,pango_text);
#else
      cairo_move_to(cr,
		    (gdouble)(mascot->clktext_x+mascot->clksd_x),
		    (gdouble)(clk_height+mascot->clktext_y+mascot->clksd_y));
      cairo_show_text(cr,mascot->digit);
#endif
      switch(mascot->clktype){
      case CLOCK_TYPE_12S:
      case CLOCK_TYPE_12M:
	cairo_move_to(cr,
		      (gdouble)(mascot->clktext_x+mascot->clksd_x+clk_width+3),
		      (gdouble)(mascot->clktext_y+mascot->clksd_y+clk_height));
	cairo_save(cr);
	cairo_scale (cr, CLOCK_AMPM_RATIO, CLOCK_AMPM_RATIO);

	// digital clock
	if(mascot->clk_pm){
	  cairo_show_text(cr,"PM");
	}
	else{
	  cairo_show_text(cr,"AM");
	}
	cairo_restore(cr);
      
	break;
      }
    }
    
    
    // Clock digit foreground
    cairo_set_source_rgba (cr, 
			   (gdouble)mascot->colclk->red/0xFFFF,
			   (gdouble)mascot->colclk->green/0xFFFF,
			   (gdouble)mascot->colclk->blue/0xFFFF,
			   (gdouble)mascot->alpclk/100.0); /* transparent */
#ifdef USE_PANGOCAIRO
    cairo_move_to(cr,
		  (gdouble)(mascot->clktext_x),
		  (gdouble)(mascot->clktext_y));
    pango_cairo_show_layout(cr,pango_text);
#else
    cairo_move_to(cr,
		  (gdouble)(mascot->clktext_x),
		  (gdouble)(clk_height+mascot->clktext_y));
    cairo_show_text(cr,mascot->digit);
#endif
    switch(mascot->clktype){
    case CLOCK_TYPE_12S:
    case CLOCK_TYPE_12M:
      cairo_move_to(cr,
		    (gdouble)(mascot->clktext_x+clk_width+3),
		    (gdouble)(mascot->clktext_y+clk_height));
      cairo_save(cr);
      cairo_scale (cr, CLOCK_AMPM_RATIO, CLOCK_AMPM_RATIO);

      // digital clock
      if(mascot->clk_pm){
	cairo_show_text(cr,"PM");
      }
      else{
	cairo_show_text(cr,"AM");
      }
      cairo_restore(cr);
      
      break;
    }
  }
  /*
  else{
    pango_text=gtk_widget_create_pango_layout(mascot->clock_main,
					      mascot->digit);
   
    
    
    if(mascot->flag_clksd){
      gdk_draw_layout(pixmap_main,
		      mascot->gc_mainsd,
		      mascot->clktext_x*((gdouble)(mascot->magnify)/100)+1,
		      1,
		      pango_text);
    }
    gdk_draw_layout(pixmap_main,
		    mascot->gc_main,
		    mascot->clktext_x*((gdouble)(mascot->magnify)/100),
		    0,
		    pango_text);
    
    switch(mascot->clktype){
    case CLOCK_TYPE_12S:
    case CLOCK_TYPE_12M:
      pango_layout_get_pixel_size(pango_text,&clk_width,&clk_height);
      
      // digital clock
      if(mascot->flag_clksd){
	if(mascot->clk_pm){
	  gdk_draw_arc(pixmap_main,mascot->gc_mainsd,
		       TRUE,
		       mascot->clktext_x*((gdouble)(mascot->magnify)/100)+1+clk_width,
		       mascot->clktext_y*((gdouble)(mascot->magnify)/100)+1+clk_height*2/3-as,
		       clk_height/3,clk_height/3,
		       0,(360*64));
	}
	else{
	  gdk_draw_arc(pixmap_main,mascot->gc_mainsd,
		       TRUE,
		       mascot->clktext_x*((gdouble)(mascot->magnify)/100)+1+clk_width,
		       mascot->clktext_y*((gdouble)(mascot->magnify)/100)+1-as,
		       clk_height/3,clk_height/3,
		       0,(360*64));
	}
      }
      
      if(mascot->clk_pm){
	gdk_draw_arc(pixmap_main,mascot->gc_main,
		     TRUE,
		     mascot->clktext_x*((gdouble)(mascot->magnify)/100)+clk_width,
		     mascot->clktext_y*((gdouble)(mascot->magnify)/100)+clk_height*2/3-as,
		     clk_height/3,clk_height/3,
		     0,(360*64));
      }
      else{
	gdk_draw_arc(pixmap_main,mascot->gc_main,
		     TRUE,
		     mascot->clktext_x*((gdouble)(mascot->magnify)/100)+clk_width,
		     mascot->clktext_y*((gdouble)(mascot->magnify)/100)-as,
		     clk_height/3,clk_height/3,
		     0,(360*64));
      }
      
      break;
    }
  }
  */

  cairo_destroy(cr);

  pixbuf_main = gdk_pixbuf_get_from_surface(surface, 0, 0, mascot->width, mascot->height);
  cairo_surface_destroy (surface);

  region = get_cairo_region_from_pixbuf(pixbuf_main, mascot->width, mascot->height, 0, 0);

  if(flag_img_cairo_go){
    
    gdk_window_input_shape_combine_region( gtk_widget_get_window(mascot->win_main), 
					   region, 0, 0 ); 
    gdk_window_set_cursor(gtk_widget_get_window(mascot->win_main),
			  mascot->cursor.normal);
  }
  else{
    gdk_window_shape_combine_region(gtk_widget_get_window(mascot->win_main), 
				    region, 0, 0 ); 
  }

  cairo_region_destroy(region);
  
  gtk_widget_queue_draw(mascot->dw_main);

#else  //////////////  GTK2  //////////////
  if (pixmap_main) {
    g_object_unref(G_OBJECT(pixmap_main));
  } 
  
  pixmap_main = gdk_pixmap_new(gtk_widget_get_window(mascot->win_main),
			       mascot->width,
			       mascot->height,
			       -1);

  gdk_draw_drawable(pixmap_main,
		    mascot->gc_main,
		    mascot->sprites[mascot->frame_pix[mascot->anime_ptn][mascot->anime_frm]].pixmap,
		    0, 0,
		    0, 0,
		    mascot->width, mascot->height);
  
  //if(flag_img_cairo_go){
  {
#ifdef USE_PANGOCAIRO
    pango_text=gtk_widget_create_pango_layout(mascot->clock_main,
					      mascot->digit);
    pango_layout_get_pixel_size(pango_text,&clk_width,&clk_height);
#endif
    cr = gdk_cairo_create(pixmap_main);
   
    cairo_select_font_face (cr, 
			    mascot->fontclk_pc.family,
			    mascot->fontclk_pc.slant,
			    mascot->fontclk_pc.weight);

    cairo_set_font_size (cr, mascot->fontclk_pc.pointsize*96.0/72.0);
#ifndef USE_PANGOCAIRO
    cairo_text_extents (cr, mascot->digit, &extents);
    clk_width=(gint)(extents.x_advance);
    clk_height=(gint)(-extents.y_bearing);
#endif

    cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
    
    if(mascot->flag_clksd){
      cairo_set_source_rgba (cr, 
			     (gdouble)mascot->colclksd->red/0xFFFF,
			     (gdouble)mascot->colclksd->green/0xFFFF,
			     (gdouble)mascot->colclksd->blue/0xFFFF,
			     (gdouble)mascot->alpclksd/100.0); /* transparent */
#ifdef USE_PANGOCAIRO
      cairo_move_to(cr,
		    (gdouble)(mascot->clktext_x+mascot->clksd_x),
		    (gdouble)(mascot->clktext_y+mascot->clksd_y));
      pango_cairo_show_layout(cr,pango_text);
#else
      cairo_move_to(cr,
		    (gdouble)(mascot->clktext_x+mascot->clksd_x),
		    (gdouble)(clk_height+mascot->clktext_y+mascot->clksd_y));
      cairo_show_text(cr,mascot->digit);
#endif
      switch(mascot->clktype){
      case CLOCK_TYPE_12S:
      case CLOCK_TYPE_12M:
	cairo_move_to(cr,
		      (gdouble)(mascot->clktext_x+mascot->clksd_x+clk_width+3),
		      (gdouble)(mascot->clktext_y+mascot->clksd_y+clk_height));
	cairo_save(cr);
	cairo_scale (cr, CLOCK_AMPM_RATIO, CLOCK_AMPM_RATIO);

	// digital clock
	if(mascot->clk_pm){
	  cairo_show_text(cr,"PM");
	}
	else{
	  cairo_show_text(cr,"AM");
	}
	cairo_restore(cr);
      
	break;
      }
    }
    
    
    // Clock digit foreground
    cairo_set_source_rgba (cr, 
			   (gdouble)mascot->colclk->red/0xFFFF,
			   (gdouble)mascot->colclk->green/0xFFFF,
			   (gdouble)mascot->colclk->blue/0xFFFF,
			   (gdouble)mascot->alpclk/100.0); /* transparent */
#ifdef USE_PANGOCAIRO
    cairo_move_to(cr,
		  (gdouble)(mascot->clktext_x),
		  (gdouble)(mascot->clktext_y));
    pango_cairo_show_layout(cr,pango_text);
#else
    cairo_move_to(cr,
		  (gdouble)(mascot->clktext_x),
		  (gdouble)(clk_height+mascot->clktext_y));
    cairo_show_text(cr,mascot->digit);
#endif
    switch(mascot->clktype){
    case CLOCK_TYPE_12S:
    case CLOCK_TYPE_12M:
      cairo_move_to(cr,
		    (gdouble)(mascot->clktext_x+clk_width+3),
		    (gdouble)(mascot->clktext_y+clk_height));
      cairo_save(cr);
      cairo_scale (cr, CLOCK_AMPM_RATIO, CLOCK_AMPM_RATIO);

      // digital clock
      if(mascot->clk_pm){
	cairo_show_text(cr,"PM");
      }
      else{
	cairo_show_text(cr,"AM");
      }
      cairo_restore(cr);
      
      break;
    }
  }
  

  {
    GtkAllocation *allocation=g_new(GtkAllocation, 1);
    GtkStyle *style=gtk_widget_get_style(mascot->dw_main);
    gtk_widget_get_allocation(mascot->dw_main,allocation);
    
    gdk_draw_drawable(gtk_widget_get_window(mascot->dw_main),
		      style->fg_gc[gtk_widget_get_state(mascot->dw_main)],
		      pixmap_main,
		      0,0,0,0,
		      allocation->width,
		      allocation->height);
    g_free(allocation);
  }

  if(flag_img_cairo_go){
    gdk_window_input_shape_combine_mask( gtk_widget_get_window(mascot->win_main), 
					 mascot->sprites[mascot->frame_pix[mascot->anime_ptn][mascot->anime_frm]].mask,
					 0, 0 ); 
    gdk_window_set_cursor(gtk_widget_get_window(mascot->win_main),
			  mascot->cursor.normal);
  }
  else{
    gdk_window_shape_combine_mask( gtk_widget_get_window(mascot->win_main), 
				   mascot->sprites[mascot->frame_pix[mascot->anime_ptn][mascot->anime_frm]].mask,
				   0, 0 ); 
  }
  
  {
    GtkAllocation *allocation=g_new(GtkAllocation, 1);
    GtkStyle *style=gtk_widget_get_style(mascot->dw_main);
    gtk_widget_get_allocation(mascot->dw_main,allocation);
    
    gdk_draw_drawable(gtk_widget_get_window(mascot->dw_main),
		      style->fg_gc[gtk_widget_get_state(mascot->dw_main)],
		      pixmap_main,
		      0,0,0,0,
		      allocation->width,
		      allocation->height);
    g_free(allocation);
  }

#endif // USE_GTK3
  

  
#ifdef USE_PANGOCAIRO
  g_object_unref(G_OBJECT(pango_text));
#endif
  if(flag_img_cairo_go) cairo_destroy(cr);

  //while (my_main_iteration(FALSE));

  //gdk_flush();

  return(0);
}


#ifndef USE_GTK3
void ReInitGC(typMascot *mascot)
{
  if(mascot->gc_main){
    gdk_gc_unref(mascot->gc_main);
    gdk_gc_unref(mascot->gc_mainsd);
  }
  mascot->gc_main = MPCreatePen(pixmap_main, mascot->colclk);
  mascot->gc_mainsd = MPCreatePen(pixmap_main, mascot->colclksd);
  
  if(mascot->gc_bal){
    gdk_gc_unref(mascot->gc_bal);
    gdk_gc_unref(mascot->gc_balbg);
    gdk_gc_unref(mascot->gc_balbd);
  }
  mascot->gc_bal = MPCreatePen(pixmap_bal, mascot->colbal);
  mascot->gc_balbg = MPCreatePen(pixmap_bal, mascot->colbalbg);
  mascot->gc_balbd = MPCreatePen(pixmap_bal, mascot->colbalbd);
  
  if(mascot->gc_clk){
    gdk_gc_unref(mascot->gc_clk);
    
    gdk_gc_unref(mascot->gc_clkbg);
    gdk_gc_unref(mascot->gc_clkbd);
    gdk_gc_unref(mascot->gc_clksd);
  }
  mascot->gc_clk = MPCreatePen(pixmap_clk, mascot->colclk);
  mascot->gc_clkbg = MPCreatePen(pixmap_clk, mascot->colclkbg);
  mascot->gc_clkbd = MPCreatePen(pixmap_clk, mascot->colclkbd);
  mascot->gc_clksd = MPCreatePen(pixmap_clk, mascot->colclksd);
}
#endif

/*
void create_default_font_selection_dialog(typMascot *mascot, gint bal_clk)
{
  GtkWidget *fdialog;
  GtkWidget *button;
  GtkWidget *label;
  confDefFont *cdata;
  gchar *tmp_label;
  ConfigFile *cfgfile;

  tmp_label=g_malloc0(sizeof(gchar)*256);

  fdialog = gtk_dialog_new();
  gtk_container_set_border_width(GTK_CONTAINER(fdialog),5);

  switch(bal_clk){
  case INIT_DEF_FONT_CLK:
    gtk_window_set_title(GTK_WINDOW(fdialog),
			 _("Cannot Load Default Clock Font"));
    label=gtkut_label_new(_("Cannot Load Default Clock Font"));
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(fdialog))),
		       label,TRUE,TRUE,0);
    sprintf(tmp_label,"\"%s\"",mascot->deffontname_clk);
    label=gtkut_label_new(tmp_label);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(fdialog))),
		       label,TRUE,TRUE,0);
    break;
  case INIT_DEF_FONT_BAL:
    gtk_window_set_title(GTK_WINDOW(fdialog),
			 _("Cannot Load Default Balloon Font"));
    label=gtkut_label_new(_("Cannot Load Default Balloon Font"));
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(fdialog))),
		       label,TRUE,TRUE,0);
    sprintf(tmp_label,"\"%s\"",mascot->deffontname_bal);
    label=gtkut_label_new(tmp_label);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(fdialog))),
		       label,TRUE,TRUE,0);
    break;
  }

  label=gtkut_label_new(_("Please Change..."));
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(fdialog))),
		     label,TRUE,TRUE,0);

#ifdef USE_GTK3
  button=gtkut_button_new_from_icon_name(_("OK"),"go-jump");
#else
  button=gtkut_button_new_from_stock(_("OK"),GTK_STOCK_OK);
#endif
  gtk_dialog_add_action_widget(GTK_DIALOG(fdialog),button,GTK_RESPONSE_OK);
  my_signal_connect(button,"clicked",close_child_dialog,fdialog);

  gtk_widget_show_all(fdialog);
  gtk_main();


  cdata=g_malloc0(sizeof(confDefFont));

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  while (my_main_iteration(FALSE));

  
  fdialog = gtk_font_selection_dialog_new(_("Default Font Selection"));

  cdata->fsd=GTK_FONT_SELECTION_DIALOG(fdialog);

  cdata->fn=NULL;

  switch(bal_clk){
  case INIT_DEF_FONT_CLK:
    gtk_font_selection_dialog_set_font_name(cdata->fsd,
					    mascot->deffontname_clk);
    break;
  case INIT_DEF_FONT_BAL:
    gtk_font_selection_dialog_set_font_name(cdata->fsd,
					    mascot->deffontname_bal);
    break;
  }

  my_signal_connect(GTK_FONT_SELECTION_DIALOG(fdialog)->ok_button,
		    "clicked", ChangeDefFontname, (gpointer)cdata);
    
  my_signal_connect(fdialog,"destroy",close_child_dialog, fdialog);
  
  my_signal_connect(GTK_FONT_SELECTION_DIALOG(fdialog)->cancel_button,
		    "clicked", close_child_dialog,fdialog);
  
  gtk_widget_show(fdialog);

  gtk_main();


  if(cdata->fn){
    switch(bal_clk){
    case INIT_DEF_FONT_CLK:
      mascot->deffontname_clk=cdata->fn;
      cfgfile = xmms_cfg_open_file(mascot->rcfile);
      if (!cfgfile)  cfgfile = xmms_cfg_new();
      xmms_cfg_write_string(cfgfile, "General",
			    "font_clock",mascot->deffontname_clk);
      xmms_cfg_write_file(cfgfile, mascot->rcfile);
      xmms_cfg_free(cfgfile);
      break;
    case INIT_DEF_FONT_BAL:
      mascot->deffontname_bal=cdata->fn;
      cfgfile = xmms_cfg_open_file(mascot->rcfile);
      if (!cfgfile)  cfgfile = xmms_cfg_new();
      xmms_cfg_write_string(cfgfile, "General",
			    "font_balloon",mascot->deffontname_bal);
      xmms_cfg_write_file(cfgfile, mascot->rcfile);
      xmms_cfg_free(cfgfile);
      break;
    }
  }

  g_free(cdata);
  g_free(tmp_label);
}
  

// フォント選択ダイアログからのフォントの変更
void ChangeDefFontname(GtkWidget *w, gpointer gdata)
{ 
  confDefFont *cdata;
  cdata=(confDefFont *)gdata;

  cdata->fn=gtk_font_selection_dialog_get_font_name(cdata->fsd);

  gtk_widget_destroy(GTK_WIDGET(cdata->fsd));
  gtk_main_quit();
}
*/

static void close_child_dialog(GtkWidget *w, GtkWidget *dialog)
{
  //gdk_pointer_ungrab(GDK_CURRENT_TIME);

  gtk_widget_destroy(dialog);
  gtk_main_quit();
}

void screen_changed(GtkWidget *widget, GdkScreen *old_screen, 
		    gpointer userdata, gboolean cairo_flag)
{
  GdkScreen *screen = gtk_widget_get_screen(widget);
#ifdef USE_GTK3
  GdkVisual *visual = gdk_screen_get_rgba_visual(screen);

  if(!visual){
    visual = gdk_screen_get_system_visual(screen);
    supports_alpha = FALSE;
  }
  else{
    supports_alpha = TRUE;
  }

  gtk_widget_set_visual(widget, visual);
#else
  /* To check if the display supports alpha channels, get the colormap */
  GdkColormap *colormap = gdk_screen_get_rgba_colormap(screen);
  
  if(cairo_flag){
    if (!colormap)
      {
	//printf("Your screen does not support alpha channels!\n");
	colormap = gdk_screen_get_rgb_colormap(screen);
	supports_alpha = FALSE;
      }
    else
      {
	//printf("Your screen supports alpha channels!\n");
	supports_alpha = TRUE;
      }
      //fflush(stdout);
  }
  else{
    colormap = gdk_screen_get_rgb_colormap(screen);
    //printf("Forced screen does not supports alpha channels!\n");
      supports_alpha = FALSE;
  }
  
  /* Now we have a colormap appropriate for the screen, use it */
  gtk_widget_set_colormap(widget, colormap);
#endif
}

void InitComposite(typMascot *mascot){
  map_main(mascot, FALSE);
#ifdef USE_BIFF
  map_biff(mascot, FALSE);
#endif
  //#endif
  map_clock(mascot, FALSE);
  map_balloon(mascot, FALSE);

#ifdef USE_GTK3
  if(gdk_screen_is_composited(gtk_widget_get_screen(mascot->win_main)))
#else
 if(gtk_widget_is_composited(mascot->win_main))
#endif    
    mascot->flag_composite=COMPOSITE_TRUE;
  else
    mascot->flag_composite=COMPOSITE_FALSE;
  mascot->force_composite=FALSE;

#ifndef FG_DRAW
  //unrealize to change colormap
  gtk_window_resize(GTK_WINDOW(mascot->win_main),1,1);
  gtk_widget_unrealize(mascot->win_main);
  if((mascot->flag_img_cairo)&&(mascot->flag_composite==COMPOSITE_TRUE)){
    screen_changed(mascot->win_main, NULL, NULL, TRUE);
    flag_img_cairo_go=TRUE;
  }
  else if((mascot->force_composite)&&(mascot->flag_composite==COMPOSITE_UNKNOWN)){
    screen_changed(mascot->win_main, NULL, NULL, TRUE);
    flag_img_cairo_go=TRUE;
  }
  else{
    screen_changed(mascot->win_main,  NULL, NULL, FALSE);
    flag_img_cairo_go=FALSE;
  }
  //re-realize
  gtk_widget_realize(mascot->win_main);


  //unrealize to change colormap

#ifdef USE_BIFF
  gtk_widget_unrealize(mascot->biff_pix);
  if(flag_img_cairo_go){
    screen_changed(mascot->biff_pix, NULL, NULL, TRUE);
  }
  else{
    screen_changed(mascot->biff_pix,  NULL, NULL, FALSE);
  }
  //re-realize
  gtk_widget_realize(mascot->biff_pix);
#endif
#endif  //#ifndef USE_WIN32

  //unrealize to change colormap
  gtk_widget_unrealize(mascot->clock_main);
  if((mascot->flag_clk_cairo)&&(mascot->flag_composite==COMPOSITE_TRUE)){
    screen_changed(mascot->clock_main, NULL, NULL, TRUE);
  }
  else if((mascot->force_composite)&&(mascot->flag_composite==COMPOSITE_UNKNOWN)){
    screen_changed(mascot->clock_main, NULL, NULL, TRUE);
  }
  else{
    screen_changed(mascot->clock_main,  NULL, NULL, FALSE);
  }
  //re-realize
  gtk_widget_realize(mascot->clock_main);
  //clear buffer
#ifdef USE_GTK3
  if(pixbuf_clk!=NULL) g_object_unref(G_OBJECT(pixbuf_clk));
  pixbuf_clk=NULL;
#else  
  if(pixmap_clk!=NULL) g_object_unref(G_OBJECT(pixmap_clk));
  pixmap_clk=NULL;
#endif
  // call configure to make pixmaps
  //dw_configure_clk(mascot->dw_clock, "configure_event",(gpointer)mascot);

  //unrealize to change colormap
  gtk_widget_unrealize(mascot->balloon_main);
  if((mascot->flag_bal_cairo)&&(mascot->flag_composite==COMPOSITE_TRUE)){
    screen_changed(mascot->balloon_main, NULL, NULL, TRUE);
  }
  else if((mascot->force_composite)&&(mascot->flag_composite==COMPOSITE_UNKNOWN)){
    screen_changed(mascot->balloon_main, NULL, NULL, TRUE);
  }
  else{
    screen_changed(mascot->balloon_main,  NULL, NULL, FALSE);
  }
  //re-realize
  gtk_widget_realize(mascot->balloon_main);
  //clear buffer
#ifdef USE_GTK3  
  if(pixbuf_bal!=NULL) g_object_unref(G_OBJECT(pixbuf_bal));
  pixbuf_bal=NULL;
#else
  if(pixmap_bal!=NULL) g_object_unref(G_OBJECT(pixmap_bal));
  pixmap_bal=NULL;
#endif
  // call configure to make pixmaps
  //dw_configure_bal(mascot->balloon_main, "configure_event",(gpointer)mascot);

  //reset input shape mask
#ifndef USE_GTK3  
  gdk_window_input_shape_combine_mask(gtk_widget_get_window(mascot->win_main),
				      NULL,0, 0 );
#endif
  gdk_window_set_cursor(gtk_widget_get_window(mascot->win_main),
			mascot->cursor.normal);
#ifdef USE_BIFF
#ifndef USE_GTK3  
  gdk_window_input_shape_combine_mask(gtk_widget_get_window(mascot->biff_pix), NULL,0, 0 );
#endif  
  gdk_window_set_cursor(gtk_widget_get_window(mascot->biff_pix),mascot->cursor.biff);
#endif

  //reset shape mask
#ifndef USE_GTK3  
#ifndef FG_DRAW
  gdk_window_shape_combine_mask(gtk_widget_get_window(mascot->win_main),
				NULL,0, 0 );
  
#ifdef USE_BIFF
  gdk_window_shape_combine_mask(gtk_widget_get_window(mascot->biff_pix), NULL,0, 0 ); 
#endif
#endif

  gdk_window_shape_combine_mask(gtk_widget_get_window(mascot->clock_main), NULL,0, 0 ); 
  gdk_window_shape_combine_mask(gtk_widget_get_window(mascot->balloon_main), NULL,0, 0 ); 
#ifdef FG_DRAW
  gdk_window_shape_combine_mask(gtk_widget_get_window(mascot->clock_fg), NULL,0, 0 ); 
  gdk_window_shape_combine_mask(gtk_widget_get_window(mascot->balloon_fg), NULL,0, 0 );
#endif
#endif
  
  if((flag_img_cairo_go)&&(mascot->sdw_flag)){
    if(mascot->sdw_x<0){
      mascot->sdw_x_int=(gint)(-mascot->sdw_x+1);
    }
    else{
      mascot->sdw_x_int=(gint)(mascot->sdw_x+1);
    }
    mascot->sdw_y_int=(gint)(mascot->sdw_y+1);
  }
  else{
    mascot->sdw_x_int=0;
    mascot->sdw_y_int=0;
  }
}
  

#ifdef USE_GTK3
cairo_region_t * get_cairo_region_from_pixbuf(GdkPixbuf *pixbuf, gint w, gint h, gint x0, gint y0){
  cairo_surface_t *surface;
  cairo_t *cr;
  cairo_region_t *region;

  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
  cr = cairo_create(surface);
  
  cairo_set_source_rgba(cr, 0, 0, 0, 0);
  cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
  cairo_rectangle(cr, 0, 0, w, h);
  cairo_fill(cr);
  cairo_paint(cr);
  
  // Shadow
  gdk_cairo_set_source_pixbuf(cr, pixbuf, x0, y0);
  cairo_paint(cr);
  cairo_destroy(cr);
  
  region = gdk_cairo_region_create_from_surface(surface);

  cairo_surface_destroy(surface);

  return(region);
}
#endif


#ifndef USE_GTK3
GdkBitmap * make_mask_from_surface(cairo_surface_t *surface){
  unsigned char *data, *p_ret;
  gint stride, width, height, sz, i, j, pos;
  GdkPixbuf *pixbuf_mask;
  GdkBitmap *mask_ret;
    
  data=cairo_image_surface_get_data(surface);
  stride=cairo_image_surface_get_stride(surface);
  width=cairo_image_surface_get_width(surface);
  height=cairo_image_surface_get_height(surface);
  sz=stride/width;

  pixbuf_mask=gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width, height);
  p_ret = gdk_pixbuf_get_pixels(pixbuf_mask);

  for(i=0; i<height; i++){
    for(j=0; j<width; j++){
      pos=(width*i+j)*sz;
      if(data[pos]>50){
	p_ret[pos]  =0xFF;
	p_ret[pos+1]=0xFF;
	p_ret[pos+2]=0xFF;
	p_ret[pos+3]=0xFF;
      }
      else{
	p_ret[pos]  =0x00;
	p_ret[pos+1]=0x00;
	p_ret[pos+2]=0x00;
	p_ret[pos+3]=0x00;
      }
    }
  }
  
  cairo_surface_destroy(surface);
  
  mask_ret = gdk_pixmap_new(NULL,
			    width, height,1); // Depth =1 (Bitmap)
  
  gdk_pixbuf_render_threshold_alpha(pixbuf_mask, mask_ret,
				    0, 0, 0, 0,
				    width, height, 10);
    
  g_object_unref(G_OBJECT(pixbuf_mask));

  return(mask_ret);
}
#endif

