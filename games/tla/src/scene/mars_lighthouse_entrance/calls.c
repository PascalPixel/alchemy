#include "ride.h"
#include "event_inline.h"
#include "field.h"
#include "module/scene/mars_lighthouse_entrance.h"
#include "module/scene/scene.h"

s32 Func_02000038(struct Actor *object) {
  Actor_SetSingleChildFlags(object, 0);
  return 0;
}

void Func_02000044(void) { Func_080d2b4c(11, 57); }

void Func_02000050(void) { Func_080d2b4c(17, 36); }

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

#include "room.h"

extern const u8 gData_02005af4[];
extern const u8 gData_02005b0c[];
extern const u8 gData_02005b54[];
extern const u8 gData_02005bcc[];
extern const u8 gData_02005d4c[];
extern const u8 gData_02005eb4[];
extern const u8 gData_02005f74[];
extern const u8 gData_020060c4[];

s32 Func_02000070(void) {
  Func_0200246c(0, 65);
  return 0;
}

const u8 *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom1)
    return gData_02005b0c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom2)
    return gData_02005b54;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom3)
    return gData_02005bcc;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom4)
    return gData_02005d4c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom5)
    return gData_02005eb4;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom6)
    return gData_02005f74;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom7)
    return gData_020060c4;
  return gData_02005af4;
}

void Func_02000118(void) {
  u8 *event = Ram_HeapSlots->eventWork;

  event[0xd38] = 1;
}

#include "map.h"

extern struct MapCell gMapCellBuffer[];

s32 Func_0200012c(u32 layer, s32 x, s32 y) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapCell *cell;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapCellBuffer;
  cell += x + (y << 7);
  return (cell->collisionCode << 8) | cell->attributeB;
}

s32 Func_02000168(u32 layer, s32 x, s32 y, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapCell *cell;
  s32 old;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapCellBuffer;
  cell += x + (y << 7);
  old = (cell->collisionCode << 8) | cell->attributeB;
  cell->collisionCode = value >> 8;
  cell->attributeB = value;
  return old;
}

#include "../../field/object/set_sprite_priority.c"
