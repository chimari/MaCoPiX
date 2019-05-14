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
#endif

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
#include<ctype.h>
#include<errno.h>

#include<gtk/gtk.h>
#include<gdk-pixbuf/gdk-pixbuf.h>
#include <cairo.h>
#include <signal.h>

#if HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif

#ifdef USE_WIN32
#ifdef USE_BIFF
#include <winsock2.h>
#endif
#include <windows.h>
#include <gdk/gdkwin32.h>
#include <mmsystem.h>
#define usleep(x) g_usleep(x)
#else // for USE_WIN32
#ifdef USE_BIFF
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif
#include <gdk/gdkx.h>
#include <X11/Xatom.h>
#endif

#include "resources.h"
#include "version.h"
#include "configfile.h"
#include "intl.h"
#include "libpop.h"
#include "sockmsg.h"
#include "gtkut.h"


#if HAVE_SYS_UTSNAME_H
#include <sys/utsname.h>
#endif

#include <string.h>
#include <dirent.h>
#include <locale.h>

#ifdef USE_SSL
#include<fcntl.h>
#include "ssl.h"
#endif

#ifndef USE_GTK3
#ifdef __GTK_STATUS_ICON_H__
#define USE_GTK_STATUS_ICON 1
#endif
#endif

#undef USE_PANGOCAIRO

#ifdef USE_WIN32
#ifndef USE_GTK3
#define FG_DRAW
#endif
#endif


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
#define FONT_CLK "Sans 14"
#define FONT_BAL "Sans 10"
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

// Titlebar Offset for Focus Follow
enum{ FF_BAR_ABS, FF_BAR_REL };


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
#define CLOCK_AMPM_RATIO 0.6

// Install mode
enum{   MENU_SELECT, 
	  MENU_INSTALL_USER,
	  MENU_INSTALL_COMMON,
	  START_MENU_SELECT,
	  START_MENU_INSTALL_USER,
	  START_MENU_INSTALL_COMMON,
 } MenuSelect;

//DEFAULT Alpha
#define DEF_ALPHA_MAIN 100
#ifdef USE_BIFF
#define DEF_ALPHA_BIFF 100
#endif

#define DEF_ALPHA_BAL 80
#define DEF_ALPHA_CLK 60
#define DEF_ALPHA_OTHER 100
#define DEF_ALPHA_SDW 40

#ifdef FG_DRAW
#define CAIRO_SHADOW_X 2.0
#define CAIRO_SHADOW_Y 2.0
#else
#define CAIRO_SHADOW_X 2.3
#define CAIRO_SHADOW_Y 2.3
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

