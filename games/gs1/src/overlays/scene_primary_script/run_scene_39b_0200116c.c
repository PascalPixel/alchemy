#include "types.h"

#define FieldScene_RunScene39b_0200116c Func_0200116c

s32 Func_0200365e();
void Func_02003648();
void Func_0200368c();
void Func_02003696();

/* A value-returning call sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void FieldScene_RunScene39b_0200116c(void)
{
    s32 record;
    s32 field8;
    s32 quotient;

    record = Value1(Func_0200365e, 0);
    field8 = *(s32 *)(record + 8);
    quotient = field8 / 0x100000;
    Func_02003648(0x205);
    if (quotient == 7) {
        Func_0200368c(8, 0x200a874);
    } else {
        Func_02003696(8, 0x200a8c8);
    }
}
