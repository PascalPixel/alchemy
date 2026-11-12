/// @file
/// The scene code of overlay 0x65d that its scene tables belong to, in ROM order; the rest of the overlay is in mikasalla/.

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
#include "main_routine_work.h"
#include "scene_ids.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern s16 gData_02000794[];

s16 *Func_02000040(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov65dRoom2)
    return gData_02000794;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern s32 gData_02000fa0[];
extern u32 gData_020010c0[];

void *Func_02000070(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov65dRoom1)
    return gData_02000fa0;
  return gData_020010c0;
}
