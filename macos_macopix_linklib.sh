#!/bin/sh

TGTPATH="@executable_path/../Resources/lib/"
OPTPATH="/usr/local/opt/"
CPPATH="/Volumes/MaCoPiX_macOS/MaCoPiX.app/Contents/Resources/lib/"
MACOPIX_BIN="/Volumes/MaCoPiX_macOS/MaCoPiX.app/Contents/MacOS/MaCoPiX-bin"
LOCALEPATH="/Volumes/MaCoPiX_macOS/MaCoPiX.app/Contents/Resources/share/locale/ja/LC_MESSAGES/"
LOCAL_IMPATH="/usr/local/opt/gtk+3/lib/gtk-3.0/3.0.0/immodules/"
TGT_IMPATH="/Volumes/MaCoPiX_macOS/MaCoPiX.app/Contents/Resources/lib/gtk-3.0/3.0.0/immodules/"
LOCAL_PLPATH="/usr/local/opt/gdk-pixbuf/lib/gdk-pixbuf-2.0/2.10.0/loaders/"
TGT_PLPATH="/Volumes/MaCoPiX_macOS/MaCoPiX.app/Contents/Resources/lib/gdk-pixbuf-2.0/2.10.0/loaders/"

cp src/macopix ${MACOPIX_BIN}
cp po/ja.gmo ${LOCALEPATH}macopix.mo

## Change links in MaCoPiX-bin
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

cp ${OPTPATH}jpeg/lib/libjpeg.9.dylib	${CPPATH}libjpeg.9.dylib
cp ${OPTPATH}libpng/lib/libpng16.16.dylib ${CPPATH}libpng16.16.dylib
cp ${OPTPATH}libtiff/lib/libtiff.5.dylib ${CPPATH}libtiff.5.dylib

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
## /usr/local/Cellar/cairo/1.16.0_2/lib/libcairo.2.dylib	
	install_name_tool -change /usr/local/Cellar/cairo/1.16.0_2/lib/libcairo.2.dylib ${TGTPATH}libcairo.2.dylib ${CPPATH}${LIBFILE}
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
##	/usr/local/Cellar/glib/2.60.4_1/lib/libglib-2.0.0.dylib 
##	/usr/local/Cellar/glib/2.60.4_1/lib/libgobject-2.0.0.dylib 
##	/usr/local/Cellar/glib/2.60.4_1/lib/libgmodule-2.0.0.dylib 
	install_name_tool -change ${OPTPATH}glib/lib/libgio-2.0.0.dylib     ${TGTPATH}libgio-2.0.0.dylib     ${CPPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/glib/2.60.4_1/lib/libglib-2.0.0.dylib    ${TGTPATH}libglib-2.0.0.dylib    ${CPPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/glib/2.60.4_1/lib/libgobject-2.0.0.dylib  ${TGTPATH}libgobject-2.0.0.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/glib/2.60.4_1/lib/libgmodule-2.0.0.dylib  ${TGTPATH}libgmodule-2.0.0.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib     ${TGTPATH}libintl.8.dylib     ${CPPATH}${LIBFILE}
LIBFILE="libgobject-2.0.0.dylib"
##	/usr/local/Cellar/glib/2.60.4_1/lib/libglib-2.0.0.dylib 
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib ${TGTPATH}libgobject-2.0.0.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/glib/2.60.4_1/lib/libglib-2.0.0.dylib ${TGTPATH}libglib-2.0.0.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}libffi/lib/libffi.6.dylib    ${TGTPATH}libffi.6.dylib    ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib  ${TGTPATH}libintl.8.dylib  ${CPPATH}${LIBFILE}
LIBFILE="libgmodule-2.0.0.dylib"
##	/usr/local/Cellar/glib/2.60.4_1/lib/libglib-2.0.0.dylib 
	install_name_tool -change ${OPTPATH}glib/lib/libgmodule-2.0.0.dylib ${TGTPATH}libgmodule-2.0.0.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/glib/2.60.4_1/lib/libglib-2.0.0.dylib ${TGTPATH}libglib-2.0.0.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib  ${TGTPATH}libintl.8.dylib  ${CPPATH}${LIBFILE}
LIBFILE="libglib-2.0.0.dylib"
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib 	       ${TGTPATH}libglib-2.0.0.dylib 	       ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}pcre/lib/libpcre.1.dylib    ${TGTPATH}libpcre.1.dylib    ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib ${TGTPATH}libintl.8.dylib ${CPPATH}${LIBFILE}
LIBFILE="libintl.8.dylib"
	install_name_tool -change ${OPTPATH}/gettext/lib/libintl.8.dylib	${TGTPATH}libintl.8.dylib  ${CPPATH}${LIBFILE}
