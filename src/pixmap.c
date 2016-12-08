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

#ifdef USE_BIFF
#include "def_biff.xpm"
#include "def_biff_r.xpm"
#endif



// *** GLOBAL ARGUMENT ***
extern GtkWidget *win_main, *clock_main, *balloon_main;
#ifdef USE_WIN32
extern GtkWidget *win_sdw;
#endif
#ifdef USE_BIFF
extern GtkWidget *biff_pix;
#endif
#ifdef USE_WIN32
extern GtkWidget *clock_fg, *balloon_fg;
#endif
extern GtkWidget *popup_menu; 
extern gint window_x, window_y;

extern GdkDrawable *pixmap_main[2], *pixmap_clk[2], *pixmap_bal[2];
#ifdef USE_WIN32
extern GdkDrawable *pixmap_sdw[2];
#endif

extern GdkGC *MPCreatePen();

extern gchar* to_utf8();

#ifdef USE_CAIRO
extern gint dw_init_main();
#ifdef USE_BIFF
extern gint dw_configure_biff_pix();
#endif
extern gint dw_configure_bal();
extern gint dw_configure_clk();
#endif
extern void raise_all();

extern void make_balloon();

#ifdef __GTK_STATUS_ICON_H__
extern void trayicon_set_tooltip();
#endif

#ifdef __GTK_STOCK_H__
extern GtkWidget* gtkut_button_new_from_stock();
#endif

extern void my_signal_connect();
extern gboolean my_main_iteration();

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
gboolean TestLoadPixmaps();
void LoadPixmaps();
#ifdef USE_BIFF
void LoadBiffPixmap();
#endif
gint DrawMascot0();
gint DrawMascot();
gint DrawMascotTemp();
gint DrawMascotWithDigit();
void ReInitGC();

void create_default_font_selection_dialog();
void ChangeDefFontname();
static void close_child_dialog();

#ifdef USE_CAIRO
void screen_changed();
gboolean supports_alpha;
#endif
void InitComposite();

gboolean flag_img_cairo_go=FALSE;


