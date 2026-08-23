#include "types.h"

extern s32 Func_02002322(void);
extern void Func_0200230a(void);
extern void Func_0200237a(s32, s32);
extern void Func_02002310(s32);
extern void Func_020023c8(s32, s32);
extern void Func_0200232c(void);

void Func_02000938(s32 x)
{
    u8 *flag = (u8 *)Func_02002322() + 91;
    s32 zero = 0;

    *flag = 1;
    Func_0200230a();
    Func_0200237a(x, 1);
    Func_02002310(2);
    Func_020023c8(x, 0);
    Func_0200232c();
    *flag = zero;
}
