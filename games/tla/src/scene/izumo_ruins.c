/// @file
/// The Izumo Ruins, under the stone circle of Izumo, in two rooms that share
/// this overlay (SceneId_IzumoRuinsFirstRoom, SceneId_IzumoRuinsSecondRoom).
/// Sand carries the party across the sandy floor (IzumoRuins_SinkIntoSand),
/// Frost freezes the puddles (IzumoRuins_FreezePuddle), Pound sinks pillars
/// (IzumoRuins_PoundPillar), Move sets the brown pillars in place
/// (IzumoRuins_FinishMovingPillar), grey pillars crumble under a second step
/// (shared/crumbling_pillars.h), and Parch dries the pool before the room of
/// the stone tablet (IzumoRuins_ParchThePool).
#include "system/iwram_call.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "types.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "module/field/object.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "types.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "system/input.h"
#include "text/msg_ids.h"
#include "field/event_inline.h"
#include "system/frame.h"
#include "scene_tables.h"

struct IzumoRuinsAnimation {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
  u8 unk0a[8];
  u16 angle;
};

struct IzumoRuinsMapLayer {
  u8 unk00[0x30];
  u8 *cells;
  u8 unk34[4];
};

struct IzumoRuinsMapWork {
  u8 unk000[0x108];
  struct IzumoRuinsMapLayer layers[3];
};

struct IzumoRuinsPlacement {
  u16 id;
  u16 unk02;
  s16 flag;
};

struct IzumoRuinsEffectWork {
  u8 unk00[26];
  s16 objectId;
};

extern u8 gMapCellBuffer[];
extern char SceneId_IzumoRuinsFirstRoom;
extern char SceneId_IzumoRuinsSecondRoom;

void Pound_SinkTarget(u16 *list, s32 mode, s32 count);
s32 MapGrid_GetCellWord(u32 layer, s32 x, s32 y);
s32 MapGrid_SetCellWord(u32 layer, s32 x, s32 y, s32 value);
void IzumoRuins_ShowPillarState(struct Actor *object);
void Actor_SelectAnimation(struct Actor *object, s32 value);
void Event_End(void);
void Actor_DisableById(s32 id);
void FlagBlock_ApplyFlags(const void *data);
void Actor_LiftTargetBlock(const void *data);
void Actor_PlaceMovedBlock(void);

extern s16 gIzumoRuins_FirstRoomMovablePillars[];
extern s16 gIzumoRuins_SecondRoomMovablePillars[];
extern s16 gIzumoRuins_FirstRoomPoundPillars[];
extern s16 gIzumoRuins_SecondRoomPoundPillars[];
extern s16 gIzumoRuins_Puddles[];
extern u32 gIzumoRuins_FirstRoomRegions[];
extern u32 gIzumoRuins_SecondRoomRegions[];
extern struct ActorPlacement gIzumoRuins_ActorPlacements[];
extern struct ActorPlacement gIzumoRuins_FirstRoomActorPlacements[];
extern struct ActorPlacement gIzumoRuins_SecondRoomActorPlacements[];
extern struct Trigger gIzumoRuins_FirstRoomTriggers[];
extern struct Trigger gIzumoRuins_SecondRoomTriggers[];

void IzumoRuins_PlacePillars(u16 *list);
void Rubble_Spray(s32 mode);
void Rubble_Burst(void);
void Map_CenterOnFollowedPoint(void);
s32 Actor_GetCellIndex(struct Actor *object);
void Actor_PlaceBlocks(const void *data);
void FieldEffect_StartBlankOverlay(s32 value);
void FlagBlock_PlaceAll(const void *data);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
#include "map.h"
#include "shared/trial_road.h"
#include "system/system.h"
#include "field_sprite.h"
#include "system/field_effect.h"
#include "module/field/field_effect.h"
#include "flags.h"
#include "shared/tables.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "scene_tables.h"
#include "field/field_sequence_event.h"
#include "call.h"
#include "objcmd.h"
#include "text/msg_ids.h"

struct Actor;
s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
s32 Field_TryJumpForward(void);
struct Actor *Actor_FindAtCell(s32 *position, s32 flags);
void Leader_SetCellShapeAt(s32 level, s32 layer);

void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

#include "shared/crumbling_pillars.h"

#include "../field/field_effect/advance_motion_priority.c"

void *Sprite_ShareVramCacheEntry(void *sprite, void *last);

#include "shared/rubble.h"

s32 MapCollision_FindOrAddTag(s32 terrainId, s32 value, s32 height);

#include "shared/actor_set_cell_shape.h"

#include "shared/leader_set_cell_shape.h"

#include "../field/object/set_sprite_priority.c"

#include "../field/field_effect/move.c"

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect/spawn.c"

#include "text/msg_ids.h"

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

