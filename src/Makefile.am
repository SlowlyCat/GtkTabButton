## Process this file with automake to produce Makefile.in

## Created by Anjuta

#uidir = $(datadir)/androiddevsuit/ui
#ui_DATA = androiddevsuit.ui

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(ANDROIDDEVSUIT_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = androiddevsuit

androiddevsuit_SOURCES = \
	afsMain.c

androiddevsuit_LDFLAGS = 

androiddevsuit_LDADD = $(ANDROIDDEVSUIT_LIBS)

#EXTRA_DIST = $(ui_DATA)

# Remove ui directory on uninstall
uninstall-local:
#	-rm -r $(uidir)

