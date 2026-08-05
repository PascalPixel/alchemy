#include "types.h"

/*
 * resource_389 owner at 0x02000ecc, 444 bytes: emit a ring of six effects
 * around scene actor 10 when it reaches tile (38, 14), then set event flag
 * 0x301.
 *
 * Complete owner: `push {r5, r6, lr}` plus the r8/r9/sl/fp saves at 0x02000ecc
 * through the single epilogue at 0x0200105c, which ends `pop {r0} / bx r0` -
 * void by the HANDOVER section-0 epilogue rule.
 *
 * Pool map from a control-flow walk: 0x02000ecc-0x0200106f is reached as
 * instructions, 0x02001070-0x02001087 is not (six words: 0x000fffff,
 * 0xfffe0000, 0x00006666, 0xffff999a, 0xffff8000, 0x00000301).  420 code +
 * 24 pool = 444, exactly the inventory row's code_bytes and span_bytes.
 *
 * Calls: 31 sites, resolved with `bun tools/overlay_call_targets.ts
 * resource_389 ecc`.  Per-target multiset:
 *   Func_0808a080  x23  0xede 0xef0 0xf0e 0xf18 0xf20 0xf30 0xf38 0xf40 0xf64
 *                       0xf6c 0xf74 0xf96 0xf9e 0xfa6 0xfca 0xfd2 0xfda 0xff8
 *                       0x1000 0x1008 0x1028 0x1030 0x1038
 *   Func_02000a68  x6   0xf5e 0xf90 0xfc4 0xff2 0x1022 0x1052 (prologue sites)
 *   Func_080f9010  x1   0xf2a
 *   Func_080770c8  x1   0x1058
 * Total 31 against an advertised 30 - a beat, the safe direction.
 *
 * DELIBERATELY NOT FOLDED.  The six emission blocks differ only in the two
 * offset constants, and each of them re-looks-up the SAME actor three times
 * before its call.  A `for` loop over an offset table would have deflated the
 * Func_0808a080 multiset from 23 to 8 and the Func_02000a68 multiset from 6 to
 * 1 (HANDOVER: "a repeated endpoint block at the end of a builder is two sites,
 * not a loop", and "do not merge behaviourally identical arms").  Likewise the
 * pair of lookups at 0x02000f18 and 0x02000f20 feeds one store from two
 * separate calls and is kept as two.
 *
 * The offsets are a ring in 16.16: 0x8000 = +0.5, 0x6666 = +0.4,
 * 0xffff999a = -0.4, 0xffff8000 = -0.5, giving (+0.5,0), (+0.4,+0.4),
 * (-0.4,+0.4), (-0.5,0), (+0.4,-0.4), (-0.4,-0.4) - six points around the
 * actor.  The seventh argument is 1 at every site and the sixth is 0 at every
 * site.  0xfffe0000 stored to the actor's y is -2.0 in the same format.
 *
 * `x + 0x000fffff when negative, then asr #20` is the tile-grid idiom (>>16 to
 * integers, >>4 for the 16-pixel grid) with the round-toward-zero bias.
 */

struct SceneActor {
    u8 unknown_00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c */
    s32 z;                          /* 0x10 */
    u8 unknown_14[0x28];
    s32 field3c;                    /* 0x3c */
};

/* Old-style declarations: overlay imports vary in arity between call sites. */
struct SceneActor *Func_0808a080();  /* scene actor by selector */
void Func_080f9010();                /* play a cue by id */
void Func_080770c8();                /* set an event flag */

/* This overlay's own seven-argument placement helper at 0x02000a68. */
void Func_02000a68();

void Func_02000ecc(void)
{
    s32 tileX = Func_0808a080(10)->x;
    s32 tileZ;
    struct SceneActor *a;
    struct SceneActor *b;
    struct SceneActor *c;

    if (tileX < 0) tileX += 0x000fffff;
    tileX >>= 20;

    tileZ >>= 20;
    tileZ = Func_0808a080(10)->z;
    if (tileZ < 0) tileZ += 0x000fffff;

    if (tileX != 38) return;
    if (tileZ != 14) return;

    Func_0808a080(10)->y = 0xfffe0000;          /* -2.0 in 16.16 */

    /* Two distinct lookups of the same actor; one feeds the store, one the
     * value stored. */
    a = Func_0808a080(10);
    a->field3c = Func_0808a080(10)->y;

    Func_080f9010(188);

    /* East. */
    a = Func_0808a080(10);
    b = Func_0808a080(10);
    c = Func_0808a080(10);
    Func_02000a68(a->x, b->y, c->z, 0x00008000, 0, 0, 1);

    /* North-east. */
    a = Func_0808a080(10);
    b = Func_0808a080(10);
    c = Func_0808a080(10);
    Func_02000a68(a->x, b->y, c->z, 0x00006666, 0x00006666, 0, 1);

    /* North-west. */
    a = Func_0808a080(10);
    b = Func_0808a080(10);
    c = Func_0808a080(10);
    Func_02000a68(a->x, b->y, c->z, 0xffff999a, 0x00006666, 0, 1);

    /* West. */
    a = Func_0808a080(10);
    b = Func_0808a080(10);
    c = Func_0808a080(10);
    Func_02000a68(a->x, b->y, c->z, 0xffff8000, 0, 0, 1);

    /* South-east. */
    a = Func_0808a080(10);
    b = Func_0808a080(10);
    c = Func_0808a080(10);
    Func_02000a68(a->x, b->y, c->z, 0x00006666, 0xffff999a, 0, 1);

    /* South-west. */
    a = Func_0808a080(10);
    b = Func_0808a080(10);
    c = Func_0808a080(10);
    Func_02000a68(a->x, b->y, c->z, 0xffff999a, 0xffff999a, 0, 1);

    Func_080770c8(0x301);           /* the flag the entry-0 root reads */
}