LIBFILE="libssl.1.0.0.dylib"
##	/usr/local/Cellar/openssl/1.0.2s/lib/libcrypto.1.0.0.dylib
	install_name_tool -change ${OPTPATH}openssl/lib/libssl.1.0.0.dylib ${TGTPATH}libssl.1.0.0.dylib ${CPPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/openssl/1.0.2s/lib/libcrypto.1.0.0.dylib ${TGTPATH}libcrypto.1.0.0.dylib ${CPPATH}${LIBFILE}
LIBFILE="libcrypto.1.0.0.dylib"
##	/usr/local/Cellar/openssl/1.0.2s/lib/libcrypto.1.0.0.dylib
	install_name_tool -change /usr/local/Cellar/openssl/1.0.2s/lib/libcrypto.1.0.0.dylib ${TGTPATH}libcrypto.1.0.0.dylib ${CPPATH}${LIBFILE}


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
##	/usr/local/Cellar/glib/2.60.4_1/lib/libglib-2.0.0.dylib
	install_name_tool -change /usr/local/Cellar/glib/2.60.4_1/lib/libglib-2.0.0.dylib ${TGTPATH}libglib-2.0.0.dylib  ${CPPATH}${LIBFILE}
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

LIBFILE="libgraphite2.3.dylib"
	install_name_tool -change ${OPTPATH}graphite2/lib/libgraphite2.3.dylib ${TGTPATH}libgraphite2.3.dylib ${CPPATH}${LIBFILE}
LIBFILE="libtiff.5.dylib"
	install_name_tool -change ${OPTPATH}libtiff/lib/libtiff.5.dylib ${TGTPATH}libtiff.5.dylib  ${CPPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}jpeg/lib/libjpeg.9.dylib ${TGTPATH}libjpeg.9.dylib   ${CPPATH}${LIBFILE}

## IMMODULES

cp ${LOCAL_IMPATH}im-am-et.so             ${TGT_IMPATH}im-am-et.so
cp ${LOCAL_IMPATH}im-cedilla.so           ${TGT_IMPATH}im-cedilla.so
cp ${LOCAL_IMPATH}im-cyrillic-translit.so ${TGT_IMPATH}im-cyrillic-translit.so
cp ${LOCAL_IMPATH}im-inuktitut.so         ${TGT_IMPATH}im-inuktitut.so
cp ${LOCAL_IMPATH}im-ipa.so               ${TGT_IMPATH}im-ipa.so
cp ${LOCAL_IMPATH}im-multipress.so        ${TGT_IMPATH}im-multipress.so
cp ${LOCAL_IMPATH}im-quartz.so            ${TGT_IMPATH}im-quartz.so
cp ${LOCAL_IMPATH}im-thai.so              ${TGT_IMPATH}im-thai.so
cp ${LOCAL_IMPATH}im-ti-er.so             ${TGT_IMPATH}im-ti-er.so
cp ${LOCAL_IMPATH}im-ti-et.so             ${TGT_IMPATH}im-ti-et.so
cp ${LOCAL_IMPATH}im-viqr.so              ${TGT_IMPATH}im-viqr.so

chmod 755 ${TGT_IMPATH}*.so	       
	
LIBFILE="im-am-et.so"
	install_name_tool -change /usr/local/Cellar/gtk+3/3.24.8/lib/libgtk-3.0.dylib ${TGTPATH}libgtk-3.0.dylib ${TGT_IMPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/gtk+3/3.24.8/lib/libgdk-3.0.dylib ${TGTPATH}libgdk-3.0.dylib ${TGT_IMPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgmodule-2.0.0.dylib           ${TGTPATH}libgmodule-2.0.0.dylib ${TGT_IMPATH}${LIBFILE}            
	install_name_tool -change ${OPTPATH}pango/lib/libpangocairo-1.0.0.dylib       ${TGTPATH}libpangocairo-1.0.0.dylib ${TGT_IMPATH}${LIBFILE}        
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo-gobject.2.dylib 	      ${TGTPATH}libcairo-gobject.2.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo.2.dylib 		      ${TGTPATH}libcairo.2.dylib ${TGT_IMPATH}${LIBFILE} 		       
	install_name_tool -change ${OPTPATH}gdk-pixbuf/lib/libgdk_pixbuf-2.0.0.dylib  ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_IMPATH}${LIBFILE}   
	install_name_tool -change ${OPTPATH}atk/lib/libatk-1.0.0.dylib 		      ${TGTPATH}libatk-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 		       
	install_name_tool -change ${OPTPATH}libepoxy/lib/libepoxy.0.dylib 	      ${TGTPATH}libepoxy.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}fribidi/lib/libfribidi.0.dylib 	      ${TGTPATH}libfribidi.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}glib/lib/libgio-2.0.0.dylib 	      ${TGTPATH}libgio-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}harfbuzz/lib/libharfbuzz.0.dylib 	      ${TGTPATH}libharfbuzz.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}pango/lib/libpangoft2-1.0.0.dylib 	      ${TGTPATH}libpangoft2-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}pango/lib/libpango-1.0.0.dylib 	      ${TGTPATH}libpango-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib 	      ${TGTPATH}libgobject-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib 	      ${TGTPATH}libglib-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}fontconfig/lib/libfontconfig.1.dylib      ${TGTPATH}libfontconfig.1.dylib ${TGT_IMPATH}${LIBFILE}       
	install_name_tool -change ${OPTPATH}freetype/lib/libfreetype.6.dylib 	      ${TGTPATH}libfreetype.6.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib               ${TGTPATH}libintl.8.dylib ${TGT_IMPATH}${LIBFILE}                 
