shell:
	rm -rf ./build/*	
	emcc ./src/*.c --embed-file ./shaders -s MIN_WEBGL_VERSION=2 -Wno-incompatible-pointer-types -Wno-static-in-inline -s USE_SDL=2 --shell-file ./env/index.html -O3 -s FULL_ES2=1 -s WASM=1 -s ASYNCIFY=1 -o ./build/index.html
