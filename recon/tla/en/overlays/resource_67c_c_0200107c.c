/* Draft, differ 10 of 37: through Func_02003d1c_ the registers agree; the listing sets r1 (33) before it loads object->x and r2 (1) right after, the draft loads x first. Tried id == 10 || id == 11, Object_GetById_, Func_02003d14_. */
#include "../../../../games/tla/src/scene/aqua_rock_inside/local.h"

void Func_0200107c(s32 unused, s32 id)
{
    if ((u32)(id - 10) <= 1) {
        struct ObjectRuntime *object = Object_GetById(10);

        object->motionFlags = 0;
        Func_02003d1c_(8, 33, 1, 1, object->x >> 20, 34);
        object = Object_GetById(11);
        object->motionFlags = 0;
        Func_02003d1c_(8, 33, 1, 1, object->x >> 20, 34);
    }
}
