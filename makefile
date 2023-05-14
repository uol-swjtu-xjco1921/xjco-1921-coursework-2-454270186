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
	rm -rf build map.png shortest.png fastest.png location.png

test:
	gcc plplot.c -o plplot `pkg-config --cflags --libs gtk+-3.0`