#include "local.h"

extern const u8 gData_02001378[];
extern const u8 gData_02001390[];
extern const u8 gData_020013a8[];
extern const u8 gData_020013d8[];
extern const u8 gData_02001498[];
extern const u8 gData_02001510[];
extern const u8 gData_02001540[];
extern const u8 gData_0200154c[];
extern const u8 gData_02001594[];
extern const u8 gData_020015c4[];
extern const u8 gData_020016cc[];
extern const u8 gData_02001714[];

const struct ScenePlacement *Func_02000de4(void) {
  if (gPartyState.scene == (s32)&SceneId_Ov673Room1)
    return (const struct ScenePlacement *)gData_02001390;
  if (gPartyState.scene == (s32)&SceneId_Ov673Room2)
    return (const struct ScenePlacement *)gData_020013a8;
  if (gPartyState.scene == (s32)&SceneId_Ov673Room3)
    return (const struct ScenePlacement *)gData_020013d8;
  if (gPartyState.scene == (s32)&SceneId_Ov673Room4)
    return (const struct ScenePlacement *)gData_02001498;
  if (gPartyState.scene == (s32)&SceneId_Ov673Room5)
    return (const struct ScenePlacement *)gData_02001510;
  return (const struct ScenePlacement *)gData_02001378;
}

const struct SceneEvent *Func_02000e58(void) {
  if (gPartyState.scene == (s32)&SceneId_Ov673Room1)
    return (const struct SceneEvent *)gData_0200154c;
  if (gPartyState.scene == (s32)&SceneId_Ov673Room2)
    return (const struct SceneEvent *)gData_02001594;
  if (gPartyState.scene == (s32)&SceneId_Ov673Room3)
    return (const struct SceneEvent *)gData_020015c4;
  if (gPartyState.scene == (s32)&SceneId_Ov673Room4)
    return (const struct SceneEvent *)gData_020016cc;
  if (gPartyState.scene == (s32)&SceneId_Ov673Room5)
    return (const struct SceneEvent *)gData_02001714;
  return (const struct SceneEvent *)gData_02001540;
}
