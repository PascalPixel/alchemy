/* Draft main:080ad40c, complete extent 252 bytes through 080ad508.
 * Bindings: gMenuWork=03001f2c; Link_DrawShiftedTilePairFar=08015418;
 * Math_Div=080022ec; Object_ApplyProjectedPlacementFar=08009008 (s32).
 * Hypothesis 2: signed object flags and the sibling's array interface.
 * Both scale words are set; only a nonnegative phase is written back.
 * Residual: 252/252 bytes, equal topology, 76 differing halfwords. Signed
 * flags recover the -13 mask; the origin induction occupies fp and vertical
 * still spills around division. Scale/request lifetimes and stores differ.
 */
#include "FOUR_OBJECT_MOTION.H"
#include "FIXED_MATH.H"

struct MenuMotionObject { u8 unknown_00[9]; s8 flags; };

extern struct FourObjectMotionState *gMenuWork;

void Link_DrawShiftedTilePairFar(void *);
s32 Object_ApplyProjectedPlacementFar(void *, s32 *, s32 *, s32);

void FourObjectMotion_UpdateBottomRow(void)
{
    struct FourObjectMotionState *work = gMenuWork;
    s32 i;
    s32 motion[2];
    s32 request[4];

    Link_DrawShiftedTilePairFar((void *)0x06002500);
    i = 0;
    do {
        struct MenuMotionObject *obj = work->objects[i];

        if (obj != NULL) {
            s32 y = (241 << 17) - ((s32)work->vertical_origins[i] << 16);
            s32 phase;
            s32 limit;

            obj->flags &= -13;
            phase = work->phases[i];
            if (phase < 0) {
                motion[0] = -phase;
                motion[1] = -phase;
            } else {
                motion[0] = phase + Math_Div(0x10000 - phase, 3);
                motion[1] = motion[0];
                work->phases[i] = motion[0];
            }
            request[0] = (s32)work->positions_x[i] << 16;
            request[1] = y;
            request[2] = ((s32)work->positions_y[i] << 16) + y;
            request[3] = 0;
            limit = work->positions_y[i] < 0 ? 0x8000 : 0x4000;
            Object_ApplyProjectedPlacementFar(obj, request, motion, limit);
        }
        i++;
    } while (i <= 3);
}
