#include "types.h"

#define RunPaletteRampEffectMode2 Func_080d9ad0
#define RunPaletteRampEffect Func_080d9ae8
s32 RunPaletteRampEffect(s32, s32);

void RunPaletteRampEffectMode2(s32 effect) {
    RunPaletteRampEffect(effect, 2);
}
