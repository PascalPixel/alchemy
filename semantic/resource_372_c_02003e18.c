/* Select actor 27's nested state byte from the low runtime flag bit. */
#include "types.h"
u8 *Func_0808a080();

void Func_02003e18(void)
{
    u8 *actor = Func_0808a080(27);
    u8 *nested = *(u8 **)(actor + 80);

    nested[35] = (*(u32 *)0x03001e40 & 1) ? 2 : 64;
}
