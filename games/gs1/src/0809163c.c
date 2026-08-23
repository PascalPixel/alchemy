#include "battle_effect_runtime.h"

void WaitFrames(void);

void Func_0809163c(s32 enabled)
{
    if (Data_03001ebc->mode_1cc == 0 && enabled != 0) {
        WaitFrames();
    }
}
