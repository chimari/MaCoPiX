//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//
//
//      main.h  
//      Configuration header  for  MaCoPiX
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
//
//  difference between Gtk+2 version
//
//  - Translucent mascot and biff images
//       gdk_window_set_opacity
//       Gtk+ 2.12
//  
//  - Tooltip for clock  (__GTK_TOOLTIP_H__)
//       Gtk+2.12
//     If no, use Gtktooltips.
//
//  - Auto detection of composited or not 
//      gtk_widget_is_composited
//       Gtk+2.10
//     If no, flag_composite=COMPISTE_UNKNOWN
//
//  - Text layout for Cairo (__PANGOCAIRO_H__)
//      pango/pangocairo.h
//       Gtk+2.10
//     If no, use cairo_show_text.
//
//  - Icon on GNOME system tray  (__GTK_STATUS_ICON_H__)
//       gtkstatusicon.h
//       Gtk+2.10 
//
//  - Translucent Clock and Balloon (USE_CAIRO)
//       cairo/cairo.h
//       Gtk+2.8??
//
//  - Combo Box    (__GTK_COMBO_BOX__)
//       gtkcombobox.h
//       Gtk+ 2.4 
//     If no, use GtkCombo.
//
//  - Color Button    (__GTK_CLOR_BUTTON__)
//       gtkcolorbutton.h
//       Gtk+ 2.4 
//     If no, use GtkColorSelection and GtkButton.
//
//  - File Chooser    (__GTK_FILE_CHOOSER__)
//       gtkfilechooser.h
//       Gtk+ 2.4 
//     If no, use GtkFileSelection.
//
//  - Icon on menus    (__GTK_STOCK_H__)
//       gtkstock.h
//       Gtk+ 2.2 (Some Items could be replaced by Gtk+ version.)
//
//


#ifndef MAIN_H
#define MAIN_H 1

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif  


#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>

#include<gtk/gtk.h>
#include<gdk-pixbuf/gdk-pixbuf.h>
//#include <gdk_imlib.h>

#ifndef DISABLE_CAIRO
#if defined(CAIRO_H)
#define USE_CAIRO
#endif
#endif

#ifdef USE_CAIRO
#include <cairo.h>
#endif

#include "version.h"
#include "configfile.h"
#include "intl.h"
#include "libpop.h"

// Homepage URL
#define DEFAULT_URL "http://rosegray.sakura.ne.jp/"

// ポップアップメッセージ
#define GTK_MSG

// マスコットファイル用拡張子
#define MASCOT_EXTENSION "*.mcpx"

// メニューファイル用拡張子
#define MENU_EXTENSION "*.menu"

// LZHファイル用拡張子
#define LZH_EXTENSION "*.lzh"

// tar.gzファイル用拡張子
#define TAR_EXTENSION "*.tar.gz"

#define MENU_EXTRACT_GTAR_COMMAND "tar -zxf %s -C %s "

// MENU
enum{ MENU_MENU,
	MENU_LHA,
	MENU_TAR
	}MENU_EXT;


// 画像ファイル用拡張子
#define PNG_EXTENSION "*.png"
#define GIF_EXTENSION "*.gif"
#define XPM_EXTENSION "*.xpm"

// リソースファイル用拡張子
#ifdef USE_WIN32
#define RC_EXTENSION "*.ini"
#else
#define RC_EXTENSION "*.rc"
#endif

// のっかりキャラ定義ファイル用拡張子
#define NKR_EXTENSION "*.ini"

// のっかりキャラ用 BMPファイル拡張子
#define BMP_EXTENSION "*.bmp"

// 個人用設定セーブディレクトリ
//  (homeのしたからの相対ディレクトリ)
#ifdef USE_WIN32
#define USER_DIR "UserData" G_DIR_SEPARATOR_S
#else
#define USER_DIR ".macopix" G_DIR_SEPARATOR_S
#endif
#define PIXDIR "pixmap" G_DIR_SEPARATOR_S
#define SOUNDDIR "sound" G_DIR_SEPARATOR_S

