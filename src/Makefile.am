SUBDIRS = m4 lib util po doc python dicts

EXTRA_DIST = \
		cracklib.spec.in \
		README-DAWG \
		README-WORDS \
		README-LICENSE \
		test-data 

AUTOMAKE_OPTIONS=dist-bzip2 dist-zip dist-xz

all-local:
	@echo
	@echo =======================================================
	@echo Be sure and obtain a large wordlist file and run
	@echo 'make install' and 'make dict' to generate the word 
	@echo list index file. See the README file for more details.
	@echo =======================================================
	@echo

test: check

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
