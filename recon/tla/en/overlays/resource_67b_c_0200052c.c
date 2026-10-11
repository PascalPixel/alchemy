// differ 4: the listing loads object->x before it starts the multiply by 48,
// and the pool load of -0x180000 before movs r0. Through the
// Engine_ObjectCreate_ inline the difference grows to 10.
#include "../../../../games/tla/src/field/overlay_67b/local.h"

void Func_0200052c(struct ObjectRuntime *object)
{
    if ((u32)Random16Far() < 0x1000) {
        struct ObjectRuntime *child = Engine_ObjectCreate(0x2a2, object->x + Random16Far() * 48 - 0x180000, object->y, object->z);

        if (child != 0) {
            ObjectDispatch_SetSingleChildFlags(child, 0);
            child->update = (void (*)(void))Func_020004e4;
            child->motion_flags = 0;
        }
    }
}
