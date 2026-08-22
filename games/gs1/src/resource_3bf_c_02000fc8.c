#include "types.h"

extern s32 Value_00000f2a;

s32 Func_02002008(s32, s32);
void Func_020065f2(s32);

void Func_02000fc8(void)
{
    if (Func_02002008(8, 8) != 0) {
        Func_020065f2((s32)&Value_00000f2a);
    }
}
