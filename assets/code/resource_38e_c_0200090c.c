#include "types.h"

/*
 * resource_38e owner at 0x0200090c, 102 bytes: the per-entity BEHAVIOUR
 * callback that walks one entity around a lobe of a sine/cosine figure and
 * advances its phase by a random step.
 *
 * Named before it was opened, by the already-converted installer.
 * semantic/overlays/resource_38e_c_02000974.c ends with
 *   entity->behaviour = (void (*)(void))0x0200890d;
 * and under the proven 0x02008000 link base 0x0200890d is file offset 0x090c
 * plus the Thumb bit.  That fixed both the role and the argument: the callback
 * receives the entity whose 0x6c field holds it, which is exactly what r0 is
 * used as here.  The installer's field map (handle at 0x50) is reused unchanged
 * and is corroborated by this owner reading the same offset.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x0200090c through the single
 * epilogue at 0x0200096c.  It ends `pop {r1} / bx r1`, so r0 survives and IS
 * the result; `movs r0, #0` immediately before it, so the callback returns 0.
 *
 * Pool map from a control-flow walk: every halfword from 0x0200090c to
 * 0x02000971 is reached as an instruction and the owner has NO literal pool at
 * all - it loads no constant wider than an 8-bit immediate plus a shift.  The
 * halfword 0x0000 at 0x02000972 is alignment ahead of the next owner
 * (0x02000974) and lies outside the row.  102 code + 0 pool = 102, matching the
 * inventory row's code_bytes and span_bytes, which are equal here for that
 * reason.
 *
 * Calls: 5 sites, matching the advertised 5:
 *   Func_080000f8  x2   0x94a 0x950   (random source)
 *   Func_08000120  x2   0x922 0x93c   (trig, sine lobe)
 *   Func_08000118  x1   0x914         (trig, cosine lobe)
 * The two Func_08000120 sites take DIFFERENT arguments - the raw phase and the
 * phase plus 0x8000 - so they are two sites, not a common subexpression.  The
 * two Func_080000f8 draws are likewise independent and are summed, not doubled.
 *
 * The vertical term is forced non-positive: `lsls #1` then `negs` only when the
 * result is greater than zero, so the entity travels on one lobe rather than a
 * full circle.  That asymmetry is real; symmetrising it would change the path.
 *
 * `if (v < 0) v += 7; v >>= 3` is division by 8 rounded toward zero, the same
 * bias-then-shift family as the tile-grid idiom.
 *
 * The phase step `(rand << 9) >> 16` twice, plus 1024, extracts bits 7..22 of
 * each draw as a 16-bit quantity - the shift pair is a field extraction, not a
 * scale.
 */

struct SceneHandle {
    u8 unknown_00[30];
    s16 field1e;                    /* 0x1e */
};

struct SceneEntity {
    u8 unknown_00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c */
    u8 unknown_10[0x20];
    s32 phase;                      /* 0x30 */
    u8 unknown_34[4];
    s32 originX;                    /* 0x38 */
    s32 originY;                    /* 0x3c */
    u8 unknown_40[0x10];
    struct SceneHandle *handle;     /* 0x50 */
};

/* Old-style declarations: overlay imports vary in arity between call sites. */
extern s32 Func_0200137a(s32);
extern s32 Func_02001390(s32);
extern s32 Func_020013aa(s32);

extern s32 Func_020013a0(void);   /* random source, two call sites */
extern s32 Func_020013a6(void);

s32 Func_0200090c(struct SceneEntity *entity)
{
    struct SceneHandle *handle = entity->handle;
    s32 vertical;
    s32 tilt;
    s32 step;

    vertical = Func_0200137a(entity->phase) * 2;
    if (vertical > 0) vertical = -vertical;

    entity->x = entity->originX + Func_02001390(entity->phase) * 2;
    entity->y = entity->originY + vertical;

    /* A quarter turn on from the position phase. */
    tilt = Func_020013aa(entity->phase + 0x8000);
    if (tilt < 0) tilt += 7;
    handle->field1e = (s16)(tilt >> 3);

    /* `lsrs` in the original: the extraction is unsigned. */
    step = (s32)(((u32)Func_020013a0() << 9) >> 16)
         + (s32)(((u32)Func_020013a6() << 9) >> 16);
    entity->phase = entity->phase + step + 1024;

    return 0;
}
