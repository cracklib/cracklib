lib_LTLIBRARIES = libcrack.la

include_HEADERS = crack.h

libcrack_la_SOURCES = 	fascist.c \
			packlib.c \
			rules.c \
			stringlib.c \
			packer.h \
			crack.h 

libcrack_la_LDFLAGS = -version-info 3:0:0

AM_CFLAGS = '-DDEFAULT_CRACKLIB_DICT="$(pkgdatadir)/pw_dict"'

AM_CPPFLAGS = -I. -I.. -I$(top_srcdir)/lib
