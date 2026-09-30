#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "FIXED_MATH.H"

extern u8 gBlendFramesLeft;
extern volatile u8 gBlendTargetLevel;
extern u8 gBlendStartLevel;
extern volatile u8 gBlendDuration;
extern u8 gBlendBrighten;
extern u16 gBlendLayers;
s32 WaitFrames(s32);

extern u8 IwramClearWords[];
extern u8 gObjAffineCount[];
extern u8 Data_03001400[];

/*
 * _call_via_r3 names a bx rN veneer slot, so this is an indirect call
 * through the register loaded just before it -- the relocated routine at
 * 0x03000164. Its argument count is not established.
 */
s32 _call_via_r3(s32, s32, s32, s32);

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

void Blend_WaitForTransition(void)
{
    if (gBlendDuration != 0) {
        do {
            WaitFrames(1);
        } while (gBlendDuration != 0);
    }
}

void Graphics_ResetFrameState(void)
{
    *(s8 *)((u32)&gObjAffineCount) = 0;
    _call_via_r3(((u32)&Data_03001400), 0x400, ((u32)&gObjAffineCount), (u32)IwramClearWords);
}
