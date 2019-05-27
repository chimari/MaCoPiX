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
#ifndef USE_GTK3
GdkBitmap *mask_bal=NULL;
#endif


gint old_x, old_y;

// *** Protptype of function in this file
void DrawBalloon2();

void DrawBalloon2(typMascot *mascot, char **wn_iwp, int wn_max)
{
  gint bal_width,bal_height;
  gint w,h;
  gint h_arrow;
  int tmp_h,tmp_w,tmp_ds;
  int i_wn;
  PangoLayout *pango_text;
  PangoLanguage *pango_lang;
  cairo_t *cr;
  cairo_t *cr_mask;
  gdouble M_PI=3.14159265;
  gdouble dx,dy,bd;
#ifndef USE_PANGOCAIRO
  cairo_text_extents_t extents;
#endif
  cairo_surface_t *surface_mask;
  gboolean shape_flag=FALSE;
#ifdef USE_GTK3
  cairo_surface_t *surface;
  cairo_region_t *region_mask;
#endif

  if(mascot->flag_composite==COMPOSITE_FALSE){
    shape_flag=TRUE;
  }
  else if((!mascot->force_composite)&&(mascot->flag_composite==COMPOSITE_UNKNOWN)){
    shape_flag=TRUE;
  }
 
  h_arrow=mascot->wbalbd*2+6;
  // bal_height, bal_width  テキスト一行の大きさ
  bal_height=0;
  bal_width=0;
  for(i_wn=0;i_wn<wn_max;i_wn++){
#ifdef USE_PANGOCAIRO
#ifdef USE_GTK3
    css_change_font(mascot->dw_balloon,mascot->fontbal_pc);
#else
    gtk_widget_modify_font(mascot->dw_balloon,mascot->fontbal);
#endif
    pango_text=gtk_widget_create_pango_layout(mascot->dw_balloon,
					      wn_iwp[i_wn]);
    pango_layout_get_pixel_size(pango_text,&tmp_w,&tmp_h);
#else
#ifdef USE_GTK3
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
					 1, 1);
    cr = cairo_create(surface);
#else
    cr = gdk_cairo_create(gtk_widget_get_window(mascot->dw_balloon));
#endif
    cairo_select_font_face (cr, 
			    mascot->fontbal_pc.family,
			    mascot->fontbal_pc.slant,
			    mascot->fontbal_pc.weight);
    cairo_set_font_size (cr, 
			 mascot->fontbal_pc.pointsize*96/72);
    cairo_text_extents (cr, wn_iwp[i_wn], &extents);
    tmp_w=(gint)(extents.x_advance);
    tmp_h=(gint)(extents.height);
    cairo_destroy(cr);
#endif
#ifdef USE_GTK3
    cairo_surface_destroy(surface);
#endif
    
    if(tmp_w>bal_width){
      bal_width=tmp_w;
    }
     if(tmp_h>bal_height){
       bal_height=tmp_h;
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

    
#ifdef USE_GTK3  ////////////////////// GTK3 ////////////////////////////////////
  if(shape_flag){
    surface_mask = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
					      w, h+h_arrow);
    cr_mask = cairo_create(surface_mask); 
    cairo_set_source_rgba (cr_mask, 0, 0, 0, 0); // transparent
    cairo_rectangle(cr_mask, 0, 0, w, h+h_arrow);
    cairo_fill(cr_mask);
    cairo_paint (cr_mask);
    cairo_set_source_rgba (cr_mask, 1, 1, 1, 1); // opaque white
  }

  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
				       w, h+h_arrow);
  cr = cairo_create(surface);
  
  cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.0);
  
