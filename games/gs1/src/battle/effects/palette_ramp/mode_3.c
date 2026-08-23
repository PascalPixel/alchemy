#include "types.h"

#define RunPaletteRampEffect Func_080d9ae8
#define RunPaletteRampEffectMode3 Func_080d9adc

s32 RunPaletteRampEffect(s32, s32);

void RunPaletteRampEffectMode3(s32 effect) {
    RunPaletteRampEffect(effect, 3);
}
