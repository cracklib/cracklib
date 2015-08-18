all: clean cracklib-words.gz cracklib-words.bz2

clean:
	rm -f cracklib-words cracklib-words.gz cracklib-words.bz2

cracklib-words.gz: cracklib-words
	rm -f cracklib-words.gz
	gzip -v9 -c cracklib-words > cracklib-words.gz

cracklib-words.bz2: cracklib-words
	rm -f cracklib-words.bz2
	bzip2 -v9 -c cracklib-words > cracklib-words.bz2

cracklib-words: files/*/*.txt.bz2
	rm -f cracklib-words
	bzcat -f `find files -type f` | tr A-Z a-z | sort | uniq > cracklib-words

upload: cracklib-words.gz
	cp cracklib-words.gz cracklib-words-`date '+%Y%m%d'`.gz
	curl --upload-file cracklib-words-`date '+%Y%m%d'`.gz ftp://upload.sf.net/incoming/
	