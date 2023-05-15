PNG_FIlE = map.png shortest.png fastest.png location.png shortest_poi.png

.PHONY: all
all: build

.PHONY: build
build:
	mkdir -p build
	cd build && \
	cmake .. && \
	make

.PHONY: run
run:
	./build/http_server

.PHONY: clean
clean:
	rm -rf build ${PNG_FIlE}

test:
	gcc plplot.c -o plplot `pkg-config --cflags --libs gtk+-3.0`