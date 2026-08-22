#include "types.h"

extern u8 Value_000022b9;

extern void Func_02001ae6(s32);
extern s32 Func_02001af6(s32, s32);
extern s32 Func_02001a56(s32, s32);
extern void Func_02001a40(s32);
extern void Func_02001b06(s32);
extern void Func_02001b0e(s32);
extern void Func_02001b26(s32, s32);

void Func_02000118(s32 object)
{
    s32 cue = (s32) &Value_000022b9;

    Func_02001ae6(cue);
    Func_02001af6(object, 0);
    if (Func_02001a56(0, 0) == 0) {
        Func_02001a40(10);
        Func_02001b06(cue + 1);
    } else {
        Func_02001b0e(cue + 2);
    }
    Func_02001b26(object, 0);
}
