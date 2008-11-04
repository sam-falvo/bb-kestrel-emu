#ifndef K2_SERBUS_H
#define K2_SERBUS_H

/*
 * Kestrel 2 Baseline Emulator
 * Release 1p1
 *
 * Copyright (c) 2006, 2007 Samuel A. Falvo II
 * See LICENSE for details.
 */

#include <lib65816/cpu.h>

typedef struct IO_SERBUS        IO_SERBUS;
typedef struct SerBusPeripheral SerBusPeripheral;
typedef struct SerBusStorage    SerBusStorage;

typedef void SerBusExchangeByteProc(IO_SERBUS *, word32);
typedef void SerBusSelectProc(IO_SERBUS *, word32);


#define IOBASE_SERBUS   0xFF0400
#define IOMASK_SERBUS   0xFFFF00


struct SerBusPeripheral
{
    SerBusSelectProc       *select;
    SerBusSelectProc       *unselect;
    SerBusExchangeByteProc *exchangeByte;
};


struct SerBusStorage
{
    int address;
    int addressLength;
    int out;
    int spiCommand;
    int spiState;
};


struct IO_SERBUS
{
    word16  SBDATX;
    word16  SBDARX;
    word16  SBDS;
    word16  SBST;

    /* private state -- emulator specific */

    SerBusPeripheral peripheral[8];
    int currentPeripheral;

    SerBusStorage storage;
};


int
serbus_initialize( void );

void
serbus_expunge( void );

byte
serbus_read( word32 address, word32 timestamp );

void
serbus_write( word32 address, byte b, word32 timestamp );

void
serbus_exchangeBytes(word32 timestamp);

#endif

