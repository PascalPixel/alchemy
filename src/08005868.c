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
 *   0x0800587e  __call_via_r3  ->  loaded from memory [r2, #0]
 *     CONTEXT-DEPENDENT: a function-pointer table entry, struct field or
 *     stack slot. Must be read with the surrounding code; must NOT be
 *     pattern-matched against other files.
 */
#include "types.h"

s32 Func_080072f0(s32, s32, s32, s32);
s32 Func_08006c68(u16, s32);

struct Work_08005868 {
    u8 unknown_00[64];
    s32 data;
};

u32 Func_08005868(u16 code)
{
    s32 *param = (s32 *)0x02004C04;
    s32 result;
    struct Work_08005868 *work;
    s32 value;

    work = *(struct Work_08005868 **)0x03001F1C;
    value = code & 0xFFFF;
    if ((Func_080072f0(value, (s32)&work->data,
                       (s32)param, *param) << 0x10) != 0) {
        return 1U;
    }
    result = Func_08006c68(value, (s32)&work->data);
    return (u32)((0 - result) | result) >> 0x1F;
}