#ifdef USE_GTK3
static GdkRGBA color_comment = {0.87, 0.00, 0.00, 1};
static GdkRGBA color_focus =   {0.53, 0.27, 0.00, 1};
static GdkRGBA color_calib =   {0.00, 0.53, 0.00, 1};
static GdkRGBA color_black =   {0.00, 0.00, 0.00, 1};
static GdkRGBA color_red   =   {1.00, 0.00, 0.00, 1};
static GdkRGBA color_blue =    {0.00, 0.00, 1.00, 1};
static GdkRGBA color_white =   {1.00, 1.00, 1.00, 1};
static GdkRGBA color_gray1 =   {0.40, 0.40, 0.40, 1};
static GdkRGBA color_gray2 =   {0.80, 0.80, 0.80, 1};
static GdkRGBA color_pink =    {1.00, 0.40, 0.40, 1};
static GdkRGBA color_pink2 =   {1.00, 0.80, 0.80, 1};
static GdkRGBA color_pale =    {0.40, 0.40, 1.00, 1};
static GdkRGBA color_pale2 =   {0.80, 0.80, 1.00, 1};
static GdkRGBA color_orange =  {1.00, 0.80, 0.40, 1};
static GdkRGBA color_orange2 = {1.00, 1.00, 0.80, 1};
static GdkRGBA color_orange3 = {0.95, 0.45, 0.02, 1};
static GdkRGBA color_green  =  {0.40, 0.80, 0.80, 1};
static GdkRGBA color_green2 =  {0.80, 1.00, 0.80, 1};
static GdkRGBA color_purple2 = {1.00, 0.80, 1.00, 1};
static GdkRGBA color_com1 =    {0.00, 0.53, 0.00, 1};
static GdkRGBA color_com2 =    {0.73, 0.53, 0.00, 1};
static GdkRGBA color_com3 =    {0.87, 0.00, 0.00, 1};
static GdkRGBA color_lblue =   {0.80, 0.80, 1.00, 1};
static GdkRGBA color_lgreen =  {0.80, 1.00, 0.80, 1};
static GdkRGBA color_lorange=  {1.00, 0.90, 0.70, 1};
static GdkRGBA color_lred   =  {1.00, 0.80, 0.80, 1};
#else
static GdkColor color_comment = {0, 0xDDDD, 0x0000, 0x0000};
static GdkColor color_focus = {0, 0x8888, 0x4444, 0x0000};
static GdkColor color_calib = {0, 0x0000, 0x8888, 0x0000};
static GdkColor color_black = {0, 0, 0, 0};
static GdkColor color_red   = {0, 0xFFFF, 0, 0};
static GdkColor color_blue = {0, 0, 0, 0xFFFF};
static GdkColor color_white = {0, 0xFFFF, 0xFFFF, 0xFFFF};
static GdkColor color_gray1 = {0, 0x6666, 0x6666, 0x6666};
static GdkColor color_gray2 = {0, 0xBBBB, 0xBBBB, 0xBBBB};
static GdkColor color_pink = {0, 0xFFFF, 0x6666, 0x6666};
static GdkColor color_pink2 = {0, 0xFFFF, 0xCCCC, 0xCCCC};
static GdkColor color_pale = {0, 0x6666, 0x6666, 0xFFFF};
static GdkColor color_pale2 = {0, 0xCCCC, 0xCCCC, 0xFFFF};
static GdkColor color_orange = {0, 0xFFFF, 0xCCCC, 0x6666};
static GdkColor color_orange2 = {0, 0xFFFF, 0xFFFF, 0xCCCC};
static GdkColor color_orange3 = {0, 0xFD00, 0x6A00, 0x0200};
static GdkColor color_green = {0, 0x6666, 0xCCCC, 0x6666};
static GdkColor color_green2 = {0, 0xCCCC, 0xFFFF, 0xCCCC};
static GdkColor color_purple2 = {0, 0xFFFF, 0xCCCC, 0xFFFF};
static GdkColor color_com1 = {0, 0x0000, 0x8888, 0x0000};
static GdkColor color_com2 = {0, 0xBBBB, 0x8888, 0x0000};
static GdkColor color_com3 = {0, 0xDDDD, 0x0000, 0x0000};
static GdkColor color_lblue = {0, 0xBBBB, 0xBBBB, 0xFFFF};
static GdkColor color_lgreen= {0, 0xBBBB, 0xFFFF, 0xBBBB};
static GdkColor color_lorange={0, 0xFFFF, 0xCCCC, 0xAAAA};
static GdkColor color_lred=   {0, 0xFFFF, 0xBBBB, 0xBBBB};
#endif




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
     CONF_DEF_COLOR_BALBD,
     NUM_CONF_COLOR
} GuiColorConf;

enum{ SET_RELEASE_BALLOON, SET_RELEASE_CLOCK } SetReleaseData;



// Setting for BIFF
#define DEF_MAIL_INTERVAL 60


enum{ MAIL_NO, MAIL_LOCAL, MAIL_POP3, MAIL_APOP, MAIL_QMAIL, MAIL_PROCMAIL } MailStatus0;

enum{ MAIL_PIX_LEFT, MAIL_PIX_RIGHT } MailPixPos;

#undef POP_DEBUG  /* pop3 debugging mode */

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
#ifdef USE_GTK3
  GdkPixbuf *pixbuf;
#ifdef USE_WIN32
  GdkPixbuf *pixbuf_sdw;
#endif
#else  
  GdkPixmap *pixmap;
  GdkBitmap *mask;
#ifdef USE_WIN32
  GdkPixmap *pixmap_sdw;
  GdkBitmap *mask_sdw;
#endif
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

// PangoCairo Font変換用
typedef struct _myPangoCairo myPangoCairo;
struct _myPangoCairo{
  gchar *family;
  gdouble pointsize;
  cairo_font_slant_t slant;
  cairo_font_weight_t weight;
};

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
#ifdef USE_GTK3
  GdkPixbuf *pixbuf;
#else
  GdkPixmap *pixmap;
  GdkBitmap *mask;
#endif  
  //GtkWidget *w_draw;
  //GtkWidget *e_draw;
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
  GtkWidget *win_main, *dw_main;
  GtkWidget *balloon_main, *dw_balloon;
  GtkWidget *clock_main, *dw_clock;
#ifdef FG_DRAW
  GtkWidget *balloon_fg, *dw_balfg;
  GtkWidget *clock_fg, *dw_clkfg;
  GtkWidget *win_sdw, *dw_sdw;
#endif
#ifdef USE_BIFF
  GtkWidget *biff_pix, *dw_biff;
