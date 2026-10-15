#include "local.h"

extern const u8 Data_02001378[];
extern const u8 Data_02001390[];
extern const u8 Data_020013a8[];
extern const u8 Data_020013d8[];
extern const u8 Data_02001498[];
extern const u8 Data_02001510[];
extern const u8 Data_02001540[];
extern const u8 Data_0200154c[];
extern const u8 Data_02001594[];
extern const u8 Data_020015c4[];
extern const u8 Data_020016cc[];
extern const u8 Data_02001714[];

const struct ScenePlacement *Func_02000de4(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov673Room1)
        return (const struct ScenePlacement *)Data_02001390;
    if (gPartyState.scene == (s32)&SceneId_Ov673Room2)
        return (const struct ScenePlacement *)Data_020013a8;
    if (gPartyState.scene == (s32)&SceneId_Ov673Room3)
        return (const struct ScenePlacement *)Data_020013d8;
    if (gPartyState.scene == (s32)&SceneId_Ov673Room4)
        return (const struct ScenePlacement *)Data_02001498;
    if (gPartyState.scene == (s32)&SceneId_Ov673Room5)
        return (const struct ScenePlacement *)Data_02001510;
    return (const struct ScenePlacement *)Data_02001378;
}

const struct SceneEvent *Func_02000e58(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov673Room1)
        return (const struct SceneEvent *)Data_0200154c;
    if (gPartyState.scene == (s32)&SceneId_Ov673Room2)
        return (const struct SceneEvent *)Data_02001594;
    if (gPartyState.scene == (s32)&SceneId_Ov673Room3)
        return (const struct SceneEvent *)Data_020015c4;
    if (gPartyState.scene == (s32)&SceneId_Ov673Room4)
        return (const struct SceneEvent *)Data_020016cc;
    if (gPartyState.scene == (s32)&SceneId_Ov673Room5)
        return (const struct SceneEvent *)Data_02001714;
    return (const struct SceneEvent *)Data_02001540;
}
