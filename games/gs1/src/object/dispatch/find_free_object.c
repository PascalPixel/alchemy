#include "types.h"

extern u8 *Data_03001e64;

void *ObjectDispatch_FindFreeObject(void)
{
    u8 *entry = Data_03001e64;
    void *ret = 0;
    s32 index = 0;

    while (index <= 63) {
        if (*(u32 *)entry == 0) {
            ret = entry;
            break;
        }
        index++;
        entry += 112;
    }
    return ret;
}
