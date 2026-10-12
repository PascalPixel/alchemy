/* Near miss, differ 2 of 49: the listing copies the mask 0x40 to r3 one instruction later
 * (after it loads the owner's x). */
#include "../../../../games/tla/src/field/overlay_6a0/ov6a0.h"

void Func_02002534(struct Ov6a0Cell *cell)
{
    struct ObjectRuntime *owner = Object_GetById(gPartyState.current_owner);
    s32 x = (owner->x >> 20) | (s16)(cell->x & 0x40);
    s32 z = (owner->z >> 20) | (s16)(cell->z & 0x40);

    Func_020024ac();
    Func_02002c5c(161);
    Func_02002b1c(cell->x, cell->z + 1, 1, 1, x, z);
    Battle_WaitMode0(12);
}
