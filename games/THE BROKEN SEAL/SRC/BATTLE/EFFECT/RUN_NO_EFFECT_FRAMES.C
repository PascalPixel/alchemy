#include "TYPES.H"

/* battle/effects/common/run_no_effect_frames.c */
void BattleFx_RunNoEffect(void);

void BattleFx_RunNoEffectFrames(s32 frame_count)
{
    s32 frame;

    frame = 0;
    if (frame_count != 0) {
        do {
            frame += 1;
            BattleFx_RunNoEffect();
        } while (frame != frame_count);
    }
}
