/* Draft main:080ad40c, complete extent 252 bytes through 080ad508.
 * Bindings: gMenuWork=03001f2c; Link_DrawShiftedTilePairFar=08015418;
 * Math_Div=080022ec; Object_ApplyProjectedPlacementFar=08009008 (s32).
 * Hypothesis 1: typed slot arrays and owned placement/scale records.
 * Both scale words are set; only a nonnegative phase is written back.
 * Residual: 248/252 bytes, equal topology, 116/119 instructions. The
 * origin induction stays in fp; vertical spills around division, the flag
 * mask becomes 243, and request stores/call argument setup differ.
 */
#include "FOUR_OBJECT_MOTION.H"
#include "FIXED_MATH.H"
#include "OVERLAY_OBJECT.H"

struct MotionScale { s32 x; s32 y; };
struct ProjectionRequest { s32 x; s32 y; s32 z; s32 w; };

extern struct FourObjectMotionState *gMenuWork;

void Link_DrawShiftedTilePairFar(void *);
s32 Object_ApplyProjectedPlacementFar(void *, struct ProjectionRequest *,
    struct MotionScale *, s32);

void FourObjectMotion_UpdateBottomRow(void)
{
    struct FourObjectMotionState *work = gMenuWork;
    s32 i;
    struct MotionScale motion;
    struct ProjectionRequest request;

    Link_DrawShiftedTilePairFar((void *)0x06002500);
    i = 0;
    do {
        struct OverlayObjectRecord *obj = work->objects[i];

        if (obj != NULL) {
            s32 y = (241 << 17) - ((s32)work->vertical_origins[i] << 16);
            s32 phase;
            s32 limit;

            obj->flags &= -13;
            phase = work->phases[i];
            if (phase < 0) {
                motion.x = -phase;
                motion.y = -phase;
            } else {
                motion.x = phase + Math_Div(0x10000 - phase, 3);
                motion.y = motion.x;
                work->phases[i] = motion.x;
            }
            request.x = (s32)work->positions_x[i] << 16;
            request.y = y;
            request.z = ((s32)work->positions_y[i] << 16) + y;
            request.w = 0;
            limit = work->positions_y[i] < 0 ? 0x8000 : 0x4000;
            Object_ApplyProjectedPlacementFar(obj, &request, &motion, limit);
        }
        i++;
    } while (i <= 3);
}
