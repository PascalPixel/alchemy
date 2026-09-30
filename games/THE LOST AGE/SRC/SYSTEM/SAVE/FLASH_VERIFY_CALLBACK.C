#include "../../../../COMMON/INCLUDE/SYSTEM/FLASH.H"

s32 RunFlashEraseVerifier(u8 *ptr, s32 (*verify)(u8 *))
{
    if (verify(ptr) != 0)
        return 0x8004;
    return 0;
}