// Folder
enum{ FOLDER_DEFAULT,
	FOLDER_PIX,
	FOLDER_SOUND,
	FOLDER_CURRENT
	}MaCoPiXFolder;

// 個人用セーブファイル
//  (USER_DIR中に作成 : マスコット非依存パラメータを保存)
#ifdef USE_WIN32
#define USER_RCFILE "macopix.ini"
#else
#define USER_RCFILE "macopix.rc"
#endif

// カテゴリー新規作成時のテンポラリネーム
#define TMP_CATEGORY_NAME "(New Category)"

// マスコットのデフォルトコード
#define DEF_CODE1 "EUC-JP"
#define DEF_CODE2 "SJIS-WIN"


// フォント
#ifdef USE_WIN32

#define FONT_CLK "arial bold 9"
#define FONT_BAL "ms pgothic 9"

#else
#ifdef USE_GTK2

#define FONT_CLK "Sans 14"
#define FONT_BAL "Sans 10"

#else

#define FONT_CLK "-alias-fixed-bold-r-normal-*-*-160-*-*-c-*-*"
#define FONT_BAL "-alias-fixed-bold-r-normal-*-*-120-*-*-c-*-*"

#endif
#endif


// 時計更新時間 [msec]
#define INTERVAL 100


// CLOCK_MODE
enum{ CLOCK_NO,
      CLOCK_PIXMAP,
      CLOCK_PANEL
}ClockMode;

//ANIME
#define MAX_PIXMAP   64
#define MAX_ANIME_FRAME   64
#define MAX_ANIME_PATTERN 10


//MENU
#define MAX_MENU_CATEGORY   20
#define MAX_MENU_CATEGORY2  99
#define MAX_MENU_TARGET     40

//DEFAULT PARAMETER for Clock & Balloon
#ifdef USE_WIN32
#define INIT_CLK_POS   20
#define INIT_CLK_TEXT   4
#define INIT_CLK_BORDER 0
#else
#define INIT_CLK_POS   20
#define INIT_CLK_TEXT   5
#define INIT_CLK_BORDER 2
#endif
#define INIT_CLK_SD   1

#define INIT_BAL_TEXT   3
#define INIT_BAL_BORDER 1


// Home Position 指定時のオフセット
//  OS, Wndow Manager, XのVersionで変化するかも?
#define ROOTOFF_X 0
#define ROOTOFF_Y (-10)

// Home Position Mode
enum{ HOMEPOS_NEVER, HOMEPOS_VANISH, HOMEPOS_BAR } HomePos;


// MOVE mode
enum{
  MOVE_FIX,
    MOVE_FOCUS
    }MoveMode;

// Focus Follow 基準位置
enum{ FF_SIDE_LEFT, FF_SIDE_RIGHT } FFPos;

// Focus Autobar タイトルバー算出法
enum{ AUTOBAR_MANUAL, AUTOBAR_ORDINAL, AUTOBAR_COMPIZ } AutoBar;


// Balloon Position
enum{
  BAL_POS_LEFT,
    BAL_POS_RIGHT
    } PosBalloon;

// Balloon Mode
enum{BALLOON_NORMAL,
       BALLOON_MAIL,
       BALLOON_POPERROR,
       BALLOON_SOCKMSG,
       BALLOON_DUET,
       BALLOON_SYS
       } TypBalloon;

// Biff用 Balloonの自然消滅コマ数
#define BALLOON_EXPIRE    150

// Socket用 Balloonの自然消滅コマ数
#define SOCK_BALLOON_EXPIRE    100

// Sockmsg　でふきだしをタイピング風にするかどうか
typedef enum {
  SOCK_NORMAL,
  SOCK_STEPPING
} SockMsgType;

// Sockmsg タイピングでのコマ間隔
#define SOCK_INTERVAL    1

// Sys用 Balloonの自然消滅コマ数
#define SYS_BALLOON_EXPIRE    50

// マスコットSocketの反応ディレイ コマ数
#define DEF_DUET_DELAY    20

