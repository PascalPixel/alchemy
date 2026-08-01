/*
 * VENEER AUDIT NOTE (mars, 2026-08-01) -- COMMENT ONLY, NO CODE CHANGE.
 *
 * This file is byte-exact, so nothing below is rewritten.  The note records
 * what the call sites actually are, so the next reader is not misled.
 *
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank: fifteen four-byte
 * `bx rN; nop` entries in register order r0..lr, ending at 0x08007320.  A
 * `bl` into that range is an INDIRECT CALL through the named register, not a
 * call to a function at the branch target.  The `Func_080072f*` prototype
 * this file declares is therefore a phantom, and the register load that
 * precedes each site -- which reads like dead code -- is the callee load.
 *
 * Why the file is still byte-identical while being wrong: a direct call to a
 * declared function at 0x080072f0 emits exactly the same `bl` the real
 * indirect call emits.  Converting to a function-pointer call would require
 * the compiler to choose the same register and therefore the same veneer
 * entry, which is an exact-lane question and is deliberately NOT attempted
 * here.
 *
 * Sites in this owner, resolved with tools/veneer_resolve.ts:
 *
 *   0x08006c12  __call_via_r3  ->  a computed value (see 0x8006bfc). CONTEXT-DEPENDENT.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern void Func_08006b84(u32 source, u8 *destination, s32 size);
extern s32 Func_080072f0(u8 *source, u8 *destination, s32 size, void *core);
extern u8 Data_08007abc[];

void Func_08006ba8(u16 sectorNum, u32 offset, u8 *destination, s32 size)
{
    u16 buffer[64];
    u16 *source;
    u16 *cursor;
    u16 count;
    void *core;
    u32 shifted;
    u32 address;

    *(volatile u16 *)0x04000204 = (*(volatile u16 *)0x04000204 & 0xFFFC) | 3;

    source = (u16 *)Func_08006b84;
    source = (u16 *)((u32)source ^ 1);
    cursor = buffer;
    count = ((u32)Func_08006ba8 - (u32)Func_08006b84) / 2;

    while (count != 0) {
        *cursor = *source;
        source++;
        cursor++;
        count--;
    }

    core = (u8 *)buffer + 1;
    shifted = sectorNum << Data_08007abc[28];
    address = offset + 0x0E000000;
    Func_080072f0((u8 *)(shifted + address), destination, size, core);
}
