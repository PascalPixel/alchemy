#include "types.h"

void Func_02001610(); s32 Func_02001626(); void Func_02001696();
void Func_020016a6(); void Func_020016b6(); void Func_020016be();
void Func_020016d6();

void Func_020000c4(s32 value)
{
    register u8 *message = (u8 *)0x28be;
    Func_02001696(message);
    Func_020016a6(value, 0);
    if (Func_02001626(0, 0) == 0) {
        Func_02001610(10);
        Func_020016b6(message + 1);
    } else {
        Func_020016be(message + 2);
    }
    Func_020016d6(value, 0);
}
