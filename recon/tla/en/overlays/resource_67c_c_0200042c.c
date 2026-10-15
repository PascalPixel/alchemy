/* Draft, differ 4: the listing loads the pool word -0x10000 before object->y, twice. */
#include "../../../../games/tla/src/scene/aqua_rock_inside/local.h"

void Func_0200042c(s32 unused, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    if (id == 10) {
        s32 row = object->z >> 20;

        if (row == 12) {
            object->motionFlags = 0;
            object->y += -0x10000;
            GameFlag_SetBit(0x95a);
            Func_02003d1c(0, 12, 1, 1, 9, row);
            Func_02003d1c(0, 12, 1, 1, 13, row);
        }
    } else if (id == 11) {
        if (object->x >> 20 == 51) {
            object->motionFlags = 0;
            object->y += -0x10000;
            GameFlag_SetBit(0x95b);
        }
    }
}
