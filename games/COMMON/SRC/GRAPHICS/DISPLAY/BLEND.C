#include "TYPES.H"
#include "FIXED_MATH.H"

extern u8 gBlendFramesLeft;
extern volatile u8 gBlendTargetLevel;
extern u8 gBlendStartLevel;
extern volatile u8 gBlendDuration;
extern u8 gBlendBrighten;
extern u16 gBlendLayers;

void Blend_SetDarkenTarget16(s32 duration)
{
    gBlendBrighten = 0;
    gBlendLayers = 0x3e;
    gBlendStartLevel = gBlendTargetLevel;
    gBlendTargetLevel = 0x10;
    gBlendDuration = duration;
    gBlendFramesLeft = gBlendDuration;
}

void Blend_SetDarkenTarget0(s32 duration)
{
    gBlendBrighten = 0;
    gBlendLayers = 0x3e;
    gBlendStartLevel = gBlendTargetLevel;
    gBlendTargetLevel = 0;
    gBlendDuration = duration;
    gBlendFramesLeft = gBlendDuration;
}

void Blend_SetBrightenTarget16(s32 duration)
{
    gBlendBrighten = 1;
    gBlendLayers = 0x3e;
    gBlendStartLevel = gBlendTargetLevel;
    gBlendTargetLevel = 0x10;
    gBlendDuration = duration;
    gBlendFramesLeft = gBlendDuration;
}

void Blend_SetBrightenTarget0(s32 duration)
{
    gBlendBrighten = 1;
    gBlendLayers = 0x3e;
    gBlendStartLevel = gBlendTargetLevel;
    gBlendTargetLevel = 0;
    gBlendDuration = duration;
    gBlendFramesLeft = gBlendDuration;
}

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

void Blend_WaitForTransition(void)
{
    if (gBlendDuration != 0) {
        do {
            WaitFrames(1);
        } while (gBlendDuration != 0);
    }
}
