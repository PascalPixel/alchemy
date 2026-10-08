#include "types.h"
#include "object_runtime.h"
#include "system.h"

void Object_ShakeAngle(u32 object_id)
{
    struct ObjectRuntime *object;
    s32 angle;
    s32 i;

    object = ObjectTable_Get(object_id);
    if (object == 0)
        return;
    angle = object->angle;
    for (i = 15; i >= 0; i--) {
        u32 a = Random16();
        u32 b = Random16();

        object->angle += (a >> 4) - (b >> 4);
        WaitFrames(1);
    }
    object->angle = angle;
}
