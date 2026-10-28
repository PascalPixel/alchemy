#include "local.h"

void Func_02000038(void) { Func_02002940_(8, 11); }

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gData_02002c54[];
extern const u8 gData_02002c6c[];
extern const u8 gData_02002d2c[];
extern const u8 gData_02002d5c[];
extern const u8 gData_02002dbc[];
extern const u8 gData_02002e04[];
extern const u8 gData_02002e64[];
extern const u8 gData_02002f9c[];
extern const u8 gData_02002fe4[];

const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 scene = gAdventure.scene;

  if (scene == (s32)&SceneId_Ov678Room2)
    return (const struct ScenePlacement *)gData_02002c6c;
  if (scene == (s32)&SceneId_Ov678Room4)
    return (const struct ScenePlacement *)gData_02002d2c;
  if (scene == (s32)&SceneId_Ov678Room5)
    return (const struct ScenePlacement *)gData_02002d5c;
  if (scene == (s32)&SceneId_Ov678Room7)
    return (const struct ScenePlacement *)gData_02002dbc;
  if (scene == (s32)&SceneId_Ov678Room8)
    return (const struct ScenePlacement *)gData_02002e04;
  if (scene == (s32)&SceneId_Ov678Room9)
    return (const struct ScenePlacement *)gData_02002f9c;
  if (scene == (s32)&SceneId_Ov678Room10)
    return (const struct ScenePlacement *)gData_02002e64;
  if (scene == (s32)&SceneId_Ov678Room11)
    return (const struct ScenePlacement *)gData_02002fe4;
  return (const struct ScenePlacement *)gData_02002c54;
}
