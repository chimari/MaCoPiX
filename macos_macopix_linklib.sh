#!/bin/sh

TGTPATH="@executable_path/../Resources/lib/"
OPTPATH="/usr/local/opt/"
CPPATH="/Volumes/MaCoPiX_macOS/MaCoPiX.app/Contents/Resources/lib/"
MACOPIX_BIN="/Volumes/MaCoPiX_macOS/MaCoPiX.app/Contents/MacOS/MaCoPiX-bin"
LOCALEPATH="/Volumes/MaCoPiX_macOS/MaCoPiX.app/Contents/Resources/share/locale/ja/LC_MESSAGES/"

cp src/macopix ${MACOPIX_BIN}
cp po/ja.gmo ${LOCALEPATH}macopix.mo

install_name_tool -change	${OPTPATH}gtk-mac-integration/lib/libgtkmacintegration-gtk3.2.dylib ${TGTPATH}libgtkmacintegration-gtk3.2.dylib ${MACOPIX_BIN}
install_name_tool -change	${OPTPATH}gtk+3/lib/libgtk-3.0.dylib ${TGTPATH}libgtk-3.0.dylib ${MACOPIX_BIN} 
install_name_tool -change	${OPTPATH}gtk+3/lib/libgdk-3.0.dylib ${TGTPATH}libgdk-3.0.dylib ${MACOPIX_BIN}
install_name_tool -change	${OPTPATH}pango/lib/libpangocairo-1.0.0.dylib ${TGTPATH}libpangocairo-1.0.0.dylib ${MACOPIX_BIN}
install_name_tool -change	${OPTPATH}pango/lib/libpango-1.0.0.dylib ${TGTPATH}libpango-1.0.0.dylib ${MACOPIX_BIN}
install_name_tool -change	${OPTPATH}atk/lib/libatk-1.0.0.dylib ${TGTPATH}libatk-1.0.0.dylib ${MACOPIX_BIN}
install_name_tool -change	${OPTPATH}cairo/lib/libcairo-gobject.2.dylib ${TGTPATH}libcairo-gobject.2.dylib ${MACOPIX_BIN}
install_name_tool -change	${OPTPATH}cairo/lib/libcairo.2.dylib ${TGTPATH}libcairo.2.dylib ${MACOPIX_BIN}
install_name_tool -change	${OPTPATH}gdk-pixbuf/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${MACOPIX_BIN}
install_name_tool -change	${OPTPATH}glib/lib/libgio-2.0.0.dylib ${TGTPATH}libgio-2.0.0.dylib ${MACOPIX_BIN}
install_name_tool -change	${OPTPATH}glib/lib/libgobject-2.0.0.dylib ${TGTPATH}libgobject-2.0.0.dylib ${MACOPIX_BIN}
install_name_tool -change	${OPTPATH}glib/lib/libgmodule-2.0.0.dylib ${TGTPATH}libgmodule-2.0.0.dylib ${MACOPIX_BIN}
install_name_tool -change	${OPTPATH}glib/lib/libglib-2.0.0.dylib ${TGTPATH}libglib-2.0.0.dylib ${MACOPIX_BIN}
install_name_tool -change	${OPTPATH}gettext/lib/libintl.8.dylib ${TGTPATH}libintl.8.dylib	 ${MACOPIX_BIN}
install_name_tool -change       ${OPTPATH}openssl/lib/libssl.1.0.0.dylib ${TGTPATH}libssl.1.0.0.dylib ${MACOPIX_BIN}
install_name_tool -change	${OPTPATH}openssl/lib/libcrypto.1.0.0.dylib ${TGTPATH}libcrypto.1.0.0.dylib ${MACOPIX_BIN}


