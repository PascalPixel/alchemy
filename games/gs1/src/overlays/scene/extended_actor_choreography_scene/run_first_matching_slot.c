#include "types.h"

/*
 * Actor slot search for resource_3b1.  The 48-byte owner at 0x02005038 has no
 * pool; the halfword at 0x02005066 is alignment before the next owner.
 */

s32 Func_0200b4e4();
void Func_0200b59a();

/*
 * Scan slots 0 through 8 inclusive.  On the first a1 that Func_0200b4e4
 * accepts, call Func_0200b59a and stop.  a0 and a1 advance together.
 */
void SceneActor_RunFirstMatchingSlot(s32 a0, s32 a1)
{
    unsigned int i = 0;

    do {
        if (Func_0200b4e4(a1)!= 0) {
            Func_0200b59a(a0, 0, 0);
            break;
        }
        i++;
        a0++;
        a1++;
    } while (i <= 8);
}
