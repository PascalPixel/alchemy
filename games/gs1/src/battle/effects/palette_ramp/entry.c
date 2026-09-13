#include "types.h"

s32 RunPaletteRampEffect(s32, s32);

void BattleFx_RunPaletteRampMode1(s32 effect)
{
    RunPaletteRampEffect(effect, 1);
}

void BattleFx_RunPaletteRampMode2(s32 effect)
{
    RunPaletteRampEffect(effect, 2);
}

void BattleFx_RunPaletteRampMode3(s32 effect)
{
    RunPaletteRampEffect(effect, 3);
}
