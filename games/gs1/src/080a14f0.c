#include "types.h"

s32 Func_080022ec(s32 value, s32 divisor);
void Func_080150a8(s32 value, s32 digits, s32 layer, s32 x, s32 y);

void Func_080a14f0(s32 number, s32 layer, s32 x, s32 y)
{
    s32 value = number;
    s32 digits = 1;

    while (digits <= 15) {
        value = Func_080022ec(value, 10);
        if (value <= 9) {
            break;
        }
        digits++;
    }

    digits++;
    x -= digits << 3;
    Func_080150a8(number, digits, layer, x, y);
}
