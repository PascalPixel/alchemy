#include "shaman_village.h"

extern u32 Data_02005aa0[];
extern u32 Data_02005aac[];
extern u32 Data_02005bc0[];
extern u32 Data_02005ca4[];
extern u32 Data_02005f98[];
extern u32 Data_020061f0[];
extern u32 Data_020062d4[];

void *Func_02000c80(void)
{
    s32 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Shaman1)
        return Data_02005aac;
    if (scene == (s32)&SceneId_Shaman3)
        return Data_02005bc0;
    if (scene == (s32)&SceneId_Shaman2)
        return Data_02005ca4;
    if (scene == (s32)&SceneId_Shaman4)
        return Data_02005f98;
    if (scene == (s32)&SceneId_Shaman5)
        return Data_020061f0;
    if (scene == (s32)&SceneId_Shaman6)
        return Data_020062d4;
    return Data_02005aa0;
}
