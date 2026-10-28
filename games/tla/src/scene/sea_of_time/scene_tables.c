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

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gData_020021e4[];
extern const u8 gData_02002214[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gAdventure.scene == (s32)&SceneId_Ov687Room2)
    return (const struct ActorPlacement *)gData_02002214;
  return (const struct ActorPlacement *)gData_020021e4;
}
