/// @file
/// The scene code of overlay 0x67f that its scene tables belong to, in ROM
/// order, and the overlay's data; the rest of the overlay is in izumo_ruins/.

#include "shared/actors.h"
#include "shared/tables.h"
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
#include "module/scene/izumo_ruins.h"

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

  if (Event_GetFlag(object->timer)) {
    upper |= 0xff00;
    lower &= ~0x80;
    if (object->terrainHeight >> 19 != height)
      height -= 4;
    Scene_SetActorSpritePriority((struct Actor *)object, 3);
  } else {
    if (lower >> 8 != 232)
      upper = (upper & 0xff) | 0xff00;
    else
      upper = (upper & 0xff) | (232 << 8);
    lower |= 0x80;
    height += 4;
    Scene_SetActorSpritePriority((struct Actor *)object, 2);
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
    object->timer = flag;
    Func_02000f30(object);
    list += 3;
  }
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const u32 *Scene_GetRegions(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov67FRoom1)
    return gData_020016d0;
  if (scene == (s32)&SceneId_Ov67FRoom2)
    return gData_020016f0;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
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

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Func_02001500(gData_02001696);
  object = work->objectCbc;
  object->priorityFlags |= 1;
  object->priorityFlags &= ~2;
  Event_End();
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

DEFINE_LEADER_SET_LAYER(Leader_SetLayer, 0)

/// Outside the first room, sets the high byte of map cell (50, 13) on layer 2
/// to 0xff; then starts the room's effect.
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
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Func_02001520();
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov67FRoom2)
    Func_02000ee8(2, 50, 13, Func_02000eac(2, 50, 13) & 255);
  Event_End();
}

void Func_02001270(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(7, 2, 8, 7, 7, 12);
  MapGrid_CopyCellsTo(7, 66, 7, 76, 8, 9);
  Event_SetFlag(0x941);
  Event_End();
}

const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov67FRoom1)
    return gData_020018ec;
  return gData_020019ac;
}

s32 Scene_Initialize(void) {
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
      Map_CenterOnFollowedPoint();
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

#include "objcmd.h"

// clang-format off
DEFINE_G_DATA_02001540(gData_02001540, 33)

DEFINE_G_DATA_02001540(gData_02001570, 44)

DEFINE_G_DATA_020012A4(gData_020015a0, 22)

DEFINE_G_DATA_020012A4(gData_020015dc, 44)

DEFINE_G_DATA_020012A4(gData_02001618, 126)

s16 gData_02001654[] = {
    13, -1,
};

s16 gData_02001658[] = {
    10, 11, 12, 13, 14, 15, -1, 0,
};

s32 gData_02001668[] = {
    (s32)gData_020015a0,
    (s32)gData_020015dc,
    (s32)gData_02001618,
};

s16 gData_02001674[] = {
    8, 0, 512, 9, 0, 513, 12, 0,
    514, -1,
};

s16 gData_02001688[] = {
    8, 0, 512, 9, 0, 513, -1,
};

s16 gData_02001696[] = {
    10, 515, 11, 516, -1,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gData_020016d0[] = {
    0x2c00d0, 0xe002f0, 0x300003c, 0x1ffff, 0xffff, 0,
    0, 0,
};

u32 gData_020016f0[] = {
    0x100190, 0x1a00160, 0x1700020, 0x1ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(172),
    SCENE_EXIT_IF(1, 171, 10, -1),
    SCENE_EXIT_IF(2, 172, 3, -1),
    SCENE_EXIT_IF(3, 172, 2, -1),
    SCENE_EXIT_IF(4, 173, 1, -1),
    SCENE_EXITS(173),
    SCENE_EXIT_IF(1, 172, 4, -1),
    SCENE_EXIT_IF(2, 173, 3, -1),
    SCENE_EXIT_IF(3, 173, 2, -1),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_02001754[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_0200176c[] = {
    {406, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(824), FACING_NORTH, 2, 0},
    {406, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(824), FACING_NORTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(824), FACING_NORTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(824), FACING_NORTH, 2, 0},
    {406, CONDITION_ALWAYS, 1, PIXELS(696), 0, PIXELS(248), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(88), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02001814[] = {
    {406, CONDITION_ALWAYS, 1, PIXELS(440), 0, PIXELS(552), FACING_NORTH, 2, 0},
    {406, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(552), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(792), 0, PIXELS(152), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(824), 0, PIXELS(152), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(760), 0, PIXELS(184), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(792), 0, PIXELS(216), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(824), 0, PIXELS(216), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(856), 0, PIXELS(184), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gData_020018ec[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8, 0x200, EVENT(Func_020010f0)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 9, 0x201, EVENT(Func_020010f0)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 12, 0x202, EVENT(Func_020010f0)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 10, 0x203, EVENT(Func_02001148)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, 0x204, EVENT(Func_02001148)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(Func_02001184)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Leader_SetLayer)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 13, CONDITION_ALWAYS, EVENT(Func_020011d4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, CONDITION_ALWAYS, EVENT(Func_02001228)},
    {8, 0, CONDITION_ALWAYS, EVENT(Func_020011d4)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02001228)},
    {TRIGGERS_END},
};

struct Trigger gData_020019ac[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8, 0x200, EVENT(Func_020010f0)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 9, 0x201, EVENT(Func_020010f0)},
    {0x50008a05, 50, CONDITION_ALWAYS, EVENT(Func_02001270)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 10, CONDITION_ALWAYS, EVENT(Func_020011d4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, EVENT(Func_02001228)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 11, CONDITION_ALWAYS, EVENT(Func_020011d4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS, EVENT(Func_02001228)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12, CONDITION_ALWAYS, EVENT(Func_020011d4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS, EVENT(Func_02001228)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 13, CONDITION_ALWAYS, EVENT(Func_020011d4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, CONDITION_ALWAYS, EVENT(Func_02001228)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 14, CONDITION_ALWAYS, EVENT(Func_020011d4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, CONDITION_ALWAYS, EVENT(Func_02001228)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15, CONDITION_ALWAYS, EVENT(Func_020011d4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS, EVENT(Func_02001228)},
    {TRIGGERS_END},
};
