#include "types.h"

/* battle/effects/common/run_no_effect_frames.c */
void Battle_Run(void);

void BattleFx_RunNoEffectFrames(s32 frame_count)
{
    s32 frame;

    frame = 0;
    if (frame_count != 0) {
        do {
            frame += 1;
            Battle_Run();
        } while (frame != frame_count);
    }
}