cp	${OPTPATH}gtk-mac-integration/lib/libgtkmacintegration-gtk3.2.dylib ${CPPATH}
cp	${OPTPATH}gtk+3/lib/libgtk-3.0.dylib ${CPPATH}
cp	${OPTPATH}gtk+3/lib/libgdk-3.0.dylib ${CPPATH}
cp	${OPTPATH}pango/lib/libpangocairo-1.0.0.dylib ${CPPATH}
cp	${OPTPATH}pango/lib/libpango-1.0.0.dylib ${CPPATH}
cp	${OPTPATH}atk/lib/libatk-1.0.0.dylib ${CPPATH}
cp	${OPTPATH}cairo/lib/libcairo-gobject.2.dylib ${CPPATH}
cp	${OPTPATH}cairo/lib/libcairo.2.dylib ${CPPATH}
cp	${OPTPATH}gdk-pixbuf/lib/libgdk_pixbuf-2.0.0.dylib ${CPPATH}
cp	${OPTPATH}glib/lib/libgio-2.0.0.dylib ${CPPATH}
cp	${OPTPATH}glib/lib/libgobject-2.0.0.dylib ${CPPATH}
cp	${OPTPATH}glib/lib/libgmodule-2.0.0.dylib ${CPPATH}
cp	${OPTPATH}glib/lib/libglib-2.0.0.dylib ${CPPATH}
cp	${OPTPATH}gettext/lib/libintl.8.dylib ${CPPATH}
cp	${OPTPATH}openssl/lib/libssl.1.0.0.dylib ${CPPATH}
cp	${OPTPATH}openssl/lib/libcrypto.1.0.0.dylib ${CPPATH}

cp      ${OPTPATH}libepoxy/lib/libepoxy.0.dylib ${CPPATH}
cp	${OPTPATH}fribidi/lib/libfribidi.0.dylib ${CPPATH}
cp	${OPTPATH}harfbuzz/lib/libharfbuzz.0.dylib ${CPPATH}
cp	${OPTPATH}pango/lib/libpangoft2-1.0.0.dylib ${CPPATH}
cp	${OPTPATH}fontconfig/lib/libfontconfig.1.dylib ${CPPATH}
cp	${OPTPATH}freetype/lib/libfreetype.6.dylib ${CPPATH}
cp	${OPTPATH}glib/lib/libgthread-2.0.0.dylib ${CPPATH}
cp	${OPTPATH}pixman/lib/libpixman-1.0.dylib ${CPPATH}
cp	${OPTPATH}libpng/lib/libpng16.16.dylib ${CPPATH}
cp 	${OPTPATH}libffi/lib/libffi.6.dylib ${CPPATH}
cp	${OPTPATH}pcre/lib/libpcre.1.dylib ${CPPATH}
cp     	${OPTPATH}graphite2/lib/libgraphite2.3.dylib ${CPPATH}

chmod 755 ${CPPATH}*dylib

LIBFILE="libgtkmacintegration-gtk3.2.dylib"
	install_name_tool -change ${OPTPATH}gtk-mac-integration/lib/libgtkmacintegration-gtk3.2.dylib ${TGTPATH}libgtkmacintegration-gtk3.2.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gtk+3/lib/libgtk-3.0.dylib               ${TGTPATH}libgtk-3.0.dylib 		   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gtk+3/lib/libgdk-3.0.dylib 		   ${TGTPATH}libgdk-3.0.dylib 		   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}pango/lib/libpangocairo-1.0.0.dylib 	   ${TGTPATH}libpangocairo-1.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}pango/lib/libpango-1.0.0.dylib 	   ${TGTPATH}libpango-1.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}atk/lib/libatk-1.0.0.dylib 		   ${TGTPATH}libatk-1.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo-gobject.2.dylib 	   ${TGTPATH}libcairo-gobject.2.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo.2.dylib 		   ${TGTPATH}libcairo.2.dylib 		   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gdk-pixbuf/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib     ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgio-2.0.0.dylib 		   ${TGTPATH}libgio-2.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib 	   ${TGTPATH}libgobject-2.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib 		   ${TGTPATH}libglib-2.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib              ${TGTPATH}libintl.8.dylib               ${CPPATH}${LIBFILE}
