#include "types.h"

extern void Func_02007106(void);
extern s32 Func_02002f3e(void);
extern void Func_020070c2(s32, s32, s32, s32, s32, s32);
extern void Func_02002606(void);
extern void Func_02007132(void);
extern void Func_020041fa(void);

void Func_0200226c(void)
{
    Func_02007106();
    if (Func_02002f3e() == 0) {
        s32 k5 = 44, k6 = 39;
        Func_020070c2(108, 39, 13, 7, k5, k6);
        Func_02002606();
    }
    Func_02007132();
    Func_020041fa();
}
