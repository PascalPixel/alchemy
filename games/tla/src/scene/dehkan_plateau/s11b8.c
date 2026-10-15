#include "types.h"
#include "party_state.h"
#include "scene_ids.h"

extern const u8 Data_020045e0[];
extern const u8 Data_020045f8[];
extern const u8 Data_020047d8[];
extern const u8 Data_02004ac0[];
extern const u8 Data_02004d78[];
extern const u8 Data_02004dd8[];
extern const u8 Data_02004e20[];
extern const u8 Data_02004f10[];
extern const u8 Data_02004f1c[];
extern const u8 Data_02005078[];
extern const u8 Data_020051bc[];
extern const u8 Data_020053b4[];
extern const u8 Data_02005450[];
extern const u8 Data_020055b8[];

const u8 *Func_020011b8(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov655Room1)
        return Data_020045f8;
    if (scene == (s32)&SceneId_Ov655Room2)
        return Data_020047d8;
    if (scene == (s32)&SceneId_Ov655Room3)
        return Data_02004ac0;
    if (scene == (s32)&SceneId_Ov655Room4)
        return Data_02004d78;
    if (scene == (s32)&SceneId_Ov655Room5)
        return Data_02004dd8;
    if (scene == (s32)&SceneId_Ov655Room6)
        return Data_02004e20;
    return Data_020045e0;
}

const u8 *Func_02001240(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov655Room1)
        return Data_02004f1c;
    if (scene == (s32)&SceneId_Ov655Room2)
        return Data_02005078;
    if (scene == (s32)&SceneId_Ov655Room3)
        return Data_020051bc;
    if (scene == (s32)&SceneId_Ov655Room4)
        return Data_020053b4;
    if (scene == (s32)&SceneId_Ov655Room5)
        return Data_02005450;
    if (scene == (s32)&SceneId_Ov655Room6)
        return Data_020055b8;
    return Data_02004f10;
}
