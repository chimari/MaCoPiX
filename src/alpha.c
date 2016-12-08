//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//
//
//     alpha.c
//     Using Alpha Blending Windows only in MS Windows
//
//                            Copyright 2002-2007  K.Chimari
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

#ifdef USE_WIN32

#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <gdk/gdkwin32.h>

void GdkWinChangeAlpha();
void GdkWinChangeAlphaFG();
void GdkWinSetAlpha();



void GdkWinChangeAlpha(GdkWindow *win, gint alpha){
  HWND hWnd;
  DWORD dwExStyle;

  hWnd = GDK_WINDOW_HWND(win);
  dwExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
  if(alpha==100){
    if(dwExStyle&WS_EX_LAYERED){
      SetWindowLong(hWnd, GWL_EXSTYLE, dwExStyle^WS_EX_LAYERED);
    }
  }
  else{
    if(!(dwExStyle&WS_EX_LAYERED)){
      SetWindowLong(hWnd, GWL_EXSTYLE, dwExStyle|WS_EX_LAYERED);
    }
    SetLayeredWindowAttributes(hWnd, 0, (255*alpha)/100, LWA_ALPHA);
  }
}

void GdkWinChangeAlphaFG(GdkWindow *win, GdkColor *col){
  HWND hWnd;
  DWORD dwExStyle;

  hWnd = GDK_WINDOW_HWND(win);
  dwExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
  if(!(dwExStyle&WS_EX_LAYERED)){
    SetWindowLong(hWnd, GWL_EXSTYLE, dwExStyle|WS_EX_LAYERED);
  }

  SetLayeredWindowAttributes(hWnd, 
	     RGB(col->red/0x100,col->green/0x100,col->blue/0x100),
	     0, LWA_COLORKEY);

}

void GdkWinSetAlpha(GdkWindow *win){
  HWND hWnd;
  DWORD dwExStyle;

  hWnd = GDK_WINDOW_HWND(win);
  dwExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
  if(!(dwExStyle&WS_EX_LAYERED)){
    SetWindowLong(hWnd, GWL_EXSTYLE, dwExStyle|WS_EX_LAYERED);
  }
}
#endif  // USE_WIN32
