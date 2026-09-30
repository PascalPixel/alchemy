#include "TYPES.H"
#include "FIXED_MATH.H"

extern u8 gBlendFramesLeft;
extern volatile u8 gBlendTargetLevel;
extern u8 gBlendStartLevel;
extern volatile u8 gBlendDuration;
extern u8 gBlendBrighten;
extern u16 gBlendLayers;

void Blend_SetDarkenTarget0(s32 duration)
{
    gBlendBrighten = 0;
    gBlendLayers = 0x3e;
    gBlendStartLevel = gBlendTargetLevel;
    gBlendTargetLevel = 0;
    gBlendDuration = duration;
    gBlendFramesLeft = gBlendDuration;
}
