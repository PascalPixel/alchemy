#include "../field/map/cell_get.c"

#include "types.h"
#include "actor.h"
#include "event_inline.h"
#include "field.h"

s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
s32 Func_020005cc(s32 layer, s32 x, s32 z, s32 height);

void Func_02000668(u16 *list) {
  for (; *list != 0xffff; list += 3) {
    struct Actor *object = Actor_Get(*list);
    s32 height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);

    object->motionFlags = 0;
    object->collisionFlags = 0;
    Actor_SetSingleChildFlags(object, 0);
    Func_020005cc(object->terrainId, object->x >> 20, object->z >> 20,
                  (height >> 19) + 4);
  }
}

#include "battle.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"
#include "magma_rock/magma_rock.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "scene_ids.h"

// For each id of a list ended by 0xffff: clears the object's mark and, on a
// cell with attribute bit 6, blocks the cells ahead of and behind it and sets
// the mark.
#include "map.h"

s32 MapCell_GetAttributes(u32 layer, s32 x, s32 z);

void Func_020006cc(u16 *ids) {
  struct Actor *object;
  u8 *blocked;
  struct MapWork *map;
  struct MapCell **cells;
  struct MapCell *cell;
  s32 x, z;

  while (*ids != 0xffff) {
    object = Actor_Get(*ids);
    blocked = (u8 *)object + 0x63;
    *blocked = 0;
    if (MapCell_GetAttributes(0, object->x >> 20, object->z >> 20) & 0x40) {
      map = Ram_HeapSlots->mapWork;
      if (map == NULL)
        return;
      cells = &map->layers[2].cells;
      z = (object->z - object->y) >> 20;
      x = object->x >> 20;
      cell = *cells;
      cell += x + ((z + 1) << 7);
      cell->collisionCode = 255;
      cell = *cells;
      cell += x + ((z - 1) << 7);
      cell->collisionCode = 255;
      *blocked = 1;
    }
    ids++;
  }
}

// Clears the object's mark and the cells ahead of and behind it that the mark
// blocked.

struct Object0200074c {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x4f];
  u8 marked;
};

void Func_0200074c(struct Object0200074c *object) {
  struct MapWork *map;
  struct MapCell **cells;
  struct MapCell *cell;
  s32 x;
  s32 z;

  if (object->marked == 0)
    return;
  map = Ram_HeapSlots->mapWork;
  if (map == NULL)
    return;
  cells = &map->layers[2].cells;
  cell = *cells;
  x = object->x >> 20;
  z = (object->z - object->y) >> 20;
  cell += x + ((z + 1) << 7);
  cell->collisionCode = 0;
  cell = *cells;
  cell += x + ((z - 1) << 7);
  cell->collisionCode = 0;
  object->marked = 0;
}

// For each id of a list ended by 0xffff: settles the object on its ground,
// copies the cell under it, and blocks the cells ahead of and behind it on a
// cell with attribute bit 6.
#include "iwram_call.h"
#include "text/msg_ids.h"

/// For each id of a list ended by 0xffff: settles the object on its ground,
/// copies the cell under it, and blocks the cells ahead of and behind it on a
/// cell with attribute bit 6.
void Func_0200079c(u16 *ids) {
  while (*ids != 0xffff) {
    struct Actor *object = Actor_Get(*ids);
    u8 *terrainId = &object->terrainId;
    s32 height = Map_GetTerrainHeight(*terrainId, object->x, object->z);
    u8 *blocked;
    struct MapWork *map;
    struct MapCell **cells;
    struct MapCell *cell;
    s32 x, z;

    object->unk4c =
        255 & MapCell_GetAttributes(0, object->x >> 20, object->z >> 20);
    object->motionFlags = 0;
    object->collisionFlags = 0;
    Actor_SetSingleChildFlags(object, 0);
    Func_020005cc(*terrainId, object->x >> 20, object->z >> 20,
                  (height >> 19) + 4);
    blocked = (u8 *)object + 99;
    *blocked = 0;
    if (MapCell_GetAttributes(0, object->x >> 20, object->z >> 20) & 64) {
      map = Ram_HeapSlots->mapWork;
      if (map == NULL)
        return;
      cells = &map->layers[2].cells;
      z = (object->z - object->y) >> 20;
      x = object->x >> 20;
      cell = *cells;
      cell += x + ((z + 1) << 7);
      cell->collisionCode = 255;
      cell = *cells;
      cell += x + ((z - 1) << 7);
      cell->collisionCode = 255;
      *blocked = 1;
    }
    ids++;
  }
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern const u8 gData_02003ea8[];
extern const u8 gData_02003ed8[];
extern const u8 gData_02003f88[];

const struct SceneEntrance *Func_0200088c(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room2)
    return (const struct SceneEntrance *)gData_02003ed8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room3)
    return (const struct SceneEntrance *)gData_02003f88;
  return (const struct SceneEntrance *)gData_02003ea8;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gData_0200403c[];
