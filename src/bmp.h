//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//
//
//      bmp.h  
//      Configu header for bmp write/read
//           originated from gimp-1.3
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

#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#define	MAXCOLORS	256

#define BitSet(byte, bit)  (((byte) & (bit)) == (bit))

#define Write(file,buffer,len)   fwrite(buffer, len, 1, file)
#define WriteOK(file,buffer,len) (Write(buffer, len, file) != 0)

extern gint32	ToL           	(guchar *);
extern void	FromL         	(gint32,
				 guchar *);
extern gint16	ToS           	(guchar *);
extern void	FromS         	(gint16,
				 guchar *);
extern int	WriteBMP 	(const gchar  	*filename,
				 GdkPixbuf	*pbuf);
extern void	WriteColorMap 	(FILE *,
				 gint *,
				 gint *,
				 gint *,
				 gint);
extern void	WriteImage    	(FILE		*filename,
				 guchar 	*data,
				 gint		width,
				 gint		height,
				 gint		encoded,
				 gint		channels,
				 gint		bpp,
				 gint		spzeile,
				 gint		MapSize);

extern struct Bitmap_File_Head_Struct
{
  gchar    zzMagic[2];  /* 00 "BM" */
  gulong   bfSize;      /* 02 */
  gushort  zzHotX;      /* 06 */
  gushort  zzHotY;      /* 08 */
  gulong   bfOffs;      /* 0A */
  gulong   biSize;      /* 0E */
} Bitmap_File_Head;

extern struct Bitmap_Head_Struct
{
  gulong   biWidth;     /* 12 */
  gulong   biHeight;    /* 16 */
  gushort  biPlanes;    /* 1A */
  gushort  biBitCnt;    /* 1C */
  gulong   biCompr;     /* 1E */
  gulong   biSizeIm;    /* 22 */
  gulong   biXPels;     /* 26 */
  gulong   biYPels;     /* 2A */
  gulong   biClrUsed;   /* 2E */
  gulong   biClrImp;    /* 32 */
                        /* 36 */
} Bitmap_Head;

/* *************************************************************** bmp.h *** */
