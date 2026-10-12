// Near miss, differ 2: the listing loads the object x (ldr r6, [r0, #8]) before it copies the mask 64
// (adds r3, r2, #0); here the copy comes first. A barrier naming column, mask and x changes the
// allocation (differ 25).
#include "../../../../games/tla/src/field/overlay_6ab/ov6ab.h"

struct Overlay6abCell {
    u8 unknown_00[6];
    s16 column;
    s16 row;
};

void Func_02003b74(void);

void Func_02003bfc(struct Overlay6abCell *cell)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);
    s32 x = (object->x >> 20) | (s16)(cell->column & 64);
    s32 z = (object->z >> 20) | (s16)(cell->row & 64);

    Func_02003b74();
    Func_0200446c(161);
    Func_02004234(cell->column, cell->row + 1, 1, 1, x, z);
    Battle_WaitMode0(12);
}
