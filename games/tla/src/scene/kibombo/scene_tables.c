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

extern u8 gData_0200635c[];

u8 *Func_020000fc(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Kibombo3)
    return gData_0200635c;
  return 0;
}
