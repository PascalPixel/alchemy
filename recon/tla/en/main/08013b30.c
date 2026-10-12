#include "types.h"

#define SOUND_PRESET_COUNT 5

s32 Sound_LoadPresetParameters(s32 preset)
{
    if ((u32)preset > SOUND_PRESET_COUNT - 1)
        preset = 0;
    return 0;
}

void Func_08013b38(void)
{
}

void Func_08013b3c(void)
{
}
