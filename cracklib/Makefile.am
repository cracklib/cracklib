SUBDIRS = m4 lib util po doc python

EXTRA_DIST = config.rpath mkinstalldirs  	\
		autogen.sh \
		cracklib.spec.in \
		README-DAWG \
		README-WORDS \
		test-data \
		dicts/cracklib-small


test: all
	util/cracklib-check < test-data

dict: all
	if test "x$(CROSS_COMPILING)" = "xno" ; then \
		export PATH="$(top_builddir)/util:$$PATH" ; \
		export LD_LIBRARY_PATH="$(top_builddir)/lib/.libs:$$LD_LIBRARY_PATH" ; \
	fi ; \
	create-cracklib-dict "$(srcdir)"/dicts/*

dict-local: all
	if test "x$(CROSS_COMPILING)" = "xno" ; then \
		export PATH="$(top_builddir)/util:$$PATH" ; \
		export LD_LIBRARY_PATH="$(top_builddir)/lib/.libs:$$LD_LIBRARY_PATH" ; \
	fi ; \
	cracklib-format "$(srcdir)"/dicts/* | cracklib-packer $(DESTDIR)/$(DEFAULT_CRACKLIB_DICT)

ACLOCAL_AMFLAGS = -I m4
