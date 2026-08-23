#include "shop.h"

s32 Shop_ComputeSalePrice(s32 item_id)
{
    s32 price;

    price = Item_Get(item_id)->price;

    if (Item_Get(item_id)->flags & 8) {
        price = 0;
    } else if (item_id & 0x400) {
        price = price / 2;
    } else {
        price = price * 3 / 4;
    }
    return price;
}
