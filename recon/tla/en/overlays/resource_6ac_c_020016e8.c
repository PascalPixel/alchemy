/* Near miss, differ 10 of 15 (13 with the pointer walk): the listing loads 0xffff from a literal pool placed after the
 * loop with a branch over it and compares the halfword with it directly; this draft builds 0xffff by
 * shift and add and keeps it in ip. A (u16) cast of a symbol address reproduces the branch over the
 * pool but still copies the value to ip. Also tried: list[count] indexing (strength-reduced to the
 * native pointer walk, differ 10 with count-- then return), s16 with -1, (u16)-1, a for loop. agscc
 * splits SImode constants after reload, so the pool word suggests a constant that never reached a
 * low-register SImode set (HImode or a symbol). */
#include "../../../../games/tla/src/field/overlay_6ac/ov6ac.h"

s32 Func_020016e8(u16 *list)
{
    s32 count;

    for (count = 0; list[count] != 0xffff; count++)
        ;
    count--;
    return count;
}
