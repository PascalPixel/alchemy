#include "scene.h"

void Func_020032c4(void)
{
    s32 variant = Object_GetById(gPartyState.current_owner)->sprite->priority;

    ObjectMotion_SetActionVariant(13, variant);
    ObjectMotion_SetActionVariant(14, variant);
}

void Func_020032f4(void)
{
    Func_02003820(Data_0200478e);
    if (GameFlag_Test(0x20b)) {
        Func_020006ac();
        WaitFrames(10);
    }
    /* FAKEMATCH: the native builds the priority before it loads the callback. */
    SCENE_TASK_ADD_LATE(Func_02002010, 0x480);
    /* FAKEMATCH: the native builds the priority before it loads the callback. */
    SCENE_TASK_ADD_LATE(Func_020032c4, 0x480);
}

void Func_02003338(s32 id)
{
    Object_GetById(id)->priority_flags |= 2;
    ObjectMotion_SetActionVariant(id, 3);
    Func_020040f8(id);
    Object_SetModeById(id, 2);
}
