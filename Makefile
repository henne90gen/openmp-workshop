all: cmake build

clean:
	rm -rf build

cmake:
	mkdir -p build && cd build && cmake -G"Unix Makefiles" ..

build:
	cd build && make

.PHONY: build cmake clean all
