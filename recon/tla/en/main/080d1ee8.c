#include "types.h"

extern const s32 Data_080f3228[];

s32 Func_080d1ee8(s32 key)
{
    const s32 *entry = Data_080f3228;
    s32 value;

    for (;;) {
        value = entry[0];
        if (value == -1)
            break;
        if (value == key) {
            value = entry[1];
            break;
        }
        entry += 2;
    }
    return value;
}
