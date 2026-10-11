/* Near miss of Func_020001d4 (15 native instructions, 10 differ): the native loads the 0xffff
 * terminator from the literal pool and compares in r1 on both tests; the draft builds it with
 * three instructions and copies it to ip for the loop. A signed list compared with -1 and casts
 * of either side measured the same or worse. */
#include "types.h"

s32 Func_020001d4(u16 *list)
{
    s32 count = 0;

    while (*list != 0xffff) {
        list++;
        count++;
    }
    count--;
    return count;
}
