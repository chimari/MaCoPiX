//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//
//
//     clock.c
//     Creating and Drawing Panel Clock Windows
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

#ifndef USE_GTK3
GdkBitmap *mask_clk=NULL;
#endif

// from gui.c

void DrawPanelClock2();

//GtkWidget * make_clock(typMascot *mascot){
void  make_clock(typMascot *mascot){
  GtkWidget *ebox;
  mascot->clock_main = gtk_window_new(GTK_WINDOW_POPUP);
  gtk_window_set_accept_focus(GTK_WINDOW(mascot->clock_main),FALSE);
  gtk_widget_set_app_paintable(mascot->clock_main, TRUE);

  ebox=gtk_event_box_new();
  gtk_container_add (GTK_CONTAINER (mascot->clock_main), ebox);
  
  mascot->dw_clock = gtk_drawing_area_new();
  gtk_widget_set_size_request (mascot->dw_clock, 1, 1);
  gtk_container_add(GTK_CONTAINER(ebox), mascot->dw_clock);
  gtk_widget_set_app_paintable(mascot->dw_clock, TRUE);
  
  gtk_widget_set_events(ebox, 
			GDK_BUTTON_MOTION_MASK | 
			GDK_BUTTON_RELEASE_MASK | 
			GDK_BUTTON_PRESS_MASK | 
			GDK_EXPOSURE_MASK);
  gtk_widget_set_events(mascot->dw_clock, 
			GDK_STRUCTURE_MASK | GDK_EXPOSURE_MASK);

  gtk_widget_realize(mascot->clock_main);

  gdk_window_set_decorations(gtk_widget_get_window(mascot->clock_main), 0);
#ifndef USE_WIN32
  /* gdk_window_set_override_redirect is not implemented (for warning) */
  gdk_window_set_override_redirect(gtk_widget_get_window(mascot->clock_main),TRUE);
#endif
  my_signal_connect(mascot->dw_clock, "configure_event",
  		    dw_configure_clk, (gpointer)mascot);
#ifdef USE_GTK3
  my_signal_connect(mascot->dw_clock, "draw",dw_expose_clk,
  		    (gpointer)mascot);
#else
  my_signal_connect(mascot->dw_clock, "expose_event",
  		    dw_expose_clk, (gpointer)mascot);
  my_signal_connect(mascot->clock_main, "expose_event",
  		    expose_clk, (gpointer)mascot);
#endif
  my_signal_connect(ebox, "button_press_event",
  		    clk_drag_begin, (gpointer)mascot);
  my_signal_connect(ebox, "button_release_event",
  		    clk_drag_end, (gpointer)mascot);
  my_signal_connect(ebox, "motion_notify_event",
  		    clk_window_motion, (gpointer)mascot);

  gdk_window_set_cursor(gtk_widget_get_window(mascot->clock_main),mascot->cursor.clk);

  gtk_window_resize (GTK_WINDOW(mascot->clock_main), 1, 1);
  gtk_widget_set_size_request (mascot->dw_clock, 1, 1);
  //dw_configure_clk(mascot->dw_clock, "configure_event",(gpointer)mascot);
}


#ifdef FG_DRAW
void make_clock_fg(typMascot *mascot){
  GtkWidget *ebox;

  mascot->clock_fg = gtk_window_new(GTK_WINDOW_POPUP);
  gtk_window_set_accept_focus(GTK_WINDOW(mascot->clock_fg),FALSE);
  gtk_widget_set_app_paintable(mascot->clock_fg, TRUE);

  ebox=gtk_event_box_new();
  gtk_container_add (GTK_CONTAINER (mascot->clock_fg), ebox);
  
  mascot->dw_clkfg = gtk_drawing_area_new();
  gtk_widget_set_size_request (mascot->dw_clkfg, 1, 1);
  gtk_container_add(GTK_CONTAINER(ebox), mascot->dw_clkfg);
  gtk_widget_set_app_paintable(mascot->dw_clkfg, TRUE);
  
  gtk_widget_realize(mascot->clock_fg);
  gdk_window_set_decorations(gtk_widget_get_window(mascot->clock_fg), 0);

  
  my_signal_connect(mascot->dw_clkfg, "configure_event",
  		    dw_configure_clk, (gpointer)mascot);
#ifdef USE_GTK3
  my_signal_connect(mascot->dw_clkfg, "draw",dw_expose_clk,
  		    (gpointer)mascot);
#else
  my_signal_connect(mascot->dw_clkfg, "expose_event",
  		    dw_expose_clk, (gpointer)mascot);
  my_signal_connect(mascot->clock_fg, "expose_event",
  		    expose_clk, (gpointer)mascot);
#endif
  
  gdk_window_resize (gtk_widget_get_window(mascot->clock_fg), 1, 1);
  gtk_widget_set_size_request (mascot->dw_clkfg, 1, 1);
  //dw_configure_clk(mascot->clock_fg, "configure_event",(gpointer)mascot);
}
#endif


