#include "islets.h"

u8 *Func_0200004c(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov674Room1)
        return gData_0200476c;
    if (scene == (s32)&SceneId_Ov674Room2)
        return gData_020047e4;
    if (scene == (s32)&SceneId_Ov674Room3)
        return gData_0200494c;
    if (scene == (s32)&SceneId_Ov674Room4)
        return gData_020049c4;
    if (scene == (s32)&SceneId_Ov674Room5) {
        if (GameFlag_Test(0xaa5))
            gData_02004a84[46] = 1;
        return gData_02004a84;
    }
    return gData_02004754;
}

void Func_020000d8(void)
{
    struct IsletsTarget target;
    struct ObjectRuntime *object = Object_GetById(10);

    if (Func_020037d8(&target)) {
        Func_02003a5c(target);
        WaitFrames(5);
        object->gravity = 0x8000;
        Object_SetModeById(10, 3);
        ObjectMotion_OffsetPositionAndResetMotion(10, 0, 8);
        Battle_WaitMode0(22);
        Func_02003ffc(240);
        Object_SetModeById(10, 8);
        object->priorityFlags |= 2;
        object->motionFlags = 0;
        object->velocityY = 0;
        object->y = 0xfff80000;
        object->terrainHeight = 0xfff80000;
        Func_02003e24(9, 1, 4, 1, 9, 10);
        Func_02003e24(9, 8, 4, 1, 9, 9);
        Func_02003e24(9, 64, 4, 1, 9, 74);
        GameFlag_SetBit(0xa2c);
    }
}
