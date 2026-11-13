/// @file
/// The Izumo Ruins, under the stone circle of Izumo, in two rooms that share
/// this overlay (SceneId_IzumoRuinsFirstRoom, SceneId_IzumoRuinsSecondRoom).
/// Sand carries the party across the sandy floor (IzumoRuins_SinkIntoSand),
/// Frost freezes the puddles (IzumoRuins_FreezePuddle), Pound sinks pillars
/// (IzumoRuins_PoundPillar), Move sets the brown pillars in place
/// (IzumoRuins_FinishMovingPillar), and Parch dries the pool before the room
/// of the stone tablet (IzumoRuins_ParchThePool).
///
/// The pillars, the rubble and the effect spawner are in izumo_ruins/, which
/// shared field effect objects link between.

#include "flags.h"
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

struct Actor;
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

// Set as Parch dries the pool before the room of the stone tablet.
#define FLAG_IZUMO_RUINS_POOL_PARCHED 0x941

s32 MapGrid_GetCellWord(u32 layer, s32 x, s32 y) {
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

s32 MapGrid_SetCellWord(u32 layer, s32 x, s32 y, s32 value) {
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

void MapGrid_SetCellHeight(s32 x, s32 z, s32 layer, s32 height);

/// Updates the cells under a pillar object by its flag (the object's action):
/// set, the upper layer's cell is marked 0xff in its high byte, the lower
/// cell loses bit 7, the height drops 4 when the object's ground height is not
/// the map's and the sprite takes priority 3; clear, the upper cell's high byte
/// becomes 232 where the lower cell's is 232 and 0xff otherwise, the lower cell
/// gains bit 7, the height rises 4 and the sprite takes priority 2. Both cells
/// are written back and MapGrid_SetCellHeight is given the cell's position and
/// height.
void IzumoRuins_ShowPillarState(struct Actor *object) {
  s32 column = object->x >> 20;
  s32 row = object->z >> 20;
  s32 lower = MapGrid_GetCellWord(0, column, row);
  s32 upper = MapGrid_GetCellWord(2, column, row);
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
  MapGrid_SetCellWord(0, column, row, lower);
  MapGrid_SetCellWord(2, column, row, upper);
  MapGrid_SetCellHeight(column << 20, row << 20, object->terrainId, height);
}

void IzumoRuins_PlacePillars(u16 *list) {
  struct Actor *object;
  s32 flag;

  while (list[0] != 0xffff) {
    object = Actor_Get(list[0]);
    flag = (s16)list[2];
    Actor_SetSpriteFlags(object, 0);
    Actor_SelectAnimation(object, Event_GetFlag((u16)flag) + 1);
    object->motionFlags = 0;
    object->collisionFlags &= 0xfe;
    object->timer = flag;
    IzumoRuins_ShowPillarState(object);
    list += 3;
  }
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const u32 *Scene_GetRegions(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_IzumoRuinsFirstRoom)
    return gIzumoRuins_FirstRoomRegions;
  if (scene == (s32)&SceneId_IzumoRuinsSecondRoom)
    return gIzumoRuins_SecondRoomRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_IzumoRuinsFirstRoom)
    return gIzumoRuins_FirstRoomActorPlacements;
  if (scene == (s32)&SceneId_IzumoRuinsSecondRoom)
    return gIzumoRuins_SecondRoomActorPlacements;
  return gIzumoRuins_ActorPlacements;
}

/// Starts the effect for this scene's room, then, for a first argument of 1,
/// runs IzumoRuins_ShowPillarState on the effect work's object.
void IzumoRuins_PoundPillar(s32 aArg, s32 b) {
  struct IzumoRuinsEffectWork *work = Ram_HeapSlots->effectWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  s16 scene;

  scene = party->scene;
  if (scene == (s32)&SceneId_IzumoRuinsFirstRoom)
    Pillars_Pound(gIzumoRuins_FirstRoomPoundPillars, aArg, b);
  else
    Pillars_Pound(gIzumoRuins_SecondRoomPoundPillars, aArg, b);
  if (aArg == 1)
    IzumoRuins_ShowPillarState(Actor_Get(work->objectId));
}

void IzumoRuins_FreezePuddle(void) {
  struct IzumoRuinsEventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *object;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FlagBlock_ApplyFlags(gIzumoRuins_Puddles);
  object = work->objectCbc;
  object->priorityFlags |= 1;
  object->priorityFlags &= ~2;
  Event_End();
}

void IzumoRuins_SinkIntoSand(void) {
  struct Actor *object0;
  s32 result1;

  object0 = Actor_Get(gMainRoutineWork.currentOwner);
  result1 = Value3((s32 (*)())MapGrid_GetCellWord, 2, (object0->x >> 20),
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
void IzumoRuins_StartMovingPillar(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_IzumoRuinsFirstRoom) {
    Actor_LiftTargetBlock(gIzumoRuins_FirstRoomMovablePillars);
  } else {
    /* FAKEMATCH(signature): the cell read through an s16 return type, so the
     * old cell is masked in r3 and 0xff00 built in r2; read as s32 the two
     * registers swap; as the header's s32 return, 6 of 29 instructions differ
     * in tla-en. */
    MapGrid_SetCellWord(
        2, 50, 13,
        0xff00 |
            (((s16 (*)(u32, s32, s32))MapGrid_GetCellWord)(2, 50, 13) & 255));
    Actor_LiftTargetBlock(gIzumoRuins_SecondRoomMovablePillars);
  }
}

void IzumoRuins_FinishMovingPillar(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_PlaceMovedBlock();
  if (gMainRoutineWork.scene == (s32)&SceneId_IzumoRuinsSecondRoom)
    MapGrid_SetCellWord(2, 50, 13, MapGrid_GetCellWord(2, 50, 13) & 255);
  Event_End();
}

void IzumoRuins_ParchThePool(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(7, 2, 8, 7, 7, 12);
  MapGrid_CopyCellsTo(7, 66, 7, 76, 8, 9);
  Event_SetFlag(FLAG_IZUMO_RUINS_POOL_PARCHED);
  Event_End();
}

const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_IzumoRuinsFirstRoom)
    return gIzumoRuins_FirstRoomTriggers;
  return gIzumoRuins_SecondRoomTriggers;
}

