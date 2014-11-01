# Fake Makefile, acturally calls build/Makefile

.PHONY: all clean

all:
	make -C build all

clean:
	make -C build clean

build/libwintf8.a:
	make -C build libwintf8.a
