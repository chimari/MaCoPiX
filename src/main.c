//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//    A unified desktop mascot program
//    for UNIX / X Window System with Gdk Environment
//
//     main.c  
//     The Main code  for  MaCoPiX
//
//      Gtk+ > 1.2.6           (or Gtk+ > 2.2.0)
//      Gdk-pixbuf  > 0.7.0(?) (or Gtk+ > 2.2.0)
//      GNU GetText > 0.10.0                     are necessary.
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

gboolean FlagInstalledMenu;

// Prototype of functions in this file
#ifdef USE_GTK3
void  my_mascot_write_color();
void  my_mascot_write_color_woa();
gdouble my_mascot_read_color();
gboolean my_def_color_check();
gboolean my_color_equal();
#else
void  my_mascot_write_color();
void  my_mascot_write_color_woa();
gint my_mascot_read_color();
gboolean my_def_color_check();
gboolean my_color_equal();
#endif
void exit_w_msg();
void ReadMenu();
gboolean ScanMenu2();
void InitDefCol();
void ReadRC();
void MascotInstall();
gboolean FullPathMascotCehck();
gchar* FullPathRcFile();
void get_option();
void get_rc_option();
void usage();
#ifdef USE_COMMON
void print_common_dir();
#endif


#ifdef USE_GTK3
GdkRGBA init_colclk = {(gdouble)COLOR_CLK_R/(gdouble)0xFFFF,
		       (gdouble)COLOR_CLK_G/(gdouble)0xFFFF,
		       (gdouble)COLOR_CLK_B/(gdouble)0xFFFF,
		       (gdouble)DEF_ALPHA_OTHER/100.0};
GdkRGBA init_colclksd = {(gdouble)COLOR_CLKSD_R/(gdouble)0xFFFF,
			 (gdouble)COLOR_CLKSD_G/(gdouble)0xFFFF,
			 (gdouble)COLOR_CLKSD_B/(gdouble)0xFFFF,
			 (gdouble)DEF_ALPHA_SDW/100.0};
GdkRGBA init_colclkbg = {(gdouble)COLOR_CLKBG_R/(gdouble)0xFFFF,
			 (gdouble)COLOR_CLKBG_G/(gdouble)0xFFFF,
			 (gdouble)COLOR_CLKBG_B/(gdouble)0xFFFF,
			 (gdouble)DEF_ALPHA_CLK/100.0};
GdkRGBA init_colclkbd = {(gdouble)COLOR_CLKBD_R/(gdouble)0xFFFF,
			 (gdouble)COLOR_CLKBD_G/(gdouble)0xFFFF,
			 (gdouble)COLOR_CLKBD_B/(gdouble)0xFFFF,
			 (gdouble)DEF_ALPHA_OTHER/100.0};

GdkRGBA init_colbal = {(gdouble)COLOR_BAL_R/(gdouble)0xFFFF,
		       (gdouble)COLOR_BAL_G/(gdouble)0xFFFF,
		       (gdouble)COLOR_BAL_B/(gdouble)0xFFFF,
		       (gdouble)DEF_ALPHA_OTHER/100.0};
GdkRGBA init_colbalbg = {(gdouble)COLOR_BALBG_R/(gdouble)0xFFFF,
			 (gdouble)COLOR_BALBG_G/(gdouble)0xFFFF,
			 (gdouble)COLOR_BALBG_B/(gdouble)0xFFFF,
			   (gdouble)DEF_ALPHA_BAL/100.0};
GdkRGBA init_colbalbd = {(gdouble)COLOR_BALBD_R/(gdouble)0xFFFF,
			 (gdouble)COLOR_BALBD_G/(gdouble)0xFFFF,
			 (gdouble)COLOR_BALBD_B/(gdouble)0xFFFF,
			 (gdouble)DEF_ALPHA_OTHER/100.0}; 
#else
GdkColor init_colclk = {0, COLOR_CLK_R, COLOR_CLK_G, COLOR_CLK_B};
GdkColor init_colclksd = {0, COLOR_CLKSD_R, COLOR_CLKSD_G, COLOR_CLKSD_B};
GdkColor init_colclkbg = {0, COLOR_CLKBG_R, COLOR_CLKBG_G, COLOR_CLKBG_B};
GdkColor init_colclkbd = {0, COLOR_CLKBD_R, COLOR_CLKBD_G, COLOR_CLKBD_B};

GdkColor init_colbal = {0, COLOR_BAL_R, COLOR_BAL_G, COLOR_BAL_B};
GdkColor init_colbalbg = {0, COLOR_BALBG_R, COLOR_BALBG_G, COLOR_BALBG_B};
GdkColor init_colbalbd = {0, COLOR_BALBD_R, COLOR_BALBD_G, COLOR_BALBD_B};
#endif 

#ifdef USE_GTK3
void  my_mascot_write_color(ConfigFile * cfg, gchar * section,
			    gchar *key_r, gchar *key_g,
			    gchar *key_b, gchar *key_p,
			    GdkRGBA *col, GdkRGBA *def_col,
			    gint alpha, gint def_alpha){
  if(gdk_rgba_equal(col, def_col)){
    xmms_cfg_remove_key(cfg, section, key_r);
    xmms_cfg_remove_key(cfg, section, key_g);
    xmms_cfg_remove_key(cfg, section, key_b);
  }
  else{
    xmms_cfg_write_int(cfg, section, key_r,
		       (gint)(col->red*(gdouble)0xFFFF));
    xmms_cfg_write_int(cfg, section, key_g,
		       (gint)(col->green*(gdouble)0xFFFF));
    xmms_cfg_write_int(cfg, section, key_b,
		       (gint)(col->blue*(gdouble)0xFFFF));
  }
  if(alpha==def_alpha)
    xmms_cfg_remove_key(cfg, section, key_p);
  else
    xmms_cfg_write_int(cfg, section, key_p, alpha);
}


void  my_mascot_write_color_woa(ConfigFile * cfg, gchar * section,
				gchar *key_r, gchar *key_g, gchar *key_b, 
				GdkRGBA *col, GdkRGBA *def_col){
  if(gdk_rgba_equal(col, def_col)){
    xmms_cfg_remove_key(cfg, section, key_r);
    xmms_cfg_remove_key(cfg, section, key_g);
    xmms_cfg_remove_key(cfg, section, key_b);
  }
  else{
    xmms_cfg_write_int(cfg, section, key_r,
		       (gint)(col->red*(gdouble)0xFFFF));
    xmms_cfg_write_int(cfg, section, key_g,
		       (gint)(col->green*(gdouble)0xFFFF));
    xmms_cfg_write_int(cfg, section, key_b,
		       (gint)(col->blue*(gdouble)0xFFFF));
  }
}


gdouble my_mascot_read_color(ConfigFile * cfg, gchar * section, gchar * key, gdouble def_val){
  gint col_tmp;
  gdouble ret;
  if(xmms_cfg_read_int(cfg, section, key, &col_tmp)){
    ret=(gdouble)col_tmp/(gdouble)0xFFFF;
  }
  else{
    ret=def_val;
  }
  return(ret);
}

gboolean my_def_color_check(GdkRGBA *col, gint r, gint g, gint b, gint a){
  if((col->red  == r) &&
     (col->green == g) &&
     (col->blue == b) &&
     (col->alpha == a)){
    return(TRUE);
  }
  else{
    return(FALSE);
  }
}

gboolean my_color_equal(GdkRGBA *col1,  GdkRGBA *col2){
  return(gdk_rgba_equal(col1, col2));
}
#else
void  my_mascot_write_color(ConfigFile * cfg, gchar * section,
			    gchar *key_r, gchar *key_g,
			    gchar *key_b, gchar *key_p,
			    GdkColor *col, GdkColor *def_col,
			    gint alpha, gint def_alpha){
  if(gdk_color_equal(col, def_col)){
    xmms_cfg_remove_key(cfg, section, key_r);
    xmms_cfg_remove_key(cfg, section, key_g);
    xmms_cfg_remove_key(cfg, section, key_b);
  }
  else{
    xmms_cfg_write_int(cfg, section, key_r, col->red);
    xmms_cfg_write_int(cfg, section, key_g, col->green);
    xmms_cfg_write_int(cfg, section, key_b, col->blue);
  }
  if(alpha==def_alpha)
    xmms_cfg_remove_key(cfg, section, key_p);
  else
    xmms_cfg_write_int(cfg, section, key_p, alpha);
}


void  my_mascot_write_color_woa(ConfigFile * cfg, gchar * section,
				gchar *key_r, gchar *key_g, gchar *key_b, 
				GdkColor *col, GdkColor *def_col){
  if(gdk_color_equal(col, def_col)){
    xmms_cfg_remove_key(cfg, section, key_r);
    xmms_cfg_remove_key(cfg, section, key_g);
    xmms_cfg_remove_key(cfg, section, key_b);
  }
  else{
    xmms_cfg_write_int(cfg, section, key_r, col->red);
    xmms_cfg_write_int(cfg, section, key_g, col->green);
    xmms_cfg_write_int(cfg, section, key_b, col->blue);
  }
}

gint my_mascot_read_color(ConfigFile * cfg, gchar * section, gchar * key, gint def_val){
  gint col_tmp;
  if(!xmms_cfg_read_int(cfg, section, key, &col_tmp)){
    col_tmp=def_val;
  }
  return(col_tmp);
}

gboolean my_def_color_check(GdkColor *col, gint alpha, gint r, gint g , gint b, gint a){
  if((col->red  == r)
     &&(col->green == g)
     &&(col->blue == b)
     &&(alpha == a)){
    return(TRUE);
  }
  else{
    return(FALSE);
  }
}

gboolean my_color_equal(GdkColor *col1,  GdkColor *col2){
  return(gdk_color_equal(col1, col2));
}
#endif

// CSS for Gtk+3
#ifdef USE_GTK3
void css_change_col(GtkWidget *widget, gchar *color){
  GtkStyleContext *style_context;
  GtkCssProvider *provider = gtk_css_provider_new ();
  gchar *tmp;
  style_context = gtk_widget_get_style_context(widget);
  gtk_style_context_add_provider(style_context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  if(gtk_minor_version>=20)  {
    tmp=g_strdup_printf("button, label { color: %s; }", color);
  } else {
    tmp=g_strdup_printf("GtkButton, GtkLabel { color: %s; }", color);
  }
  gtk_css_provider_load_from_data(GTK_CSS_PROVIDER(provider), tmp, -1, NULL);
  g_object_unref (provider);
  g_free(tmp);
}

void css_change_font(GtkWidget *widget,  myPangoCairo mpc){
  GtkStyleContext *style_context;
  GtkCssProvider *provider = gtk_css_provider_new ();
  gchar *name, *tmp;
  style_context = gtk_widget_get_style_context(widget);
  gtk_style_context_add_provider(style_context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  if(gtk_minor_version>=20)  {
    name=g_strdup("drawing_area");
  } else {
    name=g_strdup("GtkDrawingArea");
  }

  tmp=g_strdup_printf("%s { font-family: %s; }", name, mpc.family);
  gtk_css_provider_load_from_data(GTK_CSS_PROVIDER(provider), tmp, -1, NULL);
  g_free(tmp);

  tmp=g_strdup_printf("%s { font-weight: %s; }", name,
		      (mpc.weight==CAIRO_FONT_WEIGHT_BOLD) ? "bold" : "normal");
  gtk_css_provider_load_from_data(GTK_CSS_PROVIDER(provider), tmp, -1, NULL);
  g_free(tmp);

  tmp=g_strdup_printf("%s { font-size: %.0lfpx; }", name,
		      mpc.pointsize);
  gtk_css_provider_load_from_data(GTK_CSS_PROVIDER(provider), tmp, -1, NULL);
  g_free(tmp);
  
  g_object_unref (provider);
}

void css_change_pbar_height(GtkWidget *widget,  gint height){
  GtkStyleContext *style_context;
  GtkCssProvider *provider = gtk_css_provider_new ();
  gchar tmp[64];
  style_context = gtk_widget_get_style_context(widget);
  gtk_style_context_add_provider(style_context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  if(gtk_minor_version>=20)  {
    g_snprintf(tmp, sizeof tmp, "progress, trough { min-height: %dpx; }", height);
  } else {
    g_snprintf(tmp, sizeof tmp, "GtkProgressBar, trough { min-height: %dpx; }", height);
  }
  gtk_css_provider_load_from_data(GTK_CSS_PROVIDER(provider), tmp, -1, NULL);
  g_object_unref (provider);
}
#endif


void exit_w_msg(gchar *s){
  fprintf(stderr,"%s\n",s);
  exit(1);
}

gchar *x_locale_to_utf8(gchar *srcstr, gssize len,
	gsize *r, gsize *w, GError **error, gchar *locale)
{
  gchar *res;
  GIConv converter;
  
  // Default Code described in mascot file
  if(locale){
    converter = g_iconv_open ("UTF-8//IGNORE", locale);
    res = g_convert_with_iconv (srcstr, len, converter, r, w, error);
    g_iconv_close(converter);
    if (res){
      return res;
    }
  }
  
  // 2nd  current locale
  res = g_locale_to_utf8 (srcstr, len, r, w, error);
  if (res) {
    locale=NULL;
    return res;
  }
  
  // 3rd  MaCoPiX default (EUC-JP)
  converter = g_iconv_open ("UTF-8//IGNORE", DEF_CODE1);
  res = g_convert_with_iconv (srcstr, len, converter, r, w, error);
  g_iconv_close(converter);
  if (res) {
    locale=g_strdup(DEF_CODE1);
    return (res);
  }
  else{
    return (srcstr);
  }
}

gchar *x_locale_from_utf8(gchar *srcstr, gssize len,
	gsize *r, gsize *w, GError **error, gchar *locale)
{
  gchar *res;
  gchar *tmp_locale;
  GIConv converter;
  
  
  // Default Code described in mascot file
  if(locale){
    tmp_locale=g_strconcat(locale,"//IGNORE",NULL);
    converter = g_iconv_open (tmp_locale,"UTF-8");
    g_free(tmp_locale);
    res = g_convert_with_iconv (srcstr, len, converter, r, w, error);
    g_iconv_close(converter);
    if (res) 
      return res;
  }
  
  // 2nd  current locale
  res = g_locale_to_utf8 (srcstr, len, r, w, error);
  if (res) 
    return res;
  
  // 3rd  MaCoPiX default (EUC-JP)
  converter = g_iconv_open (DEF_CODE1 "//IGNORE","UTF-8");
  res = g_convert_with_iconv (srcstr, len, converter, r, w, error);
  g_iconv_close(converter);
  if (res) return res;
  else return (srcstr);
}

gchar* to_utf8(gchar *input){
  return(x_locale_to_utf8(input,-1,NULL,NULL,NULL,NULL));
}

gchar* to_locale(gchar *input){
#ifdef USE_WIN32
  //return(x_locale_from_utf8(input,-1,NULL,NULL,NULL,"SJIS"));
  return(g_win32_locale_filename_from_utf8(input));
#else
  return(x_locale_from_utf8(input,-1,NULL,NULL,NULL,NULL));
#endif
}

gchar* all_random_menu_mascot_file(typMascot *mascot)
{
  int i_cat, i_tgt, i_all=0, dest_total, dest_cat=0, dest_tgt=0;

  dest_total=RANDOM(mascot->menu_total);

  for(i_cat=0;i_cat<mascot->menu_cat_max;i_cat++){
    for(i_tgt=0;i_tgt<mascot->menu_tgt_max[i_cat];i_tgt++){
      if(i_all==dest_total){
	dest_cat=i_cat;
	dest_tgt=i_tgt;
      }
      i_all++;
    }
  }
  
  return(mascot->menu_tgt[dest_cat][dest_tgt]);
}


// メニューファイルの読み込み
void ReadMenu(typMascot *mascot, gint offset_i_cat, gchar *merge_file)
{
  ConfigFile *cfgfile;
  gchar *filename=NULL, *filename2=NULL, *filename0=NULL;
  gchar tmp0[10], f_tmp0[10];
  int i_cat, i_tgt;
  gboolean flag_end=FALSE, install_flag=FALSE;
  gint m_place=0;
  struct stat statbuf;
  time_t user_mtime=0, common_mtime=0;
  gchar *dummy;
  gint menu_total_for_install;
  gchar *install_log=NULL;
  gchar *tmp_log=NULL;
  gchar *progress_txt=NULL;

  mascot->flag_consow=FALSE;

  if(!mascot->menu_file){     // メニューなし
    mascot->menu_total=0;
    mascot->menu_cat_max=0;
    return;
  }

  /// メニューファイルを順番で探す
    
  // 1. Currentから
  if(!merge_file){
    filename = g_strdup(mascot->menu_file);
  }
  else{
    filename = g_strdup(merge_file);
  }
  
  if(access(filename,F_OK)==0){
    m_place=1;
  }

  if(mascot->flag_ow){
#ifdef USE_COMMON
    // 4.  Common Directory
    if(m_place==0){
      filename = g_strconcat(COMMON_DIR,my_basename(mascot->menu_file), NULL);
      if(access(filename,F_OK)==0){
	m_place=4;
      }
    }
#endif

    // 3. USER Directory
    if(m_place==0){
#ifdef USE_WIN32
      filename = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,USER_DIR, 
			     my_basename(mascot->menu_file), NULL);
#else
      filename = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,USER_DIR, 
			     my_basename(mascot->menu_file), NULL);
#endif
      if(access(filename,F_OK)==0){
	m_place=3;
      }
    }
  }
  else{
    // 3. USER Directory
    if(m_place==0){
#ifdef USE_WIN32
      filename = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,USER_DIR, 
			     my_basename(mascot->menu_file), NULL);
#else

      filename = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,USER_DIR, 
			     my_basename(mascot->menu_file), NULL);
#endif
      if(access(filename,F_OK)==0){
	m_place=3;
      }
    }
    
#ifdef USE_COMMON
    // 4.  Common Directory
    if(m_place==0){
      filename = g_strconcat(COMMON_DIR,my_basename(mascot->menu_file), NULL);
      if(access(filename,F_OK)==0){
	m_place=4;
      }
    }
#endif
  }



  // みつからない
  if(m_place==0){
    popup_message(mascot->win_main,
#ifdef USE_GTK3
		  "dialog-error", 
#else
		  GTK_STOCK_DIALOG_ERROR,
#endif
		  -1,
		  _("Cannot Find Menu File."),
		  " ",
		  my_basename(to_utf8(mascot->menu_file)),
		  NULL);
    exit(1);
  }

  
  // ユーザー ディレクトリにそのファイルがなかったらコピーする
  if((m_place==1)||(m_place==4)){
    if(mascot->flag_install){
#ifdef USE_COMMON
      if(mascot->flag_common){
	check_common_dir();
	filename2 = g_strconcat(COMMON_DIR, 
				my_basename(mascot->menu_file), NULL);
      }
      else{
#endif
#ifdef USE_WIN32
	filename2 = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,USER_DIR, 
				my_basename(mascot->menu_file), NULL);
#else
	filename2 = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,USER_DIR, 
				my_basename(mascot->menu_file), NULL);
#endif
#ifdef USE_COMMON
      }
#endif
      if((access(filename2,F_OK)!=0)||(mascot->flag_ow)){
	if(strcmp(filename,filename2)!=0){
	  //g_print(_("Installing %s -> %s\n"),filename,filename2);
	  copy_file(filename,filename2);
	  install_flag=TRUE;
	  FlagInstalledMenu=TRUE;
	}
      }
    }
  }

#ifdef USE_COMMON
  // Consistency Check
  if(mascot->cons_check_mode!=CONS_IGNORE){
    if(m_place!=4){
      filename0 = g_strconcat(COMMON_DIR,my_basename(mascot->menu_file), NULL);
      if(access(filename0,F_OK)==0){
	stat(filename0, &statbuf);
	common_mtime=statbuf.st_mtime;
      }
#ifdef USE_WIN32      
      filename2 = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,USER_DIR, 
			      my_basename(mascot->menu_file), NULL);
#else
      filename2 = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,USER_DIR, 
			      my_basename(mascot->menu_file), NULL);
#endif
      if(access(filename2,F_OK)==0){
	stat(filename2, &statbuf);
	user_mtime=statbuf.st_mtime;
      }
      
      stat(filename, &statbuf);
      if((user_mtime==statbuf.st_mtime)&&(user_mtime<common_mtime)){
	// Menu file in User Dir is older 
	// than the corresponded one in Common Dir.
	if(mascot->cons_check_mode==CONS_MANUAL){
	  create_cons_dialog(mascot,my_basename(filename),
			     user_mtime,common_mtime,TRUE);
	}
	else if(mascot->cons_check_mode==CONS_AUTOOW){
	  mascot->flag_consow=TRUE;
	}
	if(mascot->flag_consow){
	  //g_print(_("Installing %s -> %s\n"),filename0,filename2);
	  // Copy Common -> User
	  copy_file(filename0,filename2);
	  install_flag=TRUE;
	  FlagInstalledMenu=TRUE;
	}
      }
    }
  }
#endif

  if(offset_i_cat==0){  // 通常読み込み
    mascot->menu_file=g_strdup(filename);

    mascot->menu_total=0;
    mascot->menu_cat_max=0;// これは足し算ではないのでmergeでもクリアできる。
  }
  cfgfile = xmms_cfg_open_file(filename);
  
  if (cfgfile) {
    if(!xmms_cfg_read_string(cfgfile, "General", "code",&mascot->menu_code))
      mascot->menu_code = NULL;

    if(mascot->flag_install){
      // PRESCAN for INSTALLING
      menu_total_for_install=0;
      for(i_cat=offset_i_cat;i_cat<MAX_MENU_CATEGORY;i_cat++){
	
	sprintf(f_tmp0,"Menu%02d",(i_cat-offset_i_cat));
	
	for(i_tgt=0;i_tgt<MAX_MENU_TARGET;i_tgt++){
	  sprintf(tmp0,"file%02d",i_tgt);
	  if(!xmms_cfg_read_string(cfgfile, f_tmp0, tmp0, &dummy)){
	    if(i_tgt==0){
	      if(i_cat==0){
		popup_message(NULL,
#ifdef USE_GTK3
			      "dialog-error", 
#else
			      GTK_STOCK_DIALOG_ERROR,
#endif
			      -1,
			      _(" Cannot read the Menu File correctly!!"),
			      NULL);
		exit(1);
	      }
	      flag_end=TRUE;
	      break;
	    }
	    else{
	      break;
	    }
	  }
	  else{
	    menu_total_for_install++;
	  }
	}
	if(flag_end) break;
      }
    }

    flag_end=FALSE;

    for(i_cat=offset_i_cat;i_cat<MAX_MENU_CATEGORY;i_cat++){
      
      sprintf(f_tmp0,"Menu%02d",(i_cat-offset_i_cat));
      if(!xmms_cfg_read_string(cfgfile, f_tmp0, "Name",
			       &mascot->menu_cat[i_cat]))
	mascot->menu_cat[i_cat]=NULL;
      if(mascot->menu_cat[i_cat]){
      	mascot->menu_cat[i_cat]=
      	  x_locale_to_utf8(mascot->menu_cat[i_cat],-1,NULL,NULL,NULL,mascot->menu_code);
	if(!mascot->menu_cat[i_cat]) 
	  mascot->menu_cat[i_cat]=g_strdup(_("(Invalid Character Code)"));
      }
      
      for(i_tgt=0;i_tgt<MAX_MENU_TARGET;i_tgt++){
	sprintf(tmp0,"file%02d",i_tgt);
	if(!xmms_cfg_read_string(cfgfile, f_tmp0, tmp0,
				 &mascot->menu_tgt[i_cat][i_tgt])){
	  if(i_tgt==0){
	    if(i_cat==0){
	      popup_message(mascot->win_main,
#ifdef USE_GTK3
			    "dialog-error", 
#else
			    GTK_STOCK_DIALOG_ERROR,
#endif
			    -1,
			    _(" Cannot read the Menu File correctly!!"),
			    NULL);
	      exit(1);
	    }
	    flag_end=TRUE;
	    mascot->menu_cat_max=i_cat;
	    break;
	  }
	  else{
	    mascot->menu_tgt_max[i_cat]=i_tgt;

	    break;
	  }
	}
	else{
	  if(FlagInstalledMenu){
	    MascotInstall(mascot,mascot->menu_tgt[i_cat][i_tgt]);
	  }
	  mascot->menu_tgt_name[i_cat][i_tgt]
	    =ReadMascotName(mascot, mascot->menu_tgt[i_cat][i_tgt]);
	  if(mascot->flag_install){
	    progress_txt=g_strdup_printf(_("Installed Mascot [%2d/%2d] \"%s\"\n"),
					 i_tgt+1,menu_total_for_install,
					 mascot->menu_tgt_name[i_cat][i_tgt]);
	    if(install_log){
	      tmp_log=g_strconcat(install_log,progress_txt,NULL);
	      install_log=g_strdup(tmp_log);
	      g_free(tmp_log);
	    }
	    else{
	      install_log=g_strdup(progress_txt);
	    }
	    g_free(progress_txt);
	  }
	  mascot->menu_total++;
	}
      }
      if(flag_end) break;
    }
    xmms_cfg_free(cfgfile);
    if(mascot->flag_install){
      if(install_log){
	popup_text_view(NULL,
#ifdef USE_GTK3
			"dialog-information", 
#else
			GTK_STOCK_DIALOG_INFO,
#endif
			install_log,
			_("Mascot instllation has been completed."),
			NULL);
	g_free(install_log);
      }
    }

  }

  if(install_flag)   
    mascot->menu_file=g_strdup(filename2);

  g_free(filename);
  g_free(filename0);
  g_free(filename2);

  mascot->flag_consow=FALSE;
  if(mascot->installed_menu_dir){
    unlink_all(mascot, mascot->installed_menu_dir);
    mascot->installed_menu_dir=NULL;
  }
  FlagInstalledMenu=FALSE;

  /*for(i_cat=0;i_cat<mascot->menu_cat_max;i_cat++){
    g_print(" Category : %s \n", mascot->menu_cat[i_cat]);
    
    for(i_tgt=0;i_tgt<mascot->menu_tgt_max[i_cat];i_tgt++){
      g_print("     File : %s \n", mascot->menu_tgt[i_cat][i_tgt]);
    }    
    }*/

}


