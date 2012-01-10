.PHONY: configure
all: configure build

.PHONY: configure
configure:
	mkdir -p _build && cd _build && cmake ..

.PHONY: build
build:
	cd _build && make

.PHONY: run
run:
	@./_build/src/ekniga

.PHONY: clean
clean:
	rm -Rf _build
