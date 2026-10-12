#include "scene.h"

void Func_02003364(void)
{
    Func_02004180(0);
    if (gPartyState.entrance == 3 || gPartyState.entrance == 4) {
        if (Scene_FlagTest(0x889))
            Func_02004090(16, 0, 0);
        else
            Func_02002318();
    }
    Func_02003820(Data_020047d2);
    if (Scene_FlagTest(0x20b)) {
        Func_020006ac();
        WaitFrames(10);
    }
    Func_02003338(12);
    Func_02003338(13);
    Func_02003338(14);
    Func_02003338(15);
    if (gPartyState.entrance == 1 || gPartyState.entrance == 2)
        Scheduler_AddOrUpdateCallback(Func_020020a8, 0x480);
}
