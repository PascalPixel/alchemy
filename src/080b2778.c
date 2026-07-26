#include "types.h"

u8 *Func_08077008(void);

s32 Func_080b2778(s32 unused, s32 arg1) {
    u8 value = Func_08077008()[0xF];
    s32 result = 0;
    if (arg1 == 0) {
        result = value * 20;
    } else if (arg1 == 1) {
        result = 10;
    } else if (arg1 == 2) {
        result = 50;
    } else if (arg1 == 3) {
        result = value * 10;
    }
    return result;
}
