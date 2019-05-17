//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//
//
//     gui.c  
//     Constraction & Management of Graphical User Interfaces
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

// 画像用tree
enum
{
  COLUMN_IMGTREE_NUMBER,
  COLUMN_IMGTREE_FILENAME,
  COLUMN_IMGTREE_COLFG,
  COLUMN_IMGTREE_COLBG,
  NUM_IMGTREE_COLUMNS
};

enum
{
  COLUMN_PTNTREE_NUMBER,
  COLUMN_PTNTREE_IMAGE,
  COLUMN_PTNTREE_MIN,
  COLUMN_PTNTREE_MAX,
  COLUMN_PTNTREE_BL_NEXT,
  COLUMN_PTNTREE_BL_MIN,
  COLUMN_PTNTREE_BL_MAX,
  COLUMN_PTNTREE_COLFG,
  COLUMN_PTNTREE_COLBG,
  NUM_PTNTREE_COLUMNS
};



//Callback受け渡し用抱き合わせ構造体 フォント変更用
typedef struct{
  gchar *fn;
  GtkFontSelectionDialog *fsd;
  GtkWidget *entry;
}confFont;

//Callback受け渡し用抱き合わせ構造体 ファイル保存上書き確認用
typedef struct{
  GtkWidget *dialog;
  gboolean update;
}confSaveOther;

//Callback受け渡し用抱き合わせ構造体 テンポラリディレクトリの削除
typedef struct{
  GtkWidget *dialog;
  gboolean update;
}confUnlink;


//Callback受け渡し用抱き合わせ構造体 リリース用ファイル保存用
typedef struct{
  GtkWidget *dialog;
  GtkWidget *fdialog;
  GtkWidget *copyright_entry;
  GtkWidget *name_entry;
  GtkWidget *code_entry;
  GtkWidget *clk_font_check;
  GtkWidget *bal_font_check;
  GtkWidget *clk_color_check;
  GtkWidget *bal_color_check;
  gboolean update;
  gchar *new_copyright;
  gchar *new_name;
  gchar *new_code;
  gboolean clk_font_flag;
  gboolean bal_font_flag;
  gboolean clk_color_flag;
  gboolean bal_color_flag;
}confSaveReleaseMascot;

//Callback受け渡し用抱き合わせ構造体 
typedef struct{
  GtkWidget *dialog;
  GtkWidget *auto_check;
  gchar *filename;
  gboolean flag_ow;
  gboolean flag_auto;
}consType;



// *** GLOBAL ARGUMENT ***
GtkTooltip *tooltip;
gboolean flag_make_pattern_list[MAX_ANIME_PATTERN];
gboolean flag_make_frame_list[MAX_ANIME_PATTERN];
gboolean flag_make_cat_list[MAX_MENU_CATEGORY];
gboolean flag_make_tgt_list[MAX_MENU_CATEGORY];
gint selected_smenu;

// Need to Initialize
gboolean flagChildDialog;
gboolean flag_make_pixmap_list;
gboolean flag_make_imgtree;
gboolean flag_make_ptntree[MAX_ANIME_PATTERN];

// Prototype of Functions in this file
void MenuSaveRC();
void MenuSaveMascot();
void MenuSaveDefMenu();
void MenuDelDefMenu();
void MoveTmpDataRC();
void MoveTmpDataMascot();
void MoveTmpDataMenu();
static void conf_change();
//void change_fgcolor();
static void cc_get_combo_box();
static void cc_get_autobar_combo();
static void cc_get_spam_combo();
static void cc_get_combo_box_boolean();
static void cc_move();
static void cc_clkmode ();
static void cc_clktype ();
void cc_radio();
static void cc_homepos ();
static void cc_autobar ();
static void cc_ip_style();
#ifdef USE_COMMON
static void cc_cons_check_mode();
#endif
static void cc_bal_defpos();
#ifdef USE_BIFF
static void cc_mail_pixpos();
#endif  // USE_BIFF
static void cc_ff_side();
#ifdef USE_SOCKMSG
static void cc_sockmsg_type();
#endif
static void cc_get_adj();
static void cc_get_double();
static void cc_get_entry();
#ifdef USE_BIFF
static void cc_mail_type();
#ifdef USE_SSL
static void cc_mail_ssl();
#endif
#endif  // USE_BIFF
static void cc_signal_type();
static void cc_get_toggle();
static void cc_get_toggle_nb();
static void cc_get_toggle_home();
static void cc_get_toggle_sdw();
static void check_menu_get_toggle();
static void check_menu_get_smenu();
#ifdef USE_BIFF
static void mail_flag_toggle();
#endif  // USE_BIFF

static void test_click_sound();
static void clear_click_sound();
#ifdef USE_BIFF
static void test_biff_sound();
static void clear_biff_sound();
#endif

static void mc_up_tgt();
static void mc_down_tgt();
static void mc_up_cat();
static void mc_down_cat();

#ifdef USE_WIN32
static void uri_clicked();
#endif

void my_file_chooser_add_filter();
void my_file_chooser_add_shortcut_folder();
void my_file_chooser_set_current_folder();

void ChangeMascotFS();
void NkrChangeMascotFS();
void ChangeMenuFS();
void MakeMenuFS();
void ChangeMascotMenu();
void CatRandomChangeMascotMenu();
void ConfirmSaveOther();
void SaveMascotOther();
void SaveMenuOther();
void SaveRCOther();
void ChangeFontButton();
void ChangeColorButton();
void ChangeTgt();
void MergeCatFS();
void ChangeDuetTgt();
void ChangeClickSound();
#ifdef USE_BIFF
void ChangeBiffSound();
void ChangeMailFile();
void ChangeMailer();
#endif  // USE_BIFF


static void close_dialog();
#ifdef USE_COMMON
static void close_cons();
static void ow_cons();
static void noow_cons();
#endif
static void close_child_dialog();
gboolean close_popup();
static void destroy_popup();
static void close_conf();
static void close_smenu();

void TestPut();
gboolean test_update();
void TestAnime();
void AddPixmap();
void DelPixmap();
void ChangePixmap();
void NkrChangePixmap();
#ifdef USE_BIFF
void ChangeBiffPixmap();
#endif
void ChangeTgt();

static void create_change_image_dialog();
#ifdef USE_BIFF
static void create_change_biff_image_dialog();
#endif
static void create_add_image_dialog();
static void create_del_image_dialog();
static void create_add_pattern_dialog();
static void create_del_pattern_dialog();
static void create_copy_pattern_dialog();
static void create_add_frame_dialog();
static void create_del_frame_dialog();
static void create_change_tgt_dialog();
static void create_add_tgt_dialog();
static void create_del_tgt_ditalog();
static void create_add_cat_dialog();
static void create_del_cat_dialog();
static void create_merge_cat_dialog();

static void create_change_duet_tgt_dialog();
static void create_change_click_sound_dialog();
#ifdef USE_BIFF
static void create_change_biff_sound_dialog();
static void create_mail_file_dialog();
static void create_mailer_dialog();
#endif  // USE_BIFF

static void NewMascotSelectMascot();
static void NewMascotSelectImage();
static void MakeNewMascot();
gchar * create_new_mascot_file_selection_dialog();
gchar * create_new_mascot_image_selection_dialog();
static void create_new_mascot_dialog();

static void create_file_selection_dialog();
static void create_menu_selection_dialog();
static void create_smenu_dialog2();
static void create_nkr_selection_dialog();
static void create_new_menu_selection_dialog();
static void create_save_mascot_dialog();
static void create_save_release_mascot_dialog();
static void create_save_menu_dialog();
static void create_save_rc_dialog();
#ifndef __GTK_FONT_BUTTON_H__
static void create_font_selection_dialog();
#endif
static void create_color_selection_dialog();
static void change_colbal_default();
GtkWidget * create_menu_page();
GtkWidget * create_new_menu_page();

void make_pixmap_list();
void make_pattern_list();
void make_cat_list();
void make_frame_list();
void make_tgt_list();

GtkWidget * make_open_menu();
GtkWidget * make_save_menu();
GtkWidget * make_new_menu();
GtkWidget * make_install_menu();
GtkWidget * make_menu_menu();
GtkWidget * make_open_launcher_menu();
GtkWidget * make_open_develop_menu();
GtkWidget * make_develop_menu();
GtkWidget * make_cat_menu();
GtkWidget * make_tgt_menu();

void ChangeMascot();


#ifdef USE_COMMON
#endif

void make_smenu_list();
GtkWidget* make_start_menubar();

void do_install_user_menu();
#ifdef USE_COMMON
void do_install_common_menu();
#endif
void do_install_user_mascot();
#ifdef USE_COMMON
void do_install_common_mascot();
#endif
void do_select_mascot();

gint select_menu_from_ext();


// Img Tree
void make_img_tree();
static void imgtree_add_columns();
static GtkTreeModel *imgtree_create_items_model ();
void imgtree_update_item();
static void focus_imgtree_item();
static void act_imgtree_item();
void imgtree_cell_data_func();

// Ptn Tree
void make_ptn_tree();
static void ptntree_add_columns();
static GtkTreeModel *ptntree_create_items_model ();
void ptntree_update_item();
static void focus_ptntree_item();
void ptntree_cell_data_func();
static void ptntree_cell_edited ();
void ptntree_select_frame();


///// Widget as Global argument /////

//Move
GtkWidget *check_nb, *combo_nb, *spinner_manual;
GtkWidget *spinner_home_x, *spinner_home_y, *check_vanish;


//Rendering
GtkWidget *frame_sdw;

//Clock
GtkWidget *entry_fontname_clk;

//Balloon
GtkWidget *entry_fontname_bal;

#ifdef USE_BIFF
//Biff
GtkAdjustment *adj_pop_port;
GtkWidget *entry_mail_file;
GtkWidget *entry_mail_mailer;
GtkWidget *entry_mail_pix_file;
GtkWidget *entry_mail_sound;
#endif



// Pixmap
GtkWidget *pixmap_scrwin;
GtkWidget *pixmap_table;
GtkWidget *pixmap_entry[MAX_PIXMAP];

// Animation
GtkWidget *ptn_note;
GtkWidget *pattern_table[MAX_ANIME_PATTERN];
GtkWidget *frame_table[MAX_ANIME_PATTERN];
GtkWidget *ptn_scrwin[MAX_ANIME_PATTERN];
GtkWidget *click_sound_entry[MAX_ANIME_PATTERN];
GtkWidget *duet_tgt_entry[MAX_ANIME_PATTERN];


// Menu Category
GtkWidget *cat_table[MAX_MENU_CATEGORY];
GtkWidget *tgt_table[MAX_MENU_TARGET];
GtkWidget *cat_scrwin[MAX_MENU_CATEGORY];
GtkWidget *cat_note;

GtkWidget *menu_page_table;

//Misc
GtkWidget *entry_deffontname_clk;
GtkWidget *entry_deffontname_bal;

static confNum  *cdata[NUM_CONF_COLOR], *mcat[MAX_MENU_CATEGORY],
  *mptn[MAX_ANIME_PATTERN], *mpix[MAX_PIXMAP];
static confNum2 *mtgt[MAX_MENU_CATEGORY][MAX_MENU_TARGET];

void create_conf_num(typMascot *mascot){
  gint i_col, i_cat, i_tgt, i_ptn, i_pix;

  for(i_col=0; i_col<NUM_CONF_COLOR; i_col++){
    cdata[i_col]=g_malloc0(sizeof(confNum));
    cdata[i_col]->mascot=mascot;
    cdata[i_col]->num=i_col;
  }

  for(i_cat=0; i_cat<MAX_MENU_CATEGORY; i_cat++){
    mcat[i_cat]=g_malloc0(sizeof(confNum));
    mcat[i_cat]->mascot=mascot;
    mcat[i_cat]->num=i_cat;
    for(i_tgt=0; i_tgt<MAX_MENU_CATEGORY; i_tgt++){
      mtgt[i_cat][i_tgt]=g_malloc0(sizeof(confNum2));
      mtgt[i_cat][i_tgt]->mascot=mascot;
      mtgt[i_cat][i_tgt]->num=i_cat;
      mtgt[i_cat][i_tgt]->num2=i_tgt;
    }
  }

  for(i_ptn=0; i_ptn<MAX_ANIME_PATTERN; i_ptn++){
    mptn[i_ptn]=g_malloc0(sizeof(confNum));
    mptn[i_ptn]->mascot=mascot;
    mptn[i_ptn]->num=i_ptn;
  }
  
  for(i_pix=0; i_pix<MAX_PIXMAP; i_pix++){
    mpix[i_pix]=g_malloc0(sizeof(confNum));
    mpix[i_pix]->mascot=mascot;
    mpix[i_pix]->num=i_pix;
  }
}


void MenuSaveRC(void){
  //MoveTmpDataRC();
  SaveRC(Mascot,FALSE);
  {
    gchar *buf;
    buf=g_strdup_printf(_("Saved Resource as ...%%n    \"%s\""),Mascot->rcfile);
    DoSysBalloon(Mascot,buf);
    g_free(buf);
  }
}

void MenuSaveMascot(void){
  //MoveTmpDataMascot();
  SaveMascot(Mascot,FALSE);
  {
    gchar *buf;
    buf=g_strdup_printf(_("Saved Mascot as ...%%n    \"%s\""),Mascot->file);
    DoSysBalloon(Mascot,buf);
    g_free(buf);
  }
}

void MenuSaveDefMenu(void){
  //MoveTmpDataRC();
  SaveDefMenu(Mascot,FALSE);
  {
    gchar *buf;
    if( Mascot->menu_total>0){
      buf=g_strdup_printf(_("Set Default Menu as ...%%n    \"%s\""),Mascot->menu_file);
    }
    else{
      buf=g_strdup(_("Remove Default Menu from Resource Setting"));
    }
    DoSysBalloon(Mascot,buf);
    g_free(buf);
  }
}

void MenuDelDefMenu(void){
  Mascot->menu_total=0;
  Mascot->menu_file=NULL;
  SaveDefMenu(Mascot,FALSE);
  {
    gchar *buf;
    buf=g_strdup(_("Remove Default Menu from Resource Setting"));
    DoSysBalloon(Mascot,buf);
    g_free(buf);
  }

  gtk_widget_destroy(Mascot->PopupMenu);
  ReadMenu(Mascot,0,NULL);
  Mascot->PopupMenu=make_popup_menu(Mascot);
}


void MenuSaveAll(void){
  gchar *buf, *buf1, *buf2, *buf3, *buf4;

  SaveMascot(Mascot,FALSE);
  buf1=g_strdup_printf(_("Saved Mascot as ...%%n    \"%s\""),Mascot->file);

  SaveRC(Mascot,FALSE);
  buf2=g_strdup_printf(_("%%aSaved Resource as ...%%n    \"%s\""),Mascot->rcfile);

  if(Mascot->menu_total>0){
    SaveMenu(Mascot);
    buf3=g_strdup_printf(_("%%aSaved Menu as ...%%n    \"%s\""),Mascot->menu_file);
    buf4=g_strdup_printf(_("%%aSet Default Menu as ...%%n    \"%s\""),Mascot->menu_file);
  }
  else{
    buf3=NULL;
    buf4=g_strdup_printf(_("%%aRemove Default Menu from Resource Setting"));
  }

  SaveDefMenu(Mascot,FALSE);

  if(buf3){
    buf=g_strconcat(buf1,buf2,buf3,buf4,NULL);
    g_free(buf1);
    g_free(buf2);
    g_free(buf3);
    g_free(buf4);
  }
  else{
    buf=g_strconcat(buf1,buf2,buf4,NULL);
    g_free(buf1);
    g_free(buf2);
    g_free(buf4);
  }

  DoSysBalloon(Mascot, buf);
  g_free(buf);
}


void cc_radio(GtkWidget *button, gint *gdata)
{ 
  GSList *group=NULL;

  group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (button));

  {
    GtkWidget *w;
    gint i;
    
    for(i = 0; i < g_slist_length(group); i++){
      w = g_slist_nth_data(group, i);
      if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w))){
	*gdata  = g_slist_length(group) -1 - i;
	break;
      }
    }
  }
}

static void cc_get_combo_box (GtkWidget *widget,  gint * gdata)
{
  GtkTreeIter iter;
  if(gtk_combo_box_get_active_iter(GTK_COMBO_BOX(widget), &iter)){
    gint n;
    GtkTreeModel *model;
    
    model=gtk_combo_box_get_model(GTK_COMBO_BOX(widget));
    gtk_tree_model_get (model, &iter, 1, &n, -1);

    *gdata=n;
  }
}

static void cc_get_autobar_combo (GtkWidget *widget,  gpointer gdata)
{
  typMascot *mascot;
  GtkTreeIter iter;

  mascot=(typMascot *)gdata;
  
  if(gtk_combo_box_get_active_iter(GTK_COMBO_BOX(widget), &iter)){
    gint n;
    GtkTreeModel *model;
    
    model=gtk_combo_box_get_model(GTK_COMBO_BOX(widget));
    gtk_tree_model_get (model, &iter, 1, &n, -1);

    mascot->focus_autobar=n;
  }

#ifndef USE_WIN32
  if(mascot->focus_autobar==AUTOBAR_MANUAL){
    gtk_widget_set_sensitive(check_nb,FALSE);
    gtk_widget_set_sensitive(combo_nb,FALSE);
    gtk_widget_set_sensitive(spinner_manual,TRUE);
  }
  else{
    gtk_widget_set_sensitive(check_nb,TRUE);
    gtk_widget_set_sensitive(combo_nb,TRUE);
    gtk_widget_set_sensitive(spinner_manual,FALSE);
  }
#endif
}
 
void cc_get_spam_combo (GtkWidget *widget, gpointer gdata)
{
  typMascot *mascot;
  GtkTreeIter iter;

  mascot=(typMascot *)gdata;
  
  if(gtk_combo_box_get_active_iter(GTK_COMBO_BOX(widget), &iter)){
    gchar *c;
    GtkTreeModel *model;
    
    model=gtk_combo_box_get_model(GTK_COMBO_BOX(widget));
    gtk_tree_model_get (model, &iter, 0, &c, -1);

    gtk_entry_set_text(GTK_ENTRY(gtk_bin_get_child(GTK_BIN(widget))),c);
    g_free(c);
  }
}


static void cc_get_combo_box_boolean (GtkWidget *widget,  gint * gdata)
{
  GtkTreeIter iter;
  if(gtk_combo_box_get_active_iter(GTK_COMBO_BOX(widget), &iter)){
    gboolean flag;
    GtkTreeModel *model;
    
    model=gtk_combo_box_get_model(GTK_COMBO_BOX(widget));
    gtk_tree_model_get (model, &iter, 1, &flag, -1);

    *gdata=flag;
  }
}

static void cc_get_adj (GtkAdjustment *adj, gint * gdata)
{
  *gdata=(gint)gtk_adjustment_get_value(adj);
}

static void cc_get_double (GtkAdjustment *adj, gdouble * gdata)
{
  *gdata=gtk_adjustment_get_value(adj);
}

static void cc_get_entry (GtkWidget *widget, gchar **gdata)
{
  if(*gdata) g_free(*gdata);
  *gdata=g_strdup(gtk_entry_get_text(GTK_ENTRY(widget)));
}

#ifdef USE_BIFF
#ifdef USE_SSL
static void cc_mail_ssl (GtkWidget *widget, gpointer gdata)
{
  typMascot *mascot;
  GtkTreeIter iter;

  mascot = (typMascot *)gdata;
  
  if(gtk_combo_box_get_active_iter(GTK_COMBO_BOX(widget), &iter)){
    gint n;
    GtkTreeModel *model;
    
    model=gtk_combo_box_get_model(GTK_COMBO_BOX(widget));
    gtk_tree_model_get (model, &iter, 1, &n, -1);

    mascot->mail.ssl_mode=n;
  }

  switch(mascot->mail.ssl_mode){
  case SSL_TUNNEL:
    if(mascot->mail.pop_port==POP3_PORT_NO){
      gtk_adjustment_set_value(adj_pop_port, POP3_SSL_PORT_NO);
    }
    break;
  case SSL_NONE:
  case SSL_STARTTLS:
    if(mascot->mail.pop_port==POP3_SSL_PORT_NO){
      gtk_adjustment_set_value(adj_pop_port, POP3_PORT_NO);
    }
    break;
  }
}
#endif //USE_SSL
#endif  // USE_BIFF

static void cc_get_toggle (GtkWidget * widget, gboolean * gdata)
{
  *gdata=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
}

static void cc_get_toggle_nb (GtkWidget * widget, gboolean * gdata)
{
  *gdata=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));

  if(!*gdata){
    gtk_widget_set_sensitive(combo_nb,FALSE);
  }
  else{
    gtk_widget_set_sensitive(combo_nb,TRUE);
  }
}

static void cc_get_toggle_home (GtkWidget * widget, gboolean * gdata)
{
  *gdata=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));

  if(!*gdata){
    gtk_widget_set_sensitive(spinner_home_x,TRUE);
    gtk_widget_set_sensitive(spinner_home_y,TRUE);
    gtk_widget_set_sensitive(check_vanish,FALSE);
  }
  else{
    gtk_widget_set_sensitive(spinner_home_x,FALSE);
    gtk_widget_set_sensitive(spinner_home_y,FALSE);
    gtk_widget_set_sensitive(check_vanish,TRUE);
  }
}

static void cc_get_toggle_sdw (GtkWidget * widget, gboolean * gdata)
{
  *gdata=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));

#ifndef FG_DRAW
  if(!*gdata){
    gtk_widget_set_sensitive(frame_sdw,FALSE);
  }
  else{
    gtk_widget_set_sensitive(frame_sdw,TRUE);
  }
#endif
}

static void check_menu_get_toggle (GtkWidget * widget, gboolean * gdata)
{
  if(gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))){
    *gdata=TRUE;
  }
  else{
    *gdata=FALSE;
  }
}

static void check_menu_get_smenu (GtkWidget * widget, gint gdata)
{
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))){
    selected_smenu=(gint)gdata;
  }
}


#ifdef USE_BIFF
static void mail_flag_toggle (GtkWidget * widget)
{
  if(gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))){
    Mascot->mail.flag=TRUE;
  }
  else{
    Mascot->mail.flag=FALSE;
  }
  SetMailChecker(Mascot);
}
#endif  // USE_BIFF

static void test_click_sound (GtkWidget *widget, gpointer gdata)
{
  confNum *mptn;
  typMascot *mascot;
  int i_ptn;

  mptn=(confNum *)gdata;
  
  i_ptn=mptn->num;
  mascot=mptn->mascot;

  sound_play(mascot,mascot->click_sound[i_ptn]);
}

static void clear_click_sound (GtkWidget *widget, gpointer gdata)
{
  confNum *mptn;
  typMascot *mascot;
  int i_ptn;

  mptn=(confNum *)gdata;
  
  i_ptn=mptn->num;
  mascot=mptn->mascot;

  if(mascot->click_sound[i_ptn]) g_free(mascot->click_sound[i_ptn]);
  mascot->click_sound[i_ptn]=NULL;
  gtk_entry_set_text(GTK_ENTRY(click_sound_entry[i_ptn]),"");
}

#ifdef USE_BIFF
static void test_biff_sound (GtkWidget *widget, gpointer gdata)
{
  typMascot *mascot;
  
  mascot=(typMascot *)gdata;
  
  sound_play(mascot,mascot->mail.sound);
}

static void clear_biff_sound (GtkWidget *widget,  gpointer gdata)
{
  typMascot *mascot;
  
  mascot=(typMascot *)gdata;

  if(mascot->mail.sound) g_free(mascot->mail.sound);
  mascot->mail.sound=NULL;
  gtk_entry_set_text(GTK_ENTRY(entry_mail_sound),"");
  
}
#endif


static void mc_up_tgt (GtkWidget *widget, gpointer gdata)
{
  typMascot *mascot;
  confNum2 *mtgt;
  int i_cat, i_tgt;
  gchar *tmp_menu_tgt=NULL, *tmp_menu_tgt_name=NULL;

  mtgt=(confNum2 *)gdata;
  
  i_cat=mtgt->num;
  i_tgt=mtgt->num2;
  mascot=mtgt->mascot;

  if(i_tgt<=0) return;
  
  tmp_menu_tgt=g_strdup(mascot->menu_tgt[i_cat][i_tgt-1]);
  tmp_menu_tgt_name=g_strdup(mascot->menu_tgt_name[i_cat][i_tgt-1]);

  if(mascot->menu_tgt[i_cat][i_tgt-1]) g_free(mascot->menu_tgt[i_cat][i_tgt-1]);
  mascot->menu_tgt[i_cat][i_tgt-1]=g_strdup(mascot->menu_tgt[i_cat][i_tgt]);
  if(mascot->menu_tgt_name[i_cat][i_tgt-1]) g_free(mascot->menu_tgt_name[i_cat][i_tgt-1]);
  mascot->menu_tgt_name[i_cat][i_tgt-1]=g_strdup(mascot->menu_tgt_name[i_cat][i_tgt]);

  if(mascot->menu_tgt[i_cat][i_tgt]) g_free(mascot->menu_tgt[i_cat][i_tgt]);
  mascot->menu_tgt[i_cat][i_tgt]=g_strdup(tmp_menu_tgt);
  if(mascot->menu_tgt_name[i_cat][i_tgt]) g_free(mascot->menu_tgt_name[i_cat][i_tgt]);
  mascot->menu_tgt_name[i_cat][i_tgt]=g_strdup(tmp_menu_tgt_name);

  if(tmp_menu_tgt) g_free(tmp_menu_tgt);
  if(tmp_menu_tgt_name) g_free(tmp_menu_tgt_name);

  make_tgt_list(mascot, i_cat);
}

static void mc_down_tgt (GtkWidget *widget, gpointer gdata)
{
  typMascot *mascot;
  confNum2 *mtgt;
  int i_cat, i_tgt;
  gchar *tmp_menu_tgt=NULL, *tmp_menu_tgt_name=NULL;

  mtgt=(confNum2 *)gdata;
  
  i_cat=mtgt->num;
  i_tgt=mtgt->num2;
  mascot=mtgt->mascot;
  
  if(i_tgt>=MAX_MENU_TARGET) return;
  
  tmp_menu_tgt=g_strdup(mascot->menu_tgt[i_cat][i_tgt+1]);
  tmp_menu_tgt_name=g_strdup(mascot->menu_tgt_name[i_cat][i_tgt+1]);
  
  if(mascot->menu_tgt[i_cat][i_tgt+1]) g_free(mascot->menu_tgt[i_cat][i_tgt+1]);
  mascot->menu_tgt[i_cat][i_tgt+1]=g_strdup(mascot->menu_tgt[i_cat][i_tgt]);
  if(mascot->menu_tgt_name[i_cat][i_tgt+1]) g_free(mascot->menu_tgt_name[i_cat][i_tgt+1]);
  mascot->menu_tgt_name[i_cat][i_tgt+1]=g_strdup(mascot->menu_tgt_name[i_cat][i_tgt]);
  
  if(mascot->menu_tgt[i_cat][i_tgt]) g_free(mascot->menu_tgt[i_cat][i_tgt]);
  mascot->menu_tgt[i_cat][i_tgt]=g_strdup(tmp_menu_tgt);
  if(mascot->menu_tgt_name[i_cat][i_tgt]) g_free(mascot->menu_tgt_name[i_cat][i_tgt]);
  mascot->menu_tgt_name[i_cat][i_tgt]=g_strdup(tmp_menu_tgt_name);
  
  if(tmp_menu_tgt) g_free(tmp_menu_tgt);
  if(tmp_menu_tgt_name) g_free(tmp_menu_tgt_name);
  
  make_tgt_list(mascot,i_cat);
}


static void mc_up_cat (GtkWidget *widget, gpointer gdata)
{
  confNum *mcat;
  typMascot *mascot;
  int i_cat, i_tgt;
  gint  tmp_menu_tgt_max;
  gchar *tmp_menu_cat=NULL;
  gchar *tmp_menu_tgt[MAX_MENU_TARGET];
  gchar *tmp_menu_tgt_name[MAX_MENU_TARGET];

  mcat=(confNum *)gdata;
  
  i_cat=mcat->num;
  mascot=mcat->mascot;
  
  if(i_cat<=0) return;

  tmp_menu_tgt_max=mascot->menu_tgt_max[i_cat-1];
  tmp_menu_cat=g_strdup(mascot->menu_cat[i_cat-1]);
  
  for(i_tgt=0;i_tgt<mascot->menu_tgt_max[i_cat-1];i_tgt++){
    tmp_menu_tgt[i_tgt]
      =g_strdup(mascot->menu_tgt[i_cat-1][i_tgt]);
    tmp_menu_tgt_name[i_tgt]
      =g_strdup(mascot->menu_tgt_name[i_cat-1][i_tgt]);
  }
 
  mascot->menu_tgt_max[i_cat-1]=mascot->menu_tgt_max[i_cat];
  if(mascot->menu_cat[i_cat-1]) g_free(mascot->menu_cat[i_cat-1]);
  mascot->menu_cat[i_cat-1]=g_strdup(mascot->menu_cat[i_cat]);
  
  for(i_tgt=0;i_tgt<mascot->menu_tgt_max[i_cat-1];i_tgt++){
    if(mascot->menu_tgt[i_cat-1][i_tgt]) g_free(mascot->menu_tgt[i_cat-1][i_tgt]);
    mascot->menu_tgt[i_cat-1][i_tgt]=g_strdup(mascot->menu_tgt[i_cat][i_tgt]);
    if(mascot->menu_tgt_name[i_cat-1][i_tgt]) g_free(mascot->menu_tgt_name[i_cat-1][i_tgt]);
    mascot->menu_tgt_name[i_cat-1][i_tgt]=g_strdup(mascot->menu_tgt_name[i_cat][i_tgt]);
  }

  mascot->menu_tgt_max[i_cat]=tmp_menu_tgt_max;
  if(mascot->menu_cat[i_cat]) g_free(mascot->menu_cat[i_cat]);
  mascot->menu_cat[i_cat]=g_strdup(tmp_menu_cat);
  
  for(i_tgt=0;i_tgt<mascot->menu_tgt_max[i_cat];i_tgt++){
    if(mascot->menu_tgt[i_cat][i_tgt]) g_free(mascot->menu_tgt[i_cat][i_tgt]);
    mascot->menu_tgt[i_cat][i_tgt]=g_strdup(tmp_menu_tgt[i_tgt]);
    if(mascot->menu_tgt_name[i_cat][i_tgt]) g_free(mascot->menu_tgt_name[i_cat][i_tgt]);
    mascot->menu_tgt_name[i_cat][i_tgt]=g_strdup(tmp_menu_tgt_name[i_tgt]);
  }

  make_cat_list(mascot, cat_note, i_cat-1);
  make_cat_list(mascot, cat_note, i_cat);

  for(i_tgt=0;i_tgt<mascot->menu_tgt_max[i_cat-1];i_tgt++){
    if(tmp_menu_tgt[i_tgt]) g_free(tmp_menu_tgt[i_tgt]);
    if(tmp_menu_tgt_name[i_tgt]) g_free(tmp_menu_tgt_name[i_tgt]);
  }
  if(tmp_menu_cat) g_free(tmp_menu_cat);

  gtk_notebook_set_current_page(GTK_NOTEBOOK(cat_note), (gint)(i_cat-1));
  gtk_widget_queue_draw(GTK_WIDGET(cat_note));

}

static void mc_down_cat (GtkWidget *widget, gpointer gdata)
{
  confNum *mcat;
  typMascot *mascot;
  int i_cat, i_tgt;
  gint  tmp_menu_tgt_max;
  gchar *tmp_menu_cat=NULL;
  gchar *tmp_menu_tgt[MAX_MENU_TARGET];
  gchar *tmp_menu_tgt_name[MAX_MENU_TARGET];

  mcat=(confNum *)gdata;
  
  i_cat=mcat->num;
  mascot=mcat->mascot;
  
  if(i_cat>=mascot->menu_cat_max-1) return;

  tmp_menu_tgt_max=mascot->menu_tgt_max[i_cat+1];
  tmp_menu_cat=g_strdup(mascot->menu_cat[i_cat+1]);
  
  for(i_tgt=0;i_tgt<mascot->menu_tgt_max[i_cat+1];i_tgt++){
    tmp_menu_tgt[i_tgt]=g_strdup(mascot->menu_tgt[i_cat+1][i_tgt]);
    tmp_menu_tgt_name[i_tgt]=g_strdup(mascot->menu_tgt_name[i_cat+1][i_tgt]);
  }
 
  mascot->menu_tgt_max[i_cat+1]=mascot->menu_tgt_max[i_cat];
  if(mascot->menu_cat[i_cat+1]) g_free(mascot->menu_cat[i_cat+1]);
  mascot->menu_cat[i_cat+1]=g_strdup(mascot->menu_cat[i_cat]);
  
  for(i_tgt=0;i_tgt<mascot->menu_tgt_max[i_cat+1];i_tgt++){
    if(mascot->menu_tgt[i_cat+1][i_tgt]) g_free(mascot->menu_tgt[i_cat+1][i_tgt]);
    mascot->menu_tgt[i_cat+1][i_tgt]=g_strdup(mascot->menu_tgt[i_cat][i_tgt]);
    if(mascot->menu_tgt_name[i_cat+1][i_tgt]) g_free(mascot->menu_tgt_name[i_cat+1][i_tgt]);
    mascot->menu_tgt_name[i_cat+1][i_tgt]=g_strdup(mascot->menu_tgt_name[i_cat][i_tgt]);
  }

  mascot->menu_tgt_max[i_cat]=tmp_menu_tgt_max;
  if(mascot->menu_cat[i_cat]) g_free(mascot->menu_cat[i_cat]);
  mascot->menu_cat[i_cat]=g_strdup(tmp_menu_cat);
  
  for(i_tgt=0;i_tgt<mascot->menu_tgt_max[i_cat];i_tgt++){
    if(mascot->menu_tgt[i_cat][i_tgt]) g_free(mascot->menu_tgt[i_cat][i_tgt]);
    mascot->menu_tgt[i_cat][i_tgt]=g_strdup(tmp_menu_tgt[i_tgt]);
    if(mascot->menu_tgt_name[i_cat][i_tgt]) g_free(mascot->menu_tgt_name[i_cat][i_tgt]);
    mascot->menu_tgt_name[i_cat][i_tgt]=g_strdup(tmp_menu_tgt_name[i_tgt]);
  }

  make_cat_list(mascot, cat_note, i_cat);
  make_cat_list(mascot, cat_note, i_cat+1);

  for(i_tgt=0;i_tgt<mascot->menu_tgt_max[i_cat+1];i_tgt++){
    if(tmp_menu_tgt[i_tgt]) g_free(tmp_menu_tgt[i_tgt]);
    if(tmp_menu_tgt_name[i_tgt]) g_free(tmp_menu_tgt_name[i_tgt]);
  }
  if(tmp_menu_cat) g_free(tmp_menu_cat);
  
  gtk_notebook_set_current_page(GTK_NOTEBOOK(cat_note), (gint)(i_cat+1));
  gtk_widget_queue_draw(GTK_WIDGET(cat_note));

}

#ifdef USE_WIN32
static void uri_clicked(GtkButton *button, gpointer data)
{
  ShellExecute(NULL, 
	       "open", 
	       DEFAULT_URL,
	       NULL, 
	       NULL, 
	       SW_SHOWNORMAL);
}
#endif

void my_file_chooser_add_filter (GtkWidget *dialog, 
				 const gchar *name,
				 const gchar *pattern)
{
  GtkFileFilter *filter;
  gchar *name_tmp;

  filter=gtk_file_filter_new();
  name_tmp=g_strconcat(name,"[",pattern,"]",NULL);
  gtk_file_filter_set_name(filter, name_tmp);
  gtk_file_filter_add_pattern(filter, pattern);
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog),filter);
  g_free(name_tmp);
}

void my_file_chooser_add_shortcut_folder(GtkWidget *dialog, gint mode)
{
  gchar *dir;
  
  switch(mode){
  case FOLDER_PIX:
#ifdef USE_WIN32
    dir = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,
		      USER_DIR,PIXDIR,NULL);
#else
    dir = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,
		      USER_DIR,PIXDIR,NULL);
#endif
    break;
  case FOLDER_SOUND:
#ifdef USE_WIN32
    dir = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,
		      USER_DIR,SOUNDDIR,NULL);
#else
    dir = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,
		      USER_DIR,SOUNDDIR,NULL);
#endif
    break;
  case FOLDER_CURRENT:
    dir=g_malloc0(sizeof(gchar)*1024);
    if(getcwd(dir,1024)==NULL){
      fprintf(stderr,"GetCWD Error!\n");
      exit(1);
    }
    break;
  default:
#ifdef USE_WIN32
    dir = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,
		      USER_DIR,NULL);
#else
    dir = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,
		      USER_DIR,NULL);
#endif
  }

  if(dir)
    gtk_file_chooser_add_shortcut_folder(GTK_FILE_CHOOSER(dialog),
					 to_utf8(dir),
					 NULL);
  g_free(dir);

#ifdef USE_COMMON
  switch(mode){
  case FOLDER_PIX:
    dir = g_strconcat(COMMON_DIR,PIXDIR, NULL);
    break;
  case FOLDER_SOUND:
    dir = g_strconcat(COMMON_DIR,SOUNDDIR, NULL);
    break;
  case FOLDER_CURRENT:
    dir=NULL;
    break;
  default:
    dir = g_strconcat(COMMON_DIR, NULL);
  }

  if(dir)
    gtk_file_chooser_add_shortcut_folder(GTK_FILE_CHOOSER(dialog),
					 to_utf8(dir),
					 NULL);
  g_free(dir);
#endif
}

void my_file_chooser_set_current_folder(GtkWidget *dialog, gint mode)
{
  gchar *dir;
  
  switch(mode){
  case FOLDER_PIX:
#ifdef USE_WIN32
  dir = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,
		    USER_DIR, PIXDIR, NULL);
#else
  dir = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,
		    USER_DIR, PIXDIR, NULL);
#endif
    break;
  case FOLDER_SOUND:
#ifdef USE_WIN32
  dir = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,
		    USER_DIR, SOUNDDIR, NULL);
#else
  dir = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,
		    USER_DIR, SOUNDDIR, NULL);
#endif
    break;
  default:
#ifdef USE_WIN32
  dir = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,
		    USER_DIR, NULL);
#else
  dir = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,
		    USER_DIR, NULL);
#endif
  }

  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), 
				       to_utf8(dir));
  g_free(dir);
}



// クイックメニューからのマスコットの変更
void ChangeMascotMenu(GtkWidget *widget, gchar *mascotfile)
{ 
  Mascot->file=g_strdup(mascotfile);
  
  ChangeMascot();
}

// クイックメニューからのマスコットのカテゴリー内ランダム変更
void CatRandomChangeMascotMenu(GtkWidget *widget, gint gdata)
{
  int i_cat,i_tgt;

  i_cat=(gint)gdata;
  
  i_tgt=RANDOM(Mascot->menu_tgt_max[i_cat]);
 
  Mascot->file=g_strdup(Mascot->menu_tgt[i_cat][i_tgt]);
      
  ChangeMascot();
}

// クイックメニューからのマスコットの全ランダム変更
void AllRandomChangeMascotMenu(GtkWidget *widget)
{

  Mascot->file=g_strdup(all_random_menu_mascot_file(Mascot));
  
  ChangeMascot();

}


// リリース用マスコット保存 : Copyrightの入力終了
void GoRelease(GtkWidget *w, gpointer gdata)
{ 
  confSaveReleaseMascot *cdata;

  cdata=(confSaveReleaseMascot *)gdata;

  cdata->update=TRUE;
  cdata->new_copyright=
    g_strdup(gtk_entry_get_text(GTK_ENTRY(cdata->copyright_entry)));
  cdata->new_name=
    g_strdup(gtk_entry_get_text(GTK_ENTRY(cdata->name_entry)));
  cdata->new_code=
    g_strdup(gtk_entry_get_text(GTK_ENTRY(cdata->code_entry)));
  cdata->clk_font_flag=
    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cdata->clk_font_check));
  cdata->bal_font_flag=
    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cdata->bal_font_check));
  cdata->clk_color_flag=
    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cdata->clk_color_check));
  cdata->bal_color_flag=
    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cdata->bal_color_check));

  gtk_main_quit();
  gtk_widget_destroy(GTK_WIDGET(cdata->dialog));
}



// 色選択ダイアログからの色の変更
void ChangeFontButton(GtkWidget *w, gchar **gdata)
{ 
#ifdef USE_GTK3  
  if(*gdata) g_free(*gdata);
  *gdata = g_strdup(gtk_font_chooser_get_font(GTK_FONT_CHOOSER(w)));
#else
  if(*gdata) g_free(*gdata);
  *gdata = g_strdup(gtk_font_button_get_font_name(GTK_FONT_BUTTON(w)));
#endif
}


// 色選択ダイアログからの色の変更
void ChangeColorButton(GtkWidget *w, gpointer gdata)
{
  confNum *cdata;
  typMascot *mascot;
  int get_alpha;
  int i_col;

  cdata=(confNum *)gdata;

  i_col=cdata->num;
  mascot=cdata->mascot;

#ifdef USE_GTK3  //////////////  GTK3   //////////////
  switch(i_col){
  case CONF_COLOR_CLK:
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(w),mascot->colclk); 
    mascot->alpclk=(gint)(mascot->colclk->alpha*(gdouble)0xFFFF);
    break;
  case CONF_COLOR_CLKBG:
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(w),mascot->colclkbg);
#ifndef USE_WIN32
    mascot->alpclkbg=(gint)(mascot->colclkbg->alpha*100);
#endif    
    break;
  case CONF_COLOR_CLKBD:
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(w),mascot->colclkbd);
    mascot->alpclkbd=(gint)(mascot->colclkbd->alpha*100);
    break;
  case CONF_COLOR_CLKSD:
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(w),mascot->colclksd);
    mascot->alpclksd=(gint)(mascot->colclksd->alpha*100);
    break;
  case CONF_COLOR_BAL:
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(w),mascot->colbal);
    mascot->alpbal=(gint)(mascot->colbal->alpha*100);
    break;
  case CONF_COLOR_BALBG:
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(w),mascot->colbalbg);
#ifndef FG_DRAW
    mascot->alpbalbg=(gint)(mascot->colbalbg->alpha*100);
#endif
    break;
  case CONF_COLOR_BALBD:
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(w),mascot->colbalbd);
    mascot->alpbalbd=(gint)(mascot->colbalbd->alpha*100);
    break;
  case CONF_DEF_COLOR_CLK:
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(w),mascot->def_colclk);
    mascot->def_alpclk=(gint)(mascot->def_colclk->alpha*100);
    break;
  case CONF_DEF_COLOR_CLKBG:
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(w),mascot->def_colclkbg);
#ifndef USE_WIN32
    mascot->def_alpclkbg=(gint)(mascot->def_colclkbg->alpha*100);
#endif
    break;
  case CONF_DEF_COLOR_CLKBD:
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(w),mascot->def_colclkbd);
    mascot->def_alpclkbd=(gint)(mascot->def_colclkbd->alpha*100);
    break;
  case CONF_DEF_COLOR_CLKSD:
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(w),mascot->def_colclksd);
    mascot->def_alpclksd=(gint)(mascot->def_colclksd->alpha*100);
    break;
  case CONF_DEF_COLOR_BAL:
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(w),mascot->def_colbal);
    mascot->def_alpbal=(gint)(mascot->def_colbal->alpha*100);
    break;
  case CONF_DEF_COLOR_BALBG:
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(w),mascot->def_colbalbg);
#ifndef FG_DRAW
    mascot->def_alpbalbg=(gint)(mascot->def_colbalbg->alpha*100);
#endif
    break;
  case CONF_DEF_COLOR_BALBD:
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(w),mascot->def_colbalbd);
    mascot->def_alpbalbd=(gint)(mascot->def_colbalbd->alpha*100);
    break;
  }

#else  //////////////  GTK2  //////////////
  get_alpha=(gint)((gdouble)gtk_color_button_get_alpha(GTK_COLOR_BUTTON(w))/(gdouble)0xFFFF*100);
  switch(i_col){
  case CONF_COLOR_CLK:
    gtk_color_button_get_color(GTK_COLOR_BUTTON(w),mascot->colclk);
    mascot->alpclk=get_alpha;
    break;
  case CONF_COLOR_CLKBG:
    gtk_color_button_get_color(GTK_COLOR_BUTTON(w),mascot->colclkbg);
#ifndef USE_WIN32
    mascot->alpclkbg=get_alpha;
#endif
    break;
  case CONF_COLOR_CLKBD:
    gtk_color_button_get_color(GTK_COLOR_BUTTON(w),mascot->colclkbd);
    mascot->alpclkbd=get_alpha;
    break;
  case CONF_COLOR_CLKSD:
    gtk_color_button_get_color(GTK_COLOR_BUTTON(w),mascot->colclksd);
    mascot->alpclksd=get_alpha;
    break;
  case CONF_COLOR_BAL:
    gtk_color_button_get_color(GTK_COLOR_BUTTON(w),mascot->colbal);
    mascot->alpbal=get_alpha;
    break;
  case CONF_COLOR_BALBG:
    gtk_color_button_get_color(GTK_COLOR_BUTTON(w),mascot->colbalbg);
#ifndef FG_DRAW
    mascot->alpbalbg=get_alpha;
#endif
    break;
  case CONF_COLOR_BALBD:
    gtk_color_button_get_color(GTK_COLOR_BUTTON(w),mascot->colbalbd);
    mascot->alpbalbd=get_alpha;
    break;
  case CONF_DEF_COLOR_CLK:
    gtk_color_button_get_color(GTK_COLOR_BUTTON(w),mascot->def_colclk);
    mascot->def_alpclk=get_alpha;
    break;
  case CONF_DEF_COLOR_CLKBG:
    gtk_color_button_get_color(GTK_COLOR_BUTTON(w),mascot->def_colclkbg);
#ifndef USE_WIN32
    mascot->def_alpclkbg=get_alpha;
#endif
    break;
  case CONF_DEF_COLOR_CLKBD:
    gtk_color_button_get_color(GTK_COLOR_BUTTON(w),mascot->def_colclkbd);
    mascot->def_alpclkbd=get_alpha;
    break;
  case CONF_DEF_COLOR_CLKSD:
    gtk_color_button_get_color(GTK_COLOR_BUTTON(w),mascot->def_colclksd);
    mascot->def_alpclksd=get_alpha;
    break;
  case CONF_DEF_COLOR_BAL:
    gtk_color_button_get_color(GTK_COLOR_BUTTON(w),mascot->def_colbal);
    mascot->def_alpbal=get_alpha;
    break;
  case CONF_DEF_COLOR_BALBG:
    gtk_color_button_get_color(GTK_COLOR_BUTTON(w),mascot->def_colbalbg);
#ifndef FG_DRAW
    mascot->def_alpbalbg=get_alpha;
#endif
    break;
  case CONF_DEF_COLOR_BALBD:
    gtk_color_button_get_color(GTK_COLOR_BUTTON(w),mascot->def_colbalbd);
    mascot->def_alpbalbd=get_alpha;
    break;
  }

#endif  // USE_GTK3
}


static void close_dialog(GtkWidget *w, GtkWidget *dialog)
{
  //gdk_pointer_ungrab(GDK_CURRENT_TIME);

  gtk_main_quit();
  gtk_widget_destroy(dialog);

  Mascot->flag_menu=FALSE;
}

#ifdef USE_COMMON
static void close_cons(GtkWidget *w, GtkWidget *dialog)
{
  //gdk_pointer_ungrab(GDK_CURRENT_TIME);

  gtk_widget_destroy(dialog);
  gtk_main_quit();

  Mascot->flag_menu=FALSE;
}

static void ow_cons(GtkWidget *w, gpointer gdata)
{
  consType *cdata;

  cdata=(consType *)gdata;

  cdata->flag_auto=
    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cdata->auto_check));
  cdata->flag_ow=TRUE;
  
  gtk_widget_destroy(cdata->dialog);
  gtk_main_quit();

  Mascot->flag_menu=FALSE;
}


static void noow_cons(GtkWidget *w, gpointer gdata)
{
  consType *cdata;

  cdata=(consType *)gdata;

  cdata->flag_auto=
    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cdata->auto_check));
  cdata->flag_ow=FALSE;
  
  gtk_widget_destroy(cdata->dialog);
  gtk_main_quit();

  Mascot->flag_menu=FALSE;
}
#endif

static void close_child_dialog(GtkWidget *w, GtkWidget *dialog)
{
  //gdk_pointer_ungrab(GDK_CURRENT_TIME);

  gtk_main_quit();
  gtk_widget_destroy(dialog);
}

gboolean close_popup(gpointer data)
{
  GtkWidget *dialog;

  dialog=(GtkWidget *)data;

  gtk_main_quit();
  gtk_widget_destroy(GTK_WIDGET(dialog));

  return(FALSE);
}

static void destroy_popup(GtkWidget *w, guint data)
{
  guint timer;
  timer=(guint)data;

  g_source_remove((guint)timer);
  gtk_main_quit();
}

/*
static void close_conf(GtkWidget *w, GtkWidget *dialog)
{
  //gdk_pointer_ungrab(GDK_CURRENT_TIME);

  if(flagChildDialog){
    return;
  }

  LoadPixmaps(Mascot);
  DrawMascot0(Mascot);

  // テスト表示でShapeが変わった場合のための対応
  if(Mascot->clkmode==CLOCK_PIXMAP){ 
    DrawMascotWithDigit(Mascot);
  }
  else{
    DrawMascot(Mascot, Mascot->frame_pix[Mascot->anime_ptn][Mascot->anime_frm]);
  }


  gtk_widget_destroy(GTK_WIDGET(dialog));

  flag_make_pixmap_list=FALSE;
  Mascot->flag_menu=FALSE;

  //gdkut_flush(Mascot);
}
*/

void quit_all(void)
{
#ifdef USE_GTK_STATUS_ICON
	trayicon_hide(Mascot);
	trayicon_destroy(Mascot);
#endif
#ifdef USE_BIFF
#ifndef USE_WIN32
	kill_pop3();
#endif
#endif
	gtk_main_quit();
	exit(0);
}

static void close_smenu(GtkWidget *w, GtkWidget *dialog)
{

  while (my_main_iteration(FALSE));
  gtk_widget_destroy(GTK_WIDGET(dialog));
  while (my_main_iteration(FALSE));
  if(selected_smenu==-1) gtk_main_quit();
  
  selected_smenu=-1;
}


// マスコットテスト表示用コールバック
void TestPut(GtkWidget *w, gpointer gdata)
{
  confNum *mpix;
  typMascot *mascot;
  int i_pix;

  mpix=(confNum *)gdata;

  i_pix=mpix->num;
  mascot=mpix->mascot;

  TestLoadPixmaps(mascot,
		  mascot->sprites[i_pix].filename,
		  i_pix);
}


gboolean test_update(gpointer gdata){
  typMascot *mascot = (typMascot *)gdata;

    
  if(mascot->anime_seqend > mascot->anime_seq){
    mascot->anime_seq++;
    return(TRUE);
  }
  else{
    // ブロックループ
    if((mascot->frame_loop[mascot->anime_ptn][mascot->anime_frm].next>=0)){
      mascot->frame_loop[mascot->anime_ptn][mascot->anime_frm].seq++;
      if(mascot->frame_loop[mascot->anime_ptn][mascot->anime_frm].seq==1){
	// ブロックループ開始
	mascot->frame_loop[mascot->anime_ptn][mascot->anime_frm].seqend=
	  RANDOM(mascot->frame_loop[mascot->anime_ptn][mascot->anime_frm].max
		 -mascot->frame_loop[mascot->anime_ptn][mascot->anime_frm].min+1)
	  +mascot->frame_loop[mascot->anime_ptn][mascot->anime_frm].min;
	
	if(mascot->frame_loop[mascot->anime_ptn][mascot->anime_frm].seqend==1){
	  mascot->frame_loop[mascot->anime_ptn][mascot->anime_frm].seq=0;
	  mascot->anime_frm++;
	}
	else{
	  mascot->anime_frm=
	    mascot->frame_loop[mascot->anime_ptn][mascot->anime_frm].next;
	}
      }
      else if(mascot->frame_loop[mascot->anime_ptn][mascot->anime_frm].seqend-
	      mascot->frame_loop[mascot->anime_ptn][mascot->anime_frm].seq<=0){
	// ブロックループ終了
	mascot->frame_loop[mascot->anime_ptn][mascot->anime_frm].seq=0;
	mascot->anime_frm++;
      }
      else{
	// ブロックループ継続
	mascot->anime_frm=
	  mascot->frame_loop[mascot->anime_ptn][mascot->anime_frm].next;
      }
    }
    else{
      mascot->anime_frm++;
    }

    // パターン終了
    if(mascot->anime_frm >= mascot->frame_num[mascot->anime_ptn]){
      mascot->anime_ptn=0;
      mascot->anime_frm=0;
      mascot->anime_seq=0;
      mascot->anime_seqend=RANDOM(mascot->frame_max[0][0]
				  -mascot->frame_min[0][0]+1)
	+mascot->frame_min[0][0];
      
      TestLoadPixmaps(mascot,
		      mascot->sprites[mascot->frame_pix[0][0]].filename,
		      mascot->frame_pix[0][0]);
      mascot->test_timer=-1;
      return(FALSE);
    }

    // 新規画像ロード
    ptntree_select_frame(mascot->ptntree[mascot->anime_ptn],
			 mascot->anime_frm,
			 mascot->frame_num[mascot->anime_ptn]);

    mascot->anime_seq=1;
    mascot->anime_seqend=RANDOM(mascot->frame_max[mascot->anime_ptn][mascot->anime_frm]
				-mascot->frame_min[mascot->anime_ptn][mascot->anime_frm]+1)
      +mascot->frame_min[mascot->anime_ptn][mascot->anime_frm];
    /*
    printf("img=%d %s   seqend=%d\n",
	   mascot->frame_pix[mascot->anime_ptn][mascot->anime_frm],
	   mascot->sprites[mascot->frame_pix[mascot->anime_ptn][mascot->anime_frm]].filename,
	   mascot->anime_seqend);
    */
    return(TRUE);
  }
}


// マスコットテスト表示用コールバック Animationページ用
void TestAnime(GtkWidget *w, gpointer gdata)
{
  confNum *mptn;
  typMascot *mascot;
  int i_ptn, i_frm;
  int seqend;

  mptn=(confNum *)gdata;

  i_ptn=mptn->num;
  mascot=mptn->mascot;

  if(mascot->test_timer>0) return;

  if(flagChildDialog){
    return;
  }
  else{
    flagChildDialog=TRUE;
  }

  gtk_widget_set_sensitive(w, FALSE);

  i_frm=0;

  // アニメ

  mascot->anime_ptn=i_ptn;
  mascot->anime_frm=0;
  mascot->anime_seq=1;
  mascot->anime_seqend=
    RANDOM(mascot->frame_max[mascot->anime_ptn][mascot->anime_frm]
	   -mascot->frame_min[mascot->anime_ptn][mascot->anime_frm]+1)
    +mascot->frame_min[mascot->anime_ptn][mascot->anime_frm];
  
  ptntree_select_frame(mascot->ptntree[mascot->anime_ptn],
		       mascot->anime_frm,
		       mascot->frame_num[mascot->anime_ptn]);
  
  mascot->test_timer=g_timeout_add_full(G_PRIORITY_HIGH,
					INTERVAL, 
					(GSourceFunc)test_update, 
					(gpointer)mascot,
					NULL);
  

  gtk_widget_set_sensitive(w, TRUE);
  flagChildDialog=FALSE;

}





static void create_change_image_dialog(typMascot *mascot, gint i_pix)
{
  GtkWidget *fdialog;
  char win_title[64];
  gchar *fname=NULL;
  gchar *dest_file=NULL;
  
  if(flagChildDialog){
    return;
  }
  else{
    flagChildDialog=TRUE;
  }

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  while (my_main_iteration(FALSE));

  sprintf(win_title,_("Select New Image for No.%02d"),
	  i_pix);
  fdialog = gtk_file_chooser_dialog_new(_(win_title),
					GTK_WINDOW(mascot->conf_main),
					GTK_FILE_CHOOSER_ACTION_OPEN,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Open", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				
 
  gtk_dialog_set_default_response(GTK_DIALOG(fdialog), GTK_RESPONSE_ACCEPT); 
  gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (fdialog), 
  				   to_utf8(mascot->sprites[i_pix].filename));
  gtk_file_chooser_select_filename (GTK_FILE_CHOOSER (fdialog), 
  				   to_utf8(mascot->sprites[i_pix].filename));

  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_PIX);
  my_file_chooser_add_filter(fdialog,_("All File"),"*");
  my_file_chooser_add_filter(fdialog,"PNG",PNG_EXTENSION);
  my_file_chooser_add_filter(fdialog,"GIF",GIF_EXTENSION);
  my_file_chooser_add_filter(fdialog,"XPM",XPM_EXTENSION);

  gtk_widget_show_all(fdialog);
  
  if (gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {
    
    fname = g_strdup(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fdialog)));
    gtk_widget_destroy(fdialog);
    
    dest_file=to_locale(fname);
    if(fname) g_free(fname);

    if(access(dest_file,F_OK)==0){
      if(TestLoadPixmaps(mascot,dest_file,i_pix)){
	if(mascot->sprites[i_pix].filename) g_free(mascot->sprites[i_pix].filename);
	mascot->sprites[i_pix].filename=g_strdup(dest_file);
      }
      else{
	popup_message(mascot->win_main,
#ifdef USE_GTK3
		      "dialog-error", 
#else
		      GTK_STOCK_DIALOG_ERROR,
#endif
		      -1,
		      _("Error: File cannot be loaded."),
		      " ",
		      dest_file,
		      NULL);
      }
      
    }
    else{
      popup_message(Mascot->win_main,
#ifdef USE_GTK3
		    "dialog-error", 
#else
		    GTK_STOCK_DIALOG_ERROR,
#endif
		    -1,
		    _("Error: File cannot be opened."),
		    " ",
		    dest_file,
		    NULL);
    }
  
    //gtk_entry_set_text(GTK_ENTRY(pixmap_entry[i_pix]),my_basename(dest_file));
  }
  else{
    gtk_widget_destroy(fdialog);
  }

  if(dest_file) g_free(dest_file);
  flagChildDialog=FALSE;
}


gchar* create_nkr_change_image_dialog (typMascot *mascot,
				       gchar *lost_filename,
				       gint i_pix)
{
  GtkWidget *fdialog;
  gchar *win_title;
  gchar  *fname=NULL, *dir;
  gchar *dest_file=NULL;
  
  if(flagChildDialog){
    return(NULL);
  }
  else{
    flagChildDialog=TRUE;
  }

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  while (my_main_iteration(FALSE));

  win_title=g_strdup_printf(_("Select New Image for No.%02d"),i_pix);

  fdialog = gtk_file_chooser_dialog_new(win_title,
					NULL,
					GTK_FILE_CHOOSER_ACTION_OPEN,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Open", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				
  g_free(win_title);

  gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (fdialog), 
				 to_utf8(lost_filename));
  gtk_file_chooser_select_filename (GTK_FILE_CHOOSER (fdialog), 
				 to_utf8(lost_filename));

  win_title=g_strdup_printf(_("Please select the image File for No.%d."),i_pix);
  popup_message(mascot->win_main,
#ifdef USE_GTK3
		"dialog-warning", 
#else
		GTK_STOCK_DIALOG_WARNING,
#endif
		-1,
		_("The image described in INI cannot be found."),
		" ",
		win_title,
		NULL);
  g_free(win_title);

  my_file_chooser_add_shortcut_folder(fdialog,FOLDER_PIX);
  my_file_chooser_add_filter(fdialog,"BMP",BMP_EXTENSION);
  my_file_chooser_add_filter(fdialog,_("All File"),"*");

  gtk_widget_show_all(fdialog);

  if (gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {
    
    fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fdialog));
    gtk_widget_destroy(fdialog);

    dest_file=to_locale(fname);
    if(fname) g_free(fname);

    if(access(dest_file,F_OK)!=0){
      popup_message(mascot->conf_main,
#ifdef USE_GTK3
		    "dialog-error", 
#else
		    GTK_STOCK_DIALOG_ERROR,
#endif
		    -1,
		    _("Error: File cannot be opened."),
		    " ",
		    dest_file,
		    NULL);
    }
  }
  else {
    gtk_widget_destroy(fdialog);
  }


  flagChildDialog=FALSE;
  return(dest_file);
}


#ifdef USE_BIFF
static void create_change_biff_image_dialog(GtkWidget *w, gpointer gdata)
{
  typMascot *mascot;
  GtkWidget *fdialog;
  GtkWidget *button;
  GtkWidget *hbox;
  gchar *fname=NULL;
  gchar *dest_file=NULL;

  mascot=(typMascot *)gdata;

  if(flagChildDialog){
    return;
  }
  else{
    flagChildDialog=TRUE;
  }

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  while (my_main_iteration(FALSE));

  fdialog = gtk_file_chooser_dialog_new(_("Select New Image for Biff"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_OPEN,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Open", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				

  if(mascot->mail.pix_file){
    gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (fdialog), 
				   to_utf8(mascot->mail.pix_file));
    gtk_file_chooser_select_filename (GTK_FILE_CHOOSER (fdialog), 
				   to_utf8(mascot->mail.pix_file));
  }
  else{
    my_file_chooser_set_current_folder(fdialog, FOLDER_PIX); 
  }


  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_PIX);
  my_file_chooser_add_filter(fdialog,_("All File"),"*");
  my_file_chooser_add_filter(fdialog,"PNG",PNG_EXTENSION);
  my_file_chooser_add_filter(fdialog,"GIF",GIF_EXTENSION);
  my_file_chooser_add_filter(fdialog,"XPM",XPM_EXTENSION);

  gtk_widget_show_all(fdialog);
  
  if (gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {
    
    fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fdialog));
    gtk_widget_destroy(fdialog);

    dest_file=to_locale(fname);
    if(fname) g_free(fname);

    if(access(dest_file,F_OK)==0){
      if(mascot->mail.pix_file) g_free(mascot->mail.pix_file);
      mascot->mail.pix_file
	=g_strdup(FullPathPixmapFile(mascot, dest_file));
      gtk_entry_set_text(GTK_ENTRY(entry_mail_pix_file),
			 to_utf8(my_basename(mascot->mail.pix_file)));
    }
    else{
      popup_message(mascot->conf_main,
#ifdef USE_GTK3
		    "dialog-error", 
#else
		    GTK_STOCK_DIALOG_ERROR,
#endif
		    -1,
		    _("Error: File cannot be opened."),
		    " ",
		    dest_file,
		    NULL);
    }
  }
  else {
    gtk_widget_destroy(fdialog);
  }

  if(dest_file) g_free(dest_file);
  flagChildDialog=FALSE;
}
#endif  // USE_BIFF


static void create_add_image_dialog(GtkWidget *widget, gpointer gdata)
{
  typMascot *mascot;
  GtkWidget *fdialog;
  gchar *win_title;
  GSList *fnames;
  gchar *dest_file=NULL;
  gint i_pix;

  mascot=(typMascot *)gdata;

  if(flagChildDialog){
    return;
  }
  else{
    flagChildDialog=TRUE;
  }

  if(mascot->nPixmap>=MAX_PIXMAP){
    popup_message(mascot->conf_main,
#ifdef USE_GTK3
		  "dialog-error", 
#else
		  GTK_STOCK_DIALOG_ERROR,
#endif
		  -1,
		  _("Error: Append Image."),
		  " ",
		  _("The number of images maxes out."),
		  NULL);
    return;
  }

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  //while (my_main_iteration(FALSE));

  win_title=g_strdup_printf(_("Select New Image for No.%02d"),
			    mascot->nPixmap);

  fdialog = gtk_file_chooser_dialog_new(win_title,
					GTK_WINDOW(mascot->conf_main),
					GTK_FILE_CHOOSER_ACTION_OPEN,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Open", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
#endif
					NULL);
  g_free(win_title);

  gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(fdialog),TRUE);

  if(mascot->sprites[mascot->nPixmap-1].filename[0]=='/'){
    gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (fdialog), 
				   to_utf8(mascot->sprites[mascot->nPixmap-1].filename));
    gtk_file_chooser_select_filename (GTK_FILE_CHOOSER (fdialog), 
				      to_utf8(mascot->sprites[mascot->nPixmap-1].filename));
  }

  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_PIX);
  my_file_chooser_add_filter(fdialog,_("All File"),"*");
  my_file_chooser_add_filter(fdialog,"PNG",PNG_EXTENSION);
  my_file_chooser_add_filter(fdialog,"GIF",GIF_EXTENSION);
  my_file_chooser_add_filter(fdialog,"XPM",XPM_EXTENSION);

  gtk_widget_show_all(fdialog);

  if (gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {
    
    fnames = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(fdialog));
    gtk_widget_destroy(fdialog);

    i_pix=0;
    while((gchar *)g_slist_nth_data(fnames,i_pix)){
      dest_file=to_locale((gchar *)g_slist_nth_data(fnames,i_pix));
      if(access(dest_file,F_OK)==0){
	if(TestLoadPixmaps(mascot,
			   dest_file,
			   mascot->nPixmap)){
	  if(mascot->sprites[mascot->nPixmap].filename) g_free(mascot->sprites[mascot->nPixmap].filename);
	  mascot->sprites[mascot->nPixmap].filename=g_strdup(dest_file);
	  mascot->nPixmap++;
	  
	}
	else{
	  popup_message(mascot->conf_main,
#ifdef USE_GTK3
			"dialog-error", 
#else
			GTK_STOCK_DIALOG_ERROR,
#endif
			-1,
			_("Error: File cannot be loaded."),
			" ",
			(gchar *)g_slist_nth_data(fnames,i_pix),
			NULL);
	}
      }
      else{
	popup_message(mascot->conf_main,
#ifdef USE_GTK3
		      "dialog-error", 
#else
		      GTK_STOCK_DIALOG_ERROR,
#endif
		      -1,
		      _("Error: File cannot be opened."),
		      " ",
		      (gchar *)g_slist_nth_data(fnames,i_pix),
		      NULL);
      }

      if(mascot->nPixmap>=MAX_PIXMAP) break;
      i_pix++;
    }
  
    make_img_tree(mascot);
    
    g_slist_free(fnames);
  }
  else {
    gtk_widget_destroy(fdialog);
  }


  flagChildDialog=FALSE;
}


static void create_del_image_dialog(GtkWidget *widget, gpointer gdata)
{
  typMascot *mascot;
  GtkWidget *hbox;
  gchar *tmp;
  int i_pix, i_ptn, i_frm;
  gint del_pix;
  gint hit_ptn;
  gint hit_frm;

  mascot=(typMascot *)gdata;

  if(flagChildDialog){
    return;
  }
  else{
    flagChildDialog=TRUE;
  }

  if((mascot->imgtree_i<0)||(mascot->imgtree_i>=mascot->nPixmap)){
    popup_message(mascot->conf_main,
#ifdef USE_GTK3
		  "dialog-warning", 
#else
		  GTK_STOCK_DIALOG_WARNING,
#endif
		  -1,
		  _("Warning : Please select an image to delete in the list."),
		  NULL);
    return;
  }

  del_pix=mascot->imgtree_i;
  
  hit_ptn=-1;
  hit_frm=-1;
  
  
  for(i_ptn=0;i_ptn<MAX_ANIME_PATTERN;i_ptn++){
    for(i_frm=0;i_frm<MAX_ANIME_FRAME;i_frm++){
      if(mascot->frame_pix[i_ptn][i_frm]==del_pix){
	hit_ptn=i_ptn;
	hit_frm=i_frm;
	break;
      }
    }
    if(hit_ptn!=-1) break;
  }
  
  if(hit_ptn==-1){
    for(i_ptn=0;i_ptn<MAX_ANIME_PATTERN;i_ptn++){
      for(i_frm=0;i_frm<MAX_ANIME_FRAME;i_frm++){
	if(mascot->frame_pix[i_ptn][i_frm]>del_pix){
	  mascot->frame_pix[i_ptn][i_frm]--;
	}
      }
    }
    
    for(i_pix=del_pix;i_pix<mascot->nPixmap-1;i_pix++){
      mascot->sprites[i_pix].filename=g_strdup(mascot->sprites[i_pix+1].filename);
    }
    if(mascot->sprites[mascot->nPixmap-1].filename) g_free(mascot->sprites[mascot->nPixmap-1].filename);
    mascot->sprites[mascot->nPixmap-1].filename=NULL;
    mascot->nPixmap--;
    
    make_img_tree(mascot);
  }
  else{
    tmp=g_strdup_printf(_("Image %02d is used in Pattern %02d / Frame %02d\n"),
			del_pix,hit_ptn,hit_frm);
    popup_message(mascot->conf_main,
#ifdef USE_GTK3
		  "dialog-error", 
#else
		  GTK_STOCK_DIALOG_ERROR,
#endif
		  -1,
		  _("Error : Delete Image."),
		  " ",
		  tmp,
		  " ",
		  _("Please Change..."),
		  NULL);
    g_free(tmp);
  }
  
  flagChildDialog=FALSE;
  return;
}


static void create_add_pattern_dialog(GtkWidget *w, gpointer gdata)
{
  typMascot *mascot;
  GtkWidget *dialog;
  GtkWidget *button;
  GtkWidget *label;
  GtkAdjustment *adj;
  GtkWidget *spinner;
  GtkWidget *hbox;
  int i_ptn,i_frm;
  gint dest_ptn;

  mascot=(typMascot *)gdata;

  if(flagChildDialog){
    return;
  }
  else{
    flagChildDialog=TRUE;
  }

  if(mascot->ptn_num>=MAX_ANIME_PATTERN){
    popup_message(mascot->conf_main,
#ifdef USE_GTK3
		  "dialog-error", 
#else
		  GTK_STOCK_DIALOG_ERROR,
#endif
		  -1,
		  _("Error: Append Pattern."),
		  " ",
		  _("The number of patterns maxes out."),
		  NULL);
    flagChildDialog=FALSE;
    return;
  }

  dest_ptn=gtk_notebook_get_current_page(GTK_NOTEBOOK(ptn_note));
  dest_ptn++;

  dialog = gtk_dialog_new_with_buttons(_("Select Pattern No. to Append"),
				       GTK_WINDOW(mascot->conf_main),
				       GTK_DIALOG_MODAL,
#ifdef USE_GTK3
				       "_Cancel",GTK_RESPONSE_CANCEL,
				       "_OK",GTK_RESPONSE_OK,
#else
				       GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
				       GTK_STOCK_OK,GTK_RESPONSE_OK,
#endif
				       NULL);
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(dialog),5);
  
  hbox=gtkut_hbox_new(FALSE,0);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     hbox,TRUE,TRUE,0);

  label=gtkut_label_new(_("Append Pattern No."));
  gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,TRUE,0);

  adj = (GtkAdjustment *)gtk_adjustment_new 
    ((gdouble)dest_ptn, 1, (gdouble)mascot->ptn_num, 1.0, 1.0, 0.0);
  my_signal_connect (adj, "value_changed",cc_get_adj,&dest_ptn);
  spinner =  gtk_spin_button_new (adj, 0, 0);
  gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
  gtk_box_pack_start(GTK_BOX(hbox), spinner,FALSE, FALSE, 0);
  
  gtk_widget_show_all(dialog);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
    gtk_widget_destroy(dialog);
    
    for(i_ptn=mascot->ptn_num;i_ptn>dest_ptn;i_ptn--){
      mascot->frame_num[i_ptn]=mascot->frame_num[i_ptn-1];
      mascot->random_weight[i_ptn]=mascot->random_weight[i_ptn-1];
      mascot->click_weight[i_ptn]=mascot->click_weight[i_ptn-1];
      mascot->bal_lxoff[i_ptn]=mascot->bal_lxoff[i_ptn-1];
      mascot->bal_lyoff[i_ptn]=mascot->bal_lyoff[i_ptn-1];
      mascot->bal_rxoff[i_ptn]=mascot->bal_rxoff[i_ptn-1];
      mascot->bal_ryoff[i_ptn]=mascot->bal_ryoff[i_ptn-1];
      if(mascot->click_word[i_ptn]) g_free(mascot->click_word[i_ptn]);
      mascot->click_word[i_ptn]=g_strdup(mascot->click_word[i_ptn-1]);
      if(mascot->duet_tgt[i_ptn]) g_free(mascot->duet_tgt[i_ptn]);
      mascot->duet_tgt[i_ptn]=g_strdup(mascot->duet_tgt[i_ptn-1]);
      mascot->duet_ptn[i_ptn]=mascot->duet_ptn[i_ptn-1];
      if(mascot->duet_word[i_ptn]) g_free(mascot->duet_word[i_ptn]);
      mascot->duet_word[i_ptn]=g_strdup(mascot->duet_word[i_ptn-1]);
      mascot->duet_delay[i_ptn]=mascot->duet_delay[i_ptn-1];
      for(i_frm=0;i_frm<MAX_ANIME_FRAME;i_frm++){
	mascot->frame_pix[i_ptn][i_frm]=mascot->frame_pix[i_ptn-1][i_frm];
	mascot->frame_min[i_ptn][i_frm]=mascot->frame_min[i_ptn-1][i_frm];
	mascot->frame_max[i_ptn][i_frm]=mascot->frame_max[i_ptn-1][i_frm];
	mascot->frame_loop[i_ptn][i_frm].next=mascot->frame_loop[i_ptn-1][i_frm].next;
	mascot->frame_loop[i_ptn][i_frm].min=mascot->frame_loop[i_ptn-1][i_frm].min;
	mascot->frame_loop[i_ptn][i_frm].max=mascot->frame_loop[i_ptn-1][i_frm].max;
      }
      
      gtk_widget_destroy(mascot->ptntree[i_ptn-1]);
      flag_make_ptntree[i_ptn-1]=FALSE;
      gtk_notebook_remove_page(GTK_NOTEBOOK(ptn_note),i_ptn-1);
      flag_make_pattern_list[i_ptn-1]=FALSE;
    }

    mascot->frame_pix[dest_ptn][0]=0;
    mascot->frame_min[dest_ptn][0]=1;
    mascot->frame_max[dest_ptn][0]=1;
    mascot->frame_loop[dest_ptn][0].next=-1;
    mascot->frame_loop[dest_ptn][0].min=0;
    mascot->frame_loop[dest_ptn][0].max=0;
    mascot->frame_num[dest_ptn]=1;
    mascot->random_weight[dest_ptn]=0;
    mascot->click_weight[dest_ptn]=0;
    mascot->bal_lxoff[dest_ptn]=0;
    mascot->bal_lyoff[dest_ptn]=0;
    mascot->bal_rxoff[dest_ptn]=0;
    mascot->bal_ryoff[dest_ptn]=0;
    if(mascot->click_word[dest_ptn]) g_free(mascot->click_word[dest_ptn]);
    mascot->click_word[dest_ptn]=NULL;
    if(mascot->duet_tgt[dest_ptn]) g_free(mascot->duet_tgt[dest_ptn]);
    mascot->duet_tgt[dest_ptn]=NULL;
    mascot->duet_ptn[dest_ptn]=1;
    if(mascot->duet_word[dest_ptn]) g_free(mascot->duet_word[dest_ptn]);
    mascot->duet_word[dest_ptn]=NULL;
    mascot->duet_delay[dest_ptn]=DEF_DUET_DELAY;
    for(i_frm=1;i_frm<MAX_ANIME_FRAME;i_frm++){
      mascot->frame_pix[dest_ptn][i_frm]=-1;
      mascot->frame_min[dest_ptn][i_frm]=0;
      mascot->frame_max[dest_ptn][i_frm]=0;
      mascot->frame_loop[dest_ptn][i_frm].next=-1;
      mascot->frame_loop[dest_ptn][i_frm].min=0;
      mascot->frame_loop[dest_ptn][i_frm].max=0;
      mascot->frame_loop[dest_ptn][i_frm].seq=0;
      mascot->frame_loop[dest_ptn][i_frm].seqend=0;
    }
    
    mascot->ptn_num++;

    for(i_ptn=dest_ptn;i_ptn<mascot->ptn_num;i_ptn++){
      make_pattern_list(mascot, ptn_note, i_ptn);
    }
    gtk_notebook_set_current_page(GTK_NOTEBOOK(ptn_note), dest_ptn);
    gtk_widget_queue_draw(GTK_WIDGET(ptn_note));
  }
  else{
    gtk_widget_destroy(dialog);
  }

  flagChildDialog=FALSE;
  return;
}

static void create_del_pattern_dialog(GtkWidget *w, gpointer gdata)
{
  typMascot *mascot;
  int i_ptn,i_frm;
  gint dest_ptn;
  gboolean ret;
  gchar *tmp;
  
  mascot=(typMascot *)gdata;

  if(flagChildDialog){
    return;
  }
  else{
    flagChildDialog=TRUE;
  }

  dest_ptn=gtk_notebook_get_current_page(GTK_NOTEBOOK(ptn_note));
  
  if(dest_ptn==0){
    popup_message(mascot->conf_main,
#ifdef USE_GTK3
		  "dialog-error", 
#else
		  GTK_STOCK_DIALOG_ERROR,
#endif
		  -1,
		  _("Error: Delete Pattern."),
		  " ",
		  _("You cannot delete the Base pattern."),
		  NULL);
    flagChildDialog=FALSE;
    return;
  }
  

  tmp=g_strdup_printf(_("Do you really delete Pattern <b>%02d</b>?"),
		      dest_ptn);
  
  ret=popup_ask(mascot->conf_main,
#ifdef USE_GTK3
		"dialog-warning", 
#else
		GTK_STOCK_DIALOG_WARNING,
#endif
		_("Warning: Delete Pattern."),
		" ",
		tmp,
		NULL);
  g_free(tmp);

  if(!ret){
    flagChildDialog=FALSE;
    return;
  }

  for(i_ptn=dest_ptn;i_ptn<mascot->ptn_num-1;i_ptn++){
    mascot->frame_num[i_ptn]=mascot->frame_num[i_ptn+1];
    mascot->random_weight[i_ptn]=mascot->random_weight[i_ptn+1];
    mascot->click_weight[i_ptn]=mascot->click_weight[i_ptn+1];
    mascot->bal_lxoff[i_ptn]=mascot->bal_lxoff[i_ptn+1];
    mascot->bal_lyoff[i_ptn]=mascot->bal_lyoff[i_ptn+1];
    mascot->bal_rxoff[i_ptn]=mascot->bal_rxoff[i_ptn+1];
    mascot->bal_ryoff[i_ptn]=mascot->bal_ryoff[i_ptn+1];
    if(mascot->click_word[i_ptn]) g_free(mascot->click_word[i_ptn]);
    mascot->click_word[i_ptn]=g_strdup(mascot->click_word[i_ptn+1]);
    if(mascot->duet_tgt[i_ptn]) g_free(mascot->duet_tgt[i_ptn]);
    mascot->duet_tgt[i_ptn]=g_strdup(mascot->duet_tgt[i_ptn+1]);
    mascot->duet_ptn[i_ptn]=mascot->duet_ptn[i_ptn+1];
    if(mascot->duet_word[i_ptn]) g_free(mascot->duet_word[i_ptn]);
    mascot->duet_word[i_ptn]=g_strdup(mascot->duet_word[i_ptn+1]);
    mascot->duet_delay[i_ptn]=mascot->duet_delay[i_ptn+1];
    for(i_frm=0;i_frm<MAX_ANIME_FRAME;i_frm++){
      mascot->frame_pix[i_ptn][i_frm]=mascot->frame_pix[i_ptn+1][i_frm];
      mascot->frame_min[i_ptn][i_frm] =mascot->frame_min[i_ptn+1][i_frm];
      mascot->frame_max[i_ptn][i_frm] =mascot->frame_max[i_ptn+1][i_frm];
      mascot->frame_loop[i_ptn][i_frm].next=mascot->frame_loop[i_ptn+1][i_frm].next;
      mascot->frame_loop[i_ptn][i_frm].min=mascot->frame_loop[i_ptn+1][i_frm].min;
      mascot->frame_loop[i_ptn][i_frm].max=mascot->frame_loop[i_ptn+1][i_frm].max;
    }
  }

  for(i_ptn=mascot->ptn_num-1;i_ptn>=dest_ptn;i_ptn--){
    gtk_widget_destroy(mascot->ptntree[i_ptn]);
    flag_make_ptntree[i_ptn]=FALSE;
    flag_make_pattern_list[i_ptn]=FALSE;
    gtk_notebook_remove_page(GTK_NOTEBOOK(ptn_note),i_ptn);
  }
  
  mascot->frame_num[mascot->ptn_num-1]=0;
  mascot->random_weight[mascot->ptn_num-1]=0;
  mascot->click_weight[mascot->ptn_num-1]=0;
  mascot->bal_lxoff[mascot->ptn_num-1]=0;
  mascot->bal_lyoff[mascot->ptn_num-1]=0;
  mascot->bal_rxoff[mascot->ptn_num-1]=0;
  mascot->bal_ryoff[mascot->ptn_num-1]=0;
  if(mascot->click_word[mascot->ptn_num-1])
    g_free(mascot->click_word[mascot->ptn_num-1]);
  mascot->click_word[mascot->ptn_num-1]=NULL;
  if(mascot->duet_tgt[mascot->ptn_num-1])
    g_free(mascot->duet_tgt[mascot->ptn_num-1]);
  mascot->duet_tgt[mascot->ptn_num-1]=NULL;
  mascot->duet_ptn[mascot->ptn_num-1]=1;
  if(mascot->duet_word[mascot->ptn_num-1])
    g_free(mascot->duet_word[mascot->ptn_num-1]);
  mascot->duet_word[mascot->ptn_num-1]=NULL;
  mascot->duet_delay[mascot->ptn_num-1]=DEF_DUET_DELAY;
  for(i_frm=0;i_frm<MAX_ANIME_FRAME;i_frm++){
    mascot->frame_pix[mascot->ptn_num-1][i_frm]=-1;
    mascot->frame_min[mascot->ptn_num-1][i_frm]=0;
    mascot->frame_max[mascot->ptn_num-1][i_frm]=0;
    mascot->frame_loop[mascot->ptn_num-1][i_frm].next=-1;
    mascot->frame_loop[mascot->ptn_num-1][i_frm].min=0;
    mascot->frame_loop[mascot->ptn_num-1][i_frm].max=0;
    mascot->frame_loop[mascot->ptn_num-1][i_frm].seq=0;
    mascot->frame_loop[mascot->ptn_num-1][i_frm].seqend=0;
  }
    
  mascot->ptn_num--;

  for(i_ptn=dest_ptn;i_ptn<mascot->ptn_num;i_ptn++){
    make_pattern_list(mascot, ptn_note, i_ptn);
  }

  if(dest_ptn<=mascot->ptn_num){
    gtk_notebook_set_current_page(GTK_NOTEBOOK(ptn_note), dest_ptn-1);
  }
  else{
    gtk_notebook_set_current_page(GTK_NOTEBOOK(ptn_note), dest_ptn);
  }
  gtk_widget_queue_draw(GTK_WIDGET(ptn_note));
  
  flagChildDialog=FALSE;
  return;
}


static void create_copy_pattern_dialog(GtkWidget *w, gpointer gdata)
{
  typMascot *mascot;
  GtkWidget *dialog;
  GtkWidget *button;
  GtkWidget *label;
  GtkAdjustment *adj;
  GtkWidget *spinner;
  GtkWidget *hbox;
  int i_ptn,i_frm;
  gint from_ptn;
  gint dest_ptn;
  gchar *tmp;

  mascot=(typMascot *)gdata;

  if(flagChildDialog){
    return;
  }
  else{
    flagChildDialog=TRUE;
  }

  if(mascot->ptn_num>=MAX_ANIME_PATTERN){
    popup_message(mascot->conf_main,
#ifdef USE_GTK3
		  "dialog-error", 
#else
		  GTK_STOCK_DIALOG_ERROR,
#endif
		  -1,
		  _("Error: Append Pattern."),
		  " ",
		  _("The number of patterns maxes out."),
		  NULL);
    flagChildDialog=FALSE;
    return;
  }

  from_ptn=gtk_notebook_get_current_page(GTK_NOTEBOOK(ptn_note));

  if(from_ptn==0){
    popup_message(mascot->conf_main,
#ifdef USE_GTK3
		  "dialog-error", 
#else
		  GTK_STOCK_DIALOG_ERROR,
#endif
		  -1,
		  _("Error: Copy Pattern."),
		  " ",
		  _("You cannot copy the Base pattern."),
		  NULL);
    flagChildDialog=FALSE;
    return;
  }


  dest_ptn=mascot->ptn_num;

  dialog = gtk_dialog_new_with_buttons(_("Pattern Copy"),
				       GTK_WINDOW(mascot->conf_main),
				       GTK_DIALOG_MODAL,
#ifdef USE_GTK3
				       "_Cancel",GTK_RESPONSE_CANCEL,
				       "_OK",GTK_RESPONSE_OK,
#else
				       GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
				       GTK_STOCK_OK,GTK_RESPONSE_OK,
#endif
				       NULL);
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(dialog),5);
  
  hbox=gtkut_hbox_new(FALSE,0);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     hbox,TRUE,TRUE,0);

  tmp=g_strdup_printf(_("Pattern Copy No. <b>%02d</b> to No. "),
		      from_ptn);
  label=gtkut_label_new(tmp);
  gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,TRUE,0);

  adj = (GtkAdjustment *)gtk_adjustment_new 
    ((gdouble)dest_ptn,
     1,(gdouble)mascot->ptn_num,1.0, 1.0, 0.0);
  my_signal_connect (adj, "value_changed",cc_get_adj,&dest_ptn);
  spinner =  gtk_spin_button_new (adj, 0, 0);
  gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
  gtk_box_pack_start(GTK_BOX(hbox), spinner,FALSE, FALSE, 0);
  
  gtk_widget_show_all(dialog);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
    gtk_widget_destroy(dialog);

    for(i_ptn=mascot->ptn_num;i_ptn>dest_ptn;i_ptn--){
      mascot->frame_num[i_ptn]=mascot->frame_num[i_ptn-1];
      mascot->random_weight[i_ptn]=mascot->random_weight[i_ptn-1];
      mascot->click_weight[i_ptn]=mascot->click_weight[i_ptn-1];
      mascot->bal_lxoff[i_ptn]=mascot->bal_lxoff[i_ptn-1];
      mascot->bal_lyoff[i_ptn]=mascot->bal_lyoff[i_ptn-1];
      mascot->bal_rxoff[i_ptn]=mascot->bal_rxoff[i_ptn-1];
      mascot->bal_ryoff[i_ptn]=mascot->bal_ryoff[i_ptn-1];
      if(mascot->click_word[i_ptn]) g_free(mascot->click_word[i_ptn]);
      mascot->click_word[i_ptn]=g_strdup(mascot->click_word[i_ptn-1]);
      if(mascot->duet_tgt[i_ptn]) g_free(mascot->duet_tgt[i_ptn]);
      mascot->duet_tgt[i_ptn]=g_strdup(mascot->duet_tgt[i_ptn-1]);
      mascot->duet_ptn[i_ptn]=mascot->duet_ptn[i_ptn-1];
      if(mascot->duet_word[i_ptn]) g_free(mascot->duet_word[i_ptn]);
      mascot->duet_word[i_ptn]=g_strdup(mascot->duet_word[i_ptn-1]);
      mascot->duet_delay[i_ptn]=mascot->duet_delay[i_ptn-1];
      for(i_frm=0;i_frm<MAX_ANIME_FRAME;i_frm++){
	mascot->frame_pix[i_ptn][i_frm]=mascot->frame_pix[i_ptn-1][i_frm];
	mascot->frame_min[i_ptn][i_frm]=mascot->frame_min[i_ptn-1][i_frm];
	mascot->frame_max[i_ptn][i_frm]=mascot->frame_max[i_ptn-1][i_frm];
	mascot->frame_loop[i_ptn][i_frm].next=mascot->frame_loop[i_ptn-1][i_frm].next;
	mascot->frame_loop[i_ptn][i_frm].min=mascot->frame_loop[i_ptn-1][i_frm].min;
	mascot->frame_loop[i_ptn][i_frm].max=mascot->frame_loop[i_ptn-1][i_frm].max;
      }
      
      gtk_widget_destroy(mascot->ptntree[i_ptn-1]);
      flag_make_ptntree[i_ptn-1]=FALSE;
      gtk_notebook_remove_page(GTK_NOTEBOOK(ptn_note),i_ptn-1);
      flag_make_pattern_list[i_ptn-1]=FALSE;
    }

    mascot->frame_num[dest_ptn]=mascot->frame_num[from_ptn];
    mascot->random_weight[dest_ptn]=mascot->random_weight[from_ptn];
    mascot->click_weight[dest_ptn]=mascot->click_weight[from_ptn];
    mascot->bal_lxoff[dest_ptn]=mascot->bal_lxoff[from_ptn];
    mascot->bal_lyoff[dest_ptn]=mascot->bal_lyoff[from_ptn];
    mascot->bal_rxoff[dest_ptn]=mascot->bal_rxoff[from_ptn];
    mascot->bal_ryoff[dest_ptn]=mascot->bal_ryoff[from_ptn];
    if(mascot->click_word[dest_ptn]) g_free(mascot->click_word[dest_ptn]);
    mascot->click_word[dest_ptn]=g_strdup(mascot->click_word[from_ptn]);
    if(mascot->duet_tgt[dest_ptn]) g_free(mascot->duet_tgt[dest_ptn]);
    mascot->duet_tgt[dest_ptn]=g_strdup(mascot->duet_tgt[from_ptn]);
    mascot->duet_ptn[dest_ptn]=mascot->duet_ptn[from_ptn];
    if(mascot->duet_word[dest_ptn]) g_free(mascot->duet_word[dest_ptn]);
    mascot->duet_word[dest_ptn]=g_strdup(mascot->duet_word[from_ptn]);
    mascot->duet_delay[dest_ptn]=mascot->duet_delay[from_ptn];
    for(i_frm=0;i_frm<MAX_ANIME_FRAME;i_frm++){
      mascot->frame_pix[dest_ptn][i_frm]=mascot->frame_pix[from_ptn][i_frm];
      mascot->frame_min[dest_ptn][i_frm]=mascot->frame_min[from_ptn][i_frm];
      mascot->frame_max[dest_ptn][i_frm]=mascot->frame_max[from_ptn][i_frm];
      mascot->frame_loop[dest_ptn][i_frm].next=mascot->frame_loop[from_ptn][i_frm].next;
      mascot->frame_loop[dest_ptn][i_frm].min=mascot->frame_loop[from_ptn][i_frm].min;
      mascot->frame_loop[dest_ptn][i_frm].max=mascot->frame_loop[from_ptn][i_frm].max;
    }
    
    mascot->ptn_num++;

    for(i_ptn=dest_ptn;i_ptn<mascot->ptn_num;i_ptn++){
      make_pattern_list(mascot, ptn_note, i_ptn);
    }
    gtk_notebook_set_current_page(GTK_NOTEBOOK(ptn_note), dest_ptn);
    gtk_widget_queue_draw(GTK_WIDGET(ptn_note));
  }
  else{
    gtk_widget_destroy(dialog);
  }

  flagChildDialog=FALSE;
  return;
}


static void create_add_frame_dialog(GtkWidget *w, gpointer gdata)
{
  confNum *mptn;
  typMascot *mascot;
  GtkWidget *dialog;
  GtkWidget *button;
  GtkWidget *label;
  GtkAdjustment *adj;
  GtkWidget *spinner;
  GtkWidget *hbox;
  int i_ptn,i_frm;
  gint dest_frm;

  mptn=(confNum *)gdata;

  i_ptn=mptn->num;
  mascot=mptn->mascot;
  
  if(flagChildDialog){
    return;
  }
  else{
    flagChildDialog=TRUE;
  }

  if(mascot->frame_num[i_ptn]>=MAX_ANIME_FRAME){
    popup_message(mascot->conf_main,
#ifdef USE_GTK3
		  "dialog-error", 
#else
		  GTK_STOCK_DIALOG_ERROR,
#endif
		  -1,
		  _("Error: Append Frame."),
		  " ",
		  _("The number of frames maxes out."),
		  NULL);
    return;
  }

  if((mascot->ptntree_i_frm[i_ptn]<0)
     || (mascot->ptntree_i_frm[i_ptn]>=mascot->frame_num[i_ptn])){
    dest_frm=mascot->frame_num[i_ptn];
  }
  else{
    dest_frm=mascot->ptntree_i_frm[i_ptn];
  }
  dest_frm++;

  dialog = gtk_dialog_new_with_buttons(_("Select Frame No. to Append"),
				       GTK_WINDOW(mascot->conf_main),
				       GTK_DIALOG_MODAL,
#ifdef USE_GTK3
				       "_Cancel",GTK_RESPONSE_CANCEL,
				       "_OK",GTK_RESPONSE_OK,
#else
				       GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
				       GTK_STOCK_OK,GTK_RESPONSE_OK,
#endif
				       NULL);
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(dialog),5);
  
  hbox=gtkut_hbox_new(FALSE,0);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     hbox,TRUE,TRUE,0);

  label=gtkut_label_new(_("Append Frame No."));
  gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,TRUE,0);

  adj = (GtkAdjustment *)gtk_adjustment_new 
    ((gdouble)dest_frm, 1,
     (gdouble)mascot->frame_num[i_ptn]+1,1.0, 1.0, 0.0);
  my_signal_connect (adj, "value_changed",cc_get_adj,&dest_frm);
  spinner =  gtk_spin_button_new (adj, 0, 0);
  gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
  gtk_box_pack_start(GTK_BOX(hbox), spinner,FALSE, FALSE, 0);
  
  gtk_widget_show_all(dialog);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
    gtk_widget_destroy(dialog);

    dest_frm--;

    for(i_frm=mascot->frame_num[i_ptn];i_frm>dest_frm;i_frm--){
      mascot->frame_pix[i_ptn][i_frm]=mascot->frame_pix[i_ptn][i_frm-1];
      mascot->frame_min[i_ptn][i_frm]=mascot->frame_min[i_ptn][i_frm-1];
      mascot->frame_max[i_ptn][i_frm]=mascot->frame_max[i_ptn][i_frm-1];
      mascot->frame_loop[i_ptn][i_frm].next
	=mascot->frame_loop[i_ptn][i_frm-1].next;
      mascot->frame_loop[i_ptn][i_frm].min
	=mascot->frame_loop[i_ptn][i_frm-1].min;
      mascot->frame_loop[i_ptn][i_frm].max
	=mascot->frame_loop[i_ptn][i_frm-1].max;
    }
    mascot->frame_pix[i_ptn][dest_frm]=0;
    mascot->frame_min[i_ptn][dest_frm]=1;
    mascot->frame_max[i_ptn][dest_frm]=1;
    mascot->frame_loop[i_ptn][dest_frm].next=-1;
    mascot->frame_loop[i_ptn][dest_frm].min=0;
    mascot->frame_loop[i_ptn][dest_frm].max=0;

    mascot->frame_num[i_ptn]++;

    //make_frame_list(mascot, i_ptn);
    make_ptn_tree(mascot, i_ptn);
  }
  else{
    gtk_widget_destroy(dialog);
  }
  
  flagChildDialog=FALSE;
  return;
}



static void create_del_frame_dialog(GtkWidget *w, gpointer gdata)
{
  confNum *mptn;
  typMascot *mascot;
  GtkWidget *dialog;
  GtkWidget *button;
  GtkWidget *label;
  GtkAdjustment *adj;
  GtkWidget *spinner;
  GtkWidget *hbox;
  int i_ptn,i_frm;
  gint dest_frm;

  mptn=(confNum *)gdata;

  i_ptn=mptn->num;
  mascot=mptn->mascot;
  
  if(flagChildDialog){
    return;
  }
  else{
    flagChildDialog=TRUE;
  }

  if(mascot->frame_num[i_ptn]==1){
    popup_message(mascot->conf_main,
#ifdef USE_GTK3
		  "dialog-error", 
#else
		  GTK_STOCK_DIALOG_ERROR,
#endif
		  -1,
		  _("Error: Delete Frame."),
		  " ",
		  _("You cannot delete no more frames."),
		  NULL);
    return;
  }

  if((mascot->ptntree_i_frm[i_ptn]<0)
     || (mascot->ptntree_i_frm[i_ptn]>=mascot->frame_num[i_ptn])){
    popup_message(mascot->conf_main,
#ifdef USE_GTK3
		  "dialog-error", 
#else
		  GTK_STOCK_DIALOG_ERROR,
#endif
		  -1,
		  _("Error: Delete Frame."),
		  " ",
		  _("Please select a frame to delete."),
		  NULL);
    return;
  }

  
  dest_frm=mascot->ptntree_i_frm[i_ptn];


  for(i_frm=dest_frm;i_frm<mascot->frame_num[i_ptn]-1;i_frm++){
    mascot->frame_pix[i_ptn][i_frm]=mascot->frame_pix[i_ptn][i_frm+1];
    mascot->frame_min[i_ptn][i_frm]=mascot->frame_min[i_ptn][i_frm+1];
    mascot->frame_max[i_ptn][i_frm]=mascot->frame_max[i_ptn][i_frm+1];
    mascot->frame_loop[i_ptn][i_frm].next
      =mascot->frame_loop[i_ptn][i_frm+1].next;
    mascot->frame_loop[i_ptn][i_frm].min
      =mascot->frame_loop[i_ptn][i_frm+1].min;
    mascot->frame_loop[i_ptn][i_frm].max
      =mascot->frame_loop[i_ptn][i_frm+1].max;
  }
  mascot->frame_pix[i_ptn][mascot->frame_num[i_ptn]-1]=-1;
  mascot->frame_min[i_ptn][mascot->frame_num[i_ptn]-1]=0;
  mascot->frame_max[i_ptn][mascot->frame_num[i_ptn]-1]=0;
  mascot->frame_loop[i_ptn][mascot->frame_num[i_ptn]-1].next=-1;
  mascot->frame_loop[i_ptn][mascot->frame_num[i_ptn]-1].min=0;
  mascot->frame_loop[i_ptn][mascot->frame_num[i_ptn]-1].max=0;
  
  mascot->frame_num[i_ptn]--;
  
  //make_frame_list(mascot, i_ptn);
  make_ptn_tree(mascot, i_ptn);

  flagChildDialog=FALSE;
}

static void create_change_tgt_dialog(GtkWidget *w, gpointer gdata)
{
  confNum2 *mtgt;
  typMascot *mascot;
  int i_tmp, i_cat, i_tgt;
  GtkWidget *fdialog;
  gchar *fname=NULL;
  gchar *dest_file=NULL;
    
  mtgt=(confNum2 *)gdata;

  i_cat=mtgt->num;
  i_tgt=mtgt->num2;
  mascot=mtgt->mascot;
  
  if(flagChildDialog){
    return;
  }
  else{
    flagChildDialog=TRUE;
  }

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  while (my_main_iteration(FALSE));

  fdialog = gtk_file_chooser_dialog_new(_("Select Mascot File"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_OPEN,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Open", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				

  gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (fdialog), 
	       to_utf8(FullPathMascotFile(mascot, mascot->menu_tgt[i_cat][i_tgt])));
  gtk_file_chooser_select_filename (GTK_FILE_CHOOSER (fdialog), 
	       to_utf8(FullPathMascotFile(mascot, mascot->menu_tgt[i_cat][i_tgt])));


  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_DEFAULT);
  my_file_chooser_add_filter(fdialog,_("Mascot File"),MASCOT_EXTENSION);
  my_file_chooser_add_filter(fdialog,_("All File"),"*");

  gtk_widget_show_all(fdialog);

  if (gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {
    fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fdialog));
    gtk_widget_destroy(fdialog);
    
    dest_file=to_locale(fname);
    if(fname) g_free(fname);

    if(access(dest_file,F_OK)==0){
      if(mascot->menu_tgt[i_cat][i_tgt]) g_free(mascot->menu_tgt[i_cat][i_tgt]);
      mascot->menu_tgt[i_cat][i_tgt]=g_strdup(my_basename(dest_file));

      if(mascot->menu_tgt_name[i_cat][i_tgt]) g_free(mascot->menu_tgt_name[i_cat][i_tgt]);
      mascot->menu_tgt_name[i_cat][i_tgt]
	=ReadMascotName(mascot, mascot->menu_tgt[i_cat][i_tgt]);
      make_tgt_list(mascot, i_cat);
    }
    else{
      popup_message(mascot->win_main,
#ifdef USE_GTK3
		    "dialog-error", 
#else
		    GTK_STOCK_DIALOG_ERROR,
#endif
		    -1,
		    _("Error: File cannot be opened."),
		    " ",
		    dest_file,
		    NULL);
    }
  }
  else {
    gtk_widget_destroy(fdialog);
  }

  if(dest_file) g_free(dest_file);
  flagChildDialog=FALSE;
}


static void create_add_tgt_dialog(GtkWidget *w, gpointer gdata)
{
  confNum *mcat;
  typMascot *mascot;
  int i_cat, i_tgt;
  GtkWidget *fdialog;
  gchar *fname=NULL;
  gchar *dest_file=NULL;

  mcat=(confNum *)gdata;
  
  i_cat=mcat->num;
  mascot=mcat->mascot;

  if(flagChildDialog){
    return;
  }
  else{
    flagChildDialog=TRUE;
  }

  if(mascot->menu_tgt_max[i_cat]>=MAX_MENU_TARGET){
    popup_message(mascot->win_main,
#ifdef USE_GTK3
		  "dialog-error", 
#else
		  GTK_STOCK_DIALOG_ERROR,
#endif
		  -1,
		  _("Error: Append Mascot."),
		  " ",
		  _("The number of mascots maxes out."),
		  NULL);
    return;
  }

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  //while (my_main_iteration(FALSE));

  fdialog = gtk_file_chooser_dialog_new(_("Select New Mascot File"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_OPEN,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Open", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				


  i_tgt=mascot->menu_tgt_max[i_cat];

  gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (fdialog), 
				 to_utf8(FullPathMascotFile(mascot, 
							    mascot->menu_tgt[i_cat][i_tgt-1])));
  gtk_file_chooser_select_filename (GTK_FILE_CHOOSER (fdialog), 
 	  to_utf8(FullPathMascotFile(mascot, 
				     mascot->menu_tgt[i_cat][i_tgt-1])));


  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_DEFAULT);
  my_file_chooser_add_filter(fdialog,_("Mascot File"),MASCOT_EXTENSION);
  my_file_chooser_add_filter(fdialog,_("All File"),"*");


  gtk_widget_show_all(fdialog);
  
  if (gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {
    fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fdialog));
    gtk_widget_destroy(fdialog);
    
    dest_file=to_locale(fname);
    if(fname) g_free(fname);

    if(access(dest_file,F_OK)==0){
      if(mascot->menu_tgt[i_cat][i_tgt]) g_free(mascot->menu_tgt[i_cat][i_tgt]);
      mascot->menu_tgt[i_cat][i_tgt]=g_strdup(my_basename(dest_file));
      
      mascot->menu_tgt_max[i_cat]++;
      mascot->menu_total++;
      if(mascot->menu_tgt_name[i_cat][i_tgt]) g_free(mascot->menu_tgt_name[i_cat][i_tgt]);
      mascot->menu_tgt_name[i_cat][i_tgt]
	=ReadMascotName(mascot, mascot->menu_tgt[i_cat][i_tgt]);
      make_tgt_list(mascot, i_cat);
    }
    else{
      popup_message(mascot->win_main,
#ifdef USE_GTK3
		    "dialog-error", 
#else
		    GTK_STOCK_DIALOG_ERROR,
#endif
		    -1,
		    _("Error: File cannot be opened."),
		    " ",
		    dest_file,
		    NULL);
    }
  }
  else {
    gtk_widget_destroy(fdialog);
  }

  if(dest_file) g_free(dest_file);
  flagChildDialog=FALSE;
}


static void create_del_tgt_dialog(GtkWidget *w, gpointer gdata)
{
  confNum *mcat;
  typMascot *mascot;
  int i_cat, i_tgt, dest_tgt;
  GtkWidget *dialog;
  GtkWidget *button;
  GtkWidget *label;
  GtkAdjustment *adj;
  GtkWidget *spinner;
  GtkWidget *hbox;

  mcat=(confNum *)gdata;
  
  i_cat=mcat->num;
  mascot=mcat->mascot;

  if(flagChildDialog){
    return;
  }
  else{
    flagChildDialog=TRUE;
  }

  if(mascot->menu_tgt_max[i_cat]==1){
    popup_message(Mascot->win_main,
#ifdef USE_GTK3
		  "dialog-error", 
#else
		  GTK_STOCK_DIALOG_ERROR,
#endif
		  -1,
		  _("Error: Delete Mascot."),
		  " ",
		  _("You cannot delete no more mascots."),
		  NULL);
    return;
  }


  dest_tgt=mascot->menu_tgt_max[i_cat]-1;

  dialog = gtk_dialog_new_with_buttons(_("Select Mascot No. to Remove from the Menu"),
				       NULL,
				       GTK_DIALOG_MODAL,
#ifdef USE_GTK3
				       "_Cancel",GTK_RESPONSE_CANCEL,
				       "_OK",GTK_RESPONSE_OK,
#else
				       GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
				       GTK_STOCK_OK,GTK_RESPONSE_OK,
#endif
				       NULL);
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(dialog),5);

  hbox=gtkut_hbox_new(FALSE,0);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     hbox,TRUE,TRUE,0);

  label=gtkut_label_new(_("Remove Mascot No."));
  gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,TRUE,0);

  adj = (GtkAdjustment *)gtk_adjustment_new 
    ((gdouble)dest_tgt, 0,(gdouble)dest_tgt,1.0, 1.0, 0.0);
  my_signal_connect (adj, "value_changed",cc_get_adj,&dest_tgt);
  spinner =  gtk_spin_button_new (adj, 0, 0);
  gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
  gtk_box_pack_start(GTK_BOX(hbox), spinner,FALSE, FALSE, 0); 
  
  gtk_widget_show_all(dialog);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
    gtk_widget_destroy(dialog);

    mascot->menu_tgt_max[i_cat]--;
    mascot->menu_total--;
    for(i_tgt=dest_tgt;i_tgt<mascot->menu_tgt_max[i_cat];i_tgt++){
      if(mascot->menu_tgt[i_cat][i_tgt]) g_free(mascot->menu_tgt[i_cat][i_tgt]);
      mascot->menu_tgt[i_cat][i_tgt]=g_strdup(mascot->menu_tgt[i_cat][i_tgt+1]);
      if(mascot->menu_tgt_name[i_cat][i_tgt]) g_free(mascot->menu_tgt_name[i_cat][i_tgt]);
      mascot->menu_tgt_name[i_cat][i_tgt]=g_strdup(mascot->menu_tgt_name[i_cat][i_tgt+1]);
    }

    make_tgt_list(mascot, i_cat);
  }
  else{
    gtk_widget_destroy(dialog);
  }

  flagChildDialog=FALSE;
  return;
}


static void create_add_cat_dialog(GtkWidget *w, gpointer gdata)
{
  typMascot *mascot;
  GtkWidget *dialog;
  GtkWidget *button;
  GtkWidget *label;
  GtkAdjustment *adj;
  GtkWidget *spinner;
  GtkWidget *hbox;
  int i_cat, i_tgt;
  gint dest_cat;

  mascot=(typMascot *)gdata;

  if(flagChildDialog){
    return;
  }
  else{
    flagChildDialog=TRUE;
  }

  if(mascot->menu_cat_max>=MAX_MENU_CATEGORY){
    popup_message(mascot->win_main,
#ifdef USE_GTK3
		  "dialog-error", 
#else
		  GTK_STOCK_DIALOG_ERROR,
#endif
		  -1,
		  _("Error: Append Category."),
		  " ",
		  _("The number of categories maxes out."),
		  NULL);
    return;
  }

  dest_cat=mascot->menu_cat_max;

  dialog = gtk_dialog_new_with_buttons(_("Select Category No. to Append"),
				       NULL,
				       GTK_DIALOG_MODAL,
#ifdef USE_GTK3
				       "_Cancel",GTK_RESPONSE_CANCEL,
				       "_OK",GTK_RESPONSE_OK,
#else
				       GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
				       GTK_STOCK_OK,GTK_RESPONSE_OK,
#endif
				       NULL);
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(dialog),5);
  
  hbox=gtkut_hbox_new(FALSE,0);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     hbox,TRUE,TRUE,0);

  label=gtkut_label_new(_("Append Category No."));
  gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,TRUE,0);

  adj = (GtkAdjustment *)gtk_adjustment_new 
    ((gdouble)mascot->menu_cat_max, 0,(gdouble)mascot->menu_cat_max,
     1.0, 1.0, 0.0);
  my_signal_connect (adj, "value_changed",cc_get_adj,&dest_cat);
  spinner =  gtk_spin_button_new (adj, 0, 0);
  gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
  gtk_box_pack_start(GTK_BOX(hbox), spinner,FALSE, FALSE, 0);
  
 
  gtk_widget_show_all(dialog);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
    gtk_widget_destroy(dialog);

    for(i_cat=mascot->menu_cat_max;i_cat>dest_cat;i_cat--){
      if(mascot->menu_cat[i_cat]) g_free(mascot->menu_cat[i_cat]);
      mascot->menu_cat[i_cat]=g_strdup(mascot->menu_cat[i_cat-1]);
      mascot->menu_tgt_max[i_cat]=mascot->menu_tgt_max[i_cat-1];
      for(i_tgt=0;i_tgt<mascot->menu_tgt_max[i_cat];i_tgt++){
	if(mascot->menu_tgt[i_cat][i_tgt]) g_free(mascot->menu_tgt[i_cat][i_tgt]);
	mascot->menu_tgt[i_cat][i_tgt]
	  =g_strdup(mascot->menu_tgt[i_cat-1][i_tgt]);
	if(mascot->menu_tgt_name[i_cat][i_tgt]) g_free(mascot->menu_tgt_name[i_cat][i_tgt]);
	mascot->menu_tgt_name[i_cat][i_tgt]
	  =g_strdup(mascot->menu_tgt_name[i_cat-1][i_tgt]);
      }
    }

    if(mascot->menu_cat[dest_cat]) g_free(mascot->menu_cat[dest_cat]);
    mascot->menu_cat[dest_cat]=g_strdup(TMP_CATEGORY_NAME);
    mascot->menu_tgt_max[dest_cat]=1;

    if(mascot->menu_tgt[dest_cat][0]) g_free(mascot->menu_tgt[dest_cat][0]);
    mascot->menu_tgt[dest_cat][0]
      =g_strdup(my_basename(mascot->file));
    if(mascot->menu_tgt_name[dest_cat][0]) g_free(mascot->menu_tgt_name[dest_cat][0]);
    mascot->menu_tgt_name[dest_cat][0]
      =ReadMascotName(mascot, mascot->file);

    
    mascot->menu_cat_max++;
    mascot->menu_total++;

    if(dest_cat==mascot->menu_cat_max-1){
      make_cat_list(mascot, cat_note, dest_cat-1);
    } // Up/Downタグが付くようになる

    for(i_cat=dest_cat;i_cat<mascot->menu_cat_max;i_cat++){
      make_cat_list(mascot, cat_note, i_cat);
    }
    gtk_notebook_set_current_page(GTK_NOTEBOOK(cat_note), dest_cat);
    gtk_widget_queue_draw(GTK_WIDGET(cat_note));
  }
  else{
    gtk_widget_destroy(dialog);
  }
  
  flagChildDialog=FALSE;

  return;
}



static void create_del_cat_dialog(GtkWidget *w, gpointer gdata)
{
  typMascot *mascot;
  GtkWidget *dialog;
  GtkWidget *button;
  GtkWidget *label;
  GtkAdjustment *adj;
  GtkWidget *spinner;
  GtkWidget *hbox;
  int i_cat, i_tgt, dest_menu_tgt_max;
  gint dest_cat;

  mascot=(typMascot *)gdata;

  if(flagChildDialog){
    return;
  }
  else{
    flagChildDialog=TRUE;
  }

  if(mascot->menu_cat_max<=1){
    popup_message(mascot->win_main,
#ifdef USE_GTK3
		  "dialog-error", 
#else
		  GTK_STOCK_DIALOG_ERROR,
#endif
		  -1,
		  _("Error: Delete Category."),
		  " ",
		  _("You cannot delete no more categories."),
		  NULL);
    return;
  }
  

  dest_cat=mascot->menu_cat_max-1;

  dialog = gtk_dialog_new_with_buttons(_("Select Category No. to Delete"),
				       NULL,
				       GTK_DIALOG_MODAL,
#ifdef USE_GTK3
				       "_Cancel",GTK_RESPONSE_CANCEL,
				       "_OK",GTK_RESPONSE_OK,
#else
				       GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
				       GTK_STOCK_OK,GTK_RESPONSE_OK,
#endif
				       NULL);
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(dialog),5);
  
  hbox=gtkut_hbox_new(FALSE,0);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     hbox,TRUE,TRUE,0);

  label=gtkut_label_new(_("Delete Category No."));
  gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,TRUE,0);

  adj = (GtkAdjustment *)gtk_adjustment_new 
    ((gdouble)gtk_notebook_get_current_page(GTK_NOTEBOOK(cat_note)),
     0,(gdouble)mascot->menu_cat_max-1,
     1.0, 1.0, 0.0);
  my_signal_connect (adj, "value_changed",cc_get_adj,&dest_cat);
  spinner =  gtk_spin_button_new (adj, 0, 0);
  gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
  gtk_box_pack_start(GTK_BOX(hbox), spinner,FALSE, FALSE, 0);
  
  gtk_widget_show_all(dialog);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
    gtk_widget_destroy(dialog);

    dest_menu_tgt_max=mascot->menu_tgt_max[dest_cat];

    for(i_cat=dest_cat;i_cat<mascot->menu_cat_max-1;i_cat++){
      if(mascot->menu_cat[i_cat]) g_free(mascot->menu_cat[i_cat]);
      mascot->menu_cat[i_cat]
	=g_strdup(mascot->menu_cat[i_cat+1]);
      mascot->menu_tgt_max[i_cat]=mascot->menu_tgt_max[i_cat+1];
      for(i_tgt=0;i_tgt<mascot->menu_tgt_max[i_cat];i_tgt++){
	if(mascot->menu_tgt[i_cat][i_tgt]) g_free(mascot->menu_tgt[i_cat][i_tgt]);
	mascot->menu_tgt[i_cat][i_tgt]
	  =g_strdup(mascot->menu_tgt[i_cat+1][i_tgt]);
	if(mascot->menu_tgt_name[i_cat][i_tgt]) g_free(mascot->menu_tgt_name[i_cat][i_tgt]);
	mascot->menu_tgt_name[i_cat][i_tgt]
	  =g_strdup(mascot->menu_tgt_name[i_cat+1][i_tgt]);
      }
    }

    gtk_notebook_remove_page(GTK_NOTEBOOK(cat_note),mascot->menu_cat_max-1);
    flag_make_tgt_list[mascot->menu_cat_max-1]=FALSE;
    flag_make_cat_list[mascot->menu_cat_max-1]=FALSE;
    gtk_widget_queue_draw (GTK_WIDGET(cat_note));

    mascot->menu_cat_max--;
    mascot->menu_total-=dest_menu_tgt_max;


    for(i_cat=dest_cat;i_cat<mascot->menu_cat_max;i_cat++){
      make_cat_list(mascot, cat_note, i_cat);
    }
    
    if(dest_cat==mascot->menu_cat_max){
      make_cat_list(mascot, cat_note, dest_cat-1);
    }

    if(dest_cat<mascot->menu_cat_max){
      gtk_notebook_set_current_page(GTK_NOTEBOOK(cat_note), dest_cat);
    }
    else{
      gtk_notebook_set_current_page(GTK_NOTEBOOK(cat_note), mascot->menu_cat_max-1);
    }
    gtk_widget_queue_draw(GTK_WIDGET(cat_note));
  }
  else{
    gtk_widget_destroy(dialog);
  }
  
  flagChildDialog=FALSE;
  return;
}


static void create_merge_cat_dialog(GtkWidget *w, gpointer gdata)
{
  typMascot *mascot;
  int i_cat, old_cat_max;
  GtkWidget *fdialog;
  GSList *fnames;
  gchar *merge_file;
  gint menu_cat_max_old;
  int i_menu;
  gboolean update;
  gchar *dest_file=NULL;

  mascot=(typMascot *)gdata;

  if(flagChildDialog){
    return;
  }
  else{
    flagChildDialog=TRUE;
  }

  if(mascot->menu_cat_max>=MAX_MENU_CATEGORY){
    popup_message(mascot->win_main,
#ifdef USE_GTK3
		  "dialog-error", 
#else
		  GTK_STOCK_DIALOG_ERROR,
#endif
		  -1,
		  _("Error: Merge Menu."),
		  " ",
		  _("The number of categories maxes out."),
		  NULL);
    return;
  }

  old_cat_max=mascot->menu_cat_max;

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  while (my_main_iteration(FALSE));

  fdialog = gtk_file_chooser_dialog_new(_("Select Menu File to Merge"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_OPEN,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Open", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				

  gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(fdialog),TRUE);
  
  gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (fdialog), 
	       to_utf8(FullPathMascotFile(mascot, mascot->menu_file)));
  gtk_file_chooser_select_filename (GTK_FILE_CHOOSER (fdialog), 
	       to_utf8(FullPathMascotFile(mascot, mascot->menu_file)));

  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_DEFAULT);
  my_file_chooser_add_filter(fdialog,_("Menu File"),MENU_EXTENSION);
  my_file_chooser_add_filter(fdialog,_("All File"),"*");


  gtk_widget_show_all(fdialog);

  if (gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {
    update=FALSE;
    
    fnames = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(fdialog));
    gtk_widget_destroy(fdialog);

    i_menu=0;
    while((gchar *)g_slist_nth_data(fnames,i_menu)){
      dest_file=to_locale((gchar *)g_slist_nth_data(fnames,i_menu));
      if(access(dest_file,F_OK)==0){
	merge_file=g_strdup(dest_file);
	
	gtk_widget_destroy(mascot->PopupMenu);
	menu_cat_max_old=mascot->menu_cat_max;
	ReadMenu(mascot,mascot->menu_cat_max,merge_file);
	if(mascot->menu_cat_max>MAX_MENU_CATEGORY){
	  mascot->menu_cat_max=menu_cat_max_old;
	}
	
	update=TRUE;
      }
      else{
#ifdef GTK_MSG
	popup_message(mascot->win_main,
#ifdef USE_GTK3
		      "dialog-error", 
#else
		      GTK_STOCK_DIALOG_ERROR,
#endif
		      -1,
		      _("Error: File cannot be opened."),
		      " ",
		      (gchar *)g_slist_nth_data(fnames,i_menu),
		      NULL);
#else
	g_print(_("Cannot Open %s\n"),
		(gchar *)g_slist_nth_data(fnames,i_menu));
#endif
      }
      
      if(mascot->menu_cat_max>=MAX_MENU_CATEGORY) break;
      i_menu++;
    }
    if(update){
      mascot->PopupMenu=make_popup_menu(mascot);

      for(i_cat=old_cat_max-1;i_cat<mascot->menu_cat_max;i_cat++){
	make_cat_list(mascot, cat_note,i_cat);
      }
      gtk_notebook_set_current_page(GTK_NOTEBOOK(cat_note),mascot->menu_cat_max-1);
      gtk_widget_queue_draw(GTK_WIDGET(cat_note));
    }
    
    g_slist_free(fnames);
  }
  else {
    gtk_widget_destroy(fdialog);
  }

  if(dest_file) g_free(dest_file);
  flagChildDialog=FALSE;
}




static void create_change_duet_tgt_dialog(GtkWidget *w, gpointer gdata)
{
  confNum *mptn;
  typMascot *mascot;
  int i_ptn;
  GtkWidget *fdialog;
  gchar *fname=NULL;
  gchar *dest_file=NULL;

  mptn=(confNum *)gdata;
  i_ptn=mptn->num;
  mascot=mptn->mascot;

  if(flagChildDialog){
    return;
  }
  else{
    flagChildDialog=TRUE;
  }

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  //while (my_main_iteration(FALSE));

  fdialog = gtk_file_chooser_dialog_new(_("Select Mascot File"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_OPEN,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Open", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
#endif
					NULL);

  my_file_chooser_set_current_folder(fdialog, FOLDER_DEFAULT); 

  if(mascot->duet_tgt[i_ptn]){
    gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (fdialog), 
				     to_utf8(FullPathMascotFile(mascot,
								mascot->duet_tgt[i_ptn])));
    gtk_file_chooser_select_filename (GTK_FILE_CHOOSER (fdialog), 
				      to_utf8(FullPathMascotFile(mascot,
								 mascot->duet_tgt[i_ptn])));
  }

  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_DEFAULT);
  my_file_chooser_add_filter(fdialog,_("Mascot File"),MASCOT_EXTENSION);
  my_file_chooser_add_filter(fdialog,_("All File"),"*");


  gtk_widget_show_all(fdialog);

  if (gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {
    fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fdialog));
    gtk_widget_destroy(fdialog);
    
    dest_file=to_locale(fname);
    if(fname) g_free(fname);

    if(access(dest_file,F_OK)==0){
      if(mascot->duet_tgt[i_ptn]); g_free(mascot->duet_tgt[i_ptn]);
      mascot->duet_tgt[i_ptn]=g_strdup(my_basename(FullPathMascotFile(mascot, dest_file)));
      
      gtk_entry_set_text(GTK_ENTRY(duet_tgt_entry[i_ptn]),
			 mascot->duet_tgt[i_ptn]);
    }
    else{
      popup_message(Mascot->win_main,
#ifdef USE_GTK3
		    "dialog-error", 
#else
		    GTK_STOCK_DIALOG_ERROR,
#endif
		    -1,
		    _("Error: File cannot be opened."),
		    " ",
		    dest_file,
		    NULL);
    }
  }
  else {
    gtk_widget_destroy(fdialog);
  }

  if(dest_file) g_free(dest_file);
  flagChildDialog=FALSE;
}


static void create_change_click_sound_dialog(GtkWidget *w, gpointer gdata)
{
  confNum *mptn;
  typMascot *mascot;
  int i_ptn;
  GtkWidget *fdialog;
  gchar *fname=NULL;
  gchar *dest_file=NULL;

  mptn=(confNum *)gdata;

  i_ptn=mptn->num;
  mascot=mptn->mascot;

  if(flagChildDialog){
    return;
  }
  else{
    flagChildDialog=TRUE;
  }

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  while (my_main_iteration(FALSE));

  fdialog = gtk_file_chooser_dialog_new(_("Select Sound File"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_OPEN,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Open", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				
	     
  if(mascot->click_sound[i_ptn]){
    gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (fdialog), 
	     FullPathSoundFile(mascot,
			       to_utf8(mascot->click_sound[i_ptn],FALSE)));
    gtk_file_chooser_select_filename (GTK_FILE_CHOOSER (fdialog), 
	     FullPathSoundFile(mascot,
			       to_utf8(mascot->click_sound[i_ptn],FALSE)));
  }
  else{
    my_file_chooser_set_current_folder(fdialog, FOLDER_SOUND); 
  }

  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_SOUND);
  my_file_chooser_add_filter(fdialog,_("All File"),"*");


  gtk_widget_show_all(fdialog);

  if (gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {
    fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fdialog));
    gtk_widget_destroy(fdialog);
    
    dest_file=to_locale(fname);
    if(fname) g_free(fname);

    if(access(dest_file,F_OK)==0){
      if(mascot->click_sound[i_ptn]) g_free(mascot->click_sound[i_ptn]);
      mascot->click_sound[i_ptn]=g_strdup(FullPathSoundFile(mascot,dest_file,FALSE));

      gtk_entry_set_text(GTK_ENTRY(click_sound_entry[i_ptn]),
			 my_basename(mascot->click_sound[i_ptn]));
    }
    else{
      popup_message(mascot->win_main,
#ifdef USE_GTK3
		    "dialog-error", 
#else
		    GTK_STOCK_DIALOG_ERROR,
#endif
		    -1,
		    _("Error: File cannot be opened."),
		    " ",
		    dest_file,
		    NULL);
    }
  }
  else {
    gtk_widget_destroy(fdialog);
  }

  if(dest_file) g_free(dest_file);
  flagChildDialog=FALSE;
}


#ifdef USE_BIFF
static void create_change_biff_sound_dialog(GtkWidget *w, gpointer gdata)
{
  typMascot *mascot;
  GtkWidget *fdialog;
  GtkWidget *snd_entry;
  gchar *fname=NULL;
  gchar *dest_file=NULL;

  mascot=(typMascot *)gdata;
    
  if(flagChildDialog){
    return;
  }
  else{
    flagChildDialog=TRUE;
  }

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  while (my_main_iteration(FALSE));

  fdialog = gtk_file_chooser_dialog_new(_("Select Sound File"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_OPEN,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Open", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				

  if(mascot->mail.sound){
    gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (fdialog), 
		     to_utf8(FullPathSoundFile(mascot,
					       mascot->mail.sound,FALSE)));
    gtk_file_chooser_select_filename (GTK_FILE_CHOOSER (fdialog), 
				      to_utf8(FullPathSoundFile(mascot,
								mascot->mail.sound,FALSE)));
  }
  else{
    my_file_chooser_set_current_folder(fdialog, FOLDER_SOUND); 
  }

  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_SOUND);
  my_file_chooser_add_filter(fdialog,_("All File"),"*");

  gtk_widget_show_all(fdialog);

  if (gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {
    fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fdialog));
    gtk_widget_destroy(fdialog);

    dest_file=to_locale(fname);
    if(fname) g_free(fname);
    
    if(access(dest_file,F_OK)==0){
      if(mascot->mail.sound) g_free(mascot->mail.sound);
      mascot->mail.sound=g_strdup(FullPathSoundFile(mascot,dest_file,FALSE));
      gtk_entry_set_text(GTK_ENTRY(entry_mail_sound),
			 my_basename(mascot->mail.sound));
    }
    else{
      popup_message(mascot->win_main,
#ifdef USE_GTK3
		    "dialog-error", 
#else
		    GTK_STOCK_DIALOG_ERROR,
#endif
		    -1,
		    _("Error: File cannot be opened."),
		    " ",
		    dest_file,
		    NULL);
    }
  }
  else {
    gtk_widget_destroy(fdialog);
  }

  if(dest_file) g_free(dest_file);
  flagChildDialog=FALSE;
}

static void create_mail_file_dialog(GtkWidget *w, gpointer gdata)
{
  typMascot *mascot;
  GtkWidget *fdialog;
  gchar *dir;
  gchar *fname=NULL;
  gchar *dest_file=NULL;
    
  mascot=(typMascot *)gdata;
  
  if(flagChildDialog){
    return;
  }
  else{
    switch(mascot->mail.type){
    case MAIL_POP3:
    case MAIL_APOP:
      return;
      break;
    }
    flagChildDialog=TRUE;
  }


  // Win構築は重いので先にExposeイベント等をすべて処理してから
  while (my_main_iteration(FALSE));

  fdialog = gtk_file_chooser_dialog_new(_("Select the file refered for the mail checking"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_OPEN,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Open", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				

  if(mascot->mail.file){
    if(access(mascot->mail.file,F_OK)==0){
      gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (fdialog), 
				     to_utf8(mascot->mail.file));
      gtk_file_chooser_select_filename (GTK_FILE_CHOOSER (fdialog), 
					to_utf8(mascot->mail.file));
    }
  }
  else{
    switch(mascot->mail.type){
    case MAIL_LOCAL:
      gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (fdialog), 
				     g_getenv("MAIL"));
      gtk_file_chooser_select_filename (GTK_FILE_CHOOSER (fdialog), 
				     g_getenv("MAIL"));
      break;
    case MAIL_QMAIL:
      gtk_file_chooser_set_action   (GTK_FILE_CHOOSER (fdialog), 
				     GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
      dir=g_strconcat(
#ifdef USE_WIN32
		      get_win_home(),
#else
		      g_get_home_dir(),
#endif
		      G_DIR_SEPARATOR_S, 
		      "Maildir", 
		      G_DIR_SEPARATOR_S, 
		      "new",
		      NULL);
      if(access(dir,F_OK)==0){
	gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (fdialog), 
				       dir);
	gtk_file_chooser_select_filename (GTK_FILE_CHOOSER (fdialog), 
				       dir);
      }
      break;
    case MAIL_PROCMAIL:
      gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (fdialog), 
				       g_strconcat(set_mhdir(),PROCMAIL_LOG,NULL));
      gtk_file_chooser_select_filename (GTK_FILE_CHOOSER (fdialog), 
				       g_strconcat(set_mhdir(),PROCMAIL_LOG,NULL));
      break;
    }
  }

  gtk_widget_show_all(fdialog);

  if (gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {
    fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fdialog));
    gtk_widget_destroy(fdialog);
    
    dest_file=to_locale(fname);
    if(fname) g_free(fname);

    if(access(dest_file,F_OK)==0){
      if(mascot->mail.file) g_free(mascot->mail.file);
      mascot->mail.file=g_strdup(dest_file);

      gtk_entry_set_text(GTK_ENTRY(entry_mail_file),
			 mascot->mail.file);
    }
    else{
      popup_message(mascot->win_main,
#ifdef USE_GTK3
		    "dialog-error", 
#else
		    GTK_STOCK_DIALOG_ERROR,
#endif
		    -1,
		    _("Error: File cannot be opened."),
		    " ",
		    dest_file,
		    NULL);
    }
  }
  else {
    gtk_widget_destroy(fdialog);
  }

  if(dest_file) g_free(dest_file);
  flagChildDialog=FALSE;
}

static void create_mailer_dialog(GtkWidget *w, gpointer gdata)
{
  typMascot *mascot;
  GtkWidget *fdialog;
  gchar *dir;
  gchar *fname=NULL;
  gchar *dest_file=NULL;

  mascot=(typMascot *)gdata;

  if(flagChildDialog){
    return;
  }
  else{
    flagChildDialog=TRUE;
  }

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  while (my_main_iteration(FALSE));
  
  fdialog = gtk_file_chooser_dialog_new(_("Select mailer command"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_OPEN,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Open", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				

  if(mascot->mail.mailer){
    if(access(mascot->mail.mailer,F_OK)==0){
      gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (fdialog), 
				     to_utf8(mascot->mail.mailer));
      gtk_file_chooser_select_filename (GTK_FILE_CHOOSER (fdialog), 
					to_utf8(mascot->mail.mailer));
    }
  }
#ifdef USE_WIN32
  my_file_chooser_add_filter(fdialog,_("All File"),"*");
  my_file_chooser_add_filter(fdialog,"EXE","*.exe");
#endif

  gtk_widget_show_all(fdialog);

  if (gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {
    fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fdialog));
    gtk_widget_destroy(fdialog);
    
    dest_file=to_locale(fname);
    if(fname) g_free(fname);
    
    if(access(dest_file,F_OK)==0){
      if(mascot->mail.mailer) g_free(mascot->mail.mailer);
      mascot->mail.mailer=g_strdup(dest_file);

      gtk_entry_set_text(GTK_ENTRY(entry_mail_mailer),
			 mascot->mail.mailer);
    }
    else{
      popup_message(mascot->win_main,
#ifdef USE_GTK3
		    "dialog-error", 
#else
		    GTK_STOCK_DIALOG_ERROR,
#endif
		    -1,
		    _("Error: File cannot be opened."),
		    " ",
		    dest_file,
		    NULL);
    }
    
  }
  else {
    gtk_widget_destroy(fdialog);
  }

  if(dest_file) g_free(dest_file);
  flagChildDialog=FALSE;
}

#endif  // USE_BIFF


static void MakeNewMascot(gchar *mascot_file, gchar *image_file, gchar *copyright){
  int i_pix;
  gchar *tmp_file;
  gboolean tmp_flag;
  gchar *buf;
  
  InitMascot(Mascot);
  
  Mascot->file=mascot_file;
  if(strcmp(my_dirname(mascot_file),my_dirname(image_file))){
#ifdef USE_WIN32
    tmp_file=g_strconcat(get_win_home(), G_DIR_SEPARATOR_S,USER_DIR, PIXDIR, NULL);
#else
    tmp_file=g_strconcat(g_get_home_dir(), G_DIR_SEPARATOR_S,USER_DIR, PIXDIR, NULL);
#endif
    
    if(!strcmp(my_dirname(mascot_file),tmp_file)){
#ifdef USE_WIN32
      tmp_file=g_strconcat(get_win_home(), G_DIR_SEPARATOR_S,USER_DIR,
			   PIXDIR, my_basename(image_file), NULL);
#else
      tmp_file=g_strconcat(g_get_home_dir(), G_DIR_SEPARATOR_S,USER_DIR,
			   PIXDIR, my_basename(image_file), NULL);
#endif
    }
    else{
      tmp_file=g_strconcat(my_dirname(mascot_file), G_DIR_SEPARATOR_S,
			   my_basename(image_file), NULL);
    }
    
    
    g_print(_("Installing %s -> %s\n"),image_file,tmp_file);
    copy_file(image_file,tmp_file);
    Mascot->sprites[0].filename=tmp_file;
  }
  else{
    Mascot->sprites[0].filename=image_file;
    
  }
  
  if(copyright){
    Mascot->copyright=copyright;
  }
  
  Mascot->random_total=0;
  Mascot->click_total=0;
  
  Mascot->name=NULL;
  Mascot->move=MOVE_FIX;
  Mascot->xoff=0;
  Mascot->yoff=0;
  Mascot->x=0;
  Mascot->y=0;

  Mascot->clk_x=INIT_CLK_POS;
  Mascot->clk_y=INIT_CLK_POS;
  Mascot->clktext_x=INIT_CLK_TEXT;
  Mascot->clktext_y=INIT_CLK_TEXT;
  Mascot->clksd_x=INIT_CLK_SD;
  Mascot->clksd_y=INIT_CLK_SD;
  Mascot->wclkbd=INIT_CLK_BORDER;
  Mascot->clkmode=CLOCK_NO;
  Mascot->clktype=CLOCK_TYPE_24S;
  Mascot->flag_clksd=TRUE;
  Mascot->flag_clkrd=TRUE;
#ifdef FG_DRAW
  Mascot->flag_clkfg=TRUE;
#endif
  Mascot->fontname_clk=Mascot->deffontname_clk;
  
  Mascot->baltext_x=INIT_BAL_TEXT;
  Mascot->baltext_y=INIT_BAL_TEXT;
  Mascot->wbalbd=INIT_BAL_BORDER;
  Mascot->bal_defpos=BAL_POS_LEFT;
  Mascot->fontname_bal=Mascot->deffontname_bal;
#ifdef FG_DRAW
  Mascot->flag_balfg=TRUE;
#endif
  
#ifdef USE_GTK3  
  Mascot->colclk=gdk_rgba_copy(Mascot->def_colclk);
  Mascot->colclksd=gdk_rgba_copy(Mascot->def_colclksd);
  Mascot->colclkbg=gdk_rgba_copy(Mascot->def_colclkbg);
  Mascot->colclkbd=gdk_rgba_copy(Mascot->def_colclkbd);
  
  Mascot->colbal=gdk_rgba_copy(Mascot->def_colbal);
  Mascot->colbalbg=gdk_rgba_copy(Mascot->def_colbalbg);
  Mascot->colbalbd=gdk_rgba_copy(Mascot->def_colbalbd);
#else
  Mascot->colclk=gdk_color_copy(Mascot->def_colclk);
  Mascot->colclksd=gdk_color_copy(Mascot->def_colclksd);
  Mascot->colclkbg=gdk_color_copy(Mascot->def_colclkbg);
  Mascot->colclkbd=gdk_color_copy(Mascot->def_colclkbd);

  Mascot->colbal=gdk_color_copy(Mascot->def_colbal);
  Mascot->colbalbg=gdk_color_copy(Mascot->def_colbalbg);
  Mascot->colbalbd=gdk_color_copy(Mascot->def_colbalbd);
#endif
  
#ifdef USE_BIFF
  //// Biff
  Mascot->mail.pix_file=NULL;
  Mascot->mail.pix_pos=MAIL_PIX_RIGHT;
  Mascot->mail.pix_x=0;
  Mascot->mail.pix_y=0;
  Mascot->mail.word=NULL;
  Mascot->mail.sound=NULL;
#endif  // USE_BIFF

  Mascot->alpha_main=100;
#ifdef USE_BIFF
  Mascot->alpha_biff=100;
#endif
#ifdef USE_WIN32
  Mascot->alpha_bal=100;
  Mascot->alpha_clk=100;
#endif  
  
  for(i_pix=1;i_pix<MAX_PIXMAP;i_pix++){
    if(Mascot->sprites[i_pix].filename) g_free(Mascot->sprites[i_pix].filename);
    Mascot->sprites[i_pix].filename=NULL;
  }

  Mascot->frame_num[0]=1;
  Mascot->frame_pix[0][0]=0;
  Mascot->frame_min[0][0]=1;
  Mascot->frame_max[0][0]=100;
  
  SaveMascot(Mascot,FALSE);

  tmp_flag=Mascot->flag_install;
  Mascot->flag_install=FALSE;
  ReadMascot(Mascot,FALSE);
  InitComposite(Mascot);
  LoadPixmaps(Mascot);
#ifndef USE_GTK3
  ReInitGC(Mascot);
#endif
  map_balloon(Mascot, FALSE);
  flag_balloon=FALSE;
#ifdef USE_BIFF
  LoadBiffPixmap(Mascot->biff_pix, Mascot);
#endif  // USE_BIFF

 
  if(Mascot->clkmode!=CLOCK_NO) clock_update(Mascot, TRUE);

  if(Mascot->clkmode==CLOCK_PANEL){
    map_clock(Mascot, TRUE);
  }
  else{
    map_clock(Mascot, FALSE);
  }
  
  map_main(Mascot, TRUE);
  
  if(Mascot->clkmode!=CLOCK_NO) clock_update(Mascot, TRUE);
  
  if(Mascot->move==MOVE_FIX){
  MoveMascot(Mascot,Mascot->xfix,Mascot->yfix);
  }
  else{
    MoveToFocus(Mascot,TRUE);
  }
  
  
  Mascot->flag_install=tmp_flag;
  buf=g_strdup_printf(_("Created New Mascot ...%%n    \"%s\""), Mascot->file);
  DoSysBalloon(Mascot,buf);
  g_free(buf);

  return;
}
 

gchar * create_new_mascot_file_selection_dialog()
 {
  GtkWidget *fdialog;
  gchar *mascot_file=NULL;
  gchar *fname=NULL;
  gchar *dest_file=NULL;
    

  if(flagChildDialog){
    return(NULL);
  }
  else{
    flagChildDialog=TRUE;
  }

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  while (my_main_iteration(FALSE));
  
  fdialog = gtk_file_chooser_dialog_new(_("Select New Mascot File"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_SAVE,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Save", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				

  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_CURRENT);
  my_file_chooser_add_filter(fdialog, _("Mascot File"), MASCOT_EXTENSION);
  my_file_chooser_add_filter(fdialog, _("All File"), "*");


  gtk_widget_show_all(fdialog);

  if (gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {
    fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fdialog));
    gtk_widget_destroy(fdialog);

    dest_file=to_locale(fname);
    if(fname) g_free(fname);
    
    if(access(my_dirname(dest_file),W_OK)==0){
      mascot_file=g_strdup(dest_file);
    }
    else{
      popup_message(Mascot->win_main,
#ifdef USE_GTK3
		    "dialog-error", 
#else
		    GTK_STOCK_DIALOG_ERROR,
#endif
		    -1,
		    _("Error: File cannot be opened."),
		    " ",
		    dest_file,
		    NULL);
    }
  }
  else {
    gtk_widget_destroy(fdialog);
  }

  if(dest_file) g_free(dest_file);
  flagChildDialog=FALSE;
  return(mascot_file);
}


gchar * create_new_mascot_image_selection_dialog(gchar *mascot_file){
  GtkWidget *fdialog;
  gchar *image_file=NULL;
  gchar *fname=NULL;
  gchar *dest_file=NULL;
    

  if(flagChildDialog){
    return(NULL);
  }
  else{
    flagChildDialog=TRUE;
  }

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  //while (my_main_iteration(FALSE));

  fdialog = gtk_file_chooser_dialog_new(_("Select Image for New Mascot"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_OPEN,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Open", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				

  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_PIX);
  my_file_chooser_add_filter(fdialog,_("All File"),"*");
  my_file_chooser_add_filter(fdialog,"PNG",PNG_EXTENSION);
  my_file_chooser_add_filter(fdialog,"GIF",GIF_EXTENSION);
  my_file_chooser_add_filter(fdialog,"XPM",XPM_EXTENSION);

  gtk_file_chooser_add_shortcut_folder(GTK_FILE_CHOOSER(fdialog),
				       to_utf8(my_dirname(mascot_file)),
				       NULL);

  gtk_widget_show_all(fdialog);

  if (gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {
    fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fdialog));
    gtk_widget_destroy(fdialog);
    
    dest_file=to_locale(fname);
    if(fname) g_free(fname);

    if(access(dest_file,F_OK)==0){
      image_file=g_strdup(dest_file);
    }
    else{
      popup_message(Mascot->win_main,
#ifdef USE_GTK3
		    "dialog-error", 
#else
		    GTK_STOCK_DIALOG_ERROR,
#endif
		    -1,
		    _("Error: File cannot be opened."),
		    " ",
		    dest_file,
		    NULL);
    }
  }
  else {
    gtk_widget_destroy(fdialog);
  }

  if(dest_file) g_free(dest_file);
  flagChildDialog=FALSE;
  return(image_file);
}


static void create_new_mascot_dialog(GtkWidget *w, gpointer gdata)
{
  GtkWidget *dialog;
  GtkWidget *button_cancel;
  GtkWidget *button_ok;
  GtkWidget *table;
  GtkWidget *hbox;
  GtkWidget *label;
  GtkWidget *entry;
  gchar *mascot_file;
  gchar *image_file;
  gchar *copyright;
  
  

  Mascot->flag_menu=TRUE;

  mascot_file=NULL;
  image_file=NULL;
  copyright=NULL;

  dialog = gtk_dialog_new_with_buttons(_("Create New Mascot"),
				       NULL,
				       GTK_DIALOG_MODAL,
#ifdef USE_GTK3
				       "_Cancel",GTK_RESPONSE_CANCEL,
				       "_OK",GTK_RESPONSE_OK,
#else
				       GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
				       GTK_STOCK_OK,GTK_RESPONSE_OK,
#endif
				       NULL);
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(dialog),5);
  gtk_window_set_title(GTK_WINDOW(dialog),_("Create New Mascot"));
  
  label=gtkut_label_new(_("Creating New Mascot..."));
  gtkut_pos(label, POS_START, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     label,TRUE,TRUE,0);

  label=gtkut_label_new("");
  gtkut_pos(label, POS_START, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     label,TRUE,TRUE,0);

  hbox=gtkut_hbox_new(FALSE,0);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     hbox,TRUE,TRUE,0);
  label=gtkut_label_new(_("1. Please select Mascot File Name to be created"));
  gtkut_pos(label, POS_CENTER, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,TRUE,0);
   
  gtk_widget_show_all(dialog);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
    gtk_widget_destroy(dialog);
    
    mascot_file=create_new_mascot_file_selection_dialog();
    if(!mascot_file){
      Mascot->flag_menu=FALSE;
      return;
    }
  }
  else{
    // Mascot File選択 : Cancelが押されていたら終了
    gtk_widget_destroy(dialog);
    Mascot->flag_menu=FALSE;
    return;
  }

  dialog = gtk_dialog_new_with_buttons(_("Create New Mascot"),
				       NULL,
				       GTK_DIALOG_MODAL,
#ifdef USE_GTK3
				       "_Cancel",GTK_RESPONSE_CANCEL,
				       "_OK",GTK_RESPONSE_OK,
#else
				       GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
				       GTK_STOCK_OK,GTK_RESPONSE_OK,
#endif
				       NULL);
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(dialog),5);
  gtk_window_set_title(GTK_WINDOW(dialog),_("Create New Mascot"));
  
  label=gtkut_label_new(_("Creating New Mascot..."));
  gtkut_pos(label, POS_START, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     label,TRUE,TRUE,0);

  label=gtkut_label_new("");
  gtkut_pos(label, POS_START, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     label,TRUE,TRUE,0);

  hbox=gtkut_hbox_new(FALSE,0);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     hbox,TRUE,TRUE,0);
  label=gtkut_label_new(_("2. Please select an Image File Name to be used in Mascot"));
  gtkut_pos(label, POS_CENTER, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,TRUE,0);

  gtk_widget_show_all(dialog);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
    gtk_widget_destroy(dialog);

    image_file=create_new_mascot_image_selection_dialog(mascot_file);
    if(!image_file){
      if(mascot_file) g_free(mascot_file);
      Mascot->flag_menu=FALSE;
      return;
    }
  }
  else{
    // Mascot Image選択 : Cancelが押されていたら終了
    if(mascot_file) g_free(mascot_file);
    gtk_widget_destroy(dialog);
    Mascot->flag_menu=FALSE;
    return;
  }

  dialog = gtk_dialog_new_with_buttons(_("Create New Mascot"),
				       NULL,
				       GTK_DIALOG_MODAL,
#ifdef USE_GTK3
				       "_Cancel",GTK_RESPONSE_CANCEL,
				       "_OK",GTK_RESPONSE_OK,
#else
				       GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
				       GTK_STOCK_OK,GTK_RESPONSE_OK,
#endif
				       NULL);
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(dialog),5);
  gtk_window_set_title(GTK_WINDOW(dialog),_("Create New Mascot"));
  
  label=gtkut_label_new(_("Creating New Mascot..."));
  gtkut_pos(label, POS_START, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     label,TRUE,TRUE,0);

  label=gtkut_label_new("");
  gtkut_pos(label, POS_START, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     label,TRUE,TRUE,0);

  table=gtkut_table_new(2,6,FALSE, 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     table,TRUE,TRUE,0);
  label=gtkut_label_new(_("Mascot File : "));
  gtkut_pos(label, POS_START, POS_CENTER);
  gtkut_table_attach_defaults (table, label, 0, 1, 0, 1);
  label=gtkut_label_new(mascot_file);
  gtkut_pos(label, POS_START, POS_CENTER);
  gtkut_table_attach_defaults (table, label, 1, 2, 0, 1);
  label=gtkut_label_new(_("Image File : "));
  gtkut_pos(label, POS_START, POS_CENTER);
  gtkut_table_attach_defaults (table, label, 0, 1, 1, 2);
  label=gtkut_label_new(image_file);
  gtkut_pos(label, POS_START, POS_CENTER);
  gtkut_table_attach_defaults (table, label, 1, 2, 1, 2);
  label=gtkut_label_new("");
  gtkut_pos(label, POS_CENTER, POS_CENTER);
  gtkut_table_attach_defaults (table, label, 0, 2, 2, 3);
  label=gtkut_label_new(_("At last, Please Input Descrption for Copyright"));
  gtkut_pos(label, POS_CENTER, POS_CENTER);
  gtkut_table_attach_defaults (table, label, 0, 2, 3, 4);

  entry = gtk_entry_new ();
  gtk_entry_set_text(GTK_ENTRY(entry),"Copyright (C) ");
  label=gtkut_label_new(_("At last, Please Input Descrption for Copyright"));
  gtkut_pos(label, POS_CENTER, POS_CENTER);
  gtkut_table_attach_defaults (table, entry, 0, 2, 4, 5);
  my_signal_connect (entry,"changed",cc_get_entry,&copyright);

  gtk_widget_show_all(dialog);
  
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
    gtk_widget_destroy(dialog);

    MakeNewMascot(mascot_file, image_file, copyright);
  }
  else{
    gtk_widget_destroy(dialog);
  }

  if(mascot_file) g_free(mascot_file);
  if(image_file) g_free(image_file);
  if(copyright) g_free(copyright);

  Mascot->flag_menu=FALSE;
  return;
}


void create_pop_pass_dialog(void)
{
  GtkWidget *dialog;
  GtkWidget *button_cancel;
  GtkWidget *button_ok;
  GtkWidget *hbox;
  GtkWidget *label;
  GtkWidget *entry;
  gchar *tmp;

  Mascot->flag_menu=TRUE;

  dialog = gtk_dialog_new_with_buttons(_("Please Input POP Password"),
				       NULL,
				       GTK_DIALOG_MODAL,
#ifdef USE_GTK3
				       "_Cancel",GTK_RESPONSE_CANCEL,
				       "_OK",GTK_RESPONSE_OK,
#else
				       GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
				       GTK_STOCK_OK,GTK_RESPONSE_OK,
#endif
				       NULL);
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(dialog),5);
  
  label=gtkut_label_new(_("Please Input POP Password"));
  gtkut_pos(label, POS_START, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     label,TRUE,TRUE,0);

  label=gtkut_label_new("");
  gtkut_pos(label, POS_START, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     label,TRUE,TRUE,0);

  if(Mascot->mail.pop_server!=NULL){
    tmp=g_strdup_printf(_("Server : %s"),Mascot->mail.pop_server);
    label=gtkut_label_new(tmp);
    g_free(tmp);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		       label,TRUE,TRUE,0);
  }
  if(Mascot->mail.pop_id!=NULL){
    tmp=g_strdup_printf(_("User ID : %s"),Mascot->mail.pop_id);
    label=gtkut_label_new(tmp);
    g_free(tmp);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		       label,TRUE,TRUE,0);
  }

  label=gtkut_label_new("");
  gtkut_pos(label, POS_START, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     label,TRUE,TRUE,0);

  hbox=gtkut_hbox_new(FALSE,0);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     hbox,TRUE,TRUE,0);
  label=gtkut_label_new(_("Password : "));
  gtkut_pos(label, POS_CENTER, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,TRUE,0);
  
  entry = gtk_entry_new ();
  gtk_box_pack_start(GTK_BOX(hbox),entry,TRUE,TRUE,0);
  gtk_entry_set_visibility(GTK_ENTRY(entry), FALSE);
  if(Mascot->mail.pop_pass!=NULL)
    gtk_entry_set_text(GTK_ENTRY(entry),Mascot->mail.pop_pass);

  gtk_widget_show_all(dialog);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
    if(Mascot->mail.pop_pass) g_free(Mascot->mail.pop_pass);
    Mascot->mail.pop_pass=
      g_strdup(gtk_entry_get_text(GTK_ENTRY(entry)));
    
    gtk_widget_destroy(dialog);
  }
  else{
    gtk_widget_destroy(dialog);
  }

  Mascot->flag_menu=FALSE;
  return;
}


static void create_file_selection_dialog(GtkWidget *widget, gint gdata)
{
  GtkWidget *fdialog;
  gint  mode;
  gchar *fname=NULL;
  gboolean flag_install;
  gchar *dest_file=NULL;

    
  
  mode=(gint) gdata;

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  while (my_main_iteration(FALSE));


  Mascot->flag_menu=TRUE;
  

  switch(mode){
  case MENU_SELECT:
  case START_MENU_SELECT:
    fdialog = gtk_file_chooser_dialog_new(_("Open Mascot"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_OPEN,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Open", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				
;
    break;
  case MENU_INSTALL_USER:
  case START_MENU_INSTALL_USER:
    popup_message(Mascot->win_main,
#ifdef USE_GTK3
		  "dialog-warning", 
#else
		  GTK_STOCK_DIALOG_WARNING,
#endif
		  POPUP_TIMEOUT*3,
		  _("Caution : You're going to install a new mascot into your user's direcroty.\n"),
		  _("    If you already have the file with the same filename,"),
		  _("    it will be overwritten."),
		  _("\n    If you want to install more than one mascot with a launcher menu,"),
		  _("    please install using \"Install Launcher Menu\"."),
		  NULL);
    fdialog = gtk_file_chooser_dialog_new(_("Install New Mascot to User's Directory"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_OPEN,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Open", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				

    break;
#ifdef USE_COMMON
  case MENU_INSTALL_COMMON:
  case START_MENU_INSTALL_COMMON:
    popup_message(Mascot->win_main,
#ifdef USE_GTK3
		  "dialog-warning", 
#else
		  GTK_STOCK_DIALOG_WARNING,
#endif
		  POPUP_TIMEOUT*3,
		  _("Caution : You're going to install a new mascot into the MaCoPiX common direcroty.\n"),
		  _("    If you already have the file with the same filename,"),
		  _("    it will be overwritten."),
		  _("\n    If you want to install more than one mascot with a launcher menu,"),
		  _("    please install using \"Install Launcher Menu\"."),
		  NULL);
    fdialog = gtk_file_chooser_dialog_new(_("Install New Mascot to the Common Directory"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_OPEN,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Open", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				

#endif
  }

  if(mode==MENU_SELECT){
    if(Mascot->file){
      gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (fdialog), 
				     to_utf8(Mascot->file));
      gtk_file_chooser_select_filename (GTK_FILE_CHOOSER (fdialog), 
					to_utf8(Mascot->file));
    }
  }


  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_DEFAULT);
  my_file_chooser_add_filter(fdialog,_("Mascot File"),MASCOT_EXTENSION);
  my_file_chooser_add_filter(fdialog,_("All File"),"*");


  gtk_widget_show_all(fdialog);

  if (gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {
    fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fdialog));
    gtk_widget_destroy(fdialog);

    dest_file = to_locale(fname);
    if(fname) g_free(fname);
    
    flag_install=Mascot->flag_install;
    
    if(access(dest_file,F_OK)==0){
      Mascot->file=g_strdup(dest_file);
      
      switch(mode){
      case MENU_INSTALL_USER:
      case START_MENU_INSTALL_USER:
	Mascot->flag_install=TRUE;
	Mascot->flag_common=FALSE;
	Mascot->flag_ow=TRUE;
	Mascot->flag_ow_ini=TRUE;
	break;
      case MENU_INSTALL_COMMON:
      case START_MENU_INSTALL_COMMON:
	Mascot->flag_install=TRUE;
	Mascot->flag_common=TRUE;
	Mascot->flag_ow=TRUE;
	Mascot->flag_ow_ini=TRUE;
      }
      
      switch(mode){
      case MENU_SELECT:
      case MENU_INSTALL_USER:
      case MENU_INSTALL_COMMON:
	ChangeMascot();
      }
    }
    else{
      popup_message(Mascot->win_main,
#ifdef USE_GTK3
		    "dialog-error", 
#else
		    GTK_STOCK_DIALOG_ERROR,
#endif
		    -1,
		    _("Error: File cannot be opened."),
		    " ",
		    dest_file,
		    NULL);
    }

    Mascot->flag_menu=FALSE;

    Mascot->flag_install=flag_install;
    Mascot->flag_common=FALSE;
    switch(mode){
    case MENU_INSTALL_USER:
    case MENU_INSTALL_COMMON:
      Mascot->flag_ow=FALSE;
      Mascot->flag_ow_ini=FALSE;
    }
  }
  else {
    gtk_widget_destroy(fdialog);
    Mascot->flag_menu=FALSE;
  }

  if(dest_file) g_free(dest_file);
}

  
static void create_menu_selection_dialog(GtkWidget *widget, gint gdata)
{
  GtkWidget *fdialog;
  gchar *filepath;
  gint  mode;
  gchar *fname=NULL;
  gboolean flag_install;
  gchar *dest_file=NULL;
    
  mode=(gint) gdata;

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  //while (my_main_iteration(FALSE));

  Mascot->flag_menu=TRUE;
  
  switch(mode){
  case MENU_SELECT:
  case START_MENU_SELECT:
  fdialog = gtk_file_chooser_dialog_new(_("Open Launcher Menu"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_OPEN,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Open", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				
    break;
  case MENU_INSTALL_USER:
  case START_MENU_INSTALL_USER:
    popup_message(Mascot->win_main,
#ifdef USE_GTK3
		  "dialog-warning", 
#else
		  GTK_STOCK_DIALOG_WARNING,
#endif
		  POPUP_TIMEOUT*3,
		  _("Caution : You're going to install a new launcher menu with its mascots into your user's direcroty.\n"),
		  _("    If you already have the file with the same filename,"),
		  _("    it will be overwritten."),
		  NULL);
    fdialog = gtk_file_chooser_dialog_new(_("Install New Launcher Menu to User's Directory"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_OPEN,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Open", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				
    break;
#ifdef USE_COMMON
  case MENU_INSTALL_COMMON:
  case START_MENU_INSTALL_COMMON:
    popup_message(Mascot->win_main,
#ifdef USE_GTK3
		  "dialog-warning", 
#else
		  GTK_STOCK_DIALOG_WARNING,
#endif
		  POPUP_TIMEOUT*3,
		  _("Caution : You're going to install a new launcher menu with mascots into the MaCoPiX common direcroty.\n"),
		  _("    If you already have the file with the same filename,"),
		  _("    it will be overwritten."),
		  NULL);
    fdialog = gtk_file_chooser_dialog_new(_("Install New Launcher Menu to the Common Directory"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_OPEN,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Open", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				
#endif
  }


  if(mode==MENU_SELECT){
    if(Mascot->menu_file){
      gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (fdialog), 
				     to_utf8(Mascot->menu_file));
      gtk_file_chooser_select_filename (GTK_FILE_CHOOSER (fdialog), 
					to_utf8(Mascot->menu_file));
    }
    else{
      my_file_chooser_set_current_folder(fdialog, FOLDER_DEFAULT); 
    }
  }


  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_DEFAULT);

#ifdef USE_LHA32
  my_file_chooser_add_filter(fdialog,_("LZH File"),LZH_EXTENSION);
#endif
#if defined(USE_GTAR) || defined(USE_TAR32)
  my_file_chooser_add_filter(fdialog,_("TAR+GZ File"),TAR_EXTENSION);
#endif
  my_file_chooser_add_filter(fdialog,_("Menu File"),MENU_EXTENSION);

  my_file_chooser_add_filter(fdialog,_("All File"),"*");


  gtk_widget_show_all(fdialog);

  if (gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {
    fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fdialog));
    gtk_widget_destroy(fdialog);

    dest_file=to_locale(fname);
    if(fname) g_free(fname);
    
    flag_install=Mascot->flag_install;

    if(access(dest_file,F_OK)==0){
      switch(mode){
      case MENU_INSTALL_USER:
      case START_MENU_INSTALL_USER:
	select_menu_from_ext(Mascot,dest_file);
	Mascot->flag_install=TRUE;
	Mascot->flag_common=FALSE;
	Mascot->flag_ow=TRUE;
	Mascot->flag_ow_ini=TRUE;
	break;
      case MENU_INSTALL_COMMON:
      case START_MENU_INSTALL_COMMON:
	select_menu_from_ext(Mascot,dest_file);
	Mascot->flag_install=TRUE;
	Mascot->flag_common=TRUE;
	Mascot->flag_ow=TRUE;
	Mascot->flag_ow_ini=TRUE;
	break;
      default:
	Mascot->menu_file=g_strdup(dest_file);
      }
      gtk_widget_destroy(Mascot->PopupMenu);
      ReadMenu(Mascot,0,NULL);
      Mascot->PopupMenu=make_popup_menu(Mascot);
    }
    else{
      popup_message(Mascot->win_main,
#ifdef USE_GTK3
		    "dialog-error", 
#else
		    GTK_STOCK_DIALOG_ERROR,
#endif
		    -1,
		    _("Error: File cannot be opened."),
		    " ",
		    dest_file,
		    NULL);
    }

  
    Mascot->flag_menu=FALSE;

    Mascot->flag_install=flag_install;
    Mascot->flag_common=FALSE;
    switch(mode){
    case MENU_INSTALL_USER:
    case MENU_INSTALL_COMMON:
      Mascot->flag_ow=FALSE;
      Mascot->flag_ow_ini=FALSE;
    }
  }
  else {
    gtk_widget_destroy(fdialog);
    Mascot->flag_menu=FALSE;
  }

  if(dest_file) g_free(dest_file);
}


static void create_smenu_dialog2(void)
{
  // Win構築は重いので先にExposeイベント等をすべて処理してから
  while (my_main_iteration(FALSE));


  Mascot->flag_menu=TRUE;

  create_smenu_dialog(Mascot,TRUE);

  Mascot->flag_menu=FALSE;
}


static void create_nkr_selection_dialog(void)
{
  GtkWidget *fdialog;
  gchar *fname=NULL;
  gchar *dest_file=NULL;

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  //while (my_main_iteration(FALSE));


  fdialog = gtk_file_chooser_dialog_new(_("Convert from Nokkari-Chara"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_OPEN,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Open", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				

  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_DEFAULT);
  my_file_chooser_add_filter(fdialog,_("Nokkari-Chara File"),NKR_EXTENSION);
  my_file_chooser_add_filter(fdialog,_("All File"),"*");


  gtk_widget_show_all(fdialog);

  if (gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {
    fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fdialog));
    gtk_widget_destroy(fdialog);

    dest_file=to_locale(fname);
    if(fname) g_free(fname);
    
    if(access(dest_file,F_OK)==0){
      Mascot->inifile=g_strdup(dest_file);
      
      NkrChangeMascot();
    }
    else{
      popup_message(Mascot->win_main,
#ifdef USE_GTK3
		    "dialog-error", 
#else
		    GTK_STOCK_DIALOG_ERROR,
#endif
		    -1,
		    _("Error: File cannot be opened."),
		    " ",
		    dest_file,
		    NULL);
    }

    create_save_mascot_dialog();
  }
  else {
    gtk_widget_destroy(fdialog);
    Mascot->flag_menu=FALSE;
  }

  if(dest_file) g_free(dest_file);
}

  
static void create_new_menu_selection_dialog(void)
{
  GtkWidget *fdialog;
  gchar *fname=NULL;
  gchar *dest_file=NULL;
  FILE *fp_test;
  gchar *tmp;
    

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  while (my_main_iteration(FALSE));


  Mascot->flag_menu=TRUE;
  
  fdialog = gtk_file_chooser_dialog_new(_("Select New Launcher Menu File to Create"),					NULL,
					GTK_FILE_CHOOSER_ACTION_SAVE,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Save", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				

  my_file_chooser_set_current_folder(fdialog, FOLDER_DEFAULT); 
      
  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_DEFAULT);
  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_CURRENT);
  my_file_chooser_add_filter(fdialog,_("Menu File"),MENU_EXTENSION);
  my_file_chooser_add_filter(fdialog,_("All File"),"*");

  gtk_widget_show_all(fdialog);
  
  if (gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {
    fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fdialog));
    gtk_widget_destroy(fdialog);

    dest_file=to_locale(fname);
    if(fname) g_free(fname);

    if((fp_test=fopen(dest_file,"w"))!=NULL){
      fclose(fp_test);
      Mascot->menu_file=g_strdup(dest_file);

      Mascot->menu_cat[0]=g_strdup(TMP_CATEGORY_NAME);
      Mascot->menu_tgt_max[0]=1;
      
      Mascot->menu_tgt[0][0]
	=g_strdup(my_basename(Mascot->file));
      Mascot->menu_tgt_name[0][0]=ReadMascotName(Mascot, Mascot->file);
      
      Mascot->menu_cat_max=1;
      Mascot->menu_total=1;
      
      SaveMenu(Mascot);

      tmp=g_strdup_printf(_("Created Menu File%%n    \"%s\""),Mascot->menu_file);
      DoSysBalloon(Mascot,tmp);
      g_free(tmp);

      gtk_widget_destroy(Mascot->PopupMenu);
      Mascot->PopupMenu=make_popup_menu(Mascot);

    }
    else{
      popup_message(Mascot->win_main,
#ifdef USE_GTK3
		    "dialog-error", 
#else
		    GTK_STOCK_DIALOG_ERROR,
#endif
		    -1,
		    _("Error: File cannot be opened."),
		    " ",
		    dest_file,
		    NULL);
    }

    Mascot->flag_menu=FALSE;
  }
  else {
    gtk_widget_destroy(fdialog);
    Mascot->flag_menu=FALSE;
  }

  if(dest_file) g_free(dest_file);
}

  
static void create_save_mascot_dialog(void)
{
  GtkWidget *fdialog;
  gchar *fname=NULL;
  GtkWidget *dialog;
  GtkWidget *label;
  GtkWidget *button;
  gchar *dest_file=NULL;
  gchar *tmp;

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  //while (my_main_iteration(FALSE));

  Mascot->flag_menu=TRUE;
  
  fdialog = gtk_file_chooser_dialog_new(_("Save Mascot to Other File"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_SAVE,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Save", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_SAVE_AS, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				

  gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (fdialog), 
				   to_utf8(Mascot->file));
  gtk_file_chooser_select_filename (GTK_FILE_CHOOSER (fdialog), 
				    to_utf8(Mascot->file));

  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_DEFAULT);
  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_CURRENT);
  my_file_chooser_add_filter(fdialog,_("Mascot File"),MASCOT_EXTENSION);
  my_file_chooser_add_filter(fdialog,_("All File"),"*");

  gtk_widget_show_all(fdialog);

  if (gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {
    fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fdialog));
    gtk_widget_destroy(fdialog);
  }
  else {
    gtk_widget_destroy(fdialog);
    Mascot->flag_menu=FALSE;
    return;
  }

  dest_file=to_locale(fname);
  if(fname) g_free(fname);

  if(access(dest_file,F_OK)==0){
    dialog = gtk_dialog_new_with_buttons(_("File Save : Overwrite Confirmation"),
					 NULL,
					 GTK_DIALOG_MODAL,
#ifdef USE_GTK3
					 "_Cancel",GTK_RESPONSE_CANCEL,
					 "_OK",GTK_RESPONSE_OK,
#else
					 GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
					 GTK_STOCK_OK,GTK_RESPONSE_OK,
#endif
					 NULL);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(dialog),5);
    
    tmp=g_strdup_printf(_("File %s already exists."),dest_file);
    label=gtkut_label_new(tmp);
    g_free(tmp);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		       label,TRUE,TRUE,0);
    label=gtkut_label_new(_("Overwrite?"));
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		       label,TRUE,TRUE,0);

    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
      gtk_widget_destroy(dialog);
      
      Mascot->file=g_strdup(dest_file);
      SaveMascot(Mascot,FALSE);
      tmp=g_strdup_printf(_("Saved Mascot as ...%%n    \"%s\""),Mascot->file);
      DoSysBalloon(Mascot, tmp);
      g_free(tmp);
    }
    else{
      gtk_widget_destroy(dialog);
    }    
  }
  else{
    Mascot->file=g_strdup(dest_file);
    SaveMascot(Mascot,FALSE);
    tmp=g_strdup_printf(_("Saved Mascot as ...%%n    \"%s\""),Mascot->file);
    DoSysBalloon(Mascot,tmp);
    g_free(tmp);
  }

  if(dest_file) g_free(dest_file);
  Mascot->flag_menu=FALSE;
  return;
}


static void create_save_nokkari_dialog(void)
{
  GtkWidget *fdialog;
  gchar *fname=NULL;
  GtkWidget *dialog;
  GtkWidget *label;
  GtkWidget *button;
  gchar *dest_file=NULL;
  gchar *tmp;
    
  // Win構築は重いので先にExposeイベント等をすべて処理してから
  //while (my_main_iteration(FALSE));

  Mascot->flag_menu=TRUE;
  
  fdialog = gtk_file_chooser_dialog_new(_("Convert Mascot to Nokkari-Chara"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_SAVE,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Save", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				
  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_DEFAULT);
  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_CURRENT);
  my_file_chooser_add_filter(fdialog,_("Nokkari-Chara File"),NKR_EXTENSION);
  my_file_chooser_add_filter(fdialog,_("All File"),"*");


  gtk_widget_show_all(fdialog);

  if (gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {    
    fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fdialog));
    gtk_widget_destroy(fdialog);    
  }
  else {
    gtk_widget_destroy(fdialog);
    Mascot->flag_menu=FALSE;
    return;
  }

  dest_file=to_locale(fname);
  if(fname) g_free(fname);
    
  
  if(access(dest_file,F_OK)==0){
    dialog = gtk_dialog_new_with_buttons(_("File Save : Overwrite Confirmation"),
					 NULL,
					 GTK_DIALOG_MODAL,
#ifdef USE_GTK3
					 "_Cancel",GTK_RESPONSE_CANCEL,
					 "_OK",GTK_RESPONSE_OK,
#else
					 GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
					 GTK_STOCK_OK,GTK_RESPONSE_OK,
#endif
					 NULL);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(dialog),5);
    
    tmp=g_strdup_printf(_("File %s already exists."), dest_file);
    label=gtkut_label_new(tmp);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		       label,TRUE,TRUE,0);
    label=gtkut_label_new(_("Overwrite?"));
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		       label,TRUE,TRUE,0);

    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
      gtk_widget_destroy(dialog);
      Mascot->inifile=g_strdup(dest_file);
      NkrSave(Mascot);
    }
    else{
      gtk_widget_destroy(dialog);
    }
    
  }
  else{
    Mascot->inifile=g_strdup(dest_file);
    NkrSave(Mascot);
  }

  if(dest_file) g_free(dest_file);  
  Mascot->flag_menu=FALSE;
  return;
}

static void create_save_release_mascot_dialog(void)
{
  GtkWidget *fdialog;
  GtkWidget *dialog;
  GtkWidget *table;
  GtkWidget *label;
  GtkWidget *entry;
  GtkWidget *check;
  GtkWidget *hbox;
  gchar *new_copyright;
  gchar *new_name;
  gchar *new_code;
  gboolean clk_font_flag;
  gboolean bal_font_flag;
  gboolean clk_color_flag;
  gboolean bal_color_flag;
  gchar *fname=NULL;
  gchar *dest_file=NULL;
  gchar *tmp;
  gchar *tmp_file,*tmp_file1,*tmp_file2;
  int i_pix;


  Mascot->flag_menu=TRUE;

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  //while (my_main_iteration(FALSE));

  // Copyright用ダイアログ
  dialog = gtk_dialog_new_with_buttons(_("Setup for Release Mascot"),
				       NULL,
				       GTK_DIALOG_MODAL,
#ifdef USE_GTK3
				       "_Cancel",GTK_RESPONSE_CANCEL,
				       "_OK",GTK_RESPONSE_OK,
#else
				       GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
				       GTK_STOCK_OK,GTK_RESPONSE_OK,
#endif
				       NULL);
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(dialog),5);

  clk_font_flag=FALSE;
  bal_font_flag=FALSE;
  clk_color_flag=FALSE;
  bal_color_flag=FALSE;

  label=gtkut_label_new(_("Please edit the setting for this mascot"));
  gtkut_pos(label, POS_CENTER, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     label,TRUE,TRUE,0);
  
  table = gtkut_table_new (2, 6, FALSE, 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     table,TRUE,TRUE,0);


  label=gtkut_label_new(_("Mascot Name"));
  gtkut_pos(label, POS_CENTER, POS_CENTER);
  gtkut_table_attach(table, label, 0, 1, 0, 1,
		     GTK_FILL,GTK_SHRINK,0,0);

  entry = gtk_entry_new ();
  if(Mascot->name){
    new_name=g_strdup(Mascot->name);
    gtk_entry_set_text(GTK_ENTRY(entry), new_name);
  }
  else{
    new_name=NULL;
  }
  gtkut_table_attach_defaults(table, entry, 1, 2, 0, 1);
  my_signal_connect (entry, "changed", cc_get_entry, &new_name);

  label=gtkut_label_new(_("Copyright"));
  gtkut_pos(label, POS_CENTER, POS_CENTER);
  gtkut_table_attach(table, label, 0, 1, 1, 2,
		     GTK_FILL,GTK_SHRINK,0,0);

  entry = gtk_entry_new ();
  if(Mascot->copyright){
    new_copyright=g_strdup(Mascot->copyright);
  }
  else{
    new_copyright=g_strdup("Copyright (C)");
  }
  gtk_entry_set_text(GTK_ENTRY(entry),new_copyright);
  gtkut_table_attach_defaults(table, entry, 1, 2, 1, 2);
  my_signal_connect (entry, "changed", cc_get_entry, &new_copyright);

  label=gtkut_label_new(_("Character Code"));
  gtkut_pos(label, POS_CENTER, POS_CENTER);
  gtkut_table_attach(table, label, 0, 1, 2, 3,
		     GTK_FILL,GTK_SHRINK,0,0);

  entry = gtk_entry_new ();
  new_code=g_strdup(DEF_CODE1);
  gtk_entry_set_text(GTK_ENTRY(entry),new_code);
  gtkut_table_attach_defaults(table, entry, 1, 2, 2, 3);
  my_signal_connect (entry, "changed", cc_get_entry, &new_code);

  check = gtk_check_button_new_with_label(_("Save Fontset for Clock"));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),FALSE);
  gtkut_table_attach(table, check, 0, 2, 3, 4,
		     GTK_FILL,GTK_SHRINK,0,0);
  my_signal_connect (check, "toggled", cc_get_toggle, &clk_font_flag);

  check = gtk_check_button_new_with_label(_("Save Colors for Clock"));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),FALSE);
  gtkut_table_attach(table, check, 0, 2, 4, 5,
		     GTK_FILL,GTK_SHRINK,0,0);
  my_signal_connect (check, "toggled", cc_get_toggle, &clk_color_flag);
  
  check = gtk_check_button_new_with_label(_("Save Fontset for Balloon"));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),FALSE);
  gtkut_table_attach(table, check, 0, 2, 5, 6,
		     GTK_FILL,GTK_SHRINK,0,0);
  my_signal_connect (check, "toggled", cc_get_toggle, &bal_font_flag);

  check = gtk_check_button_new_with_label(_("Save Colors for Baloon"));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),FALSE);
  gtkut_table_attach(table, check, 0, 2, 6, 7,
		     GTK_FILL,GTK_SHRINK,0,0);
  my_signal_connect (check, "toggled", cc_get_toggle, &bal_color_flag);

  gtk_widget_show_all(dialog);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
    gtk_widget_destroy(dialog);
  }
  else{
    // Mascot File選択 : Cancelが押されていたら終了
    gtk_widget_destroy(dialog);
    Mascot->flag_menu=FALSE;
    return;
  }
  

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  //while (my_main_iteration(FALSE));

  
  fdialog = gtk_file_chooser_dialog_new(_("Save Mascot for Release"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_SAVE,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Save", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				
  
  
  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_DEFAULT);
  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_CURRENT);
  my_file_chooser_add_filter(fdialog,_("Mascot File"),MASCOT_EXTENSION);
  my_file_chooser_add_filter(fdialog,_("All File"),"*");
  gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (fdialog), 
				     my_basename(to_utf8(Mascot->file)));
  

  gtk_widget_show_all(fdialog);

  if (gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {    
    fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fdialog));
    gtk_widget_destroy(fdialog);
  }
  else {
    gtk_widget_destroy(fdialog);
    Mascot->flag_menu=FALSE;
    return;
  }


  dest_file=to_locale(fname);
  if(fname) g_free(fname);
  
  if(access(dest_file,F_OK)==0){    
    dialog = gtk_dialog_new_with_buttons(_("File Save : Overwrite Confirmation"),
					 NULL,
					 GTK_DIALOG_MODAL,
#ifdef USE_GTK3
					 "_Cancel",GTK_RESPONSE_CANCEL,
					 "_OK",GTK_RESPONSE_OK,
#else
					 GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
					 GTK_STOCK_OK,GTK_RESPONSE_OK,
#endif
					 NULL);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(dialog),5);
      
    tmp=g_strdup_printf(_("File %s already exists."),dest_file);
    label=gtkut_label_new(tmp);
    g_free(tmp);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		       label,TRUE,TRUE,0);
    label=gtkut_label_new(_("Overwrite?"));
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		       label,TRUE,TRUE,0);

    gtk_widget_show_all(dialog);
    
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {    
      gtk_widget_destroy(dialog);
    }
    else {
      gtk_widget_destroy(dialog);
      Mascot->flag_menu=FALSE;
      return;
    }

    if(new_copyright){
      Mascot->copyright=g_strdup(new_copyright);
    }
    else{
      Mascot->copyright=NULL;
    }
    if(new_name){
      Mascot->name=g_strdup(new_name);
    }
    else{
      Mascot->name=NULL;
    }
    if(new_code){
      Mascot->code=g_strdup(new_code);
    }
    else{
      Mascot->code=NULL;
    }
    Mascot->file=g_strdup(dest_file);
    SaveMascot(Mascot,TRUE);
    SaveMascot(Mascot,FALSE);

    tmp=g_strdup_printf(_("Saved Mascot as ...%%n    \"%s\""),Mascot->file);
    DoSysBalloon(Mascot,tmp);
    g_free(tmp);
      
    SetFontForReleaseData(Mascot,TRUE, SET_RELEASE_CLOCK, clk_font_flag);
    SetFontForReleaseData(Mascot,FALSE,SET_RELEASE_CLOCK, clk_font_flag);

    SetFontForReleaseData(Mascot,TRUE, SET_RELEASE_BALLOON,bal_font_flag);
    SetFontForReleaseData(Mascot,FALSE,SET_RELEASE_BALLOON,bal_font_flag);
    
    SetColorForReleaseData(Mascot,TRUE, SET_RELEASE_CLOCK, clk_color_flag);
    SetColorForReleaseData(Mascot,FALSE,SET_RELEASE_CLOCK, clk_color_flag);
      
    SetColorForReleaseData(Mascot,TRUE,SET_RELEASE_BALLOON, bal_color_flag);
    SetColorForReleaseData(Mascot,FALSE,SET_RELEASE_BALLOON,bal_color_flag);
      
    tmp_file = g_strconcat(my_dirname(Mascot->file),G_DIR_SEPARATOR_S,NULL);
#ifdef USE_WIN32
    tmp_file1 = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,USER_DIR, NULL);
#else
    tmp_file1 = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,USER_DIR, NULL);
#endif
#ifdef USE_COMMON
    tmp_file2 = g_strdup(COMMON_DIR);
      
    if((strcmp(tmp_file,tmp_file1)!=0)&&
       (strcmp(tmp_file,tmp_file2)!=0)){
#else
    if((strcmp(tmp_file,tmp_file1)!=0)){
#endif
      for(i_pix=0;i_pix<Mascot->nPixmap;i_pix++){
        tmp_file=g_strconcat(my_dirname(Mascot->file),G_DIR_SEPARATOR_S,
			     my_basename(Mascot->sprites[i_pix].filename),
			     NULL);
	  
	if(access(tmp_file,F_OK)!=0){
	  copy_file(Mascot->sprites[i_pix].filename,tmp_file);
	}
      }
    }
  }
  else{
    if(new_copyright){
      Mascot->copyright=g_strdup(new_copyright);
    }
    else{
      Mascot->copyright=NULL;
    }
    if(new_name){
      Mascot->name=g_strdup(new_name);
    }
    else{
      Mascot->name=NULL;
    }
    if(new_code){
      Mascot->code=g_strdup(new_code);
    }
    else{
      Mascot->code=NULL;
    }
    Mascot->file=g_strdup(dest_file);
    SaveMascot(Mascot,TRUE);
    SaveMascot(Mascot,FALSE);

    tmp=g_strdup_printf(_("Saved Mascot as ...%%n    \"%s\""),Mascot->file);
    DoSysBalloon(Mascot,tmp);
    g_free(tmp);
      
    SetFontForReleaseData(Mascot,TRUE, SET_RELEASE_CLOCK, clk_font_flag);
    SetFontForReleaseData(Mascot,FALSE,SET_RELEASE_CLOCK, clk_font_flag);

    SetFontForReleaseData(Mascot,TRUE, SET_RELEASE_BALLOON, bal_font_flag);
    SetFontForReleaseData(Mascot,FALSE,SET_RELEASE_BALLOON, bal_font_flag);
      
    SetColorForReleaseData(Mascot,TRUE, SET_RELEASE_CLOCK, clk_color_flag);
    SetColorForReleaseData(Mascot,FALSE,SET_RELEASE_CLOCK, clk_color_flag);

    SetColorForReleaseData(Mascot,TRUE, SET_RELEASE_BALLOON, bal_color_flag);
    SetColorForReleaseData(Mascot,FALSE,SET_RELEASE_BALLOON, bal_color_flag);
    
    
    tmp_file = g_strconcat(my_dirname(Mascot->file),G_DIR_SEPARATOR_S,NULL);
#ifdef USE_WIN32
    tmp_file1 = g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,USER_DIR, NULL);
#else
    tmp_file1 = g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,USER_DIR, NULL);
#endif
    
#ifdef USE_COMMON
    tmp_file2 = g_strdup(COMMON_DIR);
    
    if( (strcmp(tmp_file,tmp_file1)!=0) && (strcmp(tmp_file,tmp_file2)!=0) ){
#else
    if((strcmp(tmp_file,tmp_file1)!=0)){
#endif
      for(i_pix=0;i_pix<Mascot->nPixmap;i_pix++){
	  
	tmp_file=g_strconcat(my_dirname(Mascot->file),G_DIR_SEPARATOR_S,
			     my_basename(Mascot->sprites[i_pix].filename),
			     NULL);
	
	if(access(tmp_file,F_OK)!=0){
	  copy_file(Mascot->sprites[i_pix].filename,tmp_file);
	}
      }
    }
  }

  Mascot->flag_menu=FALSE;
  if(dest_file) g_free(dest_file);
  return;
}

static void create_save_menu_dialog(void)
{
  GtkWidget *fdialog;
  gchar *fname=NULL;
  GtkWidget *dialog;
  GtkWidget *label;
  GtkWidget *button;
  gchar *dest_file=NULL;
  gchar *tmp;

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  //while (my_main_iteration(FALSE));

  Mascot->flag_menu=TRUE;
  
  fdialog = gtk_file_chooser_dialog_new(_("Save Menu to File"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_SAVE,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Save", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
#endif
					NULL);				

  if(Mascot->menu_file){
    gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (fdialog), 
				     to_utf8(Mascot->menu_file));
    gtk_file_chooser_select_filename (GTK_FILE_CHOOSER (fdialog), 
				     to_utf8(Mascot->menu_file));
  }

  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_DEFAULT);
  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_CURRENT);
  my_file_chooser_add_filter(fdialog,_("Menu File"),MENU_EXTENSION);
  my_file_chooser_add_filter(fdialog,_("All File"),"*");

  gtk_widget_show_all(fdialog);

  if (gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {
    fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fdialog));
    gtk_widget_destroy(fdialog);    
  }
  else {
    gtk_widget_destroy(fdialog);
    Mascot->flag_menu=FALSE;
    return;
  }

  dest_file=to_locale(fname);
  if(fname) g_free(fname);

  if(access(dest_file,F_OK)==0){
    dialog = gtk_dialog_new_with_buttons(_("Menu Save : Overwrite Confirmation"),
					 NULL,
					 GTK_DIALOG_MODAL,
#ifdef USE_GTK3
					 "_Cancel",GTK_RESPONSE_CANCEL,
					 "_OK",GTK_RESPONSE_OK,
#else
					 GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
					 GTK_STOCK_OK,GTK_RESPONSE_OK,
#endif
					 NULL);   
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(dialog),5);
    
    tmp=g_strdup_printf(_("Menu File %s already exists."),dest_file);
    label=gtkut_label_new(tmp);
    g_free(tmp);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		       label,TRUE,TRUE,0);
    label=gtkut_label_new(_("Overwrite?"));
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		       label,TRUE,TRUE,0);

    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {    
      gtk_widget_destroy(dialog);

      Mascot->menu_file=g_strdup(dest_file);
      SaveMenu(Mascot);

      tmp=g_strdup_printf(_("Saved Menu as ...%%n    \"%s\""),Mascot->menu_file);
      DoSysBalloon(Mascot,tmp);
      g_free(tmp);
    }
    else {
      gtk_widget_destroy(dialog);
      Mascot->flag_menu=FALSE;
      return;
    }
  }
  else{
    Mascot->menu_file=g_strdup(dest_file);
    SaveMenu(Mascot);

    tmp=g_strdup_printf(_("Saved Menu as ...%%n    \"%s\""),Mascot->menu_file);
    DoSysBalloon(Mascot,tmp);
    g_free(tmp);
  }

  if(dest_file) g_free(dest_file);
  Mascot->flag_menu=FALSE;
  return;
}

static void create_save_rc_dialog(void)
{
  GtkWidget *fdialog;
  gchar *fname=NULL;
  GtkWidget *dialog;
  GtkWidget *label;
  GtkWidget *button;
  gchar *dest_file=NULL;
  gchar *tmp;

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  while (my_main_iteration(FALSE));

  Mascot->flag_menu=TRUE;
  
  fdialog = gtk_file_chooser_dialog_new(_("Save Resource to File"),
					NULL,
					GTK_FILE_CHOOSER_ACTION_SAVE,
#ifdef USE_GTK3
					"_Cancel",GTK_RESPONSE_CANCEL,
					"_Save", GTK_RESPONSE_ACCEPT,
#else
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
#endif					
					NULL);				

  if(Mascot->rcfile){
    gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (fdialog), 
				      to_utf8(Mascot->rcfile));
    gtk_file_chooser_select_filename (GTK_FILE_CHOOSER (fdialog), 
				      to_utf8(Mascot->rcfile));
  }

  my_file_chooser_add_shortcut_folder(fdialog, FOLDER_DEFAULT);
  my_file_chooser_add_filter(fdialog,_("Resource File"),RC_EXTENSION);
  my_file_chooser_add_filter(fdialog,_("All File"),"*");

  gtk_widget_show_all(fdialog);

  if (gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {
    fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fdialog));
    gtk_widget_destroy(fdialog);
  }
  else {
    gtk_widget_destroy(fdialog);
    Mascot->flag_menu=FALSE;
    return;
  }

  dest_file=to_locale(fname);
  if(fname) g_free(fname);

  if(access(dest_file,F_OK)==0){
    dialog = gtk_dialog_new_with_buttons(_("Resource Save : Overwrite Confirmation"),
					 NULL,
					 GTK_DIALOG_MODAL,
#ifdef USE_GTK3
					 "_Cancel",GTK_RESPONSE_CANCEL,
					 "_OK",GTK_RESPONSE_OK,
#else
					 GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
					 GTK_STOCK_OK,GTK_RESPONSE_OK,
#endif
					 NULL);   
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(dialog),5);
    
    tmp=g_strdup_printf(_("Resource File %s already exists."),dest_file);
    label=gtkut_label_new(tmp);
    g_free(tmp);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		       label,TRUE,TRUE,0);
    label=gtkut_label_new(_("Overwrite?"));
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		       label,TRUE,TRUE,0);
    
    gtk_widget_show_all(dialog);

      
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {    
      gtk_widget_destroy(dialog);

      Mascot->rcfile=g_strdup(dest_file);
      SaveRC(Mascot,FALSE);
      
      tmp=g_strdup_printf(_("Saved Resource as ...%%n    \"%s\""),Mascot->rcfile);
      DoSysBalloon(Mascot,tmp);
      g_free(tmp);
    }
    else {
      gtk_widget_destroy(dialog);
      Mascot->flag_menu=FALSE;
      return;
    }
  }      
  else{
    Mascot->rcfile=g_strdup(dest_file);
    SaveRC(Mascot,TRUE);
    SaveRC(Mascot,FALSE);

    tmp=g_strdup_printf(_("Saved Resource as ...%%n    \"%s\""),Mascot->rcfile);
    DoSysBalloon(Mascot,tmp);
    g_free(tmp);
  }

  if(dest_file) g_free(dest_file);
  Mascot->flag_menu=FALSE;
  return;
}



static void change_colbal_default(GtkWidget *w, gpointer gdata)
{
  typMascot *mascot;

  mascot=(typMascot *)gdata;
  
#ifdef USE_GTK3
  mascot->colbal  =gdk_rgba_copy(mascot->def_colbal);
  mascot->colbalbg=gdk_rgba_copy(mascot->def_colbalbg);
  mascot->colbalbd=gdk_rgba_copy(mascot->def_colbalbd);

  mascot->alpbal = (gint)(mascot->def_colbal->alpha*100);
  mascot->alpbalbg =(gint)(mascot->def_colbalbg->alpha*100);
  mascot->alpbalbd =(gint)(mascot->def_colbalbd->alpha*100);;
#else
  mascot->colbal  =gdk_color_copy(mascot->def_colbal);
  mascot->colbalbg=gdk_color_copy(mascot->def_colbalbg);
  mascot->colbalbd=gdk_color_copy(mascot->def_colbalbd);

  mascot->alpbal =mascot->def_alpbal;
  mascot->alpbalbg =mascot->def_alpbalbg;
  mascot->alpbalbd =mascot->def_alpbalbd;
#endif
}
  

static void change_colclk_default(GtkWidget *w, gpointer gdata)
{
  typMascot *mascot;

  mascot=(typMascot *)gdata;

#ifdef USE_GTK3
  mascot->colclk  =gdk_rgba_copy(mascot->def_colclk);
  mascot->colclkbg=gdk_rgba_copy(mascot->def_colclkbg);
  mascot->colclkbd=gdk_rgba_copy(mascot->def_colclkbd);
  mascot->colclksd=gdk_rgba_copy(mascot->def_colclksd);

  mascot->alpclk =(gint)(mascot->def_colclk->alpha*100);
  mascot->alpclksd =(gint)(mascot->def_colclksd->alpha*100);
  mascot->alpclkbg =(gint)(mascot->def_colclkbg->alpha*100);
  mascot->alpclkbd =(gint)(mascot->def_colclkbd->alpha*100);
#else
  mascot->colclk  =gdk_color_copy(mascot->def_colclk);
  mascot->colclkbg=gdk_color_copy(mascot->def_colclkbg);
  mascot->colclkbd=gdk_color_copy(mascot->def_colclkbd);
  mascot->colclksd=gdk_color_copy(mascot->def_colclksd);

  mascot->alpclk =mascot->def_alpclk;
  mascot->alpclksd =mascot->def_alpclksd;
  mascot->alpclkbg =mascot->def_alpclkbg;
  mascot->alpclkbd =mascot->def_alpclkbd;
#endif
}
  


// 設定ダイアログの生成
void create_config_dialog(GtkWidget *widget, gpointer gdata){
  typMascot *mascot;
  GtkWidget *conf_tbl;
  GtkWidget *all_note;
  GtkWidget *button;
  int i_pix;
  gint i_col, i_cat, i_tgt, i_ptn;

  mascot = (typMascot *)gdata;
  flag_make_pixmap_list=FALSE;
 
  // Win構築は重いので先にExposeイベント等をすべて処理してから
  while (my_main_iteration(FALSE));
 

  mascot->flag_menu=TRUE;
 
  mascot->imgtree_i=-1;
  for(i_ptn=0;i_ptn<MAX_ANIME_PATTERN;i_ptn++){
    mascot->ptntree_i_frm[i_ptn]=-1;
  }

  //conf_main = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  mascot->conf_main = gtk_dialog_new_with_buttons(_("Config for MaCoPiX"),
						  NULL,
						  GTK_DIALOG_MODAL,
#ifdef USE_GTK3
						  "_OK", GTK_RESPONSE_OK,
#else
						  GTK_STOCK_OK,GTK_RESPONSE_OK,
#endif
						  NULL);
  gtk_dialog_set_default_response(GTK_DIALOG(mascot->conf_main),
				  GTK_RESPONSE_OK); 
  gtk_widget_grab_focus(gtk_dialog_get_widget_for_response(GTK_DIALOG(mascot->conf_main),
							   GTK_RESPONSE_OK));
  gtk_container_set_border_width(GTK_CONTAINER(mascot->conf_main),5);

  //gtk_window_set_title(GTK_WINDOW(mascot->conf_main), 
  //		       _("Config for MaCoPiX"));
  //gtk_widget_realize(conf_main);
  //my_signal_connect(conf_main,"destroy",close_conf, GTK_WIDGET(conf_main));
  //gtk_container_set_border_width (GTK_CONTAINER (conf_main), 5);
  
  // 3x6のテーブル
  conf_tbl = gtkut_table_new (3, 6, FALSE, 0, 0, 0);
  //gtk_container_add (GTK_CONTAINER (conf_main), conf_tbl);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(mascot->conf_main))),
		     conf_tbl,FALSE, FALSE, 5);


  all_note = gtk_notebook_new ();
  gtk_notebook_set_tab_pos (GTK_NOTEBOOK (all_note), GTK_POS_TOP);
  gtk_notebook_set_scrollable (GTK_NOTEBOOK (all_note), TRUE);

  gtkut_table_attach_defaults (conf_tbl, all_note, 0, 6, 0, 1);

  /////// マスコット共通の設定 ///////
  {
  GtkWidget *conf_note;
  GtkWidget *label;

  conf_note = gtk_notebook_new ();
  gtk_notebook_set_tab_pos (GTK_NOTEBOOK (conf_note), GTK_POS_TOP);
  gtk_notebook_set_scrollable (GTK_NOTEBOOK (conf_note), TRUE);

  /*
  button=gtkut_button_new_with_icon(_("Cancel"),
#ifdef USE_GTK3
				    "process-stop"
#else
				    GTK_STOCK_CANCEL
#endif				     
				    );
  gtkut_table_attach(conf_tbl, button, 4, 5, 1, 2,
		     GTK_SHRINK,GTK_SHRINK,0,0);


  my_signal_connect(button,"clicked",close_conf, GTK_WIDGET(conf_main));

  button=gtkut_button_new_with_icon(_("OK"),
#ifdef USE_GTK3
				    "emblem-default"
#else
				    GTK_STOCK_OK
#endif				     
				    );
  gtkut_table_attach(conf_tbl, button, 5, 6, 1, 2,
		     GTK_SHRINK,GTK_SHRINK,0,0);
  my_signal_connect(button,"clicked",conf_change, GTK_WIDGET(conf_main));
  */
  
  // ノートブックの生成
  {
    gint i_resource=0;
    GtkWidget *frame;
    GtkWidget *table;
    GtkWidget *label;
    GtkWidget *combo;
    GtkWidget *clabel;
    GtkWidget *table1;
    GtkWidget *table2;
    GtkWidget *check;
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkAdjustment *adj;
    GtkWidget *scale;
    GtkWidget *spinner;
    GtkWidget *button;
    GtkWidget *scrwin;
    GtkWidget *rb[2];

    
    // 移動設定
    i_resource++;
    {
      GtkWidget *table3;
      
      // 3x2のテーブル
      table = gtkut_table_new (2, 3, FALSE, 0, 0, 0);

      frame = gtkut_frame_new (_("Focus Follow"));
      gtkut_table_attach(table, frame, 0, 2, 0, 1,
			 GTK_FILL,GTK_SHRINK,0,0);
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
	
      table1 = gtkut_table_new(2,5,FALSE, 0, 0, 5);
      gtk_container_add (GTK_CONTAINER (frame), table1);


      frame = gtkut_frame_new (_("Offset on the Title Bar"));
      gtkut_table_attach(table1, frame, 0, 2, 0, 1,
			 GTK_FILL,GTK_SHRINK,0,0);
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
	
      table2 = gtkut_table_new(2,3,FALSE, 15, 0, 5);
      gtk_container_add (GTK_CONTAINER (frame), table2);


      vbox = gtkut_hbox_new(FALSE,10);
      gtk_container_set_border_width (GTK_CONTAINER (vbox), 0);
      gtkut_table_attach_defaults (table2, vbox, 0, 2, 0, 1);

      label = gtkut_label_new (_("Starting Point"));
      gtkut_pos(label, POS_END, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 5);

      {
	GtkListStore *store;
	GtkTreeIter iter, iter_set;	  
	GtkCellRenderer *renderer;
	
	store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Left upper corner of the focused window"),
			   1, FF_SIDE_LEFT, -1);
	if(mascot->ff_side==FF_SIDE_LEFT) iter_set=iter;
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Right upper corner of the focused window"),
			   1, FF_SIDE_RIGHT, -1);
	if(mascot->ff_side==FF_SIDE_RIGHT) iter_set=iter;
	
	combo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
	gtk_box_pack_start(GTK_BOX(vbox), combo,FALSE, FALSE, 0);
	g_object_unref(store);
	
	renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo),renderer, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT(combo), renderer, "text",0,NULL);
	
	
	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo),&iter_set);
	gtk_widget_show(combo);
	my_signal_connect (combo,"changed",cc_get_combo_box,
			   &mascot->ff_side);
      }

      table3 = gtkut_table_new(3,2,FALSE, 0, 0, 5);
      gtkut_table_attach (table2, table3, 0, 2, 1, 3,
			  GTK_FILL|GTK_EXPAND,GTK_SHRINK,0,0);

      rb[FF_BAR_ABS] = gtk_radio_button_new_with_label_from_widget (NULL, _("Absolute[pix]"));
      gtkut_table_attach(table3, rb[FF_BAR_ABS], 0, 2, 0, 1,
			 GTK_SHRINK,GTK_SHRINK,0,0);
      gtk_widget_show (rb[FF_BAR_ABS]);
      my_signal_connect (rb[FF_BAR_ABS], "toggled", cc_radio, &mascot->flag_xp);
      
      rb[FF_BAR_REL] = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(rb[FF_BAR_ABS]), _("Relative[%]"));
      gtkut_table_attach(table3, rb[FF_BAR_REL], 0, 2, 1, 2,
			 GTK_SHRINK,GTK_SHRINK,0,0);
      gtk_widget_show (rb[FF_BAR_REL]);
      my_signal_connect (rb[FF_BAR_REL], "toggled", cc_radio, &mascot->flag_xp);
      
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rb[mascot->flag_xp]),TRUE);

      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->offset,
	 -mascot->width_root, mascot->width_root,
	 1.0, 10.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,&mascot->offset);
      scale =  gtkut_hscale_new (GTK_ADJUSTMENT(adj));
      gtk_scale_set_digits (GTK_SCALE (scale), 0);
      gtk_scale_set_draw_value (GTK_SCALE (scale), TRUE);
      gtkut_table_attach (table3, scale, 2, 3, 0, 1,
			  GTK_FILL|GTK_EXPAND,GTK_SHRINK,0,0);

      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->offsetp, 0, 100, 1.0, 10.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,&mascot->offsetp);
      scale =  gtkut_hscale_new (GTK_ADJUSTMENT(adj));
      gtk_scale_set_digits (GTK_SCALE (scale), 0);
      gtk_scale_set_draw_value (GTK_SCALE (scale), TRUE);
      gtkut_table_attach (table3, scale, 2, 3, 1, 2,
			  GTK_FILL|GTK_EXPAND,GTK_SHRINK,0,0);


      frame = gtkut_frame_new (_("Title Bar Size"));
      gtkut_table_attach(table1, frame, 0, 1, 1, 2,
			 GTK_FILL,GTK_FILL,0,0);
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
	
      table2 = gtkut_table_new(2,4,FALSE, 8, 5, 5);
      gtk_container_add (GTK_CONTAINER (frame), table2);


      {
	GtkListStore *store;
	GtkTreeIter iter, iter_set;	  
	GtkCellRenderer *renderer;
	
	store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Manual Scaling"),
			   1, AUTOBAR_MANUAL, -1);
	if(mascot->focus_autobar==AUTOBAR_MANUAL) iter_set=iter;
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Auto Scaling"),
			   1, AUTOBAR_ORDINAL, -1);
	if(mascot->focus_autobar==AUTOBAR_ORDINAL) iter_set=iter;

	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Auto Scaling (for Compiz)"),
			   1, AUTOBAR_COMPIZ, -1);
	if(mascot->focus_autobar==AUTOBAR_COMPIZ) iter_set=iter;
	
	combo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
	gtkut_table_attach (table2, combo,0, 2, 0, 1,
			    GTK_FILL,GTK_SHRINK,0,0);

	g_object_unref(store);
	
	renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo),renderer, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT(combo), renderer, "text",0,NULL);
	
	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo),&iter_set);
	gtk_widget_show(combo);
	my_signal_connect (combo,"changed",cc_get_autobar_combo,
			   (gpointer)mascot);
      }

#ifdef USE_WIN32
      gtk_widget_set_sensitive(combo,FALSE);
#endif

#ifndef USE_WIN32
      label = gtkut_label_new (_("Window Manager [start-up]"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtkut_table_attach_defaults (table2, label, 0, 1, 1, 2);
      {
	label = gtkut_label_new (GetCurrentWMName(mascot->conf_main));
	gtkut_pos(label, POS_START, POS_CENTER);
	gtkut_table_attach_defaults (table2, label, 1, 2, 1, 2);
      }
#endif


      label = gtkut_label_new (_("Bar Size (Manual)"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtkut_table_attach_defaults (table2, label, 0, 1, 2, 3);
      adj = (GtkAdjustment *)gtk_adjustment_new ((gdouble)mascot->bar_size, 0.0, 50.0, 1.0, 1.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,&mascot->bar_size);
      spinner_manual =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner_manual), FALSE);
      gtkut_table_attach (table2, spinner_manual, 1, 2, 2, 3,
			  GTK_SHRINK,GTK_SHRINK,0,0);
#ifdef USE_WIN32
      gtk_widget_set_sensitive(spinner_manual,FALSE);
#else
      if(mascot->focus_autobar!=AUTOBAR_MANUAL){
	gtk_widget_set_sensitive(spinner_manual,FALSE);
      }
#endif


      label = gtkut_label_new (_("Offset for Shaped Bar"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtkut_table_attach_defaults (table2, label, 0, 1, 3, 4);
      adj = (GtkAdjustment *)gtk_adjustment_new ((gdouble)mascot->bar_offset, -50, 50.0, 1.0, 1.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->bar_offset);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtkut_table_attach (table2, spinner, 1, 2, 3, 4,
			  GTK_SHRINK,GTK_SHRINK,0,0);
#ifdef USE_WIN32
      gtk_widget_set_sensitive(spinner,FALSE);
#endif


      frame = gtkut_frame_new (_("Home Position"));
      gtkut_table_attach(table1, frame, 1, 2, 1, 2,
			 GTK_FILL,GTK_FILL,0,0);
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
	
      table2 = gtkut_table_new(2,4,FALSE, 5, 5, 5);
      gtk_container_add (GTK_CONTAINER (frame), table2);


      vbox = gtkut_hbox_new(FALSE,10);
      gtk_container_set_border_width (GTK_CONTAINER (vbox), 0);
      gtkut_table_attach_defaults (table2, vbox, 0, 2, 0, 1);


      check = gtk_check_button_new_with_label(_("Auto"));
      gtk_box_pack_start(GTK_BOX(vbox), check,FALSE, FALSE, 5);
      if(mascot->home_auto){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      my_signal_connect (check, "toggled",cc_get_toggle_home,
			 &mascot->home_auto);
#ifdef USE_WIN32
      gtk_widget_set_sensitive(check,FALSE);
#endif

      
      label = gtkut_label_new (_("  X"));
      gtkut_pos(label, POS_END, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 2);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->home_x, 0,mascot->width_root, 1.0, 1.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,&mascot->home_x);
      spinner_home_x =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner_home_x), FALSE);
      gtk_box_pack_start(GTK_BOX(vbox), spinner_home_x,FALSE, FALSE, 5);
#ifdef USE_WIN32
      gtk_widget_set_sensitive(spinner_home_x,FALSE);
#endif


      label = gtkut_label_new (_("  Y"));
      gtkut_pos(label, POS_END, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 2);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->home_y, 0, mascot->height_root, 1.0, 1.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,&mascot->home_y);
      spinner_home_y =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner_home_y), FALSE);
      gtk_box_pack_start(GTK_BOX(vbox), spinner_home_y,FALSE, FALSE, 5);
#ifdef USE_WIN32
      gtk_widget_set_sensitive(spinner_home_y,FALSE);
#endif

      if(!mascot->home_auto){
	gtk_widget_set_sensitive(spinner_home_x,TRUE);
	gtk_widget_set_sensitive(spinner_home_y,TRUE);
      }
      else{
	gtk_widget_set_sensitive(spinner_home_x,FALSE);
	gtk_widget_set_sensitive(spinner_home_y,FALSE);
      }

      vbox = gtkut_hbox_new(FALSE,10);
      gtk_container_set_border_width (GTK_CONTAINER (vbox), 0);
      gtkut_table_attach_defaults (table2, vbox, 0, 2, 1, 2);

      label = gtkut_label_new ("  ");
      gtkut_pos(label, POS_START, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 5);

      check_vanish = gtk_check_button_new_with_label(_("Vanish when auto-detection failed"));
      gtk_box_pack_start(GTK_BOX(vbox), check_vanish,FALSE, FALSE, 5);
      if(mascot->home_auto_vanish){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_vanish),TRUE);
      }
      my_signal_connect (check_vanish, "toggled",cc_get_toggle_home,
			 &mascot->home_auto_vanish);
      if(!mascot->home_auto){
	gtk_widget_set_sensitive(check_vanish,FALSE);
      }
      else{
	gtk_widget_set_sensitive(check_vanish,TRUE);
      }


      label = gtkut_label_new (_("No Focus"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtkut_table_attach_defaults (table2, label, 0, 1, 2, 3);

      {
	GtkListStore *store;
	GtkTreeIter iter, iter_set;	  
	GtkCellRenderer *renderer;
	
	store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Do Nothing"),
			   1, HOMEPOS_NEVER, -1);
	if(mascot->homepos_nf==HOMEPOS_NEVER) iter_set=iter;
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Vanish"),
			   1, HOMEPOS_VANISH, -1);
	if(mascot->homepos_nf==HOMEPOS_VANISH) iter_set=iter;

	gtk_list_store_append(store, &iter);
#ifdef USE_WIN32
	gtk_list_store_set(store, &iter, 0, _("Sit on Task Bar"),
			   1, HOMEPOS_BAR, -1);
#else
	gtk_list_store_set(store, &iter, 0, _("Move to Home Position"),
			   1, HOMEPOS_BAR, -1);
#endif
	if(mascot->homepos_nf==HOMEPOS_BAR) iter_set=iter;
	
	combo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
	gtkut_table_attach_defaults (table2, combo, 1, 2, 2, 3);
	g_object_unref(store);
	
	renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo),renderer, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT(combo), renderer, "text",0,NULL);
	
	
	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo),&iter_set);
	gtk_widget_show(combo);
	my_signal_connect (combo,"changed",cc_get_combo_box,
			   &mascot->homepos_nf);
      }

      label = gtkut_label_new (_("Sticked Out"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtkut_table_attach_defaults (table2, label, 0, 1, 3, 4);

      {
	GtkListStore *store;
	GtkTreeIter iter, iter_set;	  
	GtkCellRenderer *renderer;
	
	store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Do Nothing"),
			   1, HOMEPOS_NEVER, -1);
	if(mascot->homepos_out==HOMEPOS_NEVER) iter_set=iter;
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Vanish"),
			   1, HOMEPOS_VANISH, -1);
	if(mascot->homepos_out==HOMEPOS_VANISH) iter_set=iter;

	gtk_list_store_append(store, &iter);
#ifdef USE_WIN32
	gtk_list_store_set(store, &iter, 0, _("Sit on Task Bar"),
			   1, HOMEPOS_BAR, -1);
#else
	gtk_list_store_set(store, &iter, 0, _("Move to Home Position"),
			   1, HOMEPOS_BAR, -1);
#endif
	if(mascot->homepos_out==HOMEPOS_BAR) iter_set=iter;
	
	combo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
	gtkut_table_attach_defaults (table2, combo, 1, 2, 3, 4);
	g_object_unref(store);
	
	renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo),renderer, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT(combo), renderer, "text",0,NULL);
	
	
	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo),&iter_set);
	gtk_widget_show(combo);
	my_signal_connect (combo,"changed",cc_get_combo_box,
			   &mascot->homepos_out);
      }

      frame = gtkut_frame_new (_("Management for Window w/o  Title Bar"));
      gtkut_table_attach(table1, frame, 0, 2, 2, 3,
			 GTK_FILL,GTK_SHRINK,0,0);
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      
      table2 = gtkut_table_new(3,2,FALSE, 0, 0, 5);
      gtk_container_add (GTK_CONTAINER (frame), table2);


      check_nb 
	= gtk_check_button_new_with_label(_("Ignore Window w/o Title Bar"));
      gtkut_table_attach(table2, check_nb, 0, 1, 0, 1,
			 GTK_FILL,GTK_SHRINK,0,0);
      if(mascot->no_capbar){
   	  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_nb),TRUE);
      }
      my_signal_connect (check_nb, 
			 "toggled",cc_get_toggle_nb,&mascot->no_capbar);
#ifndef USE_WIN32
      if(mascot->focus_autobar==AUTOBAR_MANUAL){
	gtk_widget_set_sensitive(check_nb,FALSE);
      }
#endif

      label = gtkut_label_new (_("     Action : "));
      gtkut_pos(label, POS_END, POS_CENTER);
      gtkut_table_attach(table2, label, 1, 2, 0, 1,
			 GTK_SHRINK,GTK_SHRINK,0,0);


      {
	GtkListStore *store;
	GtkTreeIter iter, iter_set;	  
	GtkCellRenderer *renderer;
	
	store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Do Nothing"),
			   1, HOMEPOS_NEVER, -1);
	if(mascot->homepos_nb==HOMEPOS_NEVER) iter_set=iter;
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Vanish"),
			   1, HOMEPOS_VANISH, -1);
	if(mascot->homepos_nb==HOMEPOS_VANISH) iter_set=iter;

	gtk_list_store_append(store, &iter);
#ifdef USE_WIN32
	gtk_list_store_set(store, &iter, 0, _("Sit on Task Bar"),
			   1, HOMEPOS_BAR, -1);
#else
	gtk_list_store_set(store, &iter, 0, _("Move to Home Position"),
			   1, HOMEPOS_BAR, -1);
#endif
	if(mascot->homepos_nb==HOMEPOS_BAR) iter_set=iter;
	
	combo_nb = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
	gtkut_table_attach_defaults (table2, combo_nb, 2, 3, 0, 1);
	g_object_unref(store);
	
	renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo_nb),renderer, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT(combo_nb), renderer, "text",0,NULL);
	
	
	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo_nb),&iter_set);
	gtk_widget_show(combo_nb);
	my_signal_connect (combo_nb,"changed",cc_get_combo_box,
			   &mascot->homepos_nb);
      }

      label = gtkut_label_new (_("    * Mascot could fail to follow the focus with older window managers."));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtkut_table_attach(table2, label, 0, 3, 1, 2,
			 GTK_FILL,GTK_SHRINK,0,0);


#ifdef USE_WIN32
      frame = gtkut_frame_new (_("for Windows"));
      gtkut_table_attach(table1, frame, 0, 2, 3, 4,
			 GTK_FILL,GTK_SHRINK,0,0);
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      
      table2 = gtkut_table_new(1,2,FALSE, 0, 0, 5);
      gtk_container_add (GTK_CONTAINER (frame), table2);

      check = gtk_check_button_new_with_label(_("Stay on Task Bar"));
      gtkut_table_attach(table2, check, 0, 1, 0, 1,
			 GTK_FILL,GTK_SHRINK,0,0);
      if(mascot->task_force){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      my_signal_connect (check, "toggled",cc_get_toggle,
			 &mascot->task_force);

#endif


      table2=gtkut_table_new(1,2,FALSE,0, 0, 0);
      gtkut_table_attach_defaults(table1, table2, 0, 2, 3, 4);


      frame = gtkut_frame_new (_("Foreground Mapping"));
      gtkut_table_attach(table, frame, 0, 2, 1, 2,
			 GTK_FILL,GTK_SHRINK,0,0);
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
	
      table1 = gtkut_table_new(1,1,FALSE, 0, 0, 5);
      gtk_container_add (GTK_CONTAINER (frame), table1);

      check = gtk_check_button_new_with_label(_("Force to Raise to the Foreground"));
      gtkut_table_attach_defaults(table1, check, 0, 1, 0, 1);
      if(mascot->raise_force){
   	  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      my_signal_connect (check, "toggled",cc_get_toggle,
			 &mascot->raise_force);

      check = gtk_check_button_new_with_label(_("Raise with Mascot shaping (for translucent KWin)"));
      gtkut_table_attach_defaults(table1, check, 0, 1, 1, 2);
      if(mascot->raise_kwin){
   	  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      my_signal_connect (check, "toggled",cc_get_toggle,
			 &mascot->raise_kwin);


      table1=gtkut_table_new(1,1,FALSE, 0, 0, 0);
      gtkut_table_attach_defaults(table, table1, 0, 2, 2, 3);

      label = gtkut_label_new (_("Move"));
      gtk_notebook_append_page (GTK_NOTEBOOK (conf_note), table, label);
    }

    // 時報
    i_resource++;
    {
      GtkWidget *entry;

      // 1x3のテーブル
      table = gtkut_table_new (1, 2, FALSE, 5, 5, 5);

      frame = gtkut_frame_new (_("Time Signal"));
      gtkut_table_attach(table, frame, 0, 1, 0, 1,
			 GTK_FILL,GTK_SHRINK,0,0);
	
      table1 = gtkut_table_new (3, 2, FALSE, 0, 0, 5);
      gtk_container_add (GTK_CONTAINER (frame), table1);

      check = gtk_check_button_new_with_label(_("Use Signal"));
      gtkut_table_attach(table1, check, 0, 2, 0, 1,
			 GTK_SHRINK, GTK_SHRINK, 0, 0);
      if(mascot->signal.flag){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      my_signal_connect (check, "toggled",cc_get_toggle,
			 &mascot->signal.flag);


      hbox = gtkut_hbox_new(FALSE,5);
      gtk_container_set_border_width (GTK_CONTAINER (hbox), 5);
      gtkut_table_attach(table1, hbox, 2, 3, 0, 1,
			 GTK_SHRINK, GTK_SHRINK, 0, 0);

      label = gtkut_label_new (_("Mascot Action"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);

      {
	GtkListStore *store;
	GtkTreeIter iter, iter_set;	  
	GtkCellRenderer *renderer;
	
	store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("None"),
			   1, SIGACT_NO, -1);
	if(mascot->signal.type==SIGACT_NO) iter_set=iter;
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Click Action"),
			   1, SIGACT_CLICK, -1);
	if(mascot->signal.type==SIGACT_CLICK) iter_set=iter;
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Mascot Random Change"),
			   1, SIGACT_CHANGE, -1);
	if(mascot->signal.type==SIGACT_CHANGE) iter_set=iter;
	
	combo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
	gtk_box_pack_start(GTK_BOX(hbox), combo,FALSE, FALSE, 0);
	g_object_unref(store);
	
	renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo),renderer, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT(combo), renderer, "text",0,NULL);
	
	
	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo),&iter_set);
	gtk_widget_show(combo);
	my_signal_connect (combo,"changed",cc_get_combo_box,
			   &mascot->signal.type);
      }

      label = gtkut_label_new (_("External Command"));
      gtkut_pos(label, POS_START, POS_START);
      gtkut_table_attach(table1, label, 0, 1, 1, 2, 
			 GTK_SHRINK, GTK_SHRINK, 0, 0);

      entry = gtk_entry_new ();
      my_signal_connect (entry,"changed",cc_get_entry,&mascot->signal.com);
      gtkut_table_attach (table1, entry,
			  1, 3, 1, 2,
			  GTK_EXPAND|GTK_FILL, GTK_SHRINK,0,0);
      if(mascot->signal.com){
	gtk_entry_set_text(GTK_ENTRY(entry), mascot->signal.com);
      }


      table1=gtkut_table_new(1,1,FALSE,0, 0, 0);
      gtkut_table_attach_defaults(table, table1, 0, 1, 1, 2);

      label = gtkut_label_new (_("Time Signal"));
      gtk_notebook_append_page (GTK_NOTEBOOK (conf_note), table, label);
    }

#ifdef  USE_BIFF
    // Biff
    i_resource++;
    {
      GtkWidget *entry;
      GtkWidget *table2;
      GtkWidget *frame1;

	// 1x5のテーブル
      table = gtkut_table_new (1, 5, FALSE, 0, 0, 0);
	
      frame = gtkut_frame_new (_("Server Information"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 1, 0, 1,
			 GTK_FILL|GTK_EXPAND,GTK_SHRINK,0,0);
      
      
      
      table1 = gtkut_table_new (1, 3, FALSE, 0, 0, 5);
      gtk_container_add (GTK_CONTAINER (frame), table1);
      
      hbox = gtkut_hbox_new(FALSE, 5);
      gtkut_table_attach_defaults(table1, hbox, 0, 1, 0, 1);
      
      
      label = gtkut_label_new (_("Protocol"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,0);
      
      {
	GtkListStore *store;
	GtkTreeIter iter, iter_set;	  
	GtkCellRenderer *renderer;
	
	store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("None"),
			   1, MAIL_NO, -1);
	if(mascot->mail.type==MAIL_NO) iter_set=iter;
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("POP3 (normal)"),
			   1, MAIL_POP3, -1);
	if(mascot->mail.type==MAIL_POP3) iter_set=iter;
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("POP3 (APOP auth)"),
			   1, MAIL_APOP, -1);
	if(mascot->mail.type==MAIL_APOP) iter_set=iter;
	
#ifndef USE_WIN32
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Local mbox"),
			   1, MAIL_LOCAL, -1);
	if(mascot->mail.type==MAIL_LOCAL) iter_set=iter;
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("qmail (Maildir)"),
			   1, MAIL_QMAIL, -1);
	if(mascot->mail.type==MAIL_QMAIL) iter_set=iter;
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("MH + Procmail"),
			   1, MAIL_PROCMAIL, -1);
	if(mascot->mail.type==MAIL_PROCMAIL) iter_set=iter;
#endif
	
	combo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
	gtk_box_pack_start(GTK_BOX(hbox),combo,FALSE,FALSE,0);
	g_object_unref(store);
	
	renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo),renderer, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT(combo), renderer, "text",0,NULL);
	
	
	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo),&iter_set);
	gtk_widget_show(combo);
	my_signal_connect (combo,"changed",cc_get_combo_box,
			   &mascot->mail.type);
      }
      
      label = gtkut_label_new("");
      gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,TRUE,0);
      
      
      
      frame1 = gtkut_frame_new ("POP3");
      gtk_container_set_border_width (GTK_CONTAINER (frame1), 5);
      gtkut_table_attach(table1, frame1, 0, 1, 1, 2,
			 GTK_FILL|GTK_EXPAND,GTK_SHRINK,0,0);
      
      table2 = gtkut_table_new (4, 4, FALSE, 5, 5, 5);
      gtk_container_add (GTK_CONTAINER (frame1), table2);

      label = gtkut_label_new (_("Address"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtkut_table_attach(table2, label, 0, 1, 0, 1,0,0,0,0);
      
      entry = gtk_entry_new ();
      if(mascot->mail.pop_server){
	gtk_entry_set_text(GTK_ENTRY(entry), mascot->mail.pop_server);
      }
      my_signal_connect (entry,"changed",cc_get_entry,
			 &mascot->mail.pop_server);
      gtkut_table_attach (table2, entry,
			  1, 3, 0, 1,
			  GTK_EXPAND | GTK_FILL,
			  GTK_EXPAND | GTK_FILL,0,0);
      
      
      label = gtkut_label_new (_("User ID"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtkut_table_attach(table2, label, 0, 1, 1, 2,0,0,0,0);
      
      entry = gtk_entry_new ();
      if(mascot->mail.pop_id){
	gtk_entry_set_text(GTK_ENTRY(entry), mascot->mail.pop_id);
      }
      my_signal_connect (entry,"changed",cc_get_entry,
			 &mascot->mail.pop_id);
      gtkut_table_attach (table2, entry,
			  1, 2, 1, 2,
			  GTK_EXPAND | GTK_SHRINK | GTK_FILL,
			  GTK_EXPAND | GTK_SHRINK | GTK_FILL,0,0);
      
      
      
      label = gtkut_label_new (_("   Password"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtkut_table_attach(table2, label, 2, 3, 1, 2,0,0,0,0);
      
      entry = gtk_entry_new ();
      gtk_entry_set_visibility(GTK_ENTRY(entry), FALSE);
      if(mascot->mail.pop_save){
	if(mascot->mail.pop_pass){
	  gtk_entry_set_text(GTK_ENTRY(entry), mascot->mail.pop_pass);
	}
      }
      my_signal_connect (entry,"changed",cc_get_entry,
			 &mascot->mail.pop_pass);
      gtkut_table_attach (table2, entry,
			  3, 4, 1, 2,
			  GTK_EXPAND | GTK_SHRINK | GTK_FILL,
			  GTK_EXPAND | GTK_SHRINK | GTK_FILL,0,0);
      
      
      hbox = gtkut_hbox_new(FALSE,5);
      gtkut_table_attach_defaults(table2, hbox, 3, 4, 2, 3);
      
      label = gtkut_label_new ("");
      gtk_box_pack_start(GTK_BOX(hbox), label,TRUE, TRUE, 5);
      
      check = gtk_check_button_new_with_label(_("Save Password"));
      gtk_box_pack_start(GTK_BOX(hbox), check,FALSE, FALSE, 5);
      if(mascot->mail.pop_save){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      my_signal_connect (check, "toggled",cc_get_toggle,
			 &mascot->mail.pop_save);
      
      
      
#ifdef USE_SSL
      
      label = gtkut_label_new ("SSL");
      gtkut_pos(label, POS_END, POS_CENTER);
      gtkut_table_attach(table2, label, 0, 1, 2, 3,0,0,0,0);
      
      {
	GtkListStore *store;
	GtkTreeIter iter, iter_set;	  
	GtkCellRenderer *renderer;
	
	store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("None"),
			   1, SSL_NONE, -1);
	if(mascot->mail.ssl_mode==SSL_NONE) iter_set=iter;
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Use SSL for POP3 Connection"),
			   1, SSL_TUNNEL, -1);
	if(mascot->mail.ssl_mode==SSL_TUNNEL) iter_set=iter;
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Use STARTTLS to start SSL session"),
			   1, SSL_STARTTLS, -1);
	if(mascot->mail.ssl_mode==SSL_STARTTLS) iter_set=iter;
	
	
	combo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
	gtkut_table_attach_defaults(table2, combo, 1, 3, 2, 3);
	g_object_unref(store);
	
	renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo),renderer, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT(combo), renderer, "text",0,NULL);
	
	
	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo),&iter_set);
	gtk_widget_show(combo);
	my_signal_connect (combo,"changed",cc_mail_ssl, (gpointer)mascot);
      }
      
      hbox = gtkut_hbox_new(FALSE,5);
      gtkut_table_attach_defaults(table2, hbox, 1, 4, 3, 4);
      
      check = gtk_check_button_new_with_label(_("Skip to Verify on SSL Certification"));
      gtk_box_pack_start(GTK_BOX(hbox), check,FALSE, FALSE, 0);
      if(mascot->mail.ssl_cert_skip){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      my_signal_connect (check, "toggled",cc_get_toggle,
			 &mascot->mail.ssl_cert_skip);
      
      check = gtk_check_button_new_with_label(_("Non-Blocking SSL"));
      gtk_box_pack_start(GTK_BOX(hbox), check,FALSE, FALSE, 0);
      if(mascot->mail.ssl_nonblock){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      my_signal_connect (check, "toggled",cc_get_toggle,
			 &mascot->mail.ssl_nonblock);
#endif
      
      hbox = gtkut_hbox_new(FALSE,5);
      gtkut_table_attach_defaults(table2, hbox, 3, 4, 0, 1);
      
      label = gtkut_label_new ("");
      gtk_box_pack_start(GTK_BOX(hbox), label,TRUE, TRUE, 5);
      
      label = gtkut_label_new (_("   Port No."));
      gtkut_pos(label, POS_END, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 5);
      
      adj_pop_port = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->mail.pop_port, 1, 65535, 1.0, 100.0, 0.0);
      my_signal_connect (adj_pop_port, "value_changed",cc_get_adj,
			 &mascot->mail.pop_port);
      spinner =  gtk_spin_button_new (adj_pop_port, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtk_box_pack_start(GTK_BOX(hbox), spinner,FALSE, FALSE, 0);
      
      
      
      frame1 = gtkut_frame_new (_("Local mbox / Maildir / .procmailrc"));
      gtk_container_set_border_width (GTK_CONTAINER (frame1), 5);
      gtkut_table_attach(table1, frame1, 0, 1, 2, 3,
			 GTK_FILL|GTK_EXPAND,GTK_SHRINK,0,0);
      
      table2 = gtkut_table_new (5, 1, FALSE, 0, 0, 5);
      gtk_container_add (GTK_CONTAINER (frame1), table2);


      {
	GtkWidget *hbox2;
	
	hbox2 = gtkut_hbox_new(FALSE, 0);
	gtkut_table_attach_defaults (table2, hbox2,
				     1, 5, 0, 1);
	
	label = gtkut_label_new (_("File / Directory"));
	gtkut_pos(label, POS_END, POS_CENTER);
	gtk_box_pack_start(GTK_BOX(hbox2), label,FALSE, FALSE, 5);
	
	entry_mail_file = gtk_entry_new ();
	gtk_box_pack_start(GTK_BOX(hbox2), entry_mail_file,TRUE, TRUE, 0);
	if(mascot->mail.file){
	  gtk_entry_set_text(GTK_ENTRY(entry_mail_file), mascot->mail.file);
	}
	my_signal_connect (entry_mail_file,"changed",cc_get_entry,
			   &mascot->mail.file);


	button=gtkut_button_new_with_icon(NULL,
#ifdef USE_GTK3				       
					  "document-open"
#else
					  GTK_STOCK_OPEN
#endif
					  );
	gtk_box_pack_start(GTK_BOX(hbox2), button,FALSE, FALSE, 0);
	my_signal_connect (button,"clicked",create_mail_file_dialog,
			   (gpointer)mascot);
      }
      
      
      
      frame = gtkut_frame_new (_("Operating Information"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 1, 1, 2,
			 GTK_FILL|GTK_EXPAND,GTK_SHRINK,0,0);
      
      
      
      table1 = gtkut_table_new (1, 3, FALSE, 5, 5, 5);
      gtk_container_add (GTK_CONTAINER (frame), table1);
	
      hbox = gtkut_hbox_new(FALSE,5);
      gtkut_table_attach_defaults(table1, hbox, 0, 1, 0, 1);
      
      label = gtkut_label_new (_("Interval[sec]"));
      gtkut_pos(label, POS_END, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->mail.interval, 10, 3600, 10.0, 10.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->mail.interval);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtk_box_pack_start(GTK_BOX(hbox), spinner,FALSE, FALSE, 0);
      
      label = gtkut_label_new (_("  Mailer"));
      gtkut_pos(label, POS_END, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      
      {
	GtkWidget *hbox2;
	
	hbox2 = gtkut_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(hbox), hbox2,TRUE, TRUE, 0);
	
	entry_mail_mailer = gtk_entry_new ();
	if(mascot->mail.mailer){
	  gtk_entry_set_text(GTK_ENTRY(entry_mail_mailer), mascot->mail.mailer);
	}
	my_signal_connect (entry_mail_mailer,"changed",cc_get_entry,
			   &mascot->mail.mailer);
	gtk_box_pack_start(GTK_BOX(hbox2), entry_mail_mailer,TRUE, TRUE, 0);
	button=gtkut_button_new_with_icon(NULL,
#ifdef USE_GTK3				       
					  "document-open"
#else
					  GTK_STOCK_OPEN
#endif
					  );
	gtk_box_pack_start(GTK_BOX(hbox2), button,FALSE, FALSE, 0);
	my_signal_connect (button,"clicked",create_mailer_dialog,
			   (gpointer)mascot);
      }
	

      
      hbox = gtkut_hbox_new(FALSE,5);
      gtkut_table_attach_defaults(table1, hbox, 0, 1, 1, 2);
      
      label = gtkut_label_new (_("Polling command"));
      gtkut_pos(label, POS_END, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      
      entry = gtk_entry_new ();
      if(mascot->mail.polling){
	gtk_entry_set_text(GTK_ENTRY(entry), mascot->mail.polling);
      }
      my_signal_connect (entry,"changed",cc_get_entry,
			 &mascot->mail.polling);
      gtk_box_pack_start(GTK_BOX(hbox), entry,TRUE, TRUE, 0);
      
      hbox = gtkut_hbox_new(FALSE,5);
      gtkut_table_attach_defaults(table1, hbox, 0, 1, 2, 3);
      
      label = gtkut_label_new (_("Max Mails for POP Scan"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->mail.pop_max_fs, 10, 999, 10.0, 10.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->mail.pop_max_fs);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtk_box_pack_start(GTK_BOX(hbox), spinner,FALSE, FALSE, 0);
      
      label = gtkut_label_new ("   ");
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      
      check = gtk_check_button_new_with_label(_("Tooltips on biff pix"));
      gtk_box_pack_start(GTK_BOX(hbox), check,FALSE, FALSE, 0);
      if(mascot->mail.tooltips_fl){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      my_signal_connect (check, "toggled",cc_get_toggle,
			 &mascot->mail.tooltips_fl);
      
      
      
      frame = gtkut_frame_new (_("SPAM exception"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 1, 2, 3,
			 GTK_FILL|GTK_EXPAND,GTK_SHRINK,0,0);
      
      table1 = gtkut_table_new (4, 1, FALSE, 0, 0, 5);
      gtk_container_add (GTK_CONTAINER (frame), table1);
      
      check = gtk_check_button_new_with_label(_("Use exception"));
      gtkut_table_attach(table1, check, 0, 1, 0, 1,
			 GTK_SHRINK,
			 GTK_EXPAND | GTK_FILL,0,0);
      if(mascot->mail.spam_check){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      my_signal_connect (check, "toggled",cc_get_toggle,
			 &mascot->mail.spam_check);
      
      label = gtkut_label_new ("        ");
      gtkut_pos(label, POS_END, POS_CENTER);
      gtkut_table_attach(table1, label, 1, 2, 0, 1, 
			 GTK_SHRINK,
			 GTK_EXPAND | GTK_FILL,0,0);
      
      label = gtkut_label_new (_("Mark in Header"));
      gtkut_pos(label, POS_END, POS_CENTER);
      gtkut_table_attach(table1, label, 2, 3, 0, 1, 
			 GTK_SHRINK,
			 GTK_EXPAND | GTK_FILL,0,0);
      
      
      {
	GtkListStore *store;
	GtkTreeIter iter;	  
	
	store = gtk_list_store_new(1, G_TYPE_STRING);
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, SPAM_M_SPAMASSASIN,-1);
	gtk_tree_model_iter_n_children(GTK_TREE_MODEL(store), NULL);
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, SPAM_M_POPFILE,  -1);
	gtk_tree_model_iter_n_children(GTK_TREE_MODEL(store), NULL);
	
	combo = gtk_combo_box_new_with_model_and_entry(GTK_TREE_MODEL(store));
	gtkut_table_attach (table1, combo,
			    3, 4, 0, 1,
			    GTK_FILL|GTK_EXPAND,
			    GTK_SHRINK,0,0);
	
	g_object_unref(store);
	
	
	if(mascot->mail.spam_mark){
	  gtk_entry_set_text(GTK_ENTRY(gtk_bin_get_child(GTK_BIN(combo))), 
			     mascot->mail.spam_mark);
	}
	
	gtk_widget_show(combo);
	my_signal_connect (gtk_bin_get_child(GTK_BIN(combo)),
			   "changed",
			   cc_get_entry,
			   &mascot->mail.spam_mark);
	my_signal_connect (gtk_bin_get_child(GTK_BIN(combo)),"changed",cc_get_spam_combo,
			   (gpointer)mascot);
      }
      
      frame = gtkut_frame_new (_("Mail List Window"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 1, 3, 4,
			 GTK_FILL|GTK_EXPAND,GTK_SHRINK,0,0);
      
      
      table1 = gtkut_table_new (1, 1, FALSE, 0, 0, 5);
      gtk_container_add (GTK_CONTAINER (frame), table1);
      
      hbox = gtkut_hbox_new(FALSE,5);
      gtkut_table_attach_defaults(table1, hbox, 0, 1, 0, 1);
	
      label = gtkut_label_new (_("Width"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->mail.win_width, 100.0, (gdouble)mascot->width_root, 10.0, 10.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->mail.win_width);
      scale =  gtkut_hscale_new (GTK_ADJUSTMENT(adj));
      gtk_scale_set_digits (GTK_SCALE (scale), 0);
      gtk_scale_set_draw_value (GTK_SCALE (scale), TRUE);
      gtk_box_pack_start(GTK_BOX(hbox), scale,TRUE, TRUE, 0);

      label = gtkut_label_new (_("Height"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->mail.win_height, 100.0, (gdouble)mascot->height_root, 10.0, 10.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->mail.win_height);
      scale =  gtkut_hscale_new (GTK_ADJUSTMENT(adj));
      gtk_scale_set_digits (GTK_SCALE (scale), 0);
      gtk_scale_set_draw_value (GTK_SCALE (scale), TRUE);
      gtk_box_pack_start(GTK_BOX(hbox), scale,TRUE, TRUE, 0);


      table1=gtkut_table_new(1,1,FALSE, 0, 0, 0);
      gtkut_table_attach_defaults(table, table1, 0, 1, 4, 5);

      label = gtkut_label_new (_("Biff"));
      gtk_notebook_append_page (GTK_NOTEBOOK (conf_note), table, label);
    }
#endif  // USE_BIFF


    // 描画 (Drawring)
    i_resource++;
    {
      GtkWidget *entry;
      GtkWidget *frame1;
      
      // 1x5のテーブル
      table = gtkut_table_new (1, 3, FALSE, 0, 0, 0);

      frame = gtkut_frame_new (_("Mascot Magnification"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 1, 0, 1,
			 GTK_FILL,GTK_SHRINK,0,0);
	
      table1 = gtkut_table_new (2, 2, FALSE, 5, 5, 5);
      gtk_container_add (GTK_CONTAINER (frame), table1);

      label = gtkut_label_new (_("Magnification[%]"));
      gtkut_pos(label, POS_START, POS_END);
      gtkut_table_attach(table1, label, 0, 1, 0, 1,
			 GTK_SHRINK,GTK_SHRINK,0,0);

      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->magnify, 10, 400, 1.0, 10.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->magnify);
      scale =  gtkut_hscale_new (GTK_ADJUSTMENT(adj));
      gtk_scale_set_digits (GTK_SCALE (scale), 0);
      gtk_scale_set_draw_value (GTK_SCALE (scale), TRUE);
      gtkut_table_attach(table1, scale, 1, 2, 0, 1,
			 GTK_FILL|GTK_EXPAND,GTK_SHRINK,0,0);

      label = gtkut_label_new (_("Interpolation Style"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtkut_table_attach(table1, label, 0, 1, 1, 2,
			 GTK_SHRINK,GTK_SHRINK,0,0);

      {
	GtkListStore *store;
	GtkTreeIter iter, iter_set;	  
	GtkCellRenderer *renderer;
	
	store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Nearest-Neighbor"),
			   1, MAG_IP_NEAREST, -1);
	if(mascot->ip_style==MAG_IP_NEAREST) iter_set=iter;

	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Tiles"),
			   1, MAG_IP_TILES, -1);
	if(mascot->ip_style==MAG_IP_TILES) iter_set=iter;

	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Bilinear"),
			   1, MAG_IP_BILINEAR, -1);
	if(mascot->ip_style==MAG_IP_BILINEAR) iter_set=iter;

	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Hyper"),
			   1, MAG_IP_HYPER, -1);
	if(mascot->ip_style==MAG_IP_HYPER) iter_set=iter;
	
	combo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
	gtkut_table_attach(table1, combo, 1, 2, 1, 2,
			   GTK_FILL|GTK_EXPAND,GTK_SHRINK,0,0);
	g_object_unref(store);
	
	renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo),renderer, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT(combo), renderer, "text",0,NULL);
	
	
	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo),&iter_set);
	gtk_widget_show(combo);
	my_signal_connect (combo,"changed",cc_get_combo_box,
			   &mascot->ip_style);
      }


      frame = gtkut_frame_new (_("Graphic lib. for Image Rendering (Cairo enables trancelucency w/composited desktop)"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 1, 1, 2,
			 GTK_FILL,GTK_SHRINK,0,0);

      vbox = gtkut_vbox_new(FALSE,5);
      gtk_container_set_border_width (GTK_CONTAINER (vbox), 5);
      gtk_container_add (GTK_CONTAINER (frame), vbox);

      frame1 = gtkut_frame_new (_("Current Desktop"));
      gtk_container_set_border_width (GTK_CONTAINER (frame1), 5);
      gtk_box_pack_start(GTK_BOX(vbox), frame1,TRUE, FALSE, 5);

      hbox = gtkut_hbox_new(FALSE,5);
      gtk_container_set_border_width (GTK_CONTAINER (hbox), 10);
      gtk_container_add (GTK_CONTAINER (frame1), hbox);

#ifdef USE_WIN32      
      label = gtkut_label_new (_("Translucency is partly supported by Windows native graphics."));
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      gtkut_pos(label, POS_CENTER, POS_CENTER);
#else
      switch(mascot->flag_composite){
      case COMPOSITE_FALSE:
	label = gtkut_label_new (_("Current system does not support translucent windows."));
	gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 5);
	gtkut_pos(label, POS_CENTER, POS_CENTER);
	break;
      case COMPOSITE_TRUE:
	label = gtkut_label_new (_("Current system support translucent windows."));
	gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 5);
	gtkut_pos(label, POS_CENTER, POS_CENTER);
	break;
      case COMPOSITE_UNKNOWN:
	label = gtkut_label_new (_("Translucent support is UNKNOWN.    "));
	gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 5);
	gtkut_pos(label, POS_CENTER, POS_CENTER);

	check = gtk_check_button_new_with_label(_("Forced translucent rendering"));
	gtk_box_pack_start(GTK_BOX(hbox), check,FALSE, FALSE, 5);
	if(mascot->force_composite){
	  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
	}
	my_signal_connect (check, "toggled",cc_get_toggle,
			   &mascot->force_composite);

	break;
      }
#endif

      check = gtk_check_button_new_with_label(_("Use Cairo for Mascot and Biff image rendering (Support anti-aliased outline)."));
      gtk_box_pack_start(GTK_BOX(vbox), check,FALSE, FALSE, 0);
      if(mascot->flag_img_cairo){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      my_signal_connect (check, "toggled",cc_get_toggle_sdw,
			 &mascot->flag_img_cairo);
      gtk_widget_set_sensitive(check,FALSE);

      {
	GtkWidget *vbox_sdw;
	GtkWidget *hbox_sdw;

	frame_sdw = gtkut_frame_new (_("Shadow of Mascot"));
	gtk_container_set_border_width (GTK_CONTAINER (frame_sdw), 5);
	gtk_box_pack_start(GTK_BOX(vbox), frame_sdw,FALSE, FALSE, 0);

	vbox_sdw = gtkut_vbox_new(FALSE,5);
	gtk_container_set_border_width (GTK_CONTAINER (vbox_sdw), 0);
	gtk_container_add (GTK_CONTAINER (frame_sdw), vbox_sdw);

	hbox_sdw = gtkut_hbox_new(FALSE,5);
	gtk_container_set_border_width (GTK_CONTAINER (hbox_sdw), 5);
	gtk_box_pack_start(GTK_BOX(vbox_sdw), hbox_sdw,FALSE, FALSE, 0);

	check = gtk_check_button_new_with_label(_("Drop Shadow of Mascot"));
	gtk_box_pack_start(GTK_BOX(hbox_sdw), check,FALSE, FALSE, 0);
	if(mascot->sdw_flag){
	  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
	}
	my_signal_connect (check, "toggled",cc_get_toggle,
			   &mascot->sdw_flag);


	label = gtkut_label_new (_("    Offset  X"));
	gtk_box_pack_start(GTK_BOX(hbox_sdw), label,FALSE, FALSE, 5);
	gtkut_pos(label, POS_END, POS_CENTER);

#ifdef FG_DRAW
	adj = (GtkAdjustment *)gtk_adjustment_new 
	  ((gdouble)mascot->sdw_x, -5.0, 5.0, 1.0, 1.0, 0.0);
	spinner =  gtk_spin_button_new (adj, 0, 0);
#else
	adj = (GtkAdjustment *)gtk_adjustment_new 
	  ((gdouble)mascot->sdw_x, -5.0, 5.0, 0.1, 0.1, 0.0);
	spinner =  gtk_spin_button_new (adj, 1, 1);
#endif
	gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
	gtk_box_pack_start(GTK_BOX(hbox_sdw), spinner,FALSE, FALSE, 0);
	my_signal_connect (adj, "value_changed",cc_get_double,
			   &mascot->sdw_x);

	label = gtkut_label_new (_("Y"));
	gtk_box_pack_start(GTK_BOX(hbox_sdw), label,FALSE, FALSE, 5);
	gtkut_pos(label, POS_END, POS_CENTER);

#ifdef FG_DRAW
	adj = (GtkAdjustment *)gtk_adjustment_new 
	  (mascot->sdw_y, 0.0, 5.0, 1.0, 1.0, 0.0);
	spinner =  gtk_spin_button_new (adj, 0, 0);
#else
	adj = (GtkAdjustment *)gtk_adjustment_new 
	  (mascot->sdw_y, 0.0, 5.0, 0.1, 0.1, 0.0);
	spinner =  gtk_spin_button_new (adj, 1, 1);
#endif
	gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
	gtk_box_pack_start(GTK_BOX(hbox_sdw), spinner,FALSE, FALSE, 0);
	my_signal_connect (adj, "value_changed",cc_get_double,
			   &mascot->sdw_y);

	hbox_sdw = gtkut_hbox_new(FALSE,5);
	gtk_container_set_border_width (GTK_CONTAINER (hbox_sdw), 5);
	gtk_box_pack_start(GTK_BOX(vbox_sdw), hbox_sdw,FALSE, FALSE, 0);

	label = gtkut_label_new (_("Opacity [%]"));
	gtk_box_pack_start(GTK_BOX(hbox_sdw), label,FALSE, FALSE, 5);
	gtkut_pos(label, POS_END, POS_CENTER);

	adj = (GtkAdjustment *)gtk_adjustment_new 
	  ((gdouble)mascot->sdw_alpha, 0, 100, 10.0, 10.0, 0.0);
	my_signal_connect (adj, "value_changed",cc_get_adj,
			   &mascot->sdw_alpha);
	scale =  gtkut_hscale_new (GTK_ADJUSTMENT(adj));
	gtk_scale_set_digits (GTK_SCALE (scale), 0);
	gtk_scale_set_draw_value (GTK_SCALE (scale), TRUE);
	gtk_box_pack_start(GTK_BOX(hbox_sdw), scale,TRUE, TRUE, 5);

	
#ifndef FG_DRAW
	if(!mascot->flag_img_cairo){
	  gtk_widget_set_sensitive(frame_sdw,FALSE);
	}
#endif
      }


#ifdef USE_WIN32
      check = gtk_check_button_new_with_label(_("Use Cairo for Panel Clock rendering."));
#else
      check = gtk_check_button_new_with_label(_("Support translucent panel clock (by Cairo)."));
#endif
      gtk_box_pack_start(GTK_BOX(vbox), check,FALSE, FALSE, 0);
      if(mascot->flag_clk_cairo){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      gtk_widget_set_sensitive(check,FALSE);
      my_signal_connect (check, "toggled",cc_get_toggle,
			 &mascot->flag_clk_cairo);
      
#ifdef USE_WIN32
      check = gtk_check_button_new_with_label(_("Use Cairo for Balloon rendering."));
#else
      check = gtk_check_button_new_with_label(_("Support translucent balloon (by Cairo)."));
#endif
      gtk_box_pack_start(GTK_BOX(vbox), check,FALSE, FALSE, 0);
      if(mascot->flag_bal_cairo){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      gtk_widget_set_sensitive(check,FALSE);
      my_signal_connect (check, "toggled",cc_get_toggle,
			 &mascot->flag_bal_cairo);
    
      table1=gtkut_table_new(1,1,FALSE, 0, 0, 0);
      gtkut_table_attach_defaults(table, table1, 0, 1, 2, 3);

      label = gtkut_label_new (_("Rendering"));

      gtk_notebook_append_page (GTK_NOTEBOOK (conf_note), table, label);

    }

    // フォント/色
    i_resource++;
    {
      GtkWidget *entry;
      
      // 1x5のテーブル
      table = gtkut_table_new (1, 5, FALSE, 0, 0, 0);
      

      frame = gtkut_frame_new (_("Default for Clock"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 1, 0, 1,
			 GTK_FILL|GTK_EXPAND,GTK_SHRINK,0,0);

      table1=gtkut_table_new(4,2, FALSE, 0, 5, 5);
      gtk_container_add (GTK_CONTAINER (frame), table1);

      hbox = gtkut_hbox_new(FALSE,0);
      gtkut_table_attach(table1, hbox, 0, 4, 0, 1,
			 GTK_FILL|GTK_EXPAND, GTK_SHRINK,0,0);
	
      label = gtkut_label_new (_("Fontset"));
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);

      button = gtk_font_button_new_with_font(mascot->deffontname_clk);
      gtk_box_pack_start(GTK_BOX(hbox), button,TRUE, TRUE, 0);
      gtk_font_button_set_show_style(GTK_FONT_BUTTON(button),TRUE);
      gtk_font_button_set_use_font(GTK_FONT_BUTTON(button),TRUE);
      gtk_font_button_set_use_size(GTK_FONT_BUTTON(button),TRUE);
      my_signal_connect(button,"font-set",ChangeFontButton, 
      			&mascot->deffontname_clk);

      hbox = gtkut_hbox_new(FALSE,0);
      gtkut_table_attach(table1, hbox, 0, 1, 1, 2,
			 GTK_FILL, GTK_SHRINK,0,0);
      label = gtkut_label_new (_("Text"));
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      gtkut_pos(label, POS_END, POS_CENTER);

#ifdef USE_GTK3
      button = gtk_color_button_new_with_rgba(mascot->def_colclk);
      gtk_color_chooser_set_use_alpha(GTK_COLOR_CHOOSER(button),TRUE);
#else
      button = gtk_color_button_new_with_color(mascot->def_colclk);
      gtk_color_button_set_use_alpha(GTK_COLOR_BUTTON(button),TRUE);
      gtk_color_button_set_alpha(GTK_COLOR_BUTTON(button),
				 (gint)((gdouble)mascot->def_alpclk/100.0*0xFFFF));
#endif
      gtk_box_pack_start(GTK_BOX(hbox), button,FALSE, FALSE, 0);
      my_signal_connect(button,"color-set",ChangeColorButton, 
      			(gpointer)cdata[CONF_DEF_COLOR_CLK]);

      hbox = gtkut_hbox_new(FALSE,0);
      gtkut_table_attach(table1, hbox, 1, 2, 1, 2,
			 GTK_FILL, GTK_SHRINK,0,0);
				  
      label = gtkut_label_new (_("BG"));
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      gtkut_pos(label, POS_END, POS_CENTER);

#ifdef USE_GTK3
      button = gtk_color_button_new_with_rgba(mascot->def_colclkbg);
#ifndef USE_WIN32
      gtk_color_chooser_set_use_alpha(GTK_COLOR_CHOOSER(button),TRUE);
#endif
#else
      button = gtk_color_button_new_with_color(mascot->def_colclkbg);
#ifndef USE_WIN32
      gtk_color_button_set_use_alpha(GTK_COLOR_BUTTON(button),TRUE);
      gtk_color_button_set_alpha(GTK_COLOR_BUTTON(button),
				 (gint)((gdouble)mascot->def_alpclkbg/100.0*0xFFFF));
#endif
#endif
      gtk_box_pack_start(GTK_BOX(hbox), button,FALSE, FALSE, 0);
      my_signal_connect(button,"color-set",ChangeColorButton, 
      			(gpointer)cdata[CONF_DEF_COLOR_CLKBG]);

      hbox = gtkut_hbox_new(FALSE,0);
      gtkut_table_attach(table1, hbox, 2, 3, 1, 2,
			 GTK_FILL, GTK_SHRINK,0,0);
      label = gtkut_label_new (_("Border"));
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      gtkut_pos(label, POS_END, POS_CENTER);

#ifdef USE_GTK3
      button = gtk_color_button_new_with_rgba(mascot->def_colclkbd);
      gtk_color_chooser_set_use_alpha(GTK_COLOR_CHOOSER(button),TRUE);
#else
      button = gtk_color_button_new_with_color(mascot->def_colclkbd);
      gtk_color_button_set_use_alpha(GTK_COLOR_BUTTON(button),TRUE);
      gtk_color_button_set_alpha(GTK_COLOR_BUTTON(button),
				 (gint)((gdouble)mascot->def_alpclkbd/100.0*0xFFFF));
#endif
      gtk_box_pack_start(GTK_BOX(hbox), button,FALSE, FALSE, 0);
      my_signal_connect(button,"color-set",ChangeColorButton, 
      			(gpointer)cdata[CONF_DEF_COLOR_CLKBD]);

      hbox = gtkut_hbox_new(FALSE,0);
      gtkut_table_attach(table1, hbox, 3, 4, 1, 2,
			 GTK_FILL, GTK_SHRINK,0,0);
      label = gtkut_label_new (_("Shadow"));
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      gtkut_pos(label, POS_END, POS_CENTER);

#ifdef USE_GTK3
      button = gtk_color_button_new_with_rgba(mascot->def_colclksd);
      gtk_color_chooser_set_use_alpha(GTK_COLOR_CHOOSER(button),TRUE);
#else
      button = gtk_color_button_new_with_color(mascot->def_colclksd);
      gtk_color_button_set_use_alpha(GTK_COLOR_BUTTON(button),TRUE);
      gtk_color_button_set_alpha(GTK_COLOR_BUTTON(button),
				 (gint)((gdouble)mascot->def_alpclksd/100.0*0xFFFF));
#endif
      gtk_box_pack_start(GTK_BOX(hbox), button,FALSE, FALSE, 0);
      my_signal_connect(button,"color-set",ChangeColorButton, 
      			(gpointer)cdata[CONF_DEF_COLOR_CLKSD]);


      frame = gtkut_frame_new (_("Default for Balloon"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 1, 1, 2,
			 GTK_FILL,GTK_SHRINK,0,0);

      table1=gtkut_table_new(3,2,FALSE, 0, 5, 5);
      gtk_container_add (GTK_CONTAINER (frame), table1);

      hbox = gtkut_hbox_new(FALSE,0);
      gtkut_table_attach_defaults(table1, hbox, 0, 3, 0, 1);

      label = gtkut_label_new (_("Fontset"));
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);

      button = gtk_font_button_new_with_font(mascot->deffontname_bal);
      gtk_box_pack_start(GTK_BOX(hbox), button,TRUE, TRUE, 0);
      gtk_font_button_set_show_style(GTK_FONT_BUTTON(button),TRUE);
      gtk_font_button_set_use_font(GTK_FONT_BUTTON(button),TRUE);
      gtk_font_button_set_use_size(GTK_FONT_BUTTON(button),TRUE);
      my_signal_connect(button,"font-set",ChangeFontButton, 
      			&mascot->deffontname_bal);

      hbox = gtkut_hbox_new(FALSE,0);
      gtkut_table_attach_defaults(table1, hbox, 0, 1, 1, 2);
      label = gtkut_label_new (_("Text"));
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      gtkut_pos(label, POS_START, POS_CENTER);

#ifdef USE_GTK3
      button = gtk_color_button_new_with_rgba(mascot->def_colbal);
      gtk_color_chooser_set_use_alpha(GTK_COLOR_CHOOSER(button),TRUE);
#else
      button = gtk_color_button_new_with_color(mascot->def_colbal);
      gtk_color_button_set_use_alpha(GTK_COLOR_BUTTON(button),TRUE);
      gtk_color_button_set_alpha(GTK_COLOR_BUTTON(button),
				 (gint)((gdouble)mascot->def_alpbal/100.0*0xFFFF));
#endif
      gtk_box_pack_start(GTK_BOX(hbox), button,FALSE, FALSE, 0);
      my_signal_connect(button,"color-set",ChangeColorButton, 
      			(gpointer)cdata[CONF_DEF_COLOR_BAL]);

      hbox = gtkut_hbox_new(FALSE,0);
      gtkut_table_attach_defaults(table1, hbox, 1, 2, 1, 2);
      label = gtkut_label_new (_("BG"));
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      gtkut_pos(label, POS_START, POS_CENTER);

#ifdef USE_GTK3
      button = gtk_color_button_new_with_rgba(mascot->def_colbalbg);
#ifndef FG_DRAW
      gtk_color_chooser_set_use_alpha(GTK_COLOR_CHOOSER(button),TRUE);
#endif
#else
      button = gtk_color_button_new_with_color(mascot->def_colbalbg);
#ifndef FG_DRAW
      gtk_color_button_set_use_alpha(GTK_COLOR_BUTTON(button),TRUE);
      gtk_color_button_set_alpha(GTK_COLOR_BUTTON(button),
				 (gint)((gdouble)mascot->def_alpbalbg/100.0*0xFFFF));
#endif
#endif
      gtk_box_pack_start(GTK_BOX(hbox), button,FALSE, FALSE, 0);     
      my_signal_connect(button,"color-set",ChangeColorButton, 
      			(gpointer)cdata[CONF_DEF_COLOR_BALBG]);

      hbox = gtkut_hbox_new(FALSE,0);
      gtkut_table_attach_defaults(table1, hbox, 2, 3, 1, 2);
      label = gtkut_label_new (_("Border"));
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      gtkut_pos(label, POS_START, POS_CENTER);

#ifdef USE_GTK3
      button = gtk_color_button_new_with_rgba(mascot->def_colbalbd);
      gtk_color_chooser_set_use_alpha(GTK_COLOR_CHOOSER(button),TRUE);
#else
      button = gtk_color_button_new_with_color(mascot->def_colbalbd);
      gtk_color_button_set_use_alpha(GTK_COLOR_BUTTON(button),TRUE);
      gtk_color_button_set_alpha(GTK_COLOR_BUTTON(button),
				 (gint)((gdouble)mascot->def_alpbalbd/100.0*0xFFFF));
#endif
      gtk_box_pack_start(GTK_BOX(hbox), button,FALSE, FALSE, 0);
      my_signal_connect(button,"color-set",ChangeColorButton, 
      			(gpointer)cdata[CONF_DEF_COLOR_BALBD]);


#ifdef USE_WIN32
      frame = gtkut_frame_new (_("Default Opacity"));
#else
      frame = gtkut_frame_new (_("Default Opacity (Enable ONLY w/Compositing Window Managers)"));
#endif
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 1, 2, 3,
			 GTK_FILL,GTK_SHRINK,0,0);

      table1 = gtkut_table_new (4, 5, FALSE, 5, 5, 5);
      gtk_container_add (GTK_CONTAINER (frame), table1);


      check = gtk_check_button_new_with_label(_("Take precedence default opacities over mascot settings"));
      gtkut_table_attach(table1, check, 0, 2, 0, 1,
			 GTK_EXPAND|GTK_FILL, GTK_SHRINK,0,0);
      if(mascot->force_def_alpha){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      my_signal_connect (check, "toggled",cc_get_toggle,
			 &mascot->force_def_alpha);
#ifndef USE_WIN32
      if(mascot->flag_composite==COMPOSITE_FALSE)
	gtk_widget_set_sensitive(check,FALSE);
#endif

      label = gtkut_label_new (_("Mascot [%]"));
      gtkut_pos(label, POS_START, POS_END);
      gtkut_table_attach(table1, label, 0, 1, 1, 2,
			 GTK_SHRINK,GTK_SHRINK,0,0);
#ifdef USE_BIFF
      label = gtkut_label_new (_("Biff Image [%]"));
      gtkut_pos(label, POS_START, POS_END);
      gtkut_table_attach(table1, label, 0, 1, 2, 3,
			 GTK_SHRINK,GTK_SHRINK,0,0);
#endif

#ifdef USE_WIN32
      label = gtkut_label_new (_("Balloon [%]"));
      gtkut_pos(label, POS_START, POS_END);
      gtkut_table_attach(table1, label, 0, 1, 3, 4,
			 GTK_SHRINK,GTK_SHRINK,0,0);

      label = gtkut_label_new (_("Clock [%]"));
      gtkut_pos(label, POS_START, POS_END);
      gtkut_table_attach(table1, label, 0, 1, 4, 5,
			 GTK_SHRINK,GTK_SHRINK,0,0);
#endif

      
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->def_alpha_main, 0, 100, 1.0, 10.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->def_alpha_main);
      scale =  gtkut_hscale_new (GTK_ADJUSTMENT(adj));
      gtk_scale_set_digits (GTK_SCALE (scale), 0);
      gtk_scale_set_draw_value (GTK_SCALE (scale), TRUE);
      gtkut_table_attach(table1, scale, 1, 2, 1, 2,
			 GTK_EXPAND|GTK_FILL,GTK_SHRINK,0,0);
#ifndef USE_WIN32
      if(mascot->flag_composite==COMPOSITE_FALSE)
	gtk_widget_set_sensitive(scale,FALSE);
#endif

#ifdef USE_BIFF
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->def_alpha_biff, 0, 100, 1.0, 10.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->def_alpha_biff);
      scale =  gtkut_hscale_new (GTK_ADJUSTMENT(adj));
      gtk_scale_set_digits (GTK_SCALE (scale), 0);
      gtk_scale_set_draw_value (GTK_SCALE (scale), TRUE);
      gtkut_table_attach(table1, scale, 1, 2, 2, 3,
			 GTK_EXPAND|GTK_FILL,GTK_SHRINK,0,0);
#ifndef USE_WIN32
      if(mascot->flag_composite==COMPOSITE_FALSE){
	gtk_widget_set_sensitive(scale,FALSE);
      }
#endif
#endif

#ifdef FG_DRAW
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->def_alpha_bal, 0, 100, 1.0, 10.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->def_alpha_bal);
      scale =  gtkut_hscale_new (GTK_ADJUSTMENT(adj));
      gtk_scale_set_digits (GTK_SCALE (scale), 0);
      gtk_scale_set_draw_value (GTK_SCALE (scale), TRUE);
      gtkut_table_attach(table1, scale, 1, 2, 3, 4,
			 GTK_EXPAND|GTK_FILL,GTK_SHRINK,0,0);

      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->def_alpha_clk, 0, 100, 1.0, 10.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->def_alpha_clk);
      scale =  gtkut_hscale_new (GTK_ADJUSTMENT(adj));
      gtk_scale_set_digits (GTK_SCALE (scale), 0);
      gtk_scale_set_draw_value (GTK_SCALE (scale), TRUE);
      gtkut_table_attach(table1, scale, 1, 2, 4, 5,
			 GTK_EXPAND|GTK_FILL,GTK_SHRINK,0,0);


      label = gtkut_label_new ("   ");
      gtkut_pos(label, POS_START, POS_END);
      gtkut_table_attach(table1, label, 2, 3, 3, 4,
			 GTK_SHRINK,GTK_SHRINK,0,0);
     
      check = gtk_check_button_new_with_label(_("BG only"));
      gtkut_table_attach(table1, check, 3, 4, 3, 4,
			 GTK_SHRINK,GTK_SHRINK,0,0);
      if(mascot->def_flag_balfg){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      my_signal_connect (check, "toggled",cc_get_toggle,
			 &mascot->def_flag_balfg);

      label = gtkut_label_new ("   ");
      gtkut_pos(label, POS_START, POS_END);
      gtkut_table_attach(table1, label, 2, 3, 4, 5,
			 GTK_SHRINK,GTK_SHRINK,0,0);
     
      check = gtk_check_button_new_with_label(_("BG only"));
      gtkut_table_attach(table1, check, 3, 4, 4, 5,
			 GTK_SHRINK,GTK_SHRINK,0,0);
      if(mascot->def_flag_clkfg){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      my_signal_connect (check, "toggled",cc_get_toggle,
			 &mascot->def_flag_clkfg);
#endif

      table1=gtkut_table_new(1,1,FALSE, 0, 0, 0);
      gtkut_table_attach_defaults(table, table1, 0, 1, 3, 5);


      label = gtkut_label_new (_("Font/Color"));

      gtk_notebook_append_page (GTK_NOTEBOOK (conf_note), table, label);
    }

    // その他
    i_resource++;
    {
      GtkWidget *entry;
      gint y_tbl=0;
      
      // 1x5のテーブル
      table = gtkut_table_new (3, 4, FALSE, 0, 0, 0);
      
#ifdef USE_SOCKMSG
      frame = gtkut_frame_new (_("Duet Animation"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 2, y_tbl, y_tbl+1,
			 GTK_FILL,GTK_SHRINK,0,0);
      table1 = gtkut_table_new (2, 1, FALSE, 5, 5, 5);
      gtk_container_add (GTK_CONTAINER (frame), table1);

      check = gtk_check_button_new_with_label(_("Synchronize with CLICK animation   "));
      gtkut_table_attach(table1, check, 0, 1, 0, 1,
			 GTK_FILL,GTK_SHRINK,0,0);
      if(mascot->duet_use_click){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      my_signal_connect (check, "toggled",cc_get_toggle,
			 &mascot->duet_use_click);

      check = gtk_check_button_new_with_label(_("Synchronize with RANDOM animation"));
      gtkut_table_attach(table1, check, 1, 2, 0, 1,
			 GTK_FILL,GTK_SHRINK,0,0);
      if(mascot->duet_use_random){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      my_signal_connect (check, "toggled",cc_get_toggle,
			 &mascot->duet_use_random);


      y_tbl++;

      frame = gtkut_frame_new (_("Socket Message"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 2, y_tbl, y_tbl+1,
			 GTK_FILL,GTK_SHRINK,0,0);

      table1 = gtkut_table_new (3, 1, FALSE, 5, 0, 5);
      gtk_container_add (GTK_CONTAINER (frame), table1);
      
      hbox = gtkut_hbox_new(FALSE, 0);
      gtkut_table_attach(table1, hbox, 0, 1, 0, 1,
			 GTK_FILL,GTK_SHRINK,0,0);

      label = gtkut_label_new (_("Balloon printing"));
      gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
      gtkut_pos(label, POS_END, POS_CENTER);

      {
	GtkListStore *store;
	GtkTreeIter iter, iter_set;	  
	GtkCellRenderer *renderer;
	
	store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Lump Sum"),
			   1, SOCK_NORMAL, -1);
	if(mascot->sockmsg_type==SOCK_NORMAL) iter_set=iter;
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Stepping"),
			   1, SOCK_STEPPING, -1);
	if(mascot->sockmsg_type==SOCK_STEPPING) iter_set=iter;
	
	combo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
	gtk_box_pack_start(GTK_BOX(hbox), combo,FALSE, FALSE, 3);
	g_object_unref(store);
	
	renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo),renderer, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT(combo), renderer, "text",0,NULL);
	
	
	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo),&iter_set);
	gtk_widget_show(combo);
	my_signal_connect (combo,"changed",cc_get_combo_box,
			   &mascot->sockmsg_type);
      }

      hbox = gtkut_hbox_new(FALSE, 0);
      gtkut_table_attach(table1, hbox, 1, 2, 0, 1,
			 GTK_FILL,GTK_SHRINK,0,0);

      label = gtkut_label_new (_("  Stepping interval [frame]"));
      gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
      gtkut_pos(label, POS_END, POS_CENTER);

      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->sockmsg_step, 0,10, 1.0, 1.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->sockmsg_step);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtk_box_pack_start(GTK_BOX(hbox), spinner,FALSE, FALSE, 3);

      hbox = gtkut_hbox_new(FALSE, 0);
      gtkut_table_attach(table1, hbox, 2, 3, 0, 1,
			 GTK_FILL,GTK_SHRINK,0,0);

      label = gtkut_label_new (_("  Default expiration [ms]"));
      gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
      gtkut_pos(label, POS_END, POS_CENTER);

      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->sockmsg_expire_def, 100, 65535, 100.0, 1000.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->sockmsg_expire_def);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtk_box_pack_start(GTK_BOX(hbox), spinner,FALSE, FALSE, 3);

#endif //USE_SOCKMSG

      y_tbl++;
      frame = gtkut_frame_new (_("Auto Install"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 2, y_tbl, y_tbl+1,
			 GTK_FILL,GTK_SHRINK,0,0);

      table1 = gtkut_table_new (2, 2, FALSE, 5 ,5, 5);
      gtk_container_add (GTK_CONTAINER (frame), table1);

      check = gtk_check_button_new_with_label(_("Auto Mascot Install to User Dir."));
      gtkut_table_attach(table1, check, 0, 2, 0, 1,
			 GTK_FILL,GTK_SHRINK,0,0);
      if(mascot->flag_install){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      my_signal_connect (check, "toggled",cc_get_toggle,
			 &mascot->flag_install);

#ifdef USE_COMMON
      label = gtkut_label_new(_("User & Common Dir Consistency Check"));
      gtkut_table_attach(table1, label, 0, 1, 1, 2,
			 GTK_SHRINK,GTK_SHRINK,0,0);

      {
	GtkListStore *store;
	GtkTreeIter iter, iter_set;	  
	GtkCellRenderer *renderer;
	
	store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Manual"),
			   1, CONS_MANUAL, -1);
	if(mascot->cons_check_mode==CONS_MANUAL) iter_set=iter;

	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Auto Overwrite"),
			   1, CONS_AUTOOW, -1);
	if(mascot->cons_check_mode==CONS_AUTOOW) iter_set=iter;

	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Ignore"),
			   1, CONS_IGNORE, -1);
	if(mascot->cons_check_mode==CONS_IGNORE) iter_set=iter;

	combo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
	gtkut_table_attach_defaults(table1, combo, 1, 2, 1, 2);
	g_object_unref(store);
	
	renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo),renderer, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT(combo), renderer, "text",0,NULL);
	
	
	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo),&iter_set);
	gtk_widget_show(combo);
	my_signal_connect (combo,"changed",cc_get_combo_box,
			   &mascot->cons_check_mode);
      }
#endif


#ifndef USE_WIN32
      y_tbl++;
      frame = gtkut_frame_new (_("Sound Command"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 2, y_tbl, y_tbl+1,
			 GTK_FILL,GTK_SHRINK,0,0);
      
      table1 = gtkut_table_new(2,2,FALSE, 0 ,0, 5);
      gtk_container_add (GTK_CONTAINER (frame), table1);

      entry = gtk_entry_new ();
      if(mascot->sound_command){
	gtk_entry_set_text(GTK_ENTRY(entry),mascot->sound_command);
      }
      gtkut_table_attach_defaults(table1, entry, 0, 2, 0, 1);
      my_signal_connect (entry,"changed",cc_get_entry,
			 &mascot->sound_command);
      
      label = gtkut_label_new(_("(\"%s\" will be translated to the sound file.)"));
      gtkut_table_attach(table1, label, 0, 2, 1, 2,
			 GTK_SHRINK,GTK_SHRINK,0,0);
#endif

#ifdef USE_GTAR
      y_tbl++;
      frame = gtkut_frame_new (_("GNU tar Command [for *.tar.gz Extraction]"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 2, y_tbl, y_tbl+1,
			 GTK_FILL,GTK_SHRINK,0,0);
      
      table1 = gtkut_table_new(2,2,FALSE,0,0,5);
      gtk_container_add (GTK_CONTAINER (frame), table1);

      entry = gtk_entry_new ();
      if(mascot->tar_command){
	gtk_entry_set_text(GTK_ENTRY(entry),mascot->tar_command);
      }
      gtkut_table_attach_defaults(table1, entry, 0, 2, 0, 1);
      my_signal_connect (entry,"changed",cc_get_entry,
			 &mascot->tar_command);

      label = gtkut_label_new(_("(1st \"%s\" : *.tar.gz file, 2nd \"%s\" : temp dir.)"));
      gtkut_table_attach(table1, label, 0, 2, 1, 2,
			 GTK_SHRINK,GTK_SHRINK,0,0);
#endif

#ifdef USE_GTK_STATUS_ICON
      y_tbl++;
      frame = gtkut_frame_new (_("Tray Icon"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 2, y_tbl, y_tbl+1,
			 GTK_FILL,GTK_SHRINK,0,0);
      
      table1 = gtkut_table_new(2,1,FALSE,0,0,5);
      gtk_container_add (GTK_CONTAINER (frame), table1);

      check = gtk_check_button_new_with_label(_("Show Tray Icon in Task Bar"));
      if(mascot->tray_icon_flag){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      my_signal_connect (check, "toggled",cc_get_toggle,
			 &mascot->tray_icon_flag);
      gtkut_table_attach_defaults(table1, check, 0, 2, 0, 1);
#endif

      table1=gtkut_table_new(1,1,FALSE,0,0,0);
      gtkut_table_attach_defaults(table, table1, 
				0, 2, y_tbl+1, y_tbl+2);


      label = gtkut_label_new (_("Misc."));

      gtk_notebook_append_page (GTK_NOTEBOOK (conf_note), table, label);
    }

    // 情報
    i_resource++;
    {
      GtkWidget *entry;
      gchar buf[1024];
#if HAVE_SYS_UTSNAME_H
      struct utsname utsbuf;
#endif
      
      // 1x4のテーブル
      table = gtkut_table_new (1, 4, FALSE,0,0,0);
      

      frame = gtkut_frame_new (_("About This Program"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 1, 0, 1,
			 GTK_FILL,GTK_SHRINK,0,0);

      vbox = gtkut_vbox_new(FALSE,0);
      gtk_container_add (GTK_CONTAINER (frame), vbox);
      gtk_container_set_border_width (GTK_CONTAINER (vbox), 5);
      label = gtkut_label_new ("<span size=\"larger\"><b>MaCoPiX</b> : <b>Mascot Constructive Pilot for X</b></span>   version "VERSION);
      gtkut_pos(label, POS_CENTER, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 0);

      g_snprintf(buf, sizeof(buf),
		 "GTK+ %d.%d.%d / GLib %d.%d.%d",
		 gtk_major_version, gtk_minor_version, gtk_micro_version,
		 glib_major_version, glib_minor_version, glib_micro_version);
      label = gtkut_label_new (buf);
      gtkut_pos(label, POS_CENTER, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 0);

#if HAVE_SYS_UTSNAME_H
      uname(&utsbuf);
      g_snprintf(buf, sizeof(buf),
		 "Operating System: %s %s (%s)",
		 utsbuf.sysname, utsbuf.release, utsbuf.machine);
#elif defined(USE_WIN32)
      g_snprintf(buf, sizeof(buf),
		 "Operating System: %s",
		 WindowsVersion());
#else
      g_snprintf(buf, sizeof(buf),
		 "Operating System: unknown UNIX");
#endif
      label = gtkut_label_new (buf);
      gtkut_pos(label, POS_CENTER, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 0);

      g_snprintf(buf, sizeof(buf),
		 "Compiled-in features : Biff=%s, SSL=%s, SockMsg=%s, Cairo=%s, UNLHA32=%s, TAR32=%s", 
#ifdef USE_BIFF
		 "ON",
#else
		 "OFF",
#endif
#ifdef USE_SSL
#ifdef USE_GNUTLS
		 "GNUTLS",
#else
		 "OpenSSL",
#endif
#else
		 "OFF",
#endif
#ifdef USE_SOCKMSG
		 "ON",
#else
		 "OFF",
#endif
		 "ON",
#ifdef USE_UNLHA32
		 "ON",
#else
                 "OFF",
#endif
#ifdef USE_TAR32
		 "ON"
#else
                 "OFF"
#endif
		 );
      label = gtkut_label_new (buf);
      gtkut_pos(label, POS_CENTER, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 0);

      label = gtkut_label_new ("");
      gtkut_pos(label, POS_CENTER, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 0);

      label = gtkut_label_new ("&#xA9; 2002 K. Chimari &lt;<i>chimari@rosegray.sakura.ne.jp</i>&gt;");
      gtkut_pos(label, POS_CENTER, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 0);

#ifdef USE_WIN32
      hbox = gtkut_hbox_new(FALSE, 0);
      gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

      button = gtk_button_new_with_label(" "DEFAULT_URL" ");
      gtk_box_pack_start(GTK_BOX(vbox), 
			 button, TRUE, FALSE, 0);
      gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE);
      my_signal_connect(button,"clicked",uri_clicked, NULL);
#ifdef USE_GTK3
      css_change_col(gtk_bin_get_child(GTK_BIN(button)),"blue");
#else
      gtk_widget_modify_fg(gtk_bin_get_child(GTK_BIN(button)),
			   GTK_STATE_NORMAL,&color_blue);
#endif
#else
      label = gtkut_label_new (DEFAULT_URL);
      gtkut_pos(label, POS_CENTER, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 0);
#endif 

      frame = gtkut_frame_new (_("Resource File"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 1, 1, 2,
			 GTK_FILL,GTK_SHRINK,0,0);

      table1 = gtkut_table_new(2,1,FALSE,0,0,5);
      gtk_container_add (GTK_CONTAINER (frame), table1);

      entry = gtk_entry_new ();
      if(mascot->rcfile){
	gtk_entry_set_text(GTK_ENTRY(entry),
 	                   to_utf8(mascot->rcfile));
      }
      gtkut_table_attach_defaults(table1, entry, 0, 2, 0, 1);
      

      frame = gtkut_frame_new (_("Menu File"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 1, 2, 3,
			 GTK_FILL,GTK_SHRINK,0,0);

      table1 = gtkut_table_new(2,2,FALSE,5,5,5);
      gtk_container_add (GTK_CONTAINER (frame), table1);

      entry = gtk_entry_new ();
      if(mascot->menu_file){
	gtk_entry_set_text(GTK_ENTRY(entry),
                           to_utf8(mascot->menu_file));
      }
      gtkut_table_attach_defaults(table1, entry, 0, 2, 0, 1);
      
      label = gtkut_label_new (_("Character Code"));
      gtkut_pos(label, POS_CENTER, POS_CENTER);
      gtkut_table_attach(table1, label, 0, 1, 1, 2,
			 GTK_SHRINK,GTK_SHRINK,0,0);

      entry = gtk_entry_new ();
      if(mascot->menu_code){
	gtk_entry_set_text(GTK_ENTRY(entry),mascot->menu_code);
      }
      else{
	gtk_entry_set_text(GTK_ENTRY(entry),_("(Based on the locale environment)"));
      }
      gtkut_table_attach_defaults(table1, entry, 1, 2, 1, 2);


      table1=gtkut_table_new(1,1,FALSE, 0, 0, 0);
      gtkut_table_attach_defaults(table, table1, 0, 1, 3, 4);
    
      label = gtkut_label_new (_("Info."));

      gtk_notebook_append_page (GTK_NOTEBOOK (conf_note), table, label);
    }
  }

  label = gtkut_label_new (_("Resource"));
  gtk_notebook_append_page (GTK_NOTEBOOK (all_note), conf_note, label);
  }

 
  /////// マスコット別の設定 ///////
  {
  GtkWidget *conf_note;
  GtkWidget *label;

  conf_note = gtk_notebook_new ();
  gtk_notebook_set_tab_pos (GTK_NOTEBOOK (conf_note), GTK_POS_TOP);
  gtk_notebook_set_scrollable (GTK_NOTEBOOK (conf_note), TRUE);

  // ノートブックの生成
  {
    gint i_mascot=0;
    GtkWidget *frame;
    GtkWidget *table;
    GtkWidget *label;
    GtkWidget *combo;
    GtkWidget *clabel;
    GtkWidget *table1;
    GtkWidget *table2;
    GtkWidget *check;
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkAdjustment *adj;
    GtkWidget *scale;
    GtkWidget *spinner;
    GtkWidget *button;
    GtkWidget *scrwin;

    
    // 移動設定
    i_mascot++;
    {
      
      // 3x6のテーブル
      table = gtkut_table_new (3, 4, FALSE,0,0,0);

      hbox = gtkut_hbox_new(FALSE,5);
      gtk_container_set_border_width (GTK_CONTAINER (hbox), 5);
      gtkut_table_attach(table, hbox, 0, 2, 0, 1,
			 GTK_FILL,GTK_SHRINK,0,0);
      label = gtkut_label_new (_("Mode"));
      gtkut_pos(label, POS_END, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
  
      {
	GtkListStore *store;
	GtkTreeIter iter, iter_set;	  
	GtkCellRenderer *renderer;
	
	store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Fix Position"),
			   1, MOVE_FIX, -1);
	if(mascot->move==MOVE_FIX) iter_set=iter;

	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Focus Follow"),
			   1, MOVE_FOCUS, -1);
	if(mascot->move==MOVE_FOCUS) iter_set=iter;

	combo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
	gtk_box_pack_start(GTK_BOX(hbox), combo,FALSE, FALSE, 0);
	g_object_unref(store);
	
	renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo),renderer, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT(combo), renderer, "text",0,NULL);
	
	
	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo),&iter_set);
	gtk_widget_show(combo);
	my_signal_connect (combo,"changed",cc_get_combo_box,
			   &mascot->move);
      }

      frame = gtkut_frame_new (_("Fix Position"));
      gtkut_table_attach(table, frame, 0, 2, 1, 2,
			 GTK_FILL,GTK_SHRINK,0,0);
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);

      table1 = gtkut_table_new(2,1,FALSE,0,0,0);
      gtk_container_add (GTK_CONTAINER (frame), table1);
      
      vbox = gtkut_hbox_new(FALSE,0);
      gtk_container_set_border_width (GTK_CONTAINER (vbox), 5);
      label = gtkut_label_new (_("X"));
      gtkut_pos(label, POS_END, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 0);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->xfix, 0, (gdouble)mascot->width_root, 1.0, 1.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,&mascot->xfix);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtk_box_pack_start(GTK_BOX(vbox), spinner,FALSE, FALSE, 0);
      gtkut_table_attach_defaults (table1, vbox, 1, 2, 3, 4);

      vbox = gtkut_hbox_new(FALSE,0);
      gtk_container_set_border_width (GTK_CONTAINER (vbox), 5);
      label = gtkut_label_new (_("Y"));
      gtkut_pos(label, POS_END, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 0);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->yfix, 0, (gdouble)mascot->height_root, 1.0, 1.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,&mascot->yfix);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtk_box_pack_start(GTK_BOX(vbox), spinner,FALSE, FALSE, 0);
      gtkut_table_attach_defaults (table1, vbox, 2, 3, 3, 4);
    
      
    
      frame = gtkut_frame_new (_("Mascot Offset for Focus Follow"));
      gtkut_table_attach (table, frame, 0, 2, 2, 3,
			  GTK_FILL,GTK_SHRINK,0,0);
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);

      table1 = gtkut_table_new(2,1,FALSE,0,0,0);
      gtk_container_add (GTK_CONTAINER (frame), table1);
      
      vbox = gtkut_hbox_new(FALSE,0);
      gtkut_table_attach_defaults (table1, vbox, 0, 1, 0, 1);
      gtk_container_set_border_width (GTK_CONTAINER (vbox), 5);
      label = gtkut_label_new (_("X"));
      gtkut_pos(label, POS_END, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 0);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->xoff, 0, (gdouble)mascot->width_root, 1.0, 1.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,&mascot->xoff);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtk_box_pack_start(GTK_BOX(vbox), spinner,FALSE, FALSE, 0);

      vbox = gtkut_hbox_new(FALSE,0);
      gtkut_table_attach_defaults (table1, vbox, 1, 2, 0, 1);
      gtk_container_set_border_width (GTK_CONTAINER (vbox), 5);
      label = gtkut_label_new (_("Y"));
      gtkut_pos(label, POS_END, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 0);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->yoff, 0, (gdouble)mascot->height_root, 1.0, 1.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,&mascot->yoff);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtk_box_pack_start(GTK_BOX(vbox), spinner,FALSE, FALSE, 0);
   
     
      table1=gtkut_table_new(1,1,FALSE,0,0,0);
      gtkut_table_attach_defaults(table, table1, 0, 2, 3,4);

      label = gtkut_label_new (_("Move"));
      gtk_notebook_append_page (GTK_NOTEBOOK (conf_note), table, label);
    }

    // 時計設定
    i_mascot++;
    {
      GtkWidget *entry;
      
      // 2x4のテーブル
      table = gtkut_table_new (2, 5, FALSE,0,0,0);
      
      hbox = gtkut_hbox_new(FALSE,5);
      gtkut_table_attach(table, hbox, 0, 1, 0, 1,
			 GTK_FILL,GTK_SHRINK,0,0);
      gtk_container_set_border_width (GTK_CONTAINER (hbox), 5);
      label = gtkut_label_new (_("Mode"));
      gtkut_pos(label, POS_END, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);

      {
	GtkListStore *store;
	GtkTreeIter iter, iter_set;	  
	GtkCellRenderer *renderer;
	
	store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("No Clock"),
			   1, CLOCK_NO, -1);
	if(mascot->clkmode==CLOCK_NO) iter_set=iter;

	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Clock on Image"),
			   1, CLOCK_PIXMAP, -1);
	if(mascot->clkmode==CLOCK_PIXMAP) iter_set=iter;

	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Panel Clock"),
			   1, CLOCK_PANEL, -1);
	if(mascot->clkmode==CLOCK_PANEL) iter_set=iter;

	combo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
	gtk_box_pack_start(GTK_BOX(hbox), combo,FALSE, FALSE, 0);
	g_object_unref(store);
	
	renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo),renderer, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT(combo), renderer, "text",0,NULL);
	
	
	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo),&iter_set);
	gtk_widget_show(combo);
	my_signal_connect (combo,"changed",cc_get_combo_box,
			   &mascot->clkmode);
      }

      hbox = gtkut_hbox_new(FALSE,5);
      gtkut_table_attach(table, hbox, 1, 2, 0, 1,
			 GTK_FILL,GTK_SHRINK,0,0);
      gtk_container_set_border_width (GTK_CONTAINER (hbox), 5);
      label = gtkut_label_new (_("Digit Type"));
      gtkut_pos(label, POS_END, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);

      {
	GtkListStore *store;
	GtkTreeIter iter, iter_set;	  
	GtkCellRenderer *renderer;
	
	store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("14:00:00"),
			   1, CLOCK_TYPE_24S, -1);
	if(mascot->clktype==CLOCK_TYPE_24S) iter_set=iter;

	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("14:00"),
			   1, CLOCK_TYPE_24M, -1);
	if(mascot->clktype==CLOCK_TYPE_24M) iter_set=iter;

	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("PM  2:00:00"),
			   1, CLOCK_TYPE_12S, -1);
	if(mascot->clktype==CLOCK_TYPE_12S) iter_set=iter;

	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("PM  2:00"),
			   1, CLOCK_TYPE_12M, -1);
	if(mascot->clktype==CLOCK_TYPE_12M) iter_set=iter;

	combo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
	gtk_box_pack_start(GTK_BOX(hbox), combo,FALSE, FALSE, 0);
	g_object_unref(store);
	
	renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo),renderer, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT(combo), renderer, "text",0,NULL);
	
	
	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo),&iter_set);
	gtk_widget_show(combo);
	my_signal_connect (combo,"changed",cc_get_combo_box,
			   &mascot->clktype);
      }

      frame = gtkut_frame_new (_("Fontset"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 2, 1, 2,
			 GTK_FILL,GTK_SHRINK,0,0);
	
      table1 = gtkut_table_new (2, 1, FALSE,0,0,5);
      gtk_container_add (GTK_CONTAINER (frame), table1);

      button = gtk_font_button_new_with_font(mascot->fontname_clk);
      gtk_font_button_set_show_style(GTK_FONT_BUTTON(button),TRUE);
      gtk_font_button_set_use_font(GTK_FONT_BUTTON(button),TRUE);
      gtk_font_button_set_use_size(GTK_FONT_BUTTON(button),TRUE);
      my_signal_connect(button,"font-set",ChangeFontButton, 
      			&mascot->fontname_clk);
      gtkut_table_attach (table1, button, 0, 2, 0, 1, 
			  GTK_FILL|GTK_EXPAND,GTK_SHRINK,0,0);

      frame = gtkut_frame_new (_("Size/Position"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 1, 2, 3,
			 GTK_FILL,GTK_SHRINK,0,0);
	
      table1 = gtkut_table_new (5, 3, FALSE, 5, 0, 5);
      gtk_container_add (GTK_CONTAINER (frame), table1);
#ifndef USE_GTK3      
      gtk_table_set_col_spacing (GTK_TABLE(table1), 0, 15);
      gtk_table_set_col_spacing (GTK_TABLE(table1), 2, 15);
      gtk_table_set_row_spacing (GTK_TABLE(table1), 0, 5);
      gtk_table_set_row_spacing (GTK_TABLE(table1), 1, 5);
#endif

      label = gtkut_label_new (_("Panel Position"));
      gtkut_table_attach (table1, label, 0, 1, 0, 1,
			  GTK_SHRINK, GTK_SHRINK, 5, 5);
      gtkut_pos(label, POS_START, POS_CENTER);
      label = gtkut_label_new (_("X"));
      gtkut_table_attach (table1, label, 1, 2, 0, 1,
			  GTK_SHRINK, GTK_SHRINK, 5, 5);
      gtkut_pos(label, POS_START, POS_CENTER);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->clk_x, (gdouble)(-mascot->width*3), (gdouble)(mascot->width*4), 1.0, 
	 10.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,&mascot->clk_x);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtkut_table_attach (table1, spinner, 2, 3, 0, 1,
			  GTK_SHRINK, GTK_SHRINK, 5, 5);
      label = gtkut_label_new (_("Y"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtkut_table_attach (table1, label, 3, 4, 0, 1,
			  GTK_SHRINK, GTK_SHRINK, 5, 5);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->clk_y, (gdouble)(-mascot->height*2), (gdouble)(mascot->height*3), 1.0, 
	 10.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,&mascot->clk_y);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtkut_table_attach (table1, spinner, 4, 5, 0, 1,
			  GTK_SHRINK, GTK_SHRINK, 5, 5);

      label = gtkut_label_new (_("Text Offset"));
      gtkut_table_attach (table1, label, 0, 1,1, 2,
			  GTK_SHRINK, GTK_SHRINK, 5, 5);
      gtkut_pos(label, POS_START, POS_CENTER);
      label = gtkut_label_new (_("X"));
      gtkut_pos(label, POS_END, POS_CENTER);
      gtkut_table_attach (table1, label, 1, 2, 1, 2,
			  GTK_SHRINK, GTK_SHRINK, 5, 5);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->clktext_x, 1, (gdouble)mascot->width, 1.0, 10.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->clktext_x);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtkut_table_attach (table1, spinner, 2, 3, 1, 2,
			  GTK_SHRINK, GTK_SHRINK, 5, 5);
      label = gtkut_label_new (_("Y"));
      gtkut_pos(label, POS_END, POS_CENTER);
      gtkut_table_attach (table1, label, 3, 4, 1, 2,
			  GTK_SHRINK, GTK_SHRINK, 5, 5);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->clktext_y, 1, (gdouble)mascot->height, 1.0, 10.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->clktext_y);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtkut_table_attach (table1, spinner, 4, 5, 1, 2,
			  GTK_SHRINK, GTK_SHRINK, 5, 5);

      label = gtkut_label_new (_("Border Width"));
      gtkut_table_attach (table1, label, 0, 1,2, 3,
			  GTK_SHRINK, GTK_SHRINK, 5, 5);
      gtkut_pos(label, POS_START, POS_CENTER);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->wclkbd, 0, 10, 1.0, 1.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->wclkbd);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtkut_table_attach (table1, spinner, 2, 3, 2, 3,
			  GTK_SHRINK, GTK_SHRINK, 5, 5);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);


      frame = gtkut_frame_new (_("Decoration"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 1, 3, 4,
			 GTK_FILL,GTK_SHRINK,0,0);

      vbox = gtkut_vbox_new(FALSE,5);
      gtk_container_add (GTK_CONTAINER (frame), vbox);
      gtk_container_set_border_width (GTK_CONTAINER (vbox), 0);
      
      
      hbox = gtkut_hbox_new(FALSE,5);
      gtk_box_pack_start(GTK_BOX(vbox), hbox,FALSE, FALSE, 0);
      gtk_container_set_border_width (GTK_CONTAINER (hbox), 5);
      
      check = gtk_check_button_new_with_label(_("Drop Shadow"));
      gtk_box_pack_start(GTK_BOX(hbox), check,FALSE, FALSE, 0);
      if(mascot->flag_clksd){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      my_signal_connect (check, "toggled",cc_get_toggle,
			 &mascot->flag_clksd);

      label = gtkut_label_new (_("   Shadow Offset  X"));
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      gtkut_pos(label, POS_END, POS_CENTER);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->clksd_x, -5, 5, 1.0, 10.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->clksd_x);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtk_box_pack_start(GTK_BOX(hbox), spinner,FALSE, FALSE, 0);
      label = gtkut_label_new (_("Y"));
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->clksd_y, -5, 5, 1.0, 10.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->clksd_y);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtk_box_pack_start(GTK_BOX(hbox), spinner,FALSE, FALSE, 0);

      hbox = gtkut_hbox_new(FALSE,5);
      gtk_box_pack_start(GTK_BOX(vbox), hbox,FALSE, FALSE, 0);
      gtk_container_set_border_width (GTK_CONTAINER (hbox), 5);

      check = gtk_check_button_new_with_label(_("Rounded Corner for Panel Clock"));
      gtk_box_pack_start(GTK_BOX(hbox), check,FALSE, FALSE, 0);
      if(mascot->flag_clkrd){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      my_signal_connect (check, "toggled",cc_get_toggle,
			 &mascot->flag_clkrd);

      frame = gtkut_frame_new (_("Color"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 1, 2, 2, 4,
			 GTK_FILL,GTK_SHRINK,0,0);
	
      table1 = gtkut_table_new (2, 6, FALSE,0,5,5);
      gtk_container_add (GTK_CONTAINER (frame), table1);

      label = gtkut_label_new (_("Text"));
      gtkut_table_attach (table1, label, 0, 1,0, 1,
			  GTK_SHRINK, GTK_SHRINK, 5, 5);
      gtkut_pos(label, POS_START, POS_CENTER);

#ifdef USE_GTK3
      button = gtk_color_button_new_with_rgba(mascot->colclk);
      gtk_color_chooser_set_use_alpha(GTK_COLOR_CHOOSER(button),TRUE);
#else
      button = gtk_color_button_new_with_color(mascot->colclk);
      gtk_color_button_set_use_alpha(GTK_COLOR_BUTTON(button),TRUE);
      gtk_color_button_set_alpha(GTK_COLOR_BUTTON(button),
				 (gint)((gdouble)mascot->alpclk/100.0*0xFFFF));
#endif
      gtkut_table_attach (table1, button,1,2,0,1,
			  GTK_SHRINK, GTK_SHRINK, 5, 5);
      my_signal_connect(button,"color-set",ChangeColorButton, 
      			(gpointer)cdata[CONF_COLOR_CLK]);

      label = gtkut_label_new (_("BG"));
      gtkut_table_attach (table1, label, 0, 1,1, 2,
			  GTK_SHRINK, GTK_SHRINK, 5, 5);
      gtkut_pos(label, POS_START, POS_CENTER);

#ifdef USE_GTK3
      button = gtk_color_button_new_with_rgba(mascot->colclkbg);
      gtk_color_chooser_set_use_alpha(GTK_COLOR_CHOOSER(button),TRUE);
#else
      button = gtk_color_button_new_with_color(mascot->colclkbg);
#ifndef USE_WIN32
      gtk_color_button_set_use_alpha(GTK_COLOR_BUTTON(button),TRUE);
      gtk_color_button_set_alpha(GTK_COLOR_BUTTON(button),
				 (gint)((gdouble)mascot->alpclkbg/100.0*0xFFFF));
#endif
#endif
      gtkut_table_attach(table1, button,1,2,1,2,
			 GTK_SHRINK, GTK_SHRINK, 5, 5);
      my_signal_connect(button,"color-set",ChangeColorButton, 
      			(gpointer)cdata[CONF_COLOR_CLKBG]);

      label = gtkut_label_new (_("Border"));
      gtkut_table_attach (table1, label, 0, 1,2, 3,
 			  GTK_SHRINK, GTK_SHRINK, 5, 5);
      gtkut_pos(label, POS_START, POS_CENTER);
      
#ifdef USE_GTK3
      button = gtk_color_button_new_with_rgba(mascot->colclkbd);
      gtk_color_chooser_set_use_alpha(GTK_COLOR_CHOOSER(button),TRUE);
#else
      button = gtk_color_button_new_with_color(mascot->colclkbd);
      gtk_color_button_set_use_alpha(GTK_COLOR_BUTTON(button),TRUE);
      gtk_color_button_set_alpha(GTK_COLOR_BUTTON(button),
				 (gint)((gdouble)mascot->alpclkbd/100.0*0xFFFF));
#endif
      gtkut_table_attach(table1,button,1,2,2,3,
			 GTK_SHRINK, GTK_SHRINK, 5, 5);
      my_signal_connect(button,"color-set",ChangeColorButton, 
      			(gpointer)cdata[CONF_COLOR_CLKBD]);

      label = gtkut_label_new (_("Shadow"));
      gtkut_table_attach (table1, label, 0, 1,3, 4,
			 GTK_SHRINK, GTK_SHRINK, 5, 5);
      gtkut_pos(label, POS_START, POS_CENTER);

#ifdef USE_GTK3
      button = gtk_color_button_new_with_rgba(mascot->colclksd);
      gtk_color_chooser_set_use_alpha(GTK_COLOR_CHOOSER(button),TRUE);
#else
      button = gtk_color_button_new_with_color(mascot->colclksd);
      gtk_color_button_set_use_alpha(GTK_COLOR_BUTTON(button),TRUE);
      gtk_color_button_set_alpha(GTK_COLOR_BUTTON(button),
				 (gint)((gdouble)mascot->alpclksd/100.0*0xFFFF));
#endif
      gtkut_table_attach(table1,button,1,2,3,4,
			 GTK_SHRINK, GTK_SHRINK, 5, 5);
      my_signal_connect(button,"color-set",ChangeColorButton, 
      			(gpointer)cdata[CONF_COLOR_CLKSD]);

      button=gtkut_button_new_with_icon(_("Back to Default"),
#ifdef USE_GTK3				       
					"view-refresh"
#else
					GTK_STOCK_REFRESH
#endif
					);
      gtkut_table_attach (table1, button,0,2,4,5,
			  GTK_FILL|GTK_EXPAND, GTK_SHRINK, 5, 5);
      my_signal_connect(button,"clicked",change_colclk_default, (gpointer)mascot);

#ifdef FG_DRAW
      frame = gtkut_frame_new (_("Opacity"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 2, 4, 5,
			 GTK_FILL,GTK_FILL,0,0);

      table1 = gtkut_table_new (4, 1, FALSE,0,0,5);
      gtk_container_add (GTK_CONTAINER (frame), table1);

      label = gtkut_label_new (_("Opacity Alpha[%]"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtkut_table_attach(table1, label, 0, 1, 0, 1,
			 GTK_FILL,GTK_SHRINK,0,0);
      
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->alpha_clk, 0, 100, 1.0, 10.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->alpha_clk);
      scale =  gtkut_hscale_new (GTK_ADJUSTMENT(adj));
      gtk_scale_set_digits (GTK_SCALE (scale), 0);
      gtk_scale_set_draw_value (GTK_SCALE (scale), TRUE);
      gtkut_table_attach_defaults(table1, scale, 1, 2, 0, 1);

      label = gtkut_label_new ("   ");
      gtkut_pos(label, POS_START, POS_END);
      gtkut_table_attach(table1, label, 2, 3, 0, 1,
			 GTK_SHRINK,GTK_SHRINK,0,0);
     
      check = gtk_check_button_new_with_label(_("BG only"));
      gtkut_table_attach(table1, check, 3, 4, 0, 1,
			 GTK_SHRINK,GTK_SHRINK,0,0);
      if(mascot->flag_clkfg){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      my_signal_connect (check, "toggled",cc_get_toggle,
			 &mascot->flag_clkfg);


      table1=gtkut_table_new(1,1,FALSE,0,0,0);
      gtkut_table_attach_defaults(table, table1, 0, 2, 5, 6);
#else

      table1=gtkut_table_new(1,1,FALSE,0,0,0);
      gtkut_table_attach_defaults(table, table1, 0, 2, 4, 5);
#endif

      label = gtkut_label_new (_("Clock"));

      gtk_notebook_append_page (GTK_NOTEBOOK (conf_note), table, label);
    }

   // ふきだし設定
    i_mascot++;
    {
     
      // 2x4のテーブル
      table = gtkut_table_new (2, 5, FALSE,0,0,0);
      
      frame = gtkut_frame_new (_("Fontset"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 2, 0, 1,
			 GTK_FILL,GTK_SHRINK,0,0);
	

      table1 = gtkut_table_new (2, 1, FALSE,0,0,5);
      gtk_container_add (GTK_CONTAINER (frame), table1);

      button = gtk_font_button_new_with_font(mascot->fontname_bal);
      gtkut_table_attach_defaults (table1,button,
				   0, 2, 0, 1);
      gtk_font_button_set_show_style(GTK_FONT_BUTTON(button),TRUE);
      gtk_font_button_set_use_font(GTK_FONT_BUTTON(button),TRUE);
      gtk_font_button_set_use_size(GTK_FONT_BUTTON(button),TRUE);
      my_signal_connect(button,"font-set",ChangeFontButton, 
      			&mascot->fontname_bal);

      frame = gtkut_frame_new (_("Size/Position"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 1, 1, 2,
			 GTK_FILL,GTK_SHRINK,0,0);
	
      table1 = gtkut_table_new (6, 3, FALSE,5,0,5);
      gtk_container_add (GTK_CONTAINER (frame), table1);
#ifndef USE_GTK3      
      gtk_table_set_col_spacing (GTK_TABLE(table1), 0, 15);
      gtk_table_set_col_spacing (GTK_TABLE(table1), 2, 15);
      gtk_table_set_row_spacing (GTK_TABLE(table1), 0, 5);
      gtk_table_set_row_spacing (GTK_TABLE(table1), 1, 5);
#endif
      
      label = gtkut_label_new (_("Text Offset"));
      gtkut_table_attach (table1, label, 0, 1,0, 1,
			  GTK_SHRINK,GTK_SHRINK, 0, 0);
      gtkut_pos(label, POS_START, POS_CENTER);

      label = gtkut_label_new (_("X"));
      gtkut_pos(label, POS_END, POS_CENTER);
      gtkut_table_attach (table1, label, 1, 2, 0, 1,
			  GTK_SHRINK,GTK_SHRINK,0,0);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->baltext_x, 0, 20, 1.0, 1.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->baltext_x);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtkut_table_attach (table1, spinner, 2, 3, 0, 1,
			  GTK_SHRINK,GTK_SHRINK,0,0);
      label = gtkut_label_new (_("Y"));
      gtkut_pos(label, POS_END, POS_CENTER);
      gtkut_table_attach (table1, label, 3, 4, 0, 1,
			  GTK_SHRINK,GTK_SHRINK,0,0);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->baltext_y, 0, 20, 1.0, 1.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->baltext_y);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtkut_table_attach (table1, spinner, 4, 5, 0, 1,
			  GTK_SHRINK,GTK_SHRINK,0,0);


      label = gtkut_label_new (_("Border Width"));
      gtkut_table_attach (table1, label, 0, 1,1, 2,
			  GTK_SHRINK,GTK_SHRINK,0,0);
      gtkut_pos(label, POS_START, POS_CENTER);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->wbalbd, 0, 10, 1.0, 1.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->wbalbd);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtkut_table_attach (table1, spinner, 2, 3,1, 2,
			  GTK_SHRINK,GTK_SHRINK,0,0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);

      label = gtkut_label_new (_("Default Position"));
      gtkut_table_attach (table1, label,0,1,2, 3,
			  GTK_SHRINK,GTK_SHRINK,0,0);
      gtkut_pos(label, POS_START, POS_CENTER);

      {
	GtkListStore *store;
	GtkTreeIter iter, iter_set;	  
	GtkCellRenderer *renderer;
	
	store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Left side of mascot"),
			   1, BAL_POS_LEFT, -1);
	if(mascot->bal_defpos==BAL_POS_LEFT) iter_set=iter;

	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Right side of mascot"),
			   1, BAL_POS_RIGHT, -1);
	if(mascot->bal_defpos==BAL_POS_RIGHT) iter_set=iter;

	combo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
	gtkut_table_attach (table1, combo, 2, 6, 2, 3,
			    GTK_FILL,GTK_SHRINK,0,0);
	g_object_unref(store);
	
	renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo),renderer, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT(combo), renderer, "text",0,NULL);
	
	
	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo),&iter_set);
	gtk_widget_show(combo);
	my_signal_connect (combo,"changed",cc_get_combo_box,
			   &mascot->bal_defpos);
      }

      frame = gtkut_frame_new (_("Color"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 1, 2, 1, 2,
			 GTK_FILL,GTK_SHRINK,0,0);
	
      table1 = gtkut_table_new (2, 4, FALSE,0,5,5);
      gtk_container_add (GTK_CONTAINER (frame), table1);

      label = gtkut_label_new (_("Text"));
      gtkut_table_attach(table1, label, 0, 1,0, 1,
			  GTK_SHRINK,GTK_SHRINK,0,0);
      gtkut_pos(label, POS_START, POS_CENTER);

#ifdef USE_GTK3
      button = gtk_color_button_new_with_rgba(mascot->colbal);
      gtk_color_chooser_set_use_alpha(GTK_COLOR_CHOOSER(button),TRUE);
#else
      button = gtk_color_button_new_with_color(mascot->colbal);
      gtk_color_button_set_use_alpha(GTK_COLOR_BUTTON(button),TRUE);
      gtk_color_button_set_alpha(GTK_COLOR_BUTTON(button),
				 (gint)((gdouble)mascot->alpbal/100.0*0xFFFF));
#endif
      gtkut_table_attach (table1, button,1,2,0,1,
 			  GTK_SHRINK,GTK_SHRINK,0,0);
      my_signal_connect(button,"color-set",ChangeColorButton, 
      			(gpointer)cdata[CONF_COLOR_BAL]);

      label = gtkut_label_new (_("BG"));
      gtkut_table_attach (table1, label, 0, 1,1, 2,
 			  GTK_SHRINK,GTK_SHRINK,0,0);
      gtkut_pos(label, POS_START, POS_CENTER);

#ifdef USE_GTK3
      button = gtk_color_button_new_with_rgba(mascot->colbalbg);
#ifndef FG_DRAW
      gtk_color_chooser_set_use_alpha(GTK_COLOR_CHOOSER(button),TRUE);
#endif
#else
      button = gtk_color_button_new_with_color(mascot->colbalbg);
#ifndef FG_DRAW
      gtk_color_button_set_use_alpha(GTK_COLOR_BUTTON(button),TRUE);
      gtk_color_button_set_alpha(GTK_COLOR_BUTTON(button),
				 (gint)((gdouble)mascot->alpbalbg/100.0*0xFFFF));
#endif
#endif
      gtkut_table_attach(table1, button,1,2,1,2,
  			  GTK_SHRINK,GTK_SHRINK,0,0);
      my_signal_connect(button,"color-set",ChangeColorButton, 
      			(gpointer)cdata[CONF_COLOR_BALBG]);

      label = gtkut_label_new (_("Border"));
      gtkut_table_attach (table1, label, 0, 1,2, 3,
  			  GTK_SHRINK,GTK_SHRINK,0,0);
      gtkut_pos(label, POS_START, POS_CENTER);

#ifdef USE_GTK3
      button = gtk_color_button_new_with_rgba(mascot->colbalbd);
      gtk_color_chooser_set_use_alpha(GTK_COLOR_CHOOSER(button),TRUE);
#else
      button = gtk_color_button_new_with_color(mascot->colbalbd);
      gtk_color_button_set_use_alpha(GTK_COLOR_BUTTON(button),TRUE);
      gtk_color_button_set_alpha(GTK_COLOR_BUTTON(button),
				 (gint)((gdouble)mascot->alpbalbd/100.0*0xFFFF));
#endif
      gtkut_table_attach(table1,button,1,2,2,3,
			 GTK_SHRINK,GTK_SHRINK,0,0);
      my_signal_connect(button,"color-set",ChangeColorButton, 
      			(gpointer)cdata[CONF_COLOR_BALBD]);

      button=gtkut_button_new_with_icon(_("Back to Default"),
#ifdef USE_GTK3				       
					"view-refresh"
#else
					GTK_STOCK_REFRESH
#endif
					);
      gtkut_table_attach (table1, button,0,2,3,4,
			 GTK_FILL,GTK_SHRINK,0,0);
      my_signal_connect(button,"clicked",change_colbal_default, (gpointer)mascot);

#ifdef FG_DRAW
      frame = gtkut_frame_new (_("Opacity"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 2, 2, 3,
			 GTK_FILL,GTK_FILL,0,0);

      table1 = gtkut_table_new (4, 1, FALSE,0,0,5);
      gtk_container_add (GTK_CONTAINER (frame), table1);

      label = gtkut_label_new (_("Opacity Alpha[%]"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtkut_table_attach(table1, label, 0, 1, 0, 1,
			 GTK_FILL,GTK_SHRINK,0,0);
      
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->alpha_bal, 0, 100, 1.0, 10.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->alpha_bal);
      scale =  gtkut_hscale_new (GTK_ADJUSTMENT(adj));
      gtk_scale_set_digits (GTK_SCALE (scale), 0);
      gtk_scale_set_draw_value (GTK_SCALE (scale), TRUE);
      gtkut_table_attach_defaults(table1, scale, 1, 2, 0, 1);


      label = gtkut_label_new ("   ");
      gtkut_pos(label, POS_START, POS_END);
      gtkut_table_attach(table1, label, 2, 3, 0, 1,
			 GTK_SHRINK,GTK_SHRINK,0,0);
     
      check = gtk_check_button_new_with_label(_("BG only"));
      gtkut_table_attach(table1, check, 3, 4, 0, 1,
			 GTK_SHRINK,GTK_SHRINK,0,0);
      if(mascot->flag_balfg){
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
      }
      my_signal_connect (check, "toggled",cc_get_toggle,
			 &mascot->flag_balfg);
#endif

      frame = gtkut_frame_new (_("Control Characters for Balloon Messages"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);

#ifdef USE_WIN32
      gtkut_table_attach(table, frame, 0, 2, 3, 4,
			 GTK_FILL,GTK_FILL,0,0);
#else
      gtkut_table_attach(table, frame, 0, 2, 2, 3,
			 GTK_FILL,GTK_FILL,0,0);
#endif	
      table1 = gtkut_table_new (2, 7, FALSE,0,2,5);
      gtk_container_add (GTK_CONTAINER (frame), table1);

      label = gtkut_label_new ("%[n]c");
      gtkut_table_attach_defaults (table1, label, 0, 1,0, 1);
      gtkut_pos(label, POS_START, POS_CENTER);
      label = gtkut_label_new (_("([n]+1) frames waiting. (default : n=1)"));
      gtkut_table_attach_defaults (table1, label, 1, 2,0, 1);
      gtkut_pos(label, POS_START, POS_CENTER);

      label = gtkut_label_new ("%[n]a");
      gtkut_table_attach_defaults (table1, label, 0, 1,1, 2);
      gtkut_pos(label, POS_START, POS_CENTER);
      label = gtkut_label_new (_("([n]+1) frames waiting with a new line. (default : n=10)"));
      gtkut_table_attach_defaults (table1, label, 1, 2,1, 2);
      gtkut_pos(label, POS_START, POS_CENTER);

      label = gtkut_label_new ("%n");
      gtkut_table_attach_defaults (table1, label, 0, 1,2, 3);
      gtkut_pos(label, POS_START, POS_CENTER);
      label = gtkut_label_new (_("New line."));
      gtkut_table_attach_defaults (table1, label, 1, 2,2, 3);
      gtkut_pos(label, POS_START, POS_CENTER);

      label = gtkut_label_new ("%[n]p");
      gtkut_table_attach_defaults (table1, label, 0, 1,3, 4);
      gtkut_pos(label, POS_START, POS_CENTER);
      label = gtkut_label_new (_("New page after ([n]+1) frames waiting. (default : n=10)"));
      gtkut_table_attach_defaults (table1, label, 1, 2,3, 4);
      gtkut_pos(label, POS_START, POS_CENTER);

      label = gtkut_label_new ("%m");
      gtkut_table_attach_defaults (table1, label, 0, 1,4, 5);
      gtkut_pos(label, POS_START, POS_CENTER);
      label = gtkut_label_new (_("Number of arrived mails. (only for biff message)"));
      gtkut_table_attach_defaults (table1, label, 1, 2,4, 5);
      gtkut_pos(label, POS_START, POS_CENTER);

      label = gtkut_label_new ("%f");
      gtkut_table_attach_defaults (table1, label, 0, 1,5, 6);
      gtkut_pos(label, POS_START, POS_CENTER);
      label = gtkut_label_new (_("From: of the latest mail. (only for biff message with POP)"));
      gtkut_table_attach_defaults (table1, label, 1, 2,5, 6);
      gtkut_pos(label, POS_START, POS_CENTER);

      label = gtkut_label_new ("%s");
      gtkut_table_attach_defaults (table1, label, 0, 1,6, 7);
      gtkut_pos(label, POS_START, POS_CENTER);
      label = gtkut_label_new (_("Subject: of the latest mail. (only for biff message with POP)"));
      gtkut_table_attach_defaults (table1, label, 1, 2,6, 7);
      gtkut_pos(label, POS_START, POS_CENTER);



#ifdef USE_WIN32
      table1=gtkut_table_new(1,1,FALSE,0,0,0);
      gtkut_table_attach_defaults(table, table1, 0, 2, 4, 5);
#else

      table1=gtkut_table_new(1,1,FALSE,0,0,0);
      gtkut_table_attach_defaults(table, table1, 0, 2, 3, 4);
#endif




    
      label = gtkut_label_new (_("Balloon"));

      gtk_notebook_append_page (GTK_NOTEBOOK (conf_note), table, label);
      }

    // 画像設定
    i_mascot++;
    {
      
      table = gtkut_table_new (2, 4, FALSE,0,0,0);
      
#ifdef USE_WIN32
      frame = gtkut_frame_new (_("Opacity"));
#else
      frame = gtkut_frame_new (_("Opacity (Enable ONLY w/Compositing Window Managers)"));
#endif
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 2, 0, 1,
			 GTK_FILL,GTK_SHRINK,0,0);

      table1 = gtkut_table_new (2, 1, FALSE,0,0,5);
      gtk_container_add (GTK_CONTAINER (frame), table1);

      label = gtkut_label_new (_("Opacity Alpha[%]"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtkut_table_attach(table1, label, 0, 1, 0, 1,
			 GTK_SHRINK,GTK_SHRINK,0,0);
      
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->alpha_main, 0, 100, 1.0, 10.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->alpha_main);
      scale =  gtkut_hscale_new (GTK_ADJUSTMENT(adj));
      gtk_scale_set_digits (GTK_SCALE (scale), 0);
      gtk_scale_set_draw_value (GTK_SCALE (scale), TRUE);
      gtkut_table_attach_defaults(table1, scale, 1, 2, 0, 1);
#ifndef USE_WIN32
      if(mascot->flag_composite==COMPOSITE_FALSE){
	gtk_widget_set_sensitive(scale,FALSE);
      }
#endif
      /*
      pixmap_scrwin = gtk_scrolled_window_new (NULL, NULL);
      gtkut_table_attach_defaults (table, pixmap_scrwin, 0, 2, 1, 2);
      gtk_container_set_border_width (GTK_CONTAINER (pixmap_scrwin), 5);
      gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(pixmap_scrwin),
				      GTK_POLICY_NEVER,
				      GTK_POLICY_ALWAYS);

      make_pixmap_list(mascot);
      */
      mascot->sw_imgtree = gtk_scrolled_window_new (NULL, NULL);
      gtkut_table_attach_defaults (table, mascot->sw_imgtree, 0, 2, 1, 2);
      gtk_container_set_border_width (GTK_CONTAINER (mascot->sw_imgtree), 5);
      gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(mascot->sw_imgtree),
				      GTK_POLICY_NEVER,
				      GTK_POLICY_ALWAYS);
      make_img_tree(mascot);
      
      hbox = gtkut_hbox_new(FALSE,5);
      gtk_container_set_border_width (GTK_CONTAINER (hbox), 5);
      gtkut_table_attach(table, hbox, 0, 2, 2, 3,
			 GTK_SHRINK,GTK_SHRINK,0,0);

      button=gtkut_button_new_with_icon(_("Append"),
#ifdef USE_GTK3				       
					"list-add"
#else
					GTK_STOCK_ADD
#endif
					);
      gtk_box_pack_start(GTK_BOX(hbox), button,FALSE, FALSE, 0);
      my_signal_connect(button,"clicked",create_add_image_dialog,
			(gpointer)mascot);
      

      button=gtkut_button_new_with_icon(_("Delete"),
#ifdef USE_GTK3				       
					"list-remove"
#else
					GTK_STOCK_REMOVE
#endif
					);
      gtk_box_pack_start(GTK_BOX(hbox), button,FALSE, FALSE, 0);
      my_signal_connect(button,"clicked",create_del_image_dialog,
			(gpointer)mascot);


      label = gtkut_label_new (_("Images"));

      gtk_notebook_append_page (GTK_NOTEBOOK (conf_note), table, label);
    }

    // アニメ設定
    i_mascot++;
    {
      table = gtkut_table_new (1, 2, FALSE,0,0,0);

      ptn_note = gtk_notebook_new ();
      gtk_notebook_set_tab_pos (GTK_NOTEBOOK (ptn_note), GTK_POS_RIGHT);
      gtk_notebook_set_scrollable (GTK_NOTEBOOK (ptn_note), TRUE);

      
      gtkut_table_attach_defaults (table, ptn_note, 0, 1, 0, 1);

      // Pattern 収録用notebook
      for(i_ptn=0;i_ptn<MAX_ANIME_PATTERN;i_ptn++){
	flag_make_pattern_list[i_ptn]=FALSE;
	//flag_make_frame_list[i_ptn]=FALSE;
	flag_make_ptntree[i_ptn]=FALSE;
      }
      
      i_ptn=0;
      while(mascot->frame_pix[i_ptn][0]!=-1){
	make_pattern_list(mascot, ptn_note, i_ptn);

	i_ptn++;
      }
      mascot->ptn_num=i_ptn;

      
      hbox = gtkut_hbox_new(FALSE,5);
      gtk_container_set_border_width (GTK_CONTAINER (hbox), 5);
      gtkut_table_attach(table, hbox, 0, 1, 1, 2,
			 GTK_SHRINK,GTK_SHRINK,0,0);
      button=gtkut_button_new_with_icon(_("Append Pattern"),
#ifdef USE_GTK3				       
				      "list-add"
#else
					GTK_STOCK_ADD
#endif
					);
      gtk_box_pack_start(GTK_BOX(hbox), button,FALSE, FALSE, 0);
      my_signal_connect(button,"clicked",create_add_pattern_dialog,
			(gpointer)mascot);
	  
      button=gtkut_button_new_with_icon(_("Copy Pattern"),
#ifdef USE_GTK3				       
					"edit-copy"
#else
					GTK_STOCK_COPY
#endif
					);
      gtk_box_pack_start(GTK_BOX(hbox), button,FALSE, FALSE, 0);
      my_signal_connect(button,"clicked",create_copy_pattern_dialog,
			(gpointer)mascot);
	  
      button=gtkut_button_new_with_icon(_("Delete Pattern"),
#ifdef USE_GTK3				       
					"list-remove"
#else
					GTK_STOCK_REMOVE
#endif
					);
      gtk_box_pack_start(GTK_BOX(hbox), button,FALSE, FALSE, 0);
      my_signal_connect(button,"clicked",create_del_pattern_dialog,
			(gpointer)mascot);
	  
  
      
    
      label = gtkut_label_new (_("Animation"));

      gtk_notebook_append_page (GTK_NOTEBOOK (conf_note), table, label);
  
    }

#ifdef USE_BIFF
    // Biff
    i_mascot++;
    {
      GtkWidget *entry;
      GtkWidget *table2;

      // 1x2のテーブル
      table = gtkut_table_new (1, 3, FALSE,0,0,0);
      
      frame = gtkut_frame_new (_("Biff Image"));
      // 着信画像
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 1, 0, 1,
			 GTK_FILL|GTK_EXPAND,GTK_SHRINK,0,0);
      
      
      table1 = gtkut_table_new (4, 3, FALSE,5,5,5);
      gtk_container_add (GTK_CONTAINER (frame), table1);
      
      label = gtkut_label_new (_("Image File"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtkut_table_attach(table1, label, 0, 1, 0, 1,0,0,0,0);
      
      table2 = gtkut_table_new (2, 1, FALSE,0,0,0);
      gtkut_table_attach (table1, table2,
			  1, 4, 0, 1,
			  GTK_FILL,
			  GTK_SHRINK,0,0);
      
      entry_mail_pix_file = gtk_entry_new ();
      gtkut_table_attach (table2, entry_mail_pix_file,
			  0, 1, 0, 1,
			  GTK_EXPAND | GTK_FILL,
			  GTK_EXPAND | GTK_FILL,0,0);
      if(mascot->mail.pix_file){
	gtk_entry_set_text(GTK_ENTRY(entry_mail_pix_file), 
			   my_basename(mascot->mail.pix_file));
      }
      
      button=gtkut_button_new_with_icon(NULL,
#ifdef USE_GTK3				       
					"document-open"
#else
					GTK_STOCK_OPEN
#endif
					);
      gtkut_table_attach (table2, button, 1, 2, 0, 1, 
			  GTK_SHRINK,GTK_SHRINK,0,0);
      my_signal_connect(button,"clicked",create_change_biff_image_dialog, 
			(gpointer)mascot);
      
      
      label = gtkut_label_new (_("Position"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtkut_table_attach(table1, label, 0, 1, 1, 2,
			 GTK_SHRINK,GTK_SHRINK,0,0);
      
      {
	GtkListStore *store;
	GtkTreeIter iter, iter_set;	  
	GtkCellRenderer *renderer;
	
	store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Left side of mascot"),
			   1, MAIL_PIX_LEFT, -1);
	if(mascot->mail.pix_pos==MAIL_PIX_LEFT) iter_set=iter;

	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, _("Right side of mascot"),
			   1, MAIL_PIX_RIGHT, -1);
	if(mascot->mail.pix_pos==MAIL_PIX_RIGHT) iter_set=iter;

	combo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
	gtkut_table_attach (table1, combo, 1, 2, 1, 2,
			    GTK_SHRINK,GTK_SHRINK,0,0);
	g_object_unref(store);
	
	renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo),renderer, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT(combo), renderer, "text",0,NULL);
	
	
	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo),&iter_set);
	gtk_widget_show(combo);
	my_signal_connect (combo,"changed",cc_get_combo_box,
			   &mascot->mail.pix_pos);
      }

      hbox = gtkut_hbox_new(FALSE,5);
      gtkut_table_attach(table1, hbox, 2, 3, 1, 2,0,0,0,0);
      label = gtkut_label_new (_("X"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->mail.pix_x, (gdouble)(-mascot->width),
	 (gdouble)(mascot->width), 1.0, 1.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->mail.pix_x);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtk_box_pack_start(GTK_BOX(hbox), spinner,FALSE, FALSE, 0);
      
      
      hbox = gtkut_hbox_new(FALSE,5);
      gtkut_table_attach(table1, hbox, 3, 4, 1, 2,0,0,0,0);
      label = gtkut_label_new (_("Y"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->mail.pix_y, (gdouble)(-mascot->height),
	 (gdouble)(mascot->height), 1.0, 1.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->mail.pix_y);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtk_box_pack_start(GTK_BOX(hbox), spinner,FALSE, FALSE, 0);
      
      label = gtkut_label_new (_("Opacity Alpha[%]"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtkut_table_attach(table1, label, 0, 1, 2, 3,
			 GTK_FILL,GTK_SHRINK,0,0);
      
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->alpha_biff, 0, 100, 1.0, 10.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->alpha_biff);
      scale =  gtkut_hscale_new (GTK_ADJUSTMENT(adj));
      gtk_scale_set_digits (GTK_SCALE (scale), 0);
      gtk_scale_set_draw_value (GTK_SCALE (scale), TRUE);
      gtkut_table_attach_defaults(table1, scale, 1, 4, 2, 3);
#ifndef USE_WIN32
      if(mascot->flag_composite==COMPOSITE_FALSE)
	gtk_widget_set_sensitive(scale,FALSE);
#endif

      frame = gtkut_frame_new (_("Biff Action"));
      // 着信動作
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 1, 1, 2,
			 GTK_FILL|GTK_EXPAND,GTK_SHRINK,0,0);
      
      
      table1 = gtkut_table_new (2, 3, FALSE,0,5,5);
      gtk_container_add (GTK_CONTAINER (frame), table1);
      
      label = gtkut_label_new (_("Message"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtkut_table_attach(table1, label, 0, 1, 0, 1,0,0,0,0);
	
      entry = gtk_entry_new ();
      if(mascot->mail.word){
	gtk_entry_set_text(GTK_ENTRY(entry), mascot->mail.word);
      }
      my_signal_connect (entry,"changed",cc_get_entry,&mascot->mail.word);
      gtkut_table_attach (table1, entry,
			  1, 2, 0, 1,
			  GTK_EXPAND | GTK_FILL,
			  GTK_EXPAND | GTK_FILL,0,0);
      
      label = gtkut_label_new (_("Sound File"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtkut_table_attach(table1, label, 0, 1, 1, 2,0,0,0,0);
      
      table2 = gtkut_table_new (4, 1, FALSE,0,0,0);
      gtkut_table_attach (table1, table2,
			  1, 2, 1, 2,
			  GTK_FILL,
			  GTK_SHRINK,0,0);
      
      entry_mail_sound = gtk_entry_new ();
      gtkut_table_attach (table2, entry_mail_sound,
			  0, 1, 0, 1,
			  GTK_EXPAND | GTK_FILL,
			  GTK_EXPAND | GTK_FILL,0,0);
      if(mascot->mail.sound){
	gtk_entry_set_text(GTK_ENTRY(entry_mail_sound), mascot->mail.sound);
      }
      
      button=gtkut_button_new_with_icon(NULL,
#ifdef USE_GTK3				       
					"document-open"
#else
					GTK_STOCK_OPEN
#endif
					);
      gtkut_table_attach (table2, button, 1, 2, 0, 1, 
			  GTK_SHRINK,GTK_SHRINK,0,0);
      my_signal_connect(button,"clicked",create_change_biff_sound_dialog, 
			(gpointer)mascot);
      
      button = gtk_button_new_with_label(_("Clear"));
      gtkut_table_attach (table2, button, 2, 3, 0, 1, 
			  GTK_SHRINK,GTK_SHRINK,0,0);
      my_signal_connect (button,"clicked",clear_biff_sound,
			 (gpointer)mascot);
      
      button = gtk_button_new_with_label(_("Test"));
      gtkut_table_attach (table2, button, 3, 4, 0, 1, 
			  GTK_SHRINK,GTK_SHRINK,0,0);
      my_signal_connect (button,"clicked",test_biff_sound,
			 (gpointer)mascot);
      
      table1=gtkut_table_new(1,1,FALSE,0,0,0);
      gtkut_table_attach_defaults(table, table1, 0, 1, 2, 3);
     

      label = gtkut_label_new (_("Biff"));
      gtk_notebook_append_page (GTK_NOTEBOOK (conf_note), table, label);
    }
#endif  // USE_BIFF

    // 情報
    i_mascot++;
    {
      GtkWidget *entry;
      
      // 1x4のテーブル
      table = gtkut_table_new (1, 2, FALSE,0,0,0);

      frame = gtkut_frame_new (_("About This Mascot"));
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
      gtkut_table_attach(table, frame, 0, 1, 0, 1,
			 GTK_FILL,GTK_SHRINK,0,0);

      table1 = gtkut_table_new(2,4,FALSE,5,5,5);
      gtk_container_add (GTK_CONTAINER (frame), table1);

      label = gtkut_label_new (_("Name"));
      gtkut_pos(label, POS_CENTER, POS_CENTER);
      gtkut_table_attach(table1, label, 0, 1, 0, 1,
			 GTK_FILL,GTK_SHRINK,0,0);

      entry = gtk_entry_new ();
      if(mascot->name){
	gtk_entry_set_text(GTK_ENTRY(entry),mascot->name);
      }
      gtkut_table_attach_defaults(table1, entry, 1, 2, 0, 1);
      my_signal_connect (entry,"changed",cc_get_entry, &mascot->name);

      label = gtkut_label_new (_("File"));
      gtkut_pos(label, POS_CENTER, POS_CENTER);
      gtkut_table_attach(table1, label, 0, 1, 1, 2,
			 GTK_FILL,GTK_SHRINK,0,0);

      entry = gtk_entry_new ();
      gtk_entry_set_text(GTK_ENTRY(entry),
                         to_utf8(mascot->file));
      gtkut_table_attach_defaults(table1, entry, 1, 2, 1, 2);


      label = gtkut_label_new (_("Character Code"));
      gtkut_pos(label, POS_CENTER, POS_CENTER);
      gtkut_table_attach(table1, label, 0, 1, 2, 3,
			 GTK_FILL,GTK_SHRINK,0,0);

      entry = gtk_entry_new ();
      if(mascot->code){
	gtk_entry_set_text(GTK_ENTRY(entry),mascot->code);
      }
      else{
	gtk_entry_set_text(GTK_ENTRY(entry),_("(Based on the locale environment)"));
      }
      gtkut_table_attach_defaults(table1, entry, 1, 2, 2, 3);


      if(mascot->copyright){
	label = gtkut_label_new (mascot->copyright);
	gtkut_pos(label, POS_CENTER, POS_CENTER);
	gtkut_table_attach_defaults(table1, label, 0, 2, 3, 4);
      }


      table1=gtkut_table_new(1,1,FALSE,0,0,0);
      gtkut_table_attach_defaults(table, table1, 0, 1, 1, 2);
    
      label = gtkut_label_new (_("Info."));

      gtk_notebook_append_page (GTK_NOTEBOOK (conf_note), table, label);
    }
  }

  label = gtkut_label_new (_("Mascot"));
  gtk_notebook_append_page (GTK_NOTEBOOK (all_note), conf_note, label);
  }
 
  // メニュー設定
  {
    GtkWidget *table;
    GtkWidget *label;

    if(mascot->menu_total==0){
      table=create_new_menu_page(mascot);
    }
    else{
      table=create_menu_page(mascot);
    }
    
    label = gtkut_label_new (_("Menu"));
      
    gtk_notebook_append_page (GTK_NOTEBOOK (all_note), table, label);
     
  }

  gtk_widget_show_all(mascot->conf_main);

  gtk_dialog_run(GTK_DIALOG(mascot->conf_main));

  if(GTK_IS_WIDGET(mascot->imgtree)) gtk_widget_destroy(mascot->imgtree);
  flag_make_imgtree=FALSE;

  for(i_ptn=0;i_ptn<MAX_ANIME_PATTERN;i_ptn++){
    flag_make_ptntree[i_ptn]=FALSE;
  }

  if(mascot->test_timer>0) g_source_remove(mascot->test_timer);
  mascot->test_timer=-1;
  
  if(GTK_IS_WIDGET(mascot->conf_main)) gtk_widget_destroy(mascot->conf_main);
  
  //InitMascot(mascot);

#ifdef USE_BIFF
  SetMailChecker(mascot);
#endif   // USE_BIFF

  InitComposite(mascot);
  LoadPixmaps(mascot);
#ifndef USE_GTK3
  ReInitGC(mascot);
#endif
  map_balloon(mascot, FALSE);
  flag_balloon=FALSE;
#ifdef USE_BIFF
  LoadBiffPixmap(mascot->biff_pix, mascot);
#endif

  // DrawingArea のrealize
  gtk_widget_realize(mascot->dw_main);
  gtk_widget_realize(mascot->dw_balloon);
  gtk_widget_realize(mascot->dw_clock);
  gtk_widget_realize(mascot->dw_biff);
#ifdef FG_DRAW
  gtk_widget_realize(mascot->dw_sdw);
  gtk_widget_realize(mascot->dw_clkfg);
  gtk_widget_realize(mascot->dw_balfg);
#endif

  if(mascot->clkmode!=CLOCK_NO) clock_update(mascot, TRUE);

  if(mascot->clkmode==CLOCK_PANEL){
    map_clock(mascot, TRUE);
  }
  else{
    map_clock(mascot, FALSE);
  }

  map_main(mascot, TRUE);

  if(mascot->clkmode!=CLOCK_NO)  clock_update(mascot, TRUE);

  if((mascot->move==MOVE_FIX)
     &&(mascot->xfix>=0)&&(mascot->xfix<=mascot->width_root)
     &&(mascot->yfix>=0)&&(mascot->yfix<=mascot->height_root))
    MoveMascot(mascot,mascot->xfix,mascot->yfix);
  else{
    MoveToFocus(mascot,TRUE);
 }
  

  gtk_widget_destroy(mascot->PopupMenu);
  mascot->PopupMenu=make_popup_menu(mascot);


  while (my_main_iteration(FALSE));
  gdkut_flush(mascot);

  flag_make_pixmap_list=FALSE;
  mascot->flag_menu=FALSE;
}

// 設定ダイアログ中のメニュー設定ページの作成
// メニューがある場合
GtkWidget * create_menu_page(typMascot *mascot)
{
  GtkWidget *table;
  GtkWidget *button;
  GtkWidget *hbox;

  // 2x1のテーブル
  table = gtkut_table_new (2, 1, FALSE,0,0,0);

  cat_note = gtk_notebook_new ();
  gtk_notebook_set_tab_pos (GTK_NOTEBOOK (cat_note), GTK_POS_TOP);
  gtk_notebook_set_scrollable (GTK_NOTEBOOK (cat_note), TRUE);
  
  gtkut_table_attach_defaults (table, cat_note, 0, 1, 0, 1);
  
  // Category 収録用notebook
  {
    gint i_cat=0;
    
    
    for(i_cat=0;i_cat<MAX_MENU_CATEGORY;i_cat++){
      flag_make_cat_list[i_cat]=FALSE;
      flag_make_tgt_list[i_cat]=FALSE;
    }
    
    for(i_cat=0;i_cat<mascot->menu_cat_max;i_cat++){
      make_cat_list(mascot, cat_note, i_cat);
    }
  }
  
  hbox = gtkut_hbox_new(FALSE,5);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 5);
  gtkut_table_attach(table, hbox, 0, 1, 1, 2,
		     GTK_SHRINK,GTK_SHRINK,0,0);
  
  button=gtkut_button_new_with_icon(_("Append Category"),
#ifdef USE_GTK3				       
				    "list-add"
#else
				    GTK_STOCK_ADD
#endif
				    );
  gtk_box_pack_start(GTK_BOX(hbox), button,FALSE, FALSE, 0);
  my_signal_connect(button,"clicked",create_add_cat_dialog, (gpointer)mascot);
  
  button=gtkut_button_new_with_icon(_("Delete Category"),
#ifdef USE_GTK3				       
				    "list-remove"
#else
				    GTK_STOCK_REMOVE
#endif
				    );
  gtk_box_pack_start(GTK_BOX(hbox), button,FALSE, FALSE, 0);
  my_signal_connect(button,"clicked",create_del_cat_dialog, (gpointer)mascot);
  
  button=gtkut_button_new_with_icon(_("Merge Menu"),
#ifdef USE_GTK3				       
				    "insert-link"
#else
				    GTK_STOCK_CONVERT
#endif
				    );
  gtk_box_pack_start(GTK_BOX(hbox), button,FALSE, FALSE, 0);
  my_signal_connect(button,"clicked",create_merge_cat_dialog, (gpointer)mascot);

  return(table);
}

// 設定ダイアログ中のメニュー設定ページの作成
// メニューがない場合
GtkWidget * create_new_menu_page(GtkWidget *conf_note)
{
  GtkWidget *table;
  GtkWidget *label;
  GtkWidget *vbox;

  // 1x3のテーブル
  table = gtkut_table_new (1, 3, FALSE,0,0,0);


  vbox = gtkut_vbox_new(FALSE,5);
  gtkut_table_attach(table, vbox, 0, 1, 0, 1,
		     GTK_SHRINK,GTK_SHRINK,0,0);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 5);
  label = gtkut_label_new ("");
  gtkut_pos(label, POS_CENTER, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 0);
  
  label = gtkut_label_new (_("No menu files are loaded currently."));
  gtkut_pos(label, POS_START, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 0);
  
  label = gtkut_label_new (_("You can find a menu to create new one in the popup,"));
  gtkut_pos(label, POS_START, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 0);
  
  label = gtkut_label_new (_(" and edit it here."));
  gtkut_pos(label, POS_START, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 0);
  
  return(table);
}

// 画像ファイルリスト
void make_pixmap_list(typMascot *mascot)
{
  int i_pix;
  char no_tmp[3];
  GtkWidget *label;
  GtkWidget *button;
  
  if(flag_make_pixmap_list){
    gtk_widget_destroy(pixmap_table);
    g_object_unref(pixmap_table);
  }
  else flag_make_pixmap_list=TRUE;

  pixmap_table = gtkut_table_new (3, MAX_PIXMAP+2, FALSE,0,0,5);
  //g_object_ref(pixmap_table);
  //gtk_object_sink(GTK_OBJECT(pixmap_table));

#ifdef USE_GTK3
  gtk_container_add(GTK_CONTAINER(pixmap_scrwin),pixmap_table);
#else
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW (pixmap_scrwin), pixmap_table);
#endif
  
  label = gtkut_label_new (_("No."));
  gtkut_table_attach(pixmap_table, label, 0, 1, 0, 1,
		     GTK_SHRINK,GTK_SHRINK,0,0);
  
  gtkut_pos(label, POS_START, POS_CENTER);
	 
  label = gtkut_label_new (_("Image File Name"));
  gtkut_table_attach(pixmap_table, label, 1, 2, 0, 1,
		     GTK_EXPAND,GTK_SHRINK,0,2);
  gtkut_pos(label, POS_CENTER, POS_CENTER);
  
  for(i_pix=0;i_pix<mascot->nPixmap;i_pix++){
    sprintf(no_tmp,"%02d",i_pix);
    button = gtk_button_new_with_label(no_tmp);
    gtkut_table_attach (pixmap_table,
			button, 0, 1, i_pix+1, i_pix+2,
			GTK_SHRINK,GTK_SHRINK,0,2);
    my_signal_connect(button,"clicked",TestPut, (gpointer)mpix[i_pix]);
  
    pixmap_entry[i_pix] = gtk_entry_new ();
    gtkut_table_attach(pixmap_table, pixmap_entry[i_pix],
		       1, 2, i_pix+1, i_pix+2,
		       GTK_FILL,GTK_SHRINK,0,2);
    gtk_entry_set_text(GTK_ENTRY(pixmap_entry[i_pix]),
		       my_basename(mascot->sprites[i_pix].filename));
	   
    
    button=gtkut_button_new_with_icon(NULL,
#ifdef USE_GTK3				       
				      "document-open"
#else
				      GTK_STOCK_OPEN
#endif
				      );
    gtkut_table_attach (pixmap_table, button, 
			2, 3, i_pix+1, i_pix+2,
			GTK_SHRINK,GTK_SHRINK,0,0);
    //my_signal_connect(button,"clicked",create_change_image_dialog, 
    //		      (gpointer)mpix[i_pix]);
    
  }
  label=gtkut_label_new("");
  gtkut_table_attach_defaults(pixmap_table, label, 
			    0, 3, MAX_PIXMAP+1, MAX_PIXMAP+2);
  gtk_widget_show_all(pixmap_table);
}


// パターンリスト
void make_pattern_list(typMascot *mascot, GtkWidget *ptn_note, int i_ptn)
{
  GtkWidget *label;
  GtkWidget *button;
  GtkAdjustment *adj;
  GtkWidget *spinner;
  GtkWidget *hbox;
  GtkWidget *entry;
  GtkWidget *frame;
  GtkWidget *table2;
  gchar bufferl[32];

  if(flag_make_pattern_list[i_ptn]) {
    gtk_widget_destroy(GTK_WIDGET(pattern_table[i_ptn]));
    g_object_unref(GTK_WIDGET(pattern_table[i_ptn]));
    //flag_make_frame_list[i_ptn]=FALSE;
    flag_make_ptntree[i_ptn]=FALSE;
  }
  else flag_make_pattern_list[i_ptn]=TRUE;

  if(i_ptn==0){
    sprintf(bufferl, _("Base"));
  }
  else{
    sprintf(bufferl, "P.%02d", i_ptn);
  }
  
  pattern_table[i_ptn] = gtkut_table_new (3, 6, FALSE,0,0,5);
  //g_object_ref (pattern_table[i_ptn]); 
  //gtk_object_sink (GTK_OBJECT (pattern_table[i_ptn]));

  ptn_scrwin[i_ptn] = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_set_size_request (ptn_scrwin[i_ptn], 30, 250);
  gtkut_table_attach_defaults (pattern_table[i_ptn],
			       ptn_scrwin[i_ptn], 0, 3, 0, 1);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(ptn_scrwin[i_ptn]),
				  GTK_POLICY_NEVER,
				  GTK_POLICY_ALWAYS);
	  
  //make_frame_list(mascot, i_ptn);
  make_ptn_tree(mascot, i_ptn);
	  
  hbox = gtkut_hbox_new(FALSE,5);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 5);
  gtkut_table_attach(pattern_table[i_ptn], hbox, 0, 3, 1, 2,
		     GTK_SHRINK,GTK_SHRINK,0,0);
  button=gtkut_button_new_with_icon(_("Append Frame"),
#ifdef USE_GTK3				       
				    "list-add"
#else
				    GTK_STOCK_ADD
#endif
				    );
  gtk_box_pack_start(GTK_BOX(hbox), button,FALSE, FALSE, 0);
  my_signal_connect(button,"clicked",create_add_frame_dialog,
		    (gpointer)mptn[i_ptn]);
	  
  button=gtkut_button_new_with_icon(_("Delete Frame"),
#ifdef USE_GTK3				       
				    "list-remove"
#else
				    GTK_STOCK_REMOVE
#endif
				    );
  gtk_box_pack_start(GTK_BOX(hbox), button,FALSE, FALSE, 0);
  my_signal_connect(button,"clicked",create_del_frame_dialog,
		    (gpointer)mptn[i_ptn]);
  
  button=gtkut_button_new_with_icon(_("Test Anime"),
#ifdef USE_GTK3				       
				    "media-playback-start"
#else
				    GTK_STOCK_REDO
#endif
				    );
  gtk_box_pack_start(GTK_BOX(hbox), button,FALSE, FALSE, 0);
  my_signal_connect(button,"clicked",TestAnime, (gpointer)mptn[i_ptn]);
  
  if(i_ptn!=0){
      label = gtkut_label_new (_("Random"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->random_weight[i_ptn], 0, 99, 1.0, 1.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->random_weight[i_ptn]);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtk_box_pack_start(GTK_BOX(hbox), spinner,FALSE, FALSE, 0);


      label = gtkut_label_new (_(" Click"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->click_weight[i_ptn], 0, 99, 1.0, 1.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->click_weight[i_ptn]);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtk_box_pack_start(GTK_BOX(hbox), spinner,FALSE, FALSE, 0);
    

      hbox = gtkut_hbox_new(FALSE,5);
      gtk_container_set_border_width (GTK_CONTAINER (hbox), 5);
      gtkut_table_attach(pattern_table[i_ptn], hbox, 0, 3, 4, 5,
			 GTK_SHRINK,GTK_SHRINK,0,0);

      label = gtkut_label_new (_("Balloon Offset X[Left]"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->bal_lxoff[i_ptn], -99, 99, 1.0, 1.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->bal_lxoff[i_ptn]);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtk_box_pack_start(GTK_BOX(hbox), spinner,FALSE, FALSE, 0);
    

      label = gtkut_label_new (_(" Y[Left]"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->bal_lyoff[i_ptn], -99, 99, 1.0, 1.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->bal_lyoff[i_ptn]);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtk_box_pack_start(GTK_BOX(hbox), spinner,FALSE, FALSE, 0);
    

      label = gtkut_label_new (_("  X[Right]"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->bal_rxoff[i_ptn], -99, 99, 1.0, 1.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->bal_rxoff[i_ptn]);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtk_box_pack_start(GTK_BOX(hbox), spinner,FALSE, FALSE, 0);
    

      label = gtkut_label_new (_(" Y[Right]"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->bal_ryoff[i_ptn], -99, 99, 1.0, 1.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->bal_ryoff[i_ptn]);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtk_box_pack_start(GTK_BOX(hbox), spinner,FALSE, FALSE, 0);

#ifdef USE_SOCKMSG      
      frame = gtkut_frame_new (_("Duet Animation"));
      gtkut_table_attach(pattern_table[i_ptn],
			 frame, 0, 3, 5, 6,
			 GTK_FILL,GTK_SHRINK,0,0);
      gtk_container_set_border_width (GTK_CONTAINER (frame), 5);
	
      table2 = gtkut_table_new(1,2,FALSE,0,0,5);
      gtk_container_add (GTK_CONTAINER (frame), table2);

      hbox = gtkut_hbox_new(FALSE,0);
      gtkut_table_attach_defaults(table2,
				  hbox, 0, 1, 0, 1);
      gtk_container_set_border_width (GTK_CONTAINER (hbox), 5);
      label = gtkut_label_new (_("Mascot File"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      duet_tgt_entry[i_ptn] = gtk_entry_new ();
      if(mascot->duet_tgt[i_ptn]){
	gtk_entry_set_text(GTK_ENTRY(duet_tgt_entry[i_ptn]),
			   mascot->duet_tgt[i_ptn]);
      }
      gtk_box_pack_start(GTK_BOX(hbox), duet_tgt_entry[i_ptn],TRUE, TRUE, 0);
      my_signal_connect (duet_tgt_entry[i_ptn],"changed",cc_get_entry,
			 &mascot->duet_tgt[i_ptn]);
      button=gtkut_button_new_with_icon(NULL,
#ifdef USE_GTK3				       
					"document-open"
#else
					GTK_STOCK_OPEN
#endif
					);
      gtk_box_pack_start(GTK_BOX(hbox), button,FALSE, FALSE, 0);
      my_signal_connect(button,"clicked",create_change_duet_tgt_dialog, 
			(gpointer)mptn[i_ptn]);
      label = gtkut_label_new (_(" Pattern"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->duet_ptn[i_ptn], 1, MAX_ANIME_PATTERN-1,
	 1.0, 1.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->duet_ptn[i_ptn]);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtk_box_pack_start(GTK_BOX(hbox), spinner,FALSE, FALSE, 0);

      hbox = gtkut_hbox_new(FALSE,0);
      gtkut_table_attach_defaults(table2,
				  hbox, 0, 1, 1, 2);
      label = gtkut_label_new (_("Message"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      entry = gtk_entry_new ();
      if(mascot->duet_word[i_ptn]){
	gtk_entry_set_text(GTK_ENTRY(entry),mascot->duet_word[i_ptn]);
      }
      gtk_box_pack_start(GTK_BOX(hbox), entry,TRUE, TRUE, 0);
      my_signal_connect (entry,"changed",cc_get_entry,
			 &mascot->duet_word[i_ptn]);
      label = gtkut_label_new (_(" Delay"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      adj = (GtkAdjustment *)gtk_adjustment_new 
	((gdouble)mascot->duet_delay[i_ptn], 0, 99,
	 1.0, 1.0, 0.0);
      my_signal_connect (adj, "value_changed",cc_get_adj,
			 &mascot->duet_delay[i_ptn]);
      spinner =  gtk_spin_button_new (adj, 0, 0);
      gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
      gtk_box_pack_start(GTK_BOX(hbox), spinner,FALSE, FALSE, 0);
    
#endif

      hbox = gtkut_hbox_new(FALSE,0);
      gtkut_table_attach(pattern_table[i_ptn],
			 hbox, 0, 3, 2, 3,
			 GTK_FILL,GTK_SHRINK,0,0);
      gtk_container_set_border_width (GTK_CONTAINER (hbox), 5);
      label = gtkut_label_new (_("Message"));
      gtkut_pos(label, POS_START, POS_CENTER);
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      entry = gtk_entry_new ();
      if(mascot->click_word[i_ptn]){
	gtk_entry_set_text(GTK_ENTRY(entry),mascot->click_word[i_ptn]);
      }
      gtk_box_pack_start(GTK_BOX(hbox), entry,TRUE, TRUE, 0);
      my_signal_connect (entry,"changed",cc_get_entry,
			 &mascot->click_word[i_ptn]);
    
      table2 = gtkut_table_new (5, 1, FALSE,0,0,5);
#ifndef USE_GTK3      
      gtk_table_set_col_spacing (GTK_TABLE(table2), 0, 0);
#endif
      gtkut_table_attach(pattern_table[i_ptn],
			 table2, 0, 3, 3, 4,
			 GTK_FILL,GTK_SHRINK,0,0);

      label = gtkut_label_new (_("Sound File"));
      gtkut_table_attach(table2,label, 0, 1, 0, 1,
			 GTK_SHRINK,GTK_SHRINK,0,0);
      gtkut_pos(label, POS_START, POS_CENTER);
      click_sound_entry[i_ptn] = gtk_entry_new ();
      if(mascot->click_sound[i_ptn]){
	gtk_entry_set_text(GTK_ENTRY(click_sound_entry[i_ptn]),
			   mascot->click_sound[i_ptn]);
      }
      gtkut_table_attach_defaults(table2,
				  click_sound_entry[i_ptn], 1, 2, 0, 1);
      button=gtkut_button_new_with_icon(NULL,
#ifdef USE_GTK3				       
					"document-open"
#else
					GTK_STOCK_OPEN
#endif
					);
      gtkut_table_attach(table2,button, 2, 3, 0, 1,
			 GTK_SHRINK,GTK_SHRINK,0,0);
      my_signal_connect(button,"clicked",create_change_click_sound_dialog, 
			(gpointer)mptn[i_ptn]);
      button = gtk_button_new_with_label(_("Clear"));
      gtkut_table_attach(table2,button, 3, 4, 0, 1,
			 GTK_SHRINK,GTK_SHRINK,0,0);
      my_signal_connect (button,"clicked",clear_click_sound,
			 (gpointer)mptn[i_ptn]);
      button = gtk_button_new_with_label(_("Test"));
      gtkut_table_attach(table2,button, 4, 5, 0, 1,
			 GTK_SHRINK,GTK_SHRINK,0,0);
      my_signal_connect (button,"clicked",test_click_sound,
			 (gpointer)mptn[i_ptn]);
  }

  
  label = gtkut_label_new (bufferl);
  gtk_notebook_insert_page (GTK_NOTEBOOK (ptn_note), 
  			    pattern_table[i_ptn], label, i_ptn);
  
  gtk_widget_show_all(ptn_note);
  
}

// メニュー用カテゴリーリスト
void make_cat_list(typMascot *mascot, GtkWidget *cat_note, int i_cat)
{
  GtkWidget *label;
  GtkWidget *button;
  GtkWidget *entry;
  GtkWidget *arrow;
  GtkWidget *hbox;
  gchar bufferl[32];

  if(flag_make_cat_list[i_cat]) {
    //gtk_widget_destroy(GTK_WIDGET(cat_table[i_cat]));
    gtk_notebook_remove_page (GTK_NOTEBOOK (cat_note), i_cat);
    gtk_widget_queue_draw(GTK_WIDGET(cat_note));
    flag_make_tgt_list[i_cat]=FALSE;
  }
  else flag_make_cat_list[i_cat]=TRUE;


  sprintf(bufferl, "C.%02d", i_cat);
  
  cat_table[i_cat] = gtkut_table_new (4, 4, FALSE,0,0,0);


  // カテゴリー名
  {
      if(i_cat!=0){
	arrow= gtkut_arrow_new(MY_ARROW_LEFT);
	button = gtk_button_new();
	gtk_container_add (GTK_CONTAINER (button), arrow);
	gtkut_table_attach(cat_table[i_cat], button,
			   0, 1, 0, 1,
			   GTK_SHRINK,GTK_SHRINK,0,0);
	my_signal_connect (button,"clicked",mc_up_cat,
			   (gpointer)mcat[i_cat]);
      }

      hbox = gtkut_hbox_new(FALSE,5);
      gtk_container_set_border_width (GTK_CONTAINER (hbox), 5);
      gtkut_table_attach(cat_table[i_cat], hbox, 1, 3, 0, 1,
			 GTK_FILL,GTK_SHRINK,0,0);

      label = gtkut_label_new (_("Category"));
      gtk_box_pack_start(GTK_BOX(hbox), label,FALSE, FALSE, 0);
      gtkut_pos(label, POS_END, POS_CENTER);
      entry = gtk_entry_new ();
      gtk_box_pack_start(GTK_BOX(hbox), entry,FALSE, FALSE, 0);
      if(mascot->menu_cat[i_cat]){
	gtk_entry_set_text(GTK_ENTRY(entry),mascot->menu_cat[i_cat]);
      }
      my_signal_connect (entry,"changed",cc_get_entry,
			 &mascot->menu_cat[i_cat]);

      if(i_cat!=mascot->menu_cat_max-1){
	arrow= gtkut_arrow_new(MY_ARROW_RIGHT);
	button = gtk_button_new();
	gtk_container_add (GTK_CONTAINER (button), arrow);
	gtkut_table_attach(cat_table[i_cat], button,
			   3, 4, 0, 1,
			   GTK_SHRINK,GTK_SHRINK,0,0);
	my_signal_connect (button,"clicked",mc_down_cat,(gpointer)mcat[i_cat]);
      }
    
    
  }

  cat_scrwin[i_cat] = gtk_scrolled_window_new (NULL, NULL);
  gtkut_table_attach_defaults (cat_table[i_cat],
			       cat_scrwin[i_cat], 0, 4, 1, 2);
  gtk_container_set_border_width (GTK_CONTAINER (cat_scrwin[i_cat]), 5);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(cat_scrwin[i_cat]),
				  GTK_POLICY_NEVER,
				  GTK_POLICY_ALWAYS);
  
  make_tgt_list(mascot, i_cat);
	  

  hbox = gtkut_hbox_new(FALSE,5);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 5);
  gtkut_table_attach(cat_table[i_cat], hbox, 0, 4, 2, 3,
		     GTK_SHRINK,GTK_SHRINK,0,0);
  button=gtkut_button_new_with_icon(_("Append Mascot"),
#ifdef USE_GTK3				       
				    "list-add"
#else
				    GTK_STOCK_ADD
#endif
				    );
  gtk_box_pack_start(GTK_BOX(hbox), button,FALSE, FALSE, 0);
  my_signal_connect(button,"clicked",create_add_tgt_dialog,
		    (gpointer)mcat[i_cat]);
	  
  button=gtkut_button_new_with_icon(_("Delete Mascot"),
#ifdef USE_GTK3				       
				    "list-remove"
#else
				    GTK_STOCK_REMOVE
#endif
				    );
  gtk_box_pack_start(GTK_BOX(hbox), button,FALSE, FALSE, 0);
  my_signal_connect(button,"clicked",create_del_tgt_dialog,i_cat);
  

  label = gtkut_label_new (bufferl);
  gtk_notebook_insert_page (GTK_NOTEBOOK (cat_note), 
			    cat_table[i_cat], label, i_cat);
  gtk_widget_show_all(cat_note);
  
}

// パターンないフレームリスト
void make_frame_list(typMascot *mascot, int i_ptn)
{
  int i_frm;
  int i_pix;
  char no_tmp[3];
  GtkWidget *label;
  GtkWidget *button;
  GtkAdjustment *adj;
  GtkWidget *spinner;

  if(flag_make_frame_list[i_ptn]){
    gtk_widget_destroy(frame_table[i_ptn]);
    g_object_unref(frame_table[i_ptn]);
  }
  else flag_make_frame_list[i_ptn]=TRUE;

  frame_table[i_ptn] = gtkut_table_new (7, MAX_ANIME_FRAME+3, FALSE,0,0,5);
  //g_object_ref (frame_table[i_ptn]); 
  //gtk_object_sink (GTK_OBJECT (frame_table[i_ptn]));
  
#ifdef USE_GTK3
  gtk_container_add(GTK_CONTAINER(ptn_scrwin[i_ptn]), frame_table[i_ptn]);
#else
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW (ptn_scrwin[i_ptn]), frame_table[i_ptn]);
#endif
  
  label = gtkut_label_new (_("No."));
  gtkut_table_attach(frame_table[i_ptn], label, 0, 1, 0, 1,
		     GTK_FILL,GTK_SHRINK,0,0);
  gtkut_pos(label, POS_START, POS_START);
	 
  label = gtkut_label_new (_("Image"));
  gtkut_table_attach(frame_table[i_ptn], label, 1, 2, 0, 1,
		     GTK_FILL,GTK_SHRINK,0,0);
  gtkut_pos(label, POS_CENTER, POS_START);
  
  label = gtkut_label_new (_("Min"));
  gtkut_table_attach(frame_table[i_ptn], label, 2, 3, 0, 1,
		     GTK_FILL,GTK_SHRINK,0,0);
  gtkut_pos(label, POS_CENTER, POS_START);
  
  label = gtkut_label_new (_("Max"));
  gtkut_table_attach(frame_table[i_ptn], label, 3, 4, 0, 1,
		     GTK_FILL,GTK_SHRINK,0,0);
  gtkut_pos(label, POS_CENTER, POS_START);
  
  label = gtkut_label_new (_("Block Loop"));
  gtkut_table_attach(frame_table[i_ptn], label, 4, 7, 0, 1,
		     GTK_FILL,GTK_SHRINK,0,0);
  gtkut_pos(label, POS_CENTER, POS_START);
  
  label = gtkut_label_new (_("Next"));
  gtkut_table_attach(frame_table[i_ptn], label, 4, 5, 1, 2,
		     GTK_FILL,GTK_SHRINK,0,0);
  gtkut_pos(label, POS_CENTER, POS_END);
  
  label = gtkut_label_new (_("Min"));
  gtkut_table_attach(frame_table[i_ptn], label, 5, 6, 1, 2,
		     GTK_FILL,GTK_SHRINK,0,0);
  gtkut_pos(label, POS_CENTER, POS_END);
  
  label = gtkut_label_new (_("Max"));
  gtkut_table_attach(frame_table[i_ptn], label, 6, 7, 1, 2,
		     GTK_FILL,GTK_SHRINK,0,0);
  gtkut_pos(label, POS_CENTER, POS_END);
  
  i_frm=0;
  for(i_frm=0;i_frm<mascot->frame_num[i_ptn];i_frm++){
    sprintf(no_tmp,"%02d",i_frm);
    button = gtk_button_new_with_label(no_tmp);
    gtkut_table_attach (frame_table[i_ptn],
			button, 0, 1, i_frm+2, i_frm+3,
			GTK_FILL,GTK_SHRINK,0,2);
    my_signal_connect(button,"clicked",TestPut, 
		      (gpointer)mpix[mascot->frame_pix[i_ptn][i_frm]]);

    // Image
    adj = (GtkAdjustment *)gtk_adjustment_new 
      ((gdouble)mascot->frame_pix[i_ptn][i_frm], 0,
       (gdouble)mascot->nPixmap-1, 1.0, 1.0, 0.0);
    my_signal_connect (adj, "value_changed",cc_get_adj,
		       &mascot->frame_pix[i_ptn][i_frm]);
    spinner =  gtk_spin_button_new (adj, 0, 0);
    gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
    gtkut_table_attach(frame_table[i_ptn], spinner,
		       1, 2, i_frm+2, i_frm+3,
		       GTK_FILL,GTK_SHRINK,0,0);


    // Min
    adj = (GtkAdjustment *)gtk_adjustment_new 
      ((gdouble)mascot->frame_min[i_ptn][i_frm], 1,999, 1.0, 1.0, 0.0);
    my_signal_connect (adj, "value_changed",cc_get_adj,
		       &mascot->frame_min[i_ptn][i_frm]);
    spinner =  gtk_spin_button_new (adj, 0, 0);
    gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
    gtkut_table_attach(frame_table[i_ptn], spinner,
		       2, 3, i_frm+2, i_frm+3,
		       GTK_FILL,GTK_SHRINK,0,0);
    
  
    // Max
    adj = (GtkAdjustment *)gtk_adjustment_new 
      ((gdouble)mascot->frame_max[i_ptn][i_frm], 1,999, 1.0, 1.0, 0.0);
    my_signal_connect (adj, "value_changed",cc_get_adj,
		       &mascot->frame_max[i_ptn][i_frm]);
    spinner =  gtk_spin_button_new (adj, 0, 0);
    gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
    gtkut_table_attach(frame_table[i_ptn], spinner,
		       3, 4, i_frm+2, i_frm+3,
		       GTK_FILL,GTK_SHRINK,0,0);

  
    // Block Next
    adj = (GtkAdjustment *)gtk_adjustment_new 
      ((gdouble)mascot->frame_loop[i_ptn][i_frm].next, -1,
       mascot->frame_num[i_ptn]-1, 1.0, 1.0, 0.0);
    my_signal_connect (adj, "value_changed",cc_get_adj,
		       &mascot->frame_loop[i_ptn][i_frm].next);
    spinner =  gtk_spin_button_new (adj, 0, 0);
    gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
    gtkut_table_attach(frame_table[i_ptn], spinner,
		       4, 5, i_frm+2, i_frm+3,
		       GTK_FILL,GTK_SHRINK,0,0);

    // Block Min
    adj = (GtkAdjustment *)gtk_adjustment_new 
      ((gdouble)mascot->frame_loop[i_ptn][i_frm].min, 0,999, 1.0, 1.0, 0.0);
    my_signal_connect (adj, "value_changed",cc_get_adj,
		       &mascot->frame_loop[i_ptn][i_frm].min);
    spinner =  gtk_spin_button_new (adj, 0, 0);
    gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
    gtkut_table_attach(frame_table[i_ptn], spinner,
		       5, 6, i_frm+2, i_frm+3,
		       GTK_FILL,GTK_SHRINK,0,0);
  
    // Block Max
    adj = (GtkAdjustment *)gtk_adjustment_new 
      ((gdouble)mascot->frame_loop[i_ptn][i_frm].max, 0,999, 1.0, 1.0, 0.0);
    my_signal_connect (adj, "value_changed",cc_get_adj,
		       &mascot->frame_loop[i_ptn][i_frm].max);
    spinner =  gtk_spin_button_new (adj, 0, 0);
    gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), FALSE);
    gtkut_table_attach(frame_table[i_ptn], spinner,
		       6, 7, i_frm+2, i_frm+3,
		       GTK_FILL,GTK_SHRINK,0,0);
  
  }
  label=gtkut_label_new("");
  gtkut_table_attach_defaults(frame_table[i_ptn], label, 
			      0, 7, MAX_ANIME_FRAME+2, MAX_ANIME_FRAME+3);


  gtk_widget_show_all(frame_table[i_ptn]);
}


// カテゴリー内ターゲットリスト
void make_tgt_list(typMascot *mascot, int i_cat)
{
  int i_tgt;
  char no_tmp[3];
  GtkWidget *label;
  GtkWidget *button;
  GtkWidget *entry;
  GtkWidget *arrow;
  GtkWidget *hbox;

  
  if(flag_make_tgt_list[i_cat]){
    gtk_widget_destroy(tgt_table[i_cat]);
    g_object_unref(tgt_table[i_cat]);
  }
  else flag_make_tgt_list[i_cat]=TRUE;

  tgt_table[i_cat] = gtkut_table_new (5, MAX_MENU_CATEGORY+2, FALSE,0,0,5);
  //g_object_ref (tgt_table[i_cat]); 
  //gtk_object_sink (GTK_OBJECT (tgt_table[i_cat]));
  
#ifdef USE_GTK3
  gtk_container_add(GTK_CONTAINER(cat_scrwin[i_cat]), tgt_table[i_cat]);
#else
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW (cat_scrwin[i_cat]), tgt_table[i_cat]);
#endif
  
  label = gtkut_label_new (_("No."));
  gtkut_table_attach(tgt_table[i_cat], label, 1, 2, 0, 1,
		     GTK_FILL,GTK_SHRINK,0,0);
  gtkut_pos(label, POS_START, POS_START);
	 
  label = gtkut_label_new (_("Name"));
  gtkut_table_attach(tgt_table[i_cat], label, 2, 3, 0, 1,
		     GTK_FILL,GTK_SHRINK,0,0);
  gtkut_pos(label, POS_CENTER, POS_START);
  
  label = gtkut_label_new (_("File"));
  gtkut_table_attach(tgt_table[i_cat], label, 3, 5, 0, 1,
		     GTK_FILL,GTK_SHRINK,0,0);
  gtkut_pos(label, POS_CENTER, POS_START);
  
  
  for(i_tgt=0;i_tgt<mascot->menu_tgt_max[i_cat];i_tgt++){
    
    if(i_tgt!=0){

      arrow= gtkut_arrow_new(MY_ARROW_UP);
      button = gtk_button_new();
      gtk_container_add (GTK_CONTAINER (button), arrow);
      gtkut_table_attach(tgt_table[i_cat], button,
			 0, 1, i_tgt+1, i_tgt+2,
			 GTK_SHRINK,GTK_SHRINK,0,0);
      my_signal_connect(button,"clicked",mc_up_tgt, 
			(gpointer)mtgt[i_cat][i_tgt]);
    }

    sprintf(no_tmp,"%02d",i_tgt);
    label = gtkut_label_new (no_tmp);
    gtkut_table_attach (tgt_table[i_cat],
			label, 1, 2, i_tgt+1, i_tgt+2,
			GTK_FILL,GTK_SHRINK,0,2);

    // Name
    entry = gtk_entry_new ();
    if(mascot->menu_tgt_name[i_cat][i_tgt]){
      gtk_entry_set_text(GTK_ENTRY(entry),mascot->menu_tgt_name[i_cat][i_tgt]);
    }
    gtkut_table_attach(tgt_table[i_cat], entry,
		       2, 3, i_tgt+1, i_tgt+2,
		       GTK_FILL,GTK_SHRINK,0,0);


    // File
    hbox = gtkut_hbox_new(FALSE,0);
    gtkut_table_attach(tgt_table[i_cat], hbox,
		       3, 4, i_tgt+1, i_tgt+2,
		       GTK_FILL,GTK_SHRINK,0,0);

    entry = gtk_entry_new ();
    if(mascot->menu_tgt[i_cat][i_tgt]){
      gtk_entry_set_text(GTK_ENTRY(entry),mascot->menu_tgt[i_cat][i_tgt]);
    }
    gtk_box_pack_start(GTK_BOX(hbox), entry,FALSE, FALSE, 0);
    
    button=gtkut_button_new_with_icon(NULL,
#ifdef USE_GTK3				       
				      "document-open"
#else
				      GTK_STOCK_OPEN
#endif
				      );
    gtk_box_pack_start(GTK_BOX(hbox), button,FALSE, FALSE, 0);
    my_signal_connect(button,"clicked",create_change_tgt_dialog, 
		      (gpointer)mtgt[i_cat][i_tgt]);
    
    if(i_tgt!=mascot->menu_tgt_max[i_cat]-1){
      arrow= gtkut_arrow_new(MY_ARROW_DOWN);
      button = gtk_button_new();
      gtk_container_add (GTK_CONTAINER (button), arrow);
      gtkut_table_attach(tgt_table[i_cat], button,
			 4, 5, i_tgt+1, i_tgt+2,
			 GTK_SHRINK,GTK_SHRINK,0,0);
      my_signal_connect(button,"clicked",mc_down_tgt, 
			(gpointer)mtgt[i_cat][i_tgt]);
    }

  }

  label=gtkut_label_new("");
  gtkut_table_attach_defaults(tgt_table[i_cat], label, 
			      0, 7, MAX_ANIME_FRAME+2, MAX_ANIME_FRAME+3);


  gtk_widget_show_all(tgt_table[i_cat]);
}


  


// ポップアップメニューの生成
GtkWidget * make_popup_menu(typMascot *mascot)
{
  GtkWidget *popup_menu; 
  GtkWidget *open_menu; 
  GtkWidget *save_menu; 
  GtkWidget *new_menu; 
  GtkWidget *install_menu; 
  GtkWidget *cat_menu; 
  GtkWidget *popup_button;
#ifdef USE_BIFF
  GtkWidget *biff_check;
#endif  // USE_BIFF
  GtkWidget *sig_check;
  GtkWidget *bar;
  GtkWidget *image;

  popup_menu = gtk_menu_new();
  gtk_widget_show(popup_menu);

  open_menu=make_open_menu();
  gtk_widget_show(open_menu);

  popup_button=gtkut_menu_item_new_with_icon(
#ifdef USE_GTK3
					 "document-open",
#else				      
					 GTK_STOCK_OPEN,
#endif
					 GTK_ICON_SIZE_MENU, _("Open"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(popup_button),open_menu);

  save_menu=make_save_menu();
  gtk_widget_show(save_menu);

  popup_button=gtkut_menu_item_new_with_icon(
#ifdef USE_GTK3
					 "document-save",
#else				      
					 GTK_STOCK_SAVE,
#endif
					 GTK_ICON_SIZE_MENU, _("Save"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(popup_button),save_menu);

  new_menu=make_new_menu();
  gtk_widget_show(new_menu);

  popup_button=gtkut_menu_item_new_with_icon(
#ifdef USE_GTK3
					 "document-new",
#else				      
					 GTK_STOCK_NEW,
#endif
					 GTK_ICON_SIZE_MENU, _("New"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(popup_button),new_menu);

  install_menu=make_install_menu();
  gtk_widget_show(install_menu);

  popup_button=gtkut_menu_item_new_with_icon(
#ifdef USE_GTK3
					 "system-software-install",
#else				      
					 GTK_STOCK_HARDDISK,
#endif
					 GTK_ICON_SIZE_MENU, _("Install"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(popup_button),install_menu);



  popup_button=gtkut_menu_item_new_with_icon(
#ifdef USE_GTK3
					 "document-properties",
#else				      
					 GTK_STOCK_PROPERTIES,
#endif
					 GTK_ICON_SIZE_MENU, _("Config"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  my_signal_connect (popup_button, "activate", create_config_dialog, (gpointer)mascot);

  bar =gtk_menu_item_new();
  gtk_widget_show (bar);
  gtk_container_add (GTK_CONTAINER (popup_menu), bar);

#ifdef USE_BIFF
  biff_check =gtk_check_menu_item_new_with_label (_("Biff Check"));
  gtk_widget_show (biff_check);
  gtk_container_add (GTK_CONTAINER (popup_menu), biff_check);
  if(Mascot->mail.flag){
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(biff_check),TRUE);
  }
  else{
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(biff_check),FALSE);
  }
  my_signal_connect (biff_check, "toggled",mail_flag_toggle,NULL);

  bar =gtk_menu_item_new();
  gtk_widget_show (bar);
  gtk_container_add (GTK_CONTAINER (popup_menu), bar);
#endif  // USE_BIFF

  sig_check =gtk_check_menu_item_new_with_label (_("Time Signal"));
  gtk_widget_show (sig_check);
  gtk_container_add (GTK_CONTAINER (popup_menu), sig_check);
  if(Mascot->signal.flag){
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(sig_check),TRUE);
  }
  else{
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(sig_check),FALSE);
  }
  my_signal_connect (sig_check, "toggled",check_menu_get_toggle,
		     &Mascot->signal.flag);


  bar =gtk_menu_item_new();
  gtk_widget_show (bar);
  gtk_container_add (GTK_CONTAINER (popup_menu), bar);

  cat_menu=make_cat_menu();
  gtk_widget_show(cat_menu);

  popup_button=gtkut_menu_item_new_with_icon(
#ifdef USE_GTK3
					 "view-refresh",
#else				      
					 GTK_STOCK_REFRESH,
#endif
					 GTK_ICON_SIZE_MENU, _("Mascot Launcher"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(popup_button),cat_menu);
  if(Mascot->menu_total==0){
    gtk_widget_set_sensitive(popup_button, FALSE);
  }

  bar =gtk_menu_item_new();
  gtk_widget_show (bar);
  gtk_container_add (GTK_CONTAINER (popup_menu), bar);

  popup_button=gtkut_menu_item_new_with_icon(
#ifdef USE_GTK3
					 "application-exit",
#else				      
					 GTK_STOCK_QUIT,
#endif
					 GTK_ICON_SIZE_MENU, _("Exit"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  my_signal_connect (popup_button, "activate",quit_all,NULL);

  gtk_widget_show_all(popup_menu);

  return(popup_menu);
}

GtkWidget * make_open_menu(void)
{
  GtkWidget *popup_menu; 
  GtkWidget *popup_button;
  GtkWidget *bar;
  GtkWidget *launcher_menu; 
  GtkWidget *develop_menu; 
   
  popup_menu = gtk_menu_new();
  gtk_widget_show (popup_menu);

  popup_button =gtk_menu_item_new_with_label (_("Open Mascot"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  my_signal_connect (popup_button, "activate",create_file_selection_dialog,
		     (gpointer)MENU_SELECT);

  bar =gtk_menu_item_new();
  gtk_widget_show (bar);
  gtk_container_add (GTK_CONTAINER (popup_menu), bar);

  launcher_menu=make_open_launcher_menu();
  gtk_widget_show (launcher_menu);

  popup_button =gtk_menu_item_new_with_label (_("Launcher Menu"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(popup_button),launcher_menu);


  bar =gtk_menu_item_new();
  gtk_widget_show (bar);
  gtk_container_add (GTK_CONTAINER (popup_menu), bar);

  develop_menu=make_open_develop_menu();
  gtk_widget_show (develop_menu);

  popup_button =gtk_menu_item_new_with_label (_("For Developper"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(popup_button),develop_menu);

  return(popup_menu);
}

GtkWidget * make_save_menu(void)
{
  GtkWidget *popup_menu; 
  GtkWidget *menu_menu; 
  GtkWidget *develop_menu; 
  GtkWidget *popup_button;
  GtkWidget *bar;

  popup_menu = gtk_menu_new();
  gtk_widget_show (popup_menu);

  popup_button =gtk_menu_item_new_with_label (_("Save All"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  my_signal_connect (popup_button, "activate",MenuSaveAll,NULL);

  bar =gtk_menu_item_new();
  gtk_widget_show (bar);
  gtk_container_add (GTK_CONTAINER (popup_menu), bar);

  popup_button =gtk_menu_item_new_with_label (_("Save Mascot"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  my_signal_connect (popup_button, "activate",MenuSaveMascot,NULL);

  popup_button =gtk_menu_item_new_with_label (_("Save Mascot As..."));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  my_signal_connect (popup_button, "activate",create_save_mascot_dialog,NULL);

  bar =gtk_menu_item_new();
  gtk_widget_show (bar);
  gtk_container_add (GTK_CONTAINER (popup_menu), bar);

  menu_menu=make_menu_menu();
  gtk_widget_show (menu_menu);
  
  popup_button =gtk_menu_item_new_with_label (_("Launcher Menu"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(popup_button),menu_menu);


  bar =gtk_menu_item_new();
  gtk_widget_show (bar);
  gtk_container_add (GTK_CONTAINER (popup_menu), bar);

  popup_button =gtk_menu_item_new_with_label (_("Save Resource"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  my_signal_connect (popup_button, "activate",create_save_rc_dialog,NULL);

  bar =gtk_menu_item_new();
  gtk_widget_show (bar);
  gtk_container_add (GTK_CONTAINER (popup_menu), bar);

  develop_menu=make_develop_menu();
  gtk_widget_show (develop_menu);

  popup_button =gtk_menu_item_new_with_label (_("For Developper"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(popup_button),develop_menu);


  return(popup_menu);
}


GtkWidget * make_new_menu(void)
{
  GtkWidget *popup_menu; 
  GtkWidget *popup_button;
  GtkWidget *bar;
   
  popup_menu = gtk_menu_new();
  gtk_widget_show (popup_menu);

  popup_button =gtk_menu_item_new_with_label (_("Create New Mascot"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  my_signal_connect (popup_button, "activate",create_new_mascot_dialog,NULL);

  bar =gtk_menu_item_new();
  gtk_widget_show (bar);
  gtk_container_add (GTK_CONTAINER (popup_menu), bar);

  popup_button =gtk_menu_item_new_with_label (_("Create New Launcher Menu"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  my_signal_connect (popup_button, "activate",create_new_menu_selection_dialog,
		     NULL);

  return(popup_menu);
}


GtkWidget * make_install_menu(void)
{
  GtkWidget *popup_menu; 
  GtkWidget *popup_button;
  GtkWidget *bar;
   
  popup_menu = gtk_menu_new();
  gtk_widget_show (popup_menu);

  popup_button =gtk_menu_item_new_with_label (_("Launcher Menu [User]"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  my_signal_connect (popup_button, "activate",create_menu_selection_dialog,
		     (gpointer)MENU_INSTALL_USER);

#ifdef USE_COMMON
  popup_button =gtk_menu_item_new_with_label (_("Launcher Menu [Common]"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  my_signal_connect (popup_button, "activate",create_menu_selection_dialog,
		     (gpointer)MENU_INSTALL_COMMON);
  if(!check_common_dir2()){
    gtk_widget_set_sensitive(popup_button, FALSE);
  }
#endif

  bar =gtk_menu_item_new();
  gtk_widget_show (bar);
  gtk_container_add (GTK_CONTAINER (popup_menu), bar);

  popup_button =gtk_menu_item_new_with_label (_("Mascot [User]"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  my_signal_connect (popup_button, "activate",create_file_selection_dialog,
		     (gpointer)MENU_INSTALL_USER);

#ifdef USE_COMMON
  popup_button =gtk_menu_item_new_with_label (_("Mascot [Common]"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  my_signal_connect (popup_button, "activate",create_file_selection_dialog,
		     (gpointer)MENU_INSTALL_COMMON);
  if(!check_common_dir2()){
    gtk_widget_set_sensitive(popup_button, FALSE);
  }
#endif

  return(popup_menu);
}


GtkWidget * make_menu_menu(void)
{
  GtkWidget *popup_menu; 
  GtkWidget *popup_button;
   
  popup_menu = gtk_menu_new();
  gtk_widget_show (popup_menu);

  popup_button =gtk_menu_item_new_with_label (_("Save"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  my_signal_connect (popup_button, "activate",create_save_menu_dialog,NULL);
  if(Mascot->menu_total==0){
    gtk_widget_set_sensitive(popup_button, FALSE);
  }

  popup_button =gtk_menu_item_new_with_label (_("Set as Default"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  my_signal_connect (popup_button, "activate",MenuSaveDefMenu,NULL);
  if(Mascot->menu_total==0){
    gtk_widget_set_sensitive(popup_button, FALSE);
  }

  popup_button =gtk_menu_item_new_with_label (_("Remove from Default"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  my_signal_connect (popup_button, "activate",MenuDelDefMenu,NULL);

  return(popup_menu);
}


GtkWidget * make_open_launcher_menu(void)
{
  GtkWidget *popup_menu; 
  GtkWidget *popup_button;
   
  popup_menu = gtk_menu_new();
  gtk_widget_show (popup_menu);

  popup_button =gtk_menu_item_new_with_label (_("File Select"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  my_signal_connect (popup_button, "activate",create_menu_selection_dialog,
		     (gpointer)MENU_SELECT);

  popup_button =gtk_menu_item_new_with_label (_("Launcher"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  my_signal_connect (popup_button, "activate",create_smenu_dialog2,NULL);

  return(popup_menu);
}


GtkWidget * make_open_develop_menu(void)
{
  GtkWidget *popup_menu; 
  GtkWidget *popup_button;
   
  popup_menu = gtk_menu_new();
  gtk_widget_show (popup_menu);

  popup_button =gtk_menu_item_new_with_label (_("Convert from Nokkari-Chara"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  my_signal_connect (popup_button, "activate",create_nkr_selection_dialog,
		     NULL);

  return(popup_menu);
}


GtkWidget * make_develop_menu(void)
{
  GtkWidget *popup_menu; 
  GtkWidget *popup_button;
   
  popup_menu = gtk_menu_new();
  gtk_widget_show (popup_menu);

  popup_button =gtk_menu_item_new_with_label (_("Save for Release"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  my_signal_connect (popup_button, "activate",
		     create_save_release_mascot_dialog,NULL);

  popup_button =gtk_menu_item_new_with_label (_("Convert to Nokkari-Chara"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  my_signal_connect (popup_button, "activate",create_save_nokkari_dialog,
		     NULL);

  return(popup_menu);
}



GtkWidget * make_cat_menu(void)
{
  GtkWidget *popup_menu; 
  GtkWidget *popup_button;
  GtkWidget *bar;
  GtkWidget *tgt_menu; 
  int i_cat;
    

  popup_menu = gtk_menu_new();
  gtk_widget_show (popup_menu);

  popup_button =gtk_menu_item_new_with_label (_("Random Change"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  my_signal_connect (popup_button, "activate",AllRandomChangeMascotMenu,
		     NULL);

  bar =gtk_menu_item_new();
  gtk_widget_show (bar);
  gtk_container_add (GTK_CONTAINER (popup_menu), bar);

  for(i_cat=0;i_cat<Mascot->menu_cat_max;i_cat++){
    tgt_menu=make_tgt_menu(i_cat);
    gtk_widget_show (tgt_menu);

    popup_button =gtk_menu_item_new_with_label (Mascot->menu_cat[i_cat]);
    gtk_widget_show (popup_button);
    gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(popup_button),tgt_menu);
  }

  return(popup_menu);
}

GtkWidget * make_tgt_menu(int i_cat)
{
  GtkWidget *popup_menu; 
  GtkWidget *popup_button;
  GtkWidget *bar;
  int i_tgt;
    

  popup_menu = gtk_menu_new();
  gtk_widget_show (popup_menu);

  popup_button =gtk_menu_item_new_with_label (_("Random Change"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
  my_signal_connect (popup_button, "activate",CatRandomChangeMascotMenu,
		     i_cat);


  bar =gtk_menu_item_new();
  gtk_widget_show (bar);
  gtk_container_add (GTK_CONTAINER (popup_menu), bar);

  for(i_tgt=0;i_tgt<Mascot->menu_tgt_max[i_cat];i_tgt++){
    popup_button 
      =gtk_menu_item_new_with_label(Mascot->menu_tgt_name[i_cat][i_tgt]);
    gtk_widget_show (popup_button);
    gtk_container_add (GTK_CONTAINER (popup_menu), popup_button);
    my_signal_connect (popup_button, "activate",ChangeMascotMenu,
		       Mascot->menu_tgt[i_cat][i_tgt]);
  }

  return(popup_menu);
}

//マスコット変更
void ChangeMascot(){

#ifdef USE_GTK3
  gdk_rgba_free(Mascot->colclk);
  gdk_rgba_free(Mascot->colclkbg);
  gdk_rgba_free(Mascot->colclkbd);
  gdk_rgba_free(Mascot->colclksd);
  gdk_rgba_free(Mascot->colbal);
  gdk_rgba_free(Mascot->colbalbd);
  gdk_rgba_free(Mascot->colbalbg);
#else
  gdk_color_free(Mascot->colclk);
  gdk_color_free(Mascot->colclkbg);
  gdk_color_free(Mascot->colclkbd);
  gdk_color_free(Mascot->colclksd);
  gdk_color_free(Mascot->colbal);
  gdk_color_free(Mascot->colbalbd);
  gdk_color_free(Mascot->colbalbg);
#endif

  InitMascot(Mascot);
  ReadMascot(Mascot,FALSE);
  InitComposite(Mascot);
  LoadPixmaps(Mascot);
#ifndef USE_GTK3
  ReInitGC(Mascot);
#endif
  map_balloon(Mascot, FALSE);
  flag_balloon=FALSE;
#ifdef USE_BIFF
  map_biff(Mascot, FALSE);
  LoadBiffPixmap(Mascot->biff_pix, Mascot);
  remap_biff_pix(Mascot);
#endif  // USE_BIFF

  //if(Mascot->clkmode!=CLOCK_NO) clock_update(Mascot, TRUE);

  if(Mascot->clkmode==CLOCK_PANEL){
    map_clock(Mascot, TRUE);
  }
  else{
    map_clock(Mascot, FALSE);
  }

  map_main(Mascot, TRUE);

  if(Mascot->clkmode!=CLOCK_NO) clock_update(Mascot, TRUE); 

  if(Mascot->move==MOVE_FIX){
    MoveMascot(Mascot,Mascot->xfix,Mascot->yfix);
  }
  else{
    MoveToFocus(Mascot,TRUE);
  }

  // DrawingArea のrealize
  gtk_widget_realize(Mascot->dw_main);
  gtk_widget_realize(Mascot->dw_balloon);
  gtk_widget_realize(Mascot->dw_clock);
  gtk_widget_realize(Mascot->dw_biff);
#ifdef FG_DRAW
  gtk_widget_realize(Mascot->dw_sdw);
  gtk_widget_realize(Mascot->dw_clkfg);
  gtk_widget_realize(Mascot->dw_balfg);
#endif
}


// のっかりキャラへマスコット変更
void NkrChangeMascot(){
  InitMascot(Mascot);

  NkrRead(Mascot);

  InitComposite(Mascot);
  LoadPixmaps(Mascot);
#ifndef USE_GTK3
  ReInitGC(Mascot);
#endif
  map_balloon(Mascot, FALSE);
  flag_balloon=FALSE;
#ifdef USE_BIFF
  map_biff(Mascot, FALSE);
  LoadBiffPixmap(Mascot->biff_pix, Mascot);
#endif  // USE_BIFF
  
  Mascot->yoff=Mascot->height - Mascot->yoff;

  map_clock(Mascot, FALSE);
  MoveToFocus(Mascot,TRUE);

  // DrawingArea のrealize
  gtk_widget_realize(Mascot->dw_main);
  gtk_widget_realize(Mascot->dw_balloon);
  gtk_widget_realize(Mascot->dw_clock);
  gtk_widget_realize(Mascot->dw_biff);
#ifdef FG_DRAW
  gtk_widget_realize(Mascot->dw_sdw);
  gtk_widget_realize(Mascot->dw_clkfg);
  gtk_widget_realize(Mascot->dw_balfg);
#endif
}



#ifdef USE_COMMON
// ファイル整合チェック
void create_cons_dialog(typMascot *mascot,
			gchar *target_file, 
			time_t user_mtime,
			time_t common_mtime,
			gboolean menu_flag)
{
  GtkWidget *cons_main;
  GtkWidget *cons_tbl;
  GtkWidget *cons_tbl2;
  GtkWidget *cons_text;
  GtkWidget *vbox;
  GtkWidget *label;
  GtkWidget *button;
  GtkWidget *check;
  gchar *tmp_text=NULL;
  gchar tmp_time[128];
  struct tm *tm_ptr;
  consType *cdata;


  // Win構築は重いので先にExposeイベント等をすべて処理してから
  while (my_main_iteration(FALSE));

  Mascot->flag_menu=TRUE;

  cdata=g_malloc0(sizeof(consType));

  cons_main = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  gtk_window_set_position(GTK_WINDOW(cons_main), GTK_WIN_POS_CENTER);

  gtk_window_set_title(GTK_WINDOW(cons_main), 
		       _("MaCoPiX : Consistency Check for Common & User Dir."));
  gtk_widget_realize(cons_main);
  my_signal_connect(cons_main,"destroy",close_cons, 
		    GTK_WIDGET(cons_main));
  gtk_container_set_border_width (GTK_CONTAINER (cons_main), 5);
  
  // 6x3のテーブル
  cons_tbl = gtkut_table_new (6, 3, TRUE,0,0,0);
  gtk_container_add (GTK_CONTAINER (cons_main), cons_tbl);
#ifdef USE_GTK3
  gtk_grid_set_row_homogeneous(GTK_GRID(cons_tbl),TRUE);
#endif

  vbox = gtkut_vbox_new(FALSE,5);
  gtkut_table_attach(cons_tbl, vbox, 0, 6, 0, 1,
		     GTK_SHRINK,GTK_SHRINK,0,0);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 5);


  label = gtkut_label_new(_("WARNING"));
  gtkut_pos(label, POS_CENTER, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 0);

  label = gtkut_label_new("");
  gtkut_pos(label, POS_CENTER, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 0);

  if(menu_flag){
    tmp_text=g_strconcat(_("Menu file : "), "\"", target_file, "\"", NULL);
  }
  else{
    tmp_text=g_strconcat(_("Mascot file : "), "\"", target_file, "\"", NULL);
  }
  label = gtkut_label_new (tmp_text);
  gtkut_pos(label, POS_START, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 0);
  label = gtkut_label_new(_("  File in User Dir is older than the corresponded one in Common Dir."));
  gtkut_pos(label, POS_START, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 0);

  // 2x2のテーブル
  cons_tbl2 = gtkut_table_new (2, 2, FALSE,0,0,0);
  gtk_box_pack_start(GTK_BOX(vbox), cons_tbl2,FALSE, FALSE, 0);


  label = gtkut_label_new(_("     [User Dir]"));
  gtkut_pos(label, POS_START, POS_CENTER);
  gtkut_table_attach(cons_tbl2, label, 0, 1, 0, 1,
		     GTK_SHRINK,GTK_SHRINK,0,0);
  tm_ptr=gmtime(&user_mtime);
  sprintf(tmp_time, _("     %4d/%02d/%02d %02d:%02d:%02d"),
	  tm_ptr->tm_year+1900,tm_ptr->tm_mon+1,tm_ptr->tm_mday,
	  tm_ptr->tm_hour,tm_ptr->tm_min,tm_ptr->tm_sec);
  label = gtkut_label_new(tmp_time);
  gtkut_pos(label, POS_START, POS_CENTER);
  gtkut_table_attach(cons_tbl2, label, 1, 2, 0, 1,
		     GTK_SHRINK,GTK_SHRINK,0,0);
  
  label = gtkut_label_new(_("      [Common Dir]"));
  gtkut_pos(label, POS_START, POS_CENTER);
  gtkut_table_attach(cons_tbl2, label, 0, 1, 1, 2,
		     GTK_SHRINK,GTK_SHRINK,0,0);
  tm_ptr=gmtime(&common_mtime);
  sprintf(tmp_time, _("     %4d/%02d/%02d %02d:%02d:%02d"),
	  tm_ptr->tm_year+1900,tm_ptr->tm_mon+1,tm_ptr->tm_mday,
	  tm_ptr->tm_hour,tm_ptr->tm_min,tm_ptr->tm_sec);
  label = gtkut_label_new(tmp_time);
  gtkut_pos(label, POS_START, POS_CENTER);
  gtkut_table_attach(cons_tbl2, label, 1, 2, 1, 2,
		     GTK_SHRINK,GTK_SHRINK,0,0);

  label = gtkut_label_new(_("Do you want to overwite the newer file to User Dir?"));
  gtkut_pos(label, POS_START, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(vbox), label,FALSE, FALSE, 0);
  
  
  check = gtk_check_button_new_with_label(_("Don't ask this from the next time."));
  gtk_box_pack_start(GTK_BOX(vbox), check, FALSE, FALSE, 0);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),FALSE);

  cdata->dialog=GTK_WIDGET(cons_main);
  cdata->auto_check=GTK_WIDGET(check);
  cdata->filename=target_file;
  cdata->flag_ow=FALSE;
  cdata->flag_auto=FALSE;

  button=gtkut_button_new_with_icon(_("Cancel"),
#ifdef USE_GTK3
				    "process-stop"
#else
				    GTK_STOCK_CANCEL
#endif				     
				    );
  gtkut_table_attach(cons_tbl, button, 4, 5, 2, 3,
		     GTK_FILL|GTK_EXPAND,GTK_SHRINK,0,0);
  my_signal_connect(button,"clicked",noow_cons, (gpointer)cdata);

  button=gtkut_button_new_with_icon(_("OK"),
#ifdef USE_GTK3
				    "emblem-default"
#else
				    GTK_STOCK_OK
#endif				     
				    );
  gtkut_table_attach(cons_tbl, button, 5, 6, 2, 3,
		     GTK_FILL|GTK_EXPAND,GTK_SHRINK,0,0);
  my_signal_connect(button,"clicked",ow_cons, (gpointer)cdata);

  
  gtk_widget_show_all(cons_main);
  
  gtk_main();

  gdkut_flush(mascot);

  mascot->flag_consow=cdata->flag_ow;
  if(cdata->flag_auto){
    if(mascot->flag_consow){
      mascot->cons_check_mode=CONS_AUTOOW;
    }
    else{
      mascot->cons_check_mode=CONS_IGNORE;
    }
  }
  else{
    mascot->cons_check_mode=CONS_MANUAL;
  }

  g_free(cdata);
}
#endif


// 起動メニュー選択ダイアログ
void create_smenu_dialog(typMascot *mascot, gboolean flag_popup)
{
  GtkWidget *main;
  GtkWidget *table;
  GtkWidget *table1;
  GtkWidget *scrwin;
  GtkWidget *button;
  GtkWidget *vscrollbar;
  GtkWidget *vbox, *vbox2;
  GtkWidget *label;
  GtkWidget *menubar;
  typScanMenu *smenu;
  gint i_menu;
  gboolean update_flag=TRUE;
#ifdef USE_WIN32
  GtkWidget *hbox;
#endif
  GtkTargetEntry drag_types[1] = {{"text/uri-list", 0, 0}};

  smenu=g_malloc0(sizeof(typScanMenu));
  selected_smenu=0;  

  ScanMenu(smenu,FALSE);

  // Win構築は重いので先にExposeイベント等をすべて処理してから
  while (my_main_iteration(FALSE));

  mascot->flag_menu=TRUE;

  
  main = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  gtk_window_set_position(GTK_WINDOW(main), GTK_WIN_POS_CENTER);

  if(smenu->max_menu==0){
    /* Drag and Drop */
    gtk_drag_dest_set (main, GTK_DEST_DEFAULT_MOTION
		       | GTK_DEST_DEFAULT_HIGHLIGHT | GTK_DEST_DEFAULT_DROP,
		       drag_types, 1, GDK_ACTION_COPY);
    my_signal_connect (main, "drag-data-received",
		       signal_drag_data_received_smenu, NULL);

    gtk_window_set_title(GTK_WINDOW(main), 
			 _("MaCoPiX : Starting up"));
    gtk_widget_realize(main);
    my_signal_connect(main,"destroy",close_smenu, GTK_WIDGET(main));


    vbox = gtkut_vbox_new(FALSE, 0);
    gtk_container_add (GTK_CONTAINER (main), vbox);

    menubar=make_start_menubar();
    gtk_box_pack_start(GTK_BOX(vbox), menubar,FALSE, FALSE, 0);


    vbox2 = gtkut_vbox_new(FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (vbox2), 10);
    gtk_box_pack_start(GTK_BOX(vbox), vbox2,FALSE, FALSE, 0);


    label=gtkut_label_new(_("No Menu Files are installed!!"));
    gtkut_pos(label, POS_START, POS_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox2),
		       label,TRUE,TRUE,0);

    label=gtkut_label_new(_("Please install one menu file at least,"));
    gtkut_pos(label, POS_START, POS_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox2),
		       label,TRUE,TRUE,0);

    label=gtkut_label_new(_(" or appoint a mascot file in option in order to start MaCoPiX!"));
    gtkut_pos(label, POS_START, POS_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox2),
		       label,TRUE,TRUE,0);

    label=gtkut_label_new("");
    gtkut_pos(label, POS_START, POS_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox2),
		       label,TRUE,TRUE,0);

    label=gtkut_label_new(_("You can download mascot files from MaCoPiX official web."));
    gtkut_pos(label, POS_START, POS_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox2),
		       label,TRUE,TRUE,0);

#ifdef USE_WIN32
    hbox = gtkut_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
    
    button = gtk_button_new_with_label(DEFAULT_URL);
    gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 0);
    gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE);
    my_signal_connect(button,"clicked",uri_clicked, NULL);
#else
    label = gtkut_label_new (DEFAULT_URL);
    gtkut_pos(label, POS_START, POS_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox2),
		       label,TRUE,TRUE,0);
#endif

    label=gtkut_label_new("");
    gtkut_pos(label, POS_START, POS_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox2),
		       label,TRUE,TRUE,0);

    label=gtkut_label_new(_("Menus and Mascots can be installed by the menu bar of this window."));
    gtkut_pos(label, POS_START, POS_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox2),
		       label,TRUE,TRUE,0);
    label=gtkut_label_new(_("Or, drag &amp; drop a mascot menu archive (*.tar.gz) directly"));
    gtkut_pos(label, POS_START, POS_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox2),
		       label,TRUE,TRUE,0);
    label=gtkut_label_new(_("into this window, in order to install it."));
    gtkut_pos(label, POS_START, POS_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox2),
		       label,TRUE,TRUE,0);
		       
    gtk_widget_show_all(main);
    
    gtk_main();
    
    gdkut_flush(Mascot);
    
    if((!Mascot->file)&&(!Mascot->menu_file)) exit(1);

    gtk_widget_destroy(main);
    
  }
  else{

    if(flag_popup){
      gtk_window_set_title(GTK_WINDOW(main), 
			   _("MaCoPiX : Launcher Menu Selection"));
    }
    else{
      gtk_window_set_title(GTK_WINDOW(main), 
			   _("MaCoPiX : Start Menu Selection"));
    }
    gtk_widget_realize(main);
    my_signal_connect(main,"destroy",close_smenu, 
		      GTK_WIDGET(main));
    gtk_container_set_border_width (GTK_CONTAINER (main), 5);
    
    // 2x2のテーブル
    
    
    table = gtkut_table_new (4, 3, FALSE,100,7,0);
    gtk_container_add (GTK_CONTAINER (main), table);
    
    scrwin = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_set_size_request (scrwin, SMENU_WIN_WIDTH, SMENU_WIN_HEIGHT);
    gtkut_table_attach_defaults (table, scrwin, 0, 4, 0, 1);
    gtk_container_set_border_width (GTK_CONTAINER (scrwin), 5);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(scrwin),
				    GTK_POLICY_NEVER,
				    GTK_POLICY_ALWAYS);
    
    make_smenu_list(scrwin, smenu);
    
    
    button=gtkut_button_new_with_icon(_("Cancel"),
#ifdef USE_GTK3
				      "process-stop"
#else
				      GTK_STOCK_CANCEL
#endif				     
				      );
    gtkut_table_attach(table, button, 2, 3, 1, 2,
		       GTK_SHRINK,GTK_SHRINK,0,0);
    my_signal_connect(button,"clicked",close_smenu,GTK_WIDGET(main));
    
    button=gtkut_button_new_with_icon(_("OK"),
#ifdef USE_GTK3
				      "emblem-default"
#else
				      GTK_STOCK_OK
#endif				     
				      );
    gtkut_table_attach(table, button, 3, 4, 1, 2,
		       GTK_SHRINK,GTK_SHRINK,0,0);
    my_signal_connect(button,"clicked",gtk_main_quit, NULL);

    gtk_widget_show_all(main);
    
    gtk_main();
    
    gdkut_flush(mascot);
    
    if(selected_smenu==-1){
      if(!flag_popup) exit(1);
    }
    else{
      if(smenu->dir[selected_smenu]==SMENU_DIR_USER){
#ifdef USE_WIN32
	mascot->menu_file=g_strconcat(get_win_home(),G_DIR_SEPARATOR_S,USER_DIR,
#else
	mascot->menu_file=g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,USER_DIR,
#endif
				      smenu->file[selected_smenu],NULL);
      }
#ifdef USE_COMMON
      else if(smenu->dir[selected_smenu]==SMENU_DIR_COMMON){
	mascot->menu_file=g_strconcat(COMMON_DIR,
				      smenu->file[selected_smenu],NULL);
      }
#endif
      else{
	mascot->menu_file=g_strdup(smenu->file[selected_smenu]);
      }
     
      if(flag_popup){// Popupから
	gtk_widget_destroy(mascot->PopupMenu);
	ReadMenu(mascot,0,NULL);
	mascot->PopupMenu=make_popup_menu(mascot);
      }
      gtk_widget_destroy(main);
    }
  }
    
  g_free(smenu);
}


GtkWidget *make_start_menubar(typMascot *mascot){
  GtkWidget *menu_bar;
  GtkWidget *menu_item;
  GtkWidget *menu;
  GtkWidget *popup_button;
  GtkWidget *bar;

  menu_bar=gtk_menu_bar_new();
  gtk_widget_show (menu_bar);


  menu_item=gtkut_menu_item_new_with_icon(
#ifdef USE_GTK3
				      "application-exit",
#else				      
				      GTK_STOCK_QUIT,
#endif
				      GTK_ICON_SIZE_MENU, _("Exit"));
  gtk_widget_show (menu_item);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), menu_item);
  
  menu=gtk_menu_new();
  gtk_widget_show (menu);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item), menu);
  
  popup_button =gtk_menu_item_new_with_label (_("Exit"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (menu), popup_button);
  my_signal_connect (popup_button, "activate",gtk_main_quit,NULL);

  // Install
  menu_item=gtkut_menu_item_new_with_icon(
#ifdef USE_GTK3
				      "system-software-install",
#else
				      GTK_STOCK_HARDDISK,
#endif				      
				      GTK_ICON_SIZE_MENU,
				      _("Install"));
  gtk_widget_show (menu_item);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), menu_item);
  
  menu=gtk_menu_new();
  gtk_widget_show (menu);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item), menu);
  
  popup_button =gtk_menu_item_new_with_label (_("Launcher Menu [User]"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (menu), popup_button);
  my_signal_connect (popup_button, "activate",do_install_user_menu,
		     (gpointer)mascot);

#ifdef USE_COMMON 
  popup_button =gtk_menu_item_new_with_label (_("Launcher Menu [Common]"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (menu), popup_button);
  my_signal_connect (popup_button, "activate",do_install_common_menu,
		     (gpointer)mascot);
#endif


  bar =gtk_menu_item_new();
  gtk_widget_show (bar);
  gtk_container_add (GTK_CONTAINER (menu), bar);

  popup_button =gtk_menu_item_new_with_label (_("Mascot [User]"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (menu), popup_button);
  my_signal_connect (popup_button, "activate",do_install_user_mascot,
		     (gpointer)mascot);

#ifdef USE_COMMON
  popup_button =gtk_menu_item_new_with_label (_("Mascot [Common]"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (menu), popup_button);
  my_signal_connect (popup_button, "activate",do_install_common_mascot,
		     (gpointer)mascot);
#endif

  // Open
  menu_item=gtkut_menu_item_new_with_icon(
#ifdef USE_GTK3
				      "document-open",
#else
				      GTK_STOCK_OPEN,
#endif				      
				      GTK_ICON_SIZE_MENU,
				      _("Open"));
  gtk_widget_show (menu_item);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), menu_item);
  
  menu=gtk_menu_new();
  gtk_widget_show (menu);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item), menu);
  
  popup_button =gtk_menu_item_new_with_label (_("Installed Mascot"));
  gtk_widget_show (popup_button);
  gtk_container_add (GTK_CONTAINER (menu), popup_button);
  my_signal_connect (popup_button, "activate",do_select_mascot,
		     (gpointer)mascot);

  gtk_widget_show_all(menu_bar);
  return(menu_bar);
}



void do_install_user_menu(GtkWidget *w, gpointer gdata){
  typMascot *mascot;

  mascot=(typMascot *)gdata;
  
  create_menu_selection_dialog(NULL,START_MENU_INSTALL_USER);

  gtk_main_quit();
}


#ifdef USE_COMMON
void do_install_common_menu(GtkWidget *w, gpointer gdata){
  typMascot *mascot;

  mascot=(typMascot *)gdata;
  
  create_menu_selection_dialog(NULL,START_MENU_INSTALL_COMMON);

  gtk_main_quit();
}
#endif

void do_install_user_mascot(GtkWidget *w, gpointer gdata){
  typMascot *mascot;

  mascot=(typMascot *)gdata;
  
  create_file_selection_dialog(NULL,START_MENU_INSTALL_USER);

  gtk_main_quit();
}


#ifdef USE_COMMON
void do_install_common_mascot(GtkWidget *w, gpointer gdata){
  typMascot *mascot;

  mascot=(typMascot *)gdata;
  
  create_file_selection_dialog(NULL,START_MENU_INSTALL_COMMON);

  gtk_main_quit();
}
#endif

void do_select_mascot(GtkWidget *w, gpointer gdata){
  typMascot *mascot;

  mascot=(typMascot *)gdata;
  
  create_file_selection_dialog(NULL,START_MENU_SELECT);

  gtk_main_quit();
}




// 起動メニューリスト
void make_smenu_list(GtkWidget *scrwin, typScanMenu *smenu)
{
  int i_menu;
  gchar tmp[15];
  GtkWidget *check;
  GtkWidget *label;
  GtkWidget *button;
  GtkWidget *smenu_table;
  GSList *group = NULL;
  
  smenu_table = gtkut_table_new (5, MAX_MENU_CATEGORY2+2, FALSE,7,7,5);

#ifdef USE_GTK3
  gtk_container_add(GTK_CONTAINER(scrwin), smenu_table);
#else
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW (scrwin), smenu_table);
#endif
 
  label = gtkut_label_new (_("File"));
  gtkut_table_attach(smenu_table, label, 1, 2, 0, 1,
		     GTK_EXPAND,GTK_SHRINK,0,2);
  gtkut_pos(label, POS_CENTER, POS_CENTER);

  label = gtkut_label_new (_("Dir."));
  gtkut_table_attach(smenu_table, label, 2, 3, 0, 1,
		     GTK_EXPAND,GTK_SHRINK,0,2);
  gtkut_pos(label, POS_CENTER, POS_CENTER);

  label = gtkut_label_new (_("Entry"));
  gtkut_table_attach(smenu_table, label, 3, 4, 0, 1,
		     GTK_EXPAND,GTK_SHRINK,0,2);
  gtkut_pos(label, POS_CENTER, POS_CENTER);

  label = gtkut_label_new (_("Category"));
  gtkut_table_attach(smenu_table, label, 4, 5, 0, 1,
		     GTK_EXPAND|GTK_FILL,GTK_SHRINK,0,2);
  gtkut_pos(label, POS_START, POS_CENTER);

  
  for(i_menu=0;i_menu<smenu->max_menu;i_menu++){
    check = gtk_radio_button_new(group);
    group=gtk_radio_button_get_group(GTK_RADIO_BUTTON(check));
    gtkut_table_attach (smenu_table,
			check, 0, 1, i_menu+1, i_menu+2,
			GTK_SHRINK,GTK_SHRINK,0,2);
    my_signal_connect (check, "toggled",check_menu_get_smenu,
		       i_menu);
    if(i_menu==0){
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),TRUE);
    }
    else{
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),FALSE);
    }

    label = gtkut_label_new (smenu->file[i_menu]);
    gtkut_table_attach(smenu_table, label,
		       1, 2, i_menu+1, i_menu+2,
		       GTK_FILL,GTK_SHRINK,0,2);
    gtkut_pos(label, POS_START, POS_START);
	   
    if(smenu->dir[i_menu]==SMENU_DIR_USER){
      label = gtkut_label_new (_("User"));
    }
#ifdef USE_COMMON
    else if(smenu->dir[i_menu]==SMENU_DIR_COMMON){
      label = gtkut_label_new (_("Common"));
    }
#endif
    else{
      label = gtkut_label_new (_("Unknown"));
    }
    gtkut_table_attach(smenu_table, label,
		       2, 3, i_menu+1, i_menu+2,
		       GTK_FILL,GTK_SHRINK,0,2);
    gtkut_pos(label, POS_START, POS_START);


    if(smenu->flag_combine[i_menu]){
      sprintf(tmp, _("%4d categories"),smenu->num[i_menu]);
    }
    else{
      sprintf(tmp, _("%4d mascots"),smenu->num[i_menu]);
    }
    label = gtkut_label_new (tmp);
    gtkut_table_attach(smenu_table, label,
		       3, 4, i_menu+1, i_menu+2,
		       GTK_FILL,GTK_SHRINK,0,2);
    gtkut_pos(label, POS_START, POS_START);

    if(smenu->flag_combine[i_menu]){
      label = gtkut_label_new (_("-- Combined Menu --"));
    }
    else{
      label = gtkut_label_new (smenu->cat[i_menu]);
    }
    gtkut_table_attach(smenu_table, label,
		       4, 5, i_menu+1, i_menu+2,
		       GTK_FILL,GTK_SHRINK,0,2);
    gtkut_pos(label, POS_START, POS_START);

    
  }
  label=gtkut_label_new("");
  gtkut_table_attach_defaults(smenu_table, label, 
			    0, 3, MAX_MENU_CATEGORY2+1, MAX_MENU_CATEGORY2+2);
  gtk_widget_show_all(smenu_table);

}


void popup_message(GtkWidget *parent, gchar* stock_id,gint delay, ...){
  va_list args;
  gchar *msg1;
  GtkWidget *dialog;
  GtkWidget *label;
  GtkWidget *button;
  GtkWidget *pixmap;
  GtkWidget *hbox;
  GtkWidget *vbox;
  gint timer;

  va_start(args, delay);

  if(delay>0){
    dialog = gtk_dialog_new();
  }
  else{
    dialog = gtk_dialog_new_with_buttons("MaCoPiX : Message",
					 GTK_WINDOW(parent),
					 GTK_DIALOG_MODAL,
#ifdef USE_GTK3
					 _("_OK"),GTK_RESPONSE_OK,
#else
					 GTK_STOCK_OK,GTK_RESPONSE_OK,
#endif
					 NULL);
  }
  gtk_container_set_border_width(GTK_CONTAINER(dialog),5);
  gtk_window_set_transient_for(GTK_WINDOW(dialog),GTK_WINDOW(parent));
  gtk_window_set_title(GTK_WINDOW(dialog),"MaCoPiX : Message");

  if(delay>0){
    timer=g_timeout_add(delay*1000, (GSourceFunc)close_popup,
			(gpointer)dialog);
    my_signal_connect(dialog,"delete-event",destroy_popup, &timer);
  }

  hbox = gtkut_hbox_new(FALSE,2);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 0);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     hbox,FALSE, FALSE, 0);

#ifdef USE_GTK3
  pixmap=gtk_image_new_from_icon_name (stock_id,
				       GTK_ICON_SIZE_DIALOG);
#else
  pixmap=gtk_image_new_from_stock (stock_id,
				   GTK_ICON_SIZE_DIALOG);
#endif

  gtk_box_pack_start(GTK_BOX(hbox), pixmap,FALSE, FALSE, 0);

  vbox = gtkut_vbox_new(FALSE,2);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 0);
  gtk_box_pack_start(GTK_BOX(hbox),vbox,FALSE, FALSE, 0);

  while(1){
    msg1=va_arg(args,gchar*);
    if(!msg1) break;
   
    label=gtkut_label_new(msg1);
    gtkut_pos(label, POS_START, POS_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox),
		       label,TRUE,TRUE,0);
  }

  va_end(args);

  gtk_widget_show_all(dialog);

  if(delay>0){
    gtk_main();
  }
  else{
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
  }
}


gboolean popup_ask(GtkWidget *parent, gchar* stock_id, ...){
  va_list args;
  gchar *msg1;
  GtkWidget *dialog;
  GtkWidget *label;
  GtkWidget *button;
  GtkWidget *pixmap;
  GtkWidget *hbox;
  GtkWidget *vbox;
  gint timer;

  va_start(args, stock_id);

  dialog = gtk_dialog_new_with_buttons("MaCoPiX : Dialog",
				       GTK_WINDOW(parent),
				       GTK_DIALOG_MODAL,
#ifdef USE_GTK3
				       _("_OK"),GTK_RESPONSE_OK,
				       _("_Cancel"),GTK_RESPONSE_CANCEL,
#else
				       GTK_STOCK_OK,GTK_RESPONSE_OK,
				       GTK_STOCK_CANCEL,GTK_RESPONSE_CENCEL,
#endif
				       NULL);
  gtk_container_set_border_width(GTK_CONTAINER(dialog),5);
  gtk_window_set_transient_for(GTK_WINDOW(dialog),GTK_WINDOW(parent));
  gtk_window_set_title(GTK_WINDOW(dialog),"MaCoPiX : Dialog");

  hbox = gtkut_hbox_new(FALSE,2);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 0);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     hbox,FALSE, FALSE, 0);

#ifdef USE_GTK3
  pixmap=gtk_image_new_from_icon_name (stock_id,
				       GTK_ICON_SIZE_DIALOG);
#else
  pixmap=gtk_image_new_from_stock (stock_id,
				   GTK_ICON_SIZE_DIALOG);
#endif

  gtk_box_pack_start(GTK_BOX(hbox), pixmap,FALSE, FALSE, 0);

  vbox = gtkut_vbox_new(FALSE,2);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 0);
  gtk_box_pack_start(GTK_BOX(hbox),vbox,FALSE, FALSE, 0);

  while(1){
    msg1=va_arg(args,gchar*);
    if(!msg1) break;
    
    label=gtkut_label_new(msg1);
    gtkut_pos(label, POS_START, POS_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox),
		       label,TRUE,TRUE,0);
  }

  va_end(args);
  
  gtk_widget_show_all(dialog);
  
  if( gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK){
    gtk_widget_destroy(dialog);
    return(TRUE);
  }
  else{
    if(GTK_IS_WIDGET(dialog)) gtk_widget_destroy(dialog);
    return(FALSE);
  }
}


void popup_progress(typMascot *mascot, gchar *msg1){
  GtkWidget *label;
  GtkWidget *align;

  mascot->pdata = g_malloc (sizeof (ProgressData));
  
  mascot->pdata->dialog = gtk_dialog_new();
  gtk_window_set_position(GTK_WINDOW(mascot->pdata->dialog),
			  GTK_WIN_POS_MOUSE);
  gtk_container_set_border_width(GTK_CONTAINER(mascot->pdata->dialog),5);
  gtk_window_set_title(GTK_WINDOW(mascot->pdata->dialog),
		       _("MaCoPiX : Installing Mascots"));

  label=gtkut_label_new(msg1);
  gtkut_pos(label, POS_START, POS_CENTER);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(mascot->pdata->dialog))),
		     label,TRUE,TRUE,0);
  

  /* GtkProgressBar を生成します。*/
  mascot->pdata->pbar = gtk_progress_bar_new ();
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(mascot->pdata->dialog))),
		     mascot->pdata->pbar,TRUE,TRUE,0);
  //gtk_progress_bar_set_bar_style (GTK_PROGRESS_BAR(mascot->pdata->pbar),
  //				  GTK_PROGRESS_CONTINUOUS);
  gtk_progress_bar_pulse(GTK_PROGRESS_BAR(mascot->pdata->pbar));
#ifdef USE_GTK3
  gtk_orientable_set_orientation (GTK_ORIENTABLE (mascot->pdata->pbar), 
				  GTK_ORIENTATION_HORIZONTAL);
  css_change_pbar_height(mascot->pdata->pbar,15);
#else
  gtk_progress_bar_set_orientation (GTK_PROGRESS_BAR (mascot->pdata->pbar), 
				    GTK_PROGRESS_RIGHT_TO_LEFT);
#endif

  gtk_container_add (GTK_CONTAINER (align), mascot->pdata->pbar);
  gtk_widget_show (mascot->pdata->pbar);

  gtk_widget_show_all(mascot->pdata->dialog);

  gdkut_flush(mascot);
}

void destroy_progress(typMascot *mascot){
  gtk_widget_destroy(mascot->pdata->dialog);
  g_free (mascot->pdata);
}

 
void gui_arg_init(){
  gint i_ptn;
  flagChildDialog=FALSE;
  flag_make_pixmap_list=FALSE;
  flag_make_imgtree=FALSE;
  for(i_ptn=0;i_ptn<MAX_ANIME_PATTERN;i_ptn++){
    flag_make_ptntree[i_ptn]=FALSE;
  }
}

gint select_menu_from_ext(typMascot *mascot, gchar *dest_file){
  gboolean hit_flag;
  gint menu_ext;
  
  hit_flag=FALSE;
  

  if(strcmp(dest_file+strlen(dest_file)-strlen(LZH_EXTENSION)+1,
	    LZH_EXTENSION+1)==0){
#ifdef USE_LHA32
    mascot->menu_file=unlha_menu(dest_file);
    mascot->installed_menu_dir=my_dirname(mascot->menu_file);
    hit_flag=TRUE;
    menu_ext=MENU_LHA;
#endif
   }
  else if(strcmp(dest_file+strlen(dest_file)-strlen(TAR_EXTENSION)+1,
		 TAR_EXTENSION+1)==0){
#if defined(USE_GTAR) || defined(USE_TAR32)
    mascot->menu_file=untar_menu(mascot,dest_file);
    mascot->installed_menu_dir=my_dirname(mascot->menu_file);
    hit_flag=TRUE;
    menu_ext=MENU_TAR;
#endif
  }
   else{
     mascot->menu_file=g_strdup(dest_file);
     hit_flag=TRUE;
     menu_ext=MENU_MENU;
   }

  if(!hit_flag){
    popup_message(mascot->win_main,
#ifdef USE_GTK3
		  "dialog-warning", 
#else
		  GTK_STOCK_DIALOG_WARNING,
#endif
		  POPUP_TIMEOUT*3,
		  _("Selected File\n"),
		  dest_file,
		  _("cannot be opend as MaCoPiX menu.\n"),
		  NULL);
  }

  return(menu_ext);

}

void unlink_all(gchar *dirname)
{
  struct dirent *entry;
  DIR *dirp;
  gchar *rm_file;
  GtkWidget *dialog;
  GtkWidget *label;
  GtkWidget *button;
  gchar *tmp;
  
  Mascot->flag_menu=TRUE;
  
  dialog = gtk_dialog_new_with_buttons(_("MaCoPiX : Deletion of temporay files"),
				       NULL,
				       GTK_DIALOG_MODAL,
#ifdef USE_GTK3
				       "_Cancel",GTK_RESPONSE_CANCEL,
				       "_OK",GTK_RESPONSE_OK,
#else
				       GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
				       GTK_STOCK_OK,GTK_RESPONSE_OK,
#endif
				       NULL);
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(dialog),5);
  
  label=gtkut_label_new(_("Install of the new menu file has been completed."));
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     label,TRUE,TRUE,0);
  label=gtkut_label_new(" ");
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		     label,TRUE,TRUE,0);
  label=gtkut_label_new(_("Remove the temporary direcotry?"));
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
		       label,TRUE,TRUE,0);
  tmp=g_strdup_printf("\"%s\"",to_utf8(dirname));
  label=gtkut_label_new(tmp);
  g_free(tmp);
  gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
  		     label,TRUE,TRUE,0);

  gtk_widget_show_all(dialog);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {    
    gtk_widget_destroy(dialog);
    
    if((dirp = opendir(dirname)) != NULL){
      while((entry = readdir(dirp))!=NULL){
	rm_file=g_strconcat(dirname,G_DIR_SEPARATOR_S,entry->d_name,NULL);
	unlink(rm_file);
	g_free(rm_file);
      }
      closedir(dirp);
      rmdir(dirname);
    }
  }
  else {
    gtk_widget_destroy(dialog);
    Mascot->flag_menu=FALSE;
    return;
  }
  
  Mascot->flag_menu=FALSE;
  return;
}



void make_img_tree(typMascot *mascot){
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *sw;
  GtkWidget *button;
  GtkTreeModel *items_model;
  
  if(flag_make_imgtree){
    gtk_widget_destroy(mascot->imgtree);
  }
  else flag_make_imgtree=TRUE;

  items_model = imgtree_create_items_model (mascot);

  /* create tree view */
  mascot->imgtree = gtk_tree_view_new_with_model (items_model);
  gtk_tree_selection_set_mode (gtk_tree_view_get_selection (GTK_TREE_VIEW (mascot->imgtree)),
			       GTK_SELECTION_SINGLE);
  imgtree_add_columns (mascot, GTK_TREE_VIEW (mascot->imgtree), 
		       items_model);

  g_object_unref(items_model);
  
#ifdef USE_GTK3
  gtk_container_add (GTK_CONTAINER (mascot->sw_imgtree), mascot->imgtree);
#else
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW (mascot->sw_imgtree),
					mascot->imgtree);
#endif

  g_signal_connect (mascot->imgtree, "cursor-changed",
		    G_CALLBACK (focus_imgtree_item), (gpointer)mascot);
  g_signal_connect(mascot->imgtree, "row-activated", 
		   G_CALLBACK (act_imgtree_item), (gpointer)mascot);
  
  gtk_widget_show_all(mascot->imgtree);

}


static void
imgtree_add_columns (typMascot *mascot,
		     GtkTreeView  *treeview, 
		     GtkTreeModel *items_model)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;  

  /* number column */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set_data (G_OBJECT (renderer), "column", 
  		     GINT_TO_POINTER (COLUMN_IMGTREE_NUMBER));
  column=gtk_tree_view_column_new_with_attributes (_("No."),
						   renderer,
						   "text",
						   COLUMN_IMGTREE_NUMBER,
#ifdef USE_GTK3
						   "foreground-rgba", COLUMN_IMGTREE_COLFG,
						   "background-rgba", COLUMN_IMGTREE_COLBG,
#else
						   "foreground-gdk", COLUMN_IMGTREE_COLFG,
						   "background-gdk", COLUMN_IMGTREE_COLBG,
#endif
						   NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW (treeview),column);


  /* Filename column */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set_data (G_OBJECT (renderer), "column", 
  		     GINT_TO_POINTER (COLUMN_IMGTREE_FILENAME));
  column=gtk_tree_view_column_new_with_attributes (_("Image File Name"),
						   renderer,
						   "text", 
						   COLUMN_IMGTREE_FILENAME,
#ifdef USE_GTK3
						   "foreground-rgba", COLUMN_IMGTREE_COLFG,
						   "background-rgba", COLUMN_IMGTREE_COLBG,
#else
						   "foreground-gdk", COLUMN_IMGTREE_COLFG,
						   "background-gdk", COLUMN_IMGTREE_COLBG,
#endif
						   
						   NULL);
  gtk_tree_view_column_set_cell_data_func(column, renderer,
					  imgtree_cell_data_func,
					  GUINT_TO_POINTER(COLUMN_IMGTREE_FILENAME),
					  NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW (treeview),column);
}

static GtkTreeModel *
imgtree_create_items_model (typMascot *mascot)
{
  gint i_pix = 0;
  GtkListStore *model;
  GtkTreeIter iter;

  /* create list store */
  model = gtk_list_store_new (NUM_IMGTREE_COLUMNS, 
			      G_TYPE_INT,     // number
			      G_TYPE_STRING,  // Filename
#ifdef USE_GTK3
			      GDK_TYPE_RGBA,   //fgcolor
			      GDK_TYPE_RGBA   //bgcolor
#else
			      GDK_TYPE_COLOR,  //fgcolor
			      GDK_TYPE_COLOR  //bgcolor
#endif
			      );  

  //gtk_list_store_set_column_types (GTK_LIST_STORE (model), 1, 
  //			   (GType []){ G_TYPE_STRING }); // NOTE
  for (i_pix = 0; i_pix < MAX_PIXMAP; i_pix++){
    if(!mascot->sprites[i_pix].filename) break;
    gtk_list_store_append (model, &iter);
    imgtree_update_item(mascot, GTK_TREE_MODEL(model), iter, i_pix);
  }
  
  return GTK_TREE_MODEL (model);
}


void imgtree_update_item(typMascot *mascot, 
			 GtkTreeModel *model, 
			 GtkTreeIter iter, 
			 gint i_pix)
{
  gtk_list_store_set (GTK_LIST_STORE(model), &iter,
		      COLUMN_IMGTREE_NUMBER, i_pix,
		      COLUMN_IMGTREE_FILENAME,  mascot->sprites[i_pix].filename,
		      COLUMN_IMGTREE_COLFG, &color_black,
		      COLUMN_IMGTREE_COLBG, (i_pix%2==0) ? &color_white : &color_pale3,
		      -1);
}


static void
focus_imgtree_item (GtkWidget *widget, gpointer data)
{
  GtkTreeIter iter;
  typMascot *mascot = (typMascot *)data;
  GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW(mascot->imgtree));
  GtkTreeSelection *selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(mascot->imgtree));
  gint i_pix;

  if (gtk_tree_selection_get_selected (selection, NULL, &iter)){
    GtkTreePath *path;
    
    path = gtk_tree_model_get_path (model, &iter);
    gtk_tree_model_get (model, &iter, COLUMN_IMGTREE_NUMBER, &i_pix, -1);
    gtk_tree_path_free (path);

    TestLoadPixmaps(mascot,
		    mascot->sprites[i_pix].filename,
		    i_pix);
    mascot->imgtree_i=i_pix;
  }
 
}


static void act_imgtree_item(GtkTreeView        *treeview,
			     GtkTreePath        *path,
			     GtkTreeViewColumn  *col,
			     gpointer            gdata){
  typMascot *mascot = (typMascot *)gdata;
  GtkTreeModel *model;
  GtkTreeIter   iter;
  gint i_pix;
  
  model = gtk_tree_view_get_model(treeview);
  
  if (gtk_tree_model_get_iter(model, &iter, path)){
    
    i_pix = gtk_tree_path_get_indices (path)[0];

    create_change_image_dialog(mascot, i_pix);

    imgtree_update_item(mascot, model, iter, i_pix);
  }
}


void imgtree_cell_data_func(GtkTreeViewColumn *col , 
			    GtkCellRenderer *renderer,
			    GtkTreeModel *model, 
			    GtkTreeIter *iter,
			    gpointer user_data)
{
  const guint index = GPOINTER_TO_UINT(user_data);
  guint64 size;
  gint int_value;
  gchar *c_buf, *str;

  switch (index) {
  case COLUMN_IMGTREE_FILENAME:
    gtk_tree_model_get (model, iter, 
			index, &c_buf,
			-1);
    break;
  }

  switch (index) {
  case COLUMN_IMGTREE_FILENAME:
    str=g_path_get_basename(c_buf);
    break;
  }

  g_object_set(renderer, "text", str, NULL);
  if(str)g_free(str);
}


/// Pattern
void make_ptn_tree(typMascot *mascot, gint i_ptn){
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *sw;
  GtkWidget *button;
  GtkTreeModel *items_model;
  
  if(flag_make_ptntree[i_ptn]){
    gtk_widget_destroy(mascot->ptntree[i_ptn]);
  }
  else flag_make_ptntree[i_ptn]=TRUE;

  items_model = ptntree_create_items_model (mascot, i_ptn);

  /* create tree view */
  mascot->ptntree[i_ptn] = gtk_tree_view_new_with_model (items_model);
  gtk_tree_selection_set_mode (gtk_tree_view_get_selection (GTK_TREE_VIEW (mascot->ptntree[i_ptn])),
			       GTK_SELECTION_SINGLE);
  ptntree_add_columns (mascot, GTK_TREE_VIEW (mascot->ptntree[i_ptn]), 
		       items_model, i_ptn);

  g_object_unref(items_model);
  
#ifdef USE_GTK3
  gtk_container_add(GTK_CONTAINER(ptn_scrwin[i_ptn]), mascot->ptntree[i_ptn]);
#else
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW (ptn_scrwin[i_ptn]),
					mascot->ptntree[i_ptn]);
#endif

  g_signal_connect (mascot->ptntree[i_ptn], "cursor-changed",
		    G_CALLBACK (focus_ptntree_item), (gpointer)mptn[i_ptn]);
  
  gtk_widget_show_all(mascot->ptntree[i_ptn]);
}


static void
ptntree_add_columns (typMascot *mascot,
		     GtkTreeView  *treeview, 
		     GtkTreeModel *items_model,
		     gint i_ptn)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;  

  /* number column */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set_data (G_OBJECT (renderer), "column", 
  		     GINT_TO_POINTER (COLUMN_PTNTREE_NUMBER));
  column=gtk_tree_view_column_new_with_attributes (_("No."),
						   renderer,
						   "text",
						   COLUMN_PTNTREE_NUMBER,
#ifdef USE_GTK3
						   "foreground-rgba", COLUMN_PTNTREE_COLFG,
						   "background-rgba", COLUMN_PTNTREE_COLBG,
#else
						   "foreground-gdk", COLUMN_PTNTREE_COLFG,
						   "background-gdk", COLUMN_PTNTREE_COLBG,
#endif
						   NULL);
  gtk_tree_view_column_set_cell_data_func(column, renderer,
					  ptntree_cell_data_func,
					  GUINT_TO_POINTER(COLUMN_PTNTREE_NUMBER),
					  NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW (treeview),column);


  /* Image column */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set_data (G_OBJECT (renderer), "column", 
  		     GINT_TO_POINTER (COLUMN_PTNTREE_IMAGE));
  column=gtk_tree_view_column_new_with_attributes (_("Image"),
						   renderer,
						   "text", 
						   COLUMN_PTNTREE_IMAGE,
#ifdef USE_GTK3
						   "foreground-rgba", COLUMN_PTNTREE_COLFG,
						   "background-rgba", COLUMN_PTNTREE_COLBG,
#else
						   "foreground-gdk", COLUMN_PTNTREE_COLFG,
						   "background-gdk", COLUMN_PTNTREE_COLBG,
#endif					   
						   NULL);
  gtk_tree_view_column_set_cell_data_func(column, renderer,
					  ptntree_cell_data_func,
					  GUINT_TO_POINTER(COLUMN_PTNTREE_IMAGE),
					  NULL);
  g_object_set (renderer, "editable", TRUE, NULL);
  g_signal_connect (renderer, "edited", G_CALLBACK (ptntree_cell_edited), (gpointer)mptn[i_ptn]);
  gtk_tree_view_append_column(GTK_TREE_VIEW (treeview),column);


  /* Min column */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set_data (G_OBJECT (renderer), "column", 
  		     GINT_TO_POINTER (COLUMN_PTNTREE_MIN));
  column=gtk_tree_view_column_new_with_attributes (_("Min"),
						   renderer,
						   "text", 
						   COLUMN_PTNTREE_MIN,
#ifdef USE_GTK3
						   "foreground-rgba", COLUMN_PTNTREE_COLFG,
						   "background-rgba", COLUMN_PTNTREE_COLBG,
#else
						   "foreground-gdk", COLUMN_PTNTREE_COLFG,
						   "background-gdk", COLUMN_PTNTREE_COLBG,
#endif					   
						   NULL);
  gtk_tree_view_column_set_cell_data_func(column, renderer,
					  ptntree_cell_data_func,
					  GUINT_TO_POINTER(COLUMN_PTNTREE_MIN),
					  NULL);
  g_object_set (renderer, "editable", TRUE, NULL);
  g_signal_connect (renderer, "edited", G_CALLBACK (ptntree_cell_edited), (gpointer)mptn[i_ptn]);
  gtk_tree_view_append_column(GTK_TREE_VIEW (treeview),column);


  /* Max column */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set_data (G_OBJECT (renderer), "column", 
  		     GINT_TO_POINTER (COLUMN_PTNTREE_MAX));
  column=gtk_tree_view_column_new_with_attributes (_("Max"),
						   renderer,
						   "text", 
						   COLUMN_PTNTREE_MAX,
#ifdef USE_GTK3
						   "foreground-rgba", COLUMN_PTNTREE_COLFG,
						   "background-rgba", COLUMN_PTNTREE_COLBG,
#else
						   "foreground-gdk", COLUMN_PTNTREE_COLFG,
						   "background-gdk", COLUMN_PTNTREE_COLBG,
#endif					   
						   NULL);
  gtk_tree_view_column_set_cell_data_func(column, renderer,
					  ptntree_cell_data_func,
					  GUINT_TO_POINTER(COLUMN_PTNTREE_MAX),
					  NULL);
  g_object_set (renderer, "editable", TRUE, NULL);
  g_signal_connect (renderer, "edited", G_CALLBACK (ptntree_cell_edited), (gpointer)mptn[i_ptn]);
  gtk_tree_view_append_column(GTK_TREE_VIEW (treeview),column);


  /* Block Loop : Next Frame column */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set_data (G_OBJECT (renderer), "column", 
  		     GINT_TO_POINTER (COLUMN_PTNTREE_BL_NEXT));
  column=gtk_tree_view_column_new_with_attributes (_("Block Loop : Next Frame"),
						   renderer,
						   "text", 
						   COLUMN_PTNTREE_BL_NEXT,
#ifdef USE_GTK3
						   "foreground-rgba", COLUMN_PTNTREE_COLFG,
						   "background-rgba", COLUMN_PTNTREE_COLBG,
#else
						   "foreground-gdk", COLUMN_PTNTREE_COLFG,
						   "background-gdk", COLUMN_PTNTREE_COLBG,
#endif					   
						   NULL);
  gtk_tree_view_column_set_cell_data_func(column, renderer,
					  ptntree_cell_data_func,
					  GUINT_TO_POINTER(COLUMN_PTNTREE_BL_NEXT),
					  NULL);
  g_object_set (renderer, "editable", TRUE, NULL);
  g_signal_connect (renderer, "edited", G_CALLBACK (ptntree_cell_edited), (gpointer)mptn[i_ptn]);
  gtk_tree_view_append_column(GTK_TREE_VIEW (treeview),column);


  /* Block Loop : Min column */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set_data (G_OBJECT (renderer), "column", 
  		     GINT_TO_POINTER (COLUMN_PTNTREE_BL_MIN));
  column=gtk_tree_view_column_new_with_attributes (_("Block Loop : Min"),
						   renderer,
						   "text", 
						   COLUMN_PTNTREE_BL_MIN,
#ifdef USE_GTK3
						   "foreground-rgba", COLUMN_PTNTREE_COLFG,
						   "background-rgba", COLUMN_PTNTREE_COLBG,
#else
						   "foreground-gdk", COLUMN_PTNTREE_COLFG,
						   "background-gdk", COLUMN_PTNTREE_COLBG,
#endif					   
						   NULL);
  gtk_tree_view_column_set_cell_data_func(column, renderer,
					  ptntree_cell_data_func,
					  GUINT_TO_POINTER(COLUMN_PTNTREE_BL_MIN),
					  NULL);
  g_object_set (renderer, "editable", TRUE, NULL);
  g_signal_connect (renderer, "edited", G_CALLBACK (ptntree_cell_edited), (gpointer)mptn[i_ptn]);
  gtk_tree_view_append_column(GTK_TREE_VIEW (treeview),column);


  /* Block Loop : Max column */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set_data (G_OBJECT (renderer), "column", 
  		     GINT_TO_POINTER (COLUMN_PTNTREE_BL_MAX));
  column=gtk_tree_view_column_new_with_attributes (_("Block Loop : Max"),
						   renderer,
						   "text", 
						   COLUMN_PTNTREE_BL_MAX,
#ifdef USE_GTK3
						   "foreground-rgba", COLUMN_PTNTREE_COLFG,
						   "background-rgba", COLUMN_PTNTREE_COLBG,
#else
						   "foreground-gdk", COLUMN_PTNTREE_COLFG,
						   "background-gdk", COLUMN_PTNTREE_COLBG,
#endif					   
						   NULL);
  gtk_tree_view_column_set_cell_data_func(column, renderer,
					  ptntree_cell_data_func,
					  GUINT_TO_POINTER(COLUMN_PTNTREE_BL_MAX),
					  NULL);
  g_object_set (renderer, "editable", TRUE, NULL);
  g_signal_connect (renderer, "edited", G_CALLBACK (ptntree_cell_edited), (gpointer)mptn[i_ptn]);
  gtk_tree_view_append_column(GTK_TREE_VIEW (treeview),column);
}

static GtkTreeModel *
ptntree_create_items_model (typMascot *mascot, gint i_ptn)
{
  gint i_frm = 0;
  GtkListStore *model;
  GtkTreeIter iter;

  /* create list store */
  model = gtk_list_store_new (NUM_PTNTREE_COLUMNS, 
			      G_TYPE_INT,     // number
			      G_TYPE_INT,     // image
			      G_TYPE_INT,     // min
			      G_TYPE_INT,     // max
			      G_TYPE_INT,     // BL : next
			      G_TYPE_INT,     // BL : min
			      G_TYPE_INT,     // BL : max
#ifdef USE_GTK3
			      GDK_TYPE_RGBA,   //fgcolor
			      GDK_TYPE_RGBA   //bgcolor
#else
			      GDK_TYPE_COLOR,  //fgcolor
			      GDK_TYPE_COLOR  //bgcolor
#endif
			      );  

  //gtk_list_store_set_column_types (GTK_LIST_STORE (model), 1, 
  //			   (GType []){ G_TYPE_STRING }); // NOTE
  for (i_frm = 0; i_frm < mascot->frame_num[i_ptn]; i_frm++){
    gtk_list_store_append (model, &iter);
    ptntree_update_item(mascot, GTK_TREE_MODEL(model), iter, i_ptn, i_frm);
  }
  
  return GTK_TREE_MODEL (model);
}


void ptntree_update_item(typMascot *mascot, 
			 GtkTreeModel *model, 
			 GtkTreeIter iter,
			 gint i_ptn,
			 gint i_frm)
{
  gtk_list_store_set (GTK_LIST_STORE(model), &iter,
		      COLUMN_PTNTREE_NUMBER, i_frm,
		      COLUMN_PTNTREE_IMAGE,  mascot->frame_pix[i_ptn][i_frm],
		      COLUMN_PTNTREE_MIN,    mascot->frame_min[i_ptn][i_frm],
		      COLUMN_PTNTREE_MAX,    mascot->frame_max[i_ptn][i_frm],
		      COLUMN_PTNTREE_BL_NEXT, mascot->frame_loop[i_ptn][i_frm].next,
		      COLUMN_PTNTREE_BL_MIN,  mascot->frame_loop[i_ptn][i_frm].min,
		      COLUMN_PTNTREE_BL_MAX,  mascot->frame_loop[i_ptn][i_frm].max,
		      COLUMN_PTNTREE_COLFG, &color_black,
		      COLUMN_PTNTREE_COLBG, (i_frm%2==0) ? &color_white : &color_pale3,
		      -1);
}


static void
focus_ptntree_item (GtkWidget *widget, gpointer gdata)
{
  confNum *mptn=(confNum *)gdata;
  typMascot *mascot = mptn->mascot;
  gint i_ptn=mptn->num;
  GtkTreeIter iter;
  GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW(widget));
  GtkTreeSelection *selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(widget));
  gint i_frm, i_pix;

  if (gtk_tree_selection_get_selected (selection, NULL, &iter)){
    GtkTreePath *path;
    
    path = gtk_tree_model_get_path (model, &iter);
    gtk_tree_model_get (model, &iter, COLUMN_PTNTREE_NUMBER, &i_frm, -1);
    gtk_tree_model_get (model, &iter, COLUMN_PTNTREE_IMAGE, &i_pix, -1);
    gtk_tree_path_free (path);

    TestLoadPixmaps(mascot,
		    mascot->sprites[i_pix].filename,
		    i_pix);
    mascot->ptntree_i_frm[i_ptn]=i_frm;
  }
 
}


void ptntree_cell_data_func(GtkTreeViewColumn *col , 
			    GtkCellRenderer *renderer,
			    GtkTreeModel *model, 
			    GtkTreeIter *iter,
			    gpointer user_data)
{
  const guint index = GPOINTER_TO_UINT(user_data);
  guint64 size;
  gint int_value;
  gchar *c_buf, *str;

  switch (index) {
  case COLUMN_PTNTREE_NUMBER:
  case COLUMN_PTNTREE_IMAGE:
  case COLUMN_PTNTREE_MIN:
  case COLUMN_PTNTREE_MAX:
  case COLUMN_PTNTREE_BL_NEXT:
  case COLUMN_PTNTREE_BL_MIN:
  case COLUMN_PTNTREE_BL_MAX:
    gtk_tree_model_get (model, iter, 
			index, &int_value,
			-1);
    break;
  }

  switch (index) {
  case COLUMN_PTNTREE_NUMBER:
    str=g_strdup_printf("%02d", int_value+1);
    break;

  case COLUMN_PTNTREE_BL_NEXT:
    if(int_value<0){
      str=NULL;
    }
    else{
      str=g_strdup_printf("%02d", int_value+1);
    }
    break;
    
  case COLUMN_PTNTREE_BL_MIN:
  case COLUMN_PTNTREE_BL_MAX:
    if(int_value<=0){
      str=NULL;
    }
    else{
      str=g_strdup_printf("%d", int_value);
    }
    break;
    
  default:
    str=g_strdup_printf("%d", int_value);
    break;
  }

  g_object_set(renderer, "text", str, NULL);
  if(str)g_free(str);
}


static void
ptntree_cell_edited (GtkCellRendererText *cell,
		     const gchar         *path_string,
		     const gchar         *new_text,
		     gpointer             gdata)
{
  confNum *mptn=(confNum *)gdata;
  gint i_ptn = mptn->num;
  typMascot *mascot = mptn->mascot;
  GtkTreePath *path = gtk_tree_path_new_from_string (path_string);
  GtkTreeIter iter;
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(mascot->ptntree[i_ptn]));
  gint column = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (cell), "column"));
  gchar *old_text;
  gint i_frm;

  gtk_tree_model_get_iter (model, &iter, path);

  gtk_tree_model_get (model, &iter, COLUMN_PTNTREE_NUMBER, &i_frm, -1);

  switch (column) {
  case COLUMN_PTNTREE_IMAGE:
    {
      gint i_pix;
      
      i_pix=atoi(new_text);
  
      if(i_pix<0){
	i_pix=0;
      }
      else if(i_pix>=mascot->nPixmap){
	i_pix=mascot->nPixmap-1;
      }
      
      mascot->frame_pix[i_ptn][i_frm]=i_pix;
    }
    gtk_list_store_set (GTK_LIST_STORE (model), &iter, column,
			mascot->frame_pix[i_ptn][i_frm], -1);
    
    break;
    
  case COLUMN_PTNTREE_MIN:
    {
      gint min, max, m_tmp;
      
      max=mascot->frame_max[i_ptn][i_frm];
      min=atoi(new_text);
      if(min<1){
	min=1;
      }
      else if(min > max){
	m_tmp=min;
	min=max;
	max=m_tmp;
      }

      mascot->frame_min[i_ptn][i_frm]=min;
      mascot->frame_max[i_ptn][i_frm]=max;
    }
    gtk_list_store_set (GTK_LIST_STORE (model), &iter,
			column, mascot->frame_min[i_ptn][i_frm],
			COLUMN_PTNTREE_MAX, mascot->frame_max[i_ptn][i_frm],
			-1);
    break;
    
  case COLUMN_PTNTREE_MAX:
    {
      gint min, max, m_tmp;
      
      min=mascot->frame_min[i_ptn][i_frm];
      max=atoi(new_text);
      if(max<1){
	max=1;
      }
      else if(max < min){
	m_tmp=max;
	max=min;
	min=m_tmp;
      }
      
      mascot->frame_min[i_ptn][i_frm]=min;
      mascot->frame_max[i_ptn][i_frm]=max;
    }
    gtk_list_store_set (GTK_LIST_STORE (model), &iter,
			column, mascot->frame_max[i_ptn][i_frm],
			COLUMN_PTNTREE_MIN, mascot->frame_min[i_ptn][i_frm],
			-1);
    break;

  case COLUMN_PTNTREE_BL_NEXT:
    {
      gint next, min, max;

      next=atoi(new_text)-1;
      min=mascot->frame_loop[i_ptn][i_frm].min;
      max=mascot->frame_loop[i_ptn][i_frm].max;
      
      if(next>=i_frm){
	next=i_frm;
      }
      
      if(next<0){
	next=-1;
	min=0;
	max=0;
      }
      else{
	if(next > mascot->frame_num[i_ptn]-1){
	  next = mascot->frame_num[i_ptn]-1;
	}
	if(min<1){
	  min=1;
	}
	if(max<min){
	  max=min;
	}
      }
      
      mascot->frame_loop[i_ptn][i_frm].next=next;
      mascot->frame_loop[i_ptn][i_frm].min=min;
      mascot->frame_loop[i_ptn][i_frm].max=max;
    }
    gtk_list_store_set (GTK_LIST_STORE (model), &iter,
			column, mascot->frame_loop[i_ptn][i_frm].next,
			COLUMN_PTNTREE_BL_MIN, mascot->frame_loop[i_ptn][i_frm].min,
			COLUMN_PTNTREE_BL_MAX, mascot->frame_loop[i_ptn][i_frm].max,
			-1);
    break;

  case COLUMN_PTNTREE_BL_MIN:
    {
      gint min, max, m_tmp, next;
      
      max=mascot->frame_loop[i_ptn][i_frm].max;
      next=mascot->frame_loop[i_ptn][i_frm].next;
      min=atoi(new_text);

      if(next<0){
	min=0;
	max=0;
      }
      else{
	if(min<1){
	  min=1;
	}
	if(min > max){
	  m_tmp=min;
	  min=max;
	  max=m_tmp;
	}
      }

      mascot->frame_loop[i_ptn][i_frm].min=min;
      mascot->frame_loop[i_ptn][i_frm].max=max;
    }
    gtk_list_store_set (GTK_LIST_STORE (model), &iter,
			column, mascot->frame_loop[i_ptn][i_frm].min,
			COLUMN_PTNTREE_BL_MAX, mascot->frame_loop[i_ptn][i_frm].max,
			-1);
    break;

  case COLUMN_PTNTREE_BL_MAX:
    {
      gint min, max, m_tmp, next;
      
      min=mascot->frame_loop[i_ptn][i_frm].min;
      next=mascot->frame_loop[i_ptn][i_frm].next;
      max=atoi(new_text);
      
      if(next<0){
	min=0;
	max=0;
      }
      else{
	if(max<1){
	  max=1;
	}
	else if(max < min){
	  m_tmp=max;
	  max=min;
	  min=m_tmp;
	}
      }

      mascot->frame_loop[i_ptn][i_frm].min=min;
      mascot->frame_loop[i_ptn][i_frm].max=max;
    }
    gtk_list_store_set (GTK_LIST_STORE (model), &iter,
			column, mascot->frame_loop[i_ptn][i_frm].max,
			COLUMN_PTNTREE_BL_MIN, mascot->frame_loop[i_ptn][i_frm].min,
			-1);
    break;
  }
  
  gtk_tree_path_free (path);
  ptntree_update_item(mascot, GTK_TREE_MODEL(model), iter, i_ptn, i_frm);
}


void ptntree_select_frame(GtkWidget *tree, gint set_frm, gint max_frm){
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree));
  GtkTreePath *path;
  GtkTreeIter  iter;
  gint i_frm;

  path=gtk_tree_path_new_first();
  
  for(i_frm=0;i_frm<max_frm;i_frm++){
    gtk_tree_model_get_iter (model, &iter, path);
    gtk_tree_model_get (model, &iter, COLUMN_PTNTREE_NUMBER, &i_frm, -1);
    
    if(i_frm==set_frm){
      gtk_widget_grab_focus (tree);
      gtk_tree_view_set_cursor(GTK_TREE_VIEW(tree), path, NULL, FALSE);
      break;
    }
    else{
      gtk_tree_path_next(path);
    }
  }
  gtk_tree_path_free(path);
}
