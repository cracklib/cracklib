lib_LTLIBRARIES = libcrack.la

include_HEADERS = crack.h packer.h

libcrack_la_SOURCES = 	fascist.c \
			packlib.c \
			rules.c \
			stringlib.c \
			packer.h \
			crack.h 

#
# For initial release, use 10:0:8 to get 2.8.0
# For next ABI changing release, use 3:0:0
# After that, follow the libtool recommended incrementing procedure
#
libcrack_la_LDFLAGS = -version-info 10:0:8

# Link in NLS libs. Needed by FreeBSD build
libcrack_la_LIBADD = $(LTLIBINTL)

AM_CPPFLAGS = -I. -I.. -I$(top_srcdir)/lib -DIN_CRACKLIB '-DDEFAULT_CRACKLIB_DICT="$(DEFAULT_CRACKLIB_DICT)"'
