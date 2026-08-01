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
 *   0x08004444  __call_via_r0  ->  loaded from memory [r6, #0]
 *     CONTEXT-DEPENDENT: a function-pointer table entry, struct field or
 *     stack slot. Must be read with the surrounding code; must NOT be
 *     pattern-matched against other files.
 */
#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
void Func_080072e4(s32);
extern u8 Data_03001d34;
extern u8 Data_03001a20[];
void Func_08004420(s32 arg0) {
    s32 key = arg0;
    u8 *p = Data_03001a20;
    s32 i;
    key = key >> 8;
    if (Data_03001d34 == 1) {
        i = 0x15;
        p -= 8;
loop:
        i -= 1;
        if (i != 0) {
            p += 8;
            if (p[5] == key) Func_080072e4(*(s32 *)p);
            goto loop;
        }
    }
}
