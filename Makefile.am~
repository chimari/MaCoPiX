SUBDIRS= m4 po src

EXTRA_DIST =  config.rpath\
	ChangeLog.ja \
	README.ja \
	README-win32-ja.txt \
	README-win32-en.txt \
	NKR.README.ja \
	autogen.sh\
	macopix.1\
	makewin32.sh

man_MANS = macopix.1

BZIP2_ENV =
GZIP_ENV =
TAR = tar

release: distdir
	-chmod -R a+r $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) -chozf $(distdir).tar.gz $(distdir)
	BZIP2=$(BZIP2_ENV) $(TAR) --bzip2 -chof $(distdir).tar.bz2 $(distdir)
	-rm -rf $(distdir)


ACLOCAL_AMFLAGS = -I m4
