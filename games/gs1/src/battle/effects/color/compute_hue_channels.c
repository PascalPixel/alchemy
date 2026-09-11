#include "types.h"
#include "scene.h"
#include "abi/battle/effects/color/compute_hue_channels.h"

void BattleFx_ComputeHueChannels(s32 value, s32 *maximum, s32 *center, s32 *minimum)
{
    *maximum = Battle_Place(value + 0x780000, 0, 0x1F0000);
    *center = Battle_Place(value, 0, 0x1F0000);
    *minimum = Battle_Place(value + 0xFF880000, 0, 0x1F0000);
}
