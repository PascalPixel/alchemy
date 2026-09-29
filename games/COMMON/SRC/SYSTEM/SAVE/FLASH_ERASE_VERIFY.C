#include "FLASH.H"

extern u8 *gFlash;

s32 CountRemainingErasedFlashBytes(u8 *ptr)
{
    s32 remaining = *(s32 *)(gFlash + 4);

    while (remaining != 0) {
        u8 byte = *ptr;
        ptr++;
        if (byte != 0xff)
            break;
        remaining--;
    }
    return remaining;
}
