/*
 * VENEER AUDIT NOTE (2026-08-01) -- COMMENT ONLY, NO CODE CHANGE.
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
 * entry, which is a byte-exact source question and is deliberately NOT attempted
 * here.
 *
 * Sites in this owner, resolved with tools/veneer_resolve.ts:
 *
 *   0x08006ccc  __call_via_r3  ->  a computed value (see 0x8006cb8). CONTEXT-DEPENDENT.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

u8 *Func_08006c24(u8 *, u8 *, u16);
s32 Func_080072f0(s32, s32, s32, s32);

extern u8 Data_08007abc[];

s32 Func_08006c68(u16 sector, s32 arg1)
{
    u16 buffer[128];
    u16 *source;
    u16 *destination;
    u16 count;
    u32 wait;
    s32 core;
    u32 shifted;
    u32 address;

    wait = *(volatile u16 *)0x04000204;
    wait &= 0xFFFC;
    wait |= 3;
    *(volatile u16 *)0x04000204 = wait;

    source = (u16 *)Func_08006c24;
    source = (u16 *)((u32)source ^ 1);
    destination = buffer;
    count = ((u32)Func_08006c68 - (u32)Func_08006c24) / 2;

    while (count != 0) {
        *destination = *source;
        source++;
        destination++;
        count--;
    }

    core = (s32)((u8 *)buffer + 1);
    shifted = sector << Data_08007abc[28];
    address = shifted + 0x0E000000;
    return Func_080072f0(
        arg1,
        address,
        *(u16 *)(Data_08007abc + 24),
        core);
}
