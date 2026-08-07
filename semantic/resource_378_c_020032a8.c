#include "types.h"

extern u8 *Func_0808a080(s32 actorId);

/* Return nonzero while actor 0 faces inside the wrapped 0xa001..0xdffe arc. */
s32 Func_020032a8(void)
{
    u8 *actor = Func_0808a080(0);
    u16 wrapped = (u16)(*(u16 *)(actor + 6) + 0x5fff);
    return wrapped <= 0x3ffe;
}
