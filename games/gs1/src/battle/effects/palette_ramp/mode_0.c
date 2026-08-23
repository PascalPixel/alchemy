#include "types.h"

#define RunPaletteRampEffect Func_080d9ae8
#define RunPaletteRampEffectMode0 Func_080d9ab8

s32 RunPaletteRampEffect(s32, s32);

void RunPaletteRampEffectMode0(s32 effect) {
    RunPaletteRampEffect(effect, 0);
}
