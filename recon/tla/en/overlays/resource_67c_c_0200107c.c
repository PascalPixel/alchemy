/* Draft, differ 20: the listing keeps 0 in r5 and 34 in r6, the draft the other way round, and loads object->x after movs r1. */
#include "../../../../games/tla/src/field/overlay_67c/local.h"

void Func_0200107c(s32 unused, s32 id)
{
    if ((u32)(id - 10) <= 1) {
        struct ObjectRuntime *object = Object_GetById(10);

        object->motion_flags = 0;
        Func_02003d1c_(8, 33, 1, 1, object->x >> 20, 34);
        object = Object_GetById(11);
        object->motion_flags = 0;
        Func_02003d1c_(8, 33, 1, 1, object->x >> 20, 34);
    }
}