LIBFILE="im-cedilla.so"
	install_name_tool -change /usr/local/Cellar/gtk+3/3.24.8/lib/libgtk-3.0.dylib ${TGTPATH}libgtk-3.0.dylib ${TGT_IMPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/gtk+3/3.24.8/lib/libgdk-3.0.dylib ${TGTPATH}libgdk-3.0.dylib ${TGT_IMPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgmodule-2.0.0.dylib           ${TGTPATH}libgmodule-2.0.0.dylib ${TGT_IMPATH}${LIBFILE}           
	install_name_tool -change ${OPTPATH}pango/lib/libpangocairo-1.0.0.dylib       ${TGTPATH}libpangocairo-1.0.0.dylib ${TGT_IMPATH}${LIBFILE}       
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo-gobject.2.dylib 	      ${TGTPATH}libcairo-gobject.2.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo.2.dylib 		      ${TGTPATH}libcairo.2.dylib ${TGT_IMPATH}${LIBFILE} 		      
	install_name_tool -change ${OPTPATH}gdk-pixbuf/lib/libgdk_pixbuf-2.0.0.dylib  ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_IMPATH}${LIBFILE}  
	install_name_tool -change ${OPTPATH}atk/lib/libatk-1.0.0.dylib 		      ${TGTPATH}libatk-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 		      
	install_name_tool -change ${OPTPATH}libepoxy/lib/libepoxy.0.dylib 	      ${TGTPATH}libepoxy.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}fribidi/lib/libfribidi.0.dylib 	      ${TGTPATH}libfribidi.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libgio-2.0.0.dylib 	      ${TGTPATH}libgio-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}harfbuzz/lib/libharfbuzz.0.dylib 	      ${TGTPATH}libharfbuzz.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}pango/lib/libpangoft2-1.0.0.dylib 	      ${TGTPATH}libpangoft2-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}pango/lib/libpango-1.0.0.dylib 	      ${TGTPATH}libpango-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib 	      ${TGTPATH}libgobject-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib 	      ${TGTPATH}libglib-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}fontconfig/lib/libfontconfig.1.dylib      ${TGTPATH}libfontconfig.1.dylib ${TGT_IMPATH}${LIBFILE}      
	install_name_tool -change ${OPTPATH}freetype/lib/libfreetype.6.dylib 	      ${TGTPATH}libfreetype.6.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib               ${TGTPATH}libintl.8.dylib ${TGT_IMPATH}${LIBFILE}               
LIBFILE="im-cyrillic-translit.so"
	install_name_tool -change /usr/local/Cellar/gtk+3/3.24.8/lib/libgtk-3.0.dylib ${TGTPATH}libgtk-3.0.dylib ${TGT_IMPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/gtk+3/3.24.8/lib/libgdk-3.0.dylib ${TGTPATH}libgdk-3.0.dylib ${TGT_IMPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgmodule-2.0.0.dylib          ${TGTPATH}libgmodule-2.0.0.dylib ${TGT_IMPATH}${LIBFILE}            
	install_name_tool -change ${OPTPATH}pango/lib/libpangocairo-1.0.0.dylib      ${TGTPATH}libpangocairo-1.0.0.dylib ${TGT_IMPATH}${LIBFILE}        
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo-gobject.2.dylib 	     ${TGTPATH}libcairo-gobject.2.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo.2.dylib 		     ${TGTPATH}libcairo.2.dylib ${TGT_IMPATH}${LIBFILE} 		       
	install_name_tool -change ${OPTPATH}gdk-pixbuf/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_IMPATH}${LIBFILE}   
	install_name_tool -change ${OPTPATH}atk/lib/libatk-1.0.0.dylib 		     ${TGTPATH}libatk-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 		       
	install_name_tool -change ${OPTPATH}libepoxy/lib/libepoxy.0.dylib 	     ${TGTPATH}libepoxy.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}fribidi/lib/libfribidi.0.dylib 	     ${TGTPATH}libfribidi.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}glib/lib/libgio-2.0.0.dylib 	     ${TGTPATH}libgio-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}harfbuzz/lib/libharfbuzz.0.dylib 	     ${TGTPATH}libharfbuzz.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}pango/lib/libpangoft2-1.0.0.dylib 	     ${TGTPATH}libpangoft2-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}pango/lib/libpango-1.0.0.dylib 	     ${TGTPATH}libpango-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib 	     ${TGTPATH}libgobject-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib 	     ${TGTPATH}libglib-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}fontconfig/lib/libfontconfig.1.dylib     ${TGTPATH}libfontconfig.1.dylib ${TGT_IMPATH}${LIBFILE}       
	install_name_tool -change ${OPTPATH}freetype/lib/libfreetype.6.dylib 	     ${TGTPATH}libfreetype.6.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib              ${TGTPATH}libintl.8.dylib ${TGT_IMPATH}${LIBFILE}                  
