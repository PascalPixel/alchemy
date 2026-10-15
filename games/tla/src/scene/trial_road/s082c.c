#include "trial_road.h"

void Func_0200082c(s32 a, s32 id)
{
    s32 x;

    if ((u32)(id - 15) <= 1)
    {
        Func_0200062c(id);
        return;
    }
    /* FAKEMATCH: one variable holds the object and then its cell column, so the column stays in r0 as in the native (differ 10 with a separate object). */
    x = (s32)Object_GetById(id);
    x = ((struct ObjectRuntime *)x)->x >> 20;
    id = 0;
    if (x == 13)
        id = 17;
    if (x == 15)
        id = 18;
    if (x == 17)
        id = 19;
    if (x == 19)
        id = 20;
    if (id != 0)
    {
        s32 flag = id + 0x203;

        if (GameFlag_Test(flag) == 0)
        {
            struct ObjectRuntime *object = Object_GetById(id);

            object->update = NULL;
            Animation_ApplyChildValues(object, 0);
            Object_SetModeById(id, 6);
            Func_0200070c(id, -16);
            GameFlag_SetBit(flag);
        }
    }
}
