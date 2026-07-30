typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

/*
 * Resource 373: probe the square ahead of an actor and cancel its motion
 * when the square is blocked.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x020002a8, a 12-byte local
 * frame opened at 0x020002bc, and the single `add sp, #12` epilogue at
 * 0x02000338..0x0200033e, followed by its three-word literal pool.  Every
 * branch converges on that epilogue and nothing stays live past it.
 *
 * Both calls are placed.  Neither is an interworking `call_via rN` site.
 *
 * The direction table at 0x0200e190 holds one packed word per heading: the
 * high half is the X step and the low half the Z step, each promoted to
 * 16.16 fixed point by masking or shifting rather than by a multiply.  That
 * is the same packed-direction idiom the resource 3bd conversion recorded.
 *
 * UNCERTAINTY 1: the called service addresses are the ones encoded in the
 * overlay image (the 0x02000000 namespace is shared with resident services).
 * UNCERTAINTY 2: the six-entry list at 0x0200e1d0 is compared against a
 * sign-extended 16-bit identifier read through the actor's 0x50 handle; the
 * meaning of the identifiers is not established here, only that a match
 * suppresses the cancellation.
 */

struct Resource373Probe {
    s32 x;
    s32 y;
    s32 z;
};

struct Resource373Kind {
    s16 id;
};

struct Resource373Handle {
    u8 unknown_00[0x28];
    struct Resource373Kind *kind;
};

struct Resource373Mover {
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
    struct Resource373Handle *handle;   /* 0x50 */
};

/* Packed direction steps, 16 entries indexed by the heading's top nibble. */
#define RESOURCE_373_DIRECTION_STEPS ((const s32 *)0x0200e190)

/* Six identifiers whose presence suppresses the motion cancellation. */
#define RESOURCE_373_PASSABLE_KINDS ((const s32 *)0x0200e1d0)

struct Resource373Mover *Func_02000342(const struct Resource373Probe *probe,
                                       struct Resource373Mover *mover);
s32 Func_02006266(struct Resource373Mover *mover,
                  const struct Resource373Probe *probe);

static void cancel_motion(struct Resource373Mover *mover)
{
    mover->velocityX = 0;
    mover->velocityZ = 0;
    mover->accelerationX = (s32)0x80000000;
    mover->accelerationZ = (s32)0x80000000;
}

static void step_ahead(const struct Resource373Mover *mover, s32 step,
                       struct Resource373Probe *probe)
{
    probe->x = mover->x + (step & (s32)0xffff0000);
    probe->y = mover->y;
    probe->z = mover->z + (step << 16);
}

s32 Func_020002a8(struct Resource373Mover *mover)
{
    struct Resource373Probe probe;
    s32 direction = mover->heading >> 12;
    s32 step = RESOURCE_373_DIRECTION_STEPS[direction];
    struct Resource373Mover *blocker;

    step_ahead(mover, step, &probe);
    blocker = Func_02000342(&probe, mover);
    if (blocker != 0) {
        s16 kind = blocker->handle->kind->id;
        s32 index = 0;

        do {
            if ((s32)kind == RESOURCE_373_PASSABLE_KINDS[index]) {
                return 0;
            }
            index++;
        } while ((u32)index <= 5);

        cancel_motion(mover);
    }

    /* The table word is re-read after the probe call, exactly as encoded. */
    step = RESOURCE_373_DIRECTION_STEPS[direction];
    step_ahead(mover, step, &probe);
    if (Func_02006266(mover, &probe) > 0) {
        return 0;
    }

    cancel_motion(mover);
    return 0;
}
