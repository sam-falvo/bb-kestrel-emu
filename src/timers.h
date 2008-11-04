#ifndef K2_TIMERS_H
#define K2_TIMERS_H

/*
 * Kestrel 2 Baseline Emulator
 * Release 1p1
 *
 * Copyright (c) 2006, 2007 Samuel A. Falvo II
 * See LICENSE for details.
 */

#include <lib65816/cpu.h>


typedef struct IO_TIMERS IO_TIMERS;


#define IOBASE_TIMERS   0xFF0300
#define IOMASK_TIMERS   0xFFFF00


struct IO_TIMERS
{
    word16  TMSB;       /* Timer controlling baud rate generation for SerBus */
};



int
timers_initialize( void );

void
timers_expunge( void );

byte
timers_read( word32 address, word32 timestamp );

void
timers_write( word32 address, byte b, word32 timestamp );

word16
timers_getTMSB( void );

#endif

