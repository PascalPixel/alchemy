/* Draft of FieldScene_RunPrimarySequence, resource_39c at 0x02009db4, for
 * FIELD/MAKYURI_HEYA (it compiles with that directory's PROBE.H).
 * Remaining difference: 21 halfwords in the first two channels' spark calls.
 * The size, frame, loop and the third channel match; in the first two, the
 * game reloads the spilled options pointer into r3 after storing the flags,
 * where this reloads it into r2 ahead of them, so the argument stores are
 * scheduled differently. Tried: temporaries or inline random terms, `mode`
 * for the zero, u32 positions, j's test and update order, and positions as
 * expressions of i and j (which strength-reduces i << 20 and spills more). */
#include "PROBE.H"

/* A spark's speed across the channel: a random step of 0.2 between -0.8 and
 * 0.6. */
#define SPARK_DRIFT() ((((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334)

/* Open one of the room's three floor channels: swap in its first cells, send
 * two rows of sparks along it, and swap in the rest of the channel after each
 * row. */
void FieldScene_RunPrimarySequence(s32 mode)
{
    struct EffectOptions opts;
    u32 i, j;
    s32 back;
    s32 pos;

    Audio_PlayCue(211);
    if (mode == 0) {
        Map_CopyCellsTo(111, 57, 113, 42, 1, 1);
        Map_CopyCellsTo(111, 59, 113, 43, 1, 1);
    } else if (mode == 1) {
        Map_CopyCellsTo(113, 58, 112, 46, 1, 1);
        Map_CopyCellsTo(115, 58, 113, 46, 1, 1);
    } else {
        Map_CopyCellsTo(115, 57, 116, 44, 1, 1);
        Map_CopyCellsTo(113, 57, 115, 44, 1, 1);
    }
    opts.palette = 7;
    opts.start_scale_x = 0x8000;
    opts.start_scale_y = 0x8000;
    for (i = 0; i <= 1; i++) {
        j = 0;
        back = (203 << 18) - (i << 20);
        pos = (i << 20) + (176 << 18);
        for (; j <= 7; j++) {
            if ((j & 1) != 0) {
                if (mode == 0)
                    Effect_Spawn(198 << 18, 0, pos, SPARK_DRIFT(), mode, SPARK_DRIFT(), 0x90000,
                                 &opts);
                else if (mode == 1)
                    Effect_Spawn(pos + (192 << 15), 0, 0x02ea0000, SPARK_DRIFT(), 0,
                                 SPARK_DRIFT(), 0x90000, &opts);
                else
                    Effect_Spawn(back, 0, 0x02ca0000, SPARK_DRIFT(), 0, SPARK_DRIFT(), 0x90000,
                                 &opts);
                Event_Wait(1);
            }
            back -= 0x10000;
            pos += 0x10000;
        }
        if (mode == 0) {
            Map_CopyCellsTo(111, 58, 113, i + 43, 1, 1);
            Map_CopyCellsTo(111, 59, 113, i + 44, 1, 1);
        } else if (mode == 1) {
            Map_CopyCellsTo(114, 58, i + 113, 46, 1, 1);
            Map_CopyCellsTo(115, 58, i + 114, 46, 1, 1);
        } else {
            Map_CopyCellsTo(114, 57, 115 - i, 44, 1, 1);
            Map_CopyCellsTo(113, 57, 114 - i, 44, 1, 1);
        }
    }
}
