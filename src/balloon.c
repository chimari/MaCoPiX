//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//
//
//     balloon.c
//     Creating and Drawing Balloon Message Windows
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
#include<string.h>
#include<ctype.h>

// *** GLOBAL ARGUMENT ***
extern GtkWidget *win_main, *balloon_main;
#ifdef USE_WIN32
extern GtkWidget *balloon_fg;
#endif
extern GtkWidget *popup_menu; 
extern gint window_x, window_y;
extern GdkDrawable *pixmap_bal[2];


extern gint dw_configure_bal();
extern gint dw_expose_bal();
extern void MoveBalloon();
extern void ResizeMoveBalloon();
#ifdef USE_CAIRO
extern gint dw_configure_bal2();
extern gint dw_expose_bal2();
extern void screen_changed();
extern gboolean supports_alpha;
#endif

extern gboolean flag_balloon;

GdkBitmap *mask_bal[2]={NULL,NULL};;

#ifdef USE_WIN32
  extern void GdkWinSetAlpha();
#endif

extern gdouble GetCurrentResolution();

gint old_x, old_y;

// *** Protptype of function in this file
void DrawBalloon();
void DoBalloon();
void make_balloon();

#ifdef USE_WIN32
void make_balloon_fg();
#endif
void balloon_arg_init();

void DoSysBalloon();

