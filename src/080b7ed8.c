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
 *   0x080b7ef0  __call_via_r3  ->  0x030002c0
 *     a relocated IWRAM routine. NOT established
 */
#include "types.h"

void Func_080049ac(void);
s32 Func_080770c0(s32);
s32 Func_080051e8(void *, void *);
s32 Func_080051d8(void *, void *);
void Func_080072f0(void *);

s32 Func_080b7ed8(void) {
    u8 *state = *(u8 **)0x03001E80;
    Func_080049ac();
    if (Func_080770c0(0x16B) != 0) {
        ((void (*)(void *))0x030002C0)((void *)0x080C2A7C);
        return Func_080051e8(state, state + 0xC);
    } else {
        return Func_080051d8(state, state + 0xC);
    }
}
