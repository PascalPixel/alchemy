#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void WaitFrames(s32 frames);

void Func_0200015c(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);
    s32 n = 60;

    object->motionFlags = 3;
    while (n != 0) {
        WaitFrames(1);
        if (object->velocityY == 0)
            break;
        n--;
    }
    WaitFrames(10);
    object->motionFlags = 0;
}