gboolean TestLoadPixmaps(typMascot *mascot, gchar *filename, gint i_pix)
{
  GdkPixbuf *pixbuf = NULL,*pixbuf2 = NULL;
  gint w=0,h=0;
  gint ipstyle;
  gchar *tmp_open;
#ifdef USE_CAIRO
  cairo_t *cr;
#endif

  
  tmp_open=to_utf8(filename);
#ifdef USE_GTK2
  pixbuf = gdk_pixbuf_new_from_file(tmp_open, NULL);
#else
  pixbuf = gdk_pixbuf_new_from_file(tmp_open);
#endif
  g_free(tmp_open);
  if(!pixbuf) return(FALSE);
  
  w = gdk_pixbuf_get_width(pixbuf)*((gfloat)(mascot->magnify)/100);
  h = gdk_pixbuf_get_height(pixbuf)*((gfloat)(mascot->magnify)/100);

#ifdef USE_WIN32
  if(mascot->move==MOVE_FOCUS){
    if(mascot->yoff>0){
      mascot->sdw_height
	=mascot->yoff*(gfloat)(mascot->magnify)/100+mascot->sdw_y;
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
    
  if(mascot->sprites[i_pix].pixmap) {
#ifdef USE_GTK2
    g_object_unref(G_OBJECT(mascot->sprites[i_pix].pixmap));
#else
    gdk_pixmap_unref(mascot->sprites[i_pix].pixmap);
#endif
    mascot->sprites[i_pix].pixmap=NULL;
  }
  if(mascot->sprites[i_pix].mask) {
#ifdef USE_GTK2
    g_object_unref(G_OBJECT(mascot->sprites[i_pix].mask));
#else
    gdk_pixmap_unref(mascot->sprites[i_pix].mask);
#endif
    mascot->sprites[i_pix].mask=NULL;
  }

  pixbuf2=gdk_pixbuf_scale_simple(pixbuf,w,h,ipstyle);

#ifdef USE_CAIRO


  if(flag_img_cairo_go){
    GdkBitmap *mask=NULL;
    
    if(mascot->sdw_flag){
      w= w + mascot->sdw_x_int;
      h= h + mascot->sdw_y_int;
      
      gdk_pixbuf_render_pixmap_and_mask(pixbuf, NULL, &mask, 0x01);
    }

    gdk_pixbuf_render_pixmap_and_mask(pixbuf2, NULL,
				      &mascot->sprites[i_pix].mask, 0xc0);
    
    mascot->sprites[i_pix].pixmap = gdk_pixmap_new(win_main->window,
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
      cairo_scale (cr, (gfloat)(mascot->magnify)/100, 
		   (gfloat)(mascot->magnify)/100);
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
	cairo_move_to(cr,0,0);
	cairo_rectangle(cr, 0, 0, w, 
		      h-mascot->yoff*(gfloat)(mascot->magnify)/100-mascot->sdw_y_int);
	cairo_fill(cr);
	cairo_restore(cr);
      }
    }

    // Mascot
    cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
    cairo_move_to(cr,0,0);
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
#ifdef USE_WIN32
    if(mascot->sdw_flag){
      
      if(mascot->move==MOVE_FOCUS){
	if(mascot->sdw_height>0){
	  
	  mascot->sprites[i_pix].pixmap_sdw
	    =gdk_pixmap_new(win_sdw->window,
			    w, mascot->sdw_height,
			    -1);
	  
	  mascot->sprites[i_pix].mask_sdw
	    = gdk_pixmap_new(win_sdw->window,
			     w, mascot->sdw_height,
			     1); // Depth =1 (Bitmap)
	  
	  
	  gdk_draw_drawable(mascot->sprites[i_pix].pixmap_sdw,
			    win_sdw->style->fg_gc[GTK_WIDGET_STATE(win_sdw)],
			    mascot->sprites[i_pix].mask,
			    0, h-mascot->sdw_height,
			    0,0,
			    w,mascot->sdw_height);
	  
	  gdk_draw_drawable(mascot->sprites[i_pix].mask_sdw,
			    win_sdw->style->fg_gc[GTK_WIDGET_STATE(win_sdw)],
			    mascot->sprites[i_pix].mask,
			    0, h-mascot->sdw_height,
			    0,0,
			    w,mascot->sdw_height);
	  
	}
      }
      else{
	mascot->sprites[i_pix].pixmap_sdw
	  =gdk_pixmap_new(win_sdw->window,w,h,-1);
	
	mascot->sprites[i_pix].mask_sdw
	  = gdk_pixmap_new(win_sdw->window, w,h,1); // Depth =1 (Bitmap)
	
	gdk_draw_drawable(mascot->sprites[i_pix].pixmap_sdw,
			  win_sdw->style->fg_gc[GTK_WIDGET_STATE(win_sdw)],
			  mascot->sprites[i_pix].mask,
			  0,0,
			  0,0,
			  w,h);
	
	gdk_draw_drawable(mascot->sprites[i_pix].mask_sdw,
			  win_sdw->style->fg_gc[GTK_WIDGET_STATE(win_sdw)],
			  mascot->sprites[i_pix].mask,
			  0,0,
			  0,0,
			  w,h);
      }
    }
#endif   
  }
#else
  gdk_pixbuf_render_pixmap_and_mask(pixbuf2, &mascot->sprites[i_pix].pixmap,
				    &mascot->sprites[i_pix].mask, 0xc0);
#endif
  
#ifdef USE_GTK2
  g_object_unref(G_OBJECT(pixbuf));
  g_object_unref(G_OBJECT(pixbuf2));
#else
  gdk_pixbuf_unref(pixbuf);
  gdk_pixbuf_unref(pixbuf2);
#endif

  DrawMascotTemp(mascot,i_pix);

  return(TRUE);
  
}



void LoadPixmaps(GtkWidget *widget, //GtkWidget *draw, 
		 typMascot *mascot, typSprite *sprites){
  int i=0;
  //GdkImlibImage* im = NULL;
  GdkPixbuf *pixbuf = NULL, *pixbuf2 = NULL;
  gint w=0,h=0;
  gint ipstyle;
#ifndef USE_GTK2
  gchar *fontset=NULL;
#endif
  gchar *tmp_open;
#ifdef USE_CAIRO
  cairo_t *cr;
#endif


#ifdef USE_WIN32
  GdkWinChangeAlpha(win_main->window, mascot->alpha_main);
  GdkWinChangeAlpha(win_sdw->window,  mascot->sdw_alpha);
#ifdef USE_BIFF
  GdkWinChangeAlpha(biff_pix->window, mascot->alpha_biff);
#endif
  if((mascot->flag_balfg)&&(mascot->alpha_bal!=100)){
    GdkWinChangeAlphaFG(balloon_fg->window, mascot->colbalbg);
  }
  GdkWinChangeAlpha(balloon_main->window, mascot->alpha_bal);
  if((mascot->flag_clkfg)&&(mascot->alpha_clk!=100)){
    GdkWinChangeAlphaFG(clock_fg->window, mascot->colclkbg);
  }
  GdkWinChangeAlpha(clock_main->window, mascot->alpha_clk);
#else  // for UNIX  GNOME 2.20
#if GTK_CHECK_VERSION(2,12,0) && !defined(USE_CAIRO)
  if(!flag_img_cairo_go){
    gdk_window_set_opacity(win_main->window,  
			   (gdouble)mascot->alpha_main/100);
#ifdef USE_BIFF
    gdk_window_set_opacity(biff_pix->window,  
			   (gdouble)mascot->alpha_biff/100);
#endif
  }
#endif
#endif


  while(sprites[i].filename){
    if (sprites[i].pixmap){
#ifdef USE_GTK2
      g_object_unref(G_OBJECT(sprites[i].pixmap));
#else
      gdk_pixmap_unref(sprites[i].pixmap);
#endif
      sprites[i].pixmap=NULL;
    }
    if (sprites[i].mask){
#ifdef USE_GTK2
      g_object_unref(G_OBJECT(sprites[i].mask));
#else
      gdk_pixmap_unref(sprites[i].mask);
#endif
      sprites[i].mask=NULL;
    }

#ifdef USE_WIN32
    if (sprites[i].pixmap_sdw){
      g_object_unref(G_OBJECT(sprites[i].pixmap_sdw));
      sprites[i].pixmap_sdw=NULL;
    }
    if (sprites[i].mask_sdw){
      g_object_unref(G_OBJECT(sprites[i].mask_sdw));
      sprites[i].mask_sdw=NULL;
    }
#endif

    //im = gdk_imlib_load_image(sprites[i].filename);
    tmp_open=to_utf8(sprites[i].filename);
#ifdef USE_GTK2
    pixbuf = gdk_pixbuf_new_from_file(tmp_open, NULL);
#else
    pixbuf = gdk_pixbuf_new_from_file(tmp_open);
#endif
    // if(im==NULL){
    if(pixbuf==NULL){
      g_print (_("Cannot Load Image %s\n"), tmp_open);
      g_free(tmp_open);
      exit(1);
    }
    g_free(tmp_open);

    w = gdk_pixbuf_get_width(pixbuf)*((gfloat)(mascot->magnify)/100);
    h = gdk_pixbuf_get_height(pixbuf)*((gfloat)(mascot->magnify)/100);

#ifdef USE_WIN32
    if(mascot->move==MOVE_FOCUS){
      if(mascot->yoff>0){
	mascot->sdw_height
	  =mascot->yoff*(gfloat)(mascot->magnify)/100+mascot->sdw_y;
      }
      else{
	mascot->sdw_height=0;
      }
    }
    else{
      mascot->sdw_height=h;
    }
#endif
    
    // Imlibを使用した場合
    //w = im->rgb_width*((gfloat)(mascot->magnify)/100);
    //h = im->rgb_height*((gfloat)(mascot->magnify)/100);
    //gdk_imlib_render(im, w, h);
    //sprites[i].pixmap = gdk_imlib_move_image(im);
    //sprites[i].mask = gdk_imlib_move_mask(im);

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

#ifdef USE_CAIRO
    if(flag_img_cairo_go){
      GdkBitmap *mask=NULL;

      if(mascot->sdw_flag){
	w= w + mascot->sdw_x_int;
	h= h + mascot->sdw_y_int;
	
	gdk_pixbuf_render_pixmap_and_mask(pixbuf, NULL, &mask, 0x01);
      }

      gdk_pixbuf_render_pixmap_and_mask(pixbuf2, NULL,
					&sprites[i].mask, 0xc0);

      sprites[i].pixmap = gdk_pixmap_new(win_main->window,
					 w,
					 h,
					-1);

      // Clear BG
      cr = gdk_cairo_create(sprites[i].pixmap);
      cairo_set_source_rgba (cr, 0, 0, 0, 0);
      cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
      cairo_paint (cr);


      if((mascot->sdw_flag)){
	// Shadow
	cairo_save(cr);
	cairo_scale (cr, (gfloat)(mascot->magnify)/100, 
		     (gfloat)(mascot->magnify)/100);
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
	  cairo_move_to(cr,0,0);
	  cairo_rectangle(cr, 0, 0, w, 
			  h-mascot->yoff*(gfloat)(mascot->magnify)/100-mascot->sdw_y_int);
	  cairo_fill(cr);
	  cairo_restore(cr);
	}
      }

      // Mascot
      cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
      cairo_move_to(cr,0,0);
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
      gdk_pixbuf_render_pixmap_and_mask(pixbuf2, &sprites[i].pixmap,
					&sprites[i].mask, 0xc0); 
#ifdef USE_WIN32
    if(mascot->sdw_flag){
      
      if(mascot->move==MOVE_FOCUS){
	if(mascot->sdw_height>0){
	  
	  mascot->sprites[i].pixmap_sdw
	    =gdk_pixmap_new(win_sdw->window,
			    w, mascot->sdw_height,
			    -1);
	  
	  mascot->sprites[i].mask_sdw
	    = gdk_pixmap_new(win_sdw->window,
			     w, mascot->sdw_height,
			     1); // Depth =1 (Bitmap)
	  
	  
	  gdk_draw_drawable(mascot->sprites[i].pixmap_sdw,
			    win_sdw->style->fg_gc[GTK_WIDGET_STATE(win_sdw)],
			    mascot->sprites[i].mask,
			    0, h-mascot->sdw_height,
			    0,0,
			    w,mascot->sdw_height);
	  
	  gdk_draw_drawable(mascot->sprites[i].mask_sdw,
			    win_sdw->style->fg_gc[GTK_WIDGET_STATE(win_sdw)],
			    mascot->sprites[i].mask,
			    0, h-mascot->sdw_height,
			    0,0,
			    w,mascot->sdw_height);
	  
	}
      }
      else{
	mascot->sprites[i].pixmap_sdw
	  =gdk_pixmap_new(win_sdw->window,w,h,-1);
	
	mascot->sprites[i].mask_sdw
	  = gdk_pixmap_new(win_sdw->window, w,h,1); // Depth =1 (Bitmap)
	
	gdk_draw_drawable(mascot->sprites[i].pixmap_sdw,
			  win_sdw->style->fg_gc[GTK_WIDGET_STATE(win_sdw)],
			  mascot->sprites[i].mask,
			  0,0,
			  0,0,
			  w,h);
	
	gdk_draw_drawable(mascot->sprites[i].mask_sdw,
			  win_sdw->style->fg_gc[GTK_WIDGET_STATE(win_sdw)],
			  mascot->sprites[i].mask,
			  0,0,
			  0,0,
			  w,h);
      }
    }
#endif   
  }
#else
    gdk_pixbuf_render_pixmap_and_mask(pixbuf2, &sprites[i].pixmap,
				      &sprites[i].mask, 0xc0);
#endif
    

#ifdef USE_GTK2
    g_object_unref(G_OBJECT(pixbuf));
    g_object_unref(G_OBJECT(pixbuf2));
#else
    gdk_pixbuf_unref(pixbuf);
    gdk_pixbuf_unref(pixbuf2);
#endif
    i++;
  }

  mascot->width=w;
  mascot->height=h;

  DrawMascot0(mascot);
  dw_init_main(win_main, "configure_event",(gpointer)mascot);

#ifdef USE_GTK2
  gtk_window_resize (GTK_WINDOW(widget), w, h);
#else
  gdk_window_resize (widget->window, w, h);
#endif

#ifdef USE_WIN32
  if((mascot->sdw_flag)&&(mascot->sdw_height>0)){
    gtk_window_resize (GTK_WINDOW(win_sdw), w, mascot->sdw_height);
  }
#endif
  
  // フォントのロード
#ifdef USE_GTK2
  if (mascot->fontclk!=NULL){
    pango_font_description_free(mascot->fontclk);
    mascot->fontclk=NULL;
  }
#else
  mascot->fontclk=NULL;
#endif

  if(mascot->fontname_clk){
#ifdef USE_GTK2
    mascot->fontclk=pango_font_description_from_string(mascot->fontname_clk);
#else
    fontset = g_strdup(mascot->fontname_clk);
    mascot->fontclk=gdk_fontset_load(fontset);
#endif
  }
  if(mascot->fontclk==NULL){
#ifdef USE_GTK2
    mascot->fontclk=pango_font_description_from_string(mascot->deffontname_clk);
#else
    fontset = g_strdup(mascot->deffontname_clk);
    mascot->fontclk=gdk_fontset_load(fontset);
#endif
    while(mascot->fontclk==NULL){
      create_default_font_selection_dialog(mascot,INIT_DEF_FONT_CLK);
#ifdef USE_GTK2
      mascot->fontclk=pango_font_description_from_string(mascot->deffontname_clk);
#else
      fontset = g_strdup(mascot->deffontname_clk);
      mascot->fontclk=gdk_fontset_load(fontset);
#endif
    }
    mascot->fontname_clk=mascot->deffontname_clk;
  }

#ifdef USE_CAIRO
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
#endif


#ifdef USE_GTK2
  if (mascot->fontbal!=NULL){
    pango_font_description_free(mascot->fontbal);
    mascot->fontbal=NULL;
  }
#else
  mascot->fontbal=NULL;
#endif
  
  if(mascot->fontname_bal){
#ifdef USE_GTK2
    mascot->fontbal=pango_font_description_from_string(mascot->fontname_bal);
#else
    fontset = g_strdup(mascot->fontname_bal);
    mascot->fontbal=gdk_fontset_load(fontset);
#endif
  }
  if(mascot->fontbal==NULL){
#ifdef USE_GTK2
    mascot->fontbal=pango_font_description_from_string(mascot->deffontname_bal);
#else
    fontset = g_strdup(mascot->deffontname_bal);
    mascot->fontbal=gdk_fontset_load(fontset);
#endif
    while(mascot->fontbal==NULL){
      create_default_font_selection_dialog(mascot,INIT_DEF_FONT_BAL);
#ifdef USE_GTK2
      mascot->fontbal=pango_font_description_from_string(mascot->deffontname_bal);
#else
      fontset = g_strdup(mascot->deffontname_bal);
      mascot->fontbal=gdk_fontset_load(fontset);
#endif
    }
    mascot->fontname_bal=mascot->deffontname_bal;
  }

#ifdef USE_CAIRO
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
#endif


#ifdef USE_GTK2
  gtk_widget_modify_font(clock_main,mascot->fontclk);
  mascot->fontname_clk=pango_font_description_to_string(mascot->fontclk);
  gtk_widget_modify_font(balloon_main,mascot->fontbal);
  mascot->fontname_bal=pango_font_description_to_string(mascot->fontbal);
#ifdef USE_WIN32
  gtk_widget_modify_font(clock_fg,mascot->fontclk);
#endif
#else
  g_free(fontset);
#endif

  mascot->nPixmap = i;
  mascot->sprites = sprites;
  mascot->drag=FALSE;
  if(!mascot->digit)  strcpy(mascot->digit,"00:00:00");
  // 初回フレーム用マスク切出し


  if(flag_img_cairo_go){
#if GTK_CHECK_VERSION(2,10,0)
    gdk_window_input_shape_combine_mask
      ( win_main->window, 
  	mascot->sprites[mascot->frame_pix[0][0]].mask,
 	0, 0 ); 
    gdk_window_set_cursor(win_main->window,mascot->cursor.normal);
#endif
  }
  else{
    gdk_window_shape_combine_mask
      ( win_main->window, 
  	mascot->sprites[mascot->frame_pix[0][0]].mask,
 	0, 0 ); 
  }
  

#ifdef __GTK_STATUS_ICON_H__
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
#ifdef USE_CAIRO
  cairo_t *cr;
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
  if (mascot->mail.pixmap!=NULL){
#ifdef USE_GTK2
    g_object_unref(G_OBJECT(mascot->mail.pixmap));
#else
    gdk_pixmap_unref(mascot->mail.pixmap);
#endif
    mascot->mail.pixmap=NULL;
  }
  if (mascot->mail.mask!=NULL){
#ifdef USE_GTK2
    g_object_unref(G_OBJECT(mascot->mail.mask));
#else
    gdk_pixmap_unref(mascot->mail.mask);
#endif
    mascot->mail.mask=NULL;
  }

  if(mascot->mail.pix_file){
  tmp_open=to_utf8(mascot->mail.pix_file);
#ifdef USE_GTK2
    pixbuf = gdk_pixbuf_new_from_file(tmp_open, NULL);
#else
    pixbuf = gdk_pixbuf_new_from_file(tmp_open);
#endif
    g_free(tmp_open);
  }
  else{
    if(mascot->mail.pix_pos==MAIL_PIX_LEFT){
      pixbuf = gdk_pixbuf_new_from_xpm_data(def_biff);
    }
    else{
      pixbuf = gdk_pixbuf_new_from_xpm_data(def_biff_r);
    }
  }
  w = gdk_pixbuf_get_width(pixbuf)*((gfloat)(mascot->magnify)/100);
  h = gdk_pixbuf_get_height(pixbuf)*((gfloat)(mascot->magnify)/100);

  pixbuf2=gdk_pixbuf_scale_simple(pixbuf,w,h,ipstyle);
 #ifdef USE_CAIRO
  if(flag_img_cairo_go){
    mascot->mail.pixmap = gdk_pixmap_new(biff_pix->window,
					 w,
					 h,
					 -1);
    
    cr = gdk_cairo_create(mascot->mail.pixmap);
    cairo_set_source_rgba (cr, 1, 1, 1, 0);
    cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint (cr);
    
    cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
    //cairo_scale (cr, 
    //		 (gfloat)(mascot->magnify)/100, 
    //		 (gfloat)(mascot->magnify)/100);
    //gdk_cairo_set_source_pixbuf(cr,pixbuf,0,0);
    gdk_cairo_set_source_pixbuf(cr,pixbuf2,0,0);
    
    cairo_paint_with_alpha (cr, (gdouble)mascot->alpha_biff/100);
    //cairo_paint (cr);
    
    cairo_destroy (cr);

    gdk_pixbuf_render_pixmap_and_mask(pixbuf2, NULL,
				      &mascot->mail.mask, 0xc0);
  }
  else{
    gdk_pixbuf_render_pixmap_and_mask(pixbuf2, &mascot->mail.pixmap,
				      &mascot->mail.mask, 0xc0);
  }
#else
  gdk_pixbuf_render_pixmap_and_mask(pixbuf2, &mascot->mail.pixmap,
				    &mascot->mail.mask, 0xc0);
#endif
  
#ifdef USE_GTK2
  g_object_unref(G_OBJECT(pixbuf));
  g_object_unref(G_OBJECT(pixbuf2));
#else
  gdk_pixbuf_unref(pixbuf);
  gdk_pixbuf_unref(pixbuf2);
#endif
  // 初回フレーム用マスク切出し
  gdk_window_set_back_pixmap(widget->window,
			     mascot->mail.pixmap,
			     FALSE);
  
  
#ifdef USE_GTK2
  gtk_window_resize (GTK_WINDOW(widget), w, h);
#else
  gdk_window_resize (widget->window, w, h);
#endif

#ifdef USE_GTK2
  gdk_draw_drawable(widget->window,
#else
  gdk_draw_pixmap(widget->window,
#endif
		  widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
		  mascot->mail.pixmap,
		  0,0,0,0,
		  widget->allocation.width,
		  widget->allocation.height);

  if(flag_img_cairo_go){
#if GTK_CHECK_VERSION(2,10,0)
    gdk_window_input_shape_combine_mask( widget->window, 
					 mascot->mail.mask,
					 0, 0 ); 
    gdk_window_set_cursor(widget->window,mascot->cursor.biff);
#endif
  }
  else{
    gdk_window_shape_combine_mask( widget->window, 
				   mascot->mail.mask,
				   0, 0 ); 
  }
}
#endif

// マスコットを変換した際にバッファを満たしておく
gint DrawMascot0(typMascot *mascot)
{
  gint i_work;

  //printf("DrawMascot0\n");


  for(i_work=0;i_work<2;i_work++){
    
    if (pixmap_main[i_work]) {
#ifdef USE_GTK2
      g_object_unref(G_OBJECT(pixmap_main[i_work]));
#else
      gdk_pixmap_unref(pixmap_main[i_work]);
#endif
    } 
   
    pixmap_main[i_work] = gdk_pixmap_new(win_main->window,
					 mascot->width,
					 mascot->height,
					 -1);
#ifdef USE_WIN32
    if(mascot->sdw_flag){
      if(mascot->sdw_height>0){
	if (pixmap_sdw[i_work]) {
	  g_object_unref(G_OBJECT(pixmap_sdw[i_work]));
	} 

	pixmap_sdw[i_work] = gdk_pixmap_new(win_sdw->window,
					    mascot->width,
					    mascot->sdw_height,
					    -1);
      }
    }
#endif
  }
  
  //printf("DrawMascot0 End\n");
  return(0);
}


// Buffer中へのマスコットの書き込み
gint DrawMascot(typMascot *mascot)
{
  gint work_page;

  work_page =mascot->pixmap_page;
  work_page^=1;


  if (pixmap_main[work_page]) {
#ifdef USE_GTK2
    g_object_unref(G_OBJECT(pixmap_main[work_page]));
#else
    gdk_pixmap_unref(pixmap_main[work_page]);
#endif
  } 
  
  pixmap_main[work_page] = gdk_pixmap_new(win_main->window,
					  mascot->width,
					  mascot->height,
					  -1);
  
#ifdef USE_WIN32
  if((mascot->sdw_flag)&&(mascot->sdw_height>0)){
    if (pixmap_sdw[work_page]) {
      g_object_unref(G_OBJECT(pixmap_sdw[work_page]));
    } 
    
    pixmap_sdw[work_page] = gdk_pixmap_new(win_sdw->window,
					   mascot->width,
					   mascot->sdw_height,
					   -1);
  }
#endif
  
#ifdef USE_GTK2
  gdk_draw_drawable(pixmap_main[work_page],
		    mascot->gc_main[work_page],
		    mascot->sprites[mascot->frame_pix[mascot->anime_ptn][mascot->anime_frm]].pixmap,
		    0, 0,
		    0, 0,
		    mascot->width, mascot->height);
#else
  gdk_draw_pixmap(pixmap_main[work_page],
		  mascot->gc_main[work_page],
		  mascot->sprites[mascot->frame_pix[mascot->anime_ptn][mascot->anime_frm]].pixmap,
		  0, 0,
		  0, 0,
		  mascot->width, mascot->height);
#endif
  
#ifdef USE_WIN32
  if((mascot->sdw_flag)&&(mascot->sdw_height>0)){
    gdk_draw_drawable(pixmap_sdw[work_page],
		      mascot->gc_main[work_page],
		      mascot->sprites[mascot->frame_pix[mascot->anime_ptn][mascot->anime_frm]].pixmap_sdw,
		      0, 0,
		      0, 0,
		      mascot->width, mascot->sdw_height);
  }
#endif


  gdk_window_set_back_pixmap(win_main->window,
			     pixmap_main[work_page],
			     FALSE);
  
#ifdef USE_WIN32
  if((mascot->sdw_flag)&&(mascot->sdw_height>0)){
    gdk_window_set_back_pixmap(win_sdw->window,
			       pixmap_sdw[work_page],
			       FALSE);
  }
#endif  
  /*XSetWindowBackgroundPixmap(GDK_WINDOW_XDISPLAY(win_main->window),
    GDK_WINDOW_XWINDOW(win_main->window),
    (Pixmap )pixmap_main[work_page]);*/
  if(flag_img_cairo_go){
#if GTK_CHECK_VERSION(2,10,0)
    gdk_window_input_shape_combine_mask
      ( win_main->window,
	mascot->sprites[mascot->frame_pix[mascot->anime_ptn][mascot->anime_frm]].mask,
	0, 0 );
    gdk_window_set_cursor(win_main->window,mascot->cursor.normal);
#endif
  }
  else{
    gdk_window_shape_combine_mask
      ( win_main->window,
	mascot->sprites[mascot->frame_pix[mascot->anime_ptn][mascot->anime_frm]].mask,
	0, 0 );
#ifdef USE_WIN32
  if((mascot->sdw_flag)&&(mascot->sdw_height>0)){
      gdk_window_shape_combine_mask
	( win_sdw->window,
	  mascot->sprites[mascot->frame_pix[mascot->anime_ptn][mascot->anime_frm]].mask_sdw,
	  0, 0 );
    }
#endif
    
  }
  
#ifdef USE_GTK2
  gdk_draw_drawable(win_main->window,
		    win_main->style->fg_gc[GTK_WIDGET_STATE(win_main)],
		    pixmap_main[work_page],
		    0,0,0,0,
		    mascot->width, mascot->height);
#else
  gdk_draw_pixmap(win_main->window,
		  win_main->style->fg_gc[GTK_WIDGET_STATE(win_main)],
		  pixmap_main[work_page],
		  0,0,0,0,
		  mascot->width, mascot->height);
#endif
  
#ifdef USE_WIN32
  if((mascot->sdw_flag)&&(mascot->sdw_height>0)){
    gdk_draw_drawable(win_sdw->window,
		      win_sdw->style->fg_gc[GTK_WIDGET_STATE(win_sdw)],
		      pixmap_sdw[work_page],
		      0,0,0,0,
		      mascot->width, mascot->sdw_height);
  }
#endif  

  mascot->pixmap_page=work_page;

  if(mascot->raise_kwin) raise_all();

  while (my_main_iteration(FALSE));


  gdk_flush();
 
  return(0);
} 

gint DrawMascotTemp(typMascot *mascot, gint i_pix)
{
  gint work_page;


  work_page =mascot->pixmap_page;
  //work_page^=1;

  if (pixmap_main[work_page]) {
#ifdef USE_GTK2
    g_object_unref(G_OBJECT(pixmap_main[work_page]));
#else
    gdk_pixmap_unref(pixmap_main[work_page]);
#endif
  } 
  
  pixmap_main[work_page] = gdk_pixmap_new(win_main->window,
					  mascot->width,
					  mascot->height,
					  -1);

#ifdef USE_WIN32
  if((mascot->sdw_flag)&&(mascot->sdw_height>0)){
    if (pixmap_sdw[work_page]) {
      g_object_unref(G_OBJECT(pixmap_sdw[work_page]));
    } 
    
    pixmap_sdw[work_page] = gdk_pixmap_new(win_sdw->window,
					   mascot->width,
					   mascot->sdw_height,
					   -1);
  }
#endif
  

#ifdef USE_GTK2
  gdk_draw_drawable(
#else
  gdk_draw_pixmap(
#endif
		  pixmap_main[work_page],
		  mascot->gc_main[work_page],
                  mascot->sprites[i_pix].pixmap,
                  0, 0,
                  0, 0,
		  mascot->width, mascot->height);

#ifdef USE_WIN32
  if((mascot->sdw_flag)&&(mascot->sdw_height>0)){
    gdk_draw_drawable(pixmap_sdw[work_page],
		      mascot->gc_main[work_page],
		      mascot->sprites[i_pix].pixmap_sdw,
		      0, 0,
		      0, 0,
		      mascot->width, mascot->sdw_height);
  }
#endif

  gdk_window_set_back_pixmap(win_main->window,
			     pixmap_main[work_page],
			     FALSE);

#ifdef USE_WIN32
  if((mascot->sdw_flag)&&(mascot->sdw_height>0)){
    gdk_window_set_back_pixmap(win_sdw->window,
			       pixmap_sdw[work_page],
			       FALSE);
  }
#endif  


  if(flag_img_cairo_go){
#if GTK_CHECK_VERSION(2,10,0)
    gdk_window_input_shape_combine_mask( win_main->window, 
					 mascot->sprites[i_pix].mask,
					 0, 0 ); 
    gdk_window_set_cursor(win_main->window,mascot->cursor.normal);
#endif
  }
  else{
    gdk_window_shape_combine_mask( win_main->window, 
				   mascot->sprites[i_pix].mask,
				   0, 0 ); 
#ifdef USE_WIN32
  if((mascot->sdw_flag)&&(mascot->sdw_height>0)){
      gdk_window_shape_combine_mask
	( win_sdw->window,
	  mascot->sprites[i_pix].mask_sdw,
	  0, 0 );
    }
#endif
  }



#ifdef USE_GTK2
  gdk_draw_drawable(
#else
  gdk_draw_pixmap(
#endif
		  win_main->window,
		  win_main->style->fg_gc[GTK_WIDGET_STATE(win_main)],
		  pixmap_main[work_page],
		  0,0,0,0,
		  mascot->width, mascot->height);

 #ifdef USE_WIN32
  if((mascot->sdw_flag)&&(mascot->sdw_height>0)){
    gdk_draw_drawable(win_sdw->window,
		      win_sdw->style->fg_gc[GTK_WIDGET_STATE(win_sdw)],
		      pixmap_sdw[work_page],
		      0,0,0,0,
		      mascot->width, mascot->sdw_height);
  }
#endif   

  mascot->pixmap_page=work_page;
  
  while (my_main_iteration(FALSE));

  gdk_flush();

  return(0);
}

// Buffer中へのマスコットの書き込み
gint DrawMascotWithDigit(typMascot *mascot){
  gint clk_width,clk_height;
  //GdkGC *gc;
  gint work_page;
  gint as=0;
#ifdef USE_GTK2
  PangoLayout *pango_text;
#else
  gint lb,rb,ds;
#endif
#ifdef USE_CAIRO
  cairo_t *cr;
  cairo_text_extents_t extents;
  gdouble ampmsize=0;
#endif

  work_page =mascot->pixmap_page;
  work_page^=1;

  
  if (pixmap_main[work_page]) {
#ifdef USE_GTK2
    g_object_unref(G_OBJECT(pixmap_main[work_page]));
#else
    gdk_pixmap_unref(pixmap_main[work_page]);
#endif
  } 
  
  pixmap_main[work_page] = gdk_pixmap_new(win_main->window,
					  mascot->width,
					  mascot->height,
					  -1);

#ifdef USE_GTK2
  gdk_draw_drawable(pixmap_main[work_page],
		    mascot->gc_main[work_page],
		    mascot->sprites[mascot->frame_pix[mascot->anime_ptn][mascot->anime_frm]].pixmap,
		    0, 0,
		    0, 0,
		    mascot->width, mascot->height);
#else
  gdk_draw_pixmap(pixmap_main[work_page],
		  mascot->gc_main[work_page],
                  mascot->sprites[mascot->frame_pix[mascot->anime_ptn][mascot->anime_frm]].pixmap,
                  0, 0,
                  0, 0,
		  mascot->width, mascot->height);
#endif
  
#ifdef USE_CAIRO
  if(flag_img_cairo_go){
#ifdef __PANGOCAIRO_H__
    pango_text=gtk_widget_create_pango_layout(clock_main,
					      mascot->digit);
    pango_layout_get_pixel_size(pango_text,&clk_width,&clk_height);
#endif
    cr = gdk_cairo_create(pixmap_main[work_page]);
   
    cairo_select_font_face (cr, 
			    mascot->fontclk_pc.family,
			    mascot->fontclk_pc.slant,
			    mascot->fontclk_pc.weight);

    cairo_set_font_size (cr, mascot->fontclk_pc.pointsize*96.0/72.0);
#ifndef __PANGOCAIRO_H__
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
			     (gdouble)mascot->alpclksd/0xFFFF); /* transparent */
#ifdef __PANGOCAIRO_H__
      cairo_move_to(cr,
		    mascot->clktext_x+mascot->clksd_x,
		    mascot->clktext_y+mascot->clksd_y);
      pango_cairo_show_layout(cr,pango_text);
#else
      cairo_move_to(cr,
		    mascot->clktext_x+mascot->clksd_x,
		    clk_height+mascot->clktext_y+mascot->clksd_y);
      cairo_show_text(cr,mascot->digit);
#endif
      switch(mascot->clktype){
      case CLOCK_TYPE_12S:
      case CLOCK_TYPE_12M:
	cairo_move_to(cr,
		      mascot->clktext_x+mascot->clksd_x+clk_width+3,
		      mascot->clktext_y+mascot->clksd_y+clk_height);
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
			   (gdouble)mascot->alpclk/0xFFFF); /* transparent */
#ifdef __PANGOCAIRO_H__
    cairo_move_to(cr,
		  mascot->clktext_x,
		  mascot->clktext_y);
    pango_cairo_show_layout(cr,pango_text);
#else
    cairo_move_to(cr,
		  mascot->clktext_x,
		  clk_height+mascot->clktext_y);
    cairo_show_text(cr,mascot->digit);
#endif
    switch(mascot->clktype){
    case CLOCK_TYPE_12S:
    case CLOCK_TYPE_12M:
      cairo_move_to(cr,
		    mascot->clktext_x+clk_width+3,
		    mascot->clktext_y+clk_height);
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
  else{
#endif // #ifdef USE_CAIRO

#ifdef USE_GTK2
    pango_text=gtk_widget_create_pango_layout(clock_main,
					      mascot->digit);
#endif
    
    
    
    if(mascot->flag_clksd){
#ifdef USE_GTK2
      gdk_draw_layout(pixmap_main[work_page],
		      mascot->gc_mainsd[work_page],
		      mascot->clktext_x*((gfloat)(mascot->magnify)/100)+1,
		      1,
		      pango_text);
#else		    
      gdk_draw_string(pixmap_main[work_page],mascot->fontclk,
		      mascot->gc_mainsd[work_page],
		      mascot->clktext_x*((gfloat)(mascot->magnify)/100)+1,
		      mascot->clktext_y*((gfloat)(mascot->magnify)/100)+1,
		      mascot->digit);
#endif
    }
#ifdef USE_GTK2
    gdk_draw_layout(pixmap_main[work_page],
		    mascot->gc_main[work_page],
		    mascot->clktext_x*((gfloat)(mascot->magnify)/100),
		    0,
		    pango_text);
#else		    
    gdk_draw_string(pixmap_main[work_page],mascot->fontclk,
		    mascot->gc_main[work_page],
		    mascot->clktext_x*((gfloat)(mascot->magnify)/100),
		    mascot->clktext_y*((gfloat)(mascot->magnify)/100),
		    mascot->digit);
#endif
    
    switch(mascot->clktype){
    case CLOCK_TYPE_12S:
    case CLOCK_TYPE_12M:
#ifdef USE_GTK2
      pango_layout_get_pixel_size(pango_text,&clk_width,&clk_height);
#else
      gdk_string_extents(mascot->fontclk,mascot->digit,
			 &lb,&rb,&clk_width,&as,&ds);
      clk_height=as+ds;
#endif
      
      // digital clock
      if(mascot->flag_clksd){
	if(mascot->clk_pm){
	  gdk_draw_arc(pixmap_main[work_page],mascot->gc_mainsd[work_page],
		       TRUE,
		       mascot->clktext_x*((gfloat)(mascot->magnify)/100)+1+clk_width,
		       mascot->clktext_y*((gfloat)(mascot->magnify)/100)+1+clk_height*2/3-as,
		       clk_height/3,clk_height/3,
		       0,(360*64));
	}
	else{
	  gdk_draw_arc(pixmap_main[work_page],mascot->gc_mainsd[work_page],
		       TRUE,
		       mascot->clktext_x*((gfloat)(mascot->magnify)/100)+1+clk_width,
		       mascot->clktext_y*((gfloat)(mascot->magnify)/100)+1-as,
		       clk_height/3,clk_height/3,
		       0,(360*64));
	}
      }
      
      if(mascot->clk_pm){
	gdk_draw_arc(pixmap_main[work_page],mascot->gc_main[work_page],
		     TRUE,
		     mascot->clktext_x*((gfloat)(mascot->magnify)/100)+clk_width,
		     mascot->clktext_y*((gfloat)(mascot->magnify)/100)+clk_height*2/3-as,
		     clk_height/3,clk_height/3,
		     0,(360*64));
      }
      else{
	gdk_draw_arc(pixmap_main[work_page],mascot->gc_main[work_page],
		     TRUE,
		     mascot->clktext_x*((gfloat)(mascot->magnify)/100)+clk_width,
		     mascot->clktext_y*((gfloat)(mascot->magnify)/100)-as,
		     clk_height/3,clk_height/3,
		     0,(360*64));
      }
      
      break;
    }
#ifdef USE_CAIRO
  }
#endif
  

  gdk_window_set_back_pixmap(win_main->window,
			     pixmap_main[work_page],
			     FALSE);

  if(flag_img_cairo_go){
#if GTK_CHECK_VERSION(2,10,0)
    gdk_window_input_shape_combine_mask( win_main->window, 
					 mascot->sprites[mascot->frame_pix[mascot->anime_ptn][mascot->anime_frm]].mask,
					 0, 0 ); 
    gdk_window_set_cursor(win_main->window,mascot->cursor.normal);
#endif
  }
  else{
    gdk_window_shape_combine_mask( win_main->window, 
				   mascot->sprites[mascot->frame_pix[mascot->anime_ptn][mascot->anime_frm]].mask,
				   0, 0 ); 
  }
  
#ifdef USE_GTK2
  gdk_draw_drawable(win_main->window,
		    win_main->style->fg_gc[GTK_WIDGET_STATE(win_main)],
		    pixmap_main[work_page],
		    0,0,0,0,
		    mascot->width, mascot->height);
#else
  gdk_draw_pixmap(win_main->window,
		  win_main->style->fg_gc[GTK_WIDGET_STATE(win_main)],
		  pixmap_main[work_page],
		  0,0,0,0,
		  mascot->width, mascot->height);
#endif
  mascot->pixmap_page=work_page;
  
#ifdef USE_CAIRO
#ifdef __PANGOCAIRO_H__
  g_object_unref(G_OBJECT(pango_text));
#endif
  if(flag_img_cairo_go) cairo_destroy(cr);
#else
#ifdef USE_GTK2
  g_object_unref(G_OBJECT(pango_text));
#endif
#endif //#ifdef USE_CAIRO

  while (my_main_iteration(FALSE));

  gdk_flush();

  return(0);
}


void ReInitGC(typMascot *mascot)
{
  int i_page;

  for(i_page=0;i_page<2;i_page++){
    if(mascot->gc_main[i_page]){
       gdk_gc_unref(mascot->gc_main[i_page]);
       gdk_gc_unref(mascot->gc_mainsd[i_page]);
    }
    mascot->gc_main[i_page] = MPCreatePen(pixmap_main[i_page], mascot->colclk);
    mascot->gc_mainsd[i_page] = MPCreatePen(pixmap_main[i_page], mascot->colclksd);

    if(mascot->gc_bal[i_page]){
       gdk_gc_unref(mascot->gc_bal[i_page]);
       gdk_gc_unref(mascot->gc_balbg[i_page]);
       gdk_gc_unref(mascot->gc_balbd[i_page]);
    }
    mascot->gc_bal[i_page] = MPCreatePen(pixmap_bal[i_page], mascot->colbal);
    mascot->gc_balbg[i_page] = MPCreatePen(pixmap_bal[i_page], mascot->colbalbg);
    mascot->gc_balbd[i_page] = MPCreatePen(pixmap_bal[i_page], mascot->colbalbd);

    if(mascot->gc_clk[i_page]){
       gdk_gc_unref(mascot->gc_clk[i_page]);
       
       gdk_gc_unref(mascot->gc_clkbg[i_page]);
       gdk_gc_unref(mascot->gc_clkbd[i_page]);
       gdk_gc_unref(mascot->gc_clksd[i_page]);
    }
    mascot->gc_clk[i_page] = MPCreatePen(pixmap_clk[i_page], mascot->colclk);
    mascot->gc_clkbg[i_page] = MPCreatePen(pixmap_clk[i_page], mascot->colclkbg);
    mascot->gc_clkbd[i_page] = MPCreatePen(pixmap_clk[i_page], mascot->colclkbd);
    mascot->gc_clksd[i_page] = MPCreatePen(pixmap_clk[i_page], mascot->colclksd);
  }
}


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
    label=gtk_label_new(_("Cannot Load Default Clock Font"));
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(fdialog)->vbox),
		       label,TRUE,TRUE,0);
    sprintf(tmp_label,"\"%s\"",mascot->deffontname_clk);
    label=gtk_label_new(tmp_label);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(fdialog)->vbox),
		       label,TRUE,TRUE,0);
    break;
  case INIT_DEF_FONT_BAL:
    gtk_window_set_title(GTK_WINDOW(fdialog),
			 _("Cannot Load Default Balloon Font"));
    label=gtk_label_new(_("Cannot Load Default Balloon Font"));
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(fdialog)->vbox),
		       label,TRUE,TRUE,0);
    sprintf(tmp_label,"\"%s\"",mascot->deffontname_bal);
    label=gtk_label_new(tmp_label);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(fdialog)->vbox),
		       label,TRUE,TRUE,0);
    break;
  }

  label=gtk_label_new(_("Please Change..."));
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(fdialog)->vbox),
		     label,TRUE,TRUE,0);

