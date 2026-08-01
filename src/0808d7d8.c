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
 *   0x0808d818  __call_via_r3  ->  the RETURN VALUE of the call at 0x8091750
 *     The callee is whatever that call produced, not a fixed address.
 */
#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_080072f0(s32);
void *Func_0808d48c(s32, s32);
s32 Func_080916b0();
s32 Func_08091750();
s32 Func_08092b94(s32);
s32 Func_08092f84(s32, s32);

s32 Func_0808d7d8(s32 arg0) {
    s32 temp_r3;
    s32 var_r6;
    void *temp_r0;

    temp_r0 = Func_0808d48c(6, arg0);
    var_r6 = -1;
    if (temp_r0 != NULL) {
        temp_r3 = M2C_FIELD(temp_r0, s32 *, 8);
        if (temp_r3 != 0) {
            if (temp_r3 < 0x10000) {
                Func_080916b0();
                Func_08092b94(M2C_FIELD(temp_r0, s32 *, 8));
                Func_08092f84(-1, 0);
                var_r6 = 0;
                Func_08091750();
            } else {
                Func_080072f0(arg0);
                var_r6 = 0;
            }
        }
    }
    return var_r6;
}
