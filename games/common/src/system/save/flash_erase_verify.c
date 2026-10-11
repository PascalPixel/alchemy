#include "../../../include/system/flash.h"
#include "../../../include/system/flash_data.h"

s32 CountRemainingErasedFlashBytes(u8 *ptr)
{
    s32 remaining;

    for (remaining = gFlash->sector.size; remaining != 0; remaining--) {
        if (*ptr++ != 0xff)
            break;
    }
    return remaining;
}