// パネル時計のpixmap bufferへの描画
void DrawPanelClock2(typMascot *mascot)
{
  gint clk_width,clk_height;
  gint new_w, new_h;
  cairo_t *cr;
  cairo_t *cr_mask;
  cairo_surface_t *surface_mask;
  gdouble M_PI=3.14159265;
  gdouble dx,dy;
  cairo_text_extents_t extents;
  PangoLayout *pango_text;
  gdouble ampmsize=0;
  gboolean shape_flag=FALSE;
#ifdef USE_GTK3
  cairo_surface_t *surface;
  cairo_region_t *region_mask;
#endif

  if(mascot->flag_clkrd){
    if(mascot->flag_composite==COMPOSITE_FALSE){
      shape_flag=TRUE;
    }
    else if((!mascot->force_composite)&&(mascot->flag_composite==COMPOSITE_UNKNOWN)){
      shape_flag=TRUE;
    }
  }

#ifdef USE_PANGOCAIRO
#ifdef USE_GTK3
  //css_change_font(mascot->dw_clock,mascot->fontclk);
#else
  gtk_widget_modify_font(mascot->dw_clock,mascot->fontclk);
#endif
  pango_text=gtk_widget_create_pango_layout(mascot->dw_clock,
					    mascot->digit);
  pango_layout_get_pixel_size(pango_text,&clk_width,&clk_height);
#endif

#ifdef USE_GTK3
  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
				       1000, 1000);
  cr = cairo_create(surface);
#else
  cr = gdk_cairo_create(gtk_widget_get_window(mascot->clock_main));
#endif
  
  cairo_select_font_face (cr, 
			  mascot->fontclk_pc.family,
			  mascot->fontclk_pc.slant,
			  mascot->fontclk_pc.weight);
  cairo_set_font_size (cr, 
		       mascot->fontclk_pc.pointsize*96/72);
#ifndef USE_PANGOCAIRO
  cairo_text_extents (cr, mascot->digit, &extents);
  clk_width=(gint)(extents.x_advance);
  clk_height=(gint)(extents.height);
#endif

  switch(mascot->clktype){
  case CLOCK_TYPE_12S:
  case CLOCK_TYPE_12M:
    cairo_text_extents (cr, "AM", &extents);
    ampmsize=(gint)(extents.x_advance*CLOCK_AMPM_RATIO);
    clk_width+=ampmsize+3;
    break;
  }

  cairo_destroy(cr);
#ifdef USE_GTK3
    cairo_surface_destroy(surface);
#endif
  
  new_w=clk_width+(mascot->clktext_x+mascot->wclkbd)*2;
  new_h=clk_height+(mascot->clktext_y+mascot->wclkbd)*2;

  
#ifdef USE_GTK3  ////////////////////// GTK3 ////////////////////////////////////
  if(shape_flag){
    surface_mask = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
					      new_w,new_h);
    cr_mask = cairo_create(surface_mask); 
    cairo_set_source_rgba (cr_mask, 0, 0, 0, 0); // transparent
    cairo_rectangle(cr_mask, 0, 0, new_w,new_h);
    cairo_fill(cr_mask);
    cairo_paint (cr_mask);
    cairo_set_source_rgba (cr_mask, 1, 1, 1, 1); // opaque white
  }

  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
				       new_w,new_h);
  cr = cairo_create(surface);
