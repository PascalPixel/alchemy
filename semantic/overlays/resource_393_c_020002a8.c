typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

/*
 * resource_393 owner at 0x020002a8, 164 bytes: probe the square ahead of an
 * actor and cancel its motion when that square is blocked.
 *
 * Complete owner: 'push {r5, r6, r7, lr}' at 0x020002a8, a 12-byte local frame,
 * and the single 'add sp, #12' epilogue at 0x02000338, followed by its
 * three-word literal pool.
 *
 * TRANSPOSED from semantic/overlays/resource_373_c_020002a8.c.  The two owners
 * are the same routine shared verbatim: over all 82 halfwords they differ in
 * only 4 places - the two halves of the single BL pair, and the low halves of
 * the two pool words naming this overlay's data tables.
 *
 * What was changed:
 *  - the packed direction table 0x0200e190 becomes 0x02008f10 (file offset
 *    0x0f10 under the proven 0x02008000 link base).  Corroborated: this
 *    overlay's 0x020000c4 and 0x02000474 owners load the same address for the
 *    same table.
 *  - the passable-kind list 0x0200e1d0 becomes 0x02008f50 (offset 0x0f50),
 *    the same +0x40 spacing after the direction table as in resource_373.
 *  - the calls were re-resolved with 'bun tools/overlay_call_targets.ts
 *    resource_393 02a8': two sites, 0x0200006c (this overlay's own tile-occupant
 *    lookup) and the veneer publishing Func_080091d8 (the terrain probe).  The
 *    373 source predates the corrected 'bl' rule and spelled these as
 *    Func_02000342 / Func_02006266; both names are wrong and are replaced here.
 *
 * UNCERTAINTY (inherited): the six-entry list is compared against a
 * sign-extended 16-bit identifier read through the actor's 0x50 handle; the
 * meaning of the identifiers is not established, only that a match suppresses
 * the cancellation.
 */

struct Probe373Probe {
    s32 x;
    s32 y;
    s32 z;
};

struct Probe373Kind {
    s16 id;
};

struct Probe373Handle {
    u8 unknown_00[0x28];
    struct Probe373Kind *kind;
};

struct Probe373Mover {
    u8 unknown_00[6];
    u16 heading;                    /* top four bits select the direction */
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0x10];
    s32 velocityX;                  /* 0x24 */
    u8 unknown_28[4];
    s32 velocityZ;                  /* 0x2c */
    u8 unknown_30[8];
    s32 accelerationX;              /* 0x38 */
    u8 unknown_3c[4];
    s32 accelerationZ;              /* 0x40 */
    u8 unknown_44[0xc];
    struct Probe373Handle *handle;   /* 0x50 */
};

/* Packed direction steps, 16 entries indexed by the heading's top nibble. */
#define PROBE_DIRECTION_STEPS ((const s32 *)0x02008f10)

/* Six identifiers whose presence suppresses the motion cancellation. */
#define PROBE_PASSABLE_KINDS ((const s32 *)0x02008f50)

struct Probe373Mover *Func_0200006c(const struct Probe373Probe *probe,
                                       struct Probe373Mover *mover);
s32 Func_080091d8(struct Probe373Mover *mover,
                  const struct Probe373Probe *probe);

static void cancel_motion(struct Probe373Mover *mover)
{
    mover->velocityX = 0;
    mover->velocityZ = 0;
    mover->accelerationX = (s32)0x80000000;
    mover->accelerationZ = (s32)0x80000000;
}

static void step_ahead(const struct Probe373Mover *mover, s32 step,
                       struct Probe373Probe *probe)
{
    probe->x = mover->x + (step & (s32)0xffff0000);
    probe->y = mover->y;
    probe->z = mover->z + (step << 16);
}

s32 Func_020002a8(struct Probe373Mover *mover)
{
    struct Probe373Probe probe;
    s32 direction = mover->heading >> 12;
    s32 step = PROBE_DIRECTION_STEPS[direction];
    struct Probe373Mover *blocker;

    step_ahead(mover, step, &probe);
    blocker = Func_0200006c(&probe, mover);
    if (blocker != 0) {
        s16 kind = blocker->handle->kind->id;
        s32 index = 0;

        do {
            if ((s32)kind == PROBE_PASSABLE_KINDS[index]) {
                return 0;
            }
            index++;
        } while ((u32)index <= 5);

        cancel_motion(mover);
    }

    /* The table word is re-read after the probe call, exactly as encoded. */
    step = PROBE_DIRECTION_STEPS[direction];
    step_ahead(mover, step, &probe);
    if (Func_080091d8(mover, &probe) > 0) {
        return 0;
    }

    cancel_motion(mover);
    return 0;
}
