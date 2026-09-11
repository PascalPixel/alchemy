#include "types.h"
#include "scene.h"

extern u8 gIw;
extern u16 gIw2;
extern volatile u8 gIw3;
extern u8 gIw4;
extern volatile u8 gIw5;
extern u8 gIw6;

void Blend_ConfigureTransition(s8 mode, s32 coefficient, u32 start, s32 target, s32 duration)
{
    gIw = mode;
    gIw2 = coefficient & 0x3f;
    if (start > 0x10U) {
        gIw4 = gIw3;
    } else {
        gIw4 = start;
    }
    gIw3 = target;
    gIw6 = (gIw5 = duration);
}