void DrawBalloon(typMascot *mascot, char **wn_iwp, int wn_max)
{
  gint bal_width,bal_height;
  GdkGCValues gcv;
  gint w,h;
  gint lb,rb,as,ds=0;
  GdkPoint points[4];
  gint h_arrow=6;
  int tmp_h,tmp_w,tmp_ds;
  int i_wn;
  gint work_page;
#ifdef USE_GTK2
  PangoLayout *pango_text;
  PangoLanguage *pango_lang;
#endif
  
  work_page=mascot->bal_page;
  work_page^=1;
  
  // bal_height, bal_width  テキスト一行の大きさ
  bal_height=0;
  bal_width=0;
  for(i_wn=0;i_wn<wn_max;i_wn++){
#ifdef USE_GTK2
    pango_text=gtk_widget_create_pango_layout(balloon_main,
					      wn_iwp[i_wn]);
    pango_layout_get_pixel_size(pango_text,&tmp_w,&tmp_h);
#else
    gdk_string_extents(mascot->fontbal,wn_iwp[i_wn],
		       &lb,&rb,&tmp_w,&as,&tmp_ds);
    tmp_h=as+tmp_ds;
#endif
    
    if(tmp_w>bal_width){
      bal_width=tmp_w;
    }
     if(tmp_h>bal_height){
       bal_height=tmp_h;
#ifdef USE_GTK2
       ds=0;
#else
       ds=tmp_ds;
#endif
     }
  }
  
  // w, h バルーン全体の大きさ
  w=bal_width+(mascot->baltext_x+mascot->wbalbd)*2;
  h=(bal_height+mascot->baltext_y)*wn_max
    +mascot->baltext_y+mascot->wbalbd*2;
  mascot->balwidth=w;
  mascot->balheight=h+h_arrow;
  
  // バルーンの位置決定
  mascot->bal_pos=mascot->bal_defpos;
  if(mascot->bal_pos==BAL_POS_LEFT){
    if((mascot->x-w+mascot->bal_lxoff[mascot->anime_ptn])<0) 
      mascot->bal_pos=BAL_POS_RIGHT;
  }
  else if(mascot->bal_pos==BAL_POS_RIGHT){
    if((mascot->x+mascot->width+w+mascot->bal_lxoff[mascot->anime_ptn])
       >mascot->width_root)
      mascot->bal_pos=BAL_POS_LEFT;
  }

    
  if (pixmap_bal[work_page]) {
#ifdef USE_GTK2
    g_object_unref(G_OBJECT(pixmap_bal[work_page]));
#else
    gdk_pixmap_unref(pixmap_bal[work_page]);
#endif
  } 

 
  pixmap_bal[work_page] = gdk_pixmap_new(balloon_main->window,w,h+h_arrow,-1);
  
  if (mask_bal[work_page]) {
#ifdef USE_GTK2
    g_object_unref(G_OBJECT(mask_bal[work_page]));
#else
    gdk_pixmap_unref(mask_bal[work_page]);
#endif
  } 
  
  mask_bal[work_page] = gdk_pixmap_new(balloon_main->window,
			    w,h+h_arrow,1); // Depth =1 (Bitmap)
  
  if(mascot->gc_balmask[work_page]){
    gdk_gc_unref(mascot->gc_balmask[work_page]);
  }
  mascot->gc_balmask[work_page]=gdk_gc_new(mask_bal[work_page]);
  // いったんマスクをforegroundでクリアする
  gdk_draw_rectangle(mask_bal[work_page],mascot->gc_balmask[work_page],
		     TRUE,
		     0,0,
		     w,h+h_arrow);
  // マスクはforeground色の部分が消されるので, 残したい部分は
  // background色で描画する
  gdk_gc_get_values(mascot->gc_balmask[work_page], &gcv);
  gdk_gc_set_foreground(mascot->gc_balmask[work_page], &gcv.background);
  gdk_gc_set_background(mascot->gc_balmask[work_page], &gcv.foreground);
  
    
  // **** pixmapへのBUFFERING
  // Mask (Borderと同じ)
#ifdef USE_WIN32
  gdk_draw_arc(mask_bal[work_page],mascot->gc_balmask[work_page],
	       TRUE,
	       0,0,
	       mascot->baltext_x*2,mascot->baltext_y*2,
               0,5760*4);
  gdk_draw_arc(mask_bal[work_page],mascot->gc_balmask[work_page],
	       TRUE,
	       (w+2)-mascot->baltext_x*2-1,0,
	       mascot->baltext_x*2,mascot->baltext_y*2,
  	       0,5760*4);
  gdk_draw_arc(mask_bal[work_page],mascot->gc_balmask[work_page],
	       TRUE,
	       0,(h+2)-mascot->baltext_y*2-1,
	       mascot->baltext_x*2,mascot->baltext_y*2,
  	       0,5760*4);
  gdk_draw_arc(mask_bal[work_page],mascot->gc_balmask[work_page],
	       TRUE,
	       (w+2)-mascot->baltext_x*2-1,(h+2)-mascot->baltext_y*2-1,
	       mascot->baltext_x*2,mascot->baltext_y*2,
               0,5760*4);
#else
  gdk_draw_arc(mask_bal[work_page],mascot->gc_balmask[work_page],
	       TRUE,
	       0,0,
	       mascot->baltext_x*2,mascot->baltext_y*2,
               5760,5760);
  gdk_draw_arc(mask_bal[work_page],mascot->gc_balmask[work_page],
	       TRUE,
	       w-mascot->baltext_x*2-1,0,
	       mascot->baltext_x*2,mascot->baltext_y*2,
  	       0,5760);
  gdk_draw_arc(mask_bal[work_page],mascot->gc_balmask[work_page],
	       TRUE,
	       0,h-mascot->baltext_y*2-1,
	       mascot->baltext_x*2,mascot->baltext_y*2,
  	       5760*2,5760);
  gdk_draw_arc(mask_bal[work_page],mascot->gc_balmask[work_page],
	       TRUE,
	       w-mascot->baltext_x*2-1,h-mascot->baltext_y*2-1,
	       mascot->baltext_x*2,mascot->baltext_y*2,
               5760*3,5760);
#endif
  gdk_draw_rectangle(mask_bal[work_page],mascot->gc_balmask[work_page],
		     TRUE,
		     0,mascot->baltext_y,
		     w,h-mascot->baltext_y*2);
  gdk_draw_rectangle(mask_bal[work_page],mascot->gc_balmask[work_page],
		     TRUE,
		     mascot->baltext_x,0,
		     w-mascot->baltext_x*2,h);

  points[0].y=h+h_arrow;
  points[1].y=h-mascot->baltext_y;
  points[2].y=h-mascot->baltext_y;
  points[3].y=h+h_arrow;
  if(mascot->bal_pos==BAL_POS_RIGHT){
    points[0].x=0;
    points[1].x=10-mascot->wbalbd;
    points[2].x=16+mascot->wbalbd;
    points[3].x=0;
  }
  else if(mascot->bal_pos==BAL_POS_LEFT){
    points[0].x=w;
    points[1].x=w-10+mascot->wbalbd;
    points[2].x=w-16-mascot->wbalbd;
    points[3].x=w;
  }
  gdk_draw_polygon(mask_bal[work_page],mascot->gc_balmask[work_page],
		   TRUE,points,4);
    

  // border
  if(mascot->wbalbd>0){
#ifdef USE_WIN32
    gdk_draw_arc(pixmap_bal[work_page],mascot->gc_balbd[work_page],
		 TRUE,
		 0,0,
		 mascot->baltext_x*2,mascot->baltext_y*2,
		 0,5760*4);
    gdk_draw_arc(pixmap_bal[work_page],mascot->gc_balbd[work_page],
		 TRUE,
		 (w+2)-mascot->baltext_x*2-1,0,
		 mascot->baltext_x*2,mascot->baltext_y*2,
		 0,5760*4);
    gdk_draw_arc(pixmap_bal[work_page],mascot->gc_balbd[work_page],
		 TRUE,
		 0,(h+2)-mascot->baltext_y*2-1,
		 mascot->baltext_x*2,mascot->baltext_y*2,
		 0,5760*4);
    gdk_draw_arc(pixmap_bal[work_page],mascot->gc_balbd[work_page],
		 TRUE,
		 (w+2)-mascot->baltext_x*2-1,(h+2)-mascot->baltext_y*2-1,
		 mascot->baltext_x*2,mascot->baltext_y*2,
		 0,5760*4);
#else
    gdk_draw_arc(pixmap_bal[work_page],mascot->gc_balbd[work_page],
		 TRUE,
		 0,0,
		 mascot->baltext_x*2,mascot->baltext_y*2,
		 5760,5760);
    gdk_draw_arc(pixmap_bal[work_page],mascot->gc_balbd[work_page],
		 TRUE,
		 w-mascot->baltext_x*2-1,0,
		 mascot->baltext_x*2,mascot->baltext_y*2,
		 0,5760);
    gdk_draw_arc(pixmap_bal[work_page],mascot->gc_balbd[work_page],
		 TRUE,
		 0,h-mascot->baltext_y*2-1,
		 mascot->baltext_x*2,mascot->baltext_y*2,
		 5760*2,5760);
    gdk_draw_arc(pixmap_bal[work_page],mascot->gc_balbd[work_page],
		 TRUE,
		 w-mascot->baltext_x*2-1,(h+2)-mascot->baltext_y*2-1,
		 mascot->baltext_x*2,mascot->baltext_y*2,
		 5760*3,5760);
#endif
    gdk_draw_rectangle(pixmap_bal[work_page],mascot->gc_balbd[work_page],
		       TRUE,
		       0,mascot->baltext_y,
		       w,h-mascot->baltext_y*2);
    gdk_draw_rectangle(pixmap_bal[work_page],mascot->gc_balbd[work_page],
		       TRUE,
		       mascot->baltext_x,0,
		       w-mascot->baltext_x*2,h);
  
  
    gdk_draw_polygon(pixmap_bal[work_page],mascot->gc_balbd[work_page],
		     TRUE,points,4);
  }
  

    // background
#ifdef USE_WIN32
  gdk_draw_arc(pixmap_bal[work_page],mascot->gc_balbg[work_page],
	       TRUE,
	       mascot->wbalbd,mascot->wbalbd,
	       mascot->baltext_x*2,mascot->baltext_y*2,
               0,5760*4);
  gdk_draw_arc(pixmap_bal[work_page],mascot->gc_balbg[work_page],
	       TRUE,
	       (w+2)-mascot->baltext_x*2-mascot->wbalbd-1,
	       mascot->wbalbd,
	       mascot->baltext_x*2,mascot->baltext_y*2,
  	       0,5760*4);
  gdk_draw_arc(pixmap_bal[work_page],mascot->gc_balbg[work_page],
	       TRUE,
	       mascot->wbalbd,
	       (h+2)-mascot->baltext_y*2-mascot->wbalbd-1,
	       mascot->baltext_x*2,mascot->baltext_y*2,
  	       0,5760*4);
  gdk_draw_arc(pixmap_bal[work_page],mascot->gc_balbg[work_page],
	       TRUE,
	       (w+2)-mascot->baltext_x*2-mascot->wbalbd-1,
	       (h+2)-mascot->baltext_y*2-mascot->wbalbd-1,
	       mascot->baltext_x*2,mascot->baltext_y*2,
  	       0,5760*4);
#else
  gdk_draw_arc(pixmap_bal[work_page],mascot->gc_balbg[work_page],
	       TRUE,
	       mascot->wbalbd,mascot->wbalbd,
	       mascot->baltext_x*2,mascot->baltext_y*2,
               5760,5760);
  gdk_draw_arc(pixmap_bal[work_page],mascot->gc_balbg[work_page],
	       TRUE,
	       w-mascot->baltext_x*2-mascot->wbalbd-1,
 	       mascot->wbalbd,
		 mascot->baltext_x*2,mascot->baltext_y*2,
		 0,5760);
  gdk_draw_arc(pixmap_bal[work_page],mascot->gc_balbg[work_page],
	       TRUE,
	       mascot->wbalbd,
	       h-mascot->baltext_y*2-mascot->wbalbd-1,
	       mascot->baltext_x*2,mascot->baltext_y*2,
  	       5760*2,5760);
  gdk_draw_arc(pixmap_bal[work_page],mascot->gc_balbg[work_page],
	       TRUE,
	       w-mascot->baltext_x*2-mascot->wbalbd-1,
	       h-mascot->baltext_y*2-mascot->wbalbd-1,
	       mascot->baltext_x*2,mascot->baltext_y*2,
  	       5760*3,5760);
#endif
  gdk_draw_rectangle(pixmap_bal[work_page],mascot->gc_balbg[work_page],
		     TRUE,
		     mascot->wbalbd,
		     mascot->wbalbd+mascot->baltext_y,
		     w-mascot->wbalbd*2,
		     h-mascot->baltext_y*2-mascot->wbalbd*2);
  gdk_draw_rectangle(pixmap_bal[work_page],mascot->gc_balbg[work_page],
		     TRUE,
		     mascot->wbalbd+mascot->baltext_x,
		     mascot->wbalbd,
		     w-mascot->baltext_x*2-mascot->wbalbd*2,
		     h-mascot->wbalbd*2);
  
  points[0].y-=1;
  points[1].y-=mascot->wbalbd;
  points[2].y-=mascot->wbalbd;
  points[3].y-=1;
  if(mascot->bal_pos==BAL_POS_RIGHT){
    points[1].x+=mascot->wbalbd;
    points[2].x-=mascot->wbalbd;
  }
  else if(mascot->bal_pos==BAL_POS_LEFT){
    points[1].x-=mascot->wbalbd;
    points[2].x+=mascot->wbalbd;
  }
  gdk_draw_polygon(pixmap_bal[work_page],mascot->gc_balbg[work_page],
		   TRUE,points,4);
  
  // Balloon Text
  for(i_wn=0;i_wn<wn_max;i_wn++){
#ifdef USE_GTK2
    pango_text=gtk_widget_create_pango_layout(balloon_main,
					      wn_iwp[i_wn]);
    
    gdk_draw_layout(pixmap_bal[work_page],
		    mascot->gc_bal[work_page],
    		    mascot->baltext_x+mascot->wbalbd,
    		    (mascot->baltext_y+bal_height)*(i_wn+1)
    		    +mascot->wbalbd-bal_height,
		    pango_text);
#else
    gdk_draw_string(pixmap_bal[work_page],mascot->fontbal,
    		    mascot->gc_bal[work_page],
    		    mascot->baltext_x+mascot->wbalbd,
    		    (mascot->baltext_y+bal_height)*(i_wn+1)
    		    +mascot->wbalbd-ds,
    		    wn_iwp[i_wn]);
#endif
  }
  mascot->bal_page=work_page;

  //  Balloon BG Draw  
#ifdef USE_WIN32
  if((mascot->flag_balfg)&&(mascot->alpha_bal!=100)){
    gdk_window_set_back_pixmap(balloon_fg->window,
			       pixmap_bal[mascot->bal_page],
			       FALSE);
  }
#endif

  gdk_window_set_back_pixmap(balloon_main->window,
			     pixmap_bal[mascot->bal_page],
			     FALSE);

 
  // Resize of Balloon (calling dw_bal_expose)
#ifdef USE_WIN32
  if((mascot->flag_balfg)&&(mascot->alpha_bal!=100)){
    gdk_window_resize (balloon_fg->window,w,h+h_arrow);
  }
#endif
  gdk_window_resize (balloon_main->window,w,h+h_arrow);
  //ResizeMoveBalloon (mascot,mascot->x, mascot->y, w,h+h_arrow);


#ifdef USE_WIN32
  if((mascot->flag_balfg)&&(mascot->alpha_bal!=100)){
    gdk_window_shape_combine_mask( balloon_fg->window,
				   mask_bal[mascot->bal_page],
				   0,0);
  }
#endif

  gdk_window_shape_combine_mask( balloon_main->window,
				 mask_bal[mascot->bal_page],
				 0,0);


  //  End of Balloon Draw  

  MoveBalloon(mascot,mascot->x,mascot->y);
  gdk_flush();

#ifdef USE_WIN32
  if((mascot->flag_balfg)&&(mascot->alpha_bal!=100)){
    gtk_widget_map(balloon_fg);
  }
#endif
  gtk_widget_map(balloon_main);
  // ToolTips使用時に削除 for v1.4.0
  //while (g_main_iteration(FALSE));

#ifdef USE_GTK2
  if(pango_text)
    g_object_unref(G_OBJECT(pango_text));
#endif
  gdk_flush();
}

