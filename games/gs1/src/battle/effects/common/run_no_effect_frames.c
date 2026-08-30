#include "types.h"

void Func_080dbb98(void);

void BattleEffect_RunNoEffectFrames(s32 frame_count) {
    s32 frame;

    frame = 0;
    if (frame_count != 0) {
        do {
            frame += 1;
            Func_080dbb98();
        } while (frame != frame_count);
    }
}
