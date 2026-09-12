#include "scene.h"
#include "battle_effect_runtime.h"
#include "types.h"

/* battle/effects/interpolate/apply_color_to_target_buffer.c */
s32 Battle_SetMode(s32, s32, s32, s32);

void BattleFx_ApplyColorToTargetBuffer(s32 value, s32 mode)
{
    struct BattleEffectBuffers *buffers = gIw;

    if (buffers != NULL) {
        Battle_SetMode(value, (s32)buffers, (s32)buffers->buffer_e00, mode);
    }
}