#ifdef USE_CAIRO
void DrawBalloon2(typMascot *mascot, char **wn_iwp, int wn_max)
{
  gint bal_width,bal_height;
  GdkGCValues gcv;
  gint w,h;
  gint lb,rb,as,ds=0;
  GdkPoint points[4];
  gint h_arrow=6;
  int tmp_h,tmp_w,tmp_ds;
  int i_wn;
  gint work_page;
  PangoLayout *pango_text;
  PangoLanguage *pango_lang;
  cairo_t *cr;
  cairo_t *cr_mask;
  gdouble M_PI=3.14159265;
  gdouble dx,dy;
#ifndef __PANGOCAIRO_H__
  cairo_text_extents_t extents;
#endif  
  gboolean shape_flag=FALSE;

  work_page=mascot->bal_page;
  work_page^=1;
  
  if(mascot->flag_composite==COMPOSITE_FALSE){
    shape_flag=TRUE;
  }
  else if((!mascot->force_composite)&&(mascot->flag_composite==COMPOSITE_UNKNOWN)){
    shape_flag=TRUE;
  }

  h_arrow+=mascot->wbalbd;

  // bal_height, bal_width  テキスト一行の大きさ
  bal_height=0;
  bal_width=0;
  for(i_wn=0;i_wn<wn_max;i_wn++){
#ifdef __PANGOCAIRO_H__
    pango_text=gtk_widget_create_pango_layout(balloon_main,
					      wn_iwp[i_wn]);
    pango_layout_get_pixel_size(pango_text,&tmp_w,&tmp_h);
#else

    cr = gdk_cairo_create(balloon_main->window);
    
    cairo_select_font_face (cr, 
			    mascot->fontbal_pc.family,
			    mascot->fontbal_pc.slant,
			    mascot->fontbal_pc.weight);
    cairo_set_font_size (cr, 
			 mascot->fontbal_pc.pointsize*GetCurrentResolution()/72.0);
    cairo_text_extents (cr, wn_iwp[i_wn], &extents);
    tmp_w=(gint)(extents.x_advance);
    tmp_h=(gint)(extents.height);
    cairo_destroy(cr);
#endif
    
    if(tmp_w>bal_width){
      bal_width=tmp_w;
    }
     if(tmp_h>bal_height){
       bal_height=tmp_h;
       ds=0;
     }
  }
  
  // w, h バルーン全体の大きさ
  w=bal_width+(mascot->baltext_x+mascot->wbalbd)*2;
  h=(bal_height+mascot->baltext_y)*wn_max
    +mascot->baltext_y+mascot->wbalbd*2;
  mascot->balwidth=w;
  mascot->balheight=h+h_arrow;
  
  // バルーンの位置決定
  mascot->bal_pos=mascot->bal_defpos;
  if(mascot->bal_pos==BAL_POS_LEFT){
    if((mascot->x-w+mascot->bal_lxoff[mascot->anime_ptn])<0) 
      mascot->bal_pos=BAL_POS_RIGHT;
  }
  else if(mascot->bal_pos==BAL_POS_RIGHT){
    if((mascot->x+mascot->width+w+mascot->bal_lxoff[mascot->anime_ptn])
       >mascot->width_root)
      mascot->bal_pos=BAL_POS_LEFT;
  }

    
  if (pixmap_bal[work_page]) {
      g_object_unref(G_OBJECT(pixmap_bal[work_page]));
  } 

 
  pixmap_bal[work_page] = gdk_pixmap_new(balloon_main->window,
					  w,
					  h+h_arrow,
					  -1);

  if(shape_flag){
    if (mask_bal[work_page]) {
#ifdef USE_GTK2
      g_object_unref(G_OBJECT(mask_bal[work_page]));
#else
      gdk_pixmap_unref(mask_bal[work_page]);
#endif
    } 
    
    mask_bal[work_page] = gdk_pixmap_new(balloon_main->window,
					 w,h+h_arrow,1); // Depth =1 (Bitmap)
    
    cr_mask = gdk_cairo_create(mask_bal[work_page]);
    cairo_set_operator (cr_mask, CAIRO_OPERATOR_CLEAR);
    cairo_paint (cr_mask);
    cairo_set_operator (cr_mask, CAIRO_OPERATOR_SOURCE);
    cairo_set_source_rgb (cr_mask, 1, 1, 1); // opaque white
  }

  cr = gdk_cairo_create(pixmap_bal[work_page]);

  if(shape_flag){
    cairo_set_source_rgb (cr, 1, 1, 1); // opaque white
  }
  else{
    cairo_set_source_rgba (cr, 1, 1, 1, 0);
  }

  
  cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
  cairo_paint (cr);

  
  cairo_set_operator (cr, CAIRO_OPERATOR_OVER);

#ifdef USE_WIN32  
  //BG should be opaque to BG only translucency for Win32
  cairo_set_source_rgba (cr, 
			 (gdouble)mascot->colbalbg->red/0xFFFF,
			 (gdouble)mascot->colbalbg->green/0xFFFF,
			 (gdouble)mascot->colbalbg->blue/0xFFFF,
			 1); /* opaque BG */
  cairo_rectangle(cr, 0, 0, w, h+h_arrow);
  cairo_fill(cr);
#else
  //Even for X, to get clear shape and border,
  //Full area of the balloon window should be painted with BG color
  cairo_set_source_rgba (cr, 
			 (gdouble)mascot->colbalbg->red/0xFFFF,
			 (gdouble)mascot->colbalbg->green/0xFFFF,
			 (gdouble)mascot->colbalbg->blue/0xFFFF,
			 (gdouble)mascot->alpbalbg/0xFFFF); /* transparent */
  if(shape_flag){
    cairo_rectangle(cr, 0, 0, w, h+h_arrow);
    cairo_fill(cr);
  }
#endif

    
  // **** pixmapへのBUFFERING
  // border
  dx=(gdouble)mascot->baltext_x;
  dy=(gdouble)mascot->baltext_y;

  cairo_move_to(cr,0,dy);

  cairo_save (cr);
  cairo_translate (cr, dx, dy);
  cairo_scale (cr, dx, dy);
  cairo_arc (cr, 0, 0, 1., 180*(M_PI/180.), 270*(M_PI/180.));
  cairo_restore (cr);

  cairo_line_to(cr, w-dx, 0);

  cairo_save (cr);
  cairo_translate (cr, w-dx, dy);
  cairo_scale (cr, dx, dy);
  cairo_arc (cr, 0, 0, 1., 270*(M_PI/180.), 360*(M_PI/180.));
  cairo_restore (cr);

  cairo_line_to(cr, w, h-dy);

  cairo_save (cr);
  cairo_translate (cr, w-dx, h-dy);
  cairo_scale (cr, dx, dy);
  cairo_arc (cr, 0, 0, 1., 0*(M_PI/180.), 90*(M_PI/180.));
  cairo_restore (cr);
  // *** Bottom
  if(mascot->bal_pos==BAL_POS_RIGHT){
    cairo_line_to(cr, dx+6+mascot->wbalbd*3,  h);
    cairo_line_to(cr, 0,  h+h_arrow);
    cairo_line_to(cr, dx+mascot->wbalbd, h);
  }
  else{
    cairo_line_to(cr, w-dx-mascot->wbalbd,  h);
    cairo_line_to(cr, w,     h+h_arrow);
    cairo_line_to(cr, w-dx-6-mascot->wbalbd*3,  h);
  }
  cairo_line_to(cr, dx,  h);

  cairo_save (cr);
  cairo_translate (cr, dx, h-dy);
  cairo_scale (cr, dx, dy);
  cairo_arc (cr, 0, 0, 1., 90*(M_PI/180.), 180*(M_PI/180.));
  cairo_restore (cr);

  cairo_line_to(cr, 0,  dy);

  if(shape_flag){
    cairo_append_path(cr_mask,cairo_copy_path(cr));
    cairo_fill(cr_mask);
    // If having a mask, never clip
    // Anti-alias of cairo could cause discoloration
  }
  else{  
    cairo_clip_preserve (cr);
  }

#ifndef USE_WIN32
  // Paint BG with alpha
  if(!shape_flag)
    cairo_fill_preserve(cr);
#endif
  
  if(mascot->wbalbd>0){
    cairo_set_source_rgba (cr, 
			   (gdouble)mascot->colbalbd->red/0xFFFF,
			   (gdouble)mascot->colbalbd->green/0xFFFF,
			   (gdouble)mascot->colbalbd->blue/0xFFFF,
			   (gdouble)mascot->alpbalbd/0xFFFF); /* transparent */
    cairo_set_line_width(cr,(gdouble)mascot->wbalbd *2);
    cairo_stroke(cr);
  }

  // Balloon Text
  cairo_set_source_rgba (cr, 
			 (gdouble)mascot->colbal->red/0xFFFF,
			 (gdouble)mascot->colbal->green/0xFFFF,
			 (gdouble)mascot->colbal->blue/0xFFFF,
			 (gdouble)mascot->alpbal/0xFFFF); /* transparent */

#ifndef __PANGOCAIRO_H__
  cairo_select_font_face (cr, 
			  mascot->fontbal_pc.family,
			  mascot->fontbal_pc.slant,
			  mascot->fontbal_pc.weight);
  cairo_set_font_size (cr, 
		       mascot->fontbal_pc.pointsize*GetCurrentResolution()/72.0);
#endif


  for(i_wn=0;i_wn<wn_max;i_wn++){
#ifdef __PANGOCAIRO_H__
    pango_text=gtk_widget_create_pango_layout(balloon_main,
					      wn_iwp[i_wn]);

    cairo_move_to(cr,
		  mascot->baltext_x+mascot->wbalbd,
		  (mascot->baltext_y+bal_height)*(i_wn+1)
		  +mascot->wbalbd-bal_height);
    pango_cairo_show_layout(cr,pango_text);
#else
    cairo_text_extents (cr, wn_iwp[i_wn], &extents);
    cairo_move_to(cr,
		  mascot->baltext_x+mascot->wbalbd,
		  -extents.y_bearing+(mascot->baltext_y+bal_height)*(i_wn+1)
		  +mascot->wbalbd-bal_height);
    cairo_show_text(cr,wn_iwp[i_wn]);
#endif
  }

  cairo_destroy(cr);

  mascot->bal_page=work_page;

 //  Balloon BG Draw  
#ifdef USE_WIN32
  if((mascot->flag_balfg)&&(mascot->alpha_bal!=100)){
    gdk_window_set_back_pixmap(balloon_fg->window,
			       pixmap_bal[mascot->bal_page],
			       FALSE);
  }
#endif

  gdk_window_set_back_pixmap(balloon_main->window,
			     pixmap_bal[mascot->bal_page],
			     FALSE);

 

#ifdef USE_WIN32
  if((mascot->flag_balfg)&&(mascot->alpha_bal!=100)){
    gdk_draw_drawable(balloon_fg->window,
		      balloon_fg->style->fg_gc[GTK_WIDGET_STATE(balloon_fg)],
		      pixmap_bal[mascot->bal_page],
		      0,0,0,0,
		      w,
		      h+h_arrow);
  }
#endif

  gdk_draw_drawable(balloon_main->window,
		    balloon_main->style->fg_gc[GTK_WIDGET_STATE(balloon_main)],
		    pixmap_bal[mascot->bal_page],
		    0,0,0,0,
		    w,
		    h+h_arrow);

  // Resize of Balloon (calling dw_bal_expose)
#ifdef USE_WIN32
    if((mascot->flag_balfg)&&(mascot->alpha_bal!=100)){
      gdk_window_resize (balloon_fg->window,w,h+h_arrow);
    }
#endif
  gdk_window_resize (balloon_main->window,w,h+h_arrow);
  //ResizeMoveBalloon (mascot,mascot->x, mascot->y, w,h+h_arrow);

  if(shape_flag){
#ifdef USE_WIN32
    if((mascot->flag_balfg)&&(mascot->alpha_bal!=100)){
      gdk_window_shape_combine_mask( balloon_fg->window,
				     mask_bal[mascot->bal_page],
				     0,0);
    }
#endif

    gdk_window_shape_combine_mask( balloon_main->window,
				   mask_bal[mascot->bal_page],
				   0,0);
    cairo_destroy(cr_mask);
  }

 
  //  End of Balloon Draw  

  MoveBalloon(mascot,mascot->x,mascot->y);
  gdk_flush();
#ifdef USE_WIN32
  if((mascot->flag_balfg)&&(mascot->alpha_bal!=100)){
    gtk_widget_map(balloon_fg);
  }
#endif
  gtk_widget_map(balloon_main);

#ifdef __PANGOCAIRO_H__
  if(pango_text)
    g_object_unref(G_OBJECT(pango_text));
#endif
  gdk_flush();

}
#endif



