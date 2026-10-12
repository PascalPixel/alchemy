/* Near miss, differ 2: the listing loads the owner's x (ldr r6, [r0, #8]) before it copies the 64 mask (adds r3, r2, #0); this draft copies the mask first. The OR written the other way round is differ 47. */
#include "types.h"
#include "field_event.h"
#include "party_state.h"

struct Cell {
    u8 unknown_00[6];
    s16 x;
    s16 z;
};

struct FieldActor *Object_GetById(s32 id);
void Battle_WaitMode0(s32 frames);
void Func_02001b38(void);
void Func_020021a8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020022c8(s32 value);

void Func_02001bc0(struct Cell *cell)
{
    struct FieldActor *object = Object_GetById(gPartyState.current_owner);
    s32 x = (object->x >> 20) | (s16)(cell->x & 64);
    s32 z = (object->z >> 20) | (s16)(cell->z & 64);

    Func_02001b38();
    Func_020022c8(161);
    Func_020021a8(cell->x, cell->z + 1, 1, 1, x, z);
    Battle_WaitMode0(12);
}