#else     ////////////////////// GTK2 ////////////////////////////////////
  if (pixmap_bal) {
      g_object_unref(G_OBJECT(pixmap_bal));
  } 

 
  pixmap_bal = gdk_pixmap_new(gtk_widget_get_window(mascot->balloon_main),
			      w,
			      h+h_arrow,
			      -1);

  if(shape_flag){
    surface_mask = cairo_image_surface_create(CAIRO_FORMAT_RGB24,
					      w, h+h_arrow);
    cr_mask = cairo_create(surface_mask); 
    cairo_set_source_rgb (cr_mask, 0, 0, 0); // opaque black
    cairo_rectangle(cr_mask, 0, 0, w, h+h_arrow);
    cairo_fill(cr_mask);
    cairo_paint (cr_mask);
    cairo_set_source_rgb (cr_mask, 1, 1, 1); // opaque white
  }

  cr = gdk_cairo_create(pixmap_bal);

  if(shape_flag){
    cairo_set_source_rgb (cr, 1.0, 1.0, 1.0); // opaque white
  }
  else{
    cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.0);
  }
#endif // USE_GTK3   ////////////////////// ////////////////////////////////////
  
  cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
  cairo_paint (cr);
  
  cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
  
#ifdef FG_DRAW
  //BG should be opaque to BG only translucency for Win32
  my_cairo_set_source_rgba (cr, mascot->colbalbd, 1.0); /* opaque BG */

  cairo_rectangle(cr, 0, 0, w, h+h_arrow);
  cairo_fill(cr);
  cairo_paint (cr);
#else
  //Even for X, to get clear shape and border,
  //Full area of the balloon window should be painted with BG color
  cairo_set_source_rgba (cr, 
			 1,
			 1,
			 1,
			 0); /* transparent */
  if(shape_flag){
    cairo_rectangle(cr, 0, 0, w, h+h_arrow);
    cairo_fill(cr);
    cairo_paint (cr);
  }
#endif

    
  // **** pixmapへのBUFFERING
  // border
  dx=(gdouble)mascot->baltext_x;
  dy=(gdouble)mascot->baltext_y;
  bd=(gdouble)mascot->wbalbd;

  cairo_move_to(cr,bd,bd+dy);

  cairo_save (cr);
  cairo_translate (cr, dx+bd, dy+bd);
  cairo_scale (cr, dx, dy);
  cairo_arc (cr, 0, 0, 1., 180*(M_PI/180.), 270*(M_PI/180.));
  cairo_restore (cr);

  cairo_line_to(cr, w-dx-bd, bd);

  cairo_save (cr);
  cairo_translate (cr, w-dx-bd, dy+bd);
  cairo_scale (cr, dx, dy);
  cairo_arc (cr, 0, 0, 1., 270*(M_PI/180.), 360*(M_PI/180.));
  cairo_restore (cr);

  cairo_line_to(cr, w-bd, h-dy-bd);

  cairo_save (cr);
  cairo_translate (cr, w-dx-bd, h-dy-bd);
  cairo_scale (cr, dx, dy);
  cairo_arc (cr, 0, 0, 1., 0*(M_PI/180.), 90*(M_PI/180.));
  cairo_restore (cr);
  // *** Bottom
  if(mascot->bal_pos==BAL_POS_RIGHT){
    cairo_line_to(cr, bd+dx+h_arrow,  h-bd);
    cairo_line_to(cr, bd,  h+h_arrow-bd);
    cairo_line_to(cr, bd+dx, h-bd);
  }
  else{
    cairo_line_to(cr, w-dx-bd,  h-bd);
    cairo_line_to(cr, w-bd, h+h_arrow-bd);
    cairo_line_to(cr, w-bd-dx-h_arrow,  h-bd);
  }
  cairo_line_to(cr, dx,  h-bd);

  cairo_save (cr);
  cairo_translate (cr, bd+dx, h-dy-bd);
  cairo_scale (cr, dx, dy);
  cairo_arc (cr, 0, 0, 1., 90*(M_PI/180.), 180*(M_PI/180.));
  cairo_restore (cr);

  cairo_line_to(cr, bd,  bd+dy);

  if(shape_flag){
    cairo_append_path(cr_mask,cairo_copy_path(cr));
    cairo_clip_preserve (cr_mask);
    cairo_paint(cr_mask);
    cairo_destroy(cr_mask);

#ifdef USE_GTK3  ////////////////////// GTK3 ////////////////////////////////////
    region_mask = gdk_cairo_region_create_from_surface(surface_mask);
#else            ////////////////////// GTK2 ////////////////////////////////////
    if (mask_bal) {
      g_object_unref(G_OBJECT(mask_bal));
    }

    mask_bal = make_mask_from_surface(surface_mask);
    // If having a mask, never clip
    // Anti-alias of cairo could cause discoloration
#endif // USE_GTK3 ///////////////////////////////////////////////////////////
  }
  else{  
    cairo_clip_preserve (cr);
  }