LIBFILE="libgtk-3.0.dylib"	
##	/usr/local/Cellar/gtk+3/3.24.8/lib/libgdk-3.0.dylib
	install_name_tool -change ${OPTPATH}gtk+3/lib/libgtk-3.0.dylib		   ${TGTPATH}libgtk-3.0.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/gtk+3/3.24.8/lib/libgdk-3.0.dylib         ${TGTPATH}libgdk-3.0.dylib             ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgmodule-2.0.0.dylib 	   ${TGTPATH}libgmodule-2.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}pango/lib/libpangocairo-1.0.0.dylib 	   ${TGTPATH}libpangocairo-1.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo-gobject.2.dylib 	   ${TGTPATH}libcairo-gobject.2.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo.2.dylib 		   ${TGTPATH}libcairo.2.dylib 		   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gdk-pixbuf/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}atk/lib/libatk-1.0.0.dylib 		   ${TGTPATH}libatk-1.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}libepoxy/lib/libepoxy.0.dylib 	   ${TGTPATH}libepoxy.0.dylib 		   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}fribidi/lib/libfribidi.0.dylib 	   ${TGTPATH}libfribidi.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgio-2.0.0.dylib 		   ${TGTPATH}libgio-2.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}harfbuzz/lib/libharfbuzz.0.dylib 	   ${TGTPATH}libharfbuzz.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}pango/lib/libpangoft2-1.0.0.dylib 	   ${TGTPATH}libpangoft2-1.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}pango/lib/libpango-1.0.0.dylib 	   ${TGTPATH}libpango-1.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib 	   ${TGTPATH}libgobject-2.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib 		   ${TGTPATH}libglib-2.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}fontconfig/lib/libfontconfig.1.dylib 	   ${TGTPATH}libfontconfig.1.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}freetype/lib/libfreetype.6.dylib 	   ${TGTPATH}libfreetype.6.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib              ${TGTPATH}libintl.8.dylib               ${CPPATH}${LIBFILE}
LIBFILE="libgdk-3.0.dylib"
	install_name_tool -change ${OPTPATH}gtk+3/lib/libgdk-3.0.dylib               ${TGTPATH}libgdk-3.0.dylib	           ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}pango/lib/libpangocairo-1.0.0.dylib      ${TGTPATH}libpangocairo-1.0.0.dylib     ${CPPATH}${LIBFILE}             
	install_name_tool -change ${OPTPATH}pango/lib/libpango-1.0.0.dylib 	   ${TGTPATH}libpango-1.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gdk-pixbuf/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo-gobject.2.dylib 	   ${TGTPATH}libcairo-gobject.2.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgio-2.0.0.dylib 		   ${TGTPATH}libgio-2.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib 	   ${TGTPATH}libgobject-2.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib 		   ${TGTPATH}libglib-2.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo.2.dylib 		   ${TGTPATH}libcairo.2.dylib 		   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}libepoxy/lib/libepoxy.0.dylib 	   ${TGTPATH}libepoxy.0.dylib 	  	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}fribidi/lib/libfribidi.0.dylib 	   ${TGTPATH}libfribidi.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib            ${TGTPATH}libintl.8.dylib               ${CPPATH}${LIBFILE}
LIBFILE="libpangocairo-1.0.0.dylib"
##	/usr/local/Cellar/pango/1.42.4_1/lib/libpango-1.0.0.dylib
##	/usr/local/Cellar/pango/1.42.4_1/lib/libpangoft2-1.0.0.dylib
	install_name_tool -change ${OPTPATH}pango/lib/libpangocairo-1.0.0.dylib  ${TGTPATH}libpangocairo-1.0.0.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/pango/1.42.4_1/lib/libpango-1.0.0.dylib       ${TGTPATH}libpango-1.0.0.dylib         ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo.2.dylib 	       ${TGTPATH}libcairo.2.dylib 	       ${CPPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/pango/1.42.4_1/lib/libpangoft2-1.0.0.dylib    ${TGTPATH}libpangoft2-1.0.0.dylib    ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}fribidi/lib/libfribidi.0.dylib       ${TGTPATH}libfribidi.0.dylib       ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib      ${TGTPATH}libgobject-2.0.0.dylib      ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgthread-2.0.0.dylib      ${TGTPATH}libgthread-2.0.0.dylib      ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib 	       ${TGTPATH}libglib-2.0.0.dylib 	       ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib 	       ${TGTPATH}libintl.8.dylib 	       ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}harfbuzz/lib/libharfbuzz.0.dylib     ${TGTPATH}libharfbuzz.0.dylib     ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}fontconfig/lib/libfontconfig.1.dylib ${TGTPATH}libfontconfig.1.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}freetype/lib/libfreetype.6.dylib     ${TGTPATH}libfreetype.6.dylib                    ${CPPATH}${LIBFILE}
