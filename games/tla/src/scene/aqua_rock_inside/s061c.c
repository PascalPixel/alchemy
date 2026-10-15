#include "local.h"

#if !defined(TLA_EDITION_JA)
void Func_0200061c(struct ObjectRuntime *object)
{
    object->motion_flags = 3;
    do {
        WaitFrames(1);
    } while (object->velocity_y != 0);
    Func_02003ef4(188);
    WaitFrames(10);
    object->motion_flags = 0;
}
#endif
