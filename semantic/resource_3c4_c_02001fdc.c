/*
 * resource_3c4 @ 0x02001fdc (98 bytes).
 *
 * Two six-argument placement calls followed by four service pokes.  The two
 * placement calls take four register arguments plus two stack words at
 * [sp,#0] and [sp,#4]; r5 holds the constant 49 across both calls and supplies
 * one stack word each time, so the pairs are (8, 49) and (49, 43) in slot
 * order.
 *
 * The 0x…00000 arguments are 12.20 fixed point: 0xc6 << 18 is 0x03180000,
 * 0xae << 18 is 0x02b80000, 0x88 << 16 is 0x00880000.
 *
 * Call symbols are the per-site stored-displacement identities decoded from
 * the reference `bl` fields (+2 rule): each site keeps its own local veneer
 * label rather than the resolved far name -- Func_0200508e/Func_020050a0 are
 * Func_080091c0, Func_020051a2/Func_020051ac are Func_0808a3c0, and
 * Func_02005162/Func_02005170 are Func_0808a0f0.
 *
 * The tail at 0x02002028 is a secondary entry into this owner used by
 * Func_02001f5c; that caller is not admitted (see the note in this overlay's
 * report), but the code it enters is this owner's and is written here.
 *
 * `add sp,#8 ; pop {r5} ; pop {r0} ; bx r0` return: void.
 */
#include "types.h"

void Func_0200508e();
void Func_020050a0();
void Func_020051a2();
void Func_020051ac();
void Func_02005162();
void Func_02005170();

void Func_02001fdc(void)
{
    Func_0200508e(72, 49, 1, 1, 8, 49);
    Func_020050a0(113, 43, 1, 1, 49, 43);
    Func_020051a2(100, 0, 0);
    Func_020051ac(101, 0, 0);
    Func_02005162(15, 0x00880000, 0x03180000);
    /* secondary entry 0x02002028 */
    Func_02005170(16, 0x03180000, 0x02b80000);
}
