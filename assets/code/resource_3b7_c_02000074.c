#include "types.h"

extern u8 Value_00000e39;
extern void Func_020019e2(s32);
extern s32 Func_020019f2(s32, s32);
extern s32 Func_020019c2(s32, s32);
extern void Func_020019ac(s32);
extern void Func_02001a02(s32);
extern void Func_02001a0a(s32);
extern void Func_02001a22(s32, s32);

void Func_02000074(s32 object)
{
    s32 cue = (s32) &Value_00000e39;
    Func_020019e2(cue);
    Func_020019f2(object, 0);
    if (Func_020019c2(0, 0) == 0) {
        Func_020019ac(10);
        Func_02001a02(cue + 1);
    } else {
        Func_02001a0a(cue + 2);
    }
    Func_02001a22(object, 0);
}
