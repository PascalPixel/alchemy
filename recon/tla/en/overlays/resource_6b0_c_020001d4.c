/* Near miss (10 instructions differ): the native loads 0xffff from the
 * literal pool, as a halfword constant, and keeps it in r1 through the
 * loop; the draft builds it in three instructions and moves it to ip.
 * Tried: a u16 end variable, an indexed loop. */
#include "types.h"

s32 Func_020001d4(u16 *list)
{
    s32 count;

    for (count = 0; list[count] != 0xffff; count++)
        ;
    count--;
    return count;
}