#endif
  GtkWidget *PopupMenu;
#ifdef USE_GTK3
  GdkSeat *seat;
#endif
  
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
  PangoFontDescription  *fontclk;
  PangoFontDescription  *fontbal;
  gchar *fontname_bal;  
  gchar *fontname_clk;  
  gchar *deffontname_bal;  
  gchar *deffontname_clk;  
  myPangoCairo fontbal_pc;
  myPangoCairo fontclk_pc;
  typSprite sprites[MAX_PIXMAP];
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
#ifdef USE_GTK3   /////////////// GTK3 ///////////////
  GdkRGBA *def_colclk;
  GdkRGBA *def_colclksd;
  GdkRGBA *def_colclkbg;
  GdkRGBA *def_colclkbd;
  GdkRGBA *def_colbal;
  GdkRGBA *def_colbalbg;
  GdkRGBA *def_colbalbd;
  GdkRGBA *colclk;
  GdkRGBA *colclksd;
  GdkRGBA *colclkbg;
  GdkRGBA *colclkbd;
  GdkRGBA *colbal;
  GdkRGBA *colbalbg;
  GdkRGBA *colbalbd;
#else             /////////////// GTK2 ///////////////
  GdkGC *gc_main;
  GdkGC *gc_mainsd;
  GdkGC *gc_clk;
  GdkGC *gc_clksd;
  GdkGC *gc_clkbg;
  GdkGC *gc_clkbd;
  GdkGC *gc_clkmask;
  GdkGC *gc_bal;
  GdkGC *gc_balbg;
  GdkGC *gc_balbd;
  GdkGC *gc_balmask;
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
#endif
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
#ifdef USE_WIN32
  gint alpha_bal;
  gint def_alpha_bal;
  gint alpha_clk;
  gint def_alpha_clk;
#endif
  gboolean flag_img_cairo;
  gboolean flag_bal_cairo;
  gboolean flag_clk_cairo;
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
  int ptn_num;
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
  gint flag_xp;
  gint offsetp;
  gboolean flag_menu;
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
#ifdef USE_GTK_STATUS_ICON
  GtkStatusIcon *tray_icon;
  gboolean tray_icon_flag;
#endif
  gint flag_composite;
  gint force_composite;
  gint    sdw_flag;
  gdouble  sdw_x;
  gdouble  sdw_y;
  gint    sdw_alpha;
  gint sdw_x_int;
  gint sdw_y_int;
};


// GUI用構造体 1
typedef struct{
  typMascot *mascot;
  gint num;
}confNum;

// GUI用構造体 2
typedef struct{
  typMascot *mascot;
  gint num;
  gint num2;
}confNum2;



///////////   Global Arguments   //////////
#ifdef USE_GTK3
GdkPixbuf *pixbuf_main, *pixbuf_clk, *pixbuf_bal;
#ifdef USE_WIN32
GdkPixbuf *pixbuf_sdw;
#endif
#else  // USE_GTK3
GdkPixmap *pixmap_main, *pixmap_clk, *pixmap_bal;
#ifdef USE_WIN32
GdkPixmap *pixmap_sdw;
#endif
#endif // USE_GTK3
gint window_x, window_y;
gboolean supports_alpha;
gboolean flag_balloon;
typMascot *Mascot;


///////////   Proto types   //////////
// main.c
#ifdef USE_GTK3
void css_change_col();
void css_change_font();
void css_change_pbar_height();
#endif
gchar* FullPathMascotFile();
gchar* FullPathPixmapFile();
gchar *FullPathSoundFile();
void ReadMenu();
gchar* to_locale();
gchar* to_utf8();
gchar* all_random_menu_mascot_file();
void SaveMenu();
void SaveRC();
void SaveDefMenu();
void SaveMascot();
void SetFontForReleaseData();
void SetColorForReleaseData();
void InitMascot();
void ReadMascot();
gchar* ReadMascotName();
void ScanMenu();

// alpha.c
#ifdef USE_WIN32
void GdkWinSetAlpha();
#endif

// balloon.c
void DoBalloon();
void DoSysBalloon();
void make_balloon();
#ifdef USE_WIN32
void make_balloon_fg();
#endif
void balloon_arg_init();

// bmpwrite.c
int WriteBMP ();