#ifdef __GTK_STOCK_H__
  button=gtkut_button_new_from_stock(_("OK"),GTK_STOCK_OK);
#else
  button=gtk_button_new_with_label(_("OK"));
#endif
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(fdialog)->action_area),
		     button,TRUE,TRUE,0);
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


static void close_child_dialog(GtkWidget *w, GtkWidget *dialog)
{
  //gdk_pointer_ungrab(GDK_CURRENT_TIME);

  gtk_widget_destroy(dialog);
  gtk_main_quit();
}

#ifdef USE_CAIRO
void screen_changed(GtkWidget *widget, GdkScreen *old_screen, 
		    gpointer userdata, gboolean cairo_flag)
{
    /* To check if the display supports alpha channels, get the colormap */
    GdkScreen *screen = gtk_widget_get_screen(widget);
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
}
#endif

void InitComposite(typMascot *mascot){
#ifdef USE_CAIRO

#ifdef USE_WIN32
  gtk_widget_unmap(win_sdw);
#endif
  gtk_widget_unmap(win_main);
#ifdef USE_BIFF
  gtk_widget_unmap(biff_pix);
#endif
  //#endif
  gtk_widget_unmap(clock_main);
  gtk_widget_unmap(balloon_main);
#ifdef USE_WIN32
  gtk_widget_unmap(balloon_fg);
  gtk_widget_unmap(balloon_fg);
#endif

#if GTK_CHECK_VERSION(2,12,0) || defined(USE_CAIRO)
#if GTK_CHECK_VERSION(2,10,0)
  if(gtk_widget_is_composited(GTK_WIDGET(win_main)))
    mascot->flag_composite=COMPOSITE_TRUE;
  else
    mascot->flag_composite=COMPOSITE_FALSE;
  mascot->force_composite=FALSE;
#else
  mascot->flag_composite=COMPOSITE_UNKNOWN;
#endif
#endif

#ifndef USE_WIN32
  //unrealize to change colormap
  gtk_window_resize(GTK_WINDOW(win_main),1,1);
  gtk_widget_unrealize(win_main);
  if((mascot->flag_img_cairo)&&(mascot->flag_composite==COMPOSITE_TRUE)){
    screen_changed(win_main, NULL, NULL, TRUE);
    flag_img_cairo_go=TRUE;
  }
  else if((mascot->force_composite)&&(mascot->flag_composite==COMPOSITE_UNKNOWN)){
    screen_changed(win_main, NULL, NULL, TRUE);
    flag_img_cairo_go=TRUE;
  }
  else{
    screen_changed(win_main,  NULL, NULL, FALSE);
    flag_img_cairo_go=FALSE;
  }
  //re-realize
  gtk_widget_realize(win_main);


  //unrealize to change colormap

#ifdef USE_BIFF
  gtk_widget_unrealize(biff_pix);
  if(flag_img_cairo_go){
    screen_changed(biff_pix, NULL, NULL, TRUE);
  }
  else{
    screen_changed(biff_pix,  NULL, NULL, FALSE);
  }
  //re-realize
  gtk_widget_realize(biff_pix);
#endif
#endif  //#ifndef USE_WIN32

  //unrealize to change colormap
  gtk_widget_unrealize(clock_main);
  if((mascot->flag_clk_cairo)&&(mascot->flag_composite==COMPOSITE_TRUE)){
    screen_changed(clock_main, NULL, NULL, TRUE);
  }
  else if((mascot->force_composite)&&(mascot->flag_composite==COMPOSITE_UNKNOWN)){
    screen_changed(clock_main, NULL, NULL, TRUE);
  }
  else{
    screen_changed(clock_main,  NULL, NULL, FALSE);
  }
  //re-realize
  gtk_widget_realize(clock_main);
  //clear buffer
  if(pixmap_clk[0]!=NULL) g_object_unref(G_OBJECT(pixmap_clk[0]));
  if(pixmap_clk[1]!=NULL) g_object_unref(G_OBJECT(pixmap_clk[1]));
  pixmap_clk[0]=NULL;
  pixmap_clk[1]=NULL;
  // call configure to make pixmaps
  dw_configure_clk(clock_main, "configure_event",(gpointer)mascot);

  //unrealize to change colormap
  gtk_widget_unrealize(balloon_main);
  if((mascot->flag_bal_cairo)&&(mascot->flag_composite==COMPOSITE_TRUE)){
    screen_changed(balloon_main, NULL, NULL, TRUE);
  }
  else if((mascot->force_composite)&&(mascot->flag_composite==COMPOSITE_UNKNOWN)){
    screen_changed(balloon_main, NULL, NULL, TRUE);
  }
  else{
    screen_changed(balloon_main,  NULL, NULL, FALSE);
  }
  //re-realize
  gtk_widget_realize(balloon_main);
  //clear buffer
  if(pixmap_bal[0]!=NULL) g_object_unref(G_OBJECT(pixmap_bal[0]));
  if(pixmap_bal[1]!=NULL) g_object_unref(G_OBJECT(pixmap_bal[1]));
  pixmap_bal[0]=NULL;
  pixmap_bal[1]=NULL;
  // call configure to make pixmaps
  dw_configure_bal(balloon_main, "configure_event",(gpointer)mascot);

  //reset input shape mask
#if GTK_CHECK_VERSION(2,10,0)
  gdk_window_input_shape_combine_mask(win_main->window, NULL,0, 0 ); 
  gdk_window_set_cursor(win_main->window,mascot->cursor.normal);
#ifdef USE_BIFF
  gdk_window_input_shape_combine_mask(biff_pix->window, NULL,0, 0 ); 
  gdk_window_set_cursor(biff_pix->window,mascot->cursor.biff);
#endif
#endif

  //reset shape mask
#ifndef USE_WIN32
  gdk_window_shape_combine_mask(win_main->window, NULL,0, 0 ); 
#ifdef USE_BIFF
  gdk_window_shape_combine_mask(biff_pix->window, NULL,0, 0 ); 
#endif
#endif

  gdk_window_shape_combine_mask(clock_main->window, NULL,0, 0 ); 
  gdk_window_shape_combine_mask(balloon_main->window, NULL,0, 0 ); 
#ifdef USE_WIN32
  gdk_window_shape_combine_mask(clock_fg->window, NULL,0, 0 ); 
  gdk_window_shape_combine_mask(balloon_fg->window, NULL,0, 0 ); 
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

#endif //#ifdef USE_CAIRO
}
  