LIBFILE="libpango-1.0.0.dylib"
	install_name_tool -change ${OPTPATH}pango/lib/libpango-1.0.0.dylib 	   ${TGTPATH}libpango-1.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib ${TGTPATH}libgobject-2.0.0.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgthread-2.0.0.dylib ${TGTPATH}libgthread-2.0.0.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib 	  ${TGTPATH}libglib-2.0.0.dylib 	  ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib 	  ${TGTPATH}libintl.8.dylib 	  ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}fribidi/lib/libfribidi.0.dylib  ${TGTPATH}libfribidi.0.dylib  ${CPPATH}${LIBFILE}
LIBFILE="libatk-1.0.0.dylib"
	install_name_tool -change ${OPTPATH}atk/lib/libatk-1.0.0.dylib 		   ${TGTPATH}libatk-1.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib    ${TGTPATH}libglib-2.0.0.dylib    ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib 	  ${TGTPATH}libintl.8.dylib 	  ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib ${TGTPATH}libgobject-2.0.0.dylib  ${CPPATH}${LIBFILE}
LIBFILE="libcairo-gobject.2.dylib"
## /usr/local/Cellar/cairo/1.16.0/lib/libcairo.2.dylib	
	install_name_tool -change /usr/local/Cellar/cairo/1.16.0/lib/libcairo.2.dylib ${TGTPATH}libcairo.2.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib ${TGTPATH}libgobject-2.0.0.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}pixman/lib/libpixman-1.0.dylib       ${TGTPATH}libpixman-1.0.dylib       ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}fontconfig/lib/libfontconfig.1.dylib ${TGTPATH}libfontconfig.1.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}freetype/lib/libfreetype.6.dylib     ${TGTPATH}libfreetype.6.dylib     ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}libpng/lib/libpng16.16.dylib 	       ${TGTPATH}libpng16.16.dylib 	       ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib      ${TGTPATH}libgobject-2.0.0.dylib      ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib 	       ${TGTPATH}libglib-2.0.0.dylib 	       ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib          ${TGTPATH}libintl.8.dylib          ${CPPATH}${LIBFILE}
LIBFILE="libcairo.2.dylib"
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo.2.dylib 	       ${TGTPATH}libcairo.2.dylib 	       ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}pixman/lib/libpixman-1.0.dylib       ${TGTPATH}libpixman-1.0.dylib        ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}fontconfig/lib/libfontconfig.1.dylib ${TGTPATH}libfontconfig.1.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}freetype/lib/libfreetype.6.dylib     ${TGTPATH}libfreetype.6.dylib     ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}libpng/lib/libpng16.16.dylib         ${TGTPATH}libpng16.16.dylib         ${CPPATH}${LIBFILE}
LIBFILE="libgdk_pixbuf-2.0.0.dylib"
	install_name_tool -change ${OPTPATH}gdk-pixbuf/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib ${TGTPATH}libgobject-2.0.0.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib 	  ${TGTPATH}libglib-2.0.0.dylib 	  ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib 	  ${TGTPATH}libintl.8.dylib 	  ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgmodule-2.0.0.dylib ${TGTPATH}libgmodule-2.0.0.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgio-2.0.0.dylib     ${TGTPATH}libgio-2.0.0.dylib     ${CPPATH}${LIBFILE}
