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

extern const u8 gData_02000f5c[];
extern const u8 gData_02000f8c[];

const struct ActorPlacement *Func_02000040(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov680Room1)
    return (const struct ActorPlacement *)gData_02000f5c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov680Room2)
    return (const struct ActorPlacement *)gData_02000f8c;
  return 0;
}
