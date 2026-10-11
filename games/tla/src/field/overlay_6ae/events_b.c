#include "ov6ae.h"

void Func_02000610(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    ObjectMotion_EnableActionAndResetMotion(id);
    object->unknown_5c[6] = (((u32)Random16Far() * 40) >> 16) + 20;
    object->update = (void (*)(void))Func_02000038;
}

void Func_02000640(s32 id)
{
    Object_GetById(id)->update = NULL;
    ObjectMotion_ArmCallback(id, 0xc000, 0);
}
