/* Draft, not-yet-c: 636 of 636 bytes, 21 differing halfwords (2026-09-26).
 * The shared EffectOptions layout identifies palette and starting scales;
 * unsigned random shifts preserve the reference's logical shift. Concrete
 * parameter types do not move the reload residual, while an inline spawn
 * boundary regresses it to 193 halfwords and is not retained.
 * Readable unit source (evconv Engine_* veneers); both drifts are one STEP()
 * multiply by 0x3333 in the call, and declaring j before p gives the
 * reference spill slots (p at sp+16, j at sp+20). Remaining: in each
 * Effect_Spawn call reload takes r2 for the spilled params pointer where the
 * reference reuses r3 after the lift store, so sched2 hoists the load and
 * reorders the stack-argument stores (reload register rotation; counter
 * types, register and a split assignment do not move it). */
#include "FIELD_EFFECT.H"

/* A random drift of about -0.8 to +0.8 in steps of 0.2. */
#define STEP() ((s32)(((u32)Engine_RandomNext() << 3) >> 16) * 0x3333)

/* Slide one of three stone doors two cells open, with dust along its edge. */
void Func_02001db4(s32 side)
{
    struct EffectOptions params;
    u32 j;
    struct EffectOptions *p;
    u32 i;
    s32 down;
    s32 up;

    Engine_AudioPlayCue(211);
    if (side == 0) {
        Engine_MapCopyCellsTo(111, 57, 113, 42, 1, 1);
        Engine_MapCopyCellsTo(111, 59, 113, 43, 1, 1);
    } else if (side == 1) {
        Engine_MapCopyCellsTo(113, 58, 112, 46, side, side);
        Engine_MapCopyCellsTo(115, 58, 113, 46, side, side);
    } else {
        Engine_MapCopyCellsTo(115, 57, 116, 44, 1, 1);
        Engine_MapCopyCellsTo(113, 57, 115, 44, 1, 1);
    }
    p = &params;
    p->palette = 7;
    p->start_scale_x = 0x8000;
    p->start_scale_y = 0x8000;
    for (i = 0; i <= 1; i++) {
        j = 0;
        down = 0x32c0000 - (i << 20);
        up = (i << 20) + 0x2c00000;
        for (; j <= 7; j++) {
            if (j & 1) {
                if (side == 0) {
                    Effect_Spawn(0x3180000, 0, up, STEP() + -0xcccc, 0, STEP() + -0xcccc, 0x90000, p);
                } else if (side == 1) {
                    Effect_Spawn(up + 0x600000, 0, 0x2ea0000, STEP() + -0xcccc, 0, STEP() + -0xcccc, 0x90000, p);
                } else {
                    Effect_Spawn(down, 0, 0x2ca0000, STEP() + -0xcccc, 0, STEP() + -0xcccc, 0x90000, p);
                }
                Engine_EventWait(1);
            }
            down += -0x10000;
            up += 0x10000;
        }
        if (side == 0) {
            Engine_MapCopyCellsTo(111, 58, 113, i + 43, 1, 1);
            Engine_MapCopyCellsTo(111, 59, 113, i + 44, 1, 1);
        } else if (side == 1) {
            Engine_MapCopyCellsTo(114, 58, i + 113, 46, side, side);
            Engine_MapCopyCellsTo(115, 58, i + 114, 46, side, side);
        } else {
            Engine_MapCopyCellsTo(114, 57, 115 - i, 44, 1, 1);
            Engine_MapCopyCellsTo(113, 57, 114 - i, 44, 1, 1);
        }
    }
}
