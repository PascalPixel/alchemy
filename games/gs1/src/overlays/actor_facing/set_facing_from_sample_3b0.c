#include "types.h"

/*
 * Scripted per-object step for resource_3b0.  It turns the object to one of two
 * fixed headings when a sampled value picks them out, then reports finished.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
s32 Func_02001246();            /* Sampled value. */

/*
 * The shift pair is a windowed extraction of bits 10..15 of the sample, giving
 * a value in 0..63; a plain right shift would let larger values reach the
 * comparisons.  The headings 0xd000 and 0xb000 are built from an immediate and
 * a shift, and the local in each arm is what forces that.  The halfword at
 * object + 6 is the facing angle.  What Func_02001246 samples is not
 * established here.
 */
s32 SceneActor_SetFacingFromSample(u8 *object)
{
    u32 sample = (u32)(Func_02001246() << 6) >> 16;   /* bits 10..15 */

    if (sample == 6) {
        s32 value = 0xd000;

        *(u16 *)(object + 6) = value;
    } else if (sample == 9) {
        s32 value = 0xb000;

        *(u16 *)(object + 6) = value;
    }

    return 1;
}
