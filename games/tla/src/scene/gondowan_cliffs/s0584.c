#include "types.h"
#include "actor.h"
#include "scene_tables.h"
#include "event_inline.h"

s32 Map_GetTerrainHeight(s32, s32, s32);

void Func_02000584(void) {
  struct Actor *object = Actor_GetById(12);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  MapGrid_CopyCellAttributes(79, 28, 1, 1, 81, 28);
  GameFlag_SetBit(0x201);
}

void Func_020005bc(void) {
  struct Actor *object = Actor_GetById(13);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  object->terrainHeight = PIXELS(32);
  object->y = PIXELS(32);
  MapGrid_CopyCellAttributes(115, 35, 1, 1, 117, 35);
  GameFlag_SetBit(0x202);
}

void Func_02000600(void) { GameFlag_SetBit(0x885); }

void Func_02000610(s32 unused, s32 cell) {
  struct Actor *object;

  if (cell == 30) {
    object = Actor_GetById(17);
    MapGrid_CopyCellsLayered(124, 1, 1, 1, 119, 43);
    Actor_SetPosition(17, PIXELS(1912), PIXELS(728));
    object->terrainHeight = object->y =
        Map_GetTerrainHeight(0, object->x, object->z);
    GameFlag_SetBit(0x886);
  }
}
