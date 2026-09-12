#include "item.h"

extern u8 Data_02000380[128];

s32 ItemCounter_Adjust(s32 index, s32 delta)
{
    s32 counter_slot = index;
    u8 *data = Data_02000380;

    index = 0;
    if (counter_slot <= 127) {
        s32 value = data[counter_slot];

        value += delta;

        if (value < 0) {
            value = 0;
        } else if (value > 99) {
            value = 99;
            index = 99;
        } else {
            index = value;
        }
        data[counter_slot] = value;
    }
    return index;
}
