//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//
//
//     bmpwrite.c
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


/* bmpwrite.c	Writes Bitmap files. Even RLE encoded ones.	 */
/*		(Windows (TM) doesn't read all of those, but who */
/*		cares? ;-)					 */
/*              I changed a few things over the time, so perhaps */
/*              it dos now, but now there's no Windows left on   */
/*              my computer...                                   */

/* Alexander.Schulz@stud.uni-karlsruhe.de			 */

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
 
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "bmp.h"

int
WriteBMP (const gchar  	*filename,
	  GdkPixbuf	*pbuf)
{
  FILE *outfile;
  gint Red[MAXCOLORS];
  gint Green[MAXCOLORS];
  gint Blue[MAXCOLORS];
  gint rows, cols, Spcols, channels, MapSize, SpZeile;
  glong BitsPerPixel;
  gint colors;
  guchar puffer[50];
  gint	encoded;
  guchar	*pixels;
  
  /* first: can we save this image? */
  pixels = (guchar *) gdk_pixbuf_get_pixels (pbuf) ;
  colors       = 0;
  BitsPerPixel = 24;
  MapSize      = 0;
  channels     = 4;

  /* Perhaps someone wants RLE encoded Bitmaps */
  encoded = 0;

  /* Let's take some file */  
  outfile = fopen (filename, "wb");
  if (!outfile) {
    g_message ("Can't open %s", filename);
    return 0;
  }

  /* fetch the image */

  /* Now, we need some further information ... */
  cols = gdk_pixbuf_get_width (pbuf);
  rows = gdk_pixbuf_get_height(pbuf);


  /* ... that we write to our headers. */
  Spcols = cols;

  if ((((Spcols * BitsPerPixel) / 8) % 4) == 0) {
    SpZeile = ((Spcols * BitsPerPixel) / 8);
  } else {
    SpZeile = ((gint) (((Spcols * BitsPerPixel) / 8) / 4) + 1) * 4;
  }
  Bitmap_File_Head.bfSize    = 0x36 + MapSize + (rows * SpZeile);
  Bitmap_File_Head.zzHotX    = 0;
  Bitmap_File_Head.zzHotY    = 0;
  Bitmap_File_Head.bfOffs    = 0x36 + MapSize;
  Bitmap_File_Head.biSize    = 40;		

  Bitmap_Head.biWidth  = cols;
  Bitmap_Head.biHeight = rows;
  Bitmap_Head.biPlanes = 1;
  Bitmap_Head.biBitCnt = BitsPerPixel;

  Bitmap_Head.biCompr = 0;
  Bitmap_Head.biSizeIm = SpZeile * rows;

  Bitmap_Head.biClrUsed = 0;
  Bitmap_Head.biClrImp = Bitmap_Head.biClrUsed;
  
  /* And now write the header and the colormap (if any) to disk */

  Write (outfile, "BM", 2);

  FromL (Bitmap_File_Head.bfSize, &puffer[0x00]);
  FromS (Bitmap_File_Head.zzHotX, &puffer[0x04]);
  FromS (Bitmap_File_Head.zzHotY, &puffer[0x06]);
  FromL (Bitmap_File_Head.bfOffs, &puffer[0x08]);
  FromL (Bitmap_File_Head.biSize, &puffer[0x0C]);

  Write (outfile, puffer, 16);

  FromL (Bitmap_Head.biWidth, &puffer[0x00]);
  FromL (Bitmap_Head.biHeight, &puffer[0x04]);
  FromS (Bitmap_Head.biPlanes, &puffer[0x08]);
  FromS (Bitmap_Head.biBitCnt, &puffer[0x0A]);
  FromL (Bitmap_Head.biCompr, &puffer[0x0C]);
  FromL (Bitmap_Head.biSizeIm, &puffer[0x10]);
  FromL (Bitmap_Head.biXPels, &puffer[0x14]);
  FromL (Bitmap_Head.biYPels, &puffer[0x18]);
  FromL (Bitmap_Head.biClrUsed, &puffer[0x1C]);
  FromL (Bitmap_Head.biClrImp, &puffer[0x20]);

  Write (outfile, puffer, 36);
  WriteColorMap (outfile, Red, Green, Blue, MapSize);

  /* After that is done, we write the image ... */
  
  WriteImage (outfile,
	      pixels, cols, rows,
	      encoded, channels, BitsPerPixel, SpZeile, MapSize);

  /* ... and exit normally */
  fclose (outfile);

  return 1;
}