// Balloonのpixmap bufferへの描画
void DoBalloon(typMascot *mascot)
{
  static int i_wp, i_wn;
  static char *wn[64][64];
  static char *wp[64], w_tmp[1024];
  static gint wp_max, wn_max[64];
  static gint wp_no, wn_no;
  static int balseqp;
  static int balseqp_biff;
  static int balseqp_sys;
  static int balseqp_sock;
  static int balseqpend[64];
  int i_p;

  char w_tmp_m[1024];
  char w_tmp_m3[1024];
  char *w_tmp_m2;
  int mp;

  if(mascot->balseq==0){ // バルーン表示文字列の解釈 
    mascot->balseq=1;    //  %p:改ページ %n:改行 %a:付加
    wp_no=0;

    switch(mascot->bal_mode){
    case BALLOON_MAIL:
      strcpy(w_tmp,mascot->mail.word);
      /* mail count %m */
      while(NULL != (w_tmp_m2 = strstr(w_tmp, "%m"))){
	mp = strlen(w_tmp) - strlen(w_tmp_m2); 
	strncpy(w_tmp_m, w_tmp, mp);
	w_tmp_m[mp] = '\0';
	w_tmp_m2 +=  strlen("%m");
	sprintf(w_tmp_m3,"%s%d%s",
		w_tmp_m, mascot->mail.count, w_tmp_m2);
	strcpy(w_tmp, w_tmp_m3);
      }
      while(NULL != (w_tmp_m2 = strstr(w_tmp, "%f"))){  // Last From
	mp = strlen(w_tmp) - strlen(w_tmp_m2); 
	strncpy(w_tmp_m, w_tmp, mp);
	w_tmp_m[mp] = '\0';
	w_tmp_m2 +=  strlen("%f");  //Last Sbject
	if(mascot->mail.last_f!=NULL)
	   sprintf(w_tmp_m3,"%s%s%s",
		   w_tmp_m, mascot->mail.last_f, w_tmp_m2);
	else
	  sprintf(w_tmp_m3,_("%s(From is not avairable)%s"),
		   w_tmp_m, w_tmp_m2);
	strcpy(w_tmp, w_tmp_m3);
      }
      while(NULL != (w_tmp_m2 = strstr(w_tmp, "%s"))){
	mp = strlen(w_tmp) - strlen(w_tmp_m2); 
	strncpy(w_tmp_m, w_tmp, mp);
	w_tmp_m[mp] = '\0';
	w_tmp_m2 +=  strlen("%s");
	if(mascot->mail.last_s!=NULL)
	  sprintf(w_tmp_m3,"%s%s%s",
		  w_tmp_m, mascot->mail.last_s, w_tmp_m2);
	else
	  sprintf(w_tmp_m3,_("%s(Subject is not avairable)%s"),
		   w_tmp_m, w_tmp_m2);
	strcpy(w_tmp, w_tmp_m3);
      }
      balseqp_biff=BALLOON_EXPIRE;
      break;
    case BALLOON_POPERROR:
      strcpy(w_tmp,_("POP3 access error"));
      balseqp_biff=BALLOON_EXPIRE;
      break;
    case BALLOON_SOCKMSG:
      strncpy(w_tmp,mascot->sockmsg,1023);
      balseqp_sock=mascot->sockmsg_expire;
      break;     
    case BALLOON_DUET:
      strncpy(w_tmp,mascot->sockmsg,1023);
      break;
    case BALLOON_SYS:
      strncpy(w_tmp,mascot->sysmsg,1023);
      balseqp_sys=SYS_BALLOON_EXPIRE;
      break;
    default:
      strcpy(w_tmp,mascot->click_word[mascot->anime_ptn]);
    }
 
    wp[wp_no]=(char *)strtok(w_tmp,"%");

    for(;;){
      wp_no++;
      if((wp[wp_no]=(char *)strtok(NULL,"%"))==NULL){
	wp_max=wp_no;
	break;
      }
      
      if(wp[wp_no][0]=='p'){
	balseqpend[wp_no-1]=10;  // デフォルト10コマ間隔とする
	wp[wp_no]++;
      }
      else if(wp[wp_no][0]=='a'){
	balseqpend[wp_no-1]=10;  // デフォルト10コマ間隔とする
	wp[wp_no]++;
	wp[wp_no] = g_strconcat(wp[wp_no-1],"%n",wp[wp_no],NULL);
      }
      else if(wp[wp_no][0]=='c'){
	balseqpend[wp_no-1]=1;  // デフォルト1コマ間隔とする
	wp[wp_no]++;
	wp[wp_no] = g_strconcat(wp[wp_no-1],wp[wp_no],NULL);
      }
      else if(isdigit(wp[wp_no][0])){
	balseqpend[wp_no-1]=wp[wp_no][0]-0x30;
	i_p=0;
	for(;;){
	  i_p++;
	  if(isdigit(wp[wp_no][i_p])){
	    balseqpend[wp_no-1]=balseqpend[wp_no-1]*10+wp[wp_no][i_p]-0x30;
	  }
	  else if(wp[wp_no][i_p]=='p'){
	    wp[wp_no]+=i_p;
	    break;
	  }
	  else if(wp[wp_no][i_p]=='a'){
	    wp[wp_no]+=i_p+1;
	    wp[wp_no] = g_strconcat(wp[wp_no-1],"%n",wp[wp_no],NULL);
	    wp[wp_no]--;
	    break;
	  }
	  else if(wp[wp_no][i_p]=='c'){
	    wp[wp_no]+=i_p+1;
	    wp[wp_no] = g_strconcat(wp[wp_no-1],wp[wp_no],NULL);
	    wp[wp_no]--;
	    break;
	  }
	  else{
	    wp[wp_no-1] = g_strconcat(wp[wp_no-1],"%",wp[wp_no],NULL);
	    wp_no--;
	    wp[wp_no]--;
	    break;
	  }
	}
	wp[wp_no]++;
      }
      else{
	wp[wp_no-1] = g_strconcat(wp[wp_no-1],"%",wp[wp_no],NULL);
	wp_no--;
      }
      
    }

    // 改行処理 %nを拾う
    for(i_wp=0;i_wp<wp_max;i_wp++){
      wn_no=0;
      wn[i_wp][wn_no]=(char *)strtok(wp[i_wp],"%");
      
      for(;;){
	wn_no++;
	if((wn[i_wp][wn_no]=(char *)strtok(NULL,"%"))==NULL){
	  wn_max[i_wp]=wn_no;
	  break;
	}
	
	if(wn[i_wp][wn_no][0]=='n'){
	    wn[i_wp][wn_no]++;
	  }
	  else{
	  wn[i_wp][wn_no-1] 
	    = g_strconcat(wn[i_wp][wn_no-1],"%",wn[i_wp][wn_no],NULL);
	  wn_no--;
	}
      }
    }
    // 文字切り分けDEBUG用
    /*  for(i_wp=0;i_wp<wp_max;i_wp++){
      for(i_wn=0;i_wn<wn_max[i_wp];i_wn++){
      printf("  %d: %d   %s\n",i_wp, i_wn, wn[i_wp][i_wn]);
      
      }
      }*/
    

    i_wp=0;
    i_wn=0;
    balseqp=0;

      
  }

  //printf("%2d :  %d/%d  %5d   %5d\n",mascot->bal_mode, i_wp, 
  // wp_max, balseqp, balseqp_biff);


  // バルーンの再描画   
  // balseqp = -1: 通常のバルーン降着, -2: Biff用バルーン降着
  if(balseqp==0){
    if(i_wp<wp_max-1){
      balseqp=balseqpend[i_wp];  // 改ページする間隔コマ数
    }

    
#ifdef USE_CAIRO
    //if((mascot->flag_bal_cairo)&&(mascot->flag_composite!=COMPOSITE_FALSE))
    if(mascot->flag_bal_cairo)
      DrawBalloon2(mascot, wn[i_wp], wn_max[i_wp]);
    else
#endif
      DrawBalloon(mascot, wn[i_wp], wn_max[i_wp]);


    if(i_wp<wp_max-1){
      i_wp++;
    }
    else{ // バルーン降着状態
      if(mascot->bal_mode==BALLOON_SOCKMSG) {
        balseqp=-3;
      }
      else if(mascot->bal_mode==BALLOON_SYS) {
        balseqp=-4;
      }
      else if((mascot->bal_mode==BALLOON_MAIL)
	      ||(mascot->bal_mode==BALLOON_POPERROR)){
	balseqp=-2;
      }
      else{
         balseqp=-1;
      }
    }
  }
  else{
    //マスコットが動いたらバルーンを再描画する
    if((old_x!=mascot->x)||(old_y!=mascot->y)){
#ifdef USE_CAIRO
      //if((mascot->flag_bal_cairo)&&(mascot->flag_composite!=COMPOSITE_FALSE))
      if(mascot->flag_bal_cairo)
	DrawBalloon2(mascot, wn[i_wp], wn_max[i_wp]);
      else
#endif
	DrawBalloon(mascot, wn[i_wp], wn_max[i_wp]);
    }

    // カウンタを減らす
    if(balseqp>0){
      balseqp--;
    }
    else if(balseqp==-2){
      if(balseqp_biff==0){  // Biff用バルーンのクローズ
#ifdef USE_WIN32
	gtk_widget_unmap(balloon_fg);
#endif
	gtk_widget_unmap(balloon_main);
	flag_balloon=FALSE;
      }
      else{
	balseqp_biff--;
      }
    } 
    else if(balseqp==-3){
      if(balseqp_sock==0){  // Socket用バルーンのクローズ
#ifdef USE_WIN32
	gtk_widget_unmap(balloon_fg);
#endif
	gtk_widget_unmap(balloon_main);
	flag_balloon=FALSE;
      }
      else{
	balseqp_sock--;
      }
    }
    else if(balseqp==-4){
      if(balseqp_sys==0){  // Sys用バルーンのクローズ
#ifdef USE_WIN32
	gtk_widget_unmap(balloon_fg);
#endif
	gtk_widget_unmap(balloon_main);

	flag_balloon=FALSE;
      }
      else{
	balseqp_sys--;
      }
    } 
  }
  old_x=mascot->x;
  old_y=mascot->y;

}


