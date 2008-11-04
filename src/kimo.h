#ifndef IO_KIMO_H
#define IO_KIMO_H

/*
 * Kestrel 2 Baseline Emulator
 * Release 1p1
 *
 * Copyright (c) 2006 Samuel A. Falvo II
 * All Rights Reserved
 *
 * Keyboard Input and MOuse controller
 */

#include <lib65816/cpu.h>

typedef struct IO_KIMO  IO_KIMO;

#define IOBASE_KIMO     0xFF0200
#define IOMASK_KIMO     0xFFFF00

struct IO_KIMO
{
    word16      kbda;
    word16      unused0;
    word16      unused1;
    word16      kbmost; /*
        Bit 15: Keycode received.
        Bit 14: 1=Key press; 0=Key release.
    */
};

int
kimo_initialize( void );

void
kimo_expunge( void );

byte
kimo_read( word32 address, word32 timestamp );

void
kimo_write( word32 address, byte b, word32 timestamp );

void
kimo_update( word32 timestamp );

#endif