// メニューファイルの検索
void ScanMenu(typScanMenu *smenu, gboolean flag_print)
{
  DIR *dirp;
  struct dirent *entry;
  struct stat statbuf;
  gchar *dirname=NULL, *cwdname=NULL;
  gchar *cp, *extp;
  gint i_menu=0;

  if(flag_print){
    g_print(_(" ================== Installed Menus ================== \n"));
  }

  cwdname=g_malloc0(sizeof(gchar)*1024);
  if(getcwd(cwdname,1024)==NULL) exit_w_msg("GetCWD error");
  extp=g_strdup(MENU_EXTENSION);
  extp++;

  smenu->max_menu=0;
  
  /// メニューファイルを順番で探す
  
#ifdef USE_COMMON
  // Common Directory
  dirname = g_strdup(COMMON_DIR);
  if((dirp = opendir(dirname)) == NULL){
    if(flag_print){
      g_print(_("   !!! Common Directory - %s - does not exist.\n"), dirname);
    }
  }
  else{
    if(flag_print){
      g_print(_("  [Common Directory]\n"));
    }
    if(chdir(dirname)!=0) exit_w_msg("ChDir error");

    while((entry = readdir(dirp))!=NULL){
      stat(entry->d_name, &statbuf);
      if(S_ISREG(statbuf.st_mode)){
	cp=entry->d_name;
	cp+=strlen(entry->d_name)-strlen(MENU_EXTENSION)+1;
	if(!strcmp(extp,cp)){ // 拡張子が .menuだったら
	  smenu->file[i_menu]=g_strdup(entry->d_name);
	  if(ScanMenu2(smenu,i_menu)){
	    if(flag_print){
	      if(smenu->flag_combine[i_menu]){
		g_print(_("  %20s *  %2d categories  -- Combined Menu --\n"),
			smenu->file[i_menu], smenu->num[i_menu]);
	      }
	      else{
		g_print(_("  %20s :  %2d mascots    [%s]\n"),
			smenu->file[i_menu], smenu->num[i_menu], 
			smenu->cat[i_menu]);
	      }
	    }

	    smenu->dir[i_menu]=SMENU_DIR_COMMON;
	    i_menu++;
	  }
	}
      }
    }
  }

  if(flag_print){
    g_print("\n");
  }
#endif

  // USER Directory
#ifdef USE_WIN32
  dirname = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,USER_DIR, NULL);
#else
  dirname = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,USER_DIR, NULL);
#endif
  if((dirp = opendir(dirname)) == NULL){
    if(flag_print){
      g_print(_("  !!! User Directory - %s - does not exist.\n"), dirname);
    }
  }
  else{
    if(flag_print){
      g_print(_("  [User Directory]\n"));
    }
    if(chdir(dirname)!=0) exit_w_msg("ChDir error");

    while((entry = readdir(dirp))!=NULL){
      stat(entry->d_name, &statbuf);
      if(S_ISREG(statbuf.st_mode)){
	cp=entry->d_name;
	cp+=strlen(entry->d_name)-strlen(MENU_EXTENSION)+1;
	if(!strcmp(extp,cp)){ // 拡張子が .menuだったら
	  smenu->file[i_menu]=g_strdup(entry->d_name);
	  if(ScanMenu2(smenu,i_menu,entry->d_name)){
	    if(flag_print){
	      if(smenu->flag_combine[i_menu]){
		g_print(_("  %20s *  %2d categories  -- Combined Menu --\n"),
			smenu->file[i_menu], smenu->num[i_menu]);
	      }
	      else{
		g_print(_("  %20s :  %2d mascots    [%s]\n"),
			smenu->file[i_menu], smenu->num[i_menu], 
			smenu->cat[i_menu]);
	      }
	    }
	    
	    smenu->dir[i_menu]=SMENU_DIR_USER;
	    i_menu++;
	  }
	}
      }
    }
  }

  smenu->max_menu=i_menu;

  if(chdir(cwdname)!=0) exit_w_msg("ChDir error");
  g_free(dirname);
  g_free(cwdname);

}


gboolean ScanMenu2(typScanMenu *smenu,gint i_menu)
{
  ConfigFile *cfgfile;
  int i_cat, i_tgt; 
  gchar tmp0[10], f_tmp0[10];
  gchar *dummy=NULL;
  gboolean flag_end=FALSE;
  gchar *menu_code;

  smenu->flag_combine[i_menu]=FALSE;
  smenu->num[i_menu]=0;

  cfgfile = xmms_cfg_open_file(smenu->file[i_menu]);

  if (cfgfile) {
    if(!xmms_cfg_read_string(cfgfile, "General", "code",&menu_code))
      menu_code = NULL;

    for(i_cat=0;i_cat<MAX_MENU_CATEGORY;i_cat++){

      sprintf(f_tmp0,"Menu%02d",i_cat);
      if(xmms_cfg_read_string(cfgfile, f_tmp0, "Name", &dummy)){
	if(i_cat==0){
	  if(dummy){
	    dummy=x_locale_to_utf8(dummy,-1,NULL,NULL,NULL,menu_code);
	    if(!dummy) dummy=g_strdup(_("(Invalid Character Code)"));
	  }
	  smenu->cat[i_menu]=g_strdup(dummy);
	}
      }
	
      for(i_tgt=0;i_tgt<MAX_MENU_TARGET;i_tgt++){
	sprintf(tmp0,"file%02d",i_tgt);
	if(!xmms_cfg_read_string(cfgfile, f_tmp0, tmp0, &dummy)){
	  if(i_tgt==0){
	    if(i_cat==0){
	      // Not Menu File
	      return(FALSE);
	    }
	    else if(i_cat==1){
	      // Simple Menu
	      flag_end=TRUE;
	      break;
	    }
	    else{
	      // Combined Menu
	      smenu->num[i_menu]=i_cat;
	      smenu->flag_combine[i_menu]=TRUE;
	      flag_end=TRUE;
	      break;
	    }
	  }
	  else{
	    smenu->num[i_menu]=i_tgt;

	    break;
	  }
	}
      }
      if(flag_end) break;
    }

    xmms_cfg_free(cfgfile);
    g_free(dummy);
  }
  return(TRUE);
}



// マスコットファイルの書き込み
void SaveMenu(typMascot *mascot)
{
  ConfigFile *cfgfile;
  gchar *filename;
  gchar tmp0[10], f_tmp0[10];
  int i_cat, i_tgt;
  gchar *tmp_conv=NULL;

  
  filename = g_strdup(mascot->menu_file);
  cfgfile = xmms_cfg_open_file(filename);
  //if (!cfgfile)  cfgfile = xmms_cfg_new();
  cfgfile = xmms_cfg_new();

  if(mascot->menu_code)
    xmms_cfg_write_string(cfgfile, "General", "code", mascot->menu_code);

  for(i_cat=0;i_cat<mascot->menu_cat_max;i_cat++){
     
    sprintf(f_tmp0,"Menu%02d",i_cat);
    if(mascot->menu_cat[i_cat]){
      tmp_conv=
	x_locale_from_utf8(mascot->menu_cat[i_cat],-1,NULL,NULL,NULL,mascot->menu_code);
      if(!tmp_conv) tmp_conv=g_strdup(_("(Invalid Character Code)"));
    }
    xmms_cfg_write_string(cfgfile, f_tmp0, "Name",tmp_conv);
    

    for(i_tgt=0;i_tgt<mascot->menu_tgt_max[i_cat];i_tgt++){
    
      sprintf(tmp0,"file%02d",i_tgt);
      xmms_cfg_write_string(cfgfile, f_tmp0, tmp0, 
			    mascot->menu_tgt[i_cat][i_tgt]);
    }

    
  }

  xmms_cfg_write_file(cfgfile, filename);
  xmms_cfg_free(cfgfile);

  g_free(filename);
  g_free(tmp_conv);
}


void InitDefCol(typMascot* mascot){
  // Default Color for GUI
#ifdef USE_GTK3
  mascot->def_colclk=g_malloc0(sizeof(GdkRGBA));
  mascot->def_colclksd=g_malloc0(sizeof(GdkRGBA));
  mascot->def_colclkbg=g_malloc0(sizeof(GdkRGBA));
  mascot->def_colclkbd=g_malloc0(sizeof(GdkRGBA));

  mascot->def_colbal=g_malloc0(sizeof(GdkRGBA));
  mascot->def_colbalbg=g_malloc0(sizeof(GdkRGBA));
  mascot->def_colbalbd=g_malloc0(sizeof(GdkRGBA));
  
  mascot->def_colclk=gdk_rgba_copy(&init_colclk);
  mascot->def_colclksd=gdk_rgba_copy(&init_colclksd);
  mascot->def_colclkbg=gdk_rgba_copy(&init_colclkbg);
  mascot->def_colclkbd=gdk_rgba_copy(&init_colclkbd);
  mascot->def_colbal=gdk_rgba_copy(&init_colbal);
  mascot->def_colbalbg=gdk_rgba_copy(&init_colbalbg);
  mascot->def_colbalbd=gdk_rgba_copy(&init_colbalbd);
  mascot->colfsfg0=gdk_rgba_copy(&color_black);
  mascot->colfsfg1=gdk_rgba_copy(&color_black);
  mascot->colfsbg0=gdk_rgba_copy(&color_pale3);
  mascot->colfsbg1=gdk_rgba_copy(&color_yellow3);
#else
  mascot->def_colclk=g_malloc0(sizeof(GdkColor));
  mascot->def_colclksd=g_malloc0(sizeof(GdkColor));
  mascot->def_colclkbg=g_malloc0(sizeof(GdkColor));
  mascot->def_colclkbd=g_malloc0(sizeof(GdkColor));

  mascot->def_colbal=g_malloc0(sizeof(GdkColor));
  mascot->def_colbalbg=g_malloc0(sizeof(GdkColor));
  mascot->def_colbalbd=g_malloc0(sizeof(GdkColor));

  mascot->def_colclk=gdk_color_copy(&init_colclk);
  mascot->def_colclksd=gdk_color_copy(&init_colclksd);
  mascot->def_colclkbg=gdk_color_copy(&init_colclkbg);
  mascot->def_colclkbd=gdk_color_copy(&init_colclkbd);
  mascot->def_colbal=gdk_color_copy(&init_colbal);
  mascot->def_colbalbg=gdk_color_copy(&init_colbalbg);
  mascot->def_colbalbd=gdk_color_copy(&init_colbalbd);
  mascot->colfsfg0=gdk_color_copy(&color_black);
  mascot->colfsfg1=gdk_color_copy(&color_black);
  mascot->colfsbg0=gdk_color_copy(&color_pale3);
  mascot->colfsbg1=gdk_color_copy(&color_yellow3);
#endif 

  mascot->def_alpclk  =DEF_ALPHA_OTHER;
  mascot->def_alpclkbg=DEF_ALPHA_CLK;
  mascot->def_alpclkbd=DEF_ALPHA_OTHER;
  mascot->def_alpclksd=DEF_ALPHA_SDW;
  mascot->def_alpbal  =DEF_ALPHA_OTHER;
  mascot->def_alpbalbg=DEF_ALPHA_BAL;
  mascot->def_alpbalbd=DEF_ALPHA_OTHER;
}



// 全体設定ファイルの読み込み
// あくまでもデフォルトなのでget_optionよりも前に実行する
// 起動時に決定 マスコットを変更しても保存されるパラメータ
void ReadRC(typMascot *mascot, gboolean def_flag)
{
  ConfigFile *cfgfile;
  gchar *filename;
  gchar *field_tmp=NULL;
  gint col_tmp;
  gint i_buf;
  gboolean b_buf;
  gchar *tmp;
  gchar *c_buf;

  // USER_DIRがないときは作成する  : おそらく初回起動時のみ
#ifdef USE_WIN32
  filename = g_strconcat(get_win_home(), G_DIR_SEPARATOR_S,USER_DIR, NULL);
#else
  filename = g_strconcat(g_get_home_dir(), G_DIR_SEPARATOR_S,USER_DIR, NULL);
#endif
  if (access(filename, F_OK) != 0) {
    tmp=g_strdup_printf(_("Creating User-Dir %s .\n"), filename);
    popup_message(NULL,
#ifdef USE_GTK3
		  "dialog-information", 
#else
		  GTK_STOCK_DIALOG_INFO,
#endif
		  POPUP_TIMEOUT,
		  tmp,
		  NULL);
    g_free(tmp);
#ifdef USE_WIN32
    mkdir(filename);
#else
    mkdir(filename,(S_IRWXU|S_IRGRP|S_IROTH));
#endif
  }
#ifdef USE_WIN32
  filename = g_strconcat(get_win_home(), G_DIR_SEPARATOR_S,USER_DIR, PIXDIR, NULL);
#else
  filename = g_strconcat(g_get_home_dir(), G_DIR_SEPARATOR_S,USER_DIR, PIXDIR, NULL);
#endif
  if (access(filename, F_OK) != 0) {
    tmp=g_strdup_printf(_("Creating User-PixmapDir %s .\n"),
			filename);
    popup_message(NULL,
#ifdef USE_GTK3
		  "dialog-information", 
#else
		  GTK_STOCK_DIALOG_INFO,
#endif
		  POPUP_TIMEOUT,
		  tmp,
		  NULL);
    g_free(tmp);
#ifdef USE_WIN32
    mkdir(filename);
#else
    mkdir(filename,(S_IRWXU|S_IRGRP|S_IROTH|S_IXGRP|S_IXOTH));
#endif
  }
#ifdef USE_WIN32
  filename = g_strconcat(get_win_home(), G_DIR_SEPARATOR_S,USER_DIR, SOUNDDIR, NULL);
#else
  filename = g_strconcat(g_get_home_dir(), G_DIR_SEPARATOR_S,USER_DIR, SOUNDDIR, NULL);
#endif
  if (access(filename, F_OK) != 0) {
    tmp=g_strdup_printf(_("Creating User-SoundDir %s .\n"),
			filename);
    popup_message(NULL,
#ifdef USE_GTK3
		  "dialog-information", 
#else
		  GTK_STOCK_DIALOG_INFO,
#endif
		  POPUP_TIMEOUT,
		  tmp,
		  NULL);
    g_free(tmp);
#ifdef USE_WIN32
    mkdir(filename);
#else
    mkdir(filename,(S_IRWXU|S_IRGRP|S_IROTH|S_IXGRP|S_IXOTH));
#endif
  }

  if(!mascot->rcfile){
#ifdef USE_WIN32
    mascot->rcfile = g_strconcat(get_win_home(), G_DIR_SEPARATOR_S,
				 USER_DIR,USER_RCFILE, NULL);
#else
    mascot->rcfile = g_strconcat(g_get_home_dir(), G_DIR_SEPARATOR_S,
				 USER_DIR,USER_RCFILE, NULL);
#endif
  }
  else{
    mascot->rcfile = FullPathRcFile(mascot->rcfile);
  }

  filename = g_strdup(mascot->rcfile);
  cfgfile = xmms_cfg_open_file(filename);

  
  if (cfgfile) {
    if(def_flag) field_tmp=g_strdup("Default-General");
    else         field_tmp=g_strdup("General");

    if(!xmms_cfg_read_boolean(cfgfile, field_tmp, "auto_install",
			      &mascot->flag_install))
      mascot->flag_install=FALSE;
    if(!xmms_cfg_read_boolean(cfgfile, field_tmp, "force_composite",
			      &mascot->force_composite))
      mascot->force_composite=FALSE;
    if(!xmms_cfg_read_boolean(cfgfile, field_tmp, "cairo_image",
			      &mascot->flag_img_cairo))
      mascot->flag_img_cairo=TRUE;
    if(!xmms_cfg_read_boolean(cfgfile, field_tmp, "cairo_balloon",
			      &mascot->flag_bal_cairo))
      mascot->flag_bal_cairo=TRUE;
    if(!xmms_cfg_read_boolean(cfgfile, field_tmp, "cairo_clock",
			      &mascot->flag_clk_cairo))
      mascot->flag_clk_cairo=TRUE;
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "magnify",
			  &mascot->magnify))
      mascot->magnify=100;
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "ip_style",
			  &mascot->ip_style))
      mascot->ip_style=MAG_IP_BILINEAR;
    if(!xmms_cfg_read_string(cfgfile, field_tmp, "font_clock",
			  &mascot->deffontname_clk))
      mascot->deffontname_clk=g_strdup(FONT_CLK);
    if(!xmms_cfg_read_string(cfgfile, field_tmp, "font_balloon",
			  &mascot->deffontname_bal))
      mascot->deffontname_bal=g_strdup(FONT_BAL);
    if(!xmms_cfg_read_string(cfgfile, field_tmp, "menu",
			  &mascot->menu_file))
      mascot->menu_file=NULL;
#ifndef USE_WIN32
    if(!xmms_cfg_read_string(cfgfile, field_tmp, "sound_command",
			  &mascot->sound_command))
      mascot->sound_command=NULL;
    if(mascot->url_command) g_free(mascot->url_command);
    if(xmms_cfg_read_string(cfgfile, field_tmp, "url_command",
			    &c_buf)){
      mascot->url_command=g_strdup(c_buf);
    }
    else{
      mascot->url_command=g_strdup(DEF_OPEN_URL);
    }
#endif
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "cons_check_mode",
			  &mascot->cons_check_mode))
      mascot->cons_check_mode=CONS_MANUAL;
#ifdef USE_GTAR
    if(!xmms_cfg_read_string(cfgfile, field_tmp, "tar_command",
			  &mascot->tar_command))
      mascot->tar_command=g_strdup(MENU_EXTRACT_GTAR_COMMAND);
#endif
#ifdef USE_GTK_STATUS_ICON
    if(!xmms_cfg_read_boolean(cfgfile, field_tmp, "tray_icon_flag",
			      &mascot->tray_icon_flag))
      mascot->tray_icon_flag=TRUE;
#endif
    

    // Color for Clock
    if(def_flag)  field_tmp=g_strdup("Default-ClockColor");
    else          field_tmp=g_strdup("ClockColor");

    mascot->def_colclk->red  =my_mascot_read_color(cfgfile, field_tmp, "text_r", init_colclk.red);
    mascot->def_colclk->green=my_mascot_read_color(cfgfile, field_tmp, "text_g", init_colclk.green);
    mascot->def_colclk->blue =my_mascot_read_color(cfgfile, field_tmp, "text_b", init_colclk.blue);
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "text_p", &col_tmp))
      col_tmp=DEF_ALPHA_OTHER;
    if(col_tmp>100){
      col_tmp=(gint)((gdouble)col_tmp*100.0/(gdouble)0xFFFF);
    }
    mascot->def_alpclk=col_tmp;
#ifdef  USE_GTK3
    mascot->def_colclk->alpha=(gdouble)mascot->def_alpclk/100.0;
#endif

    mascot->def_colclksd->red  =my_mascot_read_color(cfgfile, field_tmp, "shadow_r", init_colclksd.red);
    mascot->def_colclksd->green=my_mascot_read_color(cfgfile, field_tmp, "shadow_g", init_colclksd.green);
    mascot->def_colclksd->blue =my_mascot_read_color(cfgfile, field_tmp, "shadow_b", init_colclksd.blue);
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "shadow_p", &col_tmp))
      col_tmp=DEF_ALPHA_SDW;
    if(col_tmp>100){
      col_tmp=(gint)((gdouble)col_tmp*100.0/(gdouble)0xFFFF);
    }
    mascot->def_alpclksd=col_tmp;
#ifdef  USE_GTK3
    mascot->def_colclksd->alpha=(gdouble)mascot->def_alpclksd/100.0;
#endif

    mascot->def_colclkbg->red  =my_mascot_read_color(cfgfile, field_tmp, "bg_r", init_colclkbg.red);
    mascot->def_colclkbg->green=my_mascot_read_color(cfgfile, field_tmp, "bg_g", init_colclkbg.green);
    mascot->def_colclkbg->blue =my_mascot_read_color(cfgfile, field_tmp, "bg_b", init_colclkbg.blue);
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "bg_p", &col_tmp))
      col_tmp=DEF_ALPHA_CLK;
    if(col_tmp>100){
      col_tmp=(gint)((gdouble)col_tmp*100.0/(gdouble)0xFFFF);
    }
    mascot->def_alpclkbg=col_tmp;
#ifdef  USE_GTK3
    mascot->def_colclkbg->alpha=(gdouble)mascot->def_alpclkbg/100.0;
#endif


    mascot->def_colclkbd->red  =my_mascot_read_color(cfgfile, field_tmp, "border_r", init_colclkbd.red);
    mascot->def_colclkbd->green=my_mascot_read_color(cfgfile, field_tmp, "border_g", init_colclkbd.green);
    mascot->def_colclkbd->blue =my_mascot_read_color(cfgfile, field_tmp, "border_b", init_colclkbd.blue);
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "border_p", &col_tmp))
      col_tmp=DEF_ALPHA_OTHER;
    if(col_tmp>100){
      col_tmp=(gint)((gdouble)col_tmp*100.0/(gdouble)0xFFFF);
    }
    mascot->def_alpclkbd=col_tmp;
#ifdef  USE_GTK3
    mascot->def_colclkbd->alpha=(gdouble)mascot->def_alpclkbd/100.0;
#endif


    // Color for Balloon
    if(def_flag)  field_tmp=g_strdup("Default-BalloonColor");
    else          field_tmp=g_strdup("BalloonColor");

    mascot->def_colbal->red  =my_mascot_read_color(cfgfile, field_tmp, "text_r", init_colbal.green);
    mascot->def_colbal->green=my_mascot_read_color(cfgfile, field_tmp, "text_g", init_colbal.green);
    mascot->def_colbal->blue =my_mascot_read_color(cfgfile, field_tmp, "text_b", init_colbal.blue);
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "text_p", &col_tmp))
      col_tmp=DEF_ALPHA_OTHER;
    if(col_tmp>100){
      col_tmp=(gint)((gdouble)col_tmp*100.0/(gdouble)0xFFFF);
    }
    mascot->def_alpbal=col_tmp;
#ifdef  USE_GTK3
    mascot->def_colbal->alpha=(gdouble)mascot->def_alpbal/100.0;
#endif

    mascot->def_colbalbg->red  =my_mascot_read_color(cfgfile, field_tmp, "bg_r", init_colbalbg.red);
    mascot->def_colbalbg->green=my_mascot_read_color(cfgfile, field_tmp, "bg_g", init_colbalbg.green);
    mascot->def_colbalbg->blue =my_mascot_read_color(cfgfile, field_tmp, "bg_b", init_colbalbg.blue);
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "bg_p", &col_tmp))
      col_tmp=DEF_ALPHA_BAL;
    if(col_tmp>100){
      col_tmp=(gint)((gdouble)col_tmp*100.0/(gdouble)0xFFFF);
    }
    mascot->def_alpbalbg=col_tmp;
#ifdef  USE_GTK3
    mascot->def_colbalbg->alpha=(gdouble)mascot->def_alpbalbg/100.0;
#endif
    
    mascot->def_colbalbd->red  =my_mascot_read_color(cfgfile, field_tmp, "border_r", init_colbalbd.red);
    mascot->def_colbalbd->green=my_mascot_read_color(cfgfile, field_tmp, "border_g", init_colbalbd.green);
    mascot->def_colbalbd->blue =my_mascot_read_color(cfgfile, field_tmp, "border_b", init_colbalbd.blue);
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "border_p", &col_tmp))
      col_tmp=DEF_ALPHA_OTHER;
    if(col_tmp>100){
      col_tmp=(gint)((gdouble)col_tmp*100.0/(gdouble)0xFFFF);
    }
    mascot->def_alpbalbd=col_tmp;
#ifdef  USE_GTK3
    mascot->def_colbalbd->alpha=(gdouble)mascot->def_alpbalbd/100.0;
