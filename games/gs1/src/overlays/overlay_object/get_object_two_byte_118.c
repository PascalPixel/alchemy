#include "types.h"

#define OverlayObject_GetObjectTwoByte118 Func_02002f84

extern u8 *Func_02007c8e(s32);

s32 OverlayObject_GetObjectTwoByte118(void)
{
    return Func_02007c8e(2)[0x118];
}
