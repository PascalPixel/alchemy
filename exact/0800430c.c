#include "types.h"

s32 Func_0800430c(void) {
    s32 result = -1;
    u8 *entry = (u8 *)0x03001A20;
    volatile u16 *interruptMaster = (volatile u16 *)0x04000208;
    u32 savedInterruptMaster = *interruptMaster;

    *interruptMaster = (u16)(u32)interruptMaster;
    {
        s32 index = 0;
        do {
            if (entry[3] == 2) {
                u8 flag = entry[6] & 1;
                if (flag == 0) {
                    entry[5] |= flag + 1;
                    result = index;
                }
            }
            index++;
            entry += 8;
        } while (index <= 19);
    }
    *(volatile u16 *)0x04000208 = savedInterruptMaster;
    return result;
}