#endif


    // Focus Movement etc.
    if(def_flag) field_tmp=g_strdup("Default-Move");
    else         field_tmp=g_strdup("Move");

    if(!xmms_cfg_read_boolean(cfgfile, field_tmp, "relative_x", &b_buf)){
      mascot->flag_xp=FF_BAR_ABS;
    }
    else{
      if(b_buf){
	mascot->flag_xp=FF_BAR_REL;
      }
      else{
	mascot->flag_xp=FF_BAR_ABS;
      }
    }
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "offset",&mascot->offset))
      mascot->offset=0;
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "offsetp",&mascot->offsetp))
      mascot->offsetp=0;
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "ff_side",&mascot->ff_side))
      mascot->ff_side=FF_SIDE_RIGHT;
    if(!xmms_cfg_read_boolean(cfgfile, field_tmp, "raise_force",
			      &mascot->raise_force))
      mascot->raise_force=FALSE;
    if(!xmms_cfg_read_boolean(cfgfile, field_tmp, "raise_kwin",
			      &mascot->raise_kwin))
      mascot->raise_kwin=FALSE;
    if(xmms_cfg_read_int(cfgfile, field_tmp, "autobar2", &i_buf))
      mascot->focus_autobar=(guint)i_buf;
    else
      mascot->focus_autobar=AUTOBAR_ORDINAL;
#ifdef USE_WIN32
    if(!xmms_cfg_read_boolean(cfgfile, field_tmp, "task_force",
			      &mascot->task_force))
      mascot->task_force=FALSE;
#endif
#ifdef USE_OSX
    if(xmms_cfg_read_boolean(cfgfile, field_tmp, "mac_layer",
			     &i_buf)){
      mascot->mac_layer=i_buf;
    }
    else{
      mascot->mac_layer=MAC_LAYER_TOP;
    }
#endif
    if(!xmms_cfg_read_boolean(cfgfile, field_tmp, "no_capbar",
			      &mascot->no_capbar))
      mascot->no_capbar=FALSE;
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "bar_size",&mascot->bar_size))
      mascot->bar_size=0;
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "bar_offset",&mascot->bar_offset))
      mascot->bar_offset=0;
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "homepos_nf",&mascot->homepos_nf))
      mascot->homepos_nf=HOMEPOS_VANISH;
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "homepos_out",&mascot->homepos_out))
      mascot->homepos_out=HOMEPOS_NEVER;
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "homepos_nb",&mascot->homepos_nb))
      mascot->homepos_nb=HOMEPOS_NEVER;

    if(!xmms_cfg_read_boolean(cfgfile, field_tmp, "home_auto",
			      &mascot->home_auto))
      mascot->home_auto=TRUE;
    if(!xmms_cfg_read_boolean(cfgfile, field_tmp, "home_auto_vanish",
			      &mascot->home_auto_vanish))
      mascot->home_auto_vanish=FALSE;
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "home_x",&mascot->home_x))
      mascot->home_x=0;
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "home_y",&mascot->home_y))
      mascot->home_y=0;

    if(def_flag) field_tmp=g_strdup("Default-Shadow");
    else         field_tmp=g_strdup("Shadow");

    if(!xmms_cfg_read_boolean(cfgfile, field_tmp, "flag", &mascot->sdw_flag))
      mascot->sdw_flag=TRUE;
    if(!xmms_cfg_read_double(cfgfile, field_tmp, "x",&mascot->sdw_x))
      mascot->sdw_x=CAIRO_SHADOW_X;
    
    if(!xmms_cfg_read_double(cfgfile, field_tmp, "y",&mascot->sdw_y))
      mascot->sdw_y=CAIRO_SHADOW_Y;

    if(!xmms_cfg_read_int(cfgfile, field_tmp, "alpha",&mascot->sdw_alpha))
      mascot->sdw_alpha=DEF_ALPHA_SDW;

    // 時報
    if(def_flag) field_tmp=g_strdup("Default-Signal");
    else         field_tmp=g_strdup("Signal");

    if(!xmms_cfg_read_int(cfgfile, field_tmp, "type",&mascot->signal.type))
      mascot->signal.type=SIGACT_NO;
    if(!xmms_cfg_read_string(cfgfile, field_tmp, "command",&mascot->signal.com))
      mascot->signal.com=NULL;

#ifdef USE_BIFF
    // Biff
    if(def_flag) field_tmp=g_strdup("Default-Biff");
    else         field_tmp=g_strdup("Biff");

    if(!xmms_cfg_read_int(cfgfile, field_tmp, "type",&mascot->mail.type))
      mascot->mail.type=MAIL_NO;
    if(!xmms_cfg_read_string(cfgfile, field_tmp, "pop-id",
			      &mascot->mail.pop_id))
      mascot->mail.pop_id=NULL;
    if(!xmms_cfg_read_boolean(cfgfile, field_tmp, "pop-save",
			      &mascot->mail.pop_save))
      mascot->mail.pop_save=TRUE;
    if(mascot->mail.pop_save){
      if(!xmms_cfg_read_string(cfgfile, field_tmp, "pop-pass",
			       &mascot->mail.pop_pass))
	mascot->mail.pop_pass=NULL;
    }
    if(!xmms_cfg_read_string(cfgfile, field_tmp, "pop-server",
			     &mascot->mail.pop_server))
      mascot->mail.pop_server=NULL;
#ifdef USE_SSL
    if(xmms_cfg_read_int(cfgfile, field_tmp, "ssl-mode",&i_buf)){
      mascot->mail.ssl_mode=(guint)i_buf;
    }
    else{
      mascot->mail.ssl_mode=SSL_NONE;
    }
#else
    mascot->mail.ssl_mode=SSL_NONE;
#endif
#ifdef USE_SSL
    if(!xmms_cfg_read_boolean(cfgfile, field_tmp, "ssl-cert-skip",
			      &mascot->mail.ssl_cert_skip))
      mascot->mail.ssl_cert_skip=FALSE;
#else
    mascot->mail.ssl_cert_skip=FALSE;
#endif
#ifdef USE_SSL
    if(!xmms_cfg_read_boolean(cfgfile, field_tmp, "ssl-nonblock",
			      &mascot->mail.ssl_nonblock))
      mascot->mail.ssl_nonblock=TRUE;
#else
    mascot->mail.ssl_nonblock=TRUE;
#endif
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "pop-port",
			  &mascot->mail.pop_port)){
#ifdef USE_SSL
      if(mascot->mail.ssl_mode==SSL_TUNNEL){
	mascot->mail.pop_port=POP3_SSL_PORT_NO;
      }
      else{
	mascot->mail.pop_port=POP3_PORT_NO;
      }
#else
      mascot->mail.pop_port=POP3_PORT_NO;
#endif
    }
    if(!xmms_cfg_read_string(cfgfile, field_tmp, "file",
			     &mascot->mail.file))
      mascot->mail.file=NULL;
    if(!xmms_cfg_read_string(cfgfile, field_tmp, "polling",
			     &mascot->mail.polling))
      mascot->mail.polling=NULL;
    if(!xmms_cfg_read_string(cfgfile, field_tmp, "mailer",
			     &mascot->mail.mailer))
      mascot->mail.mailer=NULL;
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "interval",
			     &mascot->mail.interval))
      mascot->mail.interval=DEF_MAIL_INTERVAL;
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "win-width",
			     &mascot->mail.win_width))
      mascot->mail.win_width=BIFF_WIN_WIDTH;
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "win-height",
			     &mascot->mail.win_height))
      mascot->mail.win_height=BIFF_WIN_HEIGHT;
    if(!xmms_cfg_read_boolean(cfgfile, field_tmp, "spam-check",
			     &mascot->mail.spam_check))
      mascot->mail.spam_check=SPAM_CHECK;
    if(!xmms_cfg_read_string(cfgfile, field_tmp, "spam-mark",
			     &mascot->mail.spam_mark))
      mascot->mail.spam_mark=g_strdup(SPAM_M_SPAMASSASIN);
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "pop-max-fs",
			     &mascot->mail.pop_max_fs))
      mascot->mail.pop_max_fs=POP3_MAX_FS;
    if(!xmms_cfg_read_boolean(cfgfile, field_tmp, "tooltips",
			     &mascot->mail.tooltips_fl))
      mascot->mail.tooltips_fl=BIFF_TOOLTIPS;

    mascot->colfsfg0->red  =my_mascot_read_color(cfgfile, field_tmp, "fg0_r", color_black.red);
    mascot->colfsfg0->green=my_mascot_read_color(cfgfile, field_tmp, "fg0_g", color_black.green);
    mascot->colfsfg0->blue =my_mascot_read_color(cfgfile, field_tmp, "fg0_b", color_black.blue);
#ifdef  USE_GTK3
    mascot->colfsfg0->alpha=1.0;
#endif

    mascot->colfsfg1->red  =my_mascot_read_color(cfgfile, field_tmp, "fg1_r", color_black.red);
    mascot->colfsfg1->green=my_mascot_read_color(cfgfile, field_tmp, "fg1_g", color_black.green);
    mascot->colfsfg1->blue =my_mascot_read_color(cfgfile, field_tmp, "fg1_b", color_black.blue);
#ifdef  USE_GTK3
    mascot->colfsfg0->alpha=1.0;
#endif
    
    mascot->colfsbg0->red  =my_mascot_read_color(cfgfile, field_tmp, "bg0_r", color_pale3.red);
    mascot->colfsbg0->green=my_mascot_read_color(cfgfile, field_tmp, "bg0_g", color_pale3.green);
    mascot->colfsbg0->blue =my_mascot_read_color(cfgfile, field_tmp, "bg0_b", color_pale3.blue);
#ifdef  USE_GTK3
    mascot->colfsbg0->alpha=1.0;
#endif

    mascot->colfsbg1->red  =my_mascot_read_color(cfgfile, field_tmp, "bg1_r", color_yellow3.red);
    mascot->colfsbg1->green=my_mascot_read_color(cfgfile, field_tmp, "bg1_g", color_yellow3.green);
    mascot->colfsbg1->blue =my_mascot_read_color(cfgfile, field_tmp, "bg1_b", color_yellow3.blue);
#ifdef  USE_GTK3
    mascot->colfsbg1->alpha=1.0;
#endif
    
#endif  // USE_BIFF

#ifdef USE_SOCKMSG
    // デュエットアニメ
    if(def_flag) field_tmp=g_strdup("Default-Duet");
    else         field_tmp=g_strdup("Duet");

    if(!xmms_cfg_read_boolean(cfgfile, field_tmp, "use_click",
			  &mascot->duet_use_click))
      mascot->duet_use_click=TRUE;
    if(!xmms_cfg_read_boolean(cfgfile, field_tmp, "use_random",
			      &mascot->duet_use_random))
      mascot->duet_use_random=TRUE;
#endif  // USE_SOCKMSG
    

#ifdef USE_SOCKMSG
    // デュエットアニメ
    if(def_flag) field_tmp=g_strdup("Default-Sockmsg");
    else         field_tmp=g_strdup("Sockmsg");

    if(!xmms_cfg_read_int(cfgfile, field_tmp, "type",
			  &mascot->sockmsg_type))
      mascot->sockmsg_type=SOCK_NORMAL;
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "interval",
			  &mascot->sockmsg_step))
      mascot->sockmsg_step=SOCK_INTERVAL;
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "expire_def",
			  &mascot->sockmsg_expire_def))
      mascot->sockmsg_expire_def=(SYS_BALLOON_EXPIRE)*(INTERVAL);
#endif  // USE_SOCKMSG
    

    // Alpha Percentage
    if(def_flag)  field_tmp=g_strdup("Default-Alpha");
    else          field_tmp=g_strdup("Alpha");

    if(!xmms_cfg_read_boolean(cfgfile, field_tmp, "force_def",&mascot->force_def_alpha))
      mascot->force_def_alpha=FALSE;
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "main",&mascot->def_alpha_main))
      mascot->def_alpha_main=DEF_ALPHA_MAIN;
#ifdef USE_BIFF
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "biff",&mascot->def_alpha_biff))
      mascot->def_alpha_biff=DEF_ALPHA_BIFF;
#endif

#ifdef FG_DRAW
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "balloon",&mascot->def_alpha_bal))
      mascot->def_alpha_bal=DEF_ALPHA_BAL;
    if(!xmms_cfg_read_boolean(cfgfile,field_tmp,"balloonfg",&mascot->def_flag_balfg))
      mascot->def_flag_balfg=TRUE;
    if(!xmms_cfg_read_int(cfgfile, field_tmp, "clock",&mascot->def_alpha_clk))
      mascot->def_alpha_clk=DEF_ALPHA_CLK;
    if(!xmms_cfg_read_boolean(cfgfile,field_tmp,"clockfg",&mascot->def_flag_clkfg))
      mascot->def_flag_clkfg=TRUE;
#endif

    xmms_cfg_free(cfgfile);
  }
  else{
    // USER_RCFILEがないときは作成する : おそらく初回起動時のみ
    mascot->flag_install=FALSE;
    mascot->force_composite=FALSE;
    mascot->flag_img_cairo=TRUE;
    mascot->flag_bal_cairo=TRUE;
    mascot->flag_clk_cairo=TRUE;
    mascot->flag_ow=FALSE;
    mascot->flag_ow_ini=FALSE;
    mascot->magnify=100;
    mascot->ip_style=MAG_IP_BILINEAR;
    mascot->deffontname_clk=g_strdup(FONT_CLK);
    mascot->deffontname_bal=g_strdup(FONT_BAL);
    mascot->cons_check_mode=CONS_MANUAL;
#ifdef USE_GTAR
    mascot->tar_command=g_strdup(MENU_EXTRACT_GTAR_COMMAND);
#endif
#ifdef USE_GTK_STATUS_ICON
    mascot->tray_icon_flag=TRUE;
#endif

    mascot->flag_xp=FF_BAR_ABS;
    mascot->offset=0;
    mascot->offsetp=0;
    mascot->ff_side=FF_SIDE_RIGHT;
    mascot->raise_force=FALSE;
    mascot->raise_kwin=FALSE;
    mascot->focus_autobar=AUTOBAR_ORDINAL;
#ifdef USE_WIN32
    mascot->task_force=FALSE;
#endif
#ifdef USE_OSX
    mascot->mac_layer=MAC_LAYER_TOP;
#endif
    mascot->no_capbar=FALSE;
    mascot->bar_size=0;
    mascot->bar_offset=0;
    mascot->homepos_nf=HOMEPOS_VANISH;
    mascot->homepos_out=HOMEPOS_NEVER;
    mascot->homepos_nb=HOMEPOS_NEVER;
    mascot->home_auto=TRUE;
    mascot->home_auto_vanish=FALSE;
    mascot->home_x=0;
    mascot->home_y=0;

    mascot->sdw_flag=TRUE;
    mascot->sdw_x=CAIRO_SHADOW_X;
    mascot->sdw_y=CAIRO_SHADOW_Y;
    mascot->sdw_alpha=DEF_ALPHA_SDW;
    
    mascot->signal.type=SIGACT_NO;
    mascot->signal.com=NULL;

#ifdef USE_BIFF
    mascot->mail.type=MAIL_NO;
    mascot->mail.pop_id=NULL;
    mascot->mail.pop_save=TRUE;
    mascot->mail.pop_pass=NULL;
    mascot->mail.pop_server=NULL;
    mascot->mail.file=NULL;
    mascot->mail.ssl_mode=SSL_NONE;
    mascot->mail.ssl_cert_skip=FALSE;
    mascot->mail.ssl_nonblock=TRUE;
    mascot->mail.pop_port=POP3_PORT_NO;
    mascot->mail.polling=NULL;
    mascot->mail.mailer=NULL;
    mascot->mail.interval=DEF_MAIL_INTERVAL;
    mascot->mail.win_width=BIFF_WIN_WIDTH;
    mascot->mail.win_height=BIFF_WIN_HEIGHT;
    mascot->mail.spam_check=SPAM_CHECK;
    mascot->mail.spam_mark=g_strdup(SPAM_M_SPAMASSASIN);
    mascot->mail.pop_max_fs=POP3_MAX_FS;
    mascot->mail.tooltips_fl=BIFF_TOOLTIPS;
#endif  // USE_BIFF

#ifdef USE_SOCKMSG
    mascot->duet_use_click=TRUE;
    mascot->duet_use_random=TRUE;

    mascot->sockmsg_type=SOCK_NORMAL;
    mascot->sockmsg_step=SOCK_INTERVAL;
    mascot->sockmsg_expire_def=(SYS_BALLOON_EXPIRE)*(INTERVAL);
#endif  // USE_SOCKMSG

    mascot->force_def_alpha=FALSE;
    mascot->def_alpha_main=DEF_ALPHA_MAIN;
#ifdef USE_BIFF
    mascot->def_alpha_biff=DEF_ALPHA_BIFF;
#endif
#ifdef FG_DRAW
    mascot->def_alpha_clk=DEF_ALPHA_CLK;
    mascot->def_flag_clkfg=TRUE;
    mascot->def_alpha_bal=DEF_ALPHA_BAL;
    mascot->def_flag_balfg=TRUE;
#endif
    
    SaveRC(mascot,TRUE);
    SaveRC(mascot,FALSE);
  }
  g_free(field_tmp);
  g_free(filename);

}

// 全体設定ファイルの書き込み
void SaveRC(typMascot *mascot,  gboolean def_flag)
{
  ConfigFile *cfgfile;
  gchar *filename;
  gchar *field_tmp=NULL;

  filename = g_strdup(mascot->rcfile);
  cfgfile = xmms_cfg_open_file(filename);
  if (!cfgfile)  cfgfile = xmms_cfg_new();

  
  // General Setup
  if(def_flag) field_tmp=g_strdup("Default-General");
  else         field_tmp=g_strdup("General");
  xmms_cfg_write_string(cfgfile, field_tmp, "prog_ver",VERSION);
  xmms_cfg_write_boolean(cfgfile, field_tmp, "auto_install",mascot->flag_install);
  xmms_cfg_write_boolean(cfgfile, field_tmp, "force_composite",mascot->force_composite);
  xmms_cfg_write_boolean(cfgfile, field_tmp, "cairo_image",mascot->flag_img_cairo);
  xmms_cfg_write_boolean(cfgfile, field_tmp, "cairo_balloon",mascot->flag_bal_cairo);
  xmms_cfg_write_boolean(cfgfile, field_tmp, "cairo_clock",mascot->flag_clk_cairo);
  xmms_cfg_write_int(cfgfile, field_tmp, "magnify",mascot->magnify);
  xmms_cfg_write_int(cfgfile, field_tmp, "ip_style",mascot->ip_style);
  xmms_cfg_write_string(cfgfile, field_tmp, "font_clock",mascot->deffontname_clk);
  xmms_cfg_write_string(cfgfile, field_tmp, "font_balloon",mascot->deffontname_bal);
  //if(mascot->menu_file) xmms_cfg_write_string(cfgfile, field_tmp, "menu",
  //					      mascot->menu_file);
  //else     xmms_cfg_remove_key(cfgfile,field_tmp, "menu");
#ifndef USE_WIN32
  if(mascot->sound_command) 
    xmms_cfg_write_string(cfgfile, field_tmp, "sound_command",mascot->sound_command);
  else     xmms_cfg_remove_key(cfgfile,field_tmp, "sound_command");
  if(mascot->url_command) 
    xmms_cfg_write_string(cfgfile, field_tmp, "url_command",mascot->url_command);
  else    xmms_cfg_remove_key(cfgfile,field_tmp, "url_command");
  xmms_cfg_write_int(cfgfile, field_tmp, "cons_check_mode",mascot->cons_check_mode);
#endif
#ifdef USE_GTAR
  if(mascot->tar_command) 
    xmms_cfg_write_string(cfgfile, field_tmp, "tar_command",mascot->tar_command);
  else     xmms_cfg_remove_key(cfgfile,field_tmp, "tar_command");
#endif
#ifdef USE_GTK_STATUS_ICON
  xmms_cfg_write_boolean(cfgfile, field_tmp, "tray_icon_flag",mascot->tray_icon_flag);
#endif

  // Color for Clock
  if(def_flag) field_tmp=g_strdup("Default-ClockColor");
  else         field_tmp=g_strdup("ClockColor");

  my_mascot_write_color(cfgfile, field_tmp,
			"text_r", "text_g", "text_b", "text_p",
			mascot->def_colclk, &init_colclk,
			mascot->def_alpclk, DEF_ALPHA_OTHER);

  my_mascot_write_color(cfgfile, field_tmp,
			"shadow_r", "shadow_g", "shadow_b", "shadow_p",
			mascot->def_colclksd, &init_colclksd,
			mascot->def_alpclksd, DEF_ALPHA_SDW);

  my_mascot_write_color(cfgfile, field_tmp,
			"bg_r", "bg_g", "bg_b", "bg_p",
			mascot->def_colclkbg, &init_colclkbg,
			mascot->def_alpclkbg, DEF_ALPHA_CLK);

  my_mascot_write_color(cfgfile, field_tmp,
			"border_r", "border_g", "border_b", "border_p",
			mascot->def_colclkbd, &init_colclkbd,
			mascot->def_alpclkbd, DEF_ALPHA_OTHER);

  // Color for Balloon
  if(def_flag) field_tmp=g_strdup("Default-BalloonColor");
  else         field_tmp=g_strdup("BalloonColor");

  my_mascot_write_color(cfgfile, field_tmp,
			"text_r", "text_g", "text_b", "text_p",
			mascot->def_colbal, &init_colbal,
			mascot->def_alpbal, DEF_ALPHA_OTHER);

  my_mascot_write_color(cfgfile, field_tmp,
			"bg_r", "bg_g", "bg_b", "bg_p",
			mascot->def_colbalbg, &init_colbalbg,
			mascot->def_alpbalbg, DEF_ALPHA_BAL);

  my_mascot_write_color(cfgfile, field_tmp,
			"border_r", "border_g", "border_b", "border_p",
			mascot->def_colbalbd, &init_colbalbd,
			mascot->def_alpbalbd, DEF_ALPHA_OTHER);

  // Focus Movement etc.

  if(def_flag) field_tmp=g_strdup("Default-Move");
  else         field_tmp=g_strdup("Move");
  if(mascot->flag_xp==FF_BAR_ABS){
    xmms_cfg_write_boolean(cfgfile, field_tmp, "relative_x",FALSE);
  }
  else{
    xmms_cfg_write_boolean(cfgfile, field_tmp, "relative_x",TRUE);
  }
  xmms_cfg_write_int(cfgfile, field_tmp, "offset",mascot->offset);
  xmms_cfg_write_int(cfgfile, field_tmp, "offsetp",mascot->offsetp);
  xmms_cfg_write_int(cfgfile, field_tmp, "ff_side",mascot->ff_side);

  xmms_cfg_write_boolean(cfgfile, field_tmp, "raise_force",mascot->raise_force);
  xmms_cfg_write_boolean(cfgfile, field_tmp, "raise_kwin",mascot->raise_kwin);
  xmms_cfg_write_int(cfgfile, field_tmp, "autobar2",mascot->focus_autobar);
#ifdef USE_WIN32
  xmms_cfg_write_boolean(cfgfile, field_tmp, "task_force",mascot->task_force);
#endif
#ifdef USE_OSX
  xmms_cfg_write_boolean(cfgfile, field_tmp, "mac_layer",mascot->mac_layer);
#endif
  xmms_cfg_write_boolean(cfgfile, field_tmp, "no_capbar",mascot->no_capbar);
  xmms_cfg_write_int(cfgfile, field_tmp, "bar_size",mascot->bar_size);
  xmms_cfg_write_int(cfgfile, field_tmp, "bar_offset",mascot->bar_offset);
  xmms_cfg_write_int(cfgfile, field_tmp, "homepos_nf",mascot->homepos_nf);
  xmms_cfg_write_int(cfgfile, field_tmp, "homepos_out",mascot->homepos_out);
  xmms_cfg_write_int(cfgfile, field_tmp, "homepos_nb",mascot->homepos_nb);
  xmms_cfg_remove_key(cfgfile,field_tmp, "homepos");
  xmms_cfg_write_boolean(cfgfile, field_tmp, "home_auto",mascot->home_auto);
  xmms_cfg_write_boolean(cfgfile, field_tmp, "home_auto_vanish",mascot->home_auto_vanish);
  xmms_cfg_write_int(cfgfile, field_tmp, "home_x",mascot->home_x);
  xmms_cfg_write_int(cfgfile, field_tmp, "home_y",mascot->home_y);

  // Shadow with Cairo

  if(def_flag) field_tmp=g_strdup("Default-Shadow");
  else         field_tmp=g_strdup("Shadow");
  xmms_cfg_write_boolean(cfgfile, field_tmp, "flag",mascot->sdw_flag);
  xmms_cfg_write_double2(cfgfile, field_tmp, "x",mascot->sdw_x, "%.2lf");
  xmms_cfg_write_double2(cfgfile, field_tmp, "y",mascot->sdw_y, "%.2lf");
  xmms_cfg_write_int(cfgfile, field_tmp, "alpha",mascot->sdw_alpha);


  // 時報
  if(def_flag) field_tmp=g_strdup("Default-Signal");
  else         field_tmp=g_strdup("Signal");

  xmms_cfg_write_int(cfgfile, field_tmp, "type",mascot->signal.type);
  if(mascot->signal.com)
    xmms_cfg_write_string(cfgfile, field_tmp, "command",mascot->signal.com);
  else xmms_cfg_remove_key(cfgfile,field_tmp, "command");

#ifdef USE_BIFF
  // Biff

  if(def_flag) field_tmp=g_strdup("Default-Biff");
  else         field_tmp=g_strdup("Biff");

  xmms_cfg_write_int(cfgfile, field_tmp, "type",mascot->mail.type);
  if(mascot->mail.pop_id)
    xmms_cfg_write_string(cfgfile, field_tmp, "pop-id",mascot->mail.pop_id);
  else xmms_cfg_remove_key(cfgfile,field_tmp, "pop-id");

  xmms_cfg_write_boolean(cfgfile, field_tmp, "pop-save",
			 mascot->mail.pop_save);
  if(mascot->mail.pop_save){
    if(mascot->mail.pop_pass)
      xmms_cfg_write_string(cfgfile, field_tmp, "pop-pass",
			    mascot->mail.pop_pass);
    else xmms_cfg_remove_key(cfgfile,field_tmp, "pop-pass");
  }
  else{
    xmms_cfg_remove_key(cfgfile,field_tmp, "pop-pass");
  }
  if(mascot->mail.pop_server)
    xmms_cfg_write_string(cfgfile, field_tmp, "pop-server",
			  mascot->mail.pop_server);
  else xmms_cfg_remove_key(cfgfile,field_tmp, "pop-server");
#ifdef USE_SSL
  xmms_cfg_write_int(cfgfile, field_tmp, "ssl-mode",mascot->mail.ssl_mode);
  xmms_cfg_write_boolean(cfgfile, field_tmp, "ssl-cert-skip",mascot->mail.ssl_cert_skip);
  xmms_cfg_write_boolean(cfgfile, field_tmp, "ssl-nonblock",mascot->mail.ssl_nonblock);
#endif
  if(mascot->mail.pop_port!=POP3_PORT_NO)
    xmms_cfg_write_int(cfgfile, field_tmp, "pop-port",
		       mascot->mail.pop_port);
  else xmms_cfg_remove_key(cfgfile,field_tmp, "pop-port");
  if(mascot->mail.file)
    xmms_cfg_write_string(cfgfile, field_tmp, "file",
			  mascot->mail.file);
  else xmms_cfg_remove_key(cfgfile,field_tmp, "file");
  if(mascot->mail.polling)
    xmms_cfg_write_string(cfgfile, field_tmp, "polling",
			  mascot->mail.polling);
  else xmms_cfg_remove_key(cfgfile,field_tmp, "polling");
  if(mascot->mail.mailer)
    xmms_cfg_write_string(cfgfile, field_tmp, "mailer",
			  mascot->mail.mailer);
  else xmms_cfg_remove_key(cfgfile,field_tmp, "mailer");
  xmms_cfg_write_int(cfgfile, field_tmp, "interval",mascot->mail.interval);
  xmms_cfg_write_int(cfgfile, field_tmp, "win-width",mascot->mail.win_width);
  xmms_cfg_write_int(cfgfile, field_tmp, "win-height",mascot->mail.win_height);
  xmms_cfg_write_boolean(cfgfile, field_tmp, "spam-check",mascot->mail.spam_check);
  if(mascot->mail.spam_mark)
    xmms_cfg_write_string(cfgfile, field_tmp, "spam-mark",
			  mascot->mail.spam_mark);
  else xmms_cfg_remove_key(cfgfile,field_tmp, "spam-mark");
  xmms_cfg_write_int(cfgfile, field_tmp, "pop-max-fs",mascot->mail.pop_max_fs);
  xmms_cfg_write_boolean(cfgfile, field_tmp, "tooltips",mascot->mail.tooltips_fl);

  my_mascot_write_color_woa(cfgfile, field_tmp,
			    "fg0_r", "fg0_g", "fg0_b",
			    mascot->colfsfg0, &color_black);
  my_mascot_write_color_woa(cfgfile, field_tmp,
			    "fg1_r", "fg1_g", "fg1_b",
			    mascot->colfsfg1, &color_black);
  my_mascot_write_color_woa(cfgfile, field_tmp,
			    "bg0_r", "bg0_g", "bg0_b",
			    mascot->colfsbg0, &color_pale3);
  my_mascot_write_color_woa(cfgfile, field_tmp,
			    "bg1_r", "bg1_g", "bg1_b",
			    mascot->colfsbg1, &color_yellow3);
#endif

#ifdef USE_SOCKMSG
  // Duetアニメ
  if(def_flag) field_tmp=g_strdup("Default-Duet");
  else         field_tmp=g_strdup("Duet");

  xmms_cfg_write_boolean(cfgfile, field_tmp, "use_click",
			 mascot->duet_use_click);
  xmms_cfg_write_boolean(cfgfile, field_tmp, "use_random",
			 mascot->duet_use_random);
#endif  // USE_SOCKMSG

#ifdef USE_SOCKMSG
  // Sockmsg
  if(def_flag) field_tmp=g_strdup("Default-Sockmsg");
  else         field_tmp=g_strdup("Sockmsg");

  xmms_cfg_write_int(cfgfile, field_tmp, "type", mascot->sockmsg_type);
  xmms_cfg_write_int(cfgfile, field_tmp, "interval", mascot->sockmsg_step);
  xmms_cfg_write_int(cfgfile, field_tmp, "expire_def", 
		     mascot->sockmsg_expire_def);
#endif  // USE_SOCKMSG

    // Alpha Percentage
  if(def_flag)  field_tmp=g_strdup("Default-Alpha");
  else          field_tmp=g_strdup("Alpha");
    
  xmms_cfg_write_boolean(cfgfile, field_tmp, "force_def",
			 mascot->force_def_alpha);
  xmms_cfg_write_int(cfgfile, field_tmp, "main",mascot->def_alpha_main);
#ifdef USE_BIFF
  xmms_cfg_write_int(cfgfile, field_tmp, "biff",mascot->def_alpha_biff);
#endif
#ifdef FG_DRAW
  xmms_cfg_write_int(cfgfile, field_tmp, "balloon",mascot->def_alpha_bal);
  xmms_cfg_write_int(cfgfile, field_tmp, "clock",mascot->def_alpha_clk);
#endif


  xmms_cfg_write_file(cfgfile, filename);
  xmms_cfg_free(cfgfile);


  

  // POPユーザー情報を書き込む予定があるためデフォルトでユーザーのみの
  // rwパーミッション( chmod 600 )とする
#ifndef USE_WIN32
  if((chmod(filename,(S_IRUSR | S_IWUSR ))) != 0){
    g_print("Cannot Chmod Resource File %s!  Please check!!!\n",filename);
  }
#endif

  g_free(field_tmp);
  g_free(filename);
}


