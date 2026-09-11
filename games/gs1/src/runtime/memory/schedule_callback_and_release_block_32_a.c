#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* runtime/memory/schedule_callback_and_release_block_32_a.c */
s32 ScheduleCallback(s32);

extern u8 gRom;

void Runtime_ScheduleCallbackAndReleaseBlock32A(void)
{
    ScheduleCallback((s32)&gRom);
    Sys_Check(0x20);
}

/* graphics/color/transform_large_palette.c */
/* graphics/color/transform_large_palette.c */
s32 Graphics_TransformPaletteBuffer(s32, void *, void *, s32);

void Graphics_TransformLargePalette(s32 arg0, s32 arg1)
{
    void *target = *(void **)ADDR_03001ED0;
    if (target != NULL) {
        Graphics_TransformPaletteBuffer(arg0, target, (u8 *)target + 0x1000, arg1);
    }
}

/* graphics/color/transform_small_palette.c */
s32 Graphics_TransformPaletteBuffer(s32, s32, s32, s32);

void Graphics_TransformSmallPalette(s32 palette_index, s32 transform)
{
    s32 palette_buffer;

    palette_buffer = *(s32 *)ADDR_03001ED0;
    if (palette_buffer != 0) {
        Graphics_TransformPaletteBuffer(palette_index, palette_buffer, palette_buffer + 0x400, transform);
    }
}

/* graphics/color/set_palette_transform_value.c */
void Graphics_SetPaletteTransformValue(s32 arg0)
{
    u16 *target = *(u16 **)ADDR_03001ED0;
    if (target != NULL) {
        *target = arg0;
    }
}

/* graphics/color/update_palette_interpolation.c */
extern void Graphics_InterpolatePaletteBuffers(s16 *arg0, s16 *arg1, s16 *arg2, s32 arg3);

typedef struct {
    u8 padding_0000[0x400];
    s16 first[0x600];
    s16 second[0x600];
    s16 output[0xa00];
    u8 padding_3000;
    s8 value;
    s8 zero;
} PaletteInterpolationState;

void Graphics_UpdatePaletteInterpolation(s32 arg0)
{
    PaletteInterpolationState *state = *(PaletteInterpolationState **)ADDR_03001ED0;

    if (state != 0) {
        state->value = arg0;
        state->zero = 0;
        Graphics_InterpolatePaletteBuffers(state->first, state->second, state->output, arg0);
    }
}

/* graphics/color/clamp_rgb555_channel.c */
s32 Graphics_ClampRgb555Channel(s32 val)
{
    if (val > 31)
        return 31;
    if (val < 0)
        val = 0;
    return val;
}

/* graphics/color/clamp_rgb555_component.c */
s32 Graphics_ClampRgb555Component(s32 val)
{
    if (val > 31744)
        val = 31744;
    return val;
}

/* runtime/blank_display_and_run.c */
s32 Audio_PlayCue(s32);

s32 Runtime_BlankDisplayAndRun(void)
{
    *(s16 *)0x04000000 = 0x40;
    Audio_PlayCue(9);
    Sys_CheckBlankDisplayAnd();
    return 0;
}
