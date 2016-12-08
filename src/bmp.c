//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//
//
//     bmp.c
//     Read and write bmp image files
//      originated from gimp-1.3  *** see below copyright ***
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


/* bmp.c                                          */
/* Version 0.51	                                  */
/* This is a File input and output filter for the */
/* Gimp. It loads and saves images in windows(TM) */
/* bitmap format.                                 */
/* Some Parts that deal with the interaction with */
/* the Gimp are taken from the GIF plugin by      */
/* Peter Mattis & Spencer Kimball and from the    */
/* PCX plugin by Francisco Bustamante.            */
/*                                                */
/* Alexander.Schulz@stud.uni-karlsruhe.de         */

/* Changes:   28.11.1997 Noninteractive operation */
/*            16.03.1998 Endian-independent!!     */
/*	      21.03.1998 Little Bug-fix		  */
/*            06.04.1998 Bugfix in Padding        */
/*            11.04.1998 Arch. cleanup (-Wall)    */
/*                       Parses gtkrc             */
/*            14.04.1998 Another Bug in Padding   */
/*            28.04.1998 RLE-Encoding rewritten   */
/*            29.10.1998 Changes by Tor Lillqvist */
/*                       <tml@iki.fi> to support  */
/*                       16 and 32 bit images     */
/*            28.11.1998 Bug in RLE-read-padding  */
/*                       fixed.                   */
/*            19.12.1999 Resolution support added */
/*            06.05.2000 Overhaul for 16&24-bit   */
/*                       plus better OS/2 code    */
/*                       by njl195@zepler.org.uk  */

/* 
 * The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 * ----------------------------------------------------------------------------
 */
#include "main.h"
#include <stdio.h>

#include "bmp.h"

struct Bitmap_File_Head_Struct Bitmap_File_Head;
struct Bitmap_Head_Struct Bitmap_Head;

gint32 
ToL (guchar *puffer)
{
  return (puffer[0] | puffer[1]<<8 | puffer[2]<<16 | puffer[3]<<24);
}

gint16 
ToS (guchar *puffer)
{
  return (puffer[0] | puffer[1]<<8);
}

void 
FromL (gint32  wert, 
       guchar *bopuffer)
{
  bopuffer[0] = (wert & 0x000000ff)>>0x00;
  bopuffer[1] = (wert & 0x0000ff00)>>0x08;
  bopuffer[2] = (wert & 0x00ff0000)>>0x10;
  bopuffer[3] = (wert & 0xff000000)>>0x18;
}

void  
FromS (gint16  wert, 
       guchar *bopuffer)
{
  bopuffer[0] = (wert & 0x00ff)>>0x00;
  bopuffer[1] = (wert & 0xff00)>>0x08;
}

/* *************************************************************** bmp.c *** */
