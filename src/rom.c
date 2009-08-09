/*
 * Kestrel 2 Baseline Emulator
 * Release 1p1
 *
 * Copyright (c) 2006 Samuel A. Falvo II
 * All Rights Reserved
 */

#include <lib65816/cpu.h>
#include <stdio.h>
#include <stdlib.h>

#include "ram.h"

static byte *memory = 0;

/* The Kestrel 2 Baseline employs only 32KB of ROM to hold its kernel.
 * Real hardware will *likely* use the Xess board's Flash ROM to hold
 * larger software images, but the ROM will be bank switched in 16KB
 * chunks.  For now, however, ROM consumes a single 32K chunk of RAM.
 */

#define MEMSIZE     (32*1024)

int
rom_initialize( void )
{
    FILE *romfile;

    memory = (byte *)( malloc( MEMSIZE ) );
    if( memory == 0 ) goto no_memory;

    romfile = fopen( "ROMFILE", "rb" );
    if( romfile == 0 ) goto no_file;

    fread( memory, 1, MEMSIZE, romfile );
    fclose( romfile );
    return 1;

no_file:
    fprintf( stderr, "Error trying to read ROMFILE\n" );
    free( memory );

no_memory:
    fprintf( stderr, "Error initializing ROM\n" );
    return 0;
}

void
rom_expunge( void )
{
    if( memory )    free( memory );
    memory = 0;
}

byte
rom_read( word32 address, word32 unusedTimestamp )
{
    address &= 0x00007FFF;

    return (byte)( memory[ address ] );
}

void
rom_write( word32 address, byte b, word32 timestamp )
{
    ram_write( address, b, timestamp );        /* RAM write-through */
}