void make_balloon(typMascot *mascot){
  balloon_main = gtk_window_new(GTK_WINDOW_POPUP);
#ifdef USE_GTK2
  gtk_window_set_accept_focus(GTK_WINDOW(balloon_main),FALSE);
#endif
  gtk_widget_set_app_paintable(balloon_main, TRUE);
  gtk_window_set_wmclass(GTK_WINDOW(balloon_main), "balloon", "MaCoPiX");

  gtk_widget_set_events(GTK_WIDGET (balloon_main), 
			GDK_FOCUS_CHANGE_MASK | 
			GDK_BUTTON_MOTION_MASK | 
			GDK_BUTTON_RELEASE_MASK | 
			GDK_BUTTON_PRESS_MASK | 
			GDK_EXPOSURE_MASK);
  gtk_widget_realize(balloon_main);
  gdk_window_set_decorations(balloon_main->window, 0);
#ifdef USE_GTK2
  gtk_window_set_resizable(GTK_WINDOW(balloon_main),TRUE);
#endif
#ifndef USE_WIN32
  /* gdk_window_set_override_redirect is not implemented (for warning) */
  gdk_window_set_override_redirect(balloon_main->window,TRUE);
#endif


  my_signal_connect(balloon_main, "configure_event",
		    dw_configure_bal, (gpointer)mascot);
  my_signal_connect(balloon_main, "expose_event",
		    dw_expose_bal, (gpointer)mascot);

#ifdef USE_GTK2
  gtk_window_resize (GTK_WINDOW(balloon_main),1,1);
#endif
  //gtk_widget_show(balloon_main);
}


