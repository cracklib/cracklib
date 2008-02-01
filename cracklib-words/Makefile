all: cracklib-words.gz

cracklib-words.gz: cracklib-words
	rm -f cracklib-words.gz
	gzip -v9 cracklib-words

cracklib-words:
	rm -f cracklib-words
	zcat -f src/* | tr A-Z a-z | sort | uniq > cracklib-words