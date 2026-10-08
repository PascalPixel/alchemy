#include "local.h"

s32 Func_0200166c(struct ObjectRuntime *object)
{
    if (object->y + object->velocity_y < -0x300000)
        return 0;
    return 1;
}
