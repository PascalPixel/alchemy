/* Near miss, differ 32 of 61: the listing reloads the table address and the hit object's
 * animation value on every pass of the search loop and indexes the table; this draft hoists them and walks a pointer. */
#include "../../../../games/tla/src/scene/islets/islets.h"

void Func_02003d0c(struct ObjectRuntime *object)
{
    struct IsletsVector position;
    struct ObjectRuntime *hit;
    u32 i;

    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffsetFar(0x100000, (((object->angle >> 12) + 2) & 12) << 12, &position);
    hit = Func_02003fdc(&position, 1);
    if (hit != 0) {
        for (i = 0; i < 6; i++) {
            if (*((struct BattleAnimationState *)hit->animation)->value28 == gData_02004070[i])
                return;
        }
        Object_SetPositionAndResetMotion(object, object->x, object->y, object->z);
    }
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    if (Func_02003e2c(object, &position) > 0)
        Object_SetPositionAndResetMotion(object, object->x, object->y, object->z);
}
