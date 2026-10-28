#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "scene_ids.h"

extern const u8 gData_02003df4[];
extern const u8 gData_02003e14[];
extern const u8 gData_02003e44[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) {
  s16 scene = gSequenceParentWork.scene;

  if (scene == (s32)&SceneId_ShrineOfTheSeaGod9)
    return (const struct SceneRegion *)gData_02003df4;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod10)
    return (const struct SceneRegion *)gData_02003e14;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod11)
    return (const struct SceneRegion *)gData_02003e44;
  return 0;
}