// デフォルトメニューをRC-fileに保存
void SaveDefMenu(typMascot *mascot,  gboolean def_flag)
{
  ConfigFile *cfgfile;
  gchar *filename;
  gchar *field_tmp=NULL;

  filename = g_strdup(mascot->rcfile);
  cfgfile = xmms_cfg_open_file(filename);
  if (!cfgfile)  cfgfile = xmms_cfg_new();

  
  // General Setup
  if(def_flag) field_tmp=g_strdup("Default-General");
  else         field_tmp=g_strdup("General");
  if(mascot->menu_file) xmms_cfg_write_string(cfgfile, field_tmp, "menu",
  					      mascot->menu_file);
  else     xmms_cfg_remove_key(cfgfile,field_tmp, "menu");


  xmms_cfg_write_file(cfgfile, filename);
  xmms_cfg_free(cfgfile);


  // POPユーザー情報を書き込む予定があるためデフォルトでユーザーのみの
  // rwパーミッション( chmod 600 )とする
#ifndef USE_WIN32
  if((chmod(filename,(S_IRUSR | S_IWUSR ))) != 0){
    g_print("Cannot Chmod Resource File %s!  Please check!!!\n",filename);
  }
#endif


  g_free(field_tmp);
  g_free(filename);
}


// マスコットファイルの読み込み
void ReadMascot(typMascot *mascot, gboolean def_flag)
{
  ConfigFile *cfgfile;
  gchar *filename=NULL, *filename2=NULL, *filename0=NULL;
  gchar tmp[64], tmp0[64];
  int i_pix=0, i_ptn=0, i_frm=0, i_pix2=0;
  gint i_tmp;
  gchar *f_tmp0=NULL;
  gint place=0;
  gboolean install_flag=FALSE;
  struct stat statbuf;
  time_t common_mtime=0,user_mtime=0;
  gint col_tmp;
  gchar *tmp_conv=NULL;
  gboolean flag_def_col=FALSE;
  gchar *c_buf;
  gint nPix;

  mascot->random_total=0;
  mascot->click_total=0;
  mascot->flag_consow=FALSE;

  
  /// マスコットファイルを順番で探す

  // 2. Currentから
  if(place==0){
    filename = g_strdup(mascot->file);
    if(access(filename,F_OK)==0){
      place=2;
    }
  }
  
  if(mascot->flag_ow){
#ifdef USE_COMMON
    // 4.  Common Directory
    if(place==0){
      if(filename) g_free(filename);
      filename = g_strconcat(COMMON_DIR,my_basename(mascot->file), NULL);
      if(access(filename,F_OK)==0){
	place=4;
      }
    }
#endif

    // 1. メニューと同じ場所
    if(place==0){
      if(mascot->menu_file){
	if(filename) g_free(filename);
	filename = g_strconcat(my_dirname(mascot->menu_file),G_DIR_SEPARATOR_S,
			       my_basename(mascot->file),NULL);
	if(access(filename,F_OK)==0){
	  place=1;
	}
      }
    }
    
    // 3. USER Directory
    if(place==0){
      if(filename) g_free(filename);
#ifdef USE_WIN32
      filename = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,USER_DIR, 
			     my_basename(mascot->file), NULL);
#else
      filename = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,USER_DIR, 
			     my_basename(mascot->file), NULL);
#endif
      if(access(filename,F_OK)==0){
	place=3;
      }
    }
  }
  else{
    // 1. メニューと同じ場所
    if(place==0){
      if(mascot->menu_file){
	if(filename) g_free(filename);
	filename = g_strconcat(my_dirname(mascot->menu_file),G_DIR_SEPARATOR_S,
			       my_basename(mascot->file),NULL);
	if(access(filename,F_OK)==0){
	  place=1;
	}
      }
    }
    
    // 3. USER Directory
    if(place==0){
      if(filename) g_free(filename);
#ifdef USE_WIN32
      filename = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,USER_DIR, 
			     my_basename(mascot->file), NULL);
#else
      filename = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,USER_DIR, 
			     my_basename(mascot->file), NULL);
#endif
      if(access(filename,F_OK)==0){
	place=3;
      }
    }
  
#ifdef USE_COMMON
    // 4.  Common Directory
    if(place==0){
      if(filename) g_free(filename);
      filename = g_strconcat(COMMON_DIR,my_basename(mascot->file), NULL);
      if(access(filename,F_OK)==0){
	  place=4;
      }
    }
#endif
  }

  if(place==0){
    popup_message(mascot->win_main,
#ifdef USE_GTK3
		  "dialog-error", 
#else
		  GTK_STOCK_DIALOG_ERROR,
#endif
		  -1,
		  _("Cannot Find Mascot File."),
		  " ",
		  my_basename(to_utf8(mascot->file)),
		  NULL);
    exit(1);
  }

  
  // マスコットのUser Directoryへのインストール
  if((place==1)||(place==2)||(place==4)){
    if((mascot->flag_install)&&(!FlagInstalledMenu)){
#ifdef USE_COMMON
      if(mascot->flag_common){
	check_common_dir();
	if(filename2) g_free(filename2);
	filename2 = g_strconcat(COMMON_DIR, 
				my_basename(mascot->file), NULL);
      }
      else{
#endif
	if(filename2) g_free(filename2);
#ifdef USE_WIN32
	filename2 = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,USER_DIR, 
				my_basename(mascot->file), NULL);
#else
	filename2 = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,USER_DIR, 
				my_basename(mascot->file), NULL);
#endif
#ifdef USE_COMMON
      }
#endif
      if((access(filename2,F_OK)!=0)||(mascot->flag_ow)){
	if(strcmp(filename,filename2)!=0){
	  //g_print(_("Installing %s -> %s\n"),filename,filename2);
	  copy_file(filename,filename2);
	  install_flag=TRUE;
	}
      }
    }
  }

#ifdef USE_COMMON
  // Consistency Check
  if(mascot->cons_check_mode!=CONS_IGNORE){
    if(place!=4){
      if(filename0) g_free(filename0);
      filename0 = g_strconcat(COMMON_DIR,my_basename(mascot->file), NULL);
      if(access(filename0,F_OK)==0){
	stat(filename0, &statbuf);
	common_mtime=statbuf.st_mtime;
      }
      if(filename2) g_free(filename2);
#ifdef USE_WIN32
      filename2 = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,USER_DIR, 
			      my_basename(mascot->file), NULL);
#else
      filename2 = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,USER_DIR, 
			      my_basename(mascot->file), NULL);
#endif
      if(access(filename2,F_OK)==0){
	stat(filename2, &statbuf);
	user_mtime=statbuf.st_mtime;
      }
      
      stat(filename, &statbuf);
      if((user_mtime==statbuf.st_mtime)&&(user_mtime<common_mtime)){
	// Menu file in User Dir is older 
	// than the corresponded one in Common Dir.
	if(mascot->cons_check_mode==CONS_MANUAL){
	  create_cons_dialog(mascot,my_basename(filename),
			     user_mtime,common_mtime,FALSE);
	}
	else if(mascot->cons_check_mode==CONS_AUTOOW){
	  mascot->flag_consow=TRUE;
	}
	if(mascot->flag_consow){
	  //g_print(_("Installing %s -> %s\n"),filename0,filename2);
	  // Copy Common -> User
	  copy_file(filename0,filename2);
	  install_flag=TRUE;
	}
      }
    }
  }
#endif

  if(mascot->file) g_free(mascot->file);
  mascot->file=g_strdup(filename);
  cfgfile = xmms_cfg_open_file(filename);
  

  
  if (cfgfile) {
    // General
    if(f_tmp0) g_free(f_tmp0);
    if(def_flag)      f_tmp0=g_strdup("Default-General");
    else              f_tmp0=g_strdup("General");
    
    mascot->code =
      (xmms_cfg_read_string(cfgfile, f_tmp0, "code",&c_buf))
      ? c_buf : NULL;

    if(mascot->name) g_free(mascot->name);
    if(xmms_cfg_read_string(cfgfile, f_tmp0, "name", &c_buf)){
      mascot->name=
	x_locale_to_utf8(c_buf,-1,NULL,NULL,NULL,mascot->code);
      if(!mascot->name){
	mascot->name=g_strdup(_("(Invalid Character Code)"));
      }
    }
    else{
      mascot->name=NULL;
    }
      
    mascot->copyright=
      (xmms_cfg_read_string(cfgfile, f_tmp0, "copyright",&c_buf))
      ? c_buf : NULL;
    if(!xmms_cfg_read_boolean(cfgfile, f_tmp0, "default_color",&flag_def_col))
      flag_def_col=FALSE;

    // Focus Movement etc.
    
    if(f_tmp0) g_free(f_tmp0);
    if(def_flag)      f_tmp0=g_strdup("Default-Move");
    else              f_tmp0=g_strdup("Move");
    
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "move",&mascot->move))
      mascot->move=MOVE_FIX;
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "xoff",&mascot->xoff))
      mascot->xoff=0;
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "yoff",&mascot->yoff))
      mascot->yoff=0;
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "fix_x",&mascot->xfix))
      mascot->x=0;
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "fix_y",&mascot->yfix))
      mascot->y=0;

    // Clock
    if(f_tmp0) g_free(f_tmp0);
    if(def_flag)     f_tmp0=g_strdup("Default-Clock");
    else             f_tmp0=g_strdup("Clock");

    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "x",&mascot->clk_x))
      mascot->clk_x=INIT_CLK_POS;
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "y",&mascot->clk_y))
      mascot->clk_y=INIT_CLK_POS;
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "text_x",&mascot->clktext_x))
      mascot->clktext_x=INIT_CLK_TEXT;
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "text_y",&mascot->clktext_y))
      mascot->clktext_y=INIT_CLK_TEXT;
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "sd_x",&mascot->clksd_x))
      mascot->clksd_x=INIT_CLK_SD;
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "sd_y",&mascot->clksd_y))
      mascot->clksd_y=INIT_CLK_SD;
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "border",&mascot->wclkbd))
      mascot->wclkbd=INIT_CLK_BORDER;
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "mode",&mascot->clkmode))
      mascot->clkmode=CLOCK_NO;
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "type",&mascot->clktype))
      mascot->clktype=CLOCK_TYPE_24S;
    if(!xmms_cfg_read_boolean(cfgfile, f_tmp0, "shadow",&mascot->flag_clksd))
      mascot->flag_clksd=TRUE;
    if(!xmms_cfg_read_boolean(cfgfile, f_tmp0, "round",&mascot->flag_clkrd))
      mascot->flag_clkrd=TRUE;
    mascot->fontname_clk=
      (xmms_cfg_read_string(cfgfile, f_tmp0, "font",&c_buf))
      ? c_buf : NULL;
    //      mascot->fontname_clk=g_strconcat(FONT_CLK,NULL);


    // Balloon
    if(f_tmp0) g_free(f_tmp0);
    if(def_flag)    f_tmp0=g_strdup("Default-Balloon");
    else            f_tmp0=g_strdup("Balloon");

    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "text_x",&mascot->baltext_x))
      mascot->baltext_x=INIT_BAL_TEXT;
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "text_y",&mascot->baltext_y))
      mascot->baltext_y=INIT_BAL_TEXT;
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "border",&mascot->wbalbd))
      mascot->wbalbd=INIT_BAL_BORDER;
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "position",&mascot->bal_defpos))
      mascot->bal_defpos=BAL_POS_LEFT;
    mascot->fontname_bal=
      (xmms_cfg_read_string(cfgfile, f_tmp0, "font",&c_buf))
      ? c_buf : NULL;
    //      mascot->fontname_bal=g_strconcat(FONT_BAL,NULL);

    // Color for Clock
    if(f_tmp0) g_free(f_tmp0);
    if(def_flag)  f_tmp0=g_strdup("Default-ClockColor");
    else          f_tmp0=g_strdup("ClockColor");

    mascot->colclk->red  =my_mascot_read_color(cfgfile, f_tmp0, "text_r", mascot->def_colclk->red);
    mascot->colclk->green=my_mascot_read_color(cfgfile, f_tmp0, "text_g", mascot->def_colclk->green);
    mascot->colclk->blue =my_mascot_read_color(cfgfile, f_tmp0, "text_b", mascot->def_colclk->blue);
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "text_p", &col_tmp))
      col_tmp=mascot->def_alpclk;
    mascot->alpclk=col_tmp;
    
#ifdef  USE_GTK3
    mascot->colclk->alpha=(gdouble)mascot->alpclk/100.0;

    if((!flag_def_col) &&
       (my_def_color_check(mascot->colclk,
			   COLOR_CLK_R, COLOR_CLK_G, COLOR_CLK_B, DEF_ALPHA_OTHER))){
      gdk_rgba_free(mascot->def_colclk);
      mascot->colclk=gdk_rgba_copy(mascot->def_colclk);
      mascot->alpclk =mascot->def_alpclk;
    }
#else
    if((!flag_def_col) &&
       (my_def_color_check(mascot->colclk, mascot->alpclk,
			   COLOR_CLK_R, COLOR_CLK_G, COLOR_CLK_B, DEF_ALPHA_OTHER))){
      mascot->colclk=gdk_color_copy(mascot->def_colclk);
      mascot->alpclk =mascot->def_alpclk;
    }
#endif

    mascot->colclksd->red  =my_mascot_read_color(cfgfile, f_tmp0, "shadow_r", mascot->def_colclksd->red);
    mascot->colclksd->green=my_mascot_read_color(cfgfile, f_tmp0, "shadow_g", mascot->def_colclksd->green);
    mascot->colclksd->blue =my_mascot_read_color(cfgfile, f_tmp0, "shadow_b", mascot->def_colclksd->blue);
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "shadow_p", &col_tmp))
      col_tmp=mascot->def_alpclksd;
    mascot->alpclksd=col_tmp;
    
#ifdef  USE_GTK3
    mascot->colclksd->alpha=(gdouble)mascot->alpclksd/100.0;

    if((!flag_def_col) &&
       (my_def_color_check(mascot->colclksd,
			   COLOR_CLKSD_R, COLOR_CLKSD_G, COLOR_CLKSD_B, DEF_ALPHA_SDW))){
      gdk_rgba_free(mascot->def_colclksd);
      mascot->colclksd=gdk_rgba_copy(mascot->def_colclksd);
      mascot->alpclksd =mascot->def_alpclksd;
    }
#else
    if((!flag_def_col) &&
       (my_def_color_check(mascot->colclksd, mascot->alpclksd,
			   COLOR_CLKSD_R, COLOR_CLKSD_G, COLOR_CLKSD_B, DEF_ALPHA_SDW))){
      mascot->colclksd=gdk_color_copy(mascot->def_colclksd);
      mascot->alpclksd =mascot->def_alpclksd;
    }
#endif

    mascot->colclkbg->red  =my_mascot_read_color(cfgfile, f_tmp0, "bg_r", mascot->def_colclkbg->red);
    mascot->colclkbg->green=my_mascot_read_color(cfgfile, f_tmp0, "bg_g", mascot->def_colclkbg->green);
    mascot->colclkbg->blue =my_mascot_read_color(cfgfile, f_tmp0, "bg_b", mascot->def_colclkbg->blue);
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "bg_p", &col_tmp))
      col_tmp=mascot->def_alpclkbg;
    mascot->alpclkbg=col_tmp;
    
#ifdef  USE_GTK3
    mascot->colclkbg->alpha=(gdouble)mascot->alpclkbg/100.0;

    if((!flag_def_col) &&
       (my_def_color_check(mascot->colclkbg,
			   COLOR_CLKBG_R, COLOR_CLKBG_G, COLOR_CLKBG_B, DEF_ALPHA_CLK))){
      gdk_rgba_free(mascot->def_colclkbg);
      mascot->colclkbg=gdk_rgba_copy(mascot->def_colclkbg);
      mascot->alpclkbg =mascot->def_alpclkbg;
    }
#else
    if((!flag_def_col) &&
       (my_def_color_check(mascot->colclkbg, mascot->alpclkbg,
			   COLOR_CLKBG_R, COLOR_CLKBG_G, COLOR_CLKBG_B, DEF_ALPHA_CLK))){
      mascot->colclkbg=gdk_color_copy(mascot->def_colclkbg);
      mascot->alpclkbg =mascot->def_alpclkbg;
    }
#endif

    mascot->colclkbd->red  =my_mascot_read_color(cfgfile, f_tmp0, "border_r", mascot->def_colclkbd->red);
    mascot->colclkbd->green=my_mascot_read_color(cfgfile, f_tmp0, "border_g", mascot->def_colclkbd->green);
    mascot->colclkbd->blue =my_mascot_read_color(cfgfile, f_tmp0, "border_b", mascot->def_colclkbd->blue);
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "border_p", &col_tmp))
      col_tmp=mascot->def_alpclkbd;
    mascot->alpclkbd=col_tmp;
    
#ifdef  USE_GTK3
    mascot->colclkbd->alpha=(gdouble)mascot->alpclkbd/100.0;

    if((!flag_def_col) &&
       (my_def_color_check(mascot->colclkbd,
			   COLOR_CLKBD_R, COLOR_CLKBD_G, COLOR_CLKBD_B, DEF_ALPHA_OTHER))){
      gdk_rgba_free(mascot->def_colclkbd);
      mascot->colclkbd=gdk_rgba_copy(mascot->def_colclkbd);
      mascot->alpclkbd =mascot->def_alpclkbd;
    }
#else
    if((!flag_def_col) &&
       (my_def_color_check(mascot->colclkbd, mascot->alpclkbd,
			   COLOR_CLKBD_R, COLOR_CLKBD_G, COLOR_CLKBD_B, DEF_ALPHA_OTHER))){
      mascot->colclkbd=gdk_color_copy(mascot->def_colclkbd);
      mascot->alpclkbd =mascot->def_alpclkbd;
    }
#endif


    // Color for Balloon
    if(f_tmp0) g_free(f_tmp0);
    if(def_flag)  f_tmp0=g_strdup("Default-BalloonColor");
    else          f_tmp0=g_strdup("BalloonColor");

    mascot->colbal->red  =my_mascot_read_color(cfgfile, f_tmp0, "text_r", mascot->def_colbal->red);
    mascot->colbal->green=my_mascot_read_color(cfgfile, f_tmp0, "text_g", mascot->def_colbal->green);
    mascot->colbal->blue =my_mascot_read_color(cfgfile, f_tmp0, "text_b", mascot->def_colbal->blue);
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "text_p", &col_tmp))
      col_tmp=mascot->def_alpbal;
    mascot->alpbal=col_tmp;
    
#ifdef USE_GTK3
    mascot->colbal->alpha=(gdouble)mascot->alpbal/100.0;

    if((!flag_def_col) &&
       (my_def_color_check(mascot->colbal,
			   COLOR_BAL_R, COLOR_BAL_G, COLOR_BAL_B, DEF_ALPHA_OTHER))){
      gdk_rgba_free(mascot->def_colbal);
      mascot->colbal=gdk_rgba_copy(mascot->def_colbal);
      mascot->alpbal =mascot->def_alpbal;
    }
