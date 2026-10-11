#include "ov686.h"

extern const u8 Data_020032a0[];
extern const u8 Data_020032b8[];
extern const u8 Data_020034f8[];
extern const u8 Data_02003570[];
extern const u8 Data_020035d0[];
extern const u8 Data_02003708[];
extern const u8 Data_02003750[];
extern const u8 Data_02003768[];

const u8 *Func_02000600(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov686Room1)
        return Data_020032b8;
    if (gPartyState.scene == (s32)&SceneId_Ov686Room2)
        return Data_020034f8;
    if (gPartyState.scene == (s32)&SceneId_Ov686Room3)
        return Data_02003570;
    if (gPartyState.scene == (s32)&SceneId_Ov686Room4)
        return Data_020035d0;
    if (gPartyState.scene == (s32)&SceneId_Ov686Room5)
        return Data_02003708;
    if (gPartyState.scene == (s32)&SceneId_Ov686Room6)
        return Data_02003750;
    if (gPartyState.scene == (s32)&SceneId_Ov686Room8)
        return Data_02003768;
    return Data_020032a0;
}

void Func_02000698(void)
{
    Object_GetById(gPartyState.current_owner)->terrain_id = 2;
}

void Func_020006b4(void)
{
    Object_GetById(gPartyState.current_owner)->terrain_id = 0;
}
