/*
 * Kestrel 2 Baseline Emulator
 * Release 1p1
 *
 * Copyright (c) 2006, 2007 Samuel A. Falvo II
 * All Rights Reserved
 *
 * Timers for I/O, multitasking support.
 */

#include <stdio.h>
#include <string.h>

#include <lib65816/cpuevent.h>
#include <lib65816/cpu.h>

#include "io.h"


static IO_SERBUS        serbus;
static CPUEvent         event;


void
D0_exchangeByte(IO_SERBUS *bus, word32 timestamp)
{
    fprintf(stderr, ">> SerBus Device 0 got $%02X\n", bus -> SBDATX);
    bus -> SBDARX = 0;
}

void
D0_select(IO_SERBUS *bus, word32 timestamp)
{
    fprintf(stderr, "++ SerBus Device 0 selected\n");
}

void
D0_unselect(IO_SERBUS *bus, word32 timestamp)
{
    fprintf(stderr, "-- SerBus Device 0 unselected\n");
}


void D1_select(IO_SERBUS *io, word32 timestamp) {
    io -> storage.address = 0;
}

void D1_unselect(IO_SERBUS *io, word32 timestamp) {
    io -> storage.spiState = 0;
}

#define READING      ((io -> storage.spiCommand & 1) == 1)
#define WRITING      ((io -> storage.spiCommand & 1) == 0)
#define INCREMENTING ((io -> storage.spiCommand & 2) == 0)

static char diskBuffer[1024];
static int diskBlock;
static int command;
extern FILE *hardfile;

void SPI_write(int addr, int v) {
    v &= 0xFF;

    if((addr >= 0x0400) && (addr <= 0x07FF))
        diskBuffer[addr & 0x3FF] = v;

    else if(addr == 0xFC)
        diskBlock = (diskBlock & 0xFF00) | v;

    else if(addr == 0xFD)
        diskBlock = (diskBlock & 0x00FF) | (v << 8);

    else if(addr == 0xFE) {
        switch(v) {
            case 1:
                if(hardfile) {
                    fseek(hardfile, diskBlock * 1024, SEEK_SET);
                    fwrite(diskBuffer, 1024, 1, hardfile);
                }
                command = 0;
                break;

            case 2:
                if(hardfile) {
                    fseek(hardfile, diskBlock * 1024, SEEK_SET);
                    fread(diskBuffer, 1024, 1, hardfile);
                }
                command = 0;
                break;

            default:
                command = v;
        }
    }
}

int SPI_read(int addr) {
    if((addr >= 0x0400) && (addr <= 0x7FF))
        return (int)(diskBuffer[addr & 0x3FF]);

    else if(addr == 0xFC)
        return (diskBlock & 0xFF);

    else if(addr == 0xFD)
        return (diskBlock & 0xFF00) >> 8;

    else if(addr == 0xFE)
        return command;

    else if(addr == 0xFF)
        return 0;

    return 0xFF;
}

void D1_exchangeByte(IO_SERBUS *io, word32 timestamp) {
    switch(io -> storage.spiState) {
        case 0:
            printf( "WTF: %02X\n", io -> SBDATX);
            io -> storage.addressLength = (io -> SBDATX & 0x0F);
            printf("## Received address length: %d\n", io -> storage.addressLength);

            if(io -> storage.addressLength > 2)
                printf("#### Bug in 65816 OS: Storage device address too big!  %d bytes!\n", io -> storage.addressLength);

            io -> storage.spiCommand = ((io -> SBDATX >> 4) & 0x0F);
            printf("## Received transfer command: %d\n", io -> storage.spiCommand);

            io -> storage.spiState++;
            break;

        case 1:
            io -> storage.address = io -> SBDATX;
            io -> storage.addressLength--;
            printf("## Address = $00%02X\n", io -> storage.address);
            if(io -> storage.addressLength)  io -> storage.spiState++;
            else                             io -> storage.spiState = -1;
            break;

        case 2:
            io -> storage.address |= (io -> SBDATX << 8);
            io -> storage.spiState = -1;
            printf("## Address = $%04X\n", io -> storage.address);
            break;

        case -1:
            if(READING)      io -> SBDARX = SPI_read(io -> storage.address);
            else if(WRITING) SPI_write(io -> storage.address, io -> SBDATX);
            if(INCREMENTING) io -> storage.address++;
            break;
    }
}


