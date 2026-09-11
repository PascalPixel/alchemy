#include "scene.h"
#include "abi/battle/runtime/wait_if_mode_zero.h"
#include "battle_effect_runtime.h"

void WaitFrames(void);

void Battle_WaitMode0(s32 should_wait)
{
    if (gWork->mode_1cc == 0 && should_wait != 0) {
        WaitFrames();
    }
}
