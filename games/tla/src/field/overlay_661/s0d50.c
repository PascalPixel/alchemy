#include "scene.h"

void Func_02000b84(s32 x, s32 y, s32 z);
void Func_02001c5c(void);

void Func_02000d50(void)
{
    Func_02004040();
    Func_020041b8(0);
    Func_020021d4();
    Object_LinkObjectAndSetCallback(gPartyState.current_owner, 21);
    Func_02004118(0x26666, 0x4ccc);
    Scene_CamBounds(0xf60000, -1, 0x2680000, 1);
    Scene_SetPositionAndCommit(21, 248, 648);
    Scene_SetPositionAndCommit(21, 248, 576);
    Scheduler_RemoveCallbackFar(Func_02002180);
    Battle_WaitMode0(10);
    Scheduler_RemoveCallbackFar(Func_02001c5c);
    WaitFrames(1);
    Func_02004090(22, 0, 0);
    Battle_WaitMode0(20);
    Func_02000b84(0xf80000, 0, 0x2400000);
    Func_020041f8(176);
    Object_GetById(21)->velocity_y = 0x60000;
    Object_SetPartAttribute(Object_GetById(21), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(21), 1);
    Scene_SetPositionAndCommit(21, 256, 560);
    Battle_WaitMode0(20);
    ObjectMotion_EnableActionAndResetMotion(gPartyState.current_owner);
    Func_020041f8(241);
    Scene_SetSpeed(21, 0xcccc, 0x6666);
    Scene_SetPositionAndReset(21, 248, 496);
    Func_020041f8(390);
    Func_02004090(21, 0, 0);
    GameFlag_SetBit(0x88a);
    Func_02004048();
}
