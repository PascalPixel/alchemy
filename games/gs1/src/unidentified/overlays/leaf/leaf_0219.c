#include "types.h"

extern s32 Data_03001e40;
extern void Func_0200bba8(s32, s32);
extern void Func_0200bbb2(s32, s32);
extern void Func_0200b9b8(s32);

void Func_02005c20(s32 p)
{
    if ((Data_03001e40 & 2) != 0)
        Func_0200bba8(p, 7);
    else
        Func_0200bbb2(p, 0);
    if ((Data_03001e40 & 0xf) == 0)
        Func_0200b9b8(p);
}
