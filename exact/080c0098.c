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
 *   0x080c00c4  __call_via_r3  ->  0x03000168
 *     the relocated IWRAM fill, documented in semantic/main/080e15e8.c
 *     as (destination, size, value)
 */
#include "types.h"

void Func_080072f0(s32 *, s32, s32, s32);

void Func_080c0098(s32 *destination)
{
    s32 value;
    s32 count;
    s32 target;

    value = 0x03020100;
    count = 0;
    do {
        count += 1;
        *destination++ = value;
        value += 0x04040404;
    } while ((u32)count <= 0x3F);

    value = 0x03020100;
    count = 0;
    do {
        count += 1;
        *destination++ = value;
        value += 0x04040404;
    } while ((u32)count <= 0x37);

    target = 0x03000168;
    Func_080072f0(destination, 0x220, -1, target);
}
