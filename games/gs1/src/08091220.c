#include "battle_effect_runtime.h"

s32 Func_08090a5c(s32, s32, s32, s32);

void Func_08091220(s32 value, s32 mode)
{
    struct BattleEffectBuffers *buffers = Data_03001ed0;

    if (buffers != NULL) {
        Func_08090a5c(value, (s32)buffers, (s32)buffers->buffer_380, mode);
    }
}
