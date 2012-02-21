.PHONY: configure
all: configure build

.PHONY: configure
configure:
	mkdir -p _build && cd _build && cmake -DCMAKE_BUILD_TYPE=debug ..

.PHONY: build
build:
	cd _build && make

.PHONY: run
run:
	@./_build/src/ekniga

.PHONY: clean
clean:
	rm -Rf _build
