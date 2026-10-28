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

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

#include "scene_ids.h"

extern const u8 gData_020033d0[];
extern const u8 gData_020033f0[];

const struct ActorPlacement *Func_02000488(void) {
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov68fRoom6)
    return (const struct ActorPlacement *)gData_020033d0;
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov68fRoom7)
    return (const struct ActorPlacement *)gData_020033f0;
  return 0;
}
