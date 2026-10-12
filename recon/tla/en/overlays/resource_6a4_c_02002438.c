/* Near miss: differ 2 of 49 (one reordered row): the native loads x before it
 * copies the mask 64; the draft copies the mask first. */
#include "../../../../games/tla/src/field/overlay_6a4/ov6a4.h"

void Func_02002438(s16 *cell)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);
    s32 x;
    s32 z;

    x = object->x; /* FAKEMATCH: the temporary loads x before the mask is copied (one reordered row). */
    x = (x >> 20) | (s16)(cell[3] & 64);
    z = (object->z >> 20) | (s16)(cell[4] & 64);
    Func_020023b0();
    Func_02002b68(161);
    Func_02002a40(cell[3], cell[4] + 1, 1, 1, x, z);
    Battle_WaitMode0(12);
}
