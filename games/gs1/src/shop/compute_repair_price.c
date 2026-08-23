#include "shop.h"

s32 Shop_ComputeRepairPrice(s32 item_id)
{
    s32 result = Item_Get(item_id)->price / 4;

    if ((item_id & 0x400) == 0) {
        result = 0;
    }
    return result;
}
