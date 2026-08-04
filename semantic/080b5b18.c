#include "types.h"

s32 Func_080b6a60(u16 *);
u8 *Func_08077008(s32);
void Func_08077010(s32);

void Func_080b5b18(void)
{
    u16 ids[10];
    s32 count;
    s32 index;
    s32 zero;
    u8 clear;

    count = Func_080b6a60(ids);
    index = 0;
    if (index < count) {
        zero = 0;
        clear = 0;
        do {
            u8 *object = Func_08077008(ids[index]);
            s32 field = 3;
            u8 *cursor = object + 0x12f;

            do {
                *cursor-- = clear;
                field--;
            } while (field >= 0);

            object[0x132] = zero;
            object[0x133] = zero;
            object[0x134] = zero;
            object[0x135] = zero;
            object[0x136] = zero;
            object[0x137] = zero;
            object[0x138] = zero;
            object[0x139] = zero;
            object[0x13a] = zero;
            object[0x13b] = zero;
            object[0x13c] = zero;
            object[0x13d] = zero;
            object[0x13e] = zero;
            object[0x13f] = zero;
            object[0x141] = zero;
            object[0x142] = zero;
            object[0x143] = zero;
            object[0x144] = zero;
            object[0x145] = zero;
            object[0x146] = zero;
            object[0x147] = zero;
            object[0x148] = zero;

            Func_08077010(ids[index]);
            index++;
        } while (index < count);
    }
}
