#include "ride.h"
#include "event_inline.h"
#include "field.h"

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

extern char SceneId_Ov6aaRoom1;
extern char SceneId_Ov6aaRoom2;
extern char SceneId_Ov6aaRoom3;
extern char SceneId_Ov6aaRoom4;
extern char SceneId_Ov6aaRoom5;
extern char SceneId_Ov6aaRoom6;
extern char SceneId_Ov6aaRoom7;
extern const u8 gData_02005af4[];
extern const u8 gData_02005b0c[];
extern const u8 gData_02005b54[];
extern const u8 gData_02005bcc[];
extern const u8 gData_02005d4c[];
extern const u8 gData_02005eb4[];
extern const u8 gData_02005f74[];
extern const u8 gData_020060c4[];

void Func_02000414(s32 id);
void Func_02000798(void);
void Func_020019c4(void);
void Func_020046b4(void);
void Func_02004718(void);
void Func_02004dc8(void);
void Func_02004dd0(void);

s32 Func_02000070(void) {
  Func_0200246c(0, 65);
  return 0;
}

const u8 *Func_02000080(void) {
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
