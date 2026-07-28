#include "types.h"

s16 *Func_08077018(s32 flag_no);

s32 Func_080b20e8(s32 flag_no)
{
    s32 result = *Func_08077018(flag_no) / 4;

    if ((flag_no & 0x400) == 0) {
        result = 0;
    }
    return result;
}
