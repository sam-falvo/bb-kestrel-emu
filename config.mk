CC      := gcc
CCFLAGS := -c -O2 -fomit-frame-pointer -Wall
#CCFLAGS := -g -c
LD      := gcc
LDFLAGS :=
#LDFLAGS := -g

PREFIX  := /Programs/Kestrel/1
BINPATH := $(PREFIX)/bin
LIBPATH := $(PREFIX)/lib
INCPATH := $(PREFIX)/include

DELETE  := rm -rf