// Duet Anime mode
enum{
  DUET_CLICK,
    DUET_RANDOM
    }DuetAnimeMode;


// Interpolation Style for Magnification
enum{	 MAG_IP_NEAREST,
	 MAG_IP_TILES,
	 MAG_IP_BILINEAR,
	 MAG_IP_HYPER
	   } TypInterpolate;


//Clock タイプ
enum{ CLOCK_TYPE_24S, 
	CLOCK_TYPE_24M, 
	CLOCK_TYPE_12S, 
	CLOCK_TYPE_12M } ClockType;


// Font size ratio for AM/PM sign
#ifdef USE_CAIRO
#define CLOCK_AMPM_RATIO 0.6
#endif

// Install mode
enum{   MENU_SELECT, 
	  MENU_INSTALL_USER,
	  MENU_INSTALL_COMMON,
	  START_MENU_SELECT,
	  START_MENU_INSTALL_USER,
	  START_MENU_INSTALL_COMMON,
 } MenuSelect;

#if GTK_CHECK_VERSION(2,12,0) || defined(USE_CAIRO) || defined(USE_WIN32)
//DEFAULT Alpha
#define DEF_ALPHA_MAIN 100
#ifdef USE_BIFF
#define DEF_ALPHA_BIFF 100
#endif
#endif

#ifdef USE_WIN32
#define DEF_ALPHA_BAL 80
#define DEF_ALPHA_CLK 50
#endif

#ifdef USE_CAIRO
#define CAIRO_DEF_ALPHA_OTHER 0xFFFF
#define CAIRO_DEF_ALPHA_SDW 0xB000
#ifdef USE_WIN32
#define CAIRO_DEF_ALPHA_CLK 0xFFFF
#define CAIRO_DEF_ALPHA_BAL 0xFFFF
#else
#define CAIRO_DEF_ALPHA_CLK 0x9000
#define CAIRO_DEF_ALPHA_BAL 0xB000
#endif
#endif

#ifdef USE_CAIRO // SHADOW

#ifdef USE_WIN32
#define CAIRO_SHADOW_X 2.0
#define CAIRO_SHADOW_Y 2.0
#else
#define CAIRO_SHADOW_X 2.3
#define CAIRO_SHADOW_Y 2.3
#endif

#define CAIRO_SHADOW_ALPHA 40
#endif


//DEFAULT COLOR
#ifdef USE_WIN32
#define COLOR_CLK_R 0xffff
#define COLOR_CLK_G 0xf600
#define COLOR_CLK_B 0xde00

#define COLOR_CLKSD_R 0x0000
#define COLOR_CLKSD_G 0x0000
#define COLOR_CLKSD_B 0x0000

#define COLOR_CLKBG_R 0x2100
#define COLOR_CLKBG_G 0x0000
#define COLOR_CLKBG_B 0x0000

#define COLOR_CLKBD_R 0xffff
#define COLOR_CLKBD_G 0xdc00
#define COLOR_CLKBD_B 0xbf00
#else
#define COLOR_CLK_R 0xffff
#define COLOR_CLK_G 0x0000
#define COLOR_CLK_B 0x0000

#define COLOR_CLKSD_R 0x7fff
#define COLOR_CLKSD_G 0x7fff
#define COLOR_CLKSD_B 0x7fff

#define COLOR_CLKBG_R 0xffff
#define COLOR_CLKBG_G 0xcfff
#define COLOR_CLKBG_B 0xcfff

#define COLOR_CLKBD_R 0xcfff
#define COLOR_CLKBD_G 0x7fff
#define COLOR_CLKBD_B 0x7fff
#endif
#define COLOR_BAL_R 0x4B55
#define COLOR_BAL_G 0x30E2
#define COLOR_BAL_B 0x2527

#define COLOR_BALBG_R 0xffff
#define COLOR_BALBG_G 0xfa80
#define COLOR_BALBG_B 0xdbff

#define COLOR_BALBD_R 0x7e9f
#define COLOR_BALBD_G 0x67d2
#define COLOR_BALBD_B 0x53f4