#ifndef USE_WIN32
  // Paint BG with alpha
  if(!shape_flag){
    cairo_fill_preserve(cr);
  }
#endif

  ///// BACKGROUND /////
#ifdef FG_DRAW
  //BG should be opaque to BG only translucency for Win32
  my_cairo_set_source_rgba (cr, mascot->colbalbg, 1.0); /* opaque BG */
  cairo_fill_preserve(cr);
  cairo_paint (cr);
#else
  //Even for X, to get clear shape and border,
  //Full area of the balloon window should be painted with BG color
  my_cairo_set_source_rgba (cr, mascot->colbalbg, (gdouble)mascot->alpbalbg/100.0); /* transparent */

  //if(shape_flag){
  cairo_fill_preserve(cr);
  cairo_paint (cr);
  //}
#endif
  

  ///// BORDER //////
  if(mascot->wbalbd>0){
    my_cairo_set_source_rgba (cr, mascot->colbalbd, (gdouble)mascot->alpbalbd/100.0); /* transparent */
    cairo_set_line_width(cr,(gdouble)mascot->wbalbd *2);
    cairo_stroke(cr);
  }

  // Balloon Text
  my_cairo_set_source_rgba (cr, mascot->colbal, (gdouble)mascot->alpbal/100.0); /* transparent */

#ifndef USE_PANGOCAIRO
  cairo_select_font_face (cr, 
			  mascot->fontbal_pc.family,
			  mascot->fontbal_pc.slant,
			  mascot->fontbal_pc.weight);
  cairo_set_font_size (cr, 
		       mascot->fontbal_pc.pointsize*96/72);
