/* Draft main:080ad40c, 252 bytes. Score 230 (2 register, 1 operand,
 * 2 deleted). The reference reads positions_y[i] a second time after the
 * request[3] store, for the limit; this draft caches it in row_y. Every
 * re-reading spelling tried (plain, if/else, ?:, for and do loops, other
 * declaration orders and phase forms) scores 1118: the extra use lets y take
 * r4 by caller-save instead of r10 and the vertical-origin offset leaves its
 * stack slot. 2026-10-04: priority clears the real FieldSprite bitfield and
 * the division is plain C, so the memory-barrier device is gone; without the
 * zero local the draft scores 275.
 */
#include "FOUR_OBJECT_MOTION.H"
#include "FIELD_SPRITE.H"

extern struct FourObjectMotionState *gMenuWork;

void Link_DrawShiftedTilePairFar(void *);
s32 Object_ApplyProjectedPlacementFar(u32, u32 *, u32 *, u32);

void FourObjectMotion_UpdateBottomRow(void)
{
    struct FourObjectMotionState *work = gMenuWork;
    s32 i;
    u32 motion[2];
    u32 request[4];

    Link_DrawShiftedTilePairFar((void *)0x06002500);
    i = 0;
    do {
        u32 obj = (u32)work->objects[i];

        if (obj != 0) {
            u32 y = (241u << 17) - ((u32)(s32)work->vertical_origins[i] << 16);
            s32 phase;
            s32 limit;
            s32 row_y;
            /* FAKEMATCH: a zero local set before the request, as the
               reference's allocation needs. */
            u32 zero;

            ((struct FieldSprite *)obj)->priority = 0;
            phase = work->phases[i];
            if (phase < 0) {
                motion[0] = -phase;
                motion[1] = -phase;
            } else {
                motion[0] = phase + (s32)(0x10000 - phase) / 3;
                motion[1] = motion[0];
                work->phases[i] = motion[0];
            }
            zero = 0;
            request[0] = (u32)(s32)work->positions_x[i] << 16;
            request[1] = y;
            row_y = work->positions_y[i];
            request[2] = ((u32)row_y << 16) + y;
            request[3] = zero;
            limit = row_y < 0 ? 0x8000 : 0x4000;
            Object_ApplyProjectedPlacementFar(obj, request, motion, limit);
        }
        i++;
    } while (i <= 3);
}
