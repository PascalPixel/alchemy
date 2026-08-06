/* Select actor 27's nested state byte from the low runtime flag bit. */
#include "types.h"


extern u8 * Func_0200854a();
void Func_02003e18(void)
{
    u8 *actor = Func_0200854a(27);
    u8 *nested = *(u8 **)(actor + 80);

    nested[35] = (*(u32 *)0x03001e40 & 1) ? 2 : 64;
}
