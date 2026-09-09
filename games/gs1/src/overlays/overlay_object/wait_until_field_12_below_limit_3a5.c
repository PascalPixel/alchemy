/*
 * Waits for an object field to fall to a limit -- resource_3a5.
 */
#include "types.h"

extern void Func_02001fc0();
/*
 * The countdown is tested at the top of the loop and decremented inside the
 * body, after the call.  A post-decrement test would move the subtract ahead
 * of the call.
 */
void OverlayObject_WaitUntilField12BelowLimit(u8 *o, s32 limit)
{
    s32 frames = 60;

    while (frames != 0) {
        Func_02001fc0(1);
        frames--;
        if (*(s32 *)(o + 12) <= limit) break;
    }
}
