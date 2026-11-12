/// @file
/// The scene code of overlay 0x67f that its scene tables belong to, in ROM order; the rest of the overlay is in izumo_ruins/.

#include "izumo_ruins/izumo_ruins.h"
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
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"
#include "event_inline.h"

s32 Func_02000eac(u32 layer, s32 x, s32 y) {
  struct IzumoRuinsMapWork *map =
      (struct IzumoRuinsMapWork *)Ram_HeapSlots->mapWork;
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

s32 Func_02000ee8(u32 layer, s32 x, s32 y, s32 value) {
  struct IzumoRuinsMapWork *map =
      (struct IzumoRuinsMapWork *)Ram_HeapSlots->mapWork;
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

void Func_02001480(s32 x, s32 z, s32 layer, s32 height);

/// Updates the cells under a pillar object by its flag (the object's action):
/// set, the upper layer's cell is marked 0xff in its high byte, the lower
/// cell loses bit 7, the height drops 4 when the object's ground height is not
/// the map's and the sprite takes priority 3; clear, the upper cell's high byte
/// becomes 232 where the lower cell's is 232 and 0xff otherwise, the lower cell
/// gains bit 7, the height rises 4 and the sprite takes priority 2. Both cells
/// are written back and Func_02001480 is given the cell's position and height.
void Func_02000f30(struct Actor *object) {
  s32 column = object->x >> 20;
  s32 row = object->z >> 20;
  s32 lower = Func_02000eac(0, column, row);
  s32 upper = Func_02000eac(2, column, row);
  s32 height =
      Map_GetTerrainHeight(object->terrainId, object->x, object->z) >> 19;

  if (Event_GetFlag(object->action)) {
    upper |= 0xff00;
    lower &= ~0x80;
    if (object->terrainHeight >> 19 != height)
      height -= 4;
    Scene_SetActorSpritePriority((struct FieldActor *)object, 3);
  } else {
    if (lower >> 8 != 232)
      upper = (upper & 0xff) | 0xff00;
    else
      upper = (upper & 0xff) | (232 << 8);
    lower |= 0x80;
    height += 4;
    Scene_SetActorSpritePriority((struct FieldActor *)object, 2);
  }
  Func_02000ee8(0, column, row, lower);
  Func_02000ee8(2, column, row, upper);
  Func_02001480(column << 20, row << 20, object->terrainId, height);
}

void Func_0200100c(u16 *list) {
  struct Actor *object;
  s32 flag;

  while (list[0] != 0xffff) {
    object = Actor_Get(list[0]);
    flag = (s16)list[2];
    Actor_SetSingleChildFlags(object, 0);
    Func_02001428(object, Event_GetFlag((u16)flag) + 1);
    object->motionFlags = 0;
    object->collisionFlags &= 0xfe;
    object->action = flag;
    Func_02000f30(object);
    list += 3;
  }
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

u8 *Func_02001070(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov67FRoom1)
    return gData_020016d0;
  if (scene == (s32)&SceneId_Ov67FRoom2)
    return gData_020016f0;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

u8 *Func_020010b0(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov67FRoom1)
    return gData_0200176c;
  if (scene == (s32)&SceneId_Ov67FRoom2)
    return gData_02001814;
  return gData_02001754;
}

/// Starts the effect for this scene's room, then, for a first argument of 1,
/// runs Func_02000f30 on the effect work's object.
void Func_020010f0(s32 aArg, s32 b) {
  struct IzumoRuinsEffectWork *work = Ram_HeapSlots->effectWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  s16 scene;

  scene = party->scene;
  if (scene == (s32)&SceneId_Ov67FRoom1)
    Func_020000a4(gData_02001674, aArg, b);
  else
    Func_020000a4(gData_02001688, aArg, b);
  if (aArg == 1)
    Func_02000f30(Actor_Get(work->objectId));
}

void Func_02001148(void) {
  struct IzumoRuinsEventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *object;

  Func_02001488();
  FieldEvent_ResolvePendingActions(0);
  Func_02001500(gData_02001696);
  object = work->objectCbc;
  object->priorityFlags |= 1;
  object->priorityFlags &= ~2;
  Func_02001490();
}


void Func_02001184(void) {
  struct Actor *object0;
  s32 result1;

  object0 = Actor_Get(gMainRoutineWork.currentOwner);
  result1 = Value3((s32 (*)())Func_02000eac, 2, (object0->x >> 20),
                   (object0->z >> 20));
  if ((result1 >> 8) != 232)
    goto L_020091b2;
  object0->terrainId = 2;
L_020091b2:
  return;
}

void Func_020011b8(void) {
  struct Actor *object0;

  object0 = Actor_Get(gMainRoutineWork.currentOwner);
  object0->terrainId = 0;
  return;
}

/// Outside the first room, sets the high byte of map cell (50, 13) on layer 2 to
/// 0xff; then starts the room's effect.
void Func_020011d4(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov67FRoom1) {
    Func_02001518(gData_02001654);
  } else {
    /* FAKEMATCH(signature): the cell read through an s16 return type, so the
     * old cell is masked in r3 and 0xff00 built in r2; read as s32 the two
     * registers swap; as the header's s32 return, 6 of 29 instructions differ
     * in tla-en. */
    Func_02000ee8(
        2, 50, 13,
        0xff00 | (((s16 (*)(u32, s32, s32))Func_02000eac)(2, 50, 13) & 255));
    Func_02001518(gData_02001658);
  }
}

void Func_02001228(void) {
  Func_02001488();
  FieldEvent_ResolvePendingActions(0);
  Func_02001520();
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov67FRoom2)
    Func_02000ee8(2, 50, 13, Func_02000eac(2, 50, 13) & 255);
  Func_02001490();
}

void Func_02001270(void) {
  Func_02001488();
  MapGrid_CopyCellAttributes(7, 2, 8, 7, 7, 12);
  MapGrid_CopyCellsTo(7, 66, 7, 76, 8, 9);
  Event_SetFlag(0x941);
  Func_02001490();
}

u8 *Func_020012b0(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov67FRoom1)
    return gData_020018ec;
  return gData_020019ac;
}

s32 Func_020012dc(void) {
  struct IzumoRuinsEventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *object;
  s32 height;

  work->field1ac = 0x204;
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 0x20;
  Func_020014e0(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov67FRoom1) {
    Func_02001510(gData_02001654);
    Func_0200100c((u16 *)gData_02001674);
    Func_020014f8(gData_02001696);
  } else {
    Actor_Get(10)->priorityFlags |= 0x20;
    Actor_Get(11)->priorityFlags |= 0x20;
    Func_02001510(gData_02001658);
    Func_0200100c((u16 *)gData_02001688);
    if (Event_GetFlag(0x941)) {
      MapGrid_CopyCellAttributes(7, 2, 8, 7, 7, 12);
      MapGrid_CopyCellsTo(7, 66, 7, 76, 8, 9);
      Func_02001448();
      System_Wait(1);
      if (Event_GetFlag(0x109)) {
        object = Actor_Get(gMainRoutineWork.currentOwner);
        height = Map_GetTerrainHeight(object->terrainId, object->x,
                                      object->z - object->y);
        object->terrainHeight = height;
        object->y = height;
        FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 0);
      }
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }
