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
 * types, register and a split assignment do not move it).
 * Family audit (2026-09-27): complete 02001db4..02002030 owner, 636/636,
 * 21 halfwords / 18 aligned edits. Correct the scope above: only the side
 * zero/one spawn sites differ; the final side branch is already exact.
 * Outgoing words at sp+0/+4/+8/+12 are velocity_y, velocity_z, flags and
 * options. The 0x90000 flags select palette and initial scales, not lift.
 * The options pointer is spilled at sp+16, j at sp+20, with the 40-byte
 * options record at sp+24 and a 64-byte frame. All these offsets agree.
 * Exact COMMON/EFFECT/SPAWN.C reads the selected fields during the call
 * and does not retain the record pointer. Exact MAKYURI_HEYA/OPEN_STAIR.C,
 * FIELD_PROBE_SCENE.C dust rows and VINASU_HEYA/BRIDGE_EXTEND.C also use
 * caller-owned records; they supply no different lifetime/ownership rule.
 * No new admissible source-interface hypothesis follows from this audit.
 * Preserve the canonical body and all exact regions; do not retry the
 * closed parameter-type, inline-spawn, counter or declaration axes.
 * This audit changes no emitted bytes and earns 0 new DONE bytes.
 * Sol Mercury H1 (2026-09-27): the ordinary inline drift producer keeps
 * the unsigned random intermediate and returns its fixed-point step.
 * Complete normalized diff and binary equality against the macro baseline
 * confirm unchanged 636/636 bytes, 21 halfwords / 18 edits. Reload already
 * chooses r2 for the options pointer before sched2, so this producer
 * boundary cannot fix the side-zero/one argument staging. Keep the readable
 * helper as the canonical draft; close this axis. New exact bytes: 0. */
#include "FIELD_EFFECT.H"

/* A random drift of about -0.8 to +0.8 in steps of 0.2. */
static __inline__ s32 Door_DriftStep(void)
{
    u32 drift = (u32)Engine_RandomNext();

    return ((drift << 3) >> 16) * 0x3333;
}

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
                    Effect_Spawn(0x3180000, 0, up, Door_DriftStep() + -0xcccc, 0, Door_DriftStep() + -0xcccc, 0x90000, p);
                } else if (side == 1) {
                    Effect_Spawn(up + 0x600000, 0, 0x2ea0000, Door_DriftStep() + -0xcccc, 0, Door_DriftStep() + -0xcccc, 0x90000, p);
                } else {
                    Effect_Spawn(down, 0, 0x2ca0000, Door_DriftStep() + -0xcccc, 0, Door_DriftStep() + -0xcccc, 0x90000, p);
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
