sbin_PROGRAMS = cracklib-packer cracklib-unpacker cracklib-check
sbin_SCRIPTS = cracklib-update

check_PROGRAMS = testlib testnum teststr make-bad-endian
if NOT_CROSS_COMPILING
check_DATA = testdict testdict-gz testdict-bz2 testdict-xz baddict
check_SCRIPTS = both-endian.sh checkdict.sh
TESTS = $(check_SCRIPTS)
endif

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

make_bad_endian_SOURCES = packer.c ../lib/packlib.c
make_bad_endian_CPPFLAGS = -I. -I.. -I$(top_srcdir)/lib '-DDEFAULT_CRACKLIB_DICT="deleteme"' '-DLOCALEDIR="$(localedir)"' -DIN_CRACKLIB -Wall '-DSTORE_ORDER=STORE_ORDER_BE'

testdict: $(top_srcdir)/dicts/cracklib-small
	$(srcdir)/cracklib-format "$<" | $(builddir)/cracklib-packer "$@"

testdict-gz: testdict
	gzip -v9 -c $(top_srcdir)/dicts/cracklib-small > cracklib-small.gz
	$(srcdir)/cracklib-format cracklib-small.gz | $(builddir)/cracklib-packer "$@"
	cmp testdict.pwi $@.pwi

testdict-bz2: testdict
	bzip2 -v9 -c $(top_srcdir)/dicts/cracklib-small > cracklib-small.bz2
	$(srcdir)/cracklib-format cracklib-small.bz2 | $(builddir)/cracklib-packer "$@"
	cmp testdict.pwi $@.pwi

testdict-xz: testdict
	xz -v9 -c $(top_srcdir)/dicts/cracklib-small > cracklib-small.xz
	$(srcdir)/cracklib-format cracklib-small.xz | $(builddir)/cracklib-packer "$@"
	cmp testdict.pwi $@.pwi

checkdict.sh:
	echo '! $(srcdir)/cracklib-format $(top_srcdir)/dicts/cracklib-small | $(builddir)/testlib $(builddir)/testdict | grep -c ": ok"' > "$@"
	chmod +x "$@"

both-endian.sh:
	echo '[ "$(echo 50000 |./testnum baddict)" = "$(echo 50000 |./testnum testdict)" ]' > "$@"
	chmod +x "$@"

baddict: $(top_srcdir)/dicts/cracklib-small $(builddir)/make-bad-endian
	$(srcdir)/cracklib-format "$<" | $(builddir)/make-bad-endian "$@"


if NOT_CROSS_COMPILING
CLEANFILES = \
	testdict.pwi testdict.pwd testdict.hwm \
	testdict-gz.pwi testdict-gz.pwd testdict-gz.hwm \
	testdict-bz2.pwi testdict-bz2.pwd testdict-bz2.hwm \
	testdict-xz.pwi testdict-xz.pwd testdict-xz.hwm \
	baddict.pwi baddict.pwd baddict.hwm \
	cracklib-small.gz cracklib-small.bz2 cracklib-small.xz \
	checkdict.sh both-endian.sh
endif


EXTRA_DIST = $(sbin_SCRIPTS) $(sbin_PROGRAMS)

