/* Select actor 27's nested state byte from the low runtime flag bit. */
#include "types.h"


extern u8 * Func_0200854a();
/*
 * 7 halfwords out at 48 bytes, all one wall: the reference keeps the store
 * address in r2 and the stored constant in r3, ours keeps them the other way
 * round.  Declaration order, operand order, a separate pointer local, a u8
 * value and every -fthumb-orr/-fsched flag leave the pair where it is.  Same
 * join-register swap as resource_3a0:0324 and resource_39c:0030.
 */
void Func_02003e18(void)
{
    u8 *actor = Func_0200854a(27);
    u32 state = *(u32 *)0x03001e40;
    u8 *nested = *(u8 **)(actor + 80);
    u32 value;
    u8 *slot;

    state = state & 1;
    if (state != 0) {
        slot = nested + 35;
        value = 2;
    } else {
        slot = nested + 35;
        value = 64;
    }
    *slot = (u8)value;
}
