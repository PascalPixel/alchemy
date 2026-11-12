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

#include "caverns/caverns.h"

const u8 *Func_02000344(void) {
  const u8 *result = NULL;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6a9Room2)
    result = gData_02001264;
  return result;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const u8 *Func_02000374(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov6a9Room1)
    return gData_02001358;
  if (scene == (s32)&SceneId_Ov6a9Room4)
    return gData_020013b8;
  return gData_02001328;
}

s32 Func_020003b4(u32 layer, s32 x, s32 y) {
  struct CavernsMapWork *map = (struct CavernsMapWork *)Ram_HeapSlots->mapWork;
  u8 *cell;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapCellBuffer;
  cell += (x + (y << 7)) << 2;
  return (cell[2] << 8) | cell[3];
}

s32 Func_020003f0(u32 layer, s32 x, s32 y, s32 value) {
  struct CavernsMapWork *map = (struct CavernsMapWork *)Ram_HeapSlots->mapWork;
  u8 *cell;
  s32 old;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapCellBuffer;
  cell += (x + (y << 7)) << 2;
  old = (cell[2] << 8) | cell[3];
  cell[2] = value >> 8;
  cell[3] = value;
  return old;
}

#include "../field/object/wait_until_idle.c"
