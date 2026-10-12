/* Near miss: differ 4 of 82 (two reordered rows): the native loads the party
 * state's address before it builds the owner offset, and sets the first
 * argument after the movement byte is cleared. */
#include "../../../../games/tla/src/field/overlay_676/ov676.h"

s32 Func_0200014c(struct ObjectRuntime *object)
{
    u8 *tiles = Ram_HeapSlots->window_tiles;
    u8 *event = Ram_HeapSlots->event_work;
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);
    s32 force = 0;

    if (SceneActor_CalculateFixedPointDistance(&leader->x, &object->x) <= 11) {
        object->movement_state = force;
        Func_02002448(object, 2);
    } else {
        struct ObjectRuntime *target;
        u16 *flags = (u16 *)&object->action;

        if (*flags & 1)
            target = Object_GetById(10);
        else
            target = Object_GetById(9);
        if (Func_02000088(object, target, 24, 0) == 0) {
            leader = Object_GetById(gPartyState.current_owner);
            if ((*(s16 *)(event + 352) != 0 || tiles[4] != 0) && (*flags & 2))
                force = 1;
            Func_02000088(object, leader, 24, force);
        }
    }
    return 0;
}
