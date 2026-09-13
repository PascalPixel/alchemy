#include "flash.h"

s32 RunFlashEraseVerifier(u8 *ptr, s32 (*verify)(u8 *))
{
    if (verify(ptr) == 0)
        return 0;
    return 0x8004;
}