// for Callback of Configuration Dialog
enum{CONF_FONT_CLK,	   
       CONF_FONT_BAL,	   
       CONF_DEF_FONT_CLK,
       CONF_DEF_FONT_BAL,
       INIT_DEF_FONT_CLK,
       INIT_DEF_FONT_BAL} GuiFontConf;

enum{CONF_COLOR_CLK,
       CONF_COLOR_CLKBG,
       CONF_COLOR_CLKBD,
       CONF_COLOR_CLKSD,
       CONF_COLOR_BAL,  
       CONF_COLOR_BALBG,
       CONF_COLOR_BALBD,
       CONF_DEF_COLOR_CLK,  
       CONF_DEF_COLOR_CLKBG,
       CONF_DEF_COLOR_CLKBD,
       CONF_DEF_COLOR_CLKSD,
       CONF_DEF_COLOR_BAL,  
       CONF_DEF_COLOR_BALBG,
       CONF_DEF_COLOR_BALBD} GuiColorConf;

enum{ SET_RELEASE_BALLOON, SET_RELEASE_CLOCK } SetReleaseData;



// Setting for BIFF
#define DEF_MAIL_INTERVAL 60


enum{ MAIL_NO, MAIL_LOCAL, MAIL_POP3, MAIL_APOP, MAIL_QMAIL, MAIL_PROCMAIL } MailStatus0;

enum{ MAIL_PIX_LEFT, MAIL_PIX_RIGHT } MailPixPos;

#define POP_DEBUG  /* pop3 debugging mode */

#define POP3_PORT_NO         110       /* pop3 port */
#ifdef USE_SSL
#define POP3_SSL_PORT_NO         995       /* pop3 over SSL port */
#endif

#define POP3_OK              0         /* pop3 ok */
#define POP3_OK_NORMAL       0         /* pop3 ok */
#define POP3_OK_FS_OVER      1         /* pop3 ok but FS overflow */

#define POP3_ERROR           (-1)      /* pop3 error status for data read */
#define POP3_SSL_CERT        (-2)      /* pop3 error status for waiting ssl certificate */
#define POP3_ERROR_MESSAGE   "POP access error"

#define POP3_MAX_FS  40
#define SPAM_M_SPAMASSASIN    "X-Spam-Flag: YES"
#define SPAM_M_POPFILE    "X-Text-Classification: spam"
#define SPAM_CHECK   TRUE

#define PROCMAILRC G_DIR_SEPARATOR_S ".procmailrc"
#define MH_MAIL_DIR G_DIR_SEPARATOR_S "Mail"
#define PROCMAIL_LOG G_DIR_SEPARATOR_S "procmail.log"

#define BIFF_TOOLTIPS   TRUE


enum{ NO_MAIL, OLD_MAIL, NEW_MAIL, KEEP_NEW_MAIL } MailStatus;
// mail status; array subscripts

#define BIFF_WIN_WIDTH  540  // Default_Size of Biff Window
#define BIFF_WIN_HEIGHT 250


#define NKR_WIN_WIDTH  540  // Size of NKR Convert Log Window
#define NKR_WIN_HEIGHT 150


#define SMENU_WIN_WIDTH  500  // Size of SMENU Window
#define SMENU_WIN_HEIGHT 200

// カーソル
#define CURSOR_NORMAL   GDK_TCROSS
#define CURSOR_PUSH     GDK_CROSS_REVERSE
#define CURSOR_DRAG_H   GDK_SB_H_DOUBLE_ARROW
#define CURSOR_DRAG_F   GDK_PLUS
#define CURSOR_BIFF     GDK_QUESTION_ARROW
#define CURSOR_CLK      GDK_ARROW

// エラーポップアップのタイムアウト[sec]
#define POPUP_TIMEOUT 2


// 時報用設定
enum{ SIGACT_NO, SIGACT_CLICK, SIGACT_CHANGE } SignalAction;


