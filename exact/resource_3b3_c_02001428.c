/* Apply the actor's low four counter bits as its animation mode. */
#include "types.h"

void Func_02003fa2(u8 *actor, s32 mode);

s32 Func_02001428(u8 *actor)
{
    Func_02003fa2(actor, *(u16 *)(actor + 100) & 15);
    return 0;
}