#else
    if((!flag_def_col) &&
       (my_def_color_check(mascot->colbal, mascot->alpbal,
			   COLOR_BAL_R, COLOR_BAL_G, COLOR_BAL_B, DEF_ALPHA_OTHER))){
      mascot->colbal=gdk_color_copy(mascot->def_colbal);
      mascot->alpbal =mascot->def_alpbal;
    }
#endif
       
    mascot->colbalbg->red  =my_mascot_read_color(cfgfile, f_tmp0, "bg_r", mascot->def_colbalbg->red);
    mascot->colbalbg->green=my_mascot_read_color(cfgfile, f_tmp0, "bg_g", mascot->def_colbalbg->green);
    mascot->colbalbg->blue =my_mascot_read_color(cfgfile, f_tmp0, "bg_b", mascot->def_colbalbg->blue);
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "bg_p", &col_tmp))
      col_tmp=mascot->def_alpbalbg;
    mascot->alpbalbg=col_tmp;
    
#ifdef USE_GTK3
    mascot->colbalbg->alpha=(gdouble)mascot->alpbalbg/100.0;

    if((!flag_def_col) &&
       (my_def_color_check(mascot->colbalbg,
			   COLOR_BALBG_R, COLOR_BALBG_G, COLOR_BALBG_B, DEF_ALPHA_BAL))){
      mascot->colbalbg=gdk_rgba_copy(mascot->def_colbalbg);
      mascot->alpbalbg =mascot->def_alpbalbg;
    }
#else
    if((!flag_def_col) &&
       (my_def_color_check(mascot->colbalbg, mascot->alpbalbg,
			   COLOR_BALBG_R, COLOR_BALBG_G, COLOR_BALBG_B, DEF_ALPHA_BAL))){
      mascot->colbalbg=gdk_color_copy(mascot->def_colbalbg);
      mascot->alpbalbg =mascot->def_alpbalbg;
    }
#endif

    mascot->colbalbd->red  =my_mascot_read_color(cfgfile, f_tmp0, "border_r", mascot->def_colbalbd->red);
    mascot->colbalbd->green=my_mascot_read_color(cfgfile, f_tmp0, "border_g", mascot->def_colbalbd->green);
    mascot->colbalbd->blue =my_mascot_read_color(cfgfile, f_tmp0, "border_b", mascot->def_colbalbd->blue);
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "border_p", &col_tmp))
      col_tmp=mascot->def_alpbalbd;
    mascot->alpbalbd=col_tmp;
    
#ifdef USE_GTK3
    mascot->colbalbd->alpha=(gdouble)mascot->alpbalbd/100.0;
    
    if((!flag_def_col) &&
       (my_def_color_check(mascot->colbalbd,
			   COLOR_BALBD_R, COLOR_BALBD_G, COLOR_BALBD_B, DEF_ALPHA_OTHER))){
      gdk_rgba_free(mascot->def_colbalbd);
      mascot->colbalbd=gdk_rgba_copy(mascot->def_colbalbd);
      mascot->alpbalbd =mascot->def_alpbalbd;
    }
#else
    if((!flag_def_col) &&
       (my_def_color_check(mascot->colbalbd, mascot->alpbalbd,
			   COLOR_BALBD_R, COLOR_BALBD_G, COLOR_BALBD_B, DEF_ALPHA_OTHER))){
      mascot->colbalbd=gdk_color_copy(mascot->def_colbalbd);
      mascot->alpbalbd =mascot->def_alpbalbd;
    }
#endif

    
#ifdef USE_BIFF
    // Biff用データ
    if(f_tmp0) g_free(f_tmp0);
    if(def_flag)   f_tmp0=g_strdup("Default-Biff");
    else           f_tmp0=g_strdup("Biff");

    // Biff用画像ファイル
    mascot->mail.pix_file=
      (xmms_cfg_read_string(cfgfile, f_tmp0, "pix-file",&c_buf))
      ? FullPathPixmapFile(mascot, c_buf) : NULL; 

    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "pix-pos",&mascot->mail.pix_pos))
      mascot->mail.pix_pos=MAIL_PIX_RIGHT;
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "pix-x",&mascot->mail.pix_x))
      mascot->mail.pix_x=0;
    if(!xmms_cfg_read_int(cfgfile, f_tmp0, "pix-y",&mascot->mail.pix_y))
      mascot->mail.pix_y=0;

    // Biff用ふきだしメッセージ
    if(mascot->mail.word) g_free(mascot->mail.word);
    if(xmms_cfg_read_string(cfgfile, f_tmp0, "word",&c_buf)){
      mascot->mail.word=
	x_locale_to_utf8(c_buf,-1,NULL,NULL,NULL,mascot->code);
      if(!mascot->mail.word){
	mascot->mail.word=g_strdup(_("(Invalid Character Code)"));
      }
    }
    else{
      mascot->mail.word=NULL;
    }
   
    // Biff着信時再生用ファイル
    mascot->mail.sound=
      (xmms_cfg_read_string(cfgfile, f_tmp0, "sound",&c_buf))
      ? c_buf : NULL;
#endif // USE_BIFF


    // Alpha Percentage
    if(mascot->force_def_alpha){
      mascot->alpha_main=mascot->def_alpha_main;
#ifdef USE_BIFF
      mascot->alpha_biff=mascot->def_alpha_biff;
#endif
#ifdef FG_DRAW
      mascot->alpha_clk=mascot->def_alpha_clk;
      mascot->flag_clkfg=mascot->def_flag_clkfg;
      mascot->alpha_bal=mascot->def_alpha_bal;
      mascot->flag_balfg=mascot->def_flag_balfg;
#endif
    }
    else{
      if(f_tmp0) g_free(f_tmp0);
      if(def_flag)  f_tmp0=g_strdup("Default-Alpha");
      else          f_tmp0=g_strdup("Alpha");
      
      if(!xmms_cfg_read_int(cfgfile, f_tmp0, "main",&mascot->alpha_main))
	mascot->alpha_main=mascot->def_alpha_main;
#ifdef USE_BIFF
      if(!xmms_cfg_read_int(cfgfile, f_tmp0, "biff",&mascot->alpha_biff))
	mascot->alpha_biff=mascot->def_alpha_biff;
#endif
#ifdef FG_DRAW
      if(!xmms_cfg_read_int(cfgfile, f_tmp0, "clock",&mascot->alpha_clk))
	mascot->alpha_clk=mascot->def_alpha_clk;
      if(!xmms_cfg_read_boolean(cfgfile, f_tmp0,"clockfg",&mascot->flag_clkfg))
	mascot->flag_clkfg=mascot->def_flag_clkfg;
      if(!xmms_cfg_read_int(cfgfile, f_tmp0, "balloon",&mascot->alpha_bal))
	mascot->alpha_bal=mascot->def_alpha_bal;
      if(!xmms_cfg_read_boolean(cfgfile, f_tmp0,"balloonfg",&mascot->flag_balfg))
	mascot->flag_balfg=mascot->def_flag_balfg;
#endif
    }
                                                                               

    // Pixmapデータ
    if(f_tmp0) g_free(f_tmp0);
    if(def_flag)   f_tmp0=g_strdup("Default-Pixmap");
    else           f_tmp0=g_strdup("Pixmap");
    
    for(i_pix=0;i_pix<MAX_PIXMAP;i_pix++){
      sprintf(tmp, "pixmap%02d", i_pix);
      if(!xmms_cfg_read_string(cfgfile, f_tmp0, tmp, &c_buf)){
	break;
      }
      else{
	if(mascot->sprites[i_pix].filename) g_free(mascot->sprites[i_pix].filename);
	mascot->sprites[i_pix].filename=FullPathPixmapFile(mascot, c_buf);  
      }
    }

    nPix=i_pix;
    for(i_pix=nPix;i_pix<MAX_PIXMAP;i_pix++){
      if(mascot->sprites[i_pix].filename) g_free(mascot->sprites[i_pix].filename);
      mascot->sprites[i_pix].filename=NULL;  
    }

    
	
    // Animationデータ
    // 各パターンのフレーム0のpixファイル名がないとうちきり
    for(i_ptn=0;i_ptn<MAX_ANIME_PATTERN;i_ptn++){
      if(def_flag)  sprintf(tmp0, "Default-Pattern%02d", i_ptn);
      else          sprintf(tmp0, "Pattern%02d", i_ptn);
      if(!xmms_cfg_read_int(cfgfile, tmp0, "random_weight",
			    &mascot->random_weight[i_ptn])) 
	// ランダムアニメ重み
	mascot->random_weight[i_ptn]=0;
      mascot->random_total+=mascot->random_weight[i_ptn];

      if(!xmms_cfg_read_int(cfgfile, tmp0, "click_weight",
			    &mascot->click_weight[i_ptn]))
	// クリックアニメ重み
	mascot->click_weight[i_ptn]=0;
      mascot->click_total+=mascot->click_weight[i_ptn];
      
      if(!xmms_cfg_read_int(cfgfile, tmp0, "balloon_lxoff",
			    &mascot->bal_lxoff[i_ptn]))
	// ふきだしオフセット X Left
	mascot->bal_lxoff[i_ptn]=0;

      if(!xmms_cfg_read_int(cfgfile, tmp0, "balloon_lyoff",
			    &mascot->bal_lyoff[i_ptn]))
	// ふきだしオフセット Y Left
	mascot->bal_lyoff[i_ptn]=0;

      if(!xmms_cfg_read_int(cfgfile, tmp0, "balloon_rxoff",
			    &mascot->bal_rxoff[i_ptn]))
	// ふきだしオフセット X Right
	mascot->bal_rxoff[i_ptn]=0;

      if(!xmms_cfg_read_int(cfgfile, tmp0, "balloon_ryoff",
			    &mascot->bal_ryoff[i_ptn]))
	// ふきだしオフセット Y Right
	mascot->bal_ryoff[i_ptn]=0;

	// バルーン表示用テキスト

      if(mascot->click_word[i_ptn]) g_free(mascot->click_word[i_ptn]);
      if(xmms_cfg_read_string(cfgfile, tmp0, "click_word", &c_buf)){
	mascot->click_word[i_ptn]=
	  x_locale_to_utf8(c_buf,-1,NULL,NULL,NULL,mascot->code);
	if(!mascot->click_word[i_ptn]){
	  mascot->click_word[i_ptn]=g_strdup(_("(Invalid Character Code)"));
	}
      }
      else{
	mascot->click_word[i_ptn]=NULL;
      }
   
      // クリック時再生用ファイル
      mascot->click_sound[i_ptn]=
	(xmms_cfg_read_string(cfgfile, tmp0, "click_sound", &c_buf))
	? c_buf : NULL;
   

#ifdef USE_SOCKMSG
      // デュエットアニメ
      mascot->duet_tgt[i_ptn]=
	(xmms_cfg_read_string(cfgfile, tmp0, "duet_tgt", &c_buf))
	? c_buf : NULL;

      if(!xmms_cfg_read_int(cfgfile, tmp0, "duet_ptn",
			    &mascot->duet_ptn[i_ptn]))
	mascot->duet_ptn[i_ptn]=2;
   
      if(mascot->duet_word[i_ptn]) g_free(mascot->duet_word[i_ptn]);
      if(xmms_cfg_read_string(cfgfile, tmp0, "duet_word",
			       &c_buf)){
	mascot->duet_word[i_ptn]=
	  x_locale_to_utf8(c_buf,-1,NULL,NULL,NULL,mascot->code);
	if(!mascot->duet_word[i_ptn]) {
	  mascot->duet_word[i_ptn]=g_strdup(_("(Invalid Character Code)"));
	}
      }
      else{
	mascot->duet_word[i_ptn]=NULL;
      }
   
      if(!xmms_cfg_read_int(cfgfile, tmp0, "duet_delay",
			    &mascot->duet_delay[i_ptn]))
	mascot->duet_delay[i_ptn]=DEF_DUET_DELAY;
#endif  // USE_SOCKMSG      

    
      // 各フレーム
      for(i_frm=0;i_frm<MAX_ANIME_FRAME;i_frm++){
	// pixmapが読めたときだけフレームループ
	sprintf(tmp, "pix%02d", i_frm);
	if(xmms_cfg_read_int(cfgfile, tmp0, tmp,
			     &mascot->frame_pix[i_ptn][i_frm])){
	  
	  sprintf(tmp, "min%02d", i_frm);
	  if(!xmms_cfg_read_int(cfgfile, tmp0, tmp,
				 &mascot->frame_min[i_ptn][i_frm]))
	    // minimum
	    mascot->frame_min[i_ptn][i_frm]=1;
	
	  sprintf(tmp, "max%02d", i_frm);
	  if(!xmms_cfg_read_int(cfgfile, tmp0, tmp,
				 &mascot->frame_max[i_ptn][i_frm]))
	    // maximum
	    mascot->frame_max[i_ptn][i_frm]=1;

	  if(mascot->frame_min[i_ptn][i_frm]>mascot->frame_max[i_ptn][i_frm]){
	    i_tmp=mascot->frame_max[i_ptn][i_frm];
	    mascot->frame_max[i_ptn][i_frm]=mascot->frame_min[i_ptn][i_frm];
	    mascot->frame_min[i_ptn][i_frm]=i_tmp;
	  }
      
	  // Block Loop Next Frame が設定されていたら min とmaxを読む
	  sprintf(tmp, "loop%02dnext", i_frm);
	  if(xmms_cfg_read_int(cfgfile, tmp0, tmp,
				 &mascot->frame_loop[i_ptn][i_frm].next)){

	    sprintf(tmp, "loop%02dmin", i_frm);
	    if(!xmms_cfg_read_int(cfgfile, tmp0, tmp,
				&mascot->frame_loop[i_ptn][i_frm].min))
	      // Block Loop Minimum
	      mascot->frame_loop[i_ptn][i_frm].min=1;

	    sprintf(tmp, "loop%02dmax", i_frm);
	    if(!xmms_cfg_read_int(cfgfile, tmp0, tmp,
				&mascot->frame_loop[i_ptn][i_frm].max))
	      // Block Loop Maximum
	      mascot->frame_loop[i_ptn][i_frm].max=1;

	    if( mascot->frame_loop[i_ptn][i_frm].min
		> mascot->frame_loop[i_ptn][i_frm].max){
	      i_tmp= mascot->frame_loop[i_ptn][i_frm].max;
	      mascot->frame_loop[i_ptn][i_frm].max
		= mascot->frame_loop[i_ptn][i_frm].min;
	      mascot->frame_loop[i_ptn][i_frm].min=i_tmp;
	    }

	  }
	  else{
	    mascot->frame_loop[i_ptn][i_frm].next=-1;
	  }

	  if(i_frm==MAX_ANIME_FRAME-1){
	    //mascot->frame_pix[i_ptn][i_frm+1]=-1;
	    mascot->frame_num[i_ptn]=MAX_ANIME_FRAME;
	  }
	}
	else{  // pixmap指定がないときは終了
	  mascot->frame_pix[i_ptn][i_frm]=-1;
	  mascot->frame_num[i_ptn]=i_frm;
	  break;
	}
      }
      
      if(i_frm==0){
	break;
      }
    }
  xmms_cfg_free(cfgfile);
  }

  if(install_flag) {
    if(mascot->file) g_free(mascot->file);
    mascot->file=g_strdup(filename2);
  }


  if(f_tmp0) g_free(f_tmp0);
  if(filename) g_free(filename);
  if(filename2) g_free(filename2);
  if(tmp_conv) g_free(tmp_conv);

  mascot->flag_consow=FALSE;
}


// マスコットのインストール :  メニューのインストール時に使用
void MascotInstall(typMascot *mascot, gchar *mascotfile)
{
  ConfigFile *cfgfile;
  gchar *filename=NULL, *filename2=NULL;
  gchar *pixfilename=NULL, *pixfile=NULL, *pixfilename2=NULL;
  gchar tmp[64];
  int i_pix=0, i_pix2=0;
  gchar *f_tmp0=NULL;
  gint place=0,p_place=0;

  
  /// マスコットファイルを順番で探す

  // 2. Currentから
  if(place==0){
    filename = g_strdup(mascotfile);
    if(access(filename,F_OK)==0){
      place=2;
    }
  }
  
  if(mascot->flag_ow){
#ifdef USE_COMMON
    // 4.  Common Directory
    if(place==0){
      filename = g_strconcat(COMMON_DIR,mascotfile, NULL);
      if(access(filename,F_OK)==0){
	place=4;
      }
    }
#endif

    // 1. メニューと同じ場所
    if(place==0){
      if(mascot->menu_file){
	filename = g_strconcat(my_dirname(mascot->menu_file),G_DIR_SEPARATOR_S,
			       mascotfile,NULL);
	if(access(filename,F_OK)==0){
	  place=1;
	}
      }
    }
    
    // 3. USER Directory
    if(place==0){
#ifdef USE_WIN32
      filename = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,USER_DIR, 
			     mascotfile, NULL);
#else
      filename = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,USER_DIR, 
			     mascotfile, NULL);
#endif
      if(access(filename,F_OK)==0){
	place=3;
      }
    }
  }
  else{
    // 1. メニューと同じ場所
    if(place==0){
      if(mascot->menu_file){
	filename = g_strconcat(my_dirname(mascot->menu_file),G_DIR_SEPARATOR_S,
			       mascotfile,NULL);
	if(access(filename,F_OK)==0){
	  place=1;
	}
      }
    }
    
    // 3. USER Directory
    if(place==0){
#ifdef USE_WIN32
      filename = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,USER_DIR, 
			     mascotfile, NULL);
#else
      filename = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,USER_DIR, 
			     mascotfile, NULL);
#endif
      if(access(filename,F_OK)==0){
	place=3;
      }
    }
  
#ifdef USE_COMMON
    // 4.  Common Directory
    if(place==0){
      filename = g_strconcat(COMMON_DIR,mascotfile, NULL);
      if(access(filename,F_OK)==0){
	place=4;
      }
    }
#endif
  }

  if(place==0){
    popup_message(mascot->win_main,
#ifdef USE_GTK3
		  "dialog-error", 
#else
		  GTK_STOCK_DIALOG_ERROR,
#endif
		  -1,
		  _("Cannot Find Mascot File."),
		  " ",
		  to_utf8(mascot->file),
		  NULL);
    exit(1);
  }

  
  if((place==1)||(place==2)||(place==4)){
    if(mascot->flag_install){
#ifdef USE_COMMON
      if(mascot->flag_common){
	check_common_dir();
	filename2 = g_strconcat(COMMON_DIR, mascotfile, NULL);
      }
      else{
#endif
#ifdef USE_WIN32
	filename2 = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,USER_DIR, 
				mascotfile, NULL);
#else
	filename2 = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,USER_DIR, 
				mascotfile, NULL);
#endif
#ifdef USE_COMMON
      }
#endif
      if((access(filename2,F_OK)!=0)||(mascot->flag_ow)){
	if(strcmp(filename,filename2)!=0){
	  //g_print(_("Installing %s -> %s\n"),filename,filename2);
	  copy_file(filename,filename2);
	}
      }
    }

    // Pixmapデータ
    if(mascot->flag_install){
      cfgfile = xmms_cfg_open_file(filename);
      if (cfgfile) {
	// Pixmapデータ
	f_tmp0=g_strdup("Pixmap");
    
	for(i_pix=0;i_pix<MAX_PIXMAP;i_pix++){
	  sprintf(tmp, "pixmap%02d", i_pix);
	  p_place=0;
	  if(xmms_cfg_read_string(cfgfile, f_tmp0, tmp, &pixfile)){
	    /// 画像ファイルを順番で探す
  
	    // 1. マスコットファイルと同じ場所
	    if(p_place==0){
	      pixfilename = g_strconcat(my_dirname(filename), 
					G_DIR_SEPARATOR_S,pixfile, NULL);
	      if(access(pixfilename,F_OK)==0){
		p_place=1;
	      }
	    }

	    if(mascot->flag_ow){
#ifdef USE_COMMON
	      // 4.  Common Directory
	      if(p_place==0){
		pixfilename = g_strconcat(COMMON_DIR,PIXDIR, 
					  pixfile, NULL);
		if(access(pixfilename,F_OK)==0){
		  p_place=4;
		}
	      }
#endif

	      // 3. USER Pixmap Directory
	      if(p_place==0){
#ifdef USE_WIN32
		pixfilename = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,
#else
		pixfilename = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,
#endif
					  USER_DIR,PIXDIR, 
					  pixfile, NULL);
		if(access(pixfilename,F_OK)==0){
		  p_place=3;
		}
	      }
	    }
	    else{
	      // 3. USER Pixmap Directory
	      if(p_place==0){
#ifdef USE_WIN32
		pixfilename = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,
#else
		pixfilename = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,
#endif
					  USER_DIR,PIXDIR, 
					  pixfile, NULL);
		if(access(pixfilename,F_OK)==0){
		  p_place=3;
		}
	      }

#ifdef USE_COMMON
	      // 4.  Common Directory
	      if(p_place==0){
		pixfilename = g_strconcat(COMMON_DIR,PIXDIR, 
					  pixfile, NULL);
		if(access(pixfilename,F_OK)==0){
		  p_place=4;
		}
	      }
#endif
	    }

	      
	    if(p_place==0){
	      popup_message(mascot->win_main,
#ifdef USE_GTK3
			    "dialog-error", 
#else
			    GTK_STOCK_DIALOG_ERROR,
#endif
			    -1,
			    _("Cannot Find Image."),
			    " ",
			    to_utf8(pixfile),
			    NULL);
	      exit(1);
	    }
  
	    if(p_place==1){
	      if(mascot->flag_install){
#ifdef USE_COMMON
		if(mascot->flag_common){
		  check_common_dir();
		  pixfilename2 = g_strconcat(COMMON_DIR,PIXDIR,
					     pixfile, NULL);
		}
		else{
#endif
#ifdef USE_WIN32
		  pixfilename2 = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,
#else
		  pixfilename2 = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,
#endif
					     USER_DIR,PIXDIR,
					     pixfile, NULL);
#ifdef USE_COMMON
		}
#endif
		if((access(pixfilename2,F_OK)!=0)||(mascot->flag_ow)){
		  if(strcmp(pixfilename,pixfilename2)!=0){
		    //g_print(_("Installing %s -> %s\n"), pixfilename,pixfilename2);
		    copy_file(pixfilename,pixfilename2);
		  }
		}
	      }
	    }
	  }
	  else{ //if(xmms_cfg_read_string(cfgfile, f_tmp0, tmp, &pixfile))
	    break;
	  }
	}
	xmms_cfg_free(cfgfile);
      }
    }
  }
}


gchar * FullPathMascotFile(typMascot *mascot, gchar *mascotfile)
{
  gchar *filename=NULL;
  gint place=0;

  
  /// マスコットファイルを順番で探す

  // 2. Currentから
  if(place==0){
    filename = g_strdup(mascotfile);
    if(access(filename,F_OK)==0){
      place=2;
    }
  }
  
  // 1. メニューと同じ場所
  if(place==0){
    if(mascot->menu_file){
      filename = g_strconcat(my_dirname(mascot->menu_file),G_DIR_SEPARATOR_S,
			     mascotfile,NULL);
      if(access(filename,F_OK)==0){
	place=1;
      }
    }
  }

  // 3. USER Directory
  if(place==0){
#ifdef USE_WIN32
    filename = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,USER_DIR, 
			   mascotfile, NULL);
#else
    filename = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,USER_DIR, 
			   mascotfile, NULL);
#endif
    if(access(filename,F_OK)==0){
      place=3;
    }
  }
  
#ifdef USE_COMMON
  // 4.  Common Data Directory
  if(place==0){
    filename = g_strconcat(COMMON_DIR,mascotfile, NULL);
    if(access(filename,F_OK)==0){
      place=4;
    }
  }
#endif

  if(place==0){
    popup_message(mascot->win_main,
#ifdef USE_GTK3
		  "dialog-error", 
#else
		  GTK_STOCK_DIALOG_ERROR,
#endif
		  -1,
		  _("Cannot Find Mascot File."),
		  " ",
		  to_utf8(mascotfile),
		  NULL);
    exit(1);
  }


  
  return(filename);
}


gboolean FullPathMascotCheck(typMascot *mascot, gchar *mascotfile)
{
  gchar *filename=NULL;
  gint place=0;
  ConfigFile *cfgfile;
  gchar *f_tmp0=NULL;
  gchar *pix0=NULL;
  gboolean flag_mf=FALSE;


  
  /// マスコットファイルを順番で探す

  // 2. Currentから
  if(place==0){
    filename = g_strdup(mascotfile);
    if(access(filename,F_OK)==0){
      place=2;
    }
  }
  
  // 1. メニューと同じ場所
  if(place==0){
    if(mascot->menu_file){
      filename = g_strconcat(my_dirname(mascot->menu_file),G_DIR_SEPARATOR_S,
			     mascotfile,NULL);
      if(access(filename,F_OK)==0){
	place=1;
      }
    }
  }

  // 3. USER Directory
  if(place==0){
#ifdef USE_WIN32
    filename = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,USER_DIR, 
			   mascotfile, NULL);
#else
    filename = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,USER_DIR, 
			   mascotfile, NULL);
#endif
    if(access(filename,F_OK)==0){
      place=3;
    }
  }
  
#ifdef USE_COMMON
  // 4.  Common Data Directory
  if(place==0){
    filename = g_strconcat(COMMON_DIR,mascotfile, NULL);
    if(access(filename,F_OK)==0){
      place=4;
    }
  }
