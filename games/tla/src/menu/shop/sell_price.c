#include "types.h"
#include "item.h"

s32 Shop_GetSellPrice(s32 item_id)
{
    s32 price;

    price = (u16)Item_Get(item_id)->price;
    if (Item_Get(item_id)->flags & 8)
        price = 0;
    else if (item_id & 0x400)
        price /= 2;
    else
        price = price * 3 / 4;
    return price;
}