// Updates the cells under a pillar object by its flag (the object's action):
// set, the upper layer's cell is marked 0xff in its high byte, the lower
// cell loses bit 7, the height drops 4 when the object's ground height is not
// the map's and the sprite takes priority 3; clear, the upper cell's high byte
// becomes 232 where the lower cell's is 232 and 0xff otherwise, the lower cell
// gains bit 7, the height rises 4 and the sprite takes priority 2. Both cells
// are written back and MapGrid_SetCellHeight is given the cell's position and
// height.
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

// Starts the effect for this scene's room, then, for a first argument of 1,
// runs IzumoRuins_ShowPillarState on the effect work's object.
void IzumoRuins_PoundPillar(s32 aArg, s32 b) {
  struct IzumoRuinsEffectWork *work = Ram_HeapSlots->effectWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  s16 scene;

  scene = party->scene;
  if (scene == (s32)&SceneId_IzumoRuinsFirstRoom)
    Pound_SinkTarget(gIzumoRuins_FirstRoomPoundPillars, aArg, b);
  else
    Pound_SinkTarget(gIzumoRuins_SecondRoomPoundPillars, aArg, b);
  if (aArg == 1)
    IzumoRuins_ShowPillarState(Actor_Get(work->objectId));
}

void IzumoRuins_FreezePuddle(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *object;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FlagBlock_ApplyFlags(gIzumoRuins_Puddles);
  object = work->movedBlock;
  object->priorityFlags |= 1;
  object->priorityFlags &= ~2;
  Event_End();
}

void IzumoRuins_SinkIntoSand(void) {
  struct Actor *object0;
  s32 result1;

  object0 = Actor_Get(gMainRoutineWork.currentOwner);
  result1 = MapGrid_GetCellWord(2, (object0->x >> 20), (object0->z >> 20));
  if (!((result1 >> 8) != 232)) {
    object0->terrainId = 2;
  }
  return;
}

#include "shared/leader_set_layer.h"

// Outside the first room, sets the high byte of map cell (50, 13) on layer 2
// to 0xff; then starts the room's effect.
void IzumoRuins_StartMovingPillar(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_IzumoRuinsFirstRoom) {
    Actor_LiftTargetBlock(gIzumoRuins_FirstRoomMovablePillars);
  } else {
    /* FAKEMATCH(signature): the cell read through an s16 return type. */
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
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *object;
  s32 height;

  work->transition = 0x204;
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

// clang-format off
u32 gRubble_SprayScript[] = {
    SC_WAIT(33), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),
    SC_FREE,
};

u32 gRubble_BurstScript[] = {
    SC_WAIT(44), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),
    SC_FREE,
};

u32 gFieldEffect_ShortScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(22, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_MediumScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(44, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_LongScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(126, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

s16 gIzumoRuins_FirstRoomMovablePillars[] = {
    13, -1,
};

s16 gIzumoRuins_SecondRoomMovablePillars[] = {
    10, 11, 12, 13, 14, 15, -1, 0,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
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
    SCENE_EXITS(IzumoRuinsFirstRoom),
    SCENE_EXIT_UNLESS(1, Izumo, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, IzumoRuinsFirstRoom, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, IzumoRuinsFirstRoom, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, IzumoRuinsSecondRoom, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(IzumoRuinsSecondRoom),
    SCENE_EXIT_UNLESS(1, IzumoRuinsFirstRoom, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, IzumoRuinsSecondRoom, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, IzumoRuinsSecondRoom, 2, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gIzumoRuins_ActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gIzumoRuins_FirstRoomActorPlacements[] = {
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 824, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 824, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 824, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 824, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 88, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gIzumoRuins_SecondRoomActorPlacements[] = {
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 552, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 552, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 152, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 152, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 184, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 216, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 216, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 856, 0, 184, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gIzumoRuins_FirstRoomTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8,
     FLAG_SCENE_TEMP_00, EVENT(IzumoRuins_PoundPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 9,
     FLAG_SCENE_TEMP_01, EVENT(IzumoRuins_PoundPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 12,
     FLAG_SCENE_TEMP_02, EVENT(IzumoRuins_PoundPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 10, FLAG_SCENE_TEMP_03,
     EVENT(IzumoRuins_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, FLAG_SCENE_TEMP_04,
     EVENT(IzumoRuins_FreezePuddle)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(IzumoRuins_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(Leader_SetLayer)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 13,
     CONDITION_ALWAYS, EVENT(IzumoRuins_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, CONDITION_ALWAYS,
     EVENT(IzumoRuins_FinishMovingPillar)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(IzumoRuins_StartMovingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(IzumoRuins_FinishMovingPillar)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIzumoRuins_SecondRoomTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8,
     FLAG_SCENE_TEMP_00, EVENT(IzumoRuins_PoundPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 9,
     FLAG_SCENE_TEMP_01, EVENT(IzumoRuins_PoundPillar)},
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
