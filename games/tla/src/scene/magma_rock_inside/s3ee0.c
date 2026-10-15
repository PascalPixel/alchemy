#include "local.h"

extern char SceneId_Ov692Room2;
extern char SceneId_Ov692Room7;

s32 Func_02003ee0(void)
{
    s32 scene = gPartyState.scene;
    s32 entrance = gPartyState.entrance;

    if (!GameFlag_Test(0x109)) {
        if ((scene == (s32)&SceneId_Ov692Room2 && entrance == 8)
            || (scene == (s32)&SceneId_Ov692Room7 && entrance == 11)) {
            GameFlag_ClearBit(0x300);
            GameFlag_ClearBit(0x301);
            GameFlag_ClearBit(0x302);
        }
    }
    return 0;
}
