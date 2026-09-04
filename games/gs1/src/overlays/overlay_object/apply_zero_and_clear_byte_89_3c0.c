#include "types.h"

#define OverlayObject_ApplyZeroAndClearByte89 Func_02000324

void Func_02001568();

/* Complete 24-byte entity reset owner. */
s32 OverlayObject_ApplyZeroAndClearByte89(u8 *obj)
{
    Func_02001568(obj, 0);
    obj[89] = 0;
    return 0;
}
