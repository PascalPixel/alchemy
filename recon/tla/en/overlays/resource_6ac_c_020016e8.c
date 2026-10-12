/* Near miss, differ 13 of 15: the listing loads 0xffff from a literal pool placed after the
 * loop with a branch over it and compares the halfword with it directly; this draft builds 0xffff by
 * shift and add and keeps it in ip. A (u16) cast of a symbol address reproduces the branch over the
 * pool but still copies the value to ip. */
#include "../../../../games/tla/src/field/overlay_6ac/ov6ac.h"

s32 Func_020016e8(u16 *list)
{
    s32 count = 0;

    while (*list != 0xffff) {
        list++;
        count++;
    }
    return count - 1;
}
