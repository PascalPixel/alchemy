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
 *   0x08006f6e  __call_via_r1  ->  INCOMING r1, supplied by the CALLER
 *     r1 is never written in this function -- the `bl` is its SECOND
 *     instruction, right after `push {lr}`.  So this owner is a trampoline:
 *     it calls whatever function pointer the caller left in r1, returns 0 if
 *     that call yielded 0, and 0x8004 otherwise.
 *
 *     CONSEQUENCE, and it is not fixed here because this file is byte-exact:
 *     the declared signature `s32 Func_08006f6c(void)` is wrong.  This owner
 *     takes at least a function pointer in r1, and r0 is passed through to it
 *     untouched, so it very likely takes an argument there too.  Naming those
 *     parameters needs the call sites, which is a per-site read for the
 *     context-dependent pass -- flagged, not guessed.
 */
#include "types.h"

s32 Func_080072e8();

s32 Func_08006f6c(void) {
    if (Func_080072e8() == 0) {
        return 0;
    }
    return 0x8004;
}