LIBFILE="im-inuktitut.so"
	install_name_tool -change /usr/local/Cellar/gtk+3/3.24.8/lib/libgtk-3.0.dylib ${TGTPATH}libgtk-3.0.dylib ${TGT_IMPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/gtk+3/3.24.8/lib/libgdk-3.0.dylib ${TGTPATH}libgdk-3.0.dylib ${TGT_IMPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgmodule-2.0.0.dylib           ${TGTPATH}libgmodule-2.0.0.dylib ${TGT_IMPATH}${LIBFILE}           
	install_name_tool -change ${OPTPATH}pango/lib/libpangocairo-1.0.0.dylib       ${TGTPATH}libpangocairo-1.0.0.dylib ${TGT_IMPATH}${LIBFILE}       
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo-gobject.2.dylib 	      ${TGTPATH}libcairo-gobject.2.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo.2.dylib 		      ${TGTPATH}libcairo.2.dylib ${TGT_IMPATH}${LIBFILE} 		      
	install_name_tool -change ${OPTPATH}gdk-pixbuf/lib/libgdk_pixbuf-2.0.0.dylib  ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_IMPATH}${LIBFILE}  
	install_name_tool -change ${OPTPATH}atk/lib/libatk-1.0.0.dylib 		      ${TGTPATH}libatk-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 		      
	install_name_tool -change ${OPTPATH}libepoxy/lib/libepoxy.0.dylib 	      ${TGTPATH}libepoxy.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}fribidi/lib/libfribidi.0.dylib 	      ${TGTPATH}libfribidi.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libgio-2.0.0.dylib 	      ${TGTPATH}libgio-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}harfbuzz/lib/libharfbuzz.0.dylib 	      ${TGTPATH}libharfbuzz.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}pango/lib/libpangoft2-1.0.0.dylib 	      ${TGTPATH}libpangoft2-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}pango/lib/libpango-1.0.0.dylib 	      ${TGTPATH}libpango-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib 	      ${TGTPATH}libgobject-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib 	      ${TGTPATH}libglib-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}fontconfig/lib/libfontconfig.1.dylib      ${TGTPATH}libfontconfig.1.dylib ${TGT_IMPATH}${LIBFILE}      
	install_name_tool -change ${OPTPATH}freetype/lib/libfreetype.6.dylib 	      ${TGTPATH}libfreetype.6.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib               ${TGTPATH}libintl.8.dylib ${TGT_IMPATH}${LIBFILE}               
LIBFILE="im-ipa.so"
	install_name_tool -change /usr/local/Cellar/gtk+3/3.24.8/lib/libgtk-3.0.dylib ${TGTPATH}libgtk-3.0.dylib ${TGT_IMPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/gtk+3/3.24.8/lib/libgdk-3.0.dylib ${TGTPATH}libgdk-3.0.dylib ${TGT_IMPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgmodule-2.0.0.dylib          ${TGTPATH}libgmodule-2.0.0.dylib ${TGT_IMPATH}${LIBFILE}           
	install_name_tool -change ${OPTPATH}pango/lib/libpangocairo-1.0.0.dylib      ${TGTPATH}libpangocairo-1.0.0.dylib ${TGT_IMPATH}${LIBFILE}       
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo-gobject.2.dylib 	     ${TGTPATH}libcairo-gobject.2.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo.2.dylib 		     ${TGTPATH}libcairo.2.dylib ${TGT_IMPATH}${LIBFILE} 		      
	install_name_tool -change ${OPTPATH}gdk-pixbuf/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_IMPATH}${LIBFILE}  
	install_name_tool -change ${OPTPATH}atk/lib/libatk-1.0.0.dylib 		     ${TGTPATH}libatk-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 		      
	install_name_tool -change ${OPTPATH}libepoxy/lib/libepoxy.0.dylib 	     ${TGTPATH}libepoxy.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}fribidi/lib/libfribidi.0.dylib 	     ${TGTPATH}libfribidi.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libgio-2.0.0.dylib 	     ${TGTPATH}libgio-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}harfbuzz/lib/libharfbuzz.0.dylib 	     ${TGTPATH}libharfbuzz.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}pango/lib/libpangoft2-1.0.0.dylib 	     ${TGTPATH}libpangoft2-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}pango/lib/libpango-1.0.0.dylib 	     ${TGTPATH}libpango-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib 	     ${TGTPATH}libgobject-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib 	     ${TGTPATH}libglib-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}fontconfig/lib/libfontconfig.1.dylib     ${TGTPATH}libfontconfig.1.dylib ${TGT_IMPATH}${LIBFILE}      
	install_name_tool -change ${OPTPATH}freetype/lib/libfreetype.6.dylib 	     ${TGTPATH}libfreetype.6.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib              ${TGTPATH}libintl.8.dylib ${TGT_IMPATH}${LIBFILE}                
