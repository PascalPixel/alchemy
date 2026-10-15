/* Draft, differ 32 of 61 (twins in overlays 662, 668, 674, 69c, 6a1 have drafts too): the listing rereads other->animation, its value28 and the table address on every pass with the index in r4, and builds the angle through r3; a for loop strength-reduces the index, a goto loop hoists the loads. */
#include "../../../../games/tla/src/scene/mars_room_f/room.h"

extern u16 Data_02005f58[];

s32 Func_020054f8(struct ObjectRuntime *object, s32 *position);

void Func_02004fc0(struct ObjectRuntime *object)
{
    s32 position[3];
    struct ObjectRuntime *other;
    u32 i;
    s32 angle = ((object->angle >> 12) + 2 & 12) << 12;

    position[0] = object->x;
    position[1] = object->y;
    position[2] = object->z;
    Vector_AddPolarOffsetFar(0x100000, angle, position);
    other = Func_02005680(position, 1);
    if (other != NULL) {
        i = 0;
    next:
        if (((struct BattleAnimationState *)other->animation)->value28[0] == Data_02005f58[i])
            return;
        i++;
        if (i <= 5)
            goto next;
        Object_SetPositionAndResetMotion(object, object->x, object->y, object->z);
    }
    position[0] = object->x;
    position[1] = object->y;
    position[2] = object->z;
    if (Func_020054f8(object, position) > 0)
        Object_SetPositionAndResetMotion(object, object->x, object->y, object->z);
}
