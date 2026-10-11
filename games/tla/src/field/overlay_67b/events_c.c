#include "local.h"

void Func_02000d64(struct ObjectRuntime *object, s32 limit)
{
    s32 frames = 60;

    while (frames != 0) {
        WaitFrames(1);
        frames--;
        if (object->y <= limit)
            break;
    }
}
