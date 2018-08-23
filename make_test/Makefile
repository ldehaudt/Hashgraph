var-lost:
	export foo=bar
	echo "foo=[$$foo]"

var-kept:
	export foo=bar; \
	echo "foo=[$$foo]"

result.txt: source.txt
	@echo "building result.txt from source.txt"
	cp source.txt result.txt

source:
	@echo "building source.txt"
	echo "this is the source" > source.txt

result-using-var.txt: source.txt
	@echo "buildling result-using-var.txt using the $$< and $$@ vars"
	cp $< $@

srcfiles := $(shell echo src/{00..99}.txt)

src/%.txt:
	@[ -d src ] || mkdir src
	echo $* > $@

source: $(srcfiles)

dest/%.txt: src/%.txt
	@[ -d dest ] || mkdir dest
	cp $< $@

destfiles := $(patsubst src/%.txt,dest/%.txt,$(srcfiles))
destination: $(destfiles)