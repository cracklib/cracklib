SUBDIRS = lib util doc

EXTRA_DIST = 	\
		autogen.sh \
		cracklib.spec \
		README-DAWG \
		README-WORDS \
		test-data \
		dicts/cracklib-small


test: all
	util/cracklib-check < test-data

dict: all
	PATH="./util:$$PATH" ./util/create-cracklib-dict dicts/*
