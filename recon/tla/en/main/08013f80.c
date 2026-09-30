#include "TYPES.H"
#include "FIXED_MATH.H"

extern u8 gBlendFramesLeft;
extern volatile u8 gBlendTargetLevel;
extern u8 gBlendStartLevel;
extern volatile u8 gBlendDuration;
extern u8 gBlendBrighten;
extern u16 gBlendLayers;

void Blend_ConfigureTransition(s8 mode, s32 coefficient, u32 start, s32 target, s32 duration)
{
    gBlendBrighten = mode;
    gBlendLayers = coefficient & 0x3f;
    if (start > 0x10U) {
        gBlendStartLevel = gBlendTargetLevel;
    } else {
        gBlendStartLevel = start;
    }
    gBlendTargetLevel = target;
    gBlendFramesLeft = (gBlendDuration = duration);
}