void 
WriteColorMap (FILE *f, 
	       gint  red[MAXCOLORS], 
	       gint  green[MAXCOLORS],
	       gint  blue[MAXCOLORS], 
	       gint  size)
{
  gchar trgb[4];
  gint i;

  size /= 4;
  trgb[3] = 0;
  for (i = 0; i < size; i++)
    {
      trgb[0] = (guchar) blue[i];
      trgb[1] = (guchar) green[i];
      trgb[2] = (guchar) red[i];
      Write (f, trgb, 4);
    }
}

void 
WriteImage (FILE   *f, 
	    guchar *src, 
	    gint    width, 
	    gint    height,
	    gint    encoded, 
	    gint    channels, 
	    gint    bpp, 
	    gint    spzeile, 
	    gint    MapSize)
{
  guchar buf[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0};
  guchar puffer[8];
  guchar *temp, v;
  guchar *Zeile, *ketten;
  gint xpos, ypos, i, j, rowstride, laenge, thiswidth;
  gint breite, n, k;

  xpos = 0;
  rowstride = width * channels;

  /* We'll begin with the 24 bit Bitmaps, they are easy :-) */

  if (bpp == 24)
    {
      for (ypos = height - 1; ypos >= 0; ypos--)  /* for each row   */
	{
	  for (i = 0; i < width; i++)  /* for each pixel */
	    {
	      temp = src + (ypos * rowstride) + (xpos * channels);
	      buf[2] = (guchar) *temp;
	      temp++;
	      buf[1] = (guchar) *temp;
	      temp++;
	      buf[0] = (guchar) *temp;
	      xpos++;
	      Write (f, buf, 3);
	    }
	  Write (f, &buf[3], spzeile - (width * 3));
#if 0
	  cur_progress++;
	  if ((interactive_bmp) &&
	      ((cur_progress % 5) == 0))
	    gimp_progress_update ((gdouble) cur_progress /
				  (gdouble) max_progress);
#endif
	  xpos = 0;
	}
    }
  else
    {
      switch (encoded)  /* now it gets more difficult */
	{               /* uncompressed 1,4 and 8 bit */
	case 0:
	  {
	    thiswidth = (width / (8 / bpp));
	    if (width % (8 / bpp))
	      thiswidth++;

	    for (ypos = height - 1; ypos >= 0; ypos--) /* for each row */
	      {
		for (xpos = 0; xpos < width;)  /* for each _byte_ */
		  {
		    v = 0;
		    for (i = 1;
			 (i <= (8 / bpp)) && (xpos < width);
			 i++, xpos++)  /* for each pixel */
		      {
			temp = src + (ypos * rowstride) + (xpos * channels);
			v=v | ((guchar) *temp << (8 - (i * bpp)));
		      }
		    Write (f, &v, 1);
		  }
		Write (f, &buf[3], spzeile - thiswidth);
		xpos = 0;
#if 0
		cur_progress++;
		if ((interactive_bmp) &&
		    ((cur_progress % 5) == 0))
		  gimp_progress_update ((gdouble) cur_progress /
					(gdouble) max_progress);
#endif
	      }
	    break;
	  }
	default:
	  {		 /* Save RLE encoded file, quite difficult */
	    laenge = 0;
	    buf[12] = 0;
	    buf[13] = 1;
	    buf[14] = 0;
	    buf[15] = 0;
	    Zeile = (guchar *) g_malloc (width / (8 / bpp) + 10);
	    ketten = (guchar *) g_malloc (width / (8 / bpp) + 10);
	    for (ypos = height - 1; ypos >= 0; ypos--)
	      {	/* each row separately */
		/*printf("Line: %i\n",ypos); */
		j = 0;
		/* first copy the pixels to a buffer,
		 * making one byte from two 4bit pixels
		 */
		for (xpos = 0; xpos < width;)
		  {
		    v = 0;
		    for (i = 1;
			 (i <= (8 / bpp)) && (xpos < width);
			 i++, xpos++)
		      {	/* for each pixel */
			temp = src + (ypos * rowstride) + (xpos * channels);
			v = v | ((guchar) * temp << (8 - (i * bpp)));
		      }
		    Zeile[j++] = v;
		  }
		breite = width / (8 / bpp);
		if (width % (8 / bpp))
		  breite++;
		/* then check for strings of equal bytes */
		for (i = 0; i < breite;)
		  {
		    j = 0;
		    while ((i + j < breite) &&
			   (j < (255 / (8 / bpp))) &&
			   (Zeile[i + j] == Zeile[i]))
		      j++;

		    ketten[i] = j;
		    /*printf("%i:",ketten[i]); */
		    i += j;
		  }
		/*printf("\n"); */

		/* then write the strings and the other pixels to the file */
		for (i = 0; i < breite;)
		  {
		    if (ketten[i] < 3)
		      /* strings of different pixels ... */
		      {
			j = 0;
			while ((i + j < breite) &&
			       (j < (255 / (8 / bpp))) &&
			       (ketten[i + j] < 3))
			  j += ketten[i + j];

			/* this can only happen if j jumps over
			 * the end with a 2 in ketten[i+j]
			 */
			if (j > (255 / (8 / bpp)))
			  j -= 2;
			/* 00 01 and 00 02 are reserved */
			if (j > 2)
			  {
			    Write (f, &buf[12], 1);
			    n = j * (8 / bpp);
			    if (n + i * (8 / bpp) > width)
			      n--;
			    Write (f, &n, 1);
			    laenge += 2;
			    Write (f, &Zeile[i], j);
			    /*printf("0.%i.",n); */
			    /*for (k=j;k;k--) printf("#"); */
			    laenge += j;
			    if ((j) % 2)
			      {
				Write (f, &buf[12], 1);
				laenge++;
				/*printf("0"); */
			      }
			    /*printf("|"); */
			  }
			else
			  {
			    for (k = i; k < i + j; k++)
			      {
				n = (8 / bpp);
				if (n + i * (8 / bpp) > width)
				  n--;
				Write (f, &n, 1);
				Write (f, &Zeile[k], 1);
				/*printf("%i.#|",n); */
				laenge += 2;
			      }
			  }
			i += j;
		      }
		    else
		      /* strings of equal pixels */
		      {
			n = ketten[i] * (8 / bpp);
			if (n + i * (8 / bpp) > width)
			  n--;
			Write (f, &n, 1);
			Write (f, &Zeile[i], 1);
			/*printf("%i.#|",n); */
			i += ketten[i];
			laenge += 2;
		      }
		  }
		/*printf("\n"); */
		Write (f, &buf[14], 2);		 /* End of row */
		laenge += 2;
#if 0
		cur_progress++;
		if ((interactive_bmp) &&
		    ((cur_progress % 5) == 0))
		  gimp_progress_update ((gdouble) cur_progress /
					(gdouble) max_progress);
#endif
	      }
	    fseek (f, -2, SEEK_CUR);	 /* Overwrite last End of row ... */
	    Write (f, &buf[12], 2);	 /* ... with End of file */

	    fseek (f, 0x22, SEEK_SET);            /* Write length of image */
	    FromL (laenge, puffer);
	    Write (f, puffer, 4);
	    fseek (f, 0x02, SEEK_SET);            /* Write length of file */
	    laenge += (0x36 + MapSize);
	    FromL (laenge, puffer);
	    Write (f, puffer, 4);
	    g_free (ketten);
	    g_free (Zeile);
	    break;
	  }
	}
    }
#if 0  
  if (interactive_bmp)
    gimp_progress_update (1);
#endif
}

/* ********************************************************** bmpwrite.c *** */
