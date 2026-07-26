#include "types.h"

extern u8 Data_0809e9f0[];

u8 *Func_08091560(u32 arg0) {
    u8 *entry = Data_0809e9f0;
    u32 index = 0;
    if (*(u16 *)entry != arg0) {
        do {
            index += 1;
            entry += 4;
            if (index > 0x81)
                break;
        } while (*(u16 *)entry != arg0);
    }
    return entry;
}
