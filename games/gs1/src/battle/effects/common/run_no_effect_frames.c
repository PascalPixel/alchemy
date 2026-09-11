#include "types.h"
#include "scene.h"
#include "abi/battle/effects/common/run_no_effect_frames.h"

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