#endif

  
  if(place!=0){
    cfgfile = xmms_cfg_open_file(filename);
  
    if (cfgfile) {
      // Pixmap00の指定がとれればマスコットファイルと認定
      f_tmp0=g_strdup("Pixmap");
      
      if(xmms_cfg_read_string(cfgfile, f_tmp0, "pixmap00",&pix0))
	flag_mf=TRUE;
      
      xmms_cfg_free(cfgfile);
    }
  }


  if(flag_mf){
    mascot->file=filename;
    return(TRUE);
  }
  else{
    return(FALSE);
  }
}


gchar *FullPathPixmapFile(typMascot *mascot, gchar *pixfile)
{
  gchar *pixfilename=NULL, *pixfilename0=NULL, *pixfilename2=NULL;
  gint p_place=0;
  

  /// 画像ファイルを順番で探す
  
  // 2. フルパス
  if(p_place==0){
    pixfilename = g_strdup(pixfile);
    if(access(pixfilename,F_OK)==0){
      p_place=2;
    }
  }
  
  // 1. マスコットファイルと同じ場所
  if(p_place==0){
    pixfilename = g_strconcat(my_dirname(mascot->file), 
			      G_DIR_SEPARATOR_S,pixfile, NULL);
    if(access(pixfilename,F_OK)==0){
      p_place=1;
    }
  }
  
  if(mascot->flag_ow){
#ifdef USE_COMMON
    // 4.  Common Data Directory
    if(p_place==0){
      pixfilename = g_strconcat(COMMON_DIR,PIXDIR, 
				pixfile, NULL);
      if(access(pixfilename,F_OK)==0){
	p_place=4;
      }
    }
#endif

    // 3. USER Pixmap Directory
    if(p_place==0){
#ifdef USE_WIN32
      pixfilename = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,
#else
      pixfilename = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,
#endif
				USER_DIR,PIXDIR, 
				pixfile, NULL);
      if(access(pixfilename,F_OK)==0){
	p_place=3;
      }
    }
  }
  else{
    // 3. USER Pixmap Directory
    if(p_place==0){
#ifdef USE_WIN32
      pixfilename = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,
#else
      pixfilename = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,
#endif
				USER_DIR,PIXDIR, 
				pixfile, NULL);
      if(access(pixfilename,F_OK)==0){
	p_place=3;
      }
    }
  
#ifdef USE_COMMON
    // 4.  Common Data Directory
    if(p_place==0){
      pixfilename = g_strconcat(COMMON_DIR,PIXDIR, 
				pixfile, NULL);
      if(access(pixfilename,F_OK)==0){
	p_place=4;
      }
    }
#endif
  }

	      
  if(p_place==0){
    popup_message(mascot->win_main,
#ifdef USE_GTK3
		  "dialog-error", 
#else
		  GTK_STOCK_DIALOG_ERROR,
#endif
		  -1,
		  _("Cannot Find Image."),
		  " ",
		  to_utf8(pixfile),
		  NULL);
    exit(1);
  }

  // 3,4にない場合はUSER Pixmap Dirにコピーする
  if((p_place==1)||(p_place==2)){
    if((mascot->flag_install)&&(!FlagInstalledMenu)){
#ifdef USE_COMMON
      if(mascot->flag_common){
	check_common_dir();
	pixfilename2 = g_strconcat(COMMON_DIR,PIXDIR,
				   my_basename(pixfile), NULL);
      }
      else{
#endif
#ifdef USE_WIN32
	pixfilename2 = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,
#else
	pixfilename2 = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,
#endif
				   USER_DIR,PIXDIR,
				   my_basename(pixfile), NULL);
#ifdef USE_COMMON
      }
#endif
      if((access(pixfilename2,F_OK)!=0)||(mascot->flag_ow)){
	if(strcmp(pixfilename,pixfilename2)!=0){
	  //g_print(_("Installing %s -> %s\n"),pixfilename,pixfilename2);
	  copy_file(pixfilename,pixfilename2);
	  pixfilename=pixfilename2;
	}
      }
    }
  }
  else if(mascot->flag_consow){  //Consistency Check
#ifdef USE_WIN32
    pixfilename2  = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,
#else
    pixfilename2  = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,
#endif
				USER_DIR,PIXDIR,
				my_basename(pixfile), NULL);
#ifdef USE_COMMON
    pixfilename0  = g_strconcat(COMMON_DIR,PIXDIR,
			       my_basename(pixfile), NULL);
    if((access(pixfilename0,F_OK)==0)&&(access(pixfilename2,F_OK)==0)){
      //g_print(_("Installing %s -> %s\n"), pixfilename0,pixfilename2);
      copy_file(pixfilename0,pixfilename2);
      pixfilename=pixfilename2;
    }
    else{
      pixfilename0  = g_strconcat(COMMON_DIR, my_basename(pixfile), NULL);
      if((access(pixfilename0,F_OK)==0)&&(access(pixfilename2,F_OK)==0)){
	//g_print(_("Installing %s -> %s\n"), pixfilename0,pixfilename2);
	copy_file(pixfilename0,pixfilename2);
	pixfilename=pixfilename2;
      }
    }
#endif
  }

  return(pixfilename);
}


/// サウンドファイルを順番で探す
gchar *FullPathSoundFile(typMascot *mascot, gchar *wavfile, gboolean i_fl){
  gchar *wavfilename=NULL,*wavfilename2=NULL;
  gint s_place=0;

  if(wavfile==NULL) return(NULL);

  /// 音声ファイルを順番で探す
  
  // 2. フルパス
  if(s_place==0){
    wavfilename = g_strdup(wavfile);
    if(access(wavfilename,F_OK)==0){
      s_place=2;
    }
  }
  
  // 1. マスコットファイルと同じ場所
  if(s_place==0){
    wavfilename = g_strconcat(my_dirname(mascot->file), 
			      G_DIR_SEPARATOR_S,wavfile, NULL);
    if(access(wavfilename,F_OK)==0){
      s_place=1;
    }
  }
  
  if(mascot->flag_ow){
#ifdef USE_COMMON
    // 4.  Common Data Directory
    if(s_place==0){
      wavfilename = g_strconcat(COMMON_DIR,SOUNDDIR, 
				wavfile, NULL);
      if(access(wavfilename,F_OK)==0){
	s_place=4;
      }
    }
#endif

    // 3. USER Sound Directory
    if(s_place==0){
#ifdef USE_WIN32
      wavfilename = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,
#else
      wavfilename = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,
#endif
				USER_DIR,SOUNDDIR, 
				wavfile, NULL);
      if(access(wavfilename,F_OK)==0){
	s_place=3;
      }
    }
  }
  else{
    // 3. USER Sound Directory
    if(s_place==0){
#ifdef USE_WIN32
      wavfilename = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,
#else
      wavfilename = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,
#endif
				USER_DIR,SOUNDDIR, 
				wavfile, NULL);
      if(access(wavfilename,F_OK)==0){
	s_place=3;
      }
    }
    
#ifdef USE_COMMON
    // 4.  Common Data Directory
    if(s_place==0){
      wavfilename = g_strconcat(COMMON_DIR,SOUNDDIR, 
				wavfile, NULL);
      if(access(wavfilename,F_OK)==0){
	s_place=4;
      }
    }
#endif
  }

	      
  if(s_place==0){
    popup_message(mascot->win_main,
#ifdef USE_GTK3
		  "dialog-warning", 
#else
		  GTK_STOCK_DIALOG_WARNING,
#endif
		  POPUP_TIMEOUT,
		  _("Cannot Find Sound File."),
		  " ",
		  to_utf8(wavfile),
		  NULL);
    exit(1);
  }

  // 3,4にない場合はUSER Sound Dirにコピーする
  if((s_place==1)||(s_place==2)){
    if(((mascot->flag_install)&&(!FlagInstalledMenu))||(i_fl)){
#ifdef USE_COMMON
      if(mascot->flag_common){
	check_common_dir();
	wavfilename2 = g_strconcat(COMMON_DIR,SOUNDDIR,
				   my_basename(wavfile), NULL);
      }
      else{
#endif
#ifdef USE_WIN32
	wavfilename2 = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,
#else
	wavfilename2 = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,
#endif
				   USER_DIR,SOUNDDIR,
				   my_basename(wavfile), NULL);
#ifdef USE_COMMON
      }
#endif
      if((access(wavfilename2,F_OK)!=0)||(mascot->flag_ow)){
	if(strcmp(wavfilename,wavfilename2)!=0){
	  //g_print(_("Installing %s -> %s\n"),wavfilename,wavfilename2);
	  copy_file(wavfilename,wavfilename2);
	  wavfilename=wavfilename2;
	}
      }
    }
  }

  return(wavfilename);
}


/// リソースファイルを探す
// フルパスでかかれていてアクセスできるか
// そうでなければ USERDIR下で探す
// 見つからなければデフォルトのRCFILEを返せばよい
gchar  *FullPathRcFile(gchar *rcfile){
  gchar *filename1;

    
  // 1. マスコット fileと同一 Directory
  if(access(rcfile,F_OK)==0){
    filename1 = g_strdup(rcfile);
  }
  else{
#ifdef USE_WIN32
    filename1 = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,
#else
    filename1 = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,
#endif
			    USER_DIR,rcfile, NULL);
    if(access(filename1,F_OK)!=0){
      g_print(_("Cannot find resource file %s.\n"),rcfile);
      g_print(_("Loading default resource file...\n"));
#ifdef USE_WIN32
      filename1 = g_strconcat(get_win_home(), G_DIR_SEPARATOR_S,USER_DIR,
#else
      filename1 = g_strconcat(g_get_home_dir(), G_DIR_SEPARATOR_S,USER_DIR,
#endif
			      USER_RCFILE, NULL);
    }
  }
  return(filename1);
}


// マスコット名の読み込み
gchar * ReadMascotName(typMascot *mascot, gchar *mascotfile)
{
  ConfigFile *cfgfile;
  gchar *filename=NULL;
  gchar *f_tmp0=NULL;
  gchar *mascotname=NULL;
  gchar *code=NULL;

  filename=FullPathMascotFile(mascot, mascotfile);
  
  cfgfile = xmms_cfg_open_file(filename);
  
  if (cfgfile) {
    // General
    f_tmp0=g_strdup("General");
    
    if(!xmms_cfg_read_string(cfgfile, f_tmp0, "code",&code))
      code = NULL;

    if(!xmms_cfg_read_string(cfgfile, f_tmp0, "name",&mascotname))
      mascotname = g_strconcat("(",my_basename(mascotfile),")", NULL);
    if(mascotname){
      mascotname=x_locale_to_utf8(mascotname,-1,NULL,NULL,NULL,code);
      if(!mascotname) 
	mascotname=g_strdup(_("(Invalid Character Code)"));
    }
    
    xmms_cfg_free(cfgfile);
  }
  g_free(f_tmp0);
  g_free(filename);
  g_free(code);

  return(mascotname);
}


// マスコットファイルの書き込み
void SaveMascot(typMascot *mascot, gboolean def_flag)
{
  ConfigFile *cfgfile;
  gchar *filename;
  gchar tmp[64], tmp0[64];
  int i_pix=0, i_ptn=0, i_frm=0, i_pix2=0;
  gchar *f_tmp=NULL;
  gchar *tmp_conv=NULL;



  filename = g_strdup(mascot->file);
  cfgfile = xmms_cfg_open_file(filename);
  if (!cfgfile)  cfgfile = xmms_cfg_new();
  
  // General 
  f_tmp=g_strdup((def_flag) ? "Default-General" : "General");
  
  xmms_cfg_write_string(cfgfile, f_tmp, "prog_ver",VERSION);
  
  if(mascot->code){
    xmms_cfg_write_string(cfgfile, f_tmp, "code",mascot->code);
  }
  else{
    xmms_cfg_remove_key(cfgfile,f_tmp, "code");
  }
  
  if(mascot->name){
    tmp_conv=
      x_locale_from_utf8(mascot->name,-1,NULL,NULL,NULL,mascot->code);
    xmms_cfg_write_string(cfgfile, f_tmp, "name",tmp_conv);
  }
  if(mascot->copyright)
    xmms_cfg_write_string(cfgfile, f_tmp, "copyright",mascot->copyright);
  xmms_cfg_write_boolean(cfgfile, f_tmp, "default_color",TRUE);


  // Focus Movement etc.
  f_tmp=g_strdup((def_flag) ? "Default-Move" : "Move");

  xmms_cfg_write_int(cfgfile, f_tmp, "move",mascot->move);
  xmms_cfg_write_int(cfgfile, f_tmp, "xoff",mascot->xoff);
  xmms_cfg_write_int(cfgfile, f_tmp, "yoff",mascot->yoff);
  xmms_cfg_write_int(cfgfile, f_tmp, "fix_x",mascot->xfix);
  xmms_cfg_write_int(cfgfile, f_tmp, "fix_y",mascot->yfix);

  


  // Clock
  f_tmp=g_strdup((def_flag) ? "Default-Clock" : "Clock");

  xmms_cfg_write_int(cfgfile, f_tmp, "mode",mascot->clkmode);
  xmms_cfg_write_int(cfgfile, f_tmp, "type",mascot->clktype);
  xmms_cfg_write_int(cfgfile, f_tmp, "x",mascot->clk_x);
  xmms_cfg_write_int(cfgfile, f_tmp, "y",mascot->clk_y);
  xmms_cfg_write_int(cfgfile, f_tmp, "text_x",mascot->clktext_x);
  xmms_cfg_write_int(cfgfile, f_tmp, "text_y",mascot->clktext_y);
  xmms_cfg_write_int(cfgfile, f_tmp, "sd_x",mascot->clksd_x);
  xmms_cfg_write_int(cfgfile, f_tmp, "sd_y",mascot->clksd_y);
  xmms_cfg_write_int(cfgfile, f_tmp, "border",mascot->wclkbd);
  xmms_cfg_write_boolean(cfgfile, f_tmp, "shadow",mascot->flag_clksd);
  xmms_cfg_write_boolean(cfgfile, f_tmp, "round",mascot->flag_clkrd);
  if(strcmp(mascot->fontname_clk,mascot->deffontname_clk)!=0)
    xmms_cfg_write_string(cfgfile, f_tmp, "font",mascot->fontname_clk);
  else
    xmms_cfg_remove_key(cfgfile,f_tmp, "font");


  // Balloon
  f_tmp=g_strdup((def_flag) ? "Default-Balloon" : "Balloon");

  xmms_cfg_write_int(cfgfile, f_tmp, "text_x",mascot->baltext_x);
  xmms_cfg_write_int(cfgfile, f_tmp, "text_y",mascot->baltext_y);
  xmms_cfg_write_int(cfgfile, f_tmp, "border",mascot->wbalbd);
  xmms_cfg_write_int(cfgfile, f_tmp, "position",mascot->bal_defpos);
  if(strcmp(mascot->fontname_bal,mascot->deffontname_bal)!=0)
    xmms_cfg_write_string(cfgfile, f_tmp, "font",mascot->fontname_bal);
  else
    xmms_cfg_remove_key(cfgfile,f_tmp, "font");

  // Color for Clock
  f_tmp=g_strdup((def_flag) ? "Default-ClockColor" : "ClockColor");

  my_mascot_write_color(cfgfile, f_tmp,
			"text_r", "text_g", "text_b", "text_p",
			mascot->colclk, mascot->def_colclk,
			mascot->alpclk, mascot->def_alpclk);

  my_mascot_write_color(cfgfile, f_tmp,
			"shadow_r", "shadow_g", "shadow_b", "shadow_p",
			mascot->colclksd, mascot->def_colclksd,
			mascot->alpclksd, mascot->def_alpclksd);

  my_mascot_write_color(cfgfile, f_tmp,
			"bg_r", "bg_g", "bg_b", "bg_p",
			mascot->colclkbg, mascot->def_colclkbg,
			mascot->alpclkbg, mascot->def_alpclkbg);

  my_mascot_write_color(cfgfile, f_tmp,
			"border_r", "border_g", "border_b", "border_p",
			mascot->colclkbd, mascot->def_colclkbd,
			mascot->alpclkbd, mascot->def_alpclkbd);


  // Color for Balloon
  f_tmp=g_strdup((def_flag) ? "Default-BalloonColor" : "BalloonColor");

  my_mascot_write_color(cfgfile, f_tmp,
			"text_r", "text_g", "text_b", "text_p",
			mascot->colbal, mascot->def_colbal,
			mascot->alpbal, mascot->def_alpbal);
  
  my_mascot_write_color(cfgfile, f_tmp,
			"bg_r", "bg_g", "bg_b", "bg_p",
			mascot->colbalbg, mascot->def_colbalbg,
			mascot->alpbalbg, mascot->def_alpbalbg);

  my_mascot_write_color(cfgfile, f_tmp,
			"border_r", "border_g", "border_b", "border_p",
			mascot->colbalbd, mascot->def_colbalbd,
			mascot->alpbalbd, mascot->def_alpbalbd);

#ifdef USE_BIFF
  // Biffデータ
  if(def_flag) f_tmp=g_strdup("Default-Biff");
  else         f_tmp=g_strdup("Biff");

  // Biff用画像ファイル
  if(mascot->mail.pix_file) 
    xmms_cfg_write_string(cfgfile, f_tmp, "pix-file",mascot->mail.pix_file);
  else
    xmms_cfg_remove_key(cfgfile,f_tmp, "pix-file");

  xmms_cfg_write_int(cfgfile, f_tmp, "pix-pos",mascot->mail.pix_pos);
  xmms_cfg_write_int(cfgfile, f_tmp, "pix-x",mascot->mail.pix_x);
  xmms_cfg_write_int(cfgfile, f_tmp, "pix-y",mascot->mail.pix_y);

  // Biff用ふきだしメッセージ
  if(mascot->mail.word){
    tmp_conv=
      x_locale_from_utf8(mascot->mail.word,-1,NULL,NULL,NULL,mascot->code);
    xmms_cfg_write_string(cfgfile, f_tmp, "word",tmp_conv);
  }
  else{
    xmms_cfg_remove_key(cfgfile,f_tmp, "word");
  }

  // Biff着信時再生音声ファイル
  if(mascot->mail.sound){
    xmms_cfg_write_string(cfgfile, f_tmp, "sound",mascot->mail.sound);
  }
  else{
    xmms_cfg_remove_key(cfgfile, f_tmp, "sound");
  }
#endif  // USE_BIFF


  // Alpha Percentage
  if(!mascot->force_def_alpha){
    if(def_flag)  f_tmp=g_strdup("Default-Alpha");
    else          f_tmp=g_strdup("Alpha");
    
    xmms_cfg_write_int(cfgfile, f_tmp, "main",mascot->alpha_main);
#ifdef USE_BIFF
    xmms_cfg_write_int(cfgfile, f_tmp, "biff",mascot->alpha_biff);
#endif
#ifdef FG_DRAW
    xmms_cfg_write_int(cfgfile, f_tmp, "clock",mascot->alpha_clk);
    xmms_cfg_write_boolean(cfgfile, f_tmp,"clockfg",mascot->flag_clkfg);
    xmms_cfg_write_int(cfgfile, f_tmp, "balloon",mascot->alpha_bal);
    xmms_cfg_write_boolean(cfgfile, f_tmp,"balloonfg",mascot->flag_balfg);
#endif
  }

  
  // Pixmapデータ
  f_tmp=g_strdup((def_flag) ? "Default-Pixmap" : "Pixmap");

  for(i_pix=0;i_pix<MAX_PIXMAP;i_pix++){
    sprintf(tmp, "pixmap%02d", i_pix);
    
    if(mascot->sprites[i_pix].filename){
      xmms_cfg_write_string(cfgfile, f_tmp, tmp,
			    (char *)my_basename(mascot->sprites[i_pix].filename));
    }
    else{
      for(i_pix2=i_pix;i_pix2<MAX_PIXMAP;i_pix2++){
	sprintf(tmp, "pixmap%02d", i_pix2);
	xmms_cfg_remove_key(cfgfile, f_tmp, tmp);
      }
      break;
    }
      
  }
	
  // Animationデータ
  for(i_ptn=0;i_ptn<MAX_ANIME_PATTERN;i_ptn++){
    
    sprintf(tmp0, (def_flag) ? "Default-Pattern%02d" : "Pattern%02d", i_ptn);
    
    if(mascot->random_weight[i_ptn]!=0){ // ランダムアニメ重み
      xmms_cfg_write_int(cfgfile, tmp0, "random_weight",
			 mascot->random_weight[i_ptn]);
    }
    else{
      xmms_cfg_remove_key(cfgfile,tmp0, "random_weight");
    }
    
    if(mascot->click_weight[i_ptn]!=0){ // クリックアニメ重み
      xmms_cfg_write_int(cfgfile, tmp0, "click_weight",
			 mascot->click_weight[i_ptn]);
    }
    else{
      xmms_cfg_remove_key(cfgfile,tmp0, "click_weight");
    }
    
    if(mascot->bal_lxoff[i_ptn]!=0){ // ふきだし offset X[左]
      xmms_cfg_write_int(cfgfile, tmp0, "balloon_lxoff",
			 mascot->bal_lxoff[i_ptn]);
    }
    else{
      xmms_cfg_remove_key(cfgfile,tmp0, "balloon_lxoff");
    }
    
    if(mascot->bal_lyoff[i_ptn]!=0){ // ふきだし offset Y[左]
      xmms_cfg_write_int(cfgfile, tmp0, "balloon_lyoff",
			 mascot->bal_lyoff[i_ptn]);
    }
    else{
      xmms_cfg_remove_key(cfgfile,tmp0, "balloon_lyoff");
    }
    
    if(mascot->bal_rxoff[i_ptn]!=0){ // ふきだし offset X[右]
      xmms_cfg_write_int(cfgfile, tmp0, "balloon_rxoff",
			 mascot->bal_rxoff[i_ptn]);
    }
    else{
      xmms_cfg_remove_key(cfgfile,tmp0, "balloon_rxoff");
    }
    
    if(mascot->bal_ryoff[i_ptn]!=0){ // ふきだし offset Y[右]
      xmms_cfg_write_int(cfgfile, tmp0, "balloon_ryoff",
			 mascot->bal_ryoff[i_ptn]);
    }
    else{
      xmms_cfg_remove_key(cfgfile,tmp0, "balloon_ryoff");
    }
    
    if(mascot->click_word[i_ptn]){
      tmp_conv=
	x_locale_from_utf8(mascot->click_word[i_ptn],-1,NULL,NULL,NULL,mascot->code);
      xmms_cfg_write_string(cfgfile, tmp0, "click_word",tmp_conv);
    }
    else{
      xmms_cfg_remove_key(cfgfile,tmp0, "click_word");
    }
    
    if(mascot->click_sound[i_ptn]){ // クリック時再生音声ファイル
      xmms_cfg_write_string(cfgfile, tmp0, "click_sound",
			    (gchar *)mascot->click_sound[i_ptn]);
    }
    else{
      xmms_cfg_remove_key(cfgfile,tmp0, "click_sound");
    }
    
#ifdef USE_SOCKMSG
    if(mascot->duet_tgt[i_ptn]){ // デュエットアニメ
      xmms_cfg_write_string(cfgfile, tmp0, "duet_tgt",
			    (gchar *)mascot->duet_tgt[i_ptn]);
      xmms_cfg_write_int(cfgfile, tmp0, "duet_ptn",
			 mascot->duet_ptn[i_ptn]);
      if(mascot->duet_word[i_ptn]){
	tmp_conv=
	  x_locale_from_utf8(mascot->duet_word[i_ptn],-1,NULL,NULL,NULL,mascot->code);
	xmms_cfg_write_string(cfgfile, tmp0, "duet_word",tmp_conv);
      }
      xmms_cfg_write_int(cfgfile, tmp0, "duet_delay",
			 mascot->duet_delay[i_ptn]);
    }
    else{
      xmms_cfg_remove_key(cfgfile,tmp0, "duet_tgt");
      xmms_cfg_remove_key(cfgfile,tmp0, "duet_ptn");
      xmms_cfg_remove_key(cfgfile,tmp0, "duet_word");
      xmms_cfg_remove_key(cfgfile,tmp0, "duet_delay");
    }
    
#endif // USE_SOCKMSG
    
    // 各フレーム
    for(i_frm=0;i_frm<MAX_ANIME_FRAME;i_frm++){
      if(mascot->frame_pix[i_ptn][i_frm]!=-1){
	// Pixmap
	sprintf(tmp, "pix%02d", i_frm);
	xmms_cfg_write_int(cfgfile, tmp0, tmp,
			   mascot->frame_pix[i_ptn][i_frm]);
	
	// minimum
	sprintf(tmp, "min%02d", i_frm);
	xmms_cfg_write_int(cfgfile, tmp0, tmp,
			   mascot->frame_min[i_ptn][i_frm]);
	
	// maximum
	sprintf(tmp, "max%02d", i_frm);
	xmms_cfg_write_int(cfgfile, tmp0, tmp,
			   mascot->frame_max[i_ptn][i_frm]);
	
	if(mascot->frame_loop[i_ptn][i_frm].next!=-1){
	  // Block Loop Next Frame
	  sprintf(tmp, "loop%02dnext", i_frm);
	  xmms_cfg_write_int(cfgfile, tmp0, tmp,
			     mascot->frame_loop[i_ptn][i_frm].next);
	  
	  // Block Loop Minimum
	  sprintf(tmp, "loop%02dmin", i_frm);
	  xmms_cfg_write_int(cfgfile, tmp0, tmp,
			     mascot->frame_loop[i_ptn][i_frm].min);
	  
	  // Block Loop Maximum
	  sprintf(tmp, "loop%02dmax", i_frm);
	  xmms_cfg_write_int(cfgfile, tmp0, tmp,
			     mascot->frame_loop[i_ptn][i_frm].max);
	}
	else{
	  sprintf(tmp, "loop%02dnext", i_frm);
	  xmms_cfg_remove_key(cfgfile,tmp0,tmp);
	  
	  sprintf(tmp, "loop%02dmin", i_frm);
	  xmms_cfg_remove_key(cfgfile,tmp0,tmp);
	  
	  sprintf(tmp, "loop%02dmax", i_frm);
	  xmms_cfg_remove_key(cfgfile,tmp0,tmp);
	}
      }
      else{
	sprintf(tmp, "pix%02d", i_frm);
	xmms_cfg_remove_key(cfgfile,tmp0,tmp);
	
	sprintf(tmp, "min%02d", i_frm);
	xmms_cfg_remove_key(cfgfile,tmp0,tmp);
	
	sprintf(tmp, "max%02d", i_frm);
	xmms_cfg_remove_key(cfgfile,tmp0,tmp);
	
	sprintf(tmp, "loop%02dnext", i_frm);
	xmms_cfg_remove_key(cfgfile,tmp0,tmp);
	
	sprintf(tmp, "loop%02dmin", i_frm);
	xmms_cfg_remove_key(cfgfile,tmp0,tmp);
	
	sprintf(tmp, "loop%02dmax", i_frm);
	xmms_cfg_remove_key(cfgfile,tmp0,tmp);
      }
    }
  }
  
  if(!def_flag){
    if(!xmms_cfg_read_string(cfgfile, "Default-General","prog_ver",&f_tmp)){
      xmms_cfg_write_file(cfgfile, filename);
      xmms_cfg_free(cfgfile);
      SaveMascot(mascot,TRUE);
    }
    else{
      xmms_cfg_write_file(cfgfile, filename);
      xmms_cfg_free(cfgfile);
    }
  }
  else{
    xmms_cfg_write_file(cfgfile, filename);
    xmms_cfg_free(cfgfile);
  }

  g_free(f_tmp);
  g_free(filename);
  g_free(tmp_conv);
}


