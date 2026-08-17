#include "types.h"

extern void Func_020052fc(void);
extern s32 Func_020052e2(s32);
extern void Func_0200538c(s32);
extern void Func_02005394(s32);
extern void Func_020053b6(s32, s32, s32);
extern void Func_020053ca(s32, s32, s32);
extern void Func_02005336(void);

void Func_02002924(void)
{
    Func_020052fc();
    if (Func_020052e2(0x896) != 0)
        Func_0200538c(0xffd);
    else
        Func_02005394(0xfff);
    Func_020053b6(16, 0, 10);
    Func_020053ca(16, 0xc000, 10);
    Func_02005336();
}
