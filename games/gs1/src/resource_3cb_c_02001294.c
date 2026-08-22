#include "types.h"

extern s32 Func_020029b8(s32, s32);
extern void Func_02002a4a(s32, s32, s32, s32, s32, s32);
extern s32 Func_020029ca(s32, s32);
extern s32 Func_02002a56(void);

s32 Func_02001294(s32 value)
{
    s32 column;

    if (value > 999) {
        value = 999;
    }

    for (column = 0; column <= 2; column++) {
        s32 digit = Func_020029b8(value, 10);

        Func_02002a4a(27, digit, 16 - column, 8, 1, 1);
        value = Func_020029ca(value, 10);
    }

    return Func_02002a56();
}
