/*
 * Retire the two halfwords at the cartridge-interface cursor, mask its
 * interrupt, and install the tiny relocated copy loop that follows it.
 * Clean-room reconstruction from asm/08006a78.s only.
 */
#include "types.h"

void Func_08006a78(void)
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

u8 Func_08006abc(u8 *value)
{
    return *value;
}

void Func_08006ac0(u8 *destination)
{
    u8 *source;
    u32 count;
    u32 begin;
    u16 value;

    *(u8 **)0x02004c1c = destination + 1;
    source = (u8 *)Func_08006abc;
    source = (u8 *)((u32)source ^ (u32)1);
    begin = ((u32)(u8 *)Func_08006ac0 -
             (u32)(u8 *)Func_08006abc) << 15;
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
