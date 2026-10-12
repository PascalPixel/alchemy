#include "ov656.h"

void Func_02001368(void)
{
    Func_02002c68();
    Func_02002da8(0);
    Func_02002da8(0);
    Func_02002c70();
}

void Func_02001380(s32 id)
{
    struct Ov656Object *object = Object_GetById(id);

    Object_GetById(id)->priority_flags |= 0x20;
    ObjectMotion_SetActionVariant(id, 1);
    object->priority_flags &= ~2;
    object->timer = 1;
}

void Func_020013b8(s32 id)
{
    struct Ov656Object *object = Object_GetById(id);

    Object_GetById(id)->priority_flags |= 0x20;
    ObjectMotion_SetActionVariant(id, 2);
    object->priority_flags &= ~2;
    object->timer = 1;
}

void Func_020013f0(void)
{
    Object_GetById(15)->priority_flags |= 0x20;
    ObjectMotion_SetActionVariant(15, 3);
    Object_GetById(15)->priority_flags &= ~2;
    Func_02001380(16);
    Func_02001380(17);
    Func_02001380(18);
    Func_02001380(19);
    Func_02001380(20);
}

void Func_0200143c(void)
{
    Func_02001380(16);
    Func_02001380(17);
    Func_02001380(18);
    Func_02001380(19);
    Func_02001380(20);
    Func_02001380(22);
}

void Func_02001464(void)
{
    Func_020013b8(10);
    Func_020013b8(11);
}

void Func_02001474(void)
{
    struct Ov656Object *leader = Object_GetById(gPartyState.current_owner);
    s32 x = leader->x >> 20;
    s32 z = leader->z >> 20;

    Ov656_MapCopy(74, 105, 4, 85, 8, 6);
    if (z == 25) {
        if (x == 4)
            Ov656_MapCopy(18, 126, 4, 85, 1, 2);
    } else if (z == 26) {
        if (x == 7)
            Ov656_MapCopy(18, 126, 7, 86, 1, 1);
    } else if (z == 27) {
        if (x == 4)
            Ov656_MapCopy(18, 126, 4, 87, 1, 2);
        else if (x == 6)
            Ov656_MapCopy(18, 126, 6, 87, 1, 2);
        else if (x == 7)
            Ov656_MapCopy(18, 126, 7, 87, 1, 2);
        else if (x == 9)
            Ov656_MapCopy(18, 126, 9, 87, 1, 2);
    } else if (z == 29) {
        if (x == 6)
            Ov656_MapCopy(18, 126, 6, 89, 1, 2);
    }
}
