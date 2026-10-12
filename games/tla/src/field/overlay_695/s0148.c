#include "local.h"

void Func_02000148(void)
{
    struct ObjectRuntime *object = Object_GetById(16);

    GameFlag_SetBit(0x302);
    object->motion_flags = 0;
    object->priority_flags = 0;
    Func_02001060_(8, 42, 1, 1, 6, 42);
}
