all: cracklib-words.gz

cracklib-words.gz: cracklib-words
	rm -f cracklib-words.gz
	gzip -v9 -c cracklib-words > cracklib-words.gz

cracklib-words: src/clarkson-passwords src/words
	rm -f cracklib-words
	zcat -f src/* | tr A-Z a-z | sort | uniq > cracklib-words

upload: cracklib-words.gz
	cp cracklib-words.gz cracklib-words-`date '+%Y%m%d'`.gz
	curl --upload-file cracklib-words-`date '+%Y%m%d'`.gz ftp://upload.sf.net/incoming/
	