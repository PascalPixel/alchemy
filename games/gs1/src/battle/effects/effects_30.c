#include "scene.h"
#include "battle_effect_runtime.h"
#include "types.h"
#include "global_cells.h"

/* battle/effects/interpolate/apply_color_to_target_buffer.c */
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

/* battle/effects/interpolate/start_buffer.c */
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

/* battle/effects/color/clamp_rgb555_channel.c */
s32 BattleFx_ClampRgb555Channel(s32 value)
{
    if (value > 31)
        return 31;
    if (value < 0)
        value = 0;
    return value;
}

/* battle/effects/color/clamp_rgb555_component.c */
s32 BattleFx_ClampRgb555Component(s32 value)
{
    if (value > 31744)
        value = 31744;
    return value;
}
