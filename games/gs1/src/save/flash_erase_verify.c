#include "flash.h"

extern u8 *Data_02004c08;

s32 CountRemainingErasedFlashBytes(u8 *ptr)
{
    s32 remaining = *(s32 *)(Data_02004c08 + 4);

    while (remaining != 0) {
        u8 byte = *ptr;
        ptr++;
        if (byte != 0xff)
            break;
        remaining--;
    }
    return remaining;
}
