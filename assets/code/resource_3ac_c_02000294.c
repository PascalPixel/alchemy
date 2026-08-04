#include "resource_3ac.h"

#include "types.h"
extern s32 Func_02000732(s32);
extern void Func_0200076c(s32);
extern void Func_0200077c();
extern void Func_0200078c(s32, s32);
void Func_02000294(void)
{
    if (Func_02000732(0x941) != 0) {
        Func_0200076c(0x24f6);
        Func_0200077c(14, 0);
    } else {
        Func_0200077c(0x1bde);
        Func_0200078c(14, 0);
    }
}
