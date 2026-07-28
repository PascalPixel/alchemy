#include "battle_effect_runtime.h"

void Func_080030f8(void);

void Func_0809163c(s32 enabled)
{
    if (Data_03001ebc->unknown_1cc == 0 && enabled != 0) {
        Func_080030f8();
    }
}
