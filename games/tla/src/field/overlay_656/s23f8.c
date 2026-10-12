#include "ov656.h"

void Func_020023f8(void)
{
    struct Ov656Object *leader = Object_GetById(gPartyState.current_owner);

    ObjectMotion_SetActionVariant(8, leader->sprite->priority);
}

void Func_0200241c(void)
{
    if (GameFlag_Test(8))
        Ov656_BlendStart(14, 46, 1, 1, 16, 46);
    Func_02002dc8(Data_02003010);
    OV656_TASK_ADD(Func_020023f8, 0x480);
}

void Func_0200245c(s32 id)
{
    struct Ov656Object *object = Object_GetById(id);

    Object_GetById(id)->priority_flags |= 0x20;
    ObjectMotion_SetActionVariant(id, 1);
    object->motion_flags = 0;
    object->y = 0;
    object->height = 0;
}
