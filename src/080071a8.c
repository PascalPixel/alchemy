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
 *   0x080071fe  __call_via_r3  ->  loaded from memory [r0, #0]
 *     CONTEXT-DEPENDENT: a function-pointer table entry, struct field or
 *     stack slot. Must be read with the surrounding code; must NOT be
 *     pattern-matched against other files.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct Config_080071a8 {
    u8 pad_00[24];
    s32 size;
    u8 shift;
};

extern struct Config_080071a8 Data_08007c10;

s32 Func_080072f0(s32, u8 *, u32, s32);

u16 Func_080071a8(u32 slot, const u8 *source)
{
    const u8 *input;
    volatile u8 *destination;
    s32 *finish;
    u32 saved_ime;
    u32 last;
    s32 remaining;

    input = source;
    slot = (u16)slot;
    destination = (volatile u8 *)(
        0x0e000000 + (slot << Data_08007c10.shift));
    saved_ime = *(volatile u16 *)0x04000208;
    *(volatile u16 *)0x04000208 = 0;
    *(volatile u8 *)0x0e005555 = 0xaa;
    *(volatile u8 *)0x0e002aaa = 0x55;
    *(volatile u8 *)0x0e005555 = 0xa0;

    remaining = Data_08007c10.size;
    if (remaining != 0) {
        do {
            *destination++ = *input++;
            remaining--;
        } while (remaining != 0);
    }

    destination--;
    input--;
    *(volatile u16 *)0x04000208 = saved_ime;
    finish = (s32 *)0x02004c00;
    last = *input;
    return Func_080072f0(
        1,
        (u8 *)destination,
        last,
        *finish);
}
