#include "types.h"

#define FieldScene_RunGuardedRectStep Func_02001f28

extern void Func_02006dc2(void);
extern s32 Func_02002bfa(void);
extern void Func_02006d7e(s32, s32, s32, s32, s32, s32);
extern void Func_020022c2(void);
extern void Func_02006dee(void);
extern void Func_02003c9e(void);

void FieldScene_RunGuardedRectStep(void)
{
    s32 x;
    s32 y;

    Func_02006dc2();
    if (Func_02002bfa() == 0) {
        x = 45;
        y = 43;
        Func_02006d7e(109, 43, 7, 5, x, y);
        Func_020022c2();
    }
    Func_02006dee();
    Func_02003c9e();
}
