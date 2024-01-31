default: build

CC=gcc

build: build-demo build-demo-1 build-cli

build-demo:
	@echo "Building..."
	$(CC) -o bin/demo demo.c  -I. -I./lib/libch9326_x64/ -L./lib/libch9326_x64 -lch9326

build-demo-1:
	@echo "Building..."
	$(CC) -o bin/demo1 demo1.c opm.c  -I. -I./lib/libch9326_x64/ -L./lib/libch9326_x64 -lch9326

build-cli:
	@echo "Building..."
	$(CC) -o bin/cli cli.c opm.c  -I. -I./lib/libch9326_x64/ -L./lib/libch9326_x64 -lch9326



clean:
	@echo "Cleaning..."
	rm -f main
