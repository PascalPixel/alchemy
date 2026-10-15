#include "shaman_village.h"

extern u32 Data_02005410[];
extern u32 Data_02005428[];
extern s16 Data_02005590[];
struct ShamanVillageSceneBlock {
    u8 unknown_00[22];
    u8 field_16;
};

extern struct ShamanVillageSceneBlock Data_020056b0;
extern u32 Data_02005860[];
extern u32 Data_02005998[];
extern u32 Data_02005a40[];

void *Func_020002dc(void)
{
    s32 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Shaman1)
        return Data_02005428;
    if (scene == (s32)&SceneId_Shaman3)
        return Data_02005590;
    if (scene == (s32)&SceneId_Shaman2) {
        if (GameFlag_Test(0x9fe))
            Data_020056b0.field_16 = 1;
        return &Data_020056b0;
    }
    if (scene == (s32)&SceneId_Shaman4)
        return Data_02005860;
    if (scene == (s32)&SceneId_Shaman5)
        return Data_02005998;
    if (scene == (s32)&SceneId_Shaman6)
        return Data_02005a40;
    return Data_02005410;
}