// リリース用マスコットのフォント情報の除去もしくは追加
void SetFontForReleaseData(typMascot *mascot, gboolean def_flag,
			   gint field, gboolean set_flag)
{
  ConfigFile *cfgfile;
  gchar *filename=NULL;
  gchar *f_tmp=NULL;

  filename = g_strdup(mascot->file);
  cfgfile = xmms_cfg_open_file(filename);
  
  if (cfgfile) {
    if(field==SET_RELEASE_BALLOON){
      // Balloon
      if(def_flag) f_tmp=g_strdup("Default-Balloon");
      else         f_tmp=g_strdup("Balloon");

      if(!set_flag){
	xmms_cfg_remove_key(cfgfile, f_tmp, "font");
      }
      else{
	xmms_cfg_write_string(cfgfile, f_tmp, "font",mascot->fontname_bal);
      }
    }
    else if(field==SET_RELEASE_CLOCK){

      // Clock
      if(def_flag) f_tmp=g_strdup("Default-Clock");
      else         f_tmp=g_strdup("Clock");

      if(!set_flag){
	xmms_cfg_remove_key(cfgfile, f_tmp, "font");
      }
      else{
	xmms_cfg_write_string(cfgfile, f_tmp, "font",mascot->fontname_clk);
      }
    }

    xmms_cfg_write_file(cfgfile, filename);
    xmms_cfg_free(cfgfile);
  }

  g_free(f_tmp);
  g_free(filename);
}


// リリース用マスコットからの色情報の除去
void SetColorForReleaseData(typMascot *mascot, gboolean def_flag,
			    gint field, gboolean set_flag)
{
  ConfigFile *cfgfile;
  gchar *filename=NULL;
  gchar *f_tmp=NULL;

  filename = g_strdup(mascot->file);
  cfgfile = xmms_cfg_open_file(filename);
  
  if (cfgfile) {
    if(field==SET_RELEASE_BALLOON){
    // Balloon
      if(def_flag) f_tmp=g_strdup("Default-BalloonColor");
      else f_tmp=g_strdup("BalloonColor");

      if(!set_flag){
	xmms_cfg_remove_key(cfgfile, f_tmp, "text_r");
	xmms_cfg_remove_key(cfgfile, f_tmp, "text_g");
	xmms_cfg_remove_key(cfgfile, f_tmp, "text_b");
	xmms_cfg_remove_key(cfgfile, f_tmp, "bg_r");
	xmms_cfg_remove_key(cfgfile, f_tmp, "bg_g");
	xmms_cfg_remove_key(cfgfile, f_tmp, "bg_b");
	xmms_cfg_remove_key(cfgfile, f_tmp, "border_r");
	xmms_cfg_remove_key(cfgfile, f_tmp, "border_g");
	xmms_cfg_remove_key(cfgfile, f_tmp, "border_b");
	xmms_cfg_remove_key(cfgfile, f_tmp, "text_p");
	xmms_cfg_remove_key(cfgfile, f_tmp, "bg_p");
	xmms_cfg_remove_key(cfgfile, f_tmp, "border_p");
      }
      else{
	xmms_cfg_write_int(cfgfile, f_tmp, "text_r",mascot->colbal->red);
	xmms_cfg_write_int(cfgfile, f_tmp, "text_g",mascot->colbal->green);
	xmms_cfg_write_int(cfgfile, f_tmp, "text_b",mascot->colbal->blue);
	xmms_cfg_write_int(cfgfile, f_tmp, "bg_r",mascot->colbalbg->red);
	xmms_cfg_write_int(cfgfile, f_tmp, "bg_g",mascot->colbalbg->green);
	xmms_cfg_write_int(cfgfile, f_tmp, "bg_b",mascot->colbalbg->blue);
	xmms_cfg_write_int(cfgfile, f_tmp, "border_r",mascot->colbalbd->red);
	xmms_cfg_write_int(cfgfile, f_tmp, "border_g",mascot->colbalbd->green);
	xmms_cfg_write_int(cfgfile, f_tmp, "border_b",mascot->colbalbd->blue);
	xmms_cfg_write_int(cfgfile, f_tmp, "text_p",mascot->alpbal);
	xmms_cfg_write_int(cfgfile, f_tmp, "bg_p",mascot->alpbalbg);
	xmms_cfg_write_int(cfgfile, f_tmp, "border_p",mascot->alpbalbd);
      }
    }
    else if(field==SET_RELEASE_CLOCK){
      // Clock
      if(def_flag) f_tmp=g_strdup("Default-ClockColor");
      else         f_tmp=g_strdup("ClockColor");

      if(!set_flag){
	xmms_cfg_remove_key(cfgfile, f_tmp, "text_r");
	xmms_cfg_remove_key(cfgfile, f_tmp, "text_g");
	xmms_cfg_remove_key(cfgfile, f_tmp, "text_b");
	xmms_cfg_remove_key(cfgfile, f_tmp, "shadow_r");
	xmms_cfg_remove_key(cfgfile, f_tmp, "shadow_g");
	xmms_cfg_remove_key(cfgfile, f_tmp, "shadow_b");
	xmms_cfg_remove_key(cfgfile, f_tmp, "bg_r");
	xmms_cfg_remove_key(cfgfile, f_tmp, "bg_g");
	xmms_cfg_remove_key(cfgfile, f_tmp, "bg_b");
	xmms_cfg_remove_key(cfgfile, f_tmp, "border_r");
	xmms_cfg_remove_key(cfgfile, f_tmp, "border_g");
	xmms_cfg_remove_key(cfgfile, f_tmp, "border_b");
	xmms_cfg_remove_key(cfgfile, f_tmp, "text_p");
	xmms_cfg_remove_key(cfgfile, f_tmp, "shadow_p");
	xmms_cfg_remove_key(cfgfile, f_tmp, "bg_p");
	xmms_cfg_remove_key(cfgfile, f_tmp, "border_p");
      }
      else{
	xmms_cfg_write_int(cfgfile, f_tmp, "text_r",mascot->colclk->red);
	xmms_cfg_write_int(cfgfile, f_tmp, "text_g",mascot->colclk->green);
	xmms_cfg_write_int(cfgfile, f_tmp, "text_b",mascot->colclk->blue);
	xmms_cfg_write_int(cfgfile, f_tmp, "shadow_r",mascot->colclksd->red);
	xmms_cfg_write_int(cfgfile, f_tmp, "shadow_g",mascot->colclksd->green);
	xmms_cfg_write_int(cfgfile, f_tmp, "shadow_b",mascot->colclksd->blue);
	xmms_cfg_write_int(cfgfile, f_tmp, "bg_r",mascot->colclkbg->red);
	xmms_cfg_write_int(cfgfile, f_tmp, "bg_g",mascot->colclkbg->green);
	xmms_cfg_write_int(cfgfile, f_tmp, "bg_b",mascot->colclkbg->blue);
	xmms_cfg_write_int(cfgfile, f_tmp, "border_r",mascot->colclkbd->red);
	xmms_cfg_write_int(cfgfile, f_tmp, "border_g",mascot->colclkbd->green);
	xmms_cfg_write_int(cfgfile, f_tmp, "border_b",mascot->colclkbd->blue);
	xmms_cfg_write_int(cfgfile, f_tmp, "text_p",mascot->alpclk);
	xmms_cfg_write_int(cfgfile, f_tmp, "shadow_p",mascot->alpclksd);
	xmms_cfg_write_int(cfgfile, f_tmp, "bg_p",mascot->alpclkbg);
	xmms_cfg_write_int(cfgfile, f_tmp, "border_p",mascot->alpclkbd);
      }
    }

    xmms_cfg_write_file(cfgfile, filename);

    xmms_cfg_free(cfgfile);
  }

  g_free(f_tmp);
  g_free(filename);
}


  
void create_dl_smenu_dialog(typMascot *mascot, gboolean flag_popup){
  ConfigFile *cfgfile;
  FILE *fp;
  GtkWidget *dialog, *vbox, *label, *button, *bar, *table, *scrwin, *rb[MAX_TGZ_FILE];
  gchar *menu_code, *list_tmp=NULL;
  gint i_list=0, i_list_max, i_selected=0;
  gchar *tgz_file[MAX_TGZ_FILE];
  gchar *ename=NULL, *lname=NULL, *code=NULL, *ver=NULL, *date=NULL;
  gint num;
  gchar *dummy=NULL;
  gchar *tmp;
  gboolean flag_install;

  cfgfile = xmms_cfg_open_file(mascot->http_dlfile);

  if (cfgfile) {
    if(!xmms_cfg_read_string(cfgfile, "General", "code",&menu_code)){
      menu_code = NULL;
    }

    dialog = gtk_dialog_new();
    gtk_window_set_title(GTK_WINDOW(dialog), _("MaCoPiX : Download & Install Mascot"));
    gtk_container_set_border_width(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),5);
    gtk_window_set_title(GTK_WINDOW(dialog),_("MaCoPiX : Downloading Official Mascot List"));
    gtk_window_set_decorated(GTK_WINDOW(dialog),TRUE);

    label=gtkut_label_new(_("Select a mascot file to download &amp; install into your <i>User directory</i>."));
    gtkut_pos(label, POS_START, POS_CENTER);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		       label,FALSE,FALSE, 0);

#ifdef USE_WIN32
    tmp = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,USER_DIR, NULL);
#else
    tmp = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,USER_DIR, NULL);
#endif
    label=gtkut_label_new(tmp);
    gtkut_pos(label, POS_CENTER, POS_CENTER);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		       label,FALSE,FALSE, 0);
    g_free(tmp);

    label=gtkut_label_new(_("<span color=\"#FF0000\"><b>Please note that all existing files will be overwritten.</b></span>"));
    gtkut_pos(label, POS_START, POS_CENTER);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		       label,FALSE,FALSE, 0);
    
    label=gtkut_label_new("");
    gtkut_pos(label, POS_START, POS_CENTER);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		       label,TRUE,TRUE,0);

    
    scrwin= gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_set_size_request (scrwin, 400, 250);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		       scrwin,TRUE,TRUE,0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(scrwin),
				    GTK_POLICY_NEVER,
				    GTK_POLICY_AUTOMATIC);
    
    table = gtkut_table_new (7, MAX_TGZ_FILE, FALSE, 10, 10, 10);
#ifdef USE_GTK3
    gtk_container_add(GTK_CONTAINER(scrwin),table);
#else
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW (scrwin), table);
#endif
    
    label=gtkut_label_new(_("File"));
#ifdef USE_GTK3
    gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label, GTK_ALIGN_CENTER);
#else
    gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
#endif
    gtkut_table_attach(table, label, 0, 1, 0, 1,
		       GTK_SHRINK,GTK_SHRINK,0,0);

    label=gtkut_label_new(_("Name (E)"));
#ifdef USE_GTK3
    gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label, GTK_ALIGN_CENTER);
#else
    gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
#endif
    gtkut_table_attach(table, label, 1, 2, 0, 1,
		       GTK_SHRINK,GTK_SHRINK,0,0);

    tmp=g_strdup_printf(_("Name (%s)"), menu_code);
    label=gtkut_label_new(tmp);
#ifdef USE_GTK3
    gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label, GTK_ALIGN_CENTER);
#else
    gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
#endif
    gtkut_table_attach(table, label, 2, 3, 0, 1,
		       GTK_SHRINK,GTK_SHRINK,0,0);
    g_free(tmp);

    label=gtkut_label_new(_("Registered"));
#ifdef USE_GTK3
    gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label, GTK_ALIGN_CENTER);
#else
    gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
#endif
    gtkut_table_attach(table, label, 3, 4, 0, 1,
		       GTK_SHRINK,GTK_SHRINK,0,0);

    label=gtkut_label_new(_("Ver."));
#ifdef USE_GTK3
    gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label, GTK_ALIGN_CENTER);
#else
    gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
#endif
    gtkut_table_attach(table, label, 4, 5, 0, 1,
		       GTK_SHRINK,GTK_SHRINK,0,0);

    label=gtkut_label_new(_("Date"));
#ifdef USE_GTK3
    gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label, GTK_ALIGN_CENTER);
#else
    gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
#endif
    gtkut_table_attach(table, label, 5, 6, 0, 1,
		       GTK_SHRINK,GTK_SHRINK,0,0);

    label=gtkut_label_new(_("Code"));
#ifdef USE_GTK3
    gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (label, GTK_ALIGN_CENTER);
#else
    gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
#endif
    gtkut_table_attach(table, label, 6, 7, 0, 1,
		       GTK_SHRINK,GTK_SHRINK,0,0);


    list_tmp=g_strdup_printf("list-%03d", i_list);

    while((xmms_cfg_read_string(cfgfile, list_tmp, "file", &tgz_file[i_list])) && (i_list<MAX_TGZ_FILE-1)){

      if(!xmms_cfg_read_string(cfgfile, list_tmp, "ename", &ename)){
	ename=g_strdup("(<i>Not found</i>)");
      }

      if(!xmms_cfg_read_string(cfgfile, list_tmp, "lname", &dummy)){
	dummy=NULL;
      }
      if(dummy){
	dummy=x_locale_to_utf8(dummy,-1,NULL,NULL,NULL,menu_code);
	if(!dummy) dummy=g_strdup(_("(<i>Invalid Character Code</i>)"));
      }
      else{
	dummy=g_strdup(_("(<i>Invalid Character Code</i>)"));
      }
      lname=g_strdup(dummy);

      if(!xmms_cfg_read_string(cfgfile, list_tmp, "date", &date)){
	date=g_strdup("(<i>Not found</i>)");
      }
      
      if(!xmms_cfg_read_string(cfgfile, list_tmp, "ver", &ver)){
	ver=g_strdup("(<i>Not found</i>)");
      }

      if(!xmms_cfg_read_string(cfgfile, list_tmp, "code", &code)){
	code=g_strdup("(<i>Not found</i>)");
      }

      if(!xmms_cfg_read_int(cfgfile, list_tmp, "num", &num)){
	num=-1;
      }

      rb[i_list] = gtk_radio_button_new_with_label_from_widget ((i_list==0) ? NULL : GTK_RADIO_BUTTON(rb[0]),
								tgz_file[i_list]);
      gtkut_table_attach(table, rb[i_list], 0, 1, i_list+1, i_list+2,
			 GTK_FILL|GTK_EXPAND,GTK_SHRINK,0,0);
      gtk_widget_show (rb[i_list]);
      my_signal_connect (rb[i_list], "toggled", cc_radio, &i_selected);
      gtk_widget_show_all(rb[i_list]);
    
      label=gtkut_label_new(ename);
      gtkut_pos(label, POS_START, POS_CENTER);
      gtkut_table_attach(table, label, 1, 2, i_list+1, i_list+2,
			 GTK_EXPAND|GTK_FILL,GTK_SHRINK,0,0);

      label=gtkut_label_new(lname);
      gtkut_pos(label, POS_START, POS_CENTER);
      gtkut_table_attach(table, label, 2, 3, i_list+1, i_list+2,
			 GTK_EXPAND|GTK_FILL,GTK_SHRINK,0,0);
      
      if(num>0){
	tmp=g_strdup_printf(_("%d mascot"), num);
      }
      else{
	tmp=g_strdup(_("<i>unkown</i>"));
      }
      label=gtkut_label_new(_(tmp));
      gtkut_pos(label, POS_CENTER, POS_CENTER);
      gtkut_table_attach(table, label, 3, 4, i_list+1, i_list+2,
			 GTK_SHRINK,GTK_SHRINK,0,0);
      g_free(tmp);
      
      label=gtk_label_new(ver);
      gtkut_pos(label, POS_CENTER, POS_CENTER);
      gtkut_table_attach(table, label, 4, 5, i_list+1, i_list+2,
			 GTK_SHRINK,GTK_SHRINK,0,0);
      
      label=gtk_label_new(date);
      gtkut_pos(label, POS_CENTER, POS_CENTER);
      gtkut_table_attach(table, label, 5, 6, i_list+1, i_list+2,
			 GTK_SHRINK,GTK_SHRINK,0,0);

      label=gtk_label_new(code);
      gtkut_pos(label, POS_CENTER, POS_CENTER);
      gtkut_table_attach(table, label, 6, 7, i_list+1, i_list+2,
			 GTK_SHRINK,GTK_SHRINK,0,0);
      
           
      g_free(ename);
      g_free(lname);
      g_free(date);
      g_free(ver);
      g_free(code);
      g_free(list_tmp);

      i_list++;
      list_tmp=g_strdup_printf("list-%03d", i_list);
    }

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rb[0]),TRUE);
    i_list_max=i_list;
    g_free(list_tmp);

    xmms_cfg_free(cfgfile);
    
#ifdef USE_GTK3
    button=gtkut_button_new_from_icon_name(_("Download / Install"), "document-save");
#else
    button=gtkut_button_new_from_stock(_("Download / Install"), GTK_STOCK_OK);
#endif
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog),button,GTK_RESPONSE_OK);

#ifdef USE_GTK3
    button=gtkut_button_new_from_icon_name(_("Cancel"), "process-stop");
#else
    button=gtkut_button_new_from_stock(_("Cancel"), GTK_STOCK_CANCEL);
#endif
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog),button,GTK_RESPONSE_CANCEL);
    
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
      gtk_widget_destroy(dialog);

      if(access(mascot->http_dlfile,F_OK)==0){
	unlink(mascot->http_dlfile);
      }

      if(mascot->http_host) g_free(mascot->http_host);
      mascot->http_host=g_strdup(HTTP_MASCOT_HOST);
      
      if(mascot->http_path) g_free(mascot->http_path);
      mascot->http_path=g_strconcat(HTTP_MASCOT_PATH, tgz_file[i_selected], NULL);
      
      if(mascot->http_dlfile) g_free(mascot->http_dlfile);
      mascot->http_dlfile=g_strdup_printf("%s%s%s",
					  g_get_tmp_dir(), G_DIR_SEPARATOR_S,
					  tgz_file[i_selected]);
      if(access(mascot->http_dlfile, F_OK)==0) unlink(mascot->http_dlfile);
	 
      dl_mascot_tgz(mascot);

      if((mascot->http_ok)&&(access(mascot->http_dlfile,F_OK)==0)){
	flag_install=mascot->flag_install;

	select_menu_from_ext(mascot, mascot->http_dlfile);
	mascot->flag_install=TRUE;
	mascot->flag_common=FALSE;
	mascot->flag_ow=TRUE;
	mascot->flag_ow_ini=TRUE;
	
	gtk_widget_destroy(mascot->PopupMenu);
	ReadMenu(mascot,0,NULL);
	mascot->PopupMenu=make_popup_menu(mascot);
	if(flag_popup){// Popupから
	  AllRandomChangeMascotMenu(NULL, (gpointer)mascot);
	}
	
	unlink(mascot->http_dlfile);
	
	mascot->flag_menu=FALSE;

	mascot->flag_install=flag_install;
	mascot->flag_common=FALSE;
      }
      else{
	tmp=g_strconcat(mascot->http_host,
			mascot->http_path,
			NULL);
	popup_message(NULL,
#ifdef USE_GTK3
		      "dialog-error", 
#else
		      GTK_STOCK_DIALOG_ERROR,
#endif
		      -1,
		      _("Error : Failed to download the mascot archive file (tar.gz)."),
		      " ",
		      tmp,
		      NULL);
	g_free(tmp);
      }
   }
    else{
      gtk_widget_destroy(dialog);
      
      if(access(mascot->http_dlfile,F_OK)==0){
	unlink(mascot->http_dlfile);
      }
    }
  }
  else{
    popup_message(NULL,
#ifdef USE_GTK3
		  "dialog-error", 
#else
		  GTK_STOCK_DIALOG_ERROR,
#endif
		  -1,
		  _("Error : Cannot read the downloaded mascot list."),
		  " ",
		  mascot->http_dlfile,
		  NULL);
  }
}


