ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = foreign

pkgdatadir = @datadir@/aranym
man1dir = $(mandir)/man1

# Unix makefile for ARAnyM
# Inspired in BasiliskII's Makefile

SUBDIRS = src doc

## Rules

.PHONY: depend
SUFFIXES = .c .cpp .s .o .h .m .M

BUILT_SOURCES = \
	version_date.h \
	$(empty)

version_date.h:
	@echo generating $@
	@( if test -f "$(top_srcdir)/.isrelease"; then \
		echo '/* generated from version.h */'; \
		echo '#define VERSION_DATE " " RELEASE_DATE " (release)"'; \
	  elif test -d "$(top_srcdir)/.git"; then \
		echo '/* generated from git log */'; \
		cd "$(top_srcdir)" && git log -n 1 --pretty=format:'#define VERSION_DATE " %ad (git:%h)"%n' --date=format:%Y/%m/%d; \
	  else \
		echo '/* generated from local source */'; \
		echo '#define VERSION_DATE " " __DATE__ " (local)"'; \
	  fi ) > $@

built_sources: $(BUILT_SOURCES)

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(bindir) $(DESTDIR)$(man1dir) $(DESTDIR)$(pkgdatadir) $(DESTDIR)$(pkgdatadir)/atari $(DESTDIR)$(pkgdatadir)/atari/hostfs $(DESTDIR)$(pkgdatadir)/atari/nfcdrom $(DESTDIR)$(pkgdatadir)/atari/nfjpeg $(DESTDIR)$(pkgdatadir)/atari/nfosmesa $(DESTDIR)$(pkgdatadir)/atari/network $(DESTDIR)$(pkgdatadir)/atari/tools $(DESTDIR)$(pkgdatadir)/atari/sound $(DESTDIR)$(docdir)
	$(INSTALL_DATA) $(top_srcdir)/ChangeLog $(DESTDIR)$(docdir)/changelog
	$(INSTALL_DATA) $(top_srcdir)/TODO $(DESTDIR)$(docdir)/TODO
	$(INSTALL_DATA) $(top_srcdir)/AUTHORS $(DESTDIR)$(docdir)/AUTHORS
	$(INSTALL_DATA) $(top_srcdir)/NEWS $(DESTDIR)$(docdir)/NEWS
	$(INSTALL_DATA) $(top_srcdir)/README.md $(DESTDIR)$(docdir)/README
	$(INSTALL_DATA) $(top_srcdir)/FAQ $(DESTDIR)$(docdir)/FAQ
	$(INSTALL_DATA) $(top_srcdir)/COPYING $(DESTDIR)$(docdir)/COPYING
	$(INSTALL_DATA) $(top_srcdir)/BUGS $(DESTDIR)$(docdir)/BUGS
	$(INSTALL_DATA) $(top_srcdir)/INSTALL $(DESTDIR)$(docdir)/INSTALL
	$(INSTALL_DATA) $(top_srcdir)/doc/config $(DESTDIR)$(docdir)/config
	$(INSTALL_DATA) $(top_srcdir)/atari/newdesk.inf $(DESTDIR)$(pkgdatadir)/atari/newdesk.inf
	$(INSTALL_DATA) $(top_srcdir)/atari/mmusetup.cnf $(DESTDIR)$(pkgdatadir)/atari/mmusetup.cnf
	$(INSTALL_DATA) $(top_srcdir)/atari/hostfs/bdconfig.sys $(DESTDIR)$(pkgdatadir)/atari/hostfs/bdconfig.sys
	$(INSTALL_DATA) $(top_srcdir)/atari/hostfs/README $(DESTDIR)$(pkgdatadir)/atari/hostfs/README
	$(INSTALL_DATA) $(top_srcdir)/extras/hostfs/hostfs.dos $(DESTDIR)$(pkgdatadir)/atari/hostfs/hostfs.dos
	$(INSTALL_DATA) $(top_srcdir)/extras/nfcdrom/nfcdrom.bos $(DESTDIR)$(pkgdatadir)/atari/nfcdrom/nfcdrom.bos
	$(INSTALL_DATA) $(top_srcdir)/extras/nfjpeg/nfjpeg.prg $(DESTDIR)$(pkgdatadir)/atari/nfjpeg/nfjpeg.prg
	$(INSTALL_DATA) $(top_srcdir)/atari/nfosmesa/README $(DESTDIR)$(pkgdatadir)/atari/nfosmesa/README
	$(INSTALL_DATA) $(top_srcdir)/atari/network/ethernet/nfeth-config/eth0-config.sh $(DESTDIR)$(pkgdatadir)/atari/network/eth0-config.sh
	$(INSTALL_DATA) $(top_srcdir)/atari/network/ethernet/README $(DESTDIR)$(pkgdatadir)/atari/network/README
	$(INSTALL) $(top_srcdir)/tools/arabridge $(DESTDIR)$(pkgdatadir)/arabridge
	$(INSTALL) $(top_srcdir)/tools/floppy.sh $(DESTDIR)$(pkgdatadir)/floppy.sh
	$(INSTALL_DATA) $(top_srcdir)/atari/tools/README $(DESTDIR)$(pkgdatadir)/atari/tools/README
	$(INSTALL_DATA) $(top_srcdir)/atari/tools/pc101us.kbd.bz2 $(DESTDIR)$(pkgdatadir)/atari/tools/pc101us.kbd.bz2
	$(INSTALL_DATA) $(top_srcdir)/atari/sound/README $(DESTDIR)$(pkgdatadir)/atari/sound/README
	$(INSTALL_DATA) $(top_srcdir)/data/wm_icon.bmp $(DESTDIR)$(pkgdatadir)/wm_icon.bmp
	$(INSTALL_DATA) $(top_srcdir)/data/logo.bmp $(DESTDIR)$(pkgdatadir)/logo.bmp
	for s in 32 48; do \
		$(mkinstalldirs) "${DESTDIR}${datadir}/icons/hicolor/$${s}x$${s}/apps/"; \
		$(INSTALL_DATA) $(top_srcdir)/contrib/icon-$${s}.png "${DESTDIR}${datadir}/icons/hicolor/$${s}x$${s}/apps/aranym.png"; \
		$(INSTALL_DATA) $(top_srcdir)/contrib/icon-$${s}.png "${DESTDIR}${datadir}/icons/hicolor/$${s}x$${s}/apps/aranym-jit.png"; \
		$(INSTALL_DATA) $(top_srcdir)/contrib/icon-$${s}.png "${DESTDIR}${datadir}/icons/hicolor/$${s}x$${s}/apps/aranym-mmu.png"; \
	done
	$(mkinstalldirs) "${DESTDIR}${datadir}/pixmaps"
	rm -f "${DESTDIR}${datadir}/pixmaps/aranym"*.png
	ln -s ../icons/hicolor/32x32/apps/aranym.png "${DESTDIR}${datadir}/pixmaps/aranym.png"
	ln -s ../icons/hicolor/32x32/apps/aranym.png "${DESTDIR}${datadir}/pixmaps/aranym-jit.png"
	ln -s ../icons/hicolor/32x32/apps/aranym.png "${DESTDIR}${datadir}/pixmaps/aranym-mmu.png"
	$(mkinstalldirs) "${DESTDIR}${datadir}/applications"
	for name in aranym aranym-jit aranym-mmu; do \
		$(INSTALL_DATA) "$(top_srcdir)/contrib/$${name}.desktop" "${DESTDIR}${datadir}/applications/$${name}.desktop"; \
	done

