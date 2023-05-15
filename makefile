PNG_FIlE = map.png shortest.png fastest.png location.png shortest_poi.png

.PHONY: all
all: build

.PHONY: build
build:
	mkdir -p build
	cd build && \
	cmake -DRUN_MODE=1 .. && \
	make

.PHONY: test
test:
	mkdir -p build
	cd build && \
	cmake -DRUN_MODE=0 .. && \
	make

.PHONY: run
run:
	./build/main

.PHONY: clean
clean:
	rm -rf build ${PNG_FIlE}