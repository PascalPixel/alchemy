#include "inventory.h"

extern u16 Data_080b1f40[];

s32 Item_AdjustCounter(s32 item_id, s32 delta)
{
    s32 result = 0;
    u16 counter = Data_080b1f40[item_id & ITEM_ID_MASK];

    if (counter)
        result = ItemCounter_Adjust(counter - 1, delta);
    return result;
}
