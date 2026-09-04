#include "types.h"

#define Menu_SendNodeCountList Func_0801ba34

void Func_080b50e0(u16 *, s32);

void Menu_SendNodeCountList(u8 *arg0)
{
    u16 data[6];
    u8 *node = *(u8 **)(arg0 + 0x348);
    s32 count = 0;

    while (node != 0) {
        node = *(u8 **)(node + 4);
        count++;
    }
    data[count] = 0xff;
    Func_080b50e0(data, 0);
}
