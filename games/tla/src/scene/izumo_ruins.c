/// @file
/// The Izumo Ruins' pillars: those Pound sinks into the floor (Pillars_Place,
/// Pillars_Pound) and the grey ones that crack under a first step and crumble
/// under a second (CrumblingPillars_Step).
#include "shared/map_cells.h"
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

struct IzumoRuinsEventWork {
  u8 unk000[0x154];
  s16 exit;
  u8 unk156[0x56];
  s32 field1ac;
  u8 unk1b0[0xb0c];
  struct Actor *objectCbc;
};

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

void Pillars_Pound(u16 *list, s32 mode, s32 count);
void Actor_SetCellShapeAt(s32 a, s32 b, s32 owner);
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
#include "shared/actors.h"
#include "shared/tables.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "scene_tables.h"
#include "event_work.h"
#include "call.h"
#include "objcmd.h"
#include "text/msg_ids.h"

struct Actor;
s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
s32 Field_TryJumpForward(void);
struct Actor *Actor_FindAtCell(s32 *position, s32 flags);
void Leader_SetCellShapeAt(s32 a, s32 b);

void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

/// Places each pillar of a list of three-halfword entries (actor, kind,
/// flag) ended by 0xffff, its collision bit cleared, raised or sunk by its
/// flag (Actor_SetCellOpen).
void Pillars_Place(const u16 *list) {
  const u16 *entry;

  if (list[0] == 0xffff)
    return;
  entry = list;
loop:
  {
    u32 id = entry[0];
    struct Actor *object = Actor_Get(id);
    s16 kind = entry[1];
    s16 flag = entry[2];

    object->collisionFlags &= ~1;
    Actor_SetSpriteFlags(object, 0);
    Actor_SetCellOpen(id, (u16)kind, (u16)flag, object->terrainId);
    entry += 3;
  }
  if (entry[0] != 0xffff)
    goto loop;
}

