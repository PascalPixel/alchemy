#include "ov670.h"

s32 Func_02005034(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Kibonbo1)
        Func_02005114();
    else if (scene == (s32)&SceneId_Kibonbo4)
        Func_0200516c();
    else if (scene == (s32)&SceneId_Kibonbo2)
        Func_020054e0();
    else if (scene == (s32)&SceneId_Kibonbo5)
        Func_02005594();
    else if (scene == (s32)&SceneId_Kibonbo6)
        Func_020057e0();
    else if (scene == (s32)&SceneId_Kibonbo3)
        Func_02005898();
    return 0;
}