uninstall-local:
	rm -f $(DESTDIR)$(docdir)/changelog
	rm -f $(DESTDIR)$(docdir)/TODO
	rm -f $(DESTDIR)$(docdir)/AUTHORS
	rm -f $(DESTDIR)$(docdir)/NEWS
	rm -f $(DESTDIR)$(docdir)/README
	rm -f $(DESTDIR)$(docdir)/FAQ
	rm -f $(DESTDIR)$(docdir)/COPYING
	rm -f $(DESTDIR)$(docdir)/BUGS
	rm -f $(DESTDIR)$(docdir)/INSTALL
	rm -f $(DESTDIR)$(docdir)/config
	rm -f $(DESTDIR)$(pkgdatadir)/atari/newdesk.inf
	rm -f $(DESTDIR)$(pkgdatadir)/atari/mmusetup.cnf
	rm -f $(DESTDIR)$(pkgdatadir)/atari/hostfs/bdconfig.sys
	rm -f $(DESTDIR)$(pkgdatadir)/atari/hostfs/README
	rm -f $(DESTDIR)$(pkgdatadir)/atari/hostfs/hostfs.dos
	rm -f $(DESTDIR)$(pkgdatadir)/atari/nfcdrom/nfcdrom.bos
	rm -f $(DESTDIR)$(pkgdatadir)/atari/nfjpeg/nfjpeg.prg
	rm -f $(DESTDIR)$(pkgdatadir)/atari/nfosmesa/README
	rm -f $(DESTDIR)$(pkgdatadir)/atari/network/eth0-config.sh
	rm -f $(DESTDIR)$(pkgdatadir)/atari/network/README
	rm -f $(DESTDIR)$(pkgdatadir)/arabridge
	rm -f $(DESTDIR)$(pkgdatadir)/floppy.sh
	rm -f $(DESTDIR)$(pkgdatadir)/atari/tools/README
	rm -f $(DESTDIR)$(pkgdatadir)/atari/tools/pc101us.kbd.bz2
	rm -f $(DESTDIR)$(pkgdatadir)/atari/sound/README
	rm -f $(DESTDIR)$(pkgdatadir)/wm_icon.bmp
	rm -f $(DESTDIR)$(pkgdatadir)/logo.bmp
	rm -f $(DESTDIR)$(pkgdatadir)/logo.png
	rm -f $(DESTDIR)$(pkgdatadir)/logo.png
	rm -f $(DESTDIR)$(datadir)/icons/hicolor/*/apps/aranym*.png
	rm -f $(DESTDIR)$(datadir)/applications/aranym*.desktop
	rm -f $(DESTDIR)$(datadir)/pixmaps/aranym*.png
	-rmdir $(DESTDIR)$(datadir)/icons/hicolor/*/apps
	-rmdir $(DESTDIR)$(datadir)/icons/hicolor/*
	-rmdir $(DESTDIR)$(datadir)/icons/hicolor
	-rmdir $(DESTDIR)$(datadir)/icons
	-rmdir $(DESTDIR)$(datadir)/pixmaps
	rmdir $(DESTDIR)$(pkgdatadir)/atari/hostfs
	rmdir $(DESTDIR)$(pkgdatadir)/atari/nfcdrom
	rmdir $(DESTDIR)$(pkgdatadir)/atari/nfjpeg
	rmdir $(DESTDIR)$(pkgdatadir)/atari/nfosmesa
	rmdir $(DESTDIR)$(pkgdatadir)/atari/network
	rmdir $(DESTDIR)$(pkgdatadir)/atari/tools
	rmdir $(DESTDIR)$(pkgdatadir)/atari/sound
	rmdir $(DESTDIR)$(pkgdatadir)/atari
	-rmdir $(DESTDIR)$(docdir)
	-rmdir $(DESTDIR)$(man1dir)

DISTCLEANFILES = config.cache config.log config.status config.h version_date.h appimage/appimagetool

# remove the excutables copied from the src/Makefile.
# previously, the executables where built in the top level
# directory, and some external build scripts (e.g. rpm) excpect
# them there. Remove this line once the scripts have been updated
CLEANFILES = aranym$(EXEEXT) aratapif$(EXEEXT)

mrproper: distclean
	rm -f $(top_srcdir)/aclocal.m4 $(builddir)/configure $(top_srcdir)/config.h.in
	rm -rf $(top_srcdir)/autom4te.cache

bindist: all
	rm -rf tmpdist
	$(MAKE) DESTDIR=tmpdist install
	chmod -R a+r tmpdist
	cd tmpdist && GZIP=--best tar chozf ../$(PACKAGE_TARNAME).tar.gz *
	rm -rf tmpdist

appimage::
	rm -rf tmpdist
	test "$(srcdir)" = "." || test -d appimage || mkdir appimage
	unset CDPATH && \
	mkdir tmpdist && \
	DESTDIR=`(cd tmpdist; pwd)` && \
	$(MAKE) DESTDIR="$$DESTDIR" install-strip && \
	cd appimage && \
	CPU_TYPE="$(CPU_TYPE)" \
	VERSION="$(VERSION)" \
	top_srcdir="$(abs_top_srcdir)" \
	BUILDROOT="$$DESTDIR" \
	$(SHELL) "$(abs_top_srcdir)/appimage/build.sh"
	rm -rf tmpdist
	
devdist: mrproper 
	cd $(srcdir)/../ && GZIP=--best tar chozf $(PACKAGE_TARNAME).tar.gz aranym

# not really needed anymore, but *.spec files might do a "make depend"
depend:

# For tests
EXTRA_DIST = \
	BUGS FAQ README.md README-cygwin.in \
	aranym.spec autogen.sh \
	android atari contrib data tools \
	extras/hostfs/hostfs.dos \
	extras/nfcdrom/nfcdrom.bos \
	extras/nfjpeg/nfjpeg.prg \
	appimage/build.sh \
	appimage/desktopintegration \
	appimage/excludelist \
	appimage/functions.sh
