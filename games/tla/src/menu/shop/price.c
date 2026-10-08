#include "types.h"

u16 *Item_Get(s32 item);

s32 Shop_GetItemPrice(s32 item)
{
    s32 price = *Item_Get(item) >> 2;

    if ((item & 0x400) == 0)
        price = 0;
    return price;
}

void Func_08109ad8();

void Func_0810a2c8(s32 a, s32 b, s32 c, s32 d)
{
    Func_08109ad8(a, b, c, d);
}

void Func_08109cac();

void Func_0810a2d0(s32 a, s32 b, s32 c, s32 d)
{
    Func_08109cac(a, b, c, d);
}
