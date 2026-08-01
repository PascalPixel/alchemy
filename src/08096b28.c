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
 *   0x08096b5c  __call_via_r3  ->  the RETURN VALUE of the call at 0x8091750
 *     The callee is whatever that call produced, not a fixed address.
 */
#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_080072f0(s32, s32);
void Func_08015040(s32, s32);
s32 Func_080770c0(s32);
void Func_080916b0();
void Func_08091750();
void Func_08092b94(s32);
void Func_08092f84(s32, s32);

s32 Func_08096b28(void *arg0, s32 arg1, s32 arg2) {
    s32 resource;

    if (arg0 != NULL) {
        resource = M2C_FIELD(arg0, s32 *, 8);
        if (resource != 0) {
            if (resource < 0x10000) {
                Func_080916b0();
                Func_08092b94(M2C_FIELD(arg0, s32 *, 8));
                Func_08092f84(arg2, 0);
                Func_08091750();
            } else {
                Func_080072f0(arg1, arg2);
            }
        }
        if (Func_080770c0(0x142) != 0) {
            Func_080916b0();
            Func_08015040(0x927, 1);
            Func_08091750();
        }
    }
    return 0;
}
