#include "local.h"

extern u32 Data_020056a0[];
extern u32 Data_020056c0[];

u32 *Func_02000390(void)
{
    s32 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov653Room3)
        return Data_020056a0;
    if (scene == (s32)&SceneId_Ov653Room5)
        return Data_020056c0;
    return 0;
}
