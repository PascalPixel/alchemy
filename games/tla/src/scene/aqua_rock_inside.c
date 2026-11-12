/// @file
/// The scene code of overlay 0x67c that its scene tables belong to, in ROM order; the rest of the overlay is in aqua_rock_inside/.

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

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gData_02004688[];
extern const u8 gData_020046a0[];
extern const u8 gData_02004748[];
extern const u8 gData_020047a8[];
extern const u8 gData_02004808[];
extern const u8 gData_020048c8[];
extern const u8 gData_02004910[];
extern const u8 gData_020049e8[];
extern const u8 gData_02004a00[];
extern const u8 gData_02004a48[];
extern const u8 gData_02004b98[];

const u8 *Func_0200035c(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov67cRoom1)
    return gData_020046a0;
  if (scene == (s32)&SceneId_Ov67cRoom2)
    return gData_02004748;
  if (scene == (s32)&SceneId_Ov67cRoom3)
    return gData_020047a8;
  if (scene == (s32)&SceneId_Ov67cRoom4)
    return gData_02004808;
  if (scene == (s32)&SceneId_Ov67cRoom5)
    return gData_020048c8;
  if (scene == (s32)&SceneId_Ov67cRoom6)
    return gData_02004910;
  if (scene == (s32)&SceneId_Ov67cRoom7)
    return gData_020049e8;
  if (scene == (s32)&SceneId_Ov67cRoom8)
    return gData_02004a00;
  if (scene == (s32)&SceneId_Ov67cRoom9)
    return gData_02004a48;
  if (scene == (s32)&SceneId_Ov67cRoom10)
    return gData_02004b98;
  return gData_02004688;
}
