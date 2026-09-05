#include "types.h"

extern u8 *Func_02007c8e(s32);

s32 OverlayObject_GetObjectTwoByte118(void)
{
    return Func_02007c8e(2)[0x118];
}
