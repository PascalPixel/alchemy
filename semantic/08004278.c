#include "types.h"

s32 Func_08004278(u32 value)
{
    s32 result = -1;
    u8 *entry = (u8 *)0x03001a20;
    volatile u16 *interruptMaster = (volatile u16 *)0x04000208;
    u32 savedInterruptMaster = *interruptMaster;

    *interruptMaster = (u16)(u32)interruptMaster;
    {
        s32 index = 0;

        if (*(u32 *)entry == value) {
            result = 0;
        } else {
            do {
                index++;
                entry += 8;
                if (index > 19) {
                    break;
                }
            } while (*(u32 *)entry != value);
            if (index <= 19) {
                result = index;
            }
        }
        if (result != -1) {
            *(u32 *)entry = 0;
            *(u16 *)(entry + 4) = 0x7fff;
        }
    }
    *(volatile u16 *)0x04000208 = savedInterruptMaster;
    return result;
}
