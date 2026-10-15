#include "local.h"

extern s32 Data_02005d48[];
extern s32 Data_02005d54[];
extern s16 Data_02005de4[];
extern s32 Data_02005e20[];
extern s32 Data_02005e8c[];
extern s32 Data_02005ef8[];
extern s32 Data_02005f10[];
extern s32 Data_02005f40[];
extern s32 Data_02005fc4[];
extern s32 Data_0200615c[];

void *Func_02000580(void)
{
    s32 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov653Room1)
        return Data_02005d54;
    if (scene == (s32)&SceneId_Ov653Room2)
        return Data_02005de4;
    if (scene == (s32)&SceneId_Ov653Room3)
        return Data_02005e20;
    if (scene == (s32)&SceneId_Ov653Room4)
        return Data_02005e8c;
    if (scene == (s32)&SceneId_Ov653Room5)
        return Data_02005ef8;
    if (scene == (s32)&SceneId_Ov653Room6)
        return Data_02005f10;
    if (scene == (s32)&SceneId_Ov653Room7)
        return Data_02005f40;
    if (scene == (s32)&SceneId_Ov653Room8)
        return Data_02005fc4;
    if (scene == (s32)&SceneId_Ov653Room9)
        return Data_0200615c;
    return Data_02005d48;
}
