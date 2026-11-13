/// @file
/// The scene code of overlay 0x6aa, in ROM order.

#include "shared/map_cells.h"
#include "mars_lighthouse_entrance.h"
#include "event_inline.h"
#include "field.h"
#include "module/scene/mars_lighthouse_entrance.h"
#include "module/scene/scene.h"

s32 Func_02000038(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

void Func_02000044(void) { Field_RequestDjinnBattle(11, 57); }

void Func_02000050(void) { Field_RequestDjinnBattle(17, 36); }

#include "battle.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "shared/mars_lighthouse_entrance_room.h"

extern const u8 gMarsLighthouseEntrance_Placements[];
extern const u8 gMarsLighthouseEntrance_Room1Placements[];
extern const u8 gMarsLighthouseEntrance_Room2Placements[];
extern const u8 gMarsLighthouseEntrance_Room3Placements[];
extern const u8 gMarsLighthouseEntrance_Room4Placements[];
extern const u8 gMarsLighthouseEntrance_Room5Placements[];
extern const u8 gMarsLighthouseEntrance_Room6Placements[];
extern const u8 gMarsLighthouseEntrance_Room7Placements[];

s32 Func_02000070(void) {
  Func_0200246c(0, 65);
  return 0;
}

const u8 *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom1)
    return gMarsLighthouseEntrance_Room1Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom2)
    return gMarsLighthouseEntrance_Room2Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom3)
    return gMarsLighthouseEntrance_Room3Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom4)
    return gMarsLighthouseEntrance_Room4Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom5)
    return gMarsLighthouseEntrance_Room5Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom6)
    return gMarsLighthouseEntrance_Room6Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom7)
    return gMarsLighthouseEntrance_Room7Placements;
  return gMarsLighthouseEntrance_Placements;
}

void Func_02000118(void) {
  u8 *event = Ram_HeapSlots->eventWork;

  event[0xd38] = 1;
}

#include "map.h"

extern struct MapCell gMapCellBuffer[];

DEFINE_MAP_CELL_GET(Func_0200012c)

DEFINE_MAP_CELL_SWAP(Func_02000168)

#include "../field/object/set_sprite_priority.c"
