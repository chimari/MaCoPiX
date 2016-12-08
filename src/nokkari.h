//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//    A unified desktop mascot program
//    for UNIX / X Window System with Gdk Environment
//
//  nokkari.h  : Nokkari-Character Converter Header for  MaCoPiX
//                   (for Nokkari-chara ver1.40 or later)
//                                copyright K.Chimari 2002-4
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

// メニューの最大マスコット許容数
#define MASCOTNUM 64

// 一つのマスコットに対する最大アニメーションフレーム許容数
#define ANIMFRAME 29

// 最大クリックアニメーションパターン許容数
#define CLICKPATTERNMAX 10

// 最大ランダムアニメーションパターン許容数
#define RANIMPATTERNMAX 10

// デフォルトの xoff yoff (xpm直接指定モードのみ有効)
#define DEF_XOFF 100
#define DEF_YOFF 100

// のっかりキャラ用設定
// 画像ファイル数
#define NKR_MAX_FILE 30
// パターン数
#define NKR_MAX_PAT 10
// 1パターン内フレーム数
#define NKR_MAX_PAT_Y 99

// 1パターン内フレーム数 変換行列用Buffer
#define BUF_NKR_MAX_PAT_Y 500


// コード
#define NKR_CODE "SJIS"
#define MACOPIX_CODE "EUC-JP"


