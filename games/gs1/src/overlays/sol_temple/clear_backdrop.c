#include "types.h"

#define ClearSolTempleBackdrop Func_0200022c

void ClearSolTempleBackdrop(void)
{
    s32 black = 0;
    u16 *backdrop_color = (u16 *)0x5000000;
    *backdrop_color = black;
}
