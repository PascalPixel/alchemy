#include "types.h"

extern s32 Func_02005a38(s32);
extern s32 Func_02005a42(s32);
extern void Func_02005ae8(s32, s32, s32);
extern s32 Func_02005a60(s32);
extern s32 Func_02005a6a(s32);
extern void Func_02005b0c(s32, s32, s32);
extern s32 Func_02005a84(s32);
extern s32 Func_02005a8e(s32);
extern void Func_02005b30(s32, s32, s32);

void Func_02001b30(void)
{
    {
        s32 x = Func_02005a38(896);
        s32 y = Func_02005a42(904);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Func_02005ae8(1, x, y);
    }
    {
        s32 x = Func_02005a60(912);
        s32 y = Func_02005a6a(920);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Func_02005b0c(2, x, y);
    }
    {
        s32 x = Func_02005a84(928);
        s32 y = Func_02005a8e(936);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Func_02005b30(3, x, y);
    }
}
