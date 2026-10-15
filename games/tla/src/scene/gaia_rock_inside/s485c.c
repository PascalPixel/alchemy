#include "types.h"

extern u8 gData_020023c4[];
void Func_02004f6c(s32 value);
void Func_02005014(s32 value);

void Func_0200485c(s32 direction)
{
    s32 *state = (s32 *)(gData_020023c4 + 0x288);
    u8 *row = (u8 *)state + *state * 8;
    s32 next = row[((direction - 1) & 3) + 6];

    switch (next) {
    case 250:
    case 251:
        Func_02004f6c(next);
        break;
    case 0:
        next = 1;
    default:
        *state = next;
        Func_02004f6c(direction);
        break;
    }
    Func_02005014(123);
}
