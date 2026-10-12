#include "scene.h"

void Func_02000e78(void)
{
    Func_02004040();
    Func_020041b8(0);
    Func_020021d4();
    Object_LinkObjectAndSetCallback(gPartyState.current_owner, 21);
    Func_02004118(0x26666, 0x4ccc);
    Scene_CamBounds(0xe80000, -1, 0x1480000, 1);
    Scene_SetPositionAndCommit(21, 232, 328);
    Scene_CamBounds(0xe80000, -1, 0xc80000, 1);
    Scene_SetPositionAndCommit(21, 232, 280);
    Scene_SetPositionAndCommit(21, 232, 168);
    Scheduler_RemoveCallbackFar(Func_02002180);
    Scene_ArmCallback(21, 0x4000, 0);
    ObjectMotion_EnableActionAndResetMotion(gPartyState.current_owner);
    Battle_WaitMode0(20);
    GameFlag_SetBit(0x88b);
    Func_02004048();
}