#else     ////////////////////// GTK2 ////////////////////////////////////
  if (pixmap_clk) {
    g_object_unref(G_OBJECT(pixmap_clk));
  } 
  
  pixmap_clk = gdk_pixmap_new(gtk_widget_get_window(mascot->clock_main),
			      new_w,
			      new_h,
			      -1);

  if(shape_flag){
    surface_mask = cairo_image_surface_create(CAIRO_FORMAT_RGB24,
					      new_w,new_h);
    cr_mask = cairo_create(surface_mask); 
    cairo_set_source_rgb (cr_mask, 0, 0, 0); // opaque black
    cairo_rectangle(cr_mask, 0, 0, new_w,new_h);
    cairo_fill(cr_mask);
    cairo_paint (cr_mask);
    cairo_set_source_rgb (cr_mask, 1, 1, 1); // opaque white
  } 
  
  cr = gdk_cairo_create(pixmap_clk);
#endif  // USE_GTK3
  
  if((mascot->flag_composite==COMPOSITE_TRUE)
     ||((mascot->force_composite)&&(mascot->flag_composite==COMPOSITE_UNKNOWN)))
    cairo_set_source_rgba (cr, 1, 1, 1, 0);
  else
    cairo_set_source_rgb (cr, 1, 1, 1); // opaque white
  
  // draw background

  cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
  cairo_paint (cr);

  cairo_select_font_face (cr, 
			  mascot->fontclk_pc.family,
			  mascot->fontclk_pc.slant,
			  mascot->fontclk_pc.weight);
  cairo_set_font_size (cr, 
		       mascot->fontclk_pc.pointsize*96/72);

  cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);

#ifdef FG_DRAW
  //BG should be opaque to BG only translucency for Win32
  my_cairo_set_source_rgba (cr, mascot->colclkbg, 1.0); /* opaque BG */
  cairo_rectangle(cr, 0, 0, new_w, new_h);
  cairo_fill(cr);
#else
  my_cairo_set_source_rgba (cr, mascot->colclkbg, (gdouble)mascot->alpclkbg/100.0); /* transparent */
			 
  if(shape_flag){
    cairo_rectangle(cr, 0, 0, new_w, new_h);
    cairo_fill(cr);
  }
#endif

  if(mascot->flag_clkrd){
    dx=(gdouble)mascot->clktext_x;
    dy=(gdouble)mascot->clktext_y;

    cairo_move_to(cr,0,dy);

    cairo_save (cr);
    cairo_translate (cr, dx, dy);
    cairo_scale (cr, dx, dy);
    cairo_arc (cr, 0, 0, 1., 180*(M_PI/180.), 270*(M_PI/180.));
    cairo_restore (cr);

    cairo_line_to(cr, new_w-dx, 0);

    cairo_save (cr);
    cairo_translate (cr, new_w-dx, dy);
    cairo_scale (cr, dx, dy);
    cairo_arc (cr, 0, 0, 1., 270*(M_PI/180.), 360*(M_PI/180.));
    cairo_restore (cr);

    cairo_line_to(cr, new_w, new_h-dy);

    cairo_save (cr);
    cairo_translate (cr, new_w-dx, new_h-dy);
    cairo_scale (cr, dx, dy);
    cairo_arc (cr, 0, 0, 1., 0*(M_PI/180.), 90*(M_PI/180.));
    cairo_restore (cr);

    cairo_line_to(cr, dx,  new_h);

    cairo_save (cr);
    cairo_translate (cr, dx, new_h-dy);
    cairo_scale (cr, dx, dy);
    cairo_arc (cr, 0, 0, 1., 90*(M_PI/180.), 180*(M_PI/180.));
    cairo_restore (cr);

    cairo_line_to(cr, 0,  dy);
  }
  else{
    cairo_rectangle(cr, 0, 0, new_w, new_h);
  }

  if(shape_flag){
    cairo_append_path(cr_mask,cairo_copy_path(cr));
    cairo_clip_preserve (cr_mask);
    cairo_paint(cr_mask);
    cairo_destroy(cr_mask);
    
#ifdef USE_GTK3  ////////////////////// GTK3 ////////////////////////////////////
    region_mask = gdk_cairo_region_create_from_surface(surface_mask);

#else            ////////////////////// GTK2 ////////////////////////////////////
    if (mask_clk) {
      g_object_unref(G_OBJECT(mask_clk));
    }

    mask_clk = make_mask_from_surface(surface_mask);
    // If having a mask, never clip
    // Anti-alias of cairo could cause discoloration
#endif // USE_GTK3
  }
  else{
    cairo_clip_preserve (cr);
  }

