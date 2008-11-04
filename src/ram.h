#ifndef K2_RAM_H
#define K2_RAM_H

/*
 * Kestrel 2 Baseline Emulator
 * Release 1p1
 *
 * Copyright (c) 2006 Samuel A. Falvo II
 * See LICENSE for details.
 */

#include <lib65816/cpu.h>

/* The Kestrel 2 will use the Xess series of FPGA developer toolkits,
 * which comes standard with 32MB of SDRAM.  However, in the emulator,
 * the CPU can only see the first 16MB, because I have not yet
 * implemented the MMU for the CPU yet.
 */

#define MEMSIZE     (32*1048576L)

int
ram_initialize( void );

void
ram_expunge( void );

byte
ram_read( word32 address, word32 unusedTimestamp );

void
ram_write( word32 address, byte b, word32 timestamp );

#endif

