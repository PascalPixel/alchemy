#include "types.h"

void Func_080d20a4(s32 x, s32 y, u32 corner)
{
    u16 *cell = (u16 *)(0x06000200 + ((y * 26 + x) << 5));

    cell += corner >> 2;

    switch (corner & 3) {
    case 0:
        *cell &= 0xfff0;
        break;
    case 1:
        *cell &= 0xff00;
        break;
    case 2:
        *cell &= 0xf000;
        break;
    case 3:
        *cell &= 0;
        break;
    }
}
