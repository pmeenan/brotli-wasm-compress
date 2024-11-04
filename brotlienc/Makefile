CFLAGS = -Wall -c -DBROTLI_BUILDING_LIBRARY
AR = ar
ARFLAGS = rcs
LIB_NAME = libmylib.a
SRC_FILES = $(wildcard ../brotli/c/common/*.c) $(wildcard ../brotli/c/enc/*.c)
OBJ_FILES = $(SRC_FILES:.c=.o)
INC=-I../brotli/c/include

# Default target
all: brotlienc.a

brotlienc.a: $(OBJ_FILES)
	$(AR) $(ARFLAGS) $@ $^

# Rule to compile C files into object files
%.o: %.c
	$(CC) $(INC) $(CFLAGS) -o $@ $<

# Clean target
clean:
	rm -f $(OBJ_FILES) brotlienc