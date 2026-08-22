#include "types.h"

extern s32 Func_0200146e(s32);
extern void Func_020004e6(void);
extern s32 Func_020006ae(void);
extern void Func_0200148c(s32);
extern void Func_020014da(s32);
extern void Func_02000726(void);

void Func_02000368(void)
{
    if (Func_0200146e(0x9A9) != 0) {
        return;
    }
    Func_020004e6();
    if (Func_020006ae() == 0) {
        return;
    }
    Func_0200148c(0x9A9);
    Func_020014da(0x50);
    Func_02000726();
}
