/* Clear the pending object cell after restoring its mode. */
#include "types.h"

#define OverlayObject_ClearPendingAndRestoreMode Func_020009dc
void Func_0200358e(u8 *object, s32 mode);
s32 OverlayObject_ClearPendingAndRestoreMode(u8 *object)
{
    u8 **pending = (u8 **)0x0200b390;
    if (*pending) {
        Func_0200358e(object, 2);
        *pending = 0;
    }
    return 1;
}
