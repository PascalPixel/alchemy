#include "battle_effect_runtime.h"

void Func_0809088c(s16 *arg0, s16 *arg1, s16 *arg2, s32 arg3);

void Func_08091254(s32 mode)
{
    struct BattleEffectBuffers *buffers = Data_03001ed0;

    if (buffers != NULL) {
        buffers->mode_2a01 = mode;
        buffers->unknown_2a02 = 0;
        Func_0809088c((s16 *)buffers->buffer_380,
                      (s16 *)buffers->buffer_e00,
                      (s16 *)buffers->buffer_1880,
                      mode);
    }
}
