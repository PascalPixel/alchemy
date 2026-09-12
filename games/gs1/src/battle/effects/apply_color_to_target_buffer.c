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

/* battle/effects/interpolate/apply_color_to_source_buffer.c */
void BattleFx_ApplyColorToSourceBuffer(s32 value, s32 mode)
{
    struct BattleEffectBuffers *buffers = gIw;

    if (buffers != NULL) {
        Battle_SetMode(value, (s32)buffers, (s32)buffers->buffer_380, mode);
    }
}

/* battle/effects/buffers/set_primary_value.c */
void BattleFx_SetPrimaryBufferValue(unsigned int value)
{
  s16 *primary_buffer;
  primary_buffer = *((s16 **)ADDR_03001ED0);
  if (0 != primary_buffer)
  {
    *primary_buffer = value;
  }
}