LIBFILE="libgio-2.0.0.dylib"
##	/usr/local/Cellar/glib/2.60.3/lib/libglib-2.0.0.dylib 
##	/usr/local/Cellar/glib/2.60.3/lib/libgobject-2.0.0.dylib 
##	/usr/local/Cellar/glib/2.60.3/lib/libgmodule-2.0.0.dylib 
	install_name_tool -change ${OPTPATH}glib/lib/libgio-2.0.0.dylib     ${TGTPATH}libgio-2.0.0.dylib     ${CPPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/glib/2.60.3/lib/libglib-2.0.0.dylib    ${TGTPATH}libglib-2.0.0.dylib    ${CPPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/glib/2.60.3/lib/libgobject-2.0.0.dylib  ${TGTPATH}libgobject-2.0.0.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/glib/2.60.3/lib/libgmodule-2.0.0.dylib  ${TGTPATH}libgmodule-2.0.0.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib     ${TGTPATH}libintl.8.dylib     ${CPPATH}${LIBFILE}
LIBFILE="libgobject-2.0.0.dylib"
##	/usr/local/Cellar/glib/2.60.3/lib/libglib-2.0.0.dylib 
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib ${TGTPATH}libgobject-2.0.0.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/glib/2.60.3/lib/libglib-2.0.0.dylib ${TGTPATH}libglib-2.0.0.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}libffi/lib/libffi.6.dylib    ${TGTPATH}libffi.6.dylib    ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib  ${TGTPATH}libintl.8.dylib  ${CPPATH}${LIBFILE}
LIBFILE="libgmodule-2.0.0.dylib"
##	/usr/local/Cellar/glib/2.60.3/lib/libglib-2.0.0.dylib 
	install_name_tool -change ${OPTPATH}glib/lib/libgmodule-2.0.0.dylib ${TGTPATH}libgmodule-2.0.0.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/glib/2.60.3/lib/libglib-2.0.0.dylib ${TGTPATH}libglib-2.0.0.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib  ${TGTPATH}libintl.8.dylib  ${CPPATH}${LIBFILE}
LIBFILE="libglib-2.0.0.dylib"
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib 	       ${TGTPATH}libglib-2.0.0.dylib 	       ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}pcre/lib/libpcre.1.dylib    ${TGTPATH}libpcre.1.dylib    ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib ${TGTPATH}libintl.8.dylib ${CPPATH}${LIBFILE}
LIBFILE="libintl.8.dylib"
	install_name_tool -change ${OPTPATH}/gettext/lib/libintl.8.dylib	${TGTPATH}libintl.8.dylib  ${CPPATH}${LIBFILE}
LIBFILE="libssl.1.0.0.dylib"
##	/usr/local/Cellar/openssl/1.0.2r/lib/libcrypto.1.0.0.dylib
	install_name_tool -change ${OPTPATH}openssl/lib/libssl.1.0.0.dylib ${TGTPATH}libssl.1.0.0.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/openssl/1.0.2r/lib/libcrypto.1.0.0.dylib ${TGTPATH}libcrypto.1.0.0.dylib ${CPPATH}${LIBFILE}
LIBFILE="libcrypto.1.0.0.dylib"
##	/usr/local/Cellar/openssl/1.0.2r/lib/libcrypto.1.0.0.dylib
	install_name_tool -change /usr/local/Cellar/openssl/1.0.2r/lib/libcrypto.1.0.0.dylib ${TGTPATH}libcrypto.1.0.0.dylib ${CPPATH}${LIBFILE}


LIBFILE="libepoxy.0.dylib"
	install_name_tool -change ${OPTPATH}libepoxy/lib/libepoxy.0.dylib ${TGTPATH}libepoxy.0.dylib  ${CPPATH}${LIBFILE}
LIBFILE="libfribidi.0.dylib"
	install_name_tool -change ${OPTPATH}fribidi/lib/libfribidi.0.dylib ${TGTPATH}libfribidi.0.dylib  ${CPPATH}${LIBFILE}
