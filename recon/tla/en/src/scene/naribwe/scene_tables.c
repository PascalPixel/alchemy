#include "local.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const u8 *Func_0200004c(void) {
  if (gMainRoutineWork.entrance == 10)
    return gData_020008c4;
  if (GameFlag_Test(0x8ff))
    return gData_02000954;
  return gData_020006cc;
}
