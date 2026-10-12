#include "local.h"

void Func_0200014c(void)
{
    Func_020006d8(18, 2, 8);
}

#include "scene_ids.h"

extern s32 Data_020012ac[];
extern s32 Data_020010d8[];

s32 *Func_0200015c(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov65dRoom2)
        return Data_020012ac;
    return Data_020010d8;
}
