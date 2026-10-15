#include "scene.h"

extern const u8 Data_0200518c[];
extern const u8 Data_02005198[];
extern const u8 Data_0200521c[];
extern const u8 Data_020053fc[];
extern const u8 Data_020054e0[];
extern const u8 Data_02005594[];
extern const u8 Data_02005630[];
extern const u8 Data_02005678[];
extern const u8 Data_020056b4[];
extern const u8 Data_02005750[];
extern const u8 Data_0200578c[];
extern const u8 Data_020057bc[];
extern const u8 Data_020057e0[];

// Each room's table, as ☀️'s Scene_GetEntrances picks one.
const u8 *Func_020003c8(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov661Room1)
        return Data_02005198;
    if (scene == (s32)&SceneId_Ov661Room2)
        return Data_0200521c;
    if (scene == (s32)&SceneId_Ov661Room3)
        return Data_020053fc;
    if (scene == (s32)&SceneId_Ov661Room4)
        return Data_020054e0;
    if (scene == (s32)&SceneId_Ov661Room5)
        return Data_02005594;
    if (scene == (s32)&SceneId_Ov661Room6)
        return Data_02005630;
    if (scene == (s32)&SceneId_Ov661Room7)
        return Data_02005678;
    if (scene == (s32)&SceneId_Ov661Room8)
        return Data_020056b4;
    if (scene == (s32)&SceneId_Ov661Room9)
        return Data_02005750;
    if (scene == (s32)&SceneId_Ov661Room10)
        return Data_0200578c;
    if (scene == (s32)&SceneId_Ov661Room11)
        return Data_020057bc;
    if (scene == (s32)&SceneId_Ov661Room12)
        return Data_020057e0;
    return Data_0200518c;
}
