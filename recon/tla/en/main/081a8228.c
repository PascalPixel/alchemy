#include "TYPES.H"
extern u8 Data_03001ed0[];

s32 Unnamed_080f3078(s32, void *, void *, s32);
void Graphics_InterpolatePaletteBuffers(s16 *, s16 *, s16 *, s32);

struct PaletteInterpolationState {
    u8 unknown_0000[0x400];
    s16 first[0x600];
    s16 second[0x600];
    s16 output[0xa00];
    u8 unknown_3000;
    s8 value;
    s8 zero;
};

void Graphics_UpdatePaletteInterpolation(s32 value)
{
    struct PaletteInterpolationState *state =
        *(struct PaletteInterpolationState **)((u32)&Data_03001ed0);

    if (state != NULL) {
        state->value = value;
        state->zero = 0;
        Graphics_InterpolatePaletteBuffers(state->first, state->second, state->output, value);
    }
}