LIBFILE="im-multipress.so"
	install_name_tool -change /usr/local/Cellar/gtk+3/3.24.8/lib/libgtk-3.0.dylib ${TGTPATH}libgtk-3.0.dylib ${TGT_IMPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/gtk+3/3.24.8/lib/libgdk-3.0.dylib ${TGTPATH}libgdk-3.0.dylib ${TGT_IMPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgmodule-2.0.0.dylib          ${TGTPATH}libgmodule-2.0.0.dylib ${TGT_IMPATH}${LIBFILE}           
	install_name_tool -change ${OPTPATH}pango/lib/libpangocairo-1.0.0.dylib      ${TGTPATH}libpangocairo-1.0.0.dylib ${TGT_IMPATH}${LIBFILE}       
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo-gobject.2.dylib 	     ${TGTPATH}libcairo-gobject.2.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo.2.dylib 		     ${TGTPATH}libcairo.2.dylib ${TGT_IMPATH}${LIBFILE} 		      
	install_name_tool -change ${OPTPATH}gdk-pixbuf/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_IMPATH}${LIBFILE}  
	install_name_tool -change ${OPTPATH}atk/lib/libatk-1.0.0.dylib 		     ${TGTPATH}libatk-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 		      
	install_name_tool -change ${OPTPATH}libepoxy/lib/libepoxy.0.dylib 	     ${TGTPATH}libepoxy.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}fribidi/lib/libfribidi.0.dylib 	     ${TGTPATH}libfribidi.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libgio-2.0.0.dylib 	     ${TGTPATH}libgio-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}harfbuzz/lib/libharfbuzz.0.dylib 	     ${TGTPATH}libharfbuzz.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}pango/lib/libpangoft2-1.0.0.dylib 	     ${TGTPATH}libpangoft2-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}pango/lib/libpango-1.0.0.dylib 	     ${TGTPATH}libpango-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib 	     ${TGTPATH}libgobject-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib 	     ${TGTPATH}libglib-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}fontconfig/lib/libfontconfig.1.dylib     ${TGTPATH}libfontconfig.1.dylib ${TGT_IMPATH}${LIBFILE}      
	install_name_tool -change ${OPTPATH}freetype/lib/libfreetype.6.dylib 	     ${TGTPATH}libfreetype.6.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib              ${TGTPATH}libintl.8.dylib ${TGT_IMPATH}${LIBFILE}                
LIBFILE="im-quartz.so"
	install_name_tool -change /usr/local/Cellar/gtk+3/3.24.8/lib/libgtk-3.0.dylib ${TGTPATH}libgtk-3.0.dylib ${TGT_IMPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/gtk+3/3.24.8/lib/libgdk-3.0.dylib ${TGTPATH}libgdk-3.0.dylib ${TGT_IMPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgmodule-2.0.0.dylib          ${TGTPATH}libgmodule-2.0.0.dylib ${TGT_IMPATH}${LIBFILE}           
	install_name_tool -change ${OPTPATH}pango/lib/libpangocairo-1.0.0.dylib      ${TGTPATH}libpangocairo-1.0.0.dylib ${TGT_IMPATH}${LIBFILE}       
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo-gobject.2.dylib 	     ${TGTPATH}libcairo-gobject.2.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo.2.dylib 		     ${TGTPATH}libcairo.2.dylib ${TGT_IMPATH}${LIBFILE} 		      
	install_name_tool -change ${OPTPATH}gdk-pixbuf/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_IMPATH}${LIBFILE}  
	install_name_tool -change ${OPTPATH}atk/lib/libatk-1.0.0.dylib 		     ${TGTPATH}libatk-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 		      
	install_name_tool -change ${OPTPATH}libepoxy/lib/libepoxy.0.dylib 	     ${TGTPATH}libepoxy.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}fribidi/lib/libfribidi.0.dylib 	     ${TGTPATH}libfribidi.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libgio-2.0.0.dylib 	     ${TGTPATH}libgio-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}harfbuzz/lib/libharfbuzz.0.dylib 	     ${TGTPATH}libharfbuzz.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}pango/lib/libpangoft2-1.0.0.dylib 	     ${TGTPATH}libpangoft2-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}pango/lib/libpango-1.0.0.dylib 	     ${TGTPATH}libpango-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib 	     ${TGTPATH}libgobject-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib 	     ${TGTPATH}libglib-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}fontconfig/lib/libfontconfig.1.dylib     ${TGTPATH}libfontconfig.1.dylib ${TGT_IMPATH}${LIBFILE}      
	install_name_tool -change ${OPTPATH}freetype/lib/libfreetype.6.dylib 	     ${TGTPATH}libfreetype.6.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib              ${TGTPATH}libintl.8.dylib ${TGT_IMPATH}${LIBFILE}                
