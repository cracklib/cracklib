VERSION=2.10.4

all: clean cracklib-words.gz cracklib-words.bz2 cracklib-words.xz

clean:
	rm -f cracklib-words cracklib-words*.gz cracklib-words*.bz2 cracklib-words*.xz

cracklib-words.gz: cracklib-words
	rm -f cracklib-words.gz
	gzip -v9 -c cracklib-words > cracklib-words.gz
	cp cracklib-words.gz cracklib-words-$(VERSION).gz

cracklib-words.bz2: cracklib-words
	rm -f cracklib-words.bz2
	bzip2 -v9 -c cracklib-words > cracklib-words.bz2
	cp cracklib-words.bz2 cracklib-words-$(VERSION).bz2

cracklib-words.xz: cracklib-words
	rm -f cracklib-words.xz
	xz -v9 -c cracklib-words > cracklib-words.xz
	cp cracklib-words.xz cracklib-words-$(VERSION).xz

cracklib-words: files/*/*.txt.bz2
	rm -f cracklib-words
	bzcat -f `find files -type f` | tr A-Z a-z | sort | uniq > cracklib-words