// Consistency Check
enum{ CONS_MANUAL, CONS_AUTOOW, CONS_IGNORE } ConsMode;

// メニューの場所
enum{ SMENU_DIR_COMMON, SMENU_DIR_USER } ScanMenuDir;


typedef enum {
	SSL_NONE,
	SSL_TUNNEL,
	SSL_STARTTLS
} SSLType;

typedef enum {
	SSL_CERT_NONE,
	SSL_CERT_ACCEPT,
	SSL_CERT_DENY
} SSLCertRes;


// COMPOSITE_FLAG
enum{ COMPOSITE_FALSE,
      COMPOSITE_TRUE,
      COMPOSITE_UNKNOWN
}CompositeFlag;





// ランダム生成用関数
#define RANDOMIZE() srand(time(NULL)+getpid())
#define RANDOM(x)  (rand()%(x))

// プログレスバー用構造体
typedef struct _ProgressData {
  GtkWidget *pbar;
  GtkWidget *dialog;
} ProgressData;


// ブロックループ用構造体
typedef struct{
  int next;
  int seq;
  int seqend;
  int min;
  int max;
}BlockLoop;


// 画像ファイル用構造体
typedef struct{
  //gchar **xpm_data;
  gchar *filename;
  GdkPixmap *pixmap;
  GdkBitmap *mask;
#ifdef USE_WIN32
  GdkPixmap *pixmap_sdw;
  GdkBitmap *mask_sdw;
#endif
}typSprite;

// ふたつのglong構造体　(x,y) (w,h)など
typedef struct{
  glong x;
  glong y;
  gboolean flag;
}MyXY;

// カーソル構造体
typedef struct _typCursor typCursor;
struct _typCursor{
  GdkCursor *normal;
  GdkCursor *push;
  GdkCursor *drag_f;
  GdkCursor *drag_h;
  GdkCursor *biff;
  GdkCursor *clk;
};

// 時報用構造体
typedef struct _typSignal typSignal;
struct _typSignal{
  gint type;
  gchar *com;
  gboolean flag;
};

#ifdef USE_CAIRO
// PangoCairo Font変換用
typedef struct _myPangoCairo myPangoCairo;
struct _myPangoCairo{
  gchar *family;
  gdouble pointsize;
  cairo_font_slant_t slant;
  cairo_font_weight_t weight;
};
#endif

// メニュースキャン用構造体
typedef struct _typScanMenu typScanMenu;
struct _typScanMenu{
  gchar *file[MAX_MENU_CATEGORY2];
  gint  dir[MAX_MENU_CATEGORY2];
  gint  num[MAX_MENU_CATEGORY2];
  gchar *cat[MAX_MENU_CATEGORY2];
  gboolean flag_combine[MAX_MENU_CATEGORY2];
  gint  max_menu;
};

// メイル用構造体
typedef struct _typMail typMail;
struct _typMail{
  gboolean flag;
  gint  type;
  gchar *pop_id;
  gchar *pop_pass;
  gboolean pop_save;
  gchar *pop_server;
  gint  pop_port;
  gboolean  pop_child_fl;
  gboolean pop_readed;
  gchar *file;
  gchar *polling;
  gchar *mailer;
  gint  interval;
  gchar *pix_file;
  gint  pix_pos;
  gint  pix_x;
  gint  pix_y;
  gchar *word;
  gchar *sound;
  off_t    size;
  time_t   last_check;
  int      count;            // mailcount in server 
  int      new_count;
  int      fetched_count;    // fetched fs this access
  int      displayed_count;  // displayed fs
  signed int      status;
  signed int      pop3_fs_status;
  gint proc_id;
  GdkPixmap *pixmap;
  GdkBitmap *mask;
  //GtkWidget *w_draw;
  //GtkWidget *e_draw;
#ifndef __GTK_TOOLTIP_H__
  GtkTooltips *tooltips;
#endif
  gboolean  tooltips_fl;
  gboolean drag;
  gchar *fs_max;
  gint win_width;
  gint win_height;
  gboolean spam_check;
  gchar *spam_mark;
  gint spam_count;
  gint  pop_max_fs;
  gchar *last_f;
  gchar *last_s;
  guint ssl_mode;
  gint ssl_cert_res;
  gboolean ssl_cert_skip;
  gboolean ssl_nonblock;
  gchar *ssl_sub;
  gchar *ssl_iss;
  glong ssl_verify;
 };




