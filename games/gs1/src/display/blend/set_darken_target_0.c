#include "types.h"
#include "scene.h"

extern u8 gIw;
extern u16 gIw2;
extern volatile u8 gIw3;
extern u8 gIw4;
extern volatile u8 gIw5;
extern u8 gIw6;

void Blend_SetDarkenTarget0(s32 duration)
{
    gIw = 0;
    gIw2 = 0x3e;
    gIw4 = gIw3;
    gIw3 = 0;
    gIw5 = duration;
    gIw6 = gIw5;
}
