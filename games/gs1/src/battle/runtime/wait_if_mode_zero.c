#include "battle_effect_runtime.h"

void WaitFrames(void);

void BattleRuntime_WaitIfModeZero(s32 should_wait)
{
    if (Data_03001ebc->mode_1cc == 0 && should_wait != 0) {
        WaitFrames();
    }
}
