objs := main.o mandelbrot.o
CC := gcc
LD := gcc
AS := gcc
CFLAGS := -m64 $(shell pkg-config --cflags sdl2)
LDFALGS := $(shell pkg-config --libs sdl2) -lm


all: program
	./program

program: $(objs)
	$(LD) $(objs) $(LDFALGS) -o program


%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$.o: %.S
	$(AS) -c $< -o $@ -fno-pie


clean:
	rm -rf $(objs) program