sbin_PROGRAMS = cracklib-packer cracklib-unpacker cracklib-check
sbin_SCRIPTS = cracklib-update

check_PROGRAMS = testlib testnum teststr make-bad-endian
if NOT_CROSS_COMPILING
check_DATA = testdict baddict
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

checkdict.sh:
	echo '! $(srcdir)/cracklib-format $(top_srcdir)/dicts/cracklib-small | $(builddir)/testlib $(builddir)/testdict | grep -c ": ok"' > "$@"
	chmod +x "$@"

both-endian.sh:
	echo '[ "$(echo 50000 |./testnum baddict)" = "$(echo 50000 |./testnum testdict)" ]' > "$@"
	chmod +x "$@"

baddict: $(top_srcdir)/dicts/cracklib-small $(builddir)/make-bad-endian
	$(srcdir)/cracklib-format "$<" | $(builddir)/make-bad-endian "$@"


if NOT_CROSS_COMPILING
CLEANFILES = testdict.pwi testdict.pwd testdict.hwm \
	baddict.pwi baddict.pwd baddict.hwm checkdict.sh both-endian.sh
endif


EXTRA_DIST = $(sbin_SCRIPTS) $(sbin_PROGRAMS)