// callbacks.c
void MoveMascot();
int MoveToFocus();
void MoveBalloon();
#ifdef USE_BIFF
void MoveBiffPix();
#endif
void ResizeMoveBalloon();
void clk_drag_begin();
void clk_drag_end();
void clk_window_motion();
gint dw_configure_bal();
gint dw_expose_bal();
gint expose_bal();
gint dw_configure_clk();
gint dw_expose_clk();
gint expose_clk();
#ifdef USE_BIFF
gint dw_configure_biff_pix();
gint dw_expose_biff_pix();
gint expose_biff_pix();
void biff_drag_begin();
void biff_drag_end();
void biff_window_motion();
#endif
gboolean time_update();
void callbacks_arg_init();
void clock_update();
void ext_play();
void sound_play();
#ifndef USE_GTK3
GdkGC *MPCreatePen();
#endif
gint dw_init_main();
void raise_all();
void make_mascot();
void map_balloon();
void map_clock();
#ifdef USE_BIFF
void map_biff();
#endif
void map_main();

// clock.c
#ifdef USE_WIN32
void make_clock_fg();
#endif
void make_clock();
void DrawPanelClock0();

// codeconv.c
void conv_unmime_header();
void conv_unmime_header_overwrite();
#if HAVE_ICONV
gchar *conv_iconv_strdup();
#endif

// dnd.c
void signal_drag_data_received();
void signal_drag_data_received_smenu();

// gui.c
void create_conf_num();
void NkrChangeMascot();
GtkWidget * make_popup_menu();
void create_config_dialog();
void create_cons_dialog();
void create_smenu_dialog();
void gui_arg_init();
void popup_message(GtkWidget *parent, gchar* stock_id,gint delay, ...);
void popup_progress();
void destroy_progress();
void unlink_all();
void AllRandomChangeMascotMenu();
GtkWidget* make_popup_menu();
void create_pop_pass_dialog();
gchar* create_nkr_change_image_dialog();
void MenuSaveAll();
void quit_all();


// mail.c
#ifdef USE_BIFF
gint SetMailChecker();
void make_biff_pix();
void mail_arg_init();
void create_biff_dialog();
void display_biff_balloon();
void remap_biff_pix();
#ifndef USE_WIN32
void kill_pop3();
#endif
gchar* set_mhdir();
#endif

// nokkari.c
void NkrSave();
void NkrRead();

// pixmap.c
void screen_changed();
void LoadPixmaps();
void LoadBiffPixmap();
void InitComposite();
gint DrawMascot0();
gint DrawMascot();
gint DrawMascotWithDigit();
void LoadPixmaps();
void InitComposite();
#ifdef USE_BIFF
void LoadBiffPixmap();
#endif
gboolean TestLoadPixmaps();
void ReInitGC();
#ifndef USE_GTK3
GdkBitmap * make_mask_from_surface();
#endif

// pop.c
int popReadLine(char *ptr, int size, guint ssl_mode);
int popWriteLine(char *ptr, guint ssl_mode);

// sockmsg.c
#ifdef USE_SOCKMSG
SockMsgInitResult sockmsg_init(void);
void sockmsg_send_msg(gchar *msg);
SockMsgInitResult duet_sv_init(gchar *mascotname);
void duet_sv_done(gchar *mascotname, gboolean flag_close);
void duet_set_mascot(typMascot *mascot);
SockMsgInitResult duet_cl_init(gchar *mascotname);
void duet_send_msg(gchar *msg);
void duet_cl_done(gchar *mascotname, gboolean flag_close);
void sockmsg_set_mascot(typMascot *mascot);
void sockmsg_done(void);
#endif

// ssl.c
#ifdef USE_SSL
void ssl_init(void);
#endif

// sslmanager.c
gint ssl_manager_verify_cert();

// trayicon.c
#ifdef __GTK_STATUS_ICON_H__
void trayicon_create();
void trayicon_show();
void trayicon_hide();
void trayicon_destroy();
void trayicon_set_tooltip();
#endif

// unlha.c
#ifdef USE_LHA32
gchar* unlha_menu();
#endif

// unmime.c
void unmime_header();

// untar.c
// TAR
#if defined(USE_GTAR) || defined(USE_TAR32)
gchar* untar_menu();
#endif


// utils.c
void my_signal_connect();
gboolean my_main_iteration();
void copy_file();
#ifdef USE_COMMON
void check_common_dir();
gboolean check_common_dir2();
#endif
#ifdef USE_WIN32
gchar* get_win_home();
#endif
gchar* get_rc_dir();

gchar* my_dirname();
gchar* my_basename();
gdouble GetCurrentResolution();
#ifndef USE_WIN32
gchar* GetCurrentWMName();
#endif
#ifdef USE_WIN32
gchar* WindowsVersion();
#endif
void pop_debug_print (const gchar *format, ...) G_GNUC_PRINTF(1, 2);
gchar *fgets_new();

