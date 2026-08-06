#include "types.h"



/* Return nonzero while actor 0 faces inside the wrapped 0xa001..0xdffe arc. */
extern u8 * Func_020068be(s32 actorId);
s32 Func_020032a8(void)
{
    u8 *actor = Func_020068be(0);
    u16 wrapped = (u16)(*(u16 *)(actor + 6) + 0x5fff);
    return wrapped <= 0x3ffe;
}
