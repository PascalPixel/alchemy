#include "types.h"

extern u8 *Data_03001e64;

void *Func_0800c0cc(void)
{
    u8 *entry = Data_03001e64;
    void *empty_entry = 0;
    s32 index = 0;

    while (index <= 63) {
        if (*(u32 *)entry == 0) {
            empty_entry = entry;
            break;
        }
        index++;
        entry += 112;
    }
    return empty_entry;
}
