/* Draft main:080ad40c, complete extent 252 bytes through 080ad508.
 * Bindings: gMenuWork=03001f2c; Link_DrawShiftedTilePairFar=08015418;
 * Math_Div=080022ec; Object_ApplyProjectedPlacementFar=08009008 (s32).
 * Hypothesis 3: the matched sibling's unsigned projection/scale words and
 * signed one-byte flag view, retaining the shared typed four-slot state.
 * Both scale words are set; only a nonnegative phase is written back.
 * Residual: 252/252 bytes, equal topology, 76 differing halfwords and 48
 * halfword edits; identical instructions to hypothesis 2. Vertical pseudo
 * 42 occupies r4 and spills across Math_Div; origin pseudo 131 occupies fp
 * and motion pseudo 112 occupies r9. Reference keeps vertical in sl, origin
 * at sp+4 and motion in fp/r4; flag/phase and request stores also reorder.
 * Stop: three structural hypotheses exhausted; not-yet-c, no adoption.
 */
#include "FOUR_OBJECT_MOTION.H"
#include "FIXED_MATH.H"

struct MenuMotionFlags { s8 flags; };

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

            ((struct MenuMotionFlags *)(obj + 9))->flags &= -13;
            phase = work->phases[i];
            if (phase < 0) {
                motion[0] = -phase;
                motion[1] = -phase;
            } else {
                motion[0] = phase + Math_Div(0x10000 - phase, 3);
                motion[1] = motion[0];
                work->phases[i] = motion[0];
            }
            request[0] = (u32)(s32)work->positions_x[i] << 16;
            request[1] = y;
            request[2] = ((u32)(s32)work->positions_y[i] << 16) + y;
            request[3] = 0;
            limit = work->positions_y[i] < 0 ? 0x8000 : 0x4000;
            Object_ApplyProjectedPlacementFar(obj, request, motion, limit);
        }
        i++;
    } while (i <= 3);
}
