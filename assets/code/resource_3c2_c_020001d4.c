#include "types.h"

extern u8 Value_00002624;
extern void Func_02000da2(s32);
extern s32 Func_02000db2(s32, s32);
extern s32 Func_02000d4a(s32, s32);
extern void Func_02000d3c(s32);
extern void Func_02000dc2(s32);
extern void Func_02000dca(s32);
extern void Func_02000de2(s32, s32);

void Func_020001d4(s32 object)
{
    s32 cue = (s32) &Value_00002624;
    Func_02000da2(cue);
    Func_02000db2(object, 0);
    if (Func_02000d4a(0, 0) == 0) {
        Func_02000d3c(10);
        Func_02000dc2(cue + 1);
    } else {
        Func_02000dca(cue + 2);
    }
    Func_02000de2(object, 0);
}
