#Makefile for kprobe.so

CC = gcc
CFLAGS = -O2 -Wall -fmessage-length=0 -fpic
LDLIBS = -lkonoha

target = linux_32
pkgname = mod

library = "$(pkgname)_$(target).so"

.PHONY: all
all: $(library)

objs = $(pkgname).o

"$(pkgname).o": $(pkgname).c
	$(CC) $(CFLAGS) -o $@ -c $^

$(library): $(objs)
	$(CC) -shared -Wl -o $@ $^ $(LDLIBS)

.PHONY: clean
clean:
	$(RM) -rf $(objs) $(library)

.PHONY: install
install: $(library)
	mkdir -p .konoha/mod
	cp $(library) .konoha/mod
	cp mod.k .konoha/mod