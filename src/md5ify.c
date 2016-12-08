//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//
//
//     md5ify.c
//     MD5 message-digest algorithm
//      originated from fetchmail  *** see below copyright ***
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

/*
 * For license terms, see the file COPYING in this directory.
 */

/***********************************************************************
  module:       md5ify.c
  project:      fetchmail
  programmer:   Carl Harris, ceharris@mal.com
  description:  Simple interface to MD5 module.

 ***********************************************************************/
#include "main.h"

#ifdef USE_BIFF

#include <stdio.h>
#include <string.h>

#if defined(STDC_HEADERS)
#include <string.h>
#endif

#include "md5.h"

char *
MD5Digest (unsigned char *s)
{
  int i;
  /*MD5_CTX context;*/
  GMD5* context;
  /*unsigned char digest[16];*/
  gchar* digest;
  static char ascii_digest [33];

  /*MD5Init(&context);*/
  context = gnet_md5_new_incremental();
  /*MD5Update(&context, s, strlen(s));*/
  gnet_md5_update (context,s,strlen(s));
  /*MD5Final(digest, &context);*/
  gnet_md5_final(context);
  digest = gnet_md5_get_string(context);
  
  /*for (i = 0;  i < 16;  i++) 
    sprintf(ascii_digest+2*i, "%02x", digest[i]);*/
  sprintf(ascii_digest,"%s",digest);

  g_free(digest);
  gnet_md5_delete(context);  
 
  return(ascii_digest);
}

#endif  // USE_BIFF

