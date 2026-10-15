#include "lemurian_ship.h"

void Func_02001944(s32 actor, s32 id)
{
    struct LemurianShipObject *object = Object_GetById(id);

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

void Func_02001a04(void)
{
    struct LemurianShipObject *object = Object_GetById(9);

    Func_02002d40(9, 0x100, 0);
    Motion_SetVarCbAndRefresh(9, 2);
    object->field_48 = 0x8000;
    ObjectMotion_SetActionVariant(9, 1);
    LemurianShip_ObjSpeed(9, 0xcccc, 0x6666);
    ObjectMotion_Launch(9, 6, 0);
    LemurianShip_ObjCommit(9, 0x340, 88);
    Battle_WaitMode0(20);
    LemurianShip_ObjSpeed(9, 0x19999, 0xcccc);
    ObjectMotion_SetActionVariant(9, 3);
    LemurianShip_ObjCommit(9, 0x338, 72);
    LemurianShip_ObjArm(9, 0x3000, 40);
    GameFlag_SetBit(0x201);
}
