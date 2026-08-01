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
 *   0x080c16da  __call_via_r3  ->  0x03000164
 *     a relocated IWRAM routine. NOT established -- reached with two
 *     arguments at some sites and three at others. Do not assume
 */
#include "types.h"

void Func_080072f0(u32, s32, s32, u32);
void Func_08002dd8(s32);
s32 Func_08004278(u32);

s32 Func_080c16d0(s32 unused0, s32 unused1, s32 mode)
{
    Func_080072f0(0x06004000, 0x4000, mode, 0x03000164);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_08002dd8(40);
    Func_08002dd8(39);
    *(u16 *)0x04000000 = 0x1341;
    Func_08004278(0x080c1439);
    return Func_08004278(0x080c11ed);
}