#endif


  for(i_wn=0;i_wn<wn_max;i_wn++){
#ifdef USE_PANGOCAIRO
    pango_text=gtk_widget_create_pango_layout(mascot->dw_balloon,
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

#ifdef USE_GTK3  ////////////////////// GTK3 ////////////////////////////////////
  if (pixbuf_bal) {
    g_object_unref(G_OBJECT(pixbuf_bal));
  } 
  pixbuf_bal = gdk_pixbuf_get_from_surface(surface, 0, 0, w, h+h_arrow);
  //gdk_pixbuf_save(pixbuf_bal, "aho.png", "png", NULL, NULL);
  cairo_surface_destroy(surface);

#else            ////////////////////// GTK2 ////////////////////////////////////
  
#ifdef FG_DRAW
  if((mascot->flag_balfg)&&(mascot->alpha_bal!=100)){
    {
      GtkAllocation *allocation=g_new(GtkAllocation, 1);
      GtkStyle *style=gtk_widget_get_style(mascot->dw_balfg);
      gtk_widget_get_allocation(mascot->dw_balfg,allocation);
      
      gdk_draw_drawable(gtk_widget_get_window(mascot->dw_balfg),
			style->fg_gc[gtk_widget_get_state(mascot->dw_balfg)],
			pixmap_bal,
			0,0,0,0,
			w,
			h+h_arrow);
      g_free(allocation);
    }
  }
#endif

  {
    GtkAllocation *allocation=g_new(GtkAllocation, 1);
    GtkStyle *style=gtk_widget_get_style(mascot->dw_balloon);
    gtk_widget_get_allocation(mascot->dw_balloon,allocation);
    
    gdk_draw_drawable(gtk_widget_get_window(mascot->dw_balloon),
		      style->fg_gc[gtk_widget_get_state(mascot->dw_balloon)],
		      pixmap_bal,
		      0,0,0,0,
		      w,
		      h+h_arrow);
    g_free(allocation);
  }
#endif  // USE_GTK3
  
  // Resize of Balloon (calling dw_bal_expose)
#ifdef FG_DRAW
  if((mascot->flag_balfg)&&(mascot->alpha_bal!=100)){
    gtk_widget_set_size_request (mascot->dw_balfg, w, h+h_arrow);
    gdk_window_resize (gtk_widget_get_window(mascot->balloon_fg),w,h+h_arrow);
  }
#endif
  gtk_widget_set_size_request (mascot->dw_balloon, w, h+h_arrow);
  gdk_window_resize (gtk_widget_get_window(mascot->balloon_main),w,h+h_arrow);
  //ResizeMoveBalloon (mascot,mascot->x, mascot->y, w,h+h_arrow);

#ifdef USE_GTK3  ////////////////////// GTK3 ////////////////////////////////////
  if(shape_flag){
#ifdef FG_DRAW
    if((mascot->flag_balfg)&&(mascot->alpha_bal!=100)){
      gdk_window_shape_combine_region( gtk_widget_get_window(mascot->balloon_fg),
      				       region_mask,
      			       0,0);
    }
#endif
    
    gdk_window_shape_combine_region( gtk_widget_get_window(mascot->balloon_main),
    				     region_mask,
    				     0,0);

    cairo_region_destroy(region_mask);
    cairo_surface_destroy(surface_mask);
  }

#ifdef FG_DRAW
  if((mascot->flag_balfg)&&(mascot->alpha_bal!=100)){
      gtk_widget_queue_draw(mascot->dw_balfg);
  }
#endif
  gtk_widget_queue_draw(mascot->dw_balloon);
  
#else            ////////////////////// GTK2 ////////////////////////////////////
  if(shape_flag){
#ifdef FG_DRAW
    if((mascot->flag_balfg)&&(mascot->alpha_bal!=100)){
      gdk_window_shape_combine_mask( gtk_widget_get_window(mascot->balloon_fg),
				     mask_bal,
				     0,0);
    }
#endif

    gdk_window_shape_combine_mask( gtk_widget_get_window(mascot->balloon_main),
				   mask_bal,
				   0,0);
  }
#endif // USE_GTK3
 
  //  End of Balloon Draw  

  MoveBalloon(mascot,mascot->x,mascot->y);
  map_balloon(mascot, TRUE);

#ifdef USE_PANGOCAIRO
  if(pango_text)
    g_object_unref(G_OBJECT(pango_text));
#endif
  gdkut_flush(mascot->win_main);

}

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
	memcpy(w_tmp_m, w_tmp, mp);
	w_tmp_m[mp] = '\0';
	w_tmp_m2 +=  strlen("%m");
	sprintf(w_tmp_m3,"%s%d%s",
		w_tmp_m, mascot->mail.count, w_tmp_m2);
	strcpy(w_tmp, w_tmp_m3);
      }
      while(NULL != (w_tmp_m2 = strstr(w_tmp, "%f"))){  // Last From
	mp = strlen(w_tmp) - strlen(w_tmp_m2); 
	memcpy(w_tmp_m, w_tmp, mp);
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
	memcpy(w_tmp_m, w_tmp, mp);
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

    DrawBalloon2(mascot, wn[i_wp], wn_max[i_wp]);

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
      DrawBalloon2(mascot, wn[i_wp], wn_max[i_wp]);
    }

    // カウンタを減らす
    if(balseqp>0){
      balseqp--;
    }
    else if(balseqp==-2){
      if(balseqp_biff==0){  // Biff用バルーンのクローズ
	map_balloon(mascot, FALSE);
	flag_balloon=FALSE;
      }
      else{
	balseqp_biff--;
      }
    } 
    else if(balseqp==-3){
      if(balseqp_sock==0){  // Socket用バルーンのクローズ
	map_balloon(mascot, FALSE);
	flag_balloon=FALSE;
      }
      else{
	balseqp_sock--;
      }
    }
    else if(balseqp==-4){
      if(balseqp_sys==0){  // Sys用バルーンのクローズ
	map_balloon(mascot, FALSE);
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
  GtkWidget *ebox;
  
  mascot->balloon_main = gtk_window_new(GTK_WINDOW_POPUP);
  gtk_window_set_accept_focus(GTK_WINDOW(mascot->balloon_main),FALSE);
  gtk_widget_set_app_paintable(mascot->balloon_main, TRUE);
#ifndef USE_GTK3
  gtk_window_set_wmclass(GTK_WINDOW(mascot->balloon_main), "balloon", "MaCoPiX");
#endif
  
  ebox=gtk_event_box_new();
  gtk_container_add (GTK_CONTAINER (mascot->balloon_main), ebox);
  
  mascot->dw_balloon = gtk_drawing_area_new();
  gtk_widget_set_size_request (mascot->dw_balloon, 1, 1);
  gtk_container_add(GTK_CONTAINER(ebox), mascot->dw_balloon);
  gtk_widget_set_app_paintable(mascot->dw_balloon, TRUE);

  gtk_widget_realize(mascot->balloon_main);
  gdk_window_set_decorations(gtk_widget_get_window(mascot->balloon_main), 0);
  gtk_window_set_resizable(GTK_WINDOW(mascot->balloon_main),TRUE);
#ifndef USE_WIN32
  /* gdk_window_set_override_redirect is not implemented (for warning) */
  gdk_window_set_override_redirect(gtk_widget_get_window(mascot->balloon_main),TRUE);
#endif

  my_signal_connect(mascot->dw_balloon, "configure_event",
  		    dw_configure_bal, (gpointer)mascot);
#ifdef USE_GTK3
  my_signal_connect(mascot->dw_balloon, "draw",dw_expose_bal,
  		    (gpointer)mascot);
#else
  my_signal_connect(mascot->balloon_main, "expose_event",
		    expose_bal, (gpointer)mascot);
  my_signal_connect(mascot->dw_balloon, "expose_event",
		    dw_expose_bal, (gpointer)mascot);
#endif
  
  gtk_window_resize (GTK_WINDOW(mascot->balloon_main),1,1);
  gtk_widget_set_size_request (mascot->dw_balloon,1,1);
}


#ifdef FG_DRAW
void make_balloon_fg(typMascot *mascot){
  GtkWidget *ebox;

  mascot->balloon_fg = gtk_window_new(GTK_WINDOW_POPUP);
  gtk_window_set_accept_focus(GTK_WINDOW(mascot->balloon_fg),FALSE);
  gtk_widget_set_app_paintable(mascot->balloon_fg, TRUE);
#ifndef USE_GTK3
  gtk_window_set_wmclass(GTK_WINDOW(mascot->balloon_fg), "balloon", "MaCoPiX");
#endif
  
  ebox=gtk_event_box_new();
  gtk_container_add (GTK_CONTAINER (mascot->balloon_fg), ebox);
  
  mascot->dw_balfg = gtk_drawing_area_new();
  gtk_widget_set_size_request (mascot->dw_balfg, 1, 1);
  gtk_container_add(GTK_CONTAINER(ebox), mascot->dw_balfg);
  gtk_widget_set_app_paintable(mascot->dw_balfg, TRUE);

  gtk_widget_realize(mascot->balloon_fg);
  gdk_window_set_decorations(gtk_widget_get_window(mascot->balloon_fg), 0);
  gtk_window_set_resizable(GTK_WINDOW(mascot->balloon_fg),TRUE);
  
  my_signal_connect(mascot->dw_balfg, "configure_event",
  		    dw_configure_bal, (gpointer)mascot);
#ifdef USE_GTK3
  my_signal_connect(mascot->dw_balfg, "draw",dw_expose_bal,
  		    (gpointer)mascot);
#else
  my_signal_connect(mascot->balloon_fg, "expose_event",
		    expose_bal, (gpointer)mascot);
  my_signal_connect(mascot->dw_balfg, "expose_event",
		    dw_expose_bal, (gpointer)mascot);
#endif

  gtk_window_resize (GTK_WINDOW(mascot->balloon_fg),1,1);
  gtk_widget_set_size_request (mascot->dw_balfg,1,1);
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

