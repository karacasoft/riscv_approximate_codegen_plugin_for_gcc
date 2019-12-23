CPPC=g++
CC=gcc

TARGET_CC=riscv32-unknown-linux-gnu-gcc

INCLUDE:=$(shell $(TARGET_CC) -print-file-name=plugin)/include
CPPFLAGS := -g -I$(INCLUDE) -fPIC -fno-rtti

HEADERS=ap_debug.h approx_pragma.h approx_pass.h approx_attribute.h
OBJ=main.o ap_debug.o approx_pragma.o approx_pass.o approx_attribute.o

OUT_LIB=approx.so

all: $(OUT_LIB)

$(OUT_LIB): $(OBJ) $(HEADERS)
	$(CPPC) -shared $(CPPFLAGS) $(OBJ) -o $@

%.o: %.c
	$(CPPC) $(CPPFLAGS) -c $^ -o $@

clean:
	rm -rf *.o $(OUT_LIB)