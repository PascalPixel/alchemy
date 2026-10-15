#include "local.h"

#if !defined(TLA_EDITION_JA)
void Func_0200061c(struct ObjectRuntime *object)
{
    object->motionFlags = 3;
    do
    {
        WaitFrames(1);
    } while (object->velocityY != 0);
    Func_02003ef4(188);
    WaitFrames(10);
    object->motionFlags = 0;
}
#endif
