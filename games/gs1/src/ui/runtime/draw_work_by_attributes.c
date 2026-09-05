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
 *   0x08016276  __call_via_r3  ->  0x03000168
 *     the relocated IWRAM fill, documented in games/gs1/semantic/main/080e15e8.c
 *     as (destination, size, value)
 *   0x08016286  __call_via_r3  ->  0x03000168
 *     the relocated IWRAM fill, documented in games/gs1/semantic/main/080e15e8.c
 *     as (destination, size, value)
 */
#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_08017248(s32, s32, s32, s32, s32);
void Func_080170f8(s32, s32, s32, s32);
void Func_080072f0(s32, s32, s32, s32);

void UiWork_DrawByAttributes(void *arg0) {
    u32 attr;
    u32 tmp;
    u32 v0;
    u32 v1;
    u32 v2;
    u32 v3;
    s32 dst;
    s32 fn;
    void *work;

    /* 描画属性に従い転送方法を切り替える。 */
    work = *(void **)ADDR_03001E8C;
    tmp = FIELD_AT_OFFSET(arg0, u16 *, 0xA);
    attr = FIELD_AT_OFFSET(arg0, u16 *, 0x16);
    v3 = tmp;
    tmp = 0;
    FIELD_AT_OFFSET(arg0, s16 *, 0x1A) = tmp;
    v0 = FIELD_AT_OFFSET(arg0, u16 *, 0xC);
    v1 = FIELD_AT_OFFSET(arg0, u16 *, 0xE);
    v2 = FIELD_AT_OFFSET(arg0, u16 *, 8);
    if (8 & attr) {
        if (0x20 & attr) {
            Func_080170f8(v0, v1, v2, v3);
            fn = 0x03000168;
            dst = 0x06002500;
            Func_080072f0(dst, 0xF00, 0x44444444, fn);
        } else {
            fn = 0x03000168;
            dst = 0x06002500;
            Func_080072f0(dst, 0xF00, 0, fn);
        }
        Func_08017248(v0, v1, v2, v3, 0);
    } else {
        Func_080170f8(v0, v1, v2, v3);
    }
    FIELD_AT_OFFSET(work, s8 *, RENDER_DIRTY_OFS) = 1;
}
