#include "local.h"

extern u32 Data_020057a8[];
extern u32 Data_020057c0[];
extern u32 Data_020058b0[];
extern u32 Data_02005958[];
extern u32 Data_02005aa8[];
extern struct ScenePlacement Data_02005b20[];
extern struct ScenePlacement Data_02005be0[];

void *Func_020003d0(void)
{
    s32 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov653Room1)
        return Data_020057c0;
    if (scene == (s32)&SceneId_Ov653Room3)
        return Data_020058b0;
    if (scene == (s32)&SceneId_Ov653Room4)
        return Data_02005958;
    if (scene == (s32)&SceneId_Ov653Room7)
        return Data_02005aa8;
    if (scene == (s32)&SceneId_Ov653Room8) {
        if (GameFlag_Test(0x849)) {
            Data_02005b20[3].talkFacing = 0;
            Data_02005b20[4].talkFacing = 0;
            Data_02005b20[5].talkFacing = 0;
        }
        return Data_02005b20;
    }
    if (scene == (s32)&SceneId_Ov653Room9) {
        if (GameFlag_Test(0x849)) {
            Data_02005be0[5].talkFacing = 3;
            Data_02005be0[6].x = 0x1700000;
            Data_02005be0[6].z = 0x2b60000;
            Data_02005be0[7].talkFacing = 1;
        }
        return Data_02005be0;
    }
    return Data_020057a8;
}

