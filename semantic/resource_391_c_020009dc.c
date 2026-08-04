/* Clear the pending object cell after restoring its mode. */
#include "types.h"
void Func_08009080(u8 *object, s32 mode);
s32 Func_020009dc(u8 *object)
{
    u8 **pending = (u8 **)0x0200b390;
    if (*pending) {
        Func_08009080(object, 2);
        *pending = 0;
    }
    return 1;
}
