SUBDIRS = lib util doc

EXTRA_DIST = 	\
		autogen.sh \
		cracklib.spec \
		README-DAWG \
		README-WORDS \
		test-data

test:
	util/testcheck < test-data

dict:
	PATH="./util:$$PATH" ./util/create-cracklib-dict dicts/*
