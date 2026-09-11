#include "types.h"
#include "scene.h"

extern u8 gIw;
extern volatile u8 gIw2;
extern u8 gIw3;
extern volatile u8 gIw4;
extern u8 gIw5;
extern u16 gIw6;

void BlendTransition_Update(void)
{
    if (gIw4 != 0) {
        {
            volatile u16 *blend_control;
            u32 control;

            if (gIw5 != 0) {
                control = gIw6 | 0x80;
                blend_control = (volatile u16 *)0x04000050;
            } else {
                control = gIw6 | 0xc0;
                blend_control = (volatile u16 *)0x04000050;
            }
            *blend_control = control;
        }
        {
            u8 *remaining = &gIw;
            s32 delta;
            s32 level;
            s32 step;

            (*remaining)--;
            level = gIw2;
            delta = gIw3 - gIw2;
            step = *remaining;
            level += Sys_Apply(delta *step, gIw4);
            *(volatile u16 *)0x04000054 = level;
            if (*remaining == 0)
                gIw4 = 0;
        }
    }
}
