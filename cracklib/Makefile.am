SUBDIRS = m4 lib util po doc python dicts

EXTRA_DIST = config.rpath mkinstalldirs  	\
		autogen.sh \
		cracklib.spec.in \
		README-DAWG \
		README-WORDS \
		README-LICENSE \
		test-data 

all-local:
	@echo
	@echo =======================================================
	@echo Be sure and obtain a large wordlist file and run
	@echo 'make install' and 'make dict' to generate the word 
	@echo list index file. See the README file for more details.
	@echo =======================================================
	@echo

test: all
	@echo
	@echo =======================================================
	@echo If you get an error when running 'make test' about a 
	@echo missing pw_dict.pwd file, that indicates that the word
	@echo list dictionary file has not been built. You need to
	@echo at least run 'make install' and 'make dict' to install
	@echo the dictionay.  See the README file for more details.
	@echo =======================================================
	@echo
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
