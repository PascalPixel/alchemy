/*
 * Retire the two halfwords at the cartridge-interface cursor, mask its
 * interrupt, and install the tiny relocated copy loop that follows it.
 * Clean-room reconstruction from games/gs1/asm/08006a78.s only.
 */
#include "flash.h"

void StopFlashTimer(void)
{
    u16 * volatile *cursor = (u16 * volatile *)0x02004c28;
    u16 *record = *cursor;

    *record = 0;
    record++;
    *cursor = record;
    *record = 0;
    record--;
    *cursor = record;

    *(volatile u16 *)0x04000208 = 0;
    *(volatile u16 *)0x04000200 &= ~(u16)(8 << *(volatile u8 *)0x02004c20);
    *(volatile u16 *)0x04000208 = *(volatile u16 *)0x02004c2c;
}

u8 ReadFlashByte(u8 *value)
{
    return *value;
}

void CopyFlashReadRoutineToRam(void *rawDestination)
{
    u8 *destination = rawDestination;
    u8 *source;
    u32 count;
    u32 begin;
    u16 value;

    *(u8 **)0x02004c1c = destination + 1;
    source = (u8 *)ReadFlashByte;
    source = (u8 *)((u32)source ^ (u32)1);
    begin = ((u32)(u8 *)CopyFlashReadRoutineToRam -
             (u32)(u8 *)ReadFlashByte) << 15;
    count = begin;
    goto check;

loop:
    *(u16 *)destination = *(u16 *)source;
    source += 2;
    destination += 2;
    count = (((count >> 16) - 1) << 16);
check:
    value = count >> 16;
    if (value != 0)
        goto loop;
}
