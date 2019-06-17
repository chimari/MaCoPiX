SUBDIRS= intl m4 po src

EXTRA_DIST =  config.rpath\
	ChangeLog.jp \
	README.jp \
	README-win32-jp.txt \
	README-win32-en.txt \
	NKR.README.jp \
	autogen.sh\
	macopix.1\
	makewin32.sh\
	makewin64.sh\
	makeosx.sh\
	macos_macopix_linklib.sh\
	macopix.ico\
	macopix.spec

man_MANS = macopix.1

CLEANFILES      = *~ libnova/*~ icons/*~ debian/*~
DISTCLEANFILES  = 
AM_CPPFLAGS = -DLOCALEDIR=\""$(localedir)"\"

BZIP2_ENV =
GZIP_ENV =
TAR = tar

release: distdir
	-chmod -R a+r $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) -chozf $(distdir).tar.gz $(distdir)
	BZIP2=$(BZIP2_ENV) $(TAR) --bzip2 -chof $(distdir).tar.bz2 $(distdir)
	-rm -rf $(distdir)


ACLOCAL_AMFLAGS = -I m4
