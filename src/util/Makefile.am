sbin_PROGRAMS = cracklib-packer cracklib-unpacker cracklib-check
sbin_SCRIPTS = cracklib-update

check_PROGRAMS = testlib testnum teststr 

dist_sbin_SCRIPTS = create-cracklib-dict cracklib-format

LDADD = ../lib/libcrack.la

AM_CPPFLAGS = -I. -I.. -I$(top_srcdir)/lib '-DDEFAULT_CRACKLIB_DICT="$(DEFAULT_CRACKLIB_DICT)"' '-DLOCALEDIR="$(localedir)"' -Wall

cracklib_check_SOURCES = check.c
cracklib_check_LDADD = $(LDADD) $(LTLIBINTL)

cracklib_packer_SOURCES = packer.c
cracklib_packer_LDADD = $(LDADD)

cracklib_unpacker_SOURCES = unpacker.c
cracklib_unpacker_LDADD = $(LDADD)

testlib_SOURCES = testlib.c
testlib_LDADD = $(LDADD)

testnum_SOURCES = testnum.c
testnum_LDADD = $(LDADD)

teststr_SOURCES = teststr.c
teststr_LDADD = $(LDADD)

EXTRA_DIST = $(sbin_SCRIPTS) $(sbin_PROGRAMS)

