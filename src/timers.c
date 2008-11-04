/*
 * Kestrel 2 Baseline Emulator
 * Release 1p1
 *
 * Copyright (c) 2006, 2007 Samuel A. Falvo II
 * All Rights Reserved
 *
 * Timers for I/O, multitasking support.
 */

#include <string.h>

#include <lib65816/cpuevent.h>
#include <lib65816/cpu.h>

#include "io.h"


static IO_TIMERS    timers;


int
timers_initialize( void )
{
    memset( &timers, 0, sizeof( IO_TIMERS ) );
    return 1;
}


void
timers_expunge( void )
{
}


byte
timers_read( word32 address, word32 timestamp )
{
    return 0;
}


void
timers_write( word32 address, byte b, word32 timestamp )
{
    switch( address & 0x001F )
    {
        case  0: LOBYTE(timers.TMSB) = b; break;
        case  1: HIBYTE(timers.TMSB) = b; break;
        default: return;
    }
}

word16
timers_getTMSB( void ) {
    return timers.TMSB;
}