s32 Scene_Initialize(void) {
  struct IzumoRuinsEventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *object;
  s32 height;

  work->field1ac = 0x204;
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 0x20;
  FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_IzumoRuinsFirstRoom) {
    Actor_PlaceBlocks(gIzumoRuins_FirstRoomMovablePillars);
    IzumoRuins_PlacePillars((u16 *)gIzumoRuins_FirstRoomPoundPillars);
    FlagBlock_PlaceAll(gIzumoRuins_Puddles);
  } else {
    Actor_Get(10)->priorityFlags |= 0x20;
    Actor_Get(11)->priorityFlags |= 0x20;
    Actor_PlaceBlocks(gIzumoRuins_SecondRoomMovablePillars);
    IzumoRuins_PlacePillars((u16 *)gIzumoRuins_SecondRoomPoundPillars);
    if (Event_GetFlag(FLAG_IZUMO_RUINS_POOL_PARCHED)) {
      MapGrid_CopyCellAttributes(7, 2, 8, 7, 7, 12);
      MapGrid_CopyCellsTo(7, 66, 7, 76, 8, 9);
      Map_CenterOnFollowedPoint();
      System_Wait(1);
      if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
        object = Actor_Get(gMainRoutineWork.currentOwner);
        height = Map_GetTerrainHeight(object->terrainId, object->x,
                                      object->z - object->y);
        object->terrainHeight = height;
        object->y = height;
        EventCamera_FollowActor(gMainRoutineWork.currentOwner, 0);
      }
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#include "objcmd.h"

// clang-format off
DEFINE_G_DATA_02001540(gIzumoRuins_RubbleSprayScript, 33)

DEFINE_G_DATA_02001540(gIzumoRuins_RubbleBurstScript, 44)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_ShortScript, 22)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_MediumScript, 44)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_LongScript, 126)

s16 gIzumoRuins_FirstRoomMovablePillars[] = {
    13, -1,
};

s16 gIzumoRuins_SecondRoomMovablePillars[] = {
    10, 11, 12, 13, 14, 15, -1, 0,
};

s32 gFieldEffect_DurationScripts[] = {
    (s32)gFieldEffect_ShortScript,
    (s32)gFieldEffect_MediumScript,
    (s32)gFieldEffect_LongScript,
};

s16 gIzumoRuins_FirstRoomPoundPillars[] = {
    8, 0, 512, 9, 0, 513, 12, 0,
    514, -1,
};

s16 gIzumoRuins_SecondRoomPoundPillars[] = {
    8, 0, 512, 9, 0, 513, -1,
};

s16 gIzumoRuins_Puddles[] = {
    10, 515, 11, 516, -1,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gIzumoRuins_FirstRoomRegions[] = {
    0x2c00d0, 0xe002f0, 0x300003c, 0x1ffff, 0xffff, 0,
    0, 0,
};

u32 gIzumoRuins_SecondRoomRegions[] = {
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

struct ActorPlacement gIzumoRuins_ActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gIzumoRuins_FirstRoomActorPlacements[] = {
    {406, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(824), FACING_NORTH, 2, 0},
    {406, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(824), FACING_NORTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(824), FACING_NORTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(824), FACING_NORTH, 2, 0},
    {406, CONDITION_ALWAYS, 1, PIXELS(696), 0, PIXELS(248), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(88), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gIzumoRuins_SecondRoomActorPlacements[] = {
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

struct Trigger gIzumoRuins_FirstRoomTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8, 0x200,
     EVENT(IzumoRuins_PoundPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 9, 0x201,
     EVENT(IzumoRuins_PoundPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 12, 0x202,
     EVENT(IzumoRuins_PoundPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 10, 0x203,
     EVENT(IzumoRuins_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, 0x204,
     EVENT(IzumoRuins_FreezePuddle)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(IzumoRuins_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(Leader_SetLayer)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 13,
     CONDITION_ALWAYS, EVENT(IzumoRuins_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, CONDITION_ALWAYS,
     EVENT(IzumoRuins_FinishMovingPillar)},
    {8, 0, CONDITION_ALWAYS, EVENT(IzumoRuins_StartMovingPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(IzumoRuins_FinishMovingPillar)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIzumoRuins_SecondRoomTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8, 0x200,
     EVENT(IzumoRuins_PoundPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 9, 0x201,
     EVENT(IzumoRuins_PoundPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(IzumoRuins_ParchThePool)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 10,
     CONDITION_ALWAYS, EVENT(IzumoRuins_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS,
     EVENT(IzumoRuins_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 11,
     CONDITION_ALWAYS, EVENT(IzumoRuins_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS,
     EVENT(IzumoRuins_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12,
     CONDITION_ALWAYS, EVENT(IzumoRuins_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS,
     EVENT(IzumoRuins_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 13,
     CONDITION_ALWAYS, EVENT(IzumoRuins_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, CONDITION_ALWAYS,
     EVENT(IzumoRuins_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 14,
     CONDITION_ALWAYS, EVENT(IzumoRuins_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, CONDITION_ALWAYS,
     EVENT(IzumoRuins_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15,
     CONDITION_ALWAYS, EVENT(IzumoRuins_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS,
     EVENT(IzumoRuins_FinishMovingPillar)},
    {TRIGGER_TABLE_END},
};
