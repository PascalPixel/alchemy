#include "ov670.h"

extern const u8 Data_020061a8[];

void ObjectMotion_EnableActionAndResetMotion(s32 actor);

void Func_020045d0(void)
{
    Func_02005ba4(0x4cccc, 0x9999);
    Ov670_CamMove(132 << 17, 0, 200 << 17, 1);
    Func_02005bb4();
    Ov670_Wait(10);
    ObjectMotion_Launch(18, 6, 0);
    ObjectMotion_Launch(19, 6, 0);
    ObjectMotion_Launch(20, 6, 0);
    ObjectMotion_Launch(21, 6, 0);
    ObjectMotion_Launch(22, 6, 0);
    ObjectMotion_Launch(23, 6, 0);
    ObjectMotion_Launch(24, 6, 0);
    ObjectMotion_Launch(25, 6, 40);
    Ov670_ObjTurn(20, 0x5000);
    ObjectMotion_Launch(20, 2, 0);
    Ov670_MessagePut(0x4000 + 20, 0);
    Ov670_ObjTurn(23, 0x3000);
    ObjectMotion_Launch(23, 2, 0);
    Ov670_MessagePut(0x4000 + 23, 0);
    ObjectMotion_EnableActionAndSetCallback(18, Data_020061a8);
    ObjectMotion_EnableActionAndSetCallback(19, Data_020061a8);
    ObjectMotion_EnableActionAndSetCallback(20, Data_020061a8);
    ObjectMotion_EnableActionAndSetCallback(21, Data_020061a8);
    ObjectMotion_EnableActionAndSetCallback(22, Data_020061a8);
    ObjectMotion_EnableActionAndSetCallback(23, Data_020061a8);
    ObjectMotion_EnableActionAndSetCallback(24, Data_020061a8);
    ObjectMotion_EnableActionAndSetCallback(25, Data_020061a8);
    Ov670_Wait(40);
    Ov670_ObjTurn(17, 0x3000);
    ObjectMotion_Launch(17, 2, 10);
    ObjectMotion_Launch(17, 4, 10);
    Ov670_MessagePut(17, 0);
    ObjectMotion_EnableActionAndResetMotion(18);
    ObjectMotion_EnableActionAndResetMotion(19);
    ObjectMotion_EnableActionAndResetMotion(20);
    ObjectMotion_EnableActionAndResetMotion(21);
    ObjectMotion_EnableActionAndResetMotion(22);
    ObjectMotion_EnableActionAndResetMotion(23);
    ObjectMotion_EnableActionAndResetMotion(24);
    ObjectMotion_EnableActionAndResetMotion(25);
    Ov670_Wait(40);
    Ov670_ObjArm(18, 0xd000, 0);
    Ov670_ObjArm(19, 0xd000, 0);
    Ov670_ObjArm(20, 0xd000, 0);
    Ov670_ObjArm(21, 0xd000, 0);
    Ov670_ObjArm(22, 0xb000, 0);
    Ov670_ObjArm(23, 0xb000, 0);
    Ov670_ObjArm(24, 0xb000, 0);
    Ov670_ObjArm(25, 0xb000, 0);
    Func_02005b94(17, 0x107, 0);
    Ov670_MessagePut(0x8000 + 17, 0);
    Ov670_MessagePut(0x8000 + 17, 0);
    Motion_SetModeAndWaitAnimation(17, 3);
    Ov670_MessagePut(0x8000 + 17, 0);
    ObjectMotion_Launch(17, 4, 10);
    Ov670_MessagePut(0x8000 + 17, 0);
}