#ifdef USE_WIN32
void make_balloon_fg(typMascot *mascot){

  balloon_fg = gtk_window_new(GTK_WINDOW_POPUP);
#ifdef USE_GTK2
  gtk_window_set_accept_focus(GTK_WINDOW(balloon_fg),FALSE);
#endif
  gtk_widget_set_app_paintable(balloon_fg, TRUE);
  gtk_window_set_wmclass(GTK_WINDOW(balloon_fg), "balloon", "MaCoPiX");

  gtk_widget_set_events(GTK_WIDGET (balloon_fg), 
			GDK_FOCUS_CHANGE_MASK | 
			GDK_BUTTON_MOTION_MASK | 
			GDK_BUTTON_RELEASE_MASK | 
			GDK_BUTTON_PRESS_MASK | 
			GDK_EXPOSURE_MASK);
  gtk_widget_realize(balloon_fg);
  gdk_window_set_decorations(balloon_fg->window, 0);
  gtk_window_set_resizable(GTK_WINDOW(balloon_main),TRUE);

  my_signal_connect(balloon_fg, "configure_event",
		    dw_configure_bal, (gpointer)mascot);
  my_signal_connect(balloon_fg, "expose_event",
		    dw_expose_bal, (gpointer)mascot);

  gtk_window_resize (GTK_WINDOW(balloon_fg),1,1);
  //gtk_widget_show(balloon_fg);
}
#endif


void balloon_arg_init(){
  old_x=0;
  old_y=0;
}


void DoSysBalloon(typMascot *mascot, gchar *buf)
{

  mascot->balseq=0;
  mascot->bal_mode=BALLOON_SYS;
  
  if (mascot->sysmsg != NULL)
    g_free(mascot->sysmsg);
  mascot->sysmsg = g_strdup(buf);
  DoBalloon(mascot);
  flag_balloon=TRUE;
}

