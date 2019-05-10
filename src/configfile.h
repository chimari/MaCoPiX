//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//    A unified desktop mascot program
//    for UNIX / X Window System with Gdk Environment
//
//  configfile.h  : Save Data Access for MaCoPiX
//                                copyright K.Chimari 2002
//       (diverting from original program of XMMS)
//
//       XMMS - Cross-platform multimedia player
//       Copyright (C) 1998-1999  Peter Alm, Mikael Alm, Olle Hallnas, 
//                                Thomas Nilsson and 4Front Technologies
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

#ifndef XMMS_CONFIGFILE_H
#define XMMS_CONFIGFILE_H

#include <glib.h>

typedef struct
{
	gchar *key;
	gchar *value;
}
ConfigLine;

typedef struct
{
	gchar *name;
	GList *lines;
}
ConfigSection;

typedef struct
{
	GList *sections;
}
ConfigFile;

ConfigFile *xmms_cfg_new(void);
ConfigFile *xmms_cfg_open_file(gchar * filename);
gboolean xmms_cfg_write_file(ConfigFile * cfg, gchar * filename);
void xmms_cfg_free(ConfigFile * cfg);

gboolean xmms_cfg_read_string(ConfigFile * cfg, gchar * section, gchar * key, gchar ** value);
gboolean xmms_cfg_read_int(ConfigFile * cfg, gchar * section, gchar * key, gint * value);
gboolean xmms_cfg_read_boolean(ConfigFile * cfg, gchar * section, gchar * key, gboolean * value);
gboolean xmms_cfg_read_float(ConfigFile * cfg, gchar * section, gchar * key, gfloat * value);
gboolean xmms_cfg_read_double(ConfigFile * cfg, gchar * section, gchar * key, gdouble * value);
void xmms_cfg_write_double2(ConfigFile * cfg, gchar * section, gchar * key, gdouble value, gchar * format);
void xmms_cfg_write_string(ConfigFile * cfg, gchar * section, gchar * key, gchar * value);
void xmms_cfg_write_int(ConfigFile * cfg, gchar * section, gchar * key, gint value);
void xmms_cfg_write_boolean(ConfigFile * cfg, gchar * section, gchar * key, gboolean value);
void xmms_cfg_write_float(ConfigFile * cfg, gchar * section, gchar * key, gfloat value);
void xmms_cfg_write_double(ConfigFile * cfg, gchar * section, gchar * key, gdouble value);

void xmms_cfg_remove_key(ConfigFile * cfg, gchar * section, gchar * key);

#endif
