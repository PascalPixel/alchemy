#include "TYPES.H"
#include "ITEM.H"

struct ItemDefinition *Item_Get(s32 item_id);

/* What a shop pays for an item: nothing for an item flagged unsellable,
   half the price for a broken one (bit 10), otherwise three quarters. */
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
