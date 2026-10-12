/* Draft, differ 13 of 15: the native loads 0xffff from the literal pool before the loop and jumps over the pool to the count - 1 tail; this form builds 0xffff in three instructions and falls through. */
#include "types.h"

s32 Func_020001d4(u16 *list)
{
    s32 count = 0;

    while (*list != 0xffff) {
        list++;
        count++;
    }
    return count - 1;
}
