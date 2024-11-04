all:
	emcc -v
	AR=emar CC=emcc $(MAKE) -C brotlienc brotlienc.a MOREFLAGS="-Werror -O3 -msimd128"
	emcc --bind -I./brotli/c/include --pre-js './src/pre.js' src/brotli-wasm-compress.cpp -o ./bin/brotlienc.js brotlienc/brotlienc.a -s DYNAMIC_EXECUTION=0 -s TEXTDECODER=0 -s MODULARIZE=1 -s FILESYSTEM=0 -s ENVIRONMENT="web" -s EXPORTED_RUNTIME_METHODS='["cwrap", "setValue"]' -s EXPORTED_FUNCTIONS='["_malloc", "_free"]' -s ALLOW_MEMORY_GROWTH=1 -s EXPORT_NAME="brotlienc"
clean:
	$(MAKE) -C brotlienc clean
	rm -rf ./bin/*
