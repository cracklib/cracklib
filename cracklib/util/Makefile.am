sbin_PROGRAMS = cracklib-packer cracklib-unpacker cracklib-check

noinst_PROGRAMS = cracklib-packer testlib testnum teststr unpacker

sbin_SCRIPTS = create-cracklib-dict cracklib-format

LDADD = ../lib/libcrack.la
AM_CPPFLAGS = -I. -I.. -I$(top_srcdir)/lib 

cracklib-check_SOURCES = cracklib-check.c
cracklib-check_LDADD = $(LDADD)

cracklib-packer_SOURCES = packer.c
cracklib-packer_LDADD = $(LDADD)

cracklib-unpacker_SOURCES = unpacker.c
cracklib-unpacker_LDADD = $(LDADD)

testlib_SOURCES = testlib.c
testlib_LDADD = $(LDADD)

testnum_SOURCES = testnum.c
testnum_LDADD = $(LDADD)

teststr_SOURCES = teststr.c
teststr_LDADD = $(LDADD)

