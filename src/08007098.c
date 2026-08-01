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
 *   0x080070e8  __call_via_r3  ->  loaded from memory [r0, #0]
 *     CONTEXT-DEPENDENT: a function-pointer table entry, struct field or
 *     stack slot. Must be read with the surrounding code; must NOT be
 *     pattern-matched against other files.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern s32 Func_080072f0(s32 mode, u8 *destination, u8 value, s32 status);

u16 Func_08007098(u16 sector)
{
    u8 *info = (u8 *)0x08007C10;
    u8 *cursor;
    u16 saved;
    s32 count;
    u16 result;

    cursor = (u8 *)(0x0E000000 + (sector << info[28]));
    saved = *(volatile u16 *)0x04000208;
    *(volatile u16 *)0x04000208 = 0;
    *(volatile u8 *)0x0E005555 = 0xAA;
    *(volatile u8 *)0x0E002AAA = 0x55;
    *(volatile u8 *)0x0E005555 = 0xA0;
    count = *(s32 *)(info + 24);
    while (count != 0) {
        *cursor = 0xFF;
        cursor++;
        count--;
    }
    cursor--;
    *(volatile u16 *)0x04000208 = saved;
    result = Func_080072f0(1, cursor, 0xFF, *(s32 *)0x02004C00);
    if (result != 0) {
        result = (result & 0xFF00) | 2;
    }
    return result;
}
