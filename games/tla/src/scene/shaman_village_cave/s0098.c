#include "local.h"

void Func_02000098(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);
    s32 frames = 60;

    object->motionFlags = 3;
    while (frames != 0) {
        WaitFrames(1);
        if (object->velocityY == 0)
            break;
        frames--;
    }
    WaitFrames(10);
    ObjectDispatch_SetSingleChildFlags(object, 0);
    object->motionFlags = 0;
}

void Func_020000d4(void)
{
    Func_02001060_(99, 10, 4, 3, 99, 18);
}
