#include "TYPES.H"
#include "FIXED_MATH.H"

extern u8 gBlendFramesLeft;
extern volatile u8 gBlendTargetLevel;
extern u8 gBlendStartLevel;
extern volatile u8 gBlendDuration;
extern u8 gBlendBrighten;
extern u16 gBlendLayers;

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
