#include "item.h"

extern u8 Data_0807b490[];

s32 Item_AdjustCounter(s32 item_id, s32 delta)
{
    s32 mask = 0x1ff;
    u8 counter;
    s32 result = 0;

    counter = Data_0807b490[item_id & mask];
    if (counter != 0) {
        result = ItemCounter_Adjust(counter - 1, delta);
    }
    return result;
}