void InitMascot(typMascot *mascot)
{
  int i_ptn,i_frm, i_pix;
#ifdef USE_SOCKMSG
  SockMsgInitResult sockres;

  // マスコットアクション用 ソケット
  if(mascot->duet_open){
    duet_sv_done(mascot->duet_file,TRUE);
    mascot->duet_open=FALSE;
  }
  {
    sockres = duet_sv_init((gchar *)my_basename(mascot->file));
    switch (sockres) {
    case SOCKMSG_ERROR:
      mascot->duet_open=FALSE;
      break;
    case SOCKMSG_OPENED:
      // すでにソケットが存在。
      //mascot->duet_open=FALSE;
      mascot->duet_open=TRUE;
      mascot->duet_file=g_strdup(my_basename(mascot->file));
      duet_set_mascot(mascot);
      break;
    default:
      mascot->duet_open=TRUE;
      mascot->duet_file=g_strdup(my_basename(mascot->file));
      duet_set_mascot(mascot);

      break;
    }
  }
#endif // USE_SOCKMSG
  
  for(i_pix=0;i_pix<MAX_PIXMAP;i_pix++){
#ifdef USE_GTK3
    if(mascot->sprites[i_pix].pixbuf) g_object_unref(G_OBJECT(mascot->sprites[i_pix].pixbuf));
    mascot->sprites[i_pix].pixbuf=NULL;
#else
    if(mascot->sprites[i_pix].pixmap) g_object_unref(G_OBJECT(mascot->sprites[i_pix].pixmap));
    mascot->sprites[i_pix].pixmap=NULL;
    if(mascot->sprites[i_pix].mask) g_object_unref(G_OBJECT(mascot->sprites[i_pix].mask));
    mascot->sprites[i_pix].mask=NULL;
#endif
  }

  mascot->xoff=0;
  mascot->yoff=0;
  mascot->xfix=0;
  mascot->yfix=0;

#ifdef USE_WIN32
  mascot->sdw_height=0;
#endif

  mascot->test_timer=-1;
  
  mascot->anime_ptn=0;
  mascot->anime_frm=0;
  mascot->anime_seq=0;
  mascot->anime_seqend=-1;
  mascot->random_total=0;
  mascot->click_total=0;

  mascot->clkmode=CLOCK_NO;
  mascot->clkmode=CLOCK_TYPE_24S;
  mascot->clk_x=INIT_CLK_POS;
  mascot->clk_y=INIT_CLK_POS;
  mascot->clktext_x=INIT_CLK_TEXT;
  mascot->clktext_y=INIT_CLK_TEXT;
  mascot->clksd_x=INIT_CLK_SD;
  mascot->clksd_y=INIT_CLK_SD;
  mascot->flag_clksd=TRUE;
  mascot->flag_clkrd=TRUE;
  mascot->wclkbd = INIT_CLK_BORDER;
  if(mascot->fontname_clk){
    g_free(mascot->fontname_clk);
  }
  mascot->fontname_clk = NULL;


  mascot->baltext_x=INIT_BAL_TEXT;
  mascot->baltext_y=INIT_BAL_TEXT;
  mascot->wbalbd = INIT_BAL_BORDER;
  mascot->bal_defpos = BAL_POS_LEFT;
  mascot->bal_pos=mascot->bal_defpos;
  if(mascot->fontname_bal){
    g_free(mascot->fontname_bal);
  }
  mascot->fontname_bal = NULL;

#ifdef USE_GTK3
  mascot->colclk=gdk_rgba_copy(mascot->def_colclk);
  mascot->colclksd=gdk_rgba_copy(mascot->def_colclksd);
  mascot->colclkbg=gdk_rgba_copy(mascot->def_colclkbg);
  mascot->colclkbd=gdk_rgba_copy(mascot->def_colclkbd);

  mascot->colbal=gdk_rgba_copy(mascot->def_colbal);
  mascot->colbalbg=gdk_rgba_copy(mascot->def_colbalbg);
  mascot->colbalbd=gdk_rgba_copy(mascot->def_colbalbd);
#else
  mascot->colclk=gdk_color_copy(mascot->def_colclk);
  mascot->colclksd=gdk_color_copy(mascot->def_colclksd);
  mascot->colclkbg=gdk_color_copy(mascot->def_colclkbg);
  mascot->colclkbd=gdk_color_copy(mascot->def_colclkbd);

  mascot->colbal=gdk_color_copy(mascot->def_colbal);
  mascot->colbalbg=gdk_color_copy(mascot->def_colbalbg);
  mascot->colbalbd=gdk_color_copy(mascot->def_colbalbd);
#endif
#ifdef USE_BIFF
  if(mascot->mail.pix_file){
    g_free(mascot->mail.pix_file);
  }
  mascot->mail.pix_file=NULL;
  mascot->mail.pix_pos=MAIL_PIX_RIGHT;
  mascot->mail.pix_x=0;
  mascot->mail.pix_y=0;
  if(mascot->mail.word){
    g_free(mascot->mail.word);
  }
  mascot->mail.word=NULL;
  if(mascot->mail.sound){
    g_free(mascot->mail.sound);
  }
  mascot->mail.sound=NULL;
#endif // USE_BIFF

#ifdef USE_SOCKMSG
  mascot->duet_seq=-1;
  mascot->duet_mode=DUET_RANDOM;
#endif  // USE_SOCKMSG

  if(mascot->code){
    g_free(mascot->code);
  }
  mascot->code=NULL;

  for(i_ptn=0;i_ptn<MAX_ANIME_PATTERN;i_ptn++){
      mascot->frame_num[i_ptn]=0;
      mascot->random_weight[i_ptn]=0;
      mascot->click_weight[i_ptn]=0;
      mascot->bal_lxoff[i_ptn]=0;
      mascot->bal_lyoff[i_ptn]=0;
      mascot->bal_rxoff[i_ptn]=0;
      mascot->bal_ryoff[i_ptn]=0;
      if(mascot->click_word[i_ptn]){
	g_free(mascot->click_word[i_ptn]);
      }
      mascot->click_word[i_ptn]=NULL;
      if(mascot->click_sound[i_ptn]){
	g_free(mascot->click_sound[i_ptn]);
      }
      mascot->click_sound[i_ptn]=NULL;
#ifdef USE_SOCKMSG
      if(mascot->duet_tgt[i_ptn]){
	g_free(mascot->duet_tgt[i_ptn]);
      }
      mascot->duet_tgt[i_ptn]=NULL;
      mascot->duet_ptn[i_ptn]=1;
      if(mascot->duet_word[i_ptn]){
	g_free(mascot->duet_word[i_ptn]);
      }
      mascot->duet_word[i_ptn]=NULL;
      mascot->duet_delay[i_ptn]=DEF_DUET_DELAY;
#endif  // USE_SOCKMSG
    for(i_frm=0;i_frm<MAX_ANIME_FRAME;i_frm++){
      mascot->frame_pix[i_ptn][i_frm]=-1;
      mascot->frame_min[i_ptn][i_frm]=0;
      mascot->frame_max[i_ptn][i_frm]=0;
      mascot->frame_loop[i_ptn][i_frm].next=-1;
      mascot->frame_loop[i_ptn][i_frm].min=0;
      mascot->frame_loop[i_ptn][i_frm].max=0;
      mascot->frame_loop[i_ptn][i_frm].seq=0;
      mascot->frame_loop[i_ptn][i_frm].seqend=0;
    }
  }
}


void get_option(int argc, char **argv, typMascot *mascot)
{
  int i_opt;
  int valid=1;
  gchar *filename=NULL;

  mascot->file=NULL;
  mascot->flag_common=FALSE;
  
  //if(argc<2) usage();


  i_opt = 1;
  while((i_opt < argc)&&(valid==1)) {
    // --- X offset
    if((strcmp(argv[i_opt],"-o") == 0)||
	    (strcmp(argv[i_opt],"--offset") == 0)){ 
      if(i_opt+1 < argc ) {
	char* chkRF;
	gboolean flag;
	i_opt++;
	mascot->offset=strtol(argv[i_opt],&chkRF,10);
	if((flag='%')==*chkRF){
	  mascot->flag_xp=FF_BAR_REL;
	  mascot->offsetp=mascot->offset;
	  mascot->offset=0;
	  if((mascot->offsetp<0 )||(100<mascot->offsetp))mascot->offsetp=10;
	}
	else{
	  mascot->flag_xp=FF_BAR_ABS;
	}
	i_opt++;
      }
      else{
	valid = 0;
      }
    }
    // --- Menu File
    else if((strcmp(argv[i_opt],"-m") == 0)||
	    (strcmp(argv[i_opt],"--menu") == 0)){ 
      if(i_opt+1 < argc ) {
	i_opt++;
	mascot->menu_file=g_strdup(argv[i_opt]);
	i_opt++;
      }
      else{
	valid = 0;
      }
    }
    // --- Install Menu File
    else if((strcmp(argv[i_opt],"--install-user-menu") == 0)){ 
      if(i_opt+1 < argc ) {
	mascot->flag_install=TRUE;
	mascot->flag_common=FALSE;
	mascot->flag_ow=TRUE;
	mascot->flag_ow_ini=TRUE;
	i_opt++;
	mascot->menu_file=g_strdup(argv[i_opt]);
	i_opt++;
      }
      else{
	valid = 0;
      }
    }
    // --- Install Mascot File
    else if((strcmp(argv[i_opt],"--install-user-mascot") == 0)){ 
      if(i_opt+1 < argc ) {
	mascot->flag_install=TRUE;
	mascot->flag_common=FALSE;
	mascot->flag_ow=TRUE;
	mascot->flag_ow_ini=TRUE;
	i_opt++;
	if(!FullPathMascotCheck(mascot,argv[i_opt])){
	  g_print(_("Option \"%s\" is not found. Ignoring...\n"),argv[i_opt]);
	}
	i_opt++;
      }
      else{
	valid = 0;
      }
    }
#ifdef USE_COMMON
    // --- Install Menu File to Common Directory
    else if((strcmp(argv[i_opt],"--install-common-menu") == 0)){ 
      if(i_opt+1 < argc ) {
	mascot->flag_install=TRUE;
	mascot->flag_common=TRUE;
	mascot->flag_ow=TRUE;
	mascot->flag_ow_ini=TRUE;
	i_opt++;
	mascot->menu_file=g_strdup(argv[i_opt]);
	i_opt++;
      }
      else{
	valid = 0;
      }
    }
    // --- Install Mascot File to Common Directory
    else if((strcmp(argv[i_opt],"--install-common-mascot") == 0)){ 
      if(i_opt+1 < argc ) {
	mascot->flag_install=TRUE;
	mascot->flag_common=TRUE;
	mascot->flag_ow=TRUE;
	mascot->flag_ow_ini=TRUE;
	i_opt++;
	if(!FullPathMascotCheck(mascot,argv[i_opt])){
	  g_print(_("Option \"%s\" is not found. Ignoring...\n"),argv[i_opt]);
	}
	i_opt++;
      }
      else{
	valid = 0;
      }
    }
#endif
    // --- Magnification [%]
    else if((strcmp(argv[i_opt],"-mg") == 0)||
	    (strcmp(argv[i_opt],"--mag") == 0)){ 
      if(i_opt+1 < argc ) {
	i_opt++;
	mascot->magnify=atoi(argv[i_opt]);
	if(mascot->magnify<10 )mascot->magnify=10;
	if(mascot->magnify>400 )mascot->magnify=400;
	i_opt++;
      }
      else{
	valid = 0;
      }
    }
    // --- Interporation Style [%]
    else if((strcmp(argv[i_opt],"-i") == 0)||
	    (strcmp(argv[i_opt],"--ip-style") == 0)){ 
      if(i_opt+1 < argc ) {
	i_opt++;
	mascot->ip_style=atoi(argv[i_opt]);
	if((mascot->ip_style<MAG_IP_NEAREST)||
	   (mascot->ip_style>MAG_IP_HYPER))
	  mascot->ip_style=MAG_IP_BILINEAR;
	i_opt++;
      }
      else{
	valid = 0;
      }
    }
    // 自動インストール ON  
    else if ((strcmp(argv[i_opt], "-a") == 0)||
	     (strcmp(argv[i_opt], "--auto-install") == 0)){
      mascot->flag_install=TRUE;
      i_opt++;
    }
    // 自動インストール OFF
    else if ((strcmp(argv[i_opt], "-na") == 0)||
	     (strcmp(argv[i_opt], "--no-auto-install") == 0)){
      mascot->flag_install=FALSE;
      i_opt++;
    }
    // 強制上書き自動インストール ON  
    else if ((strcmp(argv[i_opt], "-O") == 0)||
	     (strcmp(argv[i_opt], "--over-write") == 0)){
      mascot->flag_ow=TRUE;
      mascot->flag_ow_ini=TRUE;
      i_opt++;
    }
#ifdef USE_BIFF
    // Biff機能 ON  
    else if ((strcmp(argv[i_opt], "-b") == 0)||
	     (strcmp(argv[i_opt], "--biff") == 0)){
      mascot->mail.flag=TRUE;
      i_opt++;
    }
#endif
    // 時報機能 ON  
    else if ((strcmp(argv[i_opt], "-s") == 0)||
	     (strcmp(argv[i_opt], "--signal") == 0)){
      mascot->signal.flag=TRUE;
      i_opt++;
    }
    // --- Resource File
    else if((strcmp(argv[i_opt],"-r") == 0)||
	    (strcmp(argv[i_opt],"--rcfile") == 0)){ 
      if(i_opt+1 < argc ) {
	i_opt++;
	mascot->rcfile=g_strconcat(argv[i_opt],NULL);
	i_opt++;
      }
      else{
	valid = 0;
      }
    }
#ifdef USE_SOCKMSG
    // Socket Message Mode ON
    else if((strcmp(argv[i_opt],"--sockmsg") == 0)){
      //Do Nothing
      i_opt++;
    }
    // Duet Anime Mode [Click] OFF
    else if((strcmp(argv[i_opt],"--no-duet-click") == 0)||
	    (strcmp(argv[i_opt],"-nc") ==0)){
      mascot->duet_use_click=FALSE;
      i_opt++;
    }
    // Duet Anime Mode [Random] OFF
    else if((strcmp(argv[i_opt],"--no-duet-random") == 0)||
	    (strcmp(argv[i_opt],"-nr") ==0)){
      mascot->duet_use_random=FALSE;
      i_opt++;
    }
#endif  // USE_SOCKMSG
    // 定型以外のオプション
    else{
      // MascotFileであるかどうかのチェック
      // mascot->file への書き込みを行う
      if(!FullPathMascotCheck(mascot,argv[i_opt])){
	g_print(_("Option \"%s\" is not found. Ignoring...\n"),argv[i_opt]);
      }
      i_opt++;
    }
  }
  
  g_free(filename);
  //if(!flag_file)   exit(0);
}

void get_rc_option(int argc, char **argv, typMascot *mascot)
{
  int i_opt;
  int valid=1;
  

  i_opt = 1;
  while((i_opt < argc)&&(valid==1)) {
    // --- ヘルプスイッチ
    if ((strcmp(argv[i_opt], "-h") == 0) ||
        (strcmp(argv[i_opt], "--help") == 0)) {
      i_opt++;
      usage();
      exit(0);
    }
#ifdef USE_COMMON
    // --- 共通ディレクトリの表示
    else if((strcmp(argv[i_opt],"-c") == 0)||
	    (strcmp(argv[i_opt],"--common-dir") == 0)){ 
      print_common_dir();
    }
#endif
    // --- リソースファイル
    else if((strcmp(argv[i_opt],"-r") == 0)||
       (strcmp(argv[i_opt],"--rcfile") == 0)){ 
      if(i_opt+1 < argc ) {
	i_opt++;
	mascot->rcfile=g_strdup(argv[i_opt]);
	i_opt++;
      }
      else{
	valid = 0;
      }
    }
    i_opt++;
  }
  
}

#ifdef USE_COMMON
void print_common_dir(void)
{
  g_print(_("macopix COMMON DIR:%s\n"),COMMON_DIR);
  exit(0);
}
#endif

void usage(void)
{
  typScanMenu *smenu;

  smenu=g_malloc0(sizeof(typScanMenu));

  g_print(_(" macopix : Mascot Constructive Pilot for X   Ver"VERSION"\n"));
  g_print(_("  [usage] %% macopix [options] (mascot file)\n"));
  g_print(_("     -h, --help               : Print this message\n"));
#ifdef USE_COMMON
  g_print(_("     -c, --common-dir         : Print COMMON Directory\n"));
#endif
#ifdef USE_BIFF
  g_print(_("     -b, --biff               : Turn on the biff function\n"));
#endif  // USE_BIFF
  g_print(_("     -s, --signal             : Turn on the time signal\n"));
  g_print(_("     -o, --offset offset[%%]   : Set offset-value for Focus Following\n"));
  g_print(_("     -r, --rcfile rc-file     : Set the resource file\n"));
  g_print(_("     -m, --menu   menu-file   : Set the file for quick menu\n"));
  g_print(_("     -mg --mag    magnify     : Set magnification percentage of mascot\n"));
  g_print(_("     -i, --ip-style (0|1|2|3) : Set interpolation style for magnification\n"));
  g_print(_("## For Installation\n"));
  g_print(_("     -a, --auto-install       : Set mascot auto install ON\n"));
  g_print(_("     -na --no-auto-install    : Set mascot auto install OFF\n"));
  g_print(_("     -O, --over-write         : Permit to overwrite files in user dir\n"));
  g_print(_("     --install-user-mascot (mascot)  : Mascot Installation to User Dir.\n"));
  g_print(_("     --install-user-menu (menu)      : Menu Installation to User Dir.\n"));
#ifdef USE_COMMON
  g_print(_("     --install-common-mascot (mascot): Mascot Installation to Common Dir.\n"));
  g_print(_("     --install-common-menu (menu)    : Menu Installation to Common Dir.\n"));
#endif
#ifdef USE_SOCKMSG
  g_print(_("## For Socket Message\n"));
  g_print(_("     --sockmsg                : Start MaCoPiX as Socket Message Receiver\n"));
  g_print(_("     --message (text)         : Send Message to Socket Message Receiver\n"));
  g_print(_("     --message-expire (time)  : Socket Message Expire Time[ms]\n"));
  g_print(_("     --message-type-lump      : Force to Set Lump Sum Balloon\n"));
  g_print(_("     --message-type-step      : Force to Set Stepping Balloon\n"));
  g_print(_("     --message-step (frame)   : Stepping Interval Frame for SockeMsg\n"));
  g_print(_("     -nc, --no-duet-click     : Duet Animation [Click] OFF\n"));
  g_print(_("     -nr, --no-duet-random    : Duet Animation [Random] OFF\n"));
#endif  // USE_SOCKMSG

  g_print("\n");

  ScanMenu(smenu, TRUE);

}

int main(int argc, char **argv)
{
#ifndef USE_WIN32  
  GdkPixbuf *icon;
#endif
  gint timer;
  gchar *rcfile=NULL;
  int i_opt ;
#ifdef USE_SOCKMSG
  SockMsgInitResult sockres;
  gchar *msg = NULL;
  gboolean sockmsg_flag;
  gint msg_expire;
  gint msg_step;
  gint msg_mode;
  gchar *tmp_msg;
#endif
#ifdef USE_WIN32
#ifdef USE_BIFF
  WSADATA wsaData;
  int nErrorStatus;
#endif
#endif

#ifdef USE_SOCKMSG
  sockmsg_flag=FALSE;
  i_opt=1;
  msg_expire=-1;
  msg_step=-1;
  msg_mode=-1;
#endif

  // Initialize Global Args in Each Files
  FlagInstalledMenu=FALSE;
  gui_arg_init();
#ifdef USE_BIFF
  mail_arg_init();
#endif
  balloon_arg_init();
  callbacks_arg_init();


  // GetText
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  bind_textdomain_codeset(PACKAGE, "UTF-8");

#ifdef USE_SOCKMSG
  while(i_opt < argc) {
    if((strcmp(argv[i_opt],"--message") == 0)){
      msg = argv[i_opt + 1];
      sockmsg_flag=TRUE;
    }
    else if((strcmp(argv[i_opt],"--sockmsg") == 0)){
      sockmsg_flag=TRUE;
    }
    else if((strcmp(argv[i_opt],"--message-expire") == 0)){
      msg_expire = atoi(argv[i_opt + 1]);
    }
    else if((strcmp(argv[i_opt],"--message-type-lump") == 0)){
      msg_mode=SOCK_NORMAL;
    }
    else if((strcmp(argv[i_opt],"--message-type-step") == 0)){
      msg_mode=SOCK_STEPPING;
    }
    else if((strcmp(argv[i_opt],"--message-step") == 0)){
      msg_step = atoi(argv[i_opt + 1]);
      msg_mode=SOCK_STEPPING;
    }
    i_opt++;
  }


  if (sockmsg_flag){
    //if(msg_expire<=0){
    //  msg_expire=(mascot->sockmsg_expire_def)*(INTERVAL);
    //}

    sockres = sockmsg_init();
    switch (sockres) {
    case SOCKMSG_ERROR:
      return -1;
    case SOCKMSG_OPENED:
      {
	if (msg == NULL)
	  tmp_msg=g_strdup_printf(_("%8d%2d%4dMacopix already running in SockMsg Mode"),
				  msg_expire,msg_mode,msg_step);
	else
	  tmp_msg=g_strdup_printf("%8d%2d%4d%s",
				  msg_expire,msg_mode,msg_step,msg);
	
	sockmsg_send_msg(tmp_msg);
	g_free(tmp_msg);
	return 0;
      } 
    default:
      if (msg != NULL){
	g_print(_("Macopix not running in SockMsg Mode\n"));
	return -1;
      }
      break;
    }
  }
#endif


  RANDOMIZE();    /* 乱数系初期化 */

  Mascot=g_malloc0(sizeof(typMascot));

  get_rc_option(argc, argv, Mascot);

#ifndef USE_GTK3
  gtk_set_locale();
#endif

#ifdef USE_GTK3
#ifndef USE_WIN32
#ifndef USE_OSX
  gdk_set_allowed_backends("x11");
#endif
#endif
#endif

  gtk_init(&argc, &argv);

#ifdef USE_GTK3
  pixbuf_main=NULL;
  pixbuf_clk=NULL;
  pixbuf_bal=NULL;
#ifdef FG_DRAW
  pixbuf_sdw=NULL;
#endif
#else
  pixmap_main=NULL;
  pixmap_clk=NULL;
  pixmap_bal=NULL;
#ifdef FG_DRAW
  pixmap_sdw=NULL;
#endif
#endif
  
  InitDefCol(Mascot);

  ReadRC(Mascot,FALSE);

  Mascot->http_host=NULL;
  Mascot->http_path=NULL;
  Mascot->http_dlfile=NULL;
#ifndef USE_WIN32  
  Mascot->url_command=g_strdup(DEF_OPEN_URL);
#endif

#ifdef USE_BIFF
  Mascot->mail.flag=FALSE;
#endif
  Mascot->signal.flag=FALSE;
  Mascot->flag_consow=FALSE;
  get_option(argc, argv, Mascot);


  // Gdk-Pixbufで使用
#ifndef USE_GTK3
  gdk_rgb_init();
#endif

#ifndef USE_WIN32
#ifdef USE_GTK3
  icon = gdk_pixbuf_new_from_resource ("/icons/macopix_icon.png", NULL);
#else
  icon = gdk_pixbuf_new_from_resource ("/icons/macopix_icon_s.png", NULL);
#endif
  gtk_window_set_default_icon(icon);
#endif

  // Imlibを使用した場合
  //gdk_imlib_init();
  //gtk_widget_push_visual(gdk_imlib_get_visual());
  //gtk_widget_push_colormap(gdk_imlib_get_colormap());

  create_conf_num(Mascot);

  // メニューの読み込みと作成
  Mascot->installed_menu_dir=NULL;
  Mascot->menu_code=NULL;
  ReadMenu(Mascot,0,NULL);
  Mascot->PopupMenu=make_popup_menu(Mascot);
  
#ifdef USE_WIN32   // Initialize Winsock2
  nErrorStatus = WSAStartup(MAKEWORD(2,0), &wsaData);
  if(atexit((void (*)(void))(WSACleanup))){
    fprintf(stderr, "WSACleanup() : Failed\n");
    exit(-1);
  }
  if (nErrorStatus!=0) {
      fprintf(stderr, "WSAStartup() : Failed\n");
      exit(-1);
  }
#endif
#ifdef USE_SSL
  ssl_init();
#endif
    
  if(!Mascot->file){
    if(Mascot->menu_file){
      Mascot->file=g_strdup(all_random_menu_mascot_file(Mascot));
    }
    else{
      // 起動メニュー選択
      create_smenu_dialog(Mascot,FALSE);

      if(Mascot->menu_file){
	ReadMenu(Mascot,0,NULL);
	Mascot->PopupMenu=make_popup_menu(Mascot);
	Mascot->file=g_strdup(all_random_menu_mascot_file(Mascot));
      }
    }
  }
 

  make_mascot(Mascot);
  
  make_balloon(Mascot);
#ifdef FG_DRAW
  make_balloon_fg(Mascot);
#endif
  make_clock(Mascot);
#ifdef FG_DRAW
  make_clock_fg(Mascot);
#endif
#ifdef USE_BIFF
  make_biff_pix(Mascot);
#endif

  InitComposite(Mascot);
  LoadPixmaps(Mascot);  

  Mascot->flag_ow=FALSE;

#ifdef USE_BIFF
  LoadBiffPixmap(Mascot->biff_pix, Mascot);
#ifndef __GTK_TOOLTIP_H__
  Mascot->mail.tooltips=gtk_tooltips_new();
#endif
#endif

  // 時計更新
  timer=g_timeout_add_full(G_PRIORITY_HIGH,
			   INTERVAL, 
			   (GSourceFunc)time_update, 
			   (gpointer)Mascot,
			   NULL);


  // 初期配置
  if(Mascot->move==MOVE_FIX){
    MoveMascot(Mascot,Mascot->xfix,Mascot->yfix);
  }
  else{
    MoveMascot(Mascot,-Mascot->height-100,-Mascot->width-100);
  }

  gdk_window_set_cursor(gtk_widget_get_window(Mascot->win_main),
			Mascot->cursor.normal);
#ifdef USE_BIFF
  gdk_window_set_cursor(gtk_widget_get_window(Mascot->biff_pix),Mascot->cursor.biff);
#endif // USE_BIFF

#ifdef FG_DRAW
  gtk_widget_show_all(Mascot->win_sdw);
#endif
  gtk_widget_show_all(Mascot->win_main);
  map_main(Mascot, TRUE);

#ifdef FG_DRAW
  gtk_widget_show_all(Mascot->balloon_fg);
#endif
  gtk_widget_show_all(Mascot->balloon_main);
  map_balloon(Mascot, FALSE);

#ifdef FG_DRAW
  gtk_widget_show_all(Mascot->clock_fg);
#endif
  gtk_widget_show_all(Mascot->clock_main);
    
  if(Mascot->clkmode!=CLOCK_PANEL){
    map_clock(Mascot, FALSE);
  }
  else{
    DrawPanelClock0(Mascot);
    map_clock(Mascot, TRUE);
  }

  if(Mascot->clkmode!=CLOCK_NO){
    clock_update(Mascot,TRUE);
    clock_update(Mascot,TRUE);
  }

#ifdef USE_BIFF
  gtk_widget_show_all(Mascot->biff_pix);
  map_biff(Mascot, FALSE);
#endif // USE_BIFF


#ifndef USE_WIN32
  signal( SIGPIPE , SIG_IGN ); 
#endif

#ifdef USE_BIFF
  SetMailChecker((gpointer)Mascot);
#endif

#ifdef USE_SOCKMSG
  if(sockmsg_flag) sockmsg_set_mascot(Mascot);
#endif

#ifdef USE_OSX
  MacSetLayer(Mascot);
#endif

  gtk_main();

#ifdef USE_SOCKMSG
  if(sockmsg_flag) sockmsg_done();
  if(Mascot->duet_open) duet_sv_done(Mascot->duet_file,TRUE);
#endif

#ifdef USE_SSL
  ssl_done();
#endif

  return 0;
}
  
