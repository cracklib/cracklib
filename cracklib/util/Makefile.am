noinst_PROGRAMS = testcheck packer testlib testnum teststr unpacker

sbin_SCRIPTS = mkdict

LDADD = ../lib/libcrack.la
AM_CPPFLAGS = -I. -I.. -I$(top_srcdir)/lib 

testcheck_SOURCES = testcheck.c
testcheck_LDADD = $(LDADD)

packer_SOURCES = packer.c
packer_LDADD = $(LDADD)

testlib_SOURCES = testlib.c
testlib_LDADD = $(LDADD)

testnum_SOURCES = testnum.c
testnum_LDADD = $(LDADD)

teststr_SOURCES = teststr.c
teststr_LDADD = $(LDADD)

unpacker_SOURCES = unpacker.c
unpacker_LDADD = $(LDADD)

