#include "ov670.h"

void Func_020031f0(s32 id)
{
    Func_02005b8c(id);
    Object_SetPartAttribute(Object_GetById(id), 15);
    ObjectMotion_SetActionVariant(id, 1);
    Object_GetById(id)->priority_flags |= 2;
}

void Func_02003220(void)
{
    if (GameFlag_Test(265)) {
        if (GameFlag_Test(0x200) == 0)
            Func_0200057c();
    } else {
        GameFlag_SetBit(0x200);
    }
}

void Func_0200324c(void)
{
    u32 i = 0;

    /* FAKEMATCH: the loop is written with goto so that it carries no loop notes: the bytes build the zero on every pass, and a for or do loop hoists it (differ 17). */
loop:
    {
        s32 id = i + 15;
        struct Ov670Object *object = Object_GetById(id);

        Func_020031f0(id);
        object->motion_flags = 0;
    }
    if (++i <= 1)
        goto loop;
    Object_GetById(15)->y = 0x200000;
    Object_GetById(16)->y = 0;
    Func_02003220();
}

void Func_02003288(void)
{
    u32 i = 0;

    /* FAKEMATCH: the loop is written with goto so that it carries no loop notes: the bytes build the zero on every pass, and a for or do loop hoists it (differ 8). */
loop:
    {
        s32 id = i + 26;
        struct Ov670Object *object = Object_GetById(id);

        Func_020031f0(id);
        object->motion_flags = 0;
    }
    if (++i <= 1)
        goto loop;
    Func_02003220();
}
