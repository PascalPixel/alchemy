/* Near miss: differ 2 of 81 (one reordered row): the native loads the party
 * state's address before it spills the third argument; the draft spills first. */
#include "../../../../games/tla/src/scene/jupiter_aerie/jupiter_aerie.h"

void Func_02002888(s32 dx, s32 dz, s32 value)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);
    u8 *event = Ram_HeapSlots->event_work;
    struct JupiterAerieWork *work = (struct JupiterAerieWork *)Data_02003488;
    s32 x = GameFlag_GetByte(work->flag);
    s32 z = GameFlag_GetByte(work->flag + 8);

    if (x != 0 || z != 0) {
        struct ObjectRuntime *shadow;

        GameFlag_SetByte(work->flag, 0);
        GameFlag_SetByte(work->flag + 8, 0);
        object->x = ((x + dx) << 20) + 0x80000;
        object->z = ((z + dz) << 20) + 0x80000;
        shadow = *(struct ObjectRuntime **)(event + 460);
        shadow->x = object->x;
        shadow->z = object->z;
        Func_02002a18();
        Func_02002640();
        *(s32 *)((u8 *)Ram_HeapSlots->event_work + 428) = value;
    }
}
