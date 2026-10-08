#include "../../../include/system/flash.h"

extern u8 *gFlash;

s32 CountRemainingErasedFlashBytes(u8 *ptr)
{
    s32 remaining;

    for (remaining = *(s32 *)(gFlash + 4); remaining != 0; remaining--) {
        if (*ptr++ != 0xff)
            break;
    }
    return remaining;
}
