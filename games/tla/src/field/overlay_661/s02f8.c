#include "scene.h"

extern const u8 Data_020049c4[];
extern const u8 Data_020049dc[];
extern const u8 Data_02004a84[];
extern const u8 Data_02004b44[];
extern const u8 Data_02004c04[];
extern const u8 Data_02004d3c[];
extern const u8 Data_02004e44[];
extern const u8 Data_02004fdc[];
extern const u8 Data_0200506c[];
extern const u8 Data_020050b4[];

const u8 *Func_020002f8(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov661Room1)
        return Data_020049dc;
    if (scene == (s32)&SceneId_Ov661Room2) {
        if (GameFlag_Test(0x8f0))
            return Data_02004b44;
        return Data_02004a84;
    }
    if (scene == (s32)&SceneId_Ov661Room3)
        return Data_02004c04;
    if (scene == (s32)&SceneId_Ov661Room4)
        return Data_02004d3c;
    if (scene == (s32)&SceneId_Ov661Room5)
        return Data_02004e44;
    if (scene == (s32)&SceneId_Ov661Room8)
        return Data_02004fdc;
    if (scene == (s32)&SceneId_Ov661Room10)
        return Data_0200506c;
    if (scene == (s32)&SceneId_Ov661Room12)
        return Data_020050b4;
    return Data_020049c4;
}
