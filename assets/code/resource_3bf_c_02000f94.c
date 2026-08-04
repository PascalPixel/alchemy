#include "types.h"

extern s32 Value_00000953;

s32 Func_020065a2(s32);
void Func_020065aa(s32, s32);

void Func_02000f94(void)
{
    if (Func_020065a2(0xea) == -1) {
        Func_020065aa((s32)&Value_00000953, 1);
    }
}
