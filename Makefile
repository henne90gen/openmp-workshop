cmake:
	mkdir build && cd build && cmake -G"Ninja" ..

build:
	cd build && ninja

e_00: build
	cd build && ./example_00

.PHONY: build
