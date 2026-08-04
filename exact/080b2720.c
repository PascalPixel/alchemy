#include "types.h"

extern s16 Data_080b41ac[][33];

s32 Func_080b2720(s32 row_no, u16 *output)
{
    s16 *src;
    u16 *dst;
    s32 count;

    count = 0;
    if (Data_080b41ac[row_no][0] != 0) {
        dst = output;
        src = Data_080b41ac[row_no];
        do {
            *dst = *src;
            count++;
            src++;
            dst++;
        } while (count <= 23 && *src != 0);
    }
    output[count] = 0;
    return count;
}
