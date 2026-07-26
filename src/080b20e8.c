#include "types.h"

s16 *Func_08077018(s32);

s32 Func_080b20e8(s32 arg0) {
    s32 result = *Func_08077018(arg0) / 4;
    if ((arg0 & 0x400) == 0) {
        result = 0;
    }
    return result;
}
