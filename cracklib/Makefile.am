SUBDIRS = m4 lib util po doc python

EXTRA_DIST = mkinstalldirs  	\
		autogen.sh \
		cracklib.spec.in \
		README-DAWG \
		README-WORDS \
		test-data \
		dicts/cracklib-small


test: all
	util/cracklib-check < test-data

dict: all
	PATH="./util:$$PATH" ./util/create-cracklib-dict dicts/*

ACLOCAL_AMFLAGS = -I m4
