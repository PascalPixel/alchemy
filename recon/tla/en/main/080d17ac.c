#include "SCENE.H"
#include "TYPES.H"

s32 BattleFx_BuildBuffer(s32, s32, s32, s32);

void BattleFx_StartBufferInterpolation(s32 mode)
{
    struct BattleEffectBuffers *buffers = Data_03001ed0;

    if (buffers != NULL) {
        buffers->mode_2a01 = mode;
        buffers->unknown_2a02 = 0;
        BattleFx_InterpolateBuffers((s16 *)buffers->buffer_380,
                                        (s16 *)buffers->buffer_e00,
                                        (s16 *)buffers->buffer_1880,
                                        mode);
    }
}
