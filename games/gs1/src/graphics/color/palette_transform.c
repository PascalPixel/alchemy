#include "types.h"
#include "global_cells.h"

s32 Func_080f3078(s32, void *, void *, s32);
void Func_080f2ebc(s16 *, s16 *, s16 *, s32);

struct PaletteInterpolationState {
    u8 unknown_0000[0x400];
    s16 first[0x600];
    s16 second[0x600];
    s16 output[0xa00];
    u8 unknown_3000;
    s8 value;
    s8 zero;
};

void Graphics_TransformLargePalette(s32 index, s32 transform)
{
    void *target = *(void **)ADDR_03001ED0;

    if (target != NULL)
        Func_080f3078(index, target, (u8 *)target + 0x1000, transform);
}

void Graphics_TransformSmallPalette(s32 index, s32 transform)
{
    void *target = *(void **)ADDR_03001ED0;

    if (target != NULL)
        Func_080f3078(index, target, (u8 *)target + 0x400, transform);
}

void Graphics_SetPaletteTransformValue(s32 value)
{
    u16 *target = *(u16 **)ADDR_03001ED0;

    if (target != NULL)
        *target = value;
}

void Graphics_UpdatePaletteInterpolation(s32 value)
{
    struct PaletteInterpolationState *state =
        *(struct PaletteInterpolationState **)ADDR_03001ED0;

    if (state != NULL) {
        state->value = value;
        state->zero = 0;
        Func_080f2ebc(state->first, state->second, state->output, value);
    }
}
