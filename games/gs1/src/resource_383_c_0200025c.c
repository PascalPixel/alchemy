#include "types.h"

extern void Func_02004fa4(void);
extern s32 Func_02004f98(void);
extern s32 Func_0200504c(s32, s32);
extern void Func_02004fb2(s32);
extern void Func_02005090(s32);
extern void Func_020050a8(s32, s32);
extern void Func_0200514a(s32, s32);
extern void Func_02005002(s32, s32);
extern void Func_02004fe6(void);

void Func_0200025c(void)
{
    Func_02004fa4();
    if (Func_02004f98() == 0) {
        Func_0200504c(18, 4);
        Func_02004fb2(20);
        Func_02005090(0x1384);
        Func_020050a8(18, 0);
    } else {
        Func_0200514a(231, 3);
        Func_02005002(231, 0);
    }
    Func_02004fe6();
}
