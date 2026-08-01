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
 *   0x08016276  __call_via_r3  ->  0x03000168
 *     the relocated IWRAM fill, documented in semantic/main/080e15e8.c
 *     as (destination, size, value)
 *   0x08016286  __call_via_r3  ->  0x03000168
 *     the relocated IWRAM fill, documented in semantic/main/080e15e8.c
 *     as (destination, size, value)
 */
#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_08017248(s32, s32, s32, s32, s32);
void Func_080170f8(s32, s32, s32, s32);
void Func_080072f0(s32, s32, s32, s32);

void Func_08016230(void *arg0) {
    u32 temp_r2;
    u32 temp_r3;
    u32 temp_r5;
    u32 temp_r6;
    u32 temp_r7;
    u32 temp_r8;
    s32 call_r0;
    s32 call_r3;
    void *temp_sl;

    /* 描画属性に従い転送方法を切り替える。 */
    temp_sl = *(void **)0x03001E8C;
    temp_r3 = M2C_FIELD(arg0, u16 *, 0xA);
    temp_r2 = M2C_FIELD(arg0, u16 *, 0x16);
    temp_r8 = temp_r3;
    temp_r3 = 0;
    M2C_FIELD(arg0, s16 *, 0x1A) = temp_r3;
    temp_r5 = M2C_FIELD(arg0, u16 *, 0xC);
    temp_r6 = M2C_FIELD(arg0, u16 *, 0xE);
    temp_r7 = M2C_FIELD(arg0, u16 *, 8);
    if (8 & temp_r2) {
        if (0x20 & temp_r2) {
            Func_080170f8(temp_r5, temp_r6, temp_r7, temp_r8);
            call_r3 = 0x03000168;
            call_r0 = 0x06002500;
            Func_080072f0(call_r0, 0xF00, 0x44444444, call_r3);
        } else {
            call_r3 = 0x03000168;
            call_r0 = 0x06002500;
            Func_080072f0(call_r0, 0xF00, 0, call_r3);
        }
        Func_08017248(temp_r5, temp_r6, temp_r7, temp_r8, 0);
    } else {
        Func_080170f8(temp_r5, temp_r6, temp_r7, temp_r8);
    }
    M2C_FIELD(temp_sl, s8 *, 0xEA3) = 1;
}
