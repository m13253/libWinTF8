# Fake Makefile, acturally calls build/Makefile

.PHONY: all

all:
	make -C build all

build/libwintf8.a:
	make -C build libwintf8.a
