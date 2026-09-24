#include "TYPES.H"
#include "GLOBAL_CELLS.H"

extern u8 gBlendFramesLeft;
extern volatile u8 gBlendTargetLevel;
extern u8 gBlendStartLevel;
extern volatile u8 gBlendDuration;
extern u8 gBlendBrighten;
extern u16 gBlendLayers;

s32 Math_Div(s32 numerator, s32 denominator);

void BlendTransition_Update(void)
{
    if (gBlendDuration != 0) {
        {
            volatile u16 *blend_control;
            u32 control;

            if (gBlendBrighten != 0) {
                control = gBlendLayers | 0x80;
                blend_control = (volatile u16 *)0x04000050;
            } else {
                control = gBlendLayers | 0xc0;
                blend_control = (volatile u16 *)0x04000050;
            }
            *blend_control = control;
        }
        {
            u8 *remaining = &gBlendFramesLeft;
            s32 delta;
            s32 level;
            s32 step;

            (*remaining)--;
            level = gBlendTargetLevel;
            delta = gBlendStartLevel - gBlendTargetLevel;
            step = *remaining;
            level += Math_Div(delta * step, gBlendDuration);
            *(volatile u16 *)0x04000054 = level;
            if (*remaining == 0)
                gBlendDuration = 0;
        }
    }
}



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

s32 WaitFrames(s32);

void Blend_WaitForTransition(void)
{
    if (*(u8 *)ADDR_03001C98 != 0) {
        do {
            WaitFrames(1);
        } while (*(u8 *)ADDR_03001C98 != 0);
    }
}
