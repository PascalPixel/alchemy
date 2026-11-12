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

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "main_routine_work.h"
#include "scene_ids.h"

extern const u8 gData_02001330[];
extern const u8 gData_02001390[];
extern const u8 gData_02001480[];
extern const u8 gData_02001504[];
extern const u8 gData_0200169c[];

const u8 *Func_02000350(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov6afRoom2)
    return gData_02001390;
  if (scene == (s32)&SceneId_Ov6afRoom3)
    return gData_02001480;
  if (scene == (s32)&SceneId_Ov6afRoom4)
    return gData_02001504;
  if (scene == (s32)&SceneId_Ov6afRoom5)
    return gData_0200169c;
  return gData_02001330;
}

#include "ram_buffer.h"
#include "map.h"

extern struct MapCell gMapCellBuffer[];

s32 Func_020003b4(u32 layer, s32 x, s32 y, s32 value) {
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

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"

void Func_020003fc(s32 a0, s32 a1) {
  MapGrid_CopyCellsLayered(0, 0x64, 3, 4,
                           (FIELD_AT((((a1 << 16) >> 12) + a0), s32, 0) + 0x3f),
                           (FIELD_AT((((a1 << 16) >> 12) + a0), s32, 4) - 2));
  MapGrid_CopyCellAttributes(1, 0, 1, 1,
                             FIELD_AT((((a1 << 16) >> 12) + a0), s32, 0),
                             FIELD_AT((((a1 << 16) >> 12) + a0), s32, 4));
  Event_SetFlag(FIELD_AT((((a1 << 16) >> 12) + a0), u16, 12));
}
