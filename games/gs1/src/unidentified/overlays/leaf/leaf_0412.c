#include "types.h"

extern s32 Func_02002878(void);
extern void Func_0200081c(void);
extern void Func_0200083e(void);

void Func_02000424(void)
{
    if (Func_02002878() != 0) {
        Func_0200081c();
    } else {
        Func_0200083e();
    }
}
