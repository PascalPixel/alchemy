/*
 * Correctness fix, veneer audit (2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320 -- so a `bl` into that
 * range is an indirect call through the named register, not a call to a
 * function at the branch target.  Resolved with tools/veneer_resolve.ts.
 *
 * Callee signatures here are established, not guessed: 0x03001388 is the
 * word copy declared in the EXACT src/080d40ec.c, and 0x03000168 is the fill
 * documented in semantic/main/080e15e8.c as (destination, size, value).
 *
 * This file already reached 0x03000164 correctly further down, through a
 * `transfer = (void (*)())0x03000164;` function pointer -- it had the right
 * idea for two of its four sites and the phantom prototype for the other
 * two. 0x03000164 itself is NOT established; typed by arity only.
 */
#include "types.h"

typedef void (*ArmFill)(void *destination, u32 size, u32 value);
typedef void (*Resident_03000164)(void *destination, u32 size, u32 value);

struct Globals {
    u8 *base;
    u8 pad04[0x78];
    void *buffer;
    u8 pad80[0x0c];
    void *target;
};

struct TransferState {
    u8 pad00[6];
    u16 control;
};

extern struct Globals Data_03001e74;

void Func_080cd508(u32);
void Func_080b5038(s32, u16, s32);
void Func_080030f8(u32);

void Func_080cdd58(u32 arg0)
{
    void *buffer = Data_03001e74.buffer;
    u8 *base = Data_03001e74.base;
    void *target = Data_03001e74.target;
    void (*transfer)();
    void *offset;
    s32 tile;
    s32 y;

    Func_080cd508(arg0);
    Func_080b5038(2, *(u16 *)(base + 0x648), 0);
    ((struct TransferState *)0x03001ad0)->control = 0x20;
    *(s32 *)((u8 *)target + 0x0c) = 0;
    ((Resident_03000164)0x03000164)((void *)0x06003fc0, 0x40, 0x03001ad0);
    ((ArmFill)0x03000168)((void *)0x0600f900, 0x200, (u32)-1);

    offset = 0;
    y = 0;
    tile = 0;
    do {
        s32 x = 0;
        s32 value = tile + 0x100;
        do {
            if (x > 15)
                *(volatile u16 *)((u8 *)offset + 0x0600fb00) = 0xff;
            else
                *(volatile u16 *)((u8 *)offset + 0x0600fb00) = value;
            x++;
            value++;
            offset = (u8 *)offset + 2;
        } while (x != 32);
        y++;
        tile += 16;
    } while (y != 16);

    transfer = (void (*)())0x03000164;
    *(volatile u16 *)0x04000000 = 0x7741;
    *(volatile u16 *)0x0400000a = 0x1f81;
    *(volatile u16 *)0x04000050 = 0x3f42;
    *(volatile u16 *)0x04000040 = 0x00f0;
    *(volatile u16 *)0x04000044 = 0x1088;
    *(volatile u16 *)0x04000042 = 0x00f0;
    *(volatile u16 *)0x04000046 = 0x1088;
    *(volatile u16 *)0x04000048 = 0x3537;
    *(volatile u16 *)0x0400004a = 0x3f21;
    *(volatile u16 *)0x04000052 = 0x100e;
    transfer(buffer, 0x4000);
    transfer((void *)0x06004000, 0x4000);
    Func_080030f8(1);
}