#ifndef FG_DRAW
  // Paint BG with alpha
  if(!shape_flag){
    cairo_fill_preserve(cr);
    cairo_paint(cr);
  }
#endif

  ///// BACKGROUND /////
  if(mascot->wclkbd>0){
    my_cairo_set_source_rgba (cr, mascot->colclkbd, (gdouble)mascot->alpclkbd/100.0); /* transparent */
			   
    cairo_set_line_width(cr,(gdouble)mascot->wclkbd *2);
    cairo_stroke(cr);
  }

  
    
  cairo_set_line_width(cr,1.0);
  cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
    
    
  if(mascot->flag_clksd){
    my_cairo_set_source_rgba (cr, mascot->colclksd, (gdouble)mascot->alpclksd/100.0); /* transparent */
			   
#ifdef USE_PANGOCAIRO
    cairo_move_to(cr,
		  mascot->wclkbd+mascot->clktext_x+mascot->clksd_x,
		  mascot->wclkbd+mascot->clktext_y+mascot->clksd_y);
    pango_cairo_show_layout(cr,pango_text);
#else
    cairo_text_extents (cr, mascot->digit, &extents);
    cairo_move_to(cr,
		  mascot->wclkbd+mascot->clktext_x+mascot->clksd_x,
		  -extents.y_bearing+mascot->wclkbd+mascot->clktext_y+mascot->clksd_y);
    cairo_show_text(cr,mascot->digit);
#endif

    
    switch(mascot->clktype){
    case CLOCK_TYPE_12S:
    case CLOCK_TYPE_12M:
      cairo_move_to(cr, 
		    new_w-ampmsize-mascot->wclkbd-mascot->clktext_x+mascot->clksd_x-1,
		    new_h-mascot->wclkbd-mascot->clktext_y+mascot->clksd_y-1);
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
  
  
  my_cairo_set_source_rgba (cr, mascot->colclk, (gdouble)mascot->alpclk/100.0); /* transparent */
			 
#ifdef USE_PANGOCAIRO
  cairo_move_to(cr,
		mascot->wclkbd+mascot->clktext_x,
		mascot->wclkbd+mascot->clktext_y);
  pango_cairo_show_layout(cr,pango_text);
#else
  cairo_text_extents (cr, mascot->digit, &extents);

  cairo_move_to(cr,
		mascot->wclkbd+mascot->clktext_x,
		-extents.y_bearing+mascot->wclkbd+mascot->clktext_y);
  cairo_show_text(cr,mascot->digit);
#endif
  
    
  switch(mascot->clktype){
  case CLOCK_TYPE_12S:
  case CLOCK_TYPE_12M:
    cairo_move_to(cr, 
		  new_w-ampmsize-mascot->wclkbd-mascot->clktext_x-1,
		  new_h-mascot->wclkbd-mascot->clktext_y-1);
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

  cairo_destroy(cr);
   

#ifdef USE_GTK3  ////////////////////// GTK3 ////////////////////////////////////
  if (pixbuf_clk) {
    g_object_unref(G_OBJECT(pixbuf_clk));
  } 
  pixbuf_clk = gdk_pixbuf_get_from_surface(surface, 0, 0, new_w, new_h);
  cairo_surface_destroy(surface);

#else            ////////////////////// GTK2 ////////////////////////////////////
  
#ifdef FG_DRAW
  if((mascot->flag_clkfg)&&(mascot->alpha_clk!=100)){
    GtkAllocation *allocation=g_new(GtkAllocation, 1);
    GtkStyle *style=gtk_widget_get_style(mascot->dw_clkfg);
    gtk_widget_get_allocation(mascot->dw_clkfg,allocation);
    
    gdk_draw_drawable(gtk_widget_get_window(mascot->dw_clkfg),
		      style->fg_gc[gtk_widget_get_state(mascot->dw_clkfg)],
		      pixmap_clk,
		      0,0,0,0,
		      allocation->width,
		      allocation->height);
    g_free(allocation);
  }
#endif
  {
    GtkAllocation *allocation=g_new(GtkAllocation, 1);
    GtkStyle *style=gtk_widget_get_style(mascot->dw_clock);
    gtk_widget_get_allocation(mascot->dw_clock,allocation);
    
    gdk_draw_drawable(gtk_widget_get_window(mascot->dw_clock),
		      style->fg_gc[gtk_widget_get_state(mascot->dw_clock)],
		      pixmap_clk,
		      0,0,0,0,
		      allocation->width,
		      allocation->height);
    g_free(allocation);
  }
#endif // USE_GTK3

#ifdef FG_DRAW
  if((mascot->flag_clkfg)&&(mascot->alpha_clk!=100)){
    gdk_window_resize (gtk_widget_get_window(mascot->clock_fg), new_w, new_h);
    gtk_widget_set_size_request (mascot->dw_clkfg, new_w,new_h);
  }
#endif
  gdk_window_resize (gtk_widget_get_window(mascot->clock_main), new_w, new_h);
  gtk_widget_set_size_request (mascot->dw_clock, new_w,new_h);

  if(shape_flag){
#ifdef USE_GTK3  ////////////////////// GTK3 ////////////////////////////////////
#ifdef FG_DRAW
    if((mascot->flag_clkfg)&&(mascot->alpha_clk!=100)){
      gdk_window_shape_combine_region( gtk_widget_get_window(mascot->clock_fg),
				       region_mask,
				       0,0);
    }
#endif

    gdk_window_shape_combine_region( gtk_widget_get_window(mascot->clock_main),
				     region_mask,
				     0,0);

    cairo_region_destroy(region_mask);
    cairo_surface_destroy(surface_mask);
#else            ////////////////////// GTK2 ////////////////////////////////////
#ifdef FG_DRAW
    if((mascot->flag_clkfg)&&(mascot->alpha_clk!=100)){
      gdk_window_shape_combine_mask( gtk_widget_get_window(mascot->clock_fg),
				     mask_clk,
				     0,0);
    }
#endif

    gdk_window_shape_combine_mask( gtk_widget_get_window(mascot->clock_main),
				   mask_clk,
				   0,0);
#endif // USE_GTK3
  }

#ifdef USE_GTK3  ////////////////////// GTK3 ////////////////////////////////////
#ifdef USE_FG_DRAW
  if((mascot->flag_clkfg)&&(mascot->alpha_clk!=100)){
      gtk_widget_queue_draw(mascot->dw_clkfg);
  }
#endif
  gtk_widget_queue_draw(mascot->dw_clock);
#else            ////////////////////// GTK2 ////////////////////////////////////
#ifdef USE_WIN32
  gdk_draw_drawable(gtk_widget_get_window(mascot->clock_fg),
		    mascot->clock_fg->style->fg_gc[GTK_WIDGET_STATE(mascot->clock_main)],
		    pixmap_clk,
		    0,0,0,0,
		    new_w,
		    new_h);
#endif
  gdk_draw_drawable(gtk_widget_get_window(mascot->clock_main),
		    mascot->clock_main->style->fg_gc[GTK_WIDGET_STATE(mascot->clock_main)],
		    pixmap_clk,
		    0,0,0,0,
		    new_w,
		    new_h);
#endif // USE_GTK3
  
  
#ifdef USE_PANGOCAIRO
  g_object_unref(G_OBJECT(pango_text));
#endif
  
  //while(my_main_iteration(FALSE));
  gdkut_flush(mascot);
  
}

void DrawPanelClock0(typMascot *mascot){
  //  if(((mascot->flag_clk_cairo)&&(mascot->flag_composite!=COMPOSITE_FALSE))
  //     ||(!mascot->flag_clkrd))
  DrawPanelClock2(mascot);
}