LIBFILE="libharfbuzz.0.dylib"
	install_name_tool -change ${OPTPATH}harfbuzz/lib/libharfbuzz.0.dylib ${TGTPATH}libharfbuzz.0.dylib  ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib ${TGTPATH}libglib-2.0.0.dylib  ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib ${TGTPATH}libintl.8.dylib  ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}freetype/lib/libfreetype.6.dylib ${TGTPATH}libfreetype.6.dylib  ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}graphite2/lib/libgraphite2.3.dylib ${TGTPATH}libgraphite2.3.dylib  ${CPPATH}${LIBFILE}
LIBFILE="libpangoft2-1.0.0.dylib"
##	/usr/local/Cellar/pango/1.42.4_1/lib/libpango-1.0.0.dylib
	install_name_tool -change /usr/local/Cellar/pango/1.42.4_1/lib/libpango-1.0.0.dylib	   ${TGTPATH}libpango-1.0.0.dylib 	   ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}pango/lib/libpangoft2-1.0.0.dylib ${TGTPATH}libpangoft2-1.0.0.dylib  ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}fribidi/lib/libfribidi.0.dylib ${TGTPATH}libfribidi.0.dylib  ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib ${TGTPATH}libgobject-2.0.0.dylib  ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgthread-2.0.0.dylib ${TGTPATH}libgthread-2.0.0.dylib  ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib ${TGTPATH}libglib-2.0.0.dylib  ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib ${TGTPATH}libintl.8.dylib  ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}harfbuzz/lib/libharfbuzz.0.dylib ${TGTPATH}libharfbuzz.0.dylib  ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}fontconfig/lib/libfontconfig.1.dylib ${TGTPATH}libfontconfig.1.dylib  ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}freetype/lib/libfreetype.6.dylib ${TGTPATH}libfreetype.6.dylib  ${CPPATH}${LIBFILE}
LIBFILE="libfontconfig.1.dylib"
	install_name_tool -change ${OPTPATH}fontconfig/lib/libfontconfig.1.dylib ${TGTPATH}libfontconfig.1.dylib  ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}freetype/lib/libfreetype.6.dylib ${TGTPATH}libfreetype.6.dylib  ${CPPATH}${LIBFILE}
LIBFILE="libfreetype.6.dylib"
	install_name_tool -change ${OPTPATH}freetype/lib/libfreetype.6.dylib ${TGTPATH}libfreetype.6.dylib  ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}libpng/lib/libpng16.16.dylib ${TGTPATH}libpng16.16.dylib  ${CPPATH}${LIBFILE}
LIBFILE="libgthread-2.0.0.dylib"
##	/usr/local/Cellar/glib/2.60.3/lib/libglib-2.0.0.dylib
	install_name_tool -change /usr/local/Cellar/glib/2.60.3/lib/libglib-2.0.0.dylib ${TGTPATH}libglib-2.0.0.dylib  ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgthread-2.0.0.dylib ${TGTPATH}libgthread-2.0.0.dylib  ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib ${TGTPATH}libintl.8.dylib  ${CPPATH}${LIBFILE}
LIBFILE="libpixman-1.0.dylib"
	install_name_tool -change ${OPTPATH}pixman/lib/libpixman-1.0.dylib ${TGTPATH}libpixman-1.0.dylib  ${CPPATH}${LIBFILE}
LIBFILE="libpng16.16.dylib"
	install_name_tool -change ${OPTPATH}libpng/lib/libpng16.16.dylib ${TGTPATH}libpng16.16.dylib  ${CPPATH}${LIBFILE}
LIBFILE="libffi.6.dylib"
	install_name_tool -change ${OPTPATH}libffi/lib/libffi.6.dylib ${TGTPATH}libffi.6.dylib  ${CPPATH}${LIBFILE}
LIBFILE="libpcre.1.dylib"
	install_name_tool -change ${OPTPATH}pcre/lib/libpcre.1.dylib ${TGTPATH}libpcre.1.dylib  ${CPPATH}${LIBFILE}

LIBFILE="libgraphite2.3.dylib"
	install_name_tool -change ${OPTPATH}graphite2/lib/libgraphite2.3.dylib ${TGTPATH}libgraphite2.3.dylib ${CPPATH}${LIBFILE}