// Finds the current target's table row and shrinks it.
void Pillars_Pound(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;

  if (*list == 0xffff)
    return;
top:
  {
    u32 entry = *list;
    s32 id = work->objectId;

    if (id == entry) {
      struct Actor *object = Actor_Get(id);
      s32 a;
      s32 flag;

      list++;
      a = list[0];
      flag = list[1];
      if (count < 8)
        object->scaleY -= 0x1f00;
      if (mode == 1) {
        Event_SetFlag(flag);
        Actor_SetCellOpen(id, a, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      return;
    }
  }
  list += 3;
  if (*list != 0xffff)
    goto top;
}

DEFINE_ACTOR_SET_CELL_OPEN(Actor_SetCellOpen, Actor_GetCellIndex,
                           Actor_SelectAnimation)

s32 MapCollision_FindOrAddTag(s32 layer, s32 shape, s32 height);

void CrumblingPillars_Place(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *leader;
  s32 height;

  while (list[0] != 0xffff) {
    u32 id = list[0];
    struct Actor *object = Actor_Get(id);
    s32 flag = list[1];
    s32 next = flag + 1;
    struct MapCell *cells = map->layers[object->terrainId].cells;
    u8 *shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);

    if (Event_GetFlag(next)) {
      Actor_SetPosition(id, 0, 0);
    } else {
      s32 index = Actor_GetCellIndex(object);
      struct MapCell *cell;
      u8 *slot;
      s32 shape, ground;

      cell = cells + index;
      slot = shapes + index;
      shape = MapGrid_GetTerrainKind(object->terrainId, object->x, object->z);
      ground = Map_GetTerrainHeight(object->terrainId, object->x, object->z);

      Actor_DisableById(id);
      cell->attributeB = 128;
      *slot = MapCollision_FindOrAddTag(object->terrainId, shape,
                                        (ground >> 19) + 4);
      if (Event_GetFlag(flag))
        Actor_SelectAnimation(object, 0);
    }
    list += 2;
  }
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  height = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
  if (leader->y < height) {
    leader->terrainHeight = height;
    leader->y = height;
  }
}

void CrumblingPillars_Step(u16 *list) {
  struct Actor *leader;
  struct Actor *pillar;
  s32 position[3];

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  pillar = Actor_FindAtCell(position, 0);
  if (pillar != 0)
    goto test;
  return;
body:
  {
    u32 id = list[0];

    if (Actor_GetById(id) != pillar) {
      list += 2;
      goto test;
    }
    {
      s32 flag = list[1];

      Event_Start();
      if (!GameFlag_Get(flag)) {
        Sound_Play(SE_CHEST_CLOSE);
        Actor_SetSpriteColor(Actor_GetById(id), 7);
        wait(2);
        Actor_SelectAnimation(pillar, 0);
        Actor_SetSpriteColor(Actor_GetById(id), 0);
        wait(2);
        Actor_SetSpriteColor(Actor_GetById(id), 7);
        wait(4);
        Actor_SetSpriteColor(Actor_GetById(id), 0);
        Rubble_Spray(0);
        GameFlag_Set(flag);
      } else if (!GameFlag_Get(flag + 1)) {
        u8 *flags = &leader->motionFlags;

        *flags = 0;
        Sound_Play(SE_PUSH);
        MapGrid_SetShake(1, 4, 1);
        Rubble_Spray(0);
        wait(8);
        Actor_SelectAnimation(pillar, 2);
        Leader_SetCellShapeAt(-2, pillar->terrainId);
        Rubble_Spray(1);
        wait(16);
        Leader_SetCellShapeAt(-2, pillar->terrainId);
        wait(4);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        wait(8);
        *flags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(pillar, 0, 0, 0);
        wait(2);
        Sound_Play(SE_THUMP);
        Rubble_Burst();
        wait(20);
        GameFlag_Set(flag + 1);
      }
      Event_End();
      return;
    }
  }
test:
  if (list[0] != 0xffff)
    goto body;
}

void CrumblingPillar_Collapse(const void *data, s32 id) {
  const struct TrapEntry *entry = data;
  struct Actor *object = Actor_Get(id);
  s32 flag;

  while (entry->actor != 0xffff) {
    if (entry->actor != id) {
      entry++;
      continue;
    }
    {
      flag = entry->flag;
      Event_Start();
      if (!Event_GetFlag(flag + 1)) {
        Event_PlaySound(SE_PUSH);
        MapGrid_SetShake(1, 4, 1);
        Rubble_Spray(0);
        System_Wait(8);
        Actor_SelectAnimation(object, 2);
        Actor_SetCellShapeAt(-4, OBJECT_BYTE(object, 34), id);
        Rubble_Spray(1);
        System_Wait(16);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        System_Wait(8);
        FieldActor_SetPositionAndResetMotion(object, 0, 0, 0);
        System_Wait(2);
        Event_PlaySound(SE_THUMP);
        Rubble_Burst();
        System_Wait(20);
        Event_SetFlag(flag);
        Event_SetFlag(flag + 1);
      }
      Event_End();
      break;
    }
  }
}

void CrumblingPillar_Prepare(s32 id) {
  Actor_Get(id)->unk5c[0] = 3;
  Actor_DisableById(id);
}

void Actor_MoveToObject(s32 id, s32 target);

void CrumblingPillars_PlaceWithStandIns(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u8 *grid;

  Actor_MoveToObject(8, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(9, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(10, gMainRoutineWork.currentOwner);
  System_Wait(1);
  CrumblingPillar_Prepare(8);
  CrumblingPillar_Prepare(9);
  CrumblingPillar_Prepare(10);
  Actor_SetAnimation(9, 0);
  System_Wait(1);
  Actor_SetPosition(8, 0, 0);
  Actor_SetPosition(9, 0, 0);
  Actor_SetPosition(10, 0, 0);
  System_Wait(1);
  grid = gMapShapeGrid;
  while (list[0] != 0xffff) {
    u32 id = list[0];
    struct Actor *actor = Actor_Get(id);
    s32 flag = list[1];
    s32 next = flag + 1;
    struct MapCell *cells = map->layers[actor->terrainId].cells;
    u8 *shapes = grid + (cells - (struct MapCell *)Ram_MapCellBuffer);

    actor->unk5c[0] = 3;
    if (Event_GetFlag(next)) {
      Actor_SetPosition(id, 0, 0);
    } else {
      s32 index = Actor_GetCellIndex(actor);
      struct MapCell *cell = cells + index;
      u8 *shape = shapes + index;
      s32 kind, ground;

      kind = MapGrid_GetTerrainKind(actor->terrainId, actor->x, actor->z);
      ground = Map_GetTerrainHeight(actor->terrainId, actor->x, actor->z);

      Actor_DisableById(id);
      cell->attributeB = 128;
      *shape =
          MapCollision_FindOrAddTag(actor->terrainId, kind, (ground >> 19) + 4);
      if (Event_GetFlag(flag))
        Actor_CopyGraphics(id, 9);
    }
    list += 2;
  }
  System_Wait(10);
}

void Actor_TakePlaceOf(struct Actor *object, struct Actor *target) {
  FieldActor_SetPositionAndResetMotion(object, target->x, target->y, target->z);
  FieldActor_SetPositionAndResetMotion(target, 0, 0, 0);
}

void CrumblingPillars_StepWithStandIn(u16 *list) {
  struct Actor *leader;
  struct Actor *helper;
  struct Actor *pillar;
  s32 position[3];

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  helper = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  pillar = Actor_FindAtCell(position, 0);
  if (pillar != 0)
    goto test;
  return;
body:
  {
    u32 id = list[0];

    if (Actor_GetById(id) != pillar) {
      list += 2;
      goto test;
    }
    {
      s32 flag = list[1];

      Event_Start();
      if (!GameFlag_Get(flag)) {
        Actor_SelectAnimation(helper, 1);
        Actor_TakePlaceOf(helper, pillar);
        wait(1);
        Sound_Play(SE_CHEST_CLOSE);
        Actor_SetSpriteColor(Actor_GetById(8), 7);
        wait(2);
        Actor_SelectAnimation(helper, 0);
        Actor_CopyGraphics(id, 9);
        Actor_SetSpriteColor(Actor_GetById(8), 0);
        wait(2);
        Actor_SetSpriteColor(Actor_GetById(8), 7);
        wait(4);
        Actor_SetSpriteColor(Actor_GetById(8), 0);
        Rubble_Spray(0);
        Actor_TakePlaceOf(pillar, helper);
        wait(1);
        GameFlag_Set(flag);
      } else if (!GameFlag_Get(flag + 1)) {
        u8 *flags;

        Actor_SelectAnimation(helper, 0);
        Actor_TakePlaceOf(helper, pillar);
        wait(1);
        flags = &leader->motionFlags;
        *flags = 0;
        Sound_Play(SE_PUSH);
        MapGrid_SetShake(1, 4, 1);
        Rubble_Spray(0);
        wait(8);
        Actor_SelectAnimation(helper, 2);
        Leader_SetCellShapeAt(-2, pillar->terrainId);
        Rubble_Spray(1);
        wait(16);
        Leader_SetCellShapeAt(-2, pillar->terrainId);
        wait(4);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        wait(8);
        *flags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(helper, 0, 0, 0);
        wait(2);
        Sound_Play(SE_THUMP);
        Rubble_Burst();
        wait(20);
        GameFlag_Set(flag + 1);
      }
      Event_End();
      return;
    }
  }
test:
  if (list[0] != 0xffff)
    goto body;
}

#include "../field/field_effect/advance_motion_priority.c"

// @file
// The Izumo Ruins' rubble, sprayed and burst from the leader as a pillar
// crumbles (Rubble_Spray, Rubble_Burst).

struct Actor;
struct FieldSprite;
s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
void Leader_SetCellShapeAt(s32 a, s32 b);
u32 Sprite_ShareVramCacheEntry(struct FieldSprite *sprite, u32 frame);

extern u32 gIzumoRuins_RubbleSprayScript[];

void Rubble_Spray(s32 spread) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;

  do {
    {
      /* FAKEMATCH(temporary): y held in a local. */
      s32 y = leader->y - (((Random16() * 14) >> 16) << 16);

      effect = (struct FieldEffect *)FieldActor_Create(
          285, (i << 17) + leader->x - 0x80000, y - 0x20000, leader->z);
    }
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags((struct Actor *)effect, 0);
      Actor_SelectAnimation((struct Actor *)effect, 2);
      Actor_StartScript(effect, gIzumoRuins_RubbleSprayScript);
      effect->scaleY = effect->scaleX = 0xb333;
      effect->sprite->priority = 1;
      if (spread != 0) {
        effect->velocity.x = Iwram_MulQ16(Trig_Cos(i << 13), 0x13333);
        effect->velocity.z = Math_Sin(i << 13);
      } else {
        effect->velocity.x = spread;
        effect->velocity.z = spread;
      }
      effect->velocity.y = -(((Random16() << 14) >> 16) + 0x6000);
      effect->spin = ((Random16() << 9) >> 16) - 0x100;
      effect->scaleRateX = -0x800;
      effect->scaleRateY = -0x600;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
    }
    i++;
  } while (i <= 7);
}

extern u32 gIzumoRuins_RubbleBurstScript[];

void Rubble_Burst(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame;
  u32 i;

  if (leader == 0)
    return;
  frame = 0;
  i = 0;
  do {
    effect = (struct FieldEffect *)FieldActor_Create(285, leader->x, leader->y,
                                                     leader->z);
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags((struct Actor *)effect, 0);
      Actor_SelectAnimation((struct Actor *)effect, 2);
      Actor_StartScript(effect, gIzumoRuins_RubbleBurstScript);
      effect->velocity.x = Trig_Cos(i << 12);
      effect->velocity.y = 0;
      effect->velocity.z = Math_Sin(i << 12);
      effect->velocity.x += effect->velocity.x >> 1;
      effect->velocity.x += ((Random16() * 3) << 14 >> 16) - 0x6000;
      effect->velocity.z += ((Random16() * 3) << 13 >> 16) - 0x3000;
      effect->spin = ((Random16() << 12) >> 16) - 0x800;
      effect->scaleRateX = 0;
      effect->scaleRateY = 0;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
      effect->sprite->priority =
          ((struct FieldSprite *)leader->sprite)->priority;
    }
    i++;
  } while (i <= 16);
}