int
serbus_initialize( void )
{
    memset( &serbus, 0, sizeof( IO_SERBUS ) );
    serbus.SBST = 0x8000;

    serbus.peripheral[0].select = &D0_select;
    serbus.peripheral[0].unselect = &D0_unselect;
    serbus.peripheral[0].exchangeByte = &D0_exchangeByte;

    serbus.peripheral[1].select = &D1_select;
    serbus.peripheral[1].unselect = &D1_unselect;
    serbus.peripheral[1].exchangeByte = &D1_exchangeByte;

    serbus.peripheral[2].select = &D0_select;
    serbus.peripheral[2].unselect = &D0_unselect;
    serbus.peripheral[2].exchangeByte = &D0_exchangeByte;

    serbus.peripheral[3].select = &D0_select;
    serbus.peripheral[3].unselect = &D0_unselect;
    serbus.peripheral[3].exchangeByte = &D0_exchangeByte;

    serbus.peripheral[4].select = &D0_select;
    serbus.peripheral[4].unselect = &D0_unselect;
    serbus.peripheral[4].exchangeByte = &D0_exchangeByte;

    serbus.peripheral[5].select = &D0_select;
    serbus.peripheral[5].unselect = &D0_unselect;
    serbus.peripheral[5].exchangeByte = &D0_exchangeByte;

    serbus.peripheral[6].select = &D0_select;
    serbus.peripheral[6].unselect = &D0_unselect;
    serbus.peripheral[6].exchangeByte = &D0_exchangeByte;

    serbus.peripheral[7].select = &D0_select;
    serbus.peripheral[7].unselect = &D0_unselect;
    serbus.peripheral[7].exchangeByte = &D0_exchangeByte;

    return 1;
}


void
serbus_expunge( void )
{
}


byte
serbus_read( word32 address, word32 timestamp )
{
    switch(address & 0x0F)
    {
        case 0: return LOBYTE(serbus.SBDATX);
        case 1: return HIBYTE(serbus.SBDARX);  /* Just to mess with your heads; don't read a write-only register! */
        case 2: return LOBYTE(serbus.SBDARX);
        case 3: return LOBYTE(serbus.SBDARX);  /* Low byte = high byte */
        case 4: return LOBYTE(serbus.SBDS);
        case 5: return HIBYTE(serbus.SBDS);
        case 6: return LOBYTE(serbus.SBST);
        case 7: return HIBYTE(serbus.SBST);
        default: return serbus_read(address % 6, timestamp);
    }
}


void
serbus_write( word32 address, byte b, word32 timestamp )
{
    int newDeviceID;

    switch( address & 0x0F )
    {
        case 0:
            LOBYTE(serbus.SBDATX) = b;
            HIBYTE(serbus.SBST) = 0;
            CPUEvent_schedule(&event, 8*(1+timers_getTMSB()), &serbus_exchangeBytes);
            break;

        case 1: return;  /* high byte is ignored */
        case 2: return;  /* Writes to read-only register ignored */
        case 3: return;
        case 4:
            LOBYTE(serbus.SBDS) = b;
            newDeviceID = b & 7;

            if( newDeviceID != serbus.currentPeripheral )
            {
                serbus.peripheral[serbus.currentPeripheral].unselect(&serbus, timestamp);
                serbus.peripheral[newDeviceID].select(&serbus, timestamp);
            }

            serbus.currentPeripheral = newDeviceID;
            return;

        case 5: HIBYTE(serbus.SBDS) = b; return;
        case 6: return;
        case 7: return;
        default: return;
    }
}

void
serbus_exchangeBytes(word32 timestamp) {
    serbus.peripheral[serbus.currentPeripheral].exchangeByte(&serbus, timestamp);
    HIBYTE(serbus.SBST) = 0x80;
}


