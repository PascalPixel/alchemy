#include "types.h"
#include "party_state.h"
#include "scene_ids.h"

extern const u8 Data_0200554c[];
extern const u8 Data_02005558[];
extern const u8 Data_02005588[];
extern const u8 Data_020057f8[];
extern const u8 Data_02005a20[];
extern const u8 Data_02005da4[];
extern const u8 Data_02005eac[];

const u8 *Func_02000598(void)
{
    s32 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov6a8Room1)
        return Data_02005558;
    if (scene == (s32)&SceneId_Ov6a8Room2)
        return Data_02005588;
    if (scene == (s32)&SceneId_Ov6a8Room3)
        return Data_020057f8;
    if (scene == (s32)&SceneId_Ov6a8Room4)
        return Data_02005a20;
    if (scene == (s32)&SceneId_Ov6a8Room5)
        return Data_02005da4;
    if (scene == (s32)&SceneId_Ov6a8Room6)
        return Data_02005eac;
    return Data_0200554c;
}
