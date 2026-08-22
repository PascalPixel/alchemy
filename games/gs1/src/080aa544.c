#include "types.h"

extern u8 *Data_03001f2c;

s32 Func_080aa544(s32 arg0) {
    u8 *current;
    s32 value;
    s32 count;
    current = Data_03001f2c + 0x134;
    arg0 += 0x3D;
    value = 0x20;
    count = 3;
    do {
        count--;
        *(s16 *)current = value;
        *(s16 *)(current + 0x10) = arg0;
        value += 0x38;
        current += 2;
    } while (count >= 0);
    return arg0;
}
