/* Near miss, overlay 673 Func_020008a0 (0x020088a0): differ 2 of 78.
   The listing schedules sub sp, #12 after the follower pointer moves to r8 and
   object->x is loaded; this draft emits it two instructions earlier. Tried: the
   declaration order, an s32[3] position and a three-field struct. */
#include "../../../../games/tla/src/scene/gabomba_catacombs/local.h"

void Func_020008a0(struct ObjectRuntime *object, s32 dz)
{
    struct GabombaCatacombsFollower *follower;
    struct GabombaCatacombsVec position;

    follower = ((struct GabombaCatacombsEventWork *)Ram_HeapSlots->event_work)->follower;

    position.x = object->x;
    position.y = object->y;
    position.z = object->z + dz;
    Func_02001174(object, &position);
    position.x = object->x;
    position.y = object->y;
    position.z = object->z + 0x80000;
    if (Func_02001174(object, &position) <= 0) {
        position.x = object->x + 0x5b333;
        position.y = object->y;
        position.z = object->z + 0x5b333;
        if (Func_02001174(object, &position) <= 0) {
            position.x = object->x - 0x5b333;
            position.y = object->y;
            position.z = object->z + 0x5b333;
            if (Func_02001174(object, &position) <= 0) {
                follower->z += dz;
                object->z += dz;
            }
        }
    }
}