LIBFILE="im-thai.so"
	install_name_tool -change /usr/local/Cellar/gtk+3/3.24.8/lib/libgtk-3.0.dylib ${TGTPATH}libgtk-3.0.dylib ${TGT_IMPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/gtk+3/3.24.8/lib/libgdk-3.0.dylib ${TGTPATH}libgdk-3.0.dylib ${TGT_IMPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgmodule-2.0.0.dylib          ${TGTPATH}libgmodule-2.0.0.dylib ${TGT_IMPATH}${LIBFILE}           
	install_name_tool -change ${OPTPATH}pango/lib/libpangocairo-1.0.0.dylib      ${TGTPATH}libpangocairo-1.0.0.dylib ${TGT_IMPATH}${LIBFILE}       
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo-gobject.2.dylib 	     ${TGTPATH}libcairo-gobject.2.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo.2.dylib 		     ${TGTPATH}libcairo.2.dylib ${TGT_IMPATH}${LIBFILE} 		      
	install_name_tool -change ${OPTPATH}gdk-pixbuf/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_IMPATH}${LIBFILE}  
	install_name_tool -change ${OPTPATH}atk/lib/libatk-1.0.0.dylib 		     ${TGTPATH}libatk-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 		      
	install_name_tool -change ${OPTPATH}libepoxy/lib/libepoxy.0.dylib 	     ${TGTPATH}libepoxy.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}fribidi/lib/libfribidi.0.dylib 	     ${TGTPATH}libfribidi.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libgio-2.0.0.dylib 	     ${TGTPATH}libgio-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}harfbuzz/lib/libharfbuzz.0.dylib 	     ${TGTPATH}libharfbuzz.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}pango/lib/libpangoft2-1.0.0.dylib 	     ${TGTPATH}libpangoft2-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}pango/lib/libpango-1.0.0.dylib 	     ${TGTPATH}libpango-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib 	     ${TGTPATH}libgobject-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib 	     ${TGTPATH}libglib-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}fontconfig/lib/libfontconfig.1.dylib     ${TGTPATH}libfontconfig.1.dylib ${TGT_IMPATH}${LIBFILE}      
	install_name_tool -change ${OPTPATH}freetype/lib/libfreetype.6.dylib 	     ${TGTPATH}libfreetype.6.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib              ${TGTPATH}libintl.8.dylib ${TGT_IMPATH}${LIBFILE}                
LIBFILE="im-ti-er.so"
	install_name_tool -change /usr/local/Cellar/gtk+3/3.24.8/lib/libgtk-3.0.dylib ${TGTPATH}libgtk-3.0.dylib ${TGT_IMPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/gtk+3/3.24.8/lib/libgdk-3.0.dylib ${TGTPATH}libgdk-3.0.dylib ${TGT_IMPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgmodule-2.0.0.dylib          ${TGTPATH}libgmodule-2.0.0.dylib ${TGT_IMPATH}${LIBFILE}            
	install_name_tool -change ${OPTPATH}pango/lib/libpangocairo-1.0.0.dylib      ${TGTPATH}libpangocairo-1.0.0.dylib ${TGT_IMPATH}${LIBFILE}        
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo-gobject.2.dylib 	     ${TGTPATH}libcairo-gobject.2.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo.2.dylib 		     ${TGTPATH}libcairo.2.dylib ${TGT_IMPATH}${LIBFILE} 		       
	install_name_tool -change ${OPTPATH}gdk-pixbuf/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_IMPATH}${LIBFILE}    
	install_name_tool -change ${OPTPATH}atk/lib/libatk-1.0.0.dylib 		     ${TGTPATH}libatk-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 		       
	install_name_tool -change ${OPTPATH}libepoxy/lib/libepoxy.0.dylib 	     ${TGTPATH}libepoxy.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}fribidi/lib/libfribidi.0.dylib 	     ${TGTPATH}libfribidi.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}glib/lib/libgio-2.0.0.dylib 	     ${TGTPATH}libgio-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}harfbuzz/lib/libharfbuzz.0.dylib 	     ${TGTPATH}libharfbuzz.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}pango/lib/libpangoft2-1.0.0.dylib 	     ${TGTPATH}libpangoft2-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}pango/lib/libpango-1.0.0.dylib 	     ${TGTPATH}libpango-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib 	     ${TGTPATH}libgobject-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib 	     ${TGTPATH}libglib-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}fontconfig/lib/libfontconfig.1.dylib     ${TGTPATH}libfontconfig.1.dylib ${TGT_IMPATH}${LIBFILE}       
	install_name_tool -change ${OPTPATH}freetype/lib/libfreetype.6.dylib 	     ${TGTPATH}libfreetype.6.dylib ${TGT_IMPATH}${LIBFILE} 	       
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib              ${TGTPATH}libintl.8.dylib ${TGT_IMPATH}${LIBFILE}                   
LIBFILE="im-ti-et.so"
	install_name_tool -change /usr/local/Cellar/gtk+3/3.24.8/lib/libgtk-3.0.dylib ${TGTPATH}libgtk-3.0.dylib ${TGT_IMPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/gtk+3/3.24.8/lib/libgdk-3.0.dylib ${TGTPATH}libgdk-3.0.dylib ${TGT_IMPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgmodule-2.0.0.dylib          ${TGTPATH}libgmodule-2.0.0.dylib ${TGT_IMPATH}${LIBFILE}           
	install_name_tool -change ${OPTPATH}pango/lib/libpangocairo-1.0.0.dylib      ${TGTPATH}libpangocairo-1.0.0.dylib ${TGT_IMPATH}${LIBFILE}       
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo-gobject.2.dylib 	     ${TGTPATH}libcairo-gobject.2.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo.2.dylib 		     ${TGTPATH}libcairo.2.dylib ${TGT_IMPATH}${LIBFILE} 		      
	install_name_tool -change ${OPTPATH}gdk-pixbuf/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_IMPATH}${LIBFILE}  
	install_name_tool -change ${OPTPATH}atk/lib/libatk-1.0.0.dylib 		     ${TGTPATH}libatk-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 		      
	install_name_tool -change ${OPTPATH}libepoxy/lib/libepoxy.0.dylib 	     ${TGTPATH}libepoxy.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}fribidi/lib/libfribidi.0.dylib 	     ${TGTPATH}libfribidi.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libgio-2.0.0.dylib 	     ${TGTPATH}libgio-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}harfbuzz/lib/libharfbuzz.0.dylib 	     ${TGTPATH}libharfbuzz.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}pango/lib/libpangoft2-1.0.0.dylib 	     ${TGTPATH}libpangoft2-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}pango/lib/libpango-1.0.0.dylib 	     ${TGTPATH}libpango-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib 	     ${TGTPATH}libgobject-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib 	     ${TGTPATH}libglib-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}fontconfig/lib/libfontconfig.1.dylib     ${TGTPATH}libfontconfig.1.dylib ${TGT_IMPATH}${LIBFILE}      
	install_name_tool -change ${OPTPATH}freetype/lib/libfreetype.6.dylib 	     ${TGTPATH}libfreetype.6.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib              ${TGTPATH}libintl.8.dylib ${TGT_IMPATH}${LIBFILE}                
