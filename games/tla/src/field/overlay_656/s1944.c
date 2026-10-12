#include "ov656.h"

void Func_02001944(s32 actor, s32 id)
{
    struct Ov656Object *object = Object_GetById(id);

    Func_02002c68();
    Func_02002da8(0);
    object->scale_x = 0x1000;
    object->scale_y = 0x1000;
    WaitFrames(1);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Battle_WaitMode0(20);
    Func_02002de0(103);
    ObjectMotion_EnableActionAndSetCallback(actor, Data_02002e30);
    Object_SetActionCallbackAndRefreshById(id, Data_02002e6c);
    Battle_WaitMode0(20);
    Func_02002c70();
}

void Func_020019ac(void)
{
    if (GameFlag_Test(0x932) == 0) {
        Func_02002c68();
        Func_02002da8(0);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        Battle_WaitMode0(20);
        Func_02001a04();
        Func_02002c70();
    }
}

void Func_020019e0(void)
{
    if (GameFlag_Test(0x932) == 0) {
        Func_02002c68();
        Func_02002da8(0);
        Func_02001a04();
        Func_02002c70();
    }
}
