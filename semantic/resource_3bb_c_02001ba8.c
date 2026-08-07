#include "types.h"

/*
 * resource_3bb owner at 0x02001ba8, 208 bytes: run this overlay's
 * six-call "handler family, incrementing id" idiom
 * (resource_3bb_c_02001fb8.c/02001b30.c already document it) with ids
 * 0/1/2/3/5/arg0, record `arg0` into a fixed cell, re-fetch a second
 * object, copy two of its halfword fields forward, then compute and
 * clamp two derived halfwords with a matching "already nonzero from the
 * other axis" fallback.
 *
 * Complete owner: `push {r5, lr}` at 0x02001ba8 through `pop {r5} / pop
 * {r0} / bx r0` at 0x02001c72-0x02001c76; an inline literal pool sits
 * mid-body (0x02001c1a-0x02001c27, reached only by the forward branch at
 * 0x02001c18 -- the same inline-pool shape
 * resource_39c_c_02002844.c/02002e08.c document), so there is no
 * trailing pool; the next owner's prologue is at 0x02001c78. One
 * argument (r0), void.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/lib/overlay_call_targets.ts`'s `+2` rule.
 *
 * `Clamp0to16384()` below is the repeated
 * `r0 > 16384 ? 16384 : (r0 < 0 ? 0 : r0)` idiom read twice
 * (0x02001c08-0x02001c18 and 0x02001c4a-0x02001c5a).
 *
 * Uncertainty: none of the nine callees are identified beyond call
 * shape; the fixed cell at 0x02000434 is the same address
 * resource_39c_c_02005900.c's overlay reads as its "current slot" index
 * cell, but resource_3bb is a different overlay entirely, so this is
 * recorded as the same address, not asserted to be the same concept.
 * `object`'s fields (0x34/0x36 copied to 0x38/0x3a, byte 0x131 cleared,
 * 0x14/0x16 the clamped outputs, 0x36/0x38/0x3a read back as the
 * fallback inputs) are inferred only from this call shape.
 */

extern void Func_08077168(s32 arg0);
extern void Func_08077150(s32 arg0);
extern void Func_0808a200(s32 arg0, s32 arg1);
extern u8 *Func_08077008(s32 arg0);
extern s32 Func_03000380(s32 arg0, s32 arg1);
extern void Func_0808a548(void);

static s32 Clamp0to16384(s32 raw)
{
    if (raw > 16384) {
        return 16384;
    }
    if (raw < 0) {
        return 0;
    }
    return raw;
}

void Func_02001ba8(s32 arg0)
{
    u8 *object;
    s32 clamped;

    s32 permuted_19;
    s32 permuted_16;
    s32 permuted_27;
    Func_08077168(0);
    Func_08077168(1);
    Func_08077168(2);
    Func_08077168(3);
    Func_08077168(5);
    Func_08077150(arg0);

    *(s32 *)0x02000434 = arg0;

    Func_0808a200(arg0, 0);
    object = Func_08077008(arg0);

    permuted_16 = *(u16 *)(object + 0x34);
    *(u16 *)(object + 0x3a) = *(u16 *)(object + 0x36);
    object[0x131] = 0;
    *(u16 *)(object + 0x38) = permuted_16;

    if (clamped == 0 && *(short *)(object + 0x38) != 0) {
        *(u16 *)(object + 20) = 1;
    }
    permuted_19 = Clamp0to16384(Func_03000380(*(short *)(object + 0x38) << 14, *(short *)(object + 0x34)));
    clamped  = permuted_19;
    *(u16 *)(object + 20) = (u16)clamped;

    permuted_27 = Clamp0to16384(Func_03000380(*(short *)(object + 0x3a) << 14, *(short *)(object + 0x36)));
    *(u16 *)(object + 22) = (u16)clamped;
    clamped  = permuted_27;
    if (clamped == 0 && *(short *)(object + 0x3a) != 0) {
        *(u16 *)(object + 22) = 1;
    }

    Func_0808a548();
}
