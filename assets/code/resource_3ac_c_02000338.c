#include "resource_3ac.h"

typedef signed int s32;
extern s32 Func_020007d6(s32);
extern void Func_02000810(s32);
extern void Func_02000820();
extern void Func_02000830(s32, s32);
void Func_02000338(void)
{
    if (Func_020007d6(0x941) != 0) {
        Func_02000810(0x24f8);
        Func_02000820(15, 0);
    } else {
        Func_02000820(0x1bdf);
        Func_02000830(15, 0);
    }
}