s32 MapCollision_FindOrAddTag(s32 terrainId, s32 value, s32 height);

DEFINE_ACTOR_SET_CELL_SHAPE_AT(Actor_SetCellShapeAt, MapGrid_GetTerrainKind,
                               MapCollision_FindOrAddTag)

void Leader_SetCellShapeAt(s32 a, s32 b) {
  Actor_SetCellShapeAt(a, b, gMainRoutineWork.currentOwner);
}

#include "../field/object/set_sprite_priority.c"

#include "../field/field_effect/move.c"

// @file
// The Izumo Ruins, under the stone circle of Izumo, in two rooms that share
// this overlay (SceneId_IzumoRuinsFirstRoom, SceneId_IzumoRuinsSecondRoom).
// Sand carries the party across the sandy floor (IzumoRuins_SinkIntoSand),
// Frost freezes the puddles (IzumoRuins_FreezePuddle), Pound sinks pillars
// (IzumoRuins_PoundPillar), Move sets the brown pillars in place
// (IzumoRuins_FinishMovingPillar), and Parch dries the pool before the room
// of the stone tablet (IzumoRuins_ParchThePool).
//
// The pillars, the rubble and the effect spawner are in izumo_ruins/, which
// shared field effect objects link between.
//
// It opens on its copy of the effect spawner, FieldEffect_Spawn.

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
  if (!((result1 >> 8) != 232)) {
    object0->terrainId = 2;
  }
  return;
}

DEFINE_LEADER_SET_LAYER(Leader_SetLayer, 0)

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

// clang-format off
u32 gIzumoRuins_RubbleSprayScript[] = {                                                               
      SC_WAIT(33), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),       
      SC_FREE,                                                                 
  };

u32 gIzumoRuins_RubbleBurstScript[] = {                                                               
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
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(IzumoRuins_StartMovingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(IzumoRuins_FinishMovingPillar)},
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
