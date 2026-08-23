#include "types.h"

#define RunPaletteRampEffect Func_080d9ae8
#define RunPaletteRampEffectMode2 Func_080d9ad0

s32 RunPaletteRampEffect(s32, s32);

void RunPaletteRampEffectMode2(s32 effect) {
    RunPaletteRampEffect(effect, 2);
}