LIBFILE="im-viqr.so"
	install_name_tool -change /usr/local/Cellar/gtk+3/3.24.8/lib/libgtk-3.0.dylib ${TGTPATH}libgtk-3.0.dylib ${TGT_IMPATH}${LIBFILE}
	install_name_tool -change /usr/local/Cellar/gtk+3/3.24.8/lib/libgdk-3.0.dylib ${TGTPATH}libgdk-3.0.dylib ${TGT_IMPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgmodule-2.0.0.dylib          ${TGTPATH}libgmodule-2.0.0.dylib ${TGT_IMPATH}${LIBFILE}           
	install_name_tool -change ${OPTPATH}pango/lib/libpangocairo-1.0.0.dylib      ${TGTPATH}libpangocairo-1.0.0.dylib ${TGT_IMPATH}${LIBFILE}       
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo-gobject.2.dylib 	     ${TGTPATH}libcairo-gobject.2.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}cairo/lib/libcairo.2.dylib 		     ${TGTPATH}libcairo.2.dylib ${TGT_IMPATH}${LIBFILE} 		      
	install_name_tool -change ${OPTPATH}gdk-pixbuf/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_IMPATH}${LIBFILE}  
	install_name_tool -change ${OPTPATH}atk/lib/libatk-1.0.0.dylib 		     ${TGTPATH}libatk-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 		      
	install_name_tool -change ${OPTPATH}libepoxy/lib/libepoxy.0.dylib 	     ${TGTPATH}libepoxy.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}fribidi/lib/libfribidi.0.dylib 	     ${TGTPATH}libfribidi.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libgio-2.0.0.dylib 	     ${TGTPATH}libgio-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}harfbuzz/lib/libharfbuzz.0.dylib 	     ${TGTPATH}libharfbuzz.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}pango/lib/libpangoft2-1.0.0.dylib 	     ${TGTPATH}libpangoft2-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}pango/lib/libpango-1.0.0.dylib 	     ${TGTPATH}libpango-1.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib 	     ${TGTPATH}libgobject-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib 	     ${TGTPATH}libglib-2.0.0.dylib ${TGT_IMPATH}${LIBFILE} 	      
	install_name_tool -change ${OPTPATH}fontconfig/lib/libfontconfig.1.dylib     ${TGTPATH}libfontconfig.1.dylib ${TGT_IMPATH}${LIBFILE}      
	install_name_tool -change ${OPTPATH}freetype/lib/libfreetype.6.dylib 	     ${TGTPATH}libfreetype.6.dylib ${TGT_IMPATH}${LIBFILE} 	   
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib              ${TGTPATH}libintl.8.dylib ${TGT_IMPATH}${LIBFILE}    



