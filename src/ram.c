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

static byte *ram = 0;

/* The Kestrel 2 will use the Xess series of FPGA developer toolkits,
 * which comes standard with 32MB of SDRAM.  However, in the emulator,
 * the CPU can only see the first 16MB, because I have not yet
 * implemented the MMU for the CPU yet.
 */

#define MEMSIZE     (32*1048576L)

int
ram_initialize( void )
{
    ram = (byte *)( malloc( MEMSIZE ) );
    if( !ram ) goto no_memory;
    return 1;

no_memory:
    fprintf( stderr, "Not enough ram for RAM emulation\n" );
    return 0;
}

void
ram_expunge( void )
{
    if( ram )    free( ram );
    ram = 0;
}

byte
ram_read( word32 address, word32 unusedTimestamp )
{
    return (byte)( ram[ address ] );
}

void
ram_write( word32 address, byte b, word32 timestamp )
{
    ram[address] = b;
}

