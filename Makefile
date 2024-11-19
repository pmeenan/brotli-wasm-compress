CPP = emcc
CPPFLAGS = -O3 -msimd128 -s DYNAMIC_EXECUTION=0 -s ALLOW_MEMORY_GROWTH=1 -s TEXTDECODER=0 -s MODULARIZE=1 -s FILESYSTEM=0 -s ENVIRONMENT="web" -s NO_FILESYSTEM=1 --pre-js './src/pre.js'
COMMONDIR = brotli/c/common
ENCDIR = brotli/c/enc
ENCSRC = src/brotli-wasm-compress.cpp $(wildcard $(COMMONDIR)/*.c) $(wildcard $(ENCDIR)/*.c)
ENCOBJ = $(ENCSRC:.c=.o)

all: bin/brotlienc.js

.c.o .cc.o:
	$(CPP) -I brotli/c/include/ -c $< -o $@

bin/brotlienc.js: src/pre.js $(ENCOBJ)
	$(CPP) $(CPPFLAGS) --bind -I brotli/c/include/ -s EXPORTED_RUNTIME_METHODS='["cwrap", "setValue"]' -s EXPORTED_FUNCTIONS='["_malloc", "_free"]' -s EXPORT_NAME="brotlienc" $(ENCOBJ) -o bin/brotlienc.js

clean:
	rm -rf ./bin/*