## GdkPixbuf-loaders
cp ${LOCAL_PLPATH}libpixbufloader-ani.so  ${TGT_PLPATH}libpixbufloader-ani.so
cp ${LOCAL_PLPATH}libpixbufloader-bmp.so  ${TGT_PLPATH}libpixbufloader-bmp.so 	 
cp ${LOCAL_PLPATH}libpixbufloader-gif.so  ${TGT_PLPATH}libpixbufloader-gif.so	 
cp ${LOCAL_PLPATH}libpixbufloader-icns.so ${TGT_PLPATH}libpixbufloader-icns.so	 
cp ${LOCAL_PLPATH}libpixbufloader-ico.so  ${TGT_PLPATH}libpixbufloader-ico.so	 
cp ${LOCAL_PLPATH}libpixbufloader-jpeg.so ${TGT_PLPATH}libpixbufloader-jpeg.so	 
cp ${LOCAL_PLPATH}libpixbufloader-png.so  ${TGT_PLPATH}libpixbufloader-png.so	 
cp ${LOCAL_PLPATH}libpixbufloader-pnm.so  ${TGT_PLPATH}libpixbufloader-pnm.so	 
cp ${LOCAL_PLPATH}libpixbufloader-qtif.so ${TGT_PLPATH}libpixbufloader-qtif.so	 
cp ${LOCAL_PLPATH}libpixbufloader-tga.so  ${TGT_PLPATH}libpixbufloader-tga.so	 
cp ${LOCAL_PLPATH}libpixbufloader-tiff.so ${TGT_PLPATH}libpixbufloader-tiff.so	 
cp ${LOCAL_PLPATH}libpixbufloader-xbm.so  ${TGT_PLPATH}libpixbufloader-xbm.so	 
cp ${LOCAL_PLPATH}libpixbufloader-xpm.so  ${TGT_PLPATH}libpixbufloader-xpm.so

chmod 755 ${TGT_PLPATH}*.so
	
LIBFILE="libpixbufloader-ani.so"
	install_name_tool -change /usr/local/Cellar/gdk-pixbuf/2.38.1/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib ${TGTPATH}libgobject-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib  ${TGTPATH}libglib-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
LIBFILE="libpixbufloader-bmp.so"
	install_name_tool -change /usr/local/Cellar/gdk-pixbuf/2.38.1/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib  ${TGTPATH}libgobject-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib  ${TGTPATH}libglib-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
LIBFILE="libpixbufloader-gif.so"
	install_name_tool -change /usr/local/Cellar/gdk-pixbuf/2.38.1/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib  ${TGTPATH}libgobject-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib  ${TGTPATH}libglib-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
LIBFILE="libpixbufloader-icns.so"
	install_name_tool -change /usr/local/Cellar/gdk-pixbuf/2.38.1/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib  ${TGTPATH}libgobject-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib  ${TGTPATH}libglib-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
LIBFILE="libpixbufloader-ico.so"
	install_name_tool -change /usr/local/Cellar/gdk-pixbuf/2.38.1/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib  ${TGTPATH}libgobject-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib  ${TGTPATH}libglib-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgio-2.0.0.dylib  ${TGTPATH}libgio-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
LIBFILE="libpixbufloader-jpeg.so"
	install_name_tool -change /usr/local/Cellar/gdk-pixbuf/2.38.1/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}jpeg/lib/libjpeg.9.dylib ${TGTPATH}libjpeg.9.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib  ${TGTPATH}libgobject-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib  ${TGTPATH}libglib-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
LIBFILE="libpixbufloader-png.so"
	install_name_tool -change /usr/local/Cellar/gdk-pixbuf/2.38.1/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}libpng/lib/libpng16.16.dylib ${TGTPATH}libpng16.16.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib  ${TGTPATH}libgobject-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib  ${TGTPATH}libglib-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
LIBFILE="libpixbufloader-pnm.so"
	install_name_tool -change /usr/local/Cellar/gdk-pixbuf/2.38.1/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib  ${TGTPATH}libgobject-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib  ${TGTPATH}libglib-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
LIBFILE="libpixbufloader-qtif.so"
	install_name_tool -change /usr/local/Cellar/gdk-pixbuf/2.38.1/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib  ${TGTPATH}libgobject-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib  ${TGTPATH}libglib-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}gettext/lib/libintl.8.dylib  ${TGTPATH}libintl.8.dylib ${TGT_PLPATH}${LIBFILE}
LIBFILE="libpixbufloader-tga.so"
	install_name_tool -change /usr/local/Cellar/gdk-pixbuf/2.38.1/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib  ${TGTPATH}libgobject-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib  ${TGTPATH}libglib-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
LIBFILE="libpixbufloader-tiff.so"
	install_name_tool -change /usr/local/Cellar/gdk-pixbuf/2.38.1/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}libtiff/lib/libtiff.5.dylib ${TGTPATH}libtiff.5.dylib  ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib  ${TGTPATH}libgobject-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib  ${TGTPATH}libglib-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
LIBFILE="libpixbufloader-xbm.so"
	install_name_tool -change /usr/local/Cellar/gdk-pixbuf/2.38.1/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib  ${TGTPATH}libgobject-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib  ${TGTPATH}libglib-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
LIBFILE="libpixbufloader-xpm.so"
	install_name_tool -change /usr/local/Cellar/gdk-pixbuf/2.38.1/lib/libgdk_pixbuf-2.0.0.dylib ${TGTPATH}libgdk_pixbuf-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libgobject-2.0.0.dylib  ${TGTPATH}libgobject-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	install_name_tool -change ${OPTPATH}glib/lib/libglib-2.0.0.dylib  ${TGTPATH}libglib-2.0.0.dylib ${TGT_PLPATH}${LIBFILE}
	

