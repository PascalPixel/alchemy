#include "types.h"

#define RunPaletteRampEffect Func_080d9ae8
#define RunPaletteRampEffectMode1 Func_080d9ac4

s32 RunPaletteRampEffect(s32, s32);

void RunPaletteRampEffectMode1(s32 effect) {
    RunPaletteRampEffect(effect, 1);
}
