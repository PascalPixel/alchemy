#include "types.h"

extern void Func_020067fe(s32, s32, s32, s32, s32, s32);
extern void Func_0200681a(s32, s32, s32, s32, s32, s32);
extern void Func_020067c8(s32 frames);

/* Publish the scene's upper prompt and lower dialogue panel. */
void Func_02003270(void)
{
    {
        s32 a5 = 3, a6 = 2;
        Func_020067fe(0, 64, 11, 68, a5, a6);
    }
    {
        s32 a5 = 11, a6 = 8;
        Func_0200681a(11, 10, 3, 2, a5, a6);
    }
    Func_020067c8(1);
}
