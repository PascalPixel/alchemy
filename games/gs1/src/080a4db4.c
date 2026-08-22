#include "types.h"

extern u8 Data_080af224;
extern u8 Data_080af228;

void Func_080150b0(s32, s32, s32, s32, s32);
void Func_08015098(u8 *, s32, s32, s32);

void Func_080a4db4(s32 value, s32 unused, s32 destination, s32 x, s32 y)
{
    s32 digits;
    s32 magnitude;

    Func_080150b0(value, 3, destination, x, y);
    digits = 1;
    magnitude = value;
    if (value < 0) {
        magnitude = -value;
    }
    if (magnitude > 9) {
        digits = 2;
    }
    magnitude = value;
    if (value < 0) {
        magnitude = -value;
    }
    if (magnitude > 99) {
        digits = 3;
    }
    if (value > 0) {
        Func_08015098(&Data_080af224, destination, x - digits * 8 + 16, y);
    } else {
        Func_08015098(&Data_080af228, destination, x - digits * 8 + 16, y);
    }
}