extern const u8 gData_020041a4[];
extern const u8 gData_0200427c[];
extern const u8 gData_020043e4[];
extern const u8 gData_020044a4[];
extern const u8 gData_020045c4[];

const struct Trigger *Func_020008d4(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room2) {
    if (gMainRoutineWork.entrance <= 3)
      return (const struct Trigger *)gData_020041a4;
    if (gMainRoutineWork.entrance <= 8)
      return (const struct Trigger *)gData_0200427c;
    if (gMainRoutineWork.entrance <= 12)
      return (const struct Trigger *)gData_020043e4;
    return (const struct Trigger *)gData_020044a4;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room3)
    return (const struct Trigger *)gData_020045c4;
  return (const struct Trigger *)gData_0200403c;
}

extern s16 gData_02003d98[];
extern s16 gData_02003dac[];
extern s16 gData_02003dd2[];
extern s16 gData_02003de4[];
extern s16 gData_02003de8[];

void Func_02003c14(s16 *table);

void Func_02000940(void) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room1) {
    MapGrid_CopyCellAttributes(98, 18, 4, 3, 78, 21);
    MapGrid_CopyCellAttributes(98, 24, 4, 3, 78, 24);
    Func_02003c14(gData_02003d98);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room2) {
    if (gMainRoutineWork.entrance <= 3) {
      Func_02003c14(gData_02003dac);
    } else if (gMainRoutineWork.entrance <= 7) {
      Func_02003c14(gData_02003dd2);
    } else if (gMainRoutineWork.entrance <= 11) {
      MapGrid_CopyCellAttributes(79, 46, 2, 1, 79, 36);
      Func_02003c14(gData_02003de4);
    } else {
      if (((s16 *)&work->target->z)[1] == 600)
        MapGrid_CopyCellAttributes(104, 38, 1, 1, 103, 38);
      else
        MapGrid_CopyCellAttributes(94, 44, 5, 1, 99, 38);
      Func_02003c14(gData_02003de8);
    }
  }
  Func_0200074c(*(struct Actor **)(event + 0xcbc));
}

void Func_02003aec();
void Func_02003c1c();
s32 Func_02003af4();

/// Rebuilds the room's opened passages on entry and sets the flag of the cell
/// the leader stands on.
s32 Func_02000a48(void) {
  struct Actor *leader =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 3260);
  s32 x, z;

  Func_02003aec();
  FieldEvent_ResolvePendingActions(0);
  Func_02003c1c();
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room1) {
    MapGrid_CopyCellAttributes(98, 21, 4, 3, 78, 21);
    MapGrid_CopyCellAttributes(98, 27, 4, 3, 78, 24);
    Func_020006cc(gData_02003d98);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room2) {
    if (gMainRoutineWork.entrance <= 3) {
      Func_020006cc(gData_02003dac);
    } else if (gMainRoutineWork.entrance <= 8) {
      Func_020006cc(gData_02003dd2);
    } else if (gMainRoutineWork.entrance <= 12) {
      /* FAKEMATCH(wrapper): forwarded inline, so that its stack arguments build
       * in order; as a direct call, 8 of 187 instructions differ in tla-en. */
      MapGrid_CopyCellAttributes(79, 37, 2, 1, 79, 36);
      Func_020006cc(gData_02003de4);
    } else {
      MapGrid_CopyCellAttributes(103, 39, 1, 1, 103, 38);
      MapGrid_CopyCellAttributes(99, 42, 5, 1, 99, 38);
      Func_020006cc(gData_02003de8);
    }
  }
  x = leader->x >> 20;
  z = leader->z >> 20;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room1) {
    if (x == 15 && z == 25)
      Event_SetFlag(0x8d6);
    else if (x == 16 && z == 22)
      Event_SetFlag(0x8d5);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room2) {
    if (gMainRoutineWork.entrance <= 3) {
      if (x == 27 && z == 33)
        Event_SetFlag(0x8d7);
    } else if (gMainRoutineWork.entrance <= 8) {
      if (x == 55 && z == 35)
        Event_SetFlag(0x8d8);
    } else if (gMainRoutineWork.entrance <= 12) {
      if (x == 16 && z == 36)
        Event_SetFlag(0x8d9);
    } else if (x == 40 && z == 37) {
      Event_SetFlag(0x8da);
    } else if (x == 35 && z == 38) {
      Event_SetFlag(0x8db);
    }
  }
  return Func_02003af4();
}
