#include "scene.h"
#include "abi/battle/effects/interpolate/apply_color_to_source_buffer.h"
#include "battle_effect_runtime.h"

void BattleFx_ApplyColorToSourceBuffer(s32 value, s32 mode)
{
    struct BattleEffectBuffers *buffers = gIw;

    if (buffers != NULL) {
        Battle_SetMode(value, (s32)buffers, (s32)buffers->buffer_380, mode);
    }
}
