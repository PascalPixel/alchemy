#include "scene.h"
#include "abi/battle/effects/interpolate/start_buffer.h"
#include "battle_effect_runtime.h"

void BattleFx_InterpolateBuffers(s16 *arg0, s16 *arg1, s16 *arg2, s32 arg3);

void BattleFx_StartBufferInterpolation(s32 mode)
{
    struct BattleEffectBuffers *buffers = gIw;

    if (buffers != NULL) {
        buffers->mode_2a01 = mode;
        buffers->unknown_2a02 = 0;
        BattleFx_InterpolateBuffers((s16 *)buffers->buffer_380,
                                        (s16 *)buffers->buffer_e00,
                                        (s16 *)buffers->buffer_1880,
                                        mode);
    }
}