typedef struct _typMascot typMascot;
struct _typMascot{
  char *file;
  char *rcfile;
  gchar *inifile;
  gchar *name;
  gchar *copyright;
  int nPixmap;
  int x;
  int y;
  int xfix;
  int yfix;
  int height;
  int width;
#ifdef USE_WIN32
  int sdw_height;
#endif
  gint magnify;
  gint ip_style;
  int height_root;
  int width_root;
  gboolean flag_install;
  gboolean flag_common;
  gboolean flag_ow;
  gboolean flag_ow_ini;
  gboolean drag;
  gboolean clk_drag;
#ifdef USE_GTK2
  PangoFontDescription  *fontclk;
  PangoFontDescription  *fontbal;
#else
  GdkFont *fontclk;
  GdkFont *fontbal;
#endif
  gchar *fontname_bal;  
  gchar *fontname_clk;  
  gchar *deffontname_bal;  
  gchar *deffontname_clk;  
#ifdef USE_CAIRO
  myPangoCairo fontbal_pc;
  myPangoCairo fontclk_pc;
#endif
  typSprite *sprites;
  int clkmode;
  int clk_x;
  int clk_y;
  int clktext_x;
  int clktext_y;
  gint clksd_x;
  gint clksd_y;
  int wclkbd;
  int wbalbd;
  int baltext_x;
  int baltext_y;
  int balseq;
  gint balwidth;
  gint balheight;
  //GtkWidget *w_drawing;
  GdkGC *gc_main[2];
  GdkGC *gc_mainsd[2];
  GdkGC *gc_clk[2];
  GdkGC *gc_clksd[2];
  GdkGC *gc_clkbg[2];
  GdkGC *gc_clkbd[2];
  GdkGC *gc_clkmask[2];
  GdkGC *gc_bal[2];
  GdkGC *gc_balbg[2];
  GdkGC *gc_balbd[2];
  GdkGC *gc_balmask[2];
  GdkColor *def_colclk;
  GdkColor *def_colclksd;
  GdkColor *def_colclkbg;
  GdkColor *def_colclkbd;
  GdkColor *def_colbal;
  GdkColor *def_colbalbg;
  GdkColor *def_colbalbd;
  GdkColor *colclk;
  GdkColor *colclksd;
  GdkColor *colclkbg;
  GdkColor *colclkbd;
  GdkColor *colbal;
  GdkColor *colbalbg;
  GdkColor *colbalbd;
#ifdef USE_CAIRO
  gint def_alpclk;
  gint def_alpclksd;
  gint def_alpclkbg;
  gint def_alpclkbd;
  gint def_alpbal;
  gint def_alpbalbg;
  gint def_alpbalbd;
  gint alpclk;
  gint alpclksd;
  gint alpclkbg;
  gint alpclkbd;
  gint alpbal;
  gint alpbalbg;
  gint alpbalbd;
  gboolean force_def_alpha;
  gint alpha_main;
  gint def_alpha_main;
#ifdef USE_BIFF
  gint alpha_biff;
  gint def_alpha_biff;
#endif
#endif
#ifdef USE_WIN32
  gint alpha_bal;
  gint def_alpha_bal;
  gint alpha_clk;
  gint def_alpha_clk;
#endif
#ifdef USE_CAIRO
  gboolean flag_img_cairo;
  gboolean flag_bal_cairo;
  gboolean flag_clk_cairo;
#endif
  gboolean flag_clksd;
  gboolean flag_clkrd;
#ifdef USE_WIN32
  gboolean def_flag_clkfg;
  gboolean flag_clkfg;
  gboolean def_flag_balfg;
  gboolean flag_balfg;
#endif
  gint clktype;
  gboolean clk_pm;
  char digit[20];  
  int anime_ptn;
  int anime_frm;
  int anime_seq;
  int anime_seqend;
  int frame_num[MAX_ANIME_PATTERN];
  int frame_pix[MAX_ANIME_PATTERN][MAX_ANIME_FRAME];
  int frame_min[MAX_ANIME_PATTERN][MAX_ANIME_FRAME];
  int frame_max[MAX_ANIME_PATTERN][MAX_ANIME_FRAME];
  BlockLoop frame_loop[MAX_ANIME_PATTERN+1][MAX_ANIME_FRAME];
  int random_weight[MAX_ANIME_PATTERN];
  int click_weight[MAX_ANIME_PATTERN];
  char *click_word[MAX_ANIME_PATTERN];
  gint bal_lxoff[MAX_ANIME_PATTERN];
  gint bal_lyoff[MAX_ANIME_PATTERN];
  gint bal_rxoff[MAX_ANIME_PATTERN];
  gint bal_ryoff[MAX_ANIME_PATTERN];
  int random_total;
  int click_total;
  gint move;
  gboolean raise_force;
  gboolean raise_kwin;
  guint focus_autobar;
#ifdef USE_WIN32
  gboolean task_force;
#endif
  gboolean no_capbar;
  gint bar_size;
  gint bar_offset;
  gint xoff;
  gint yoff;
  gint homepos_nf;
  gint homepos_out;
  gint homepos_nb;
  gboolean home_auto;
  gboolean home_auto_vanish;
  gint home_x;
  gint home_y;
  gint ff_side;
  gint offset;
  gboolean flag_xp;
  gint offsetp;
  gboolean flag_menu;
  gint pixmap_page;
  gint bal_page;
  gint clk_page;
  gint bal_mode;
  gint bal_defpos;
  gint bal_pos;
  gchar *menu_file;
  gchar *installed_menu_dir;
  gchar *menu_cat[MAX_MENU_CATEGORY];
  gchar *menu_tgt[MAX_MENU_CATEGORY][MAX_MENU_TARGET];
  gchar *menu_tgt_name[MAX_MENU_CATEGORY][MAX_MENU_TARGET];
  gint menu_cat_max;
  gint menu_tgt_max[MAX_MENU_CATEGORY];
  gint menu_total;
  gchar *sound_command;
#ifndef USE_WIN32
  gchar *tar_command;
#endif
  gchar *click_sound[MAX_ANIME_PATTERN];
  typSignal signal;
  typMail mail;
  gboolean flag_consow;
  gint cons_check_mode;
  gchar *sockmsg;
  gint  sockmsg_expire;
  gint  sockmsg_expire_def;
  gint  sockmsg_type;
  gint  sockmsg_step;
  gchar *sysmsg;
  gboolean duet_open;
  gchar   *duet_file;
  gchar   *duet_tgt[MAX_ANIME_PATTERN];
  gint     duet_ptn[MAX_ANIME_PATTERN];
  gchar   *duet_word[MAX_ANIME_PATTERN];
  gint     duet_delay[MAX_ANIME_PATTERN];
  gint     duet_seq;
  gint     duet_mode;
  gboolean duet_use_click;
  gboolean duet_use_random;
  typCursor cursor;
  gchar    *code;  
  gchar    *menu_code;  
  ProgressData *pdata;
//#if GTK_CHECK_VERSION(2, 10, 0)
#ifdef __GTK_STATUS_ICON_H__
  GtkStatusIcon *tray_icon;
  gboolean tray_icon_flag;
#endif
#if GTK_CHECK_VERSION(2,12,0) || defined(USE_CAIRO)
  gint flag_composite;
  gint force_composite;
#endif
#ifdef USE_CAIRO
  gint    sdw_flag;
  gfloat  sdw_x;
  gfloat  sdw_y;
  gint    sdw_alpha;
#endif
  gint sdw_x_int;
  gint sdw_y_int;
};

#endif
