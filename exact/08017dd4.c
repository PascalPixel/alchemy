#include "types.h"

s32 Func_080022ec(s32 dividend, s32 divisor);
s32 Func_080022fc(s32 dividend, s32 divisor);

u8 *Func_08017dd4(u8 *buffer, s32 input, s32 width)
{
    s32 offset;
    s32 value;
    s32 negative;
    s32 space;
    s32 minus;
    u8 *leading;
    u8 *trim;

    value = input;
    negative = 0;
    if (value < 0) {
        if (width == 0)
            negative = 1;
        value = (s32)(0U - (u32)value);
    }

    buffer[0] = ' ';
    for (offset = 12; offset != 0; offset--) {
        buffer[offset] = Func_080022fc(value, 10) + '0';
        value = Func_080022ec(value, 10);
    }

    offset = 0;
    buffer[13] = offset;
    space = ' ';
    offset = 1;
    minus = '-';
    for (leading = buffer; offset != 13; leading++, offset++) {
        if (leading[1] == '0') {
            if (offset != 12)
                leading[1] = space;
        } else {
            if (negative != 0)
                leading[0] = minus;
            break;
        }
    }

    if (width == 0) {
        offset = 0;
        if (buffer[0] == ' ') {
            trim = buffer;
            do {
                offset++;
                if (offset == 12)
                    break;
                trim++;
            } while (*trim == ' ');
        }
        return buffer + offset;
    }

    if ((u32)width > 12)
        width = 12;
    return buffer + 13 - width;
}
