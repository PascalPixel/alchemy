#include "types.h"

#define FieldScene_RunGuardedRectStep Func_02001f28
void Func_02006dc2(void);
s32 Func_02002bfa(void);
void Func_02006d7e(s32, s32, s32, s32, s32, s32);
void Func_020022c2(void);
void Func_02006dee(void);
void Func_02003c9e(void);

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
