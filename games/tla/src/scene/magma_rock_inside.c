/// @file
/// Magma Rock's inside, the volcano's heart: the Moai that flood its rooms with
/// lava when Burst wakes them, the platforms that ride the lava and the gates
/// that drain it, the pillars pushed into it, Fury the Djinni, and deep inside
/// the tablet where Jenna learns Blaze and the flames Blaze moves.

#include "../field/object/set_sprite_priority.c"

#include "../field/object/prepare_object.c"

#include "types.h"
#include "item_ids.h"
#include "flags.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "scene_tables.h"
#include "system/iwram_call.h"
#include "scene_ids.h"
#include "system/frame.h"
#include "field/event_inline.h"
#include "map.h"

extern struct MapCell gMapCellBuffer[];

extern char SceneId_MagmaRockInside1;
extern char SceneId_MagmaRockInside2;
extern char SceneId_MagmaRockInside3;
extern char SceneId_MagmaRockInside4;
extern char SceneId_MagmaRockInside5;
extern char SceneId_MagmaRockInside6;
extern char SceneId_MagmaRockInside7;
extern char SceneId_MagmaRockInside8;

void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Actor_SetBlendMode(struct Actor *object, u32 value);

extern u32 gMagmaRockInside_FlameScript[];

extern s32 gMagmaRockInside_LastStepCell;

void MagmaRockInside_BurstStatue(s32 a, s32 b);
void MagmaRockInside_TrackStepCell(void);
void MagmaRockInside_SetActorCellShape(s32 a, s32 b, s32 owner);
void Actor_SelectAnimation(struct Actor *object, s32 a);
void Actor_StartScript(struct Actor *object, const void *data);
void Actor_DisableById(s32 id);

static __inline__ struct Actor *Create(s32 kind, s32 x, s32 y, s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}
#include "shared/actors.h"
#include "shared/push.h"
#include "shared/map_cells.h"
#include "shared/effects.h"
#include "types.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "event_work.h"
#include "field/field.h"
#include "field/field_coordinate.h"
#include "module/field/field.h"
#include "module/field/object.h"
#include "module/scene/mars_lighthouse_aerie.h"
#include "module/scene/scene.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "system/scene.h"
#include "system/ram_buffer.h"
#include "map.h"
#include "actor.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"
#include "system/io_reg.h"
#include "system/frame.h"
#include "system/field_effect.h"
#include "macros.h"
#include "field_sprite.h"
#include "call.h"
#include "edition.h"
#include "shared/trial_road.h"
#include "shared/tables.h"
#include "system/iwram_call.h"
#include "system/heap_release.h"
#include "item_icon.h"
#include "module/field/field_effect.h"
#include "field/particle.h"
#include "graphics/resource.h"
#include "shared/tundaria_tower.h"
#include "system/task.h"
#include "objcmd.h"

struct Actor *MagmaRockInside_CreateEffectActor(s32 x, s32 y, s32 z, s32 kind) {
  struct Actor *object = FieldActor_Create(kind, x, y, z);

  if (object != NULL) {
    Scene_SetActorSpritePriority(object, 1);
    object->motionFlags = 0;
    object->collisionFlags = 8;
    Actor_SetSpriteFlags(object, 0);
    FieldActor_SetSpriteColor(object, 15);
    object->priorityFlags |= 0x22;
    return object;
  }
  return NULL;
}

#include "../field/field_effect/move.c"

// @file
#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect/spawn.c"

DEFINE_POSITION_DISTANCE(Position_Distance)

#include "../field/object/find_in_cell.c"

// Magma Rock's inside: the lava floods and drains, its platforms, statues and
// pillars, the Blaze tablet and the flames and torches it opens the way with.

struct Actor;
struct FieldSprite;
struct Particle;
struct ParticleSprite;
struct RockSprite__s4b18;

struct MagmaRockInsideSparkWork {
  u8 unk0000[0x1604];
  s16 resource;
};

void MagmaRockInside_PausePlatform();

extern s16 gMagmaRockInside_Room2Pillars[];

extern s16 gMagmaRockInside_Room3Pillars[];

extern s16 gMagmaRockInside_Room4Pillars[];

extern s16 gMagmaRockInside_Room7Pillars[];

void MagmaRockInside_MarkBlockCells(u16 *ids);

void MagmaRockInside_WaitForLanding(struct Actor *object);

s32 MagmaRockInside_GetCell(u32 layer, s32 x, s32 y);

s32 MagmaRockInside_FindStatue(u16 *table);

s32 MagmaRockInside_FlatDistance(s32 *from, s32 *to);

void MagmaRockInside_TrailEmbers(struct Actor *object);
void MagmaRockInside_DrainLava(void);

void MagmaRockInside_DropRocks(s32 x, s32 y, s32 z, s32 count);

void MagmaRockInside_StartFallingRocks(void);

void MagmaRockInside_PressGateSwitch1(void);
void MagmaRockInside_PressGateSwitch2(void);
void MagmaRockInside_PressGateSwitch3(void);

extern s16 gMagmaRockInside_PlatformPath6[];
extern s16 gMagmaRockInside_PlatformPath7[];
extern s16 gMagmaRockInside_PlatformPath8[];
extern s16 gMagmaRockInside_PlatformPath9[];

void FieldEffect_StartBlankOverlay(s32 value);

void MagmaRockInside_RaiseActorCells(u16 *list);

void HangingVine_Place(s32 id, s32 value);
s32 Actor_GetCellIndex(struct Actor *object);
s32 Field_TryJumpForward(void);
struct Actor *Actor_FindAtCell(s32 *position, s32 mode);

void MagmaRockInside_SpawnRubble(s32 spread);
void MagmaRockInside_SetLeaderCellShape(s32 a, s32 b);
void MagmaRockInside_SpawnDustRing(void);
extern u32 gMagmaRockInside_RubbleScript[];

extern u32 gMagmaRockInside_DustScript[];

void Func_02004c58(void *entry, void *destination);
void Func_02004f10(struct RockSprite__s4b18 *sprite, s32 width, s32 height,
                   u32 attributes, s32 tile);

void MagmaRockInside_UpdateFallingRocks(void);

void MagmaRockInside_FightFury(void);

void MagmaRockInside_UnblockMovedPillar(void);
void MagmaRockInside_BlockMovedPillar(void);
void MagmaRockInside_PushPillarIntoLava(void);

void MagmaRockInside_FloodWithLava(void);

void MagmaRockInside_ReleaseRocks(void);
void MagmaRockInside_TakeFlame(void);

void MagmaRockInside_GrowFlame(void);

void MagmaRockInside_PutOutFlames(void);

void MagmaRockInside_ExamineEmberStone(void);

void MagmaRockInside_StampStepCell(void);

void MagmaRockInside_StampOtherStepCell(void);

void MagmaRockInside_StepOnMarker(void);

void MagmaRockInside_LightTabletTorch(void);
void MagmaRockInside_GrowTabletFlame(void);
void MagmaRockInside_PutOutTabletFlame(void);
void MagmaRockInside_ReadBlazeTablet(void);
void MagmaRockInside_ReadBlazeTabletFromWest(void);
void MagmaRockInside_ReadBlazeTabletFromEast(void);
void MagmaRockInside_ReadBlazeTabletFromSouth(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];

void MagmaRockInside_BurstStatue(s32 mode, s32 cue);

#include "text/msg_ids.h"

void Actor_LiftTargetBlock(s16 *cells);
void Actor_PlaceMovedBlock(void);
void Import_FieldPsynergySetupActorPair(s32 owner, u16 value);
void Event_PlayCueNearFocus(struct Actor *object, s32 sound);
extern s32 gMagmaRockInside_StepCellWidth;
extern s32 gMagmaRockInside_StepCellHeight;
extern s32 gMagmaRockInside_StepCellX;
extern s32 gMagmaRockInside_StepCellZ;

extern s32 gPushLeaderSteps[];

DEFINE_BLOCK_PUSH_ONTO(MagmaRockInside_PushBlockOnto, gPushLeaderSteps,
                       MapGrid_CheckCollision, Actor_SelectAnimation)

extern struct MapCell gMapCellBuffer[];

// Sets the collision code of each cell in a layer rectangle to value.
// Layers 3 and above use the shared buffer.
s32 MagmaRockInside_SetCollisionRect(u32 layer, u32 x, u32 y, u32 width,
                                     u32 height, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapCell *cells;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cells = map->layers[layer].cells;
  else
    cells = gMapCellBuffer;
  cells += x + (y << 7);
  for (x = 0; x < height; x++) {
    struct MapCell *cell = cells + (x << 7);

    for (y = 0; y < width; y++, cell++)
      cell->collisionCode = value;
  }
  return 0;
}

void MagmaRockInside_FightFury(void) { Field_RequestDjinnBattle(15, 56); }

void MagmaRockInside_MeetDjinni(void) { Djinn_JoinParty(17, 2, 19); }

DEFINE_POSITION_FLAT_DISTANCE(MagmaRockInside_FlatDistance)

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
s32 MapCollision_FindOrAddTag(s32 layer, s32 shape, s32 height);

DEFINE_MAP_CELL_SET_SHAPE(MagmaRockInside_SetCellShape, MapGrid_GetTerrainKind,
                          MapCollision_FindOrAddTag)

void MagmaRockInside_WaitForLanding(struct Actor *object) {
  s32 count;
  s32 busy;

  count = 60;
  while (count != 0) {
    System_Wait(1);
    busy = object->velocityY;
    count--;
    if (busy == 0) {
      break;
    }
  }
}

DEFINE_MAP_CELL_GET(MagmaRockInside_GetCell)

DEFINE_MAP_CELL_SWAP(MagmaRockInside_SwapCells)

// For each id of a list ended by 0xffff: clears the object's mark and, on a
// cell with attribute bit 6, blocks the cells ahead of and behind it and sets
// the mark.
DEFINE_BLOCKS_MARK_CELLS(MagmaRockInside_MarkBlockCells,
                         MagmaRockInside_GetCell)

// Clears the object's mark and the cells ahead of and behind it that the mark
// blocked.
DEFINE_BLOCK_CLEAR_CELLS(MagmaRockInside_ClearBlockCells)

void MagmaRockInside_PausePlatform(struct Actor *object) {
  u8 *work = (u8 *)Ram_HeapSlots->eventWork;
  u8 *state = &object->movementState;

  if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 ||
      *(s16 *)(work + 350) != 0 || *(s16 *)(work + 360) != 0 ||
      *(u16 *)(work + 434) == 0)
    *state = 1;
  else
    *state = 0;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Actor_SelectAnimation();
void Actor_StartScript();
s32 GameFlag_Get();
/* VIEW(s0800): routine Actor_GetById as s32 Actor_GetById(); event.h has struct
 * Actor *Actor_GetById(u32 objectId); tried as the definition's struct Actor
 * *Actor_GetById(u32 objectId), which the object already declares, it does not
 * compile in tla-ja: games/tla/src/scene/magma_rock_inside.c:347: too few
 * arguments to function `Actor_GetById'. */
extern s32 Actor_GetById__s0800() __asm__("Actor_GetById");

void MagmaRockInside_StartLavaPlatform(s32 a0, s32 a1, s32 a2) {
  s32 t1, t4, v5, v6, v7;

  v5 = (a2 << 16);
  v7 = a1;
  t1 = Actor_GetById__s0800();
  v6 = t1;
  Actor_SelectAnimation(t1, 4);
  FIELD_AT(v6, u8, 90) = 0;
  FIELD_AT(v6, s32, 48) = 0x8000;
  FIELD_AT(v6, s32, 52) = 0x8000;
  FIELD_AT(v6, u8, 85) = 2;
  /* FAKEMATCH(barrier): an empty asm ends the scheduling block here. */
  asm("");
  v5 = (v5 >> 16);
  v5 = (v5 << 16);
  FIELD_AT(v6, u8, 35) = (128 | FIELD_AT(v6, u8, 35));
  v5 = ((u32)v5 >> 16);
  Actor_SetSpriteFlags(v6, 0);
  t4 = Event_GetFlag(v5);
  /* FAKEMATCH(barrier): an empty asm ends the scheduling block here. */
  asm("");
  if (t4 == 0)
    goto sunk;
  FIELD_AT(v6, s32, 20) = 0x100000;
  FIELD_AT(v6, s32, 12) = 0x100000;
  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    goto hook;
  Actor_StartScript(v6, v7);
  goto hook;
sunk:
  FIELD_AT(v6, s32, 20) = -0xe0000;
  FIELD_AT(v6, s32, 12) = -0xe0000;
hook:
  FIELD_AT(v6, s32, 108) = (s32)MagmaRockInside_PausePlatform;
}
#undef FIELD_AT

s32 MagmaRockInside_SetCellShape(s32 terrain, s32 x, s32 z, s32 height);

DEFINE_ACTORS_RAISE_CELLS(MagmaRockInside_RaiseActorCells,
                          MagmaRockInside_SetCellShape)

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gMagmaRockInside_Placements[];
extern struct ActorPlacement gMagmaRockInside_Room2Placements[];
extern struct ActorPlacement gMagmaRockInside_Room3Placements[];
extern struct ActorPlacement gMagmaRockInside_Room4Placements[];
extern struct ActorPlacement gMagmaRockInside_Room5Placements[];
extern struct ActorPlacement gMagmaRockInside_Room6Placements[];
extern struct ActorPlacement gMagmaRockInside_Room7Placements[];
extern struct ActorPlacement gMagmaRockInside_Room8Placements[];

extern s16 gMagmaRockInside_Room6Pillars[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene != (s32)&SceneId_MagmaRockInside1) {
    if (scene == (s32)&SceneId_MagmaRockInside2)
      return gMagmaRockInside_Room2Placements;
    if (scene == (s32)&SceneId_MagmaRockInside3)
      return gMagmaRockInside_Room3Placements;
    if (scene == (s32)&SceneId_MagmaRockInside4)
      return gMagmaRockInside_Room4Placements;
    if (scene == (s32)&SceneId_MagmaRockInside5)
      return gMagmaRockInside_Room5Placements;
    if (scene == (s32)&SceneId_MagmaRockInside6)
      return gMagmaRockInside_Room6Placements;
    if (scene == (s32)&SceneId_MagmaRockInside7)
      return gMagmaRockInside_Room7Placements;
    if (scene == (s32)&SceneId_MagmaRockInside8)
      return gMagmaRockInside_Room8Placements;
  }
  return gMagmaRockInside_Placements;
}

void MagmaRockInside_UnblockMovedPillar(void) {
  u8 *work;
  s16 scene;

  work = (u8 *)Ram_HeapSlots->eventWork;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_MagmaRockInside2)
    Actor_LiftTargetBlock(gMagmaRockInside_Room2Pillars);
  else if (scene == (s32)&SceneId_MagmaRockInside3)
    Actor_LiftTargetBlock(gMagmaRockInside_Room3Pillars);
  else if (scene == (s32)&SceneId_MagmaRockInside4)
    Actor_LiftTargetBlock(gMagmaRockInside_Room4Pillars);
  else if (scene == (s32)&SceneId_MagmaRockInside6)
    Actor_LiftTargetBlock(gMagmaRockInside_Room6Pillars);
  else if (scene == (s32)&SceneId_MagmaRockInside7)
    Actor_LiftTargetBlock(gMagmaRockInside_Room7Pillars);
  MagmaRockInside_ClearBlockCells(*(s32 *)(work + 0xcbc));
}

void MagmaRockInside_BlockMovedPillar(void) {
  struct Actor *object =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 0xcbc);
  /* FAKEMATCH(pin): copy in r5. */
  register struct Actor *copy asm("r5");
  s32 scene;
  s32 x;
  s32 z;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_PlaceMovedBlock();
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_MagmaRockInside6) {
    MagmaRockInside_MarkBlockCells(gMagmaRockInside_Room6Pillars);
    if (Map_GetTerrainHeight(2, object->x, object->z) != object->y) {
      object->terrainId = 2;
      object->motionFlags = 3;
      System_Wait(1);
      if (Event_GetFlag(FLAG_AREA_TEMP_02)) {
        object->gravity = 0x1000;
        object->priorityFlags |= 2;
        REG_BLDALPHA = 0x1000;
        REG_BLDCNT = 0x3f10;
        copy = OverlayObject_PrepareObject(object->x, object->terrainHeight,
                                           object->z + 0x100000, 312);
        MagmaRockInside_WaitForLanding(object);
        Event_PlaySound(SE_HEAVY_STOP);
        Actor_Release(copy);
        Actor_SelectAnimation(object, 4);
        System_Wait(1);
        REG_BLDCNT = 0;
      } else {
        MagmaRockInside_WaitForLanding(object);
        Event_PlaySound(SE_THUMP);
      }
      object->motionFlags = 0;
      object->terrainId = 0;
    }
  } else if (scene == (s32)&SceneId_MagmaRockInside7) {
    MagmaRockInside_MarkBlockCells(gMagmaRockInside_Room7Pillars);
    if (Map_GetTerrainHeight(2, object->x, object->z) != object->y) {
      object->terrainId = 2;
      object->motionFlags = 3;
      System_Wait(1);
      object->gravity = 0x1000;
      object->priorityFlags |= 2;
      REG_BLDALPHA = 0x1000;
      REG_BLDCNT = 0x3f10;
      copy = OverlayObject_PrepareObject(object->x, object->terrainHeight,
                                         object->z + 0x100000, 312);
      MagmaRockInside_WaitForLanding(object);
      Event_PlaySound(SE_HEAVY_STOP);
      Actor_Release(copy);
      Actor_SelectAnimation(object, 4);
      System_Wait(1);
      REG_BLDCNT = 0;
      MagmaRockInside_WaitForLanding(object);
      object->motionFlags = 0;
      object->terrainId = 0;
    }
  }
  x = object->x >> 20;
  z = object->z >> 20;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_MagmaRockInside2) {
    if (z == 16)
      Event_SetFlag(2544);
  } else if (scene == (s32)&SceneId_MagmaRockInside3) {
    if (z == 41)
      Event_SetFlag(2545);
  } else if (scene == (s32)&SceneId_MagmaRockInside4) {
    if (x == 21) {
      if (z == 43)
        Event_SetFlag(2546);
      if (z == 45)
        Event_SetFlag(2547);
      if (z == 47)
        Event_SetFlag(2548);
    }
  } else if (scene == (s32)&SceneId_MagmaRockInside6) {
    if (x == 47) {
      Event_SetFlag(2549);
      {
        s32 column = 46;
        s32 row = 73;

        MapGrid_CopyCellAttributes(column, 74, 1, 1, column, row);
      }
    }
    if (x == 57)
      Event_SetFlag(2550);
  } else if (scene == (s32)&SceneId_MagmaRockInside7) {
    if (x == 43)
      Event_SetFlag(2551);
  }
  Event_End();
}

s32 Import_BattleEffectStopMainActor(void);

void MagmaRockInside_PlayBurst(s16 value) {
  struct MainRoutineWork *party;

  BattleEffect_LoadActionEffectResources(151, 1);
  party = &gMainRoutineWork;
  Import_FieldPsynergySetupActorPair(party->currentOwner, value);
  FieldPsynergy_RunHandler(1);
  Import_BattleEffectStopMainActor();
  BattleEffect_CleanupSceneObjects();
}

extern u32 gMagmaRockInside_EmberScript[];

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

// An object's update: every fourth frame it spawns an effect of type 403 with
// its own script at a random point within 4 pixels of the object along z,
// moving along z at a random speed; every sixteenth frame it also plays sound
// 136 at the object.
void MagmaRockInside_TrailEmbers(struct Actor *object) {
  struct EffectOptions options;
  u32 frame = gFrameCount;
  s32 dz;
  s32 vz;
  s32 z;

  if ((frame & 3) != 0)
    return;
  options.type = 403;
  options.priority = 2;
  options.script = gMagmaRockInside_EmberScript;
  if ((frame & 15) == 0)
    Event_PlayCueNearFocus(object, 136);
  dz = Random16() & 15;
  vz = ((Random16() & 31) - 16) << 12;
  /* FAKEMATCH(temporary): z read into its own local. */
  z = object->z;
  FieldEffect_Spawn(object->x, object->y, z + ((dz - 8) << 15), 0, 0, vz,
                    0x320001, &options);
}

DEFINE_ACTOR_FLASH_COLOR(MagmaRockInside_FlashStatue, 10)

// The cell watch's table lookup over (id, cell, value) rows ended by 0xffff:
// for the row of the cell under the watch, sets the watch's object to the row's
// object and returns its value; otherwise returns the value of the last row
// passed (0 for none).

s32 MagmaRockInside_FindStatue(u16 *table) {
  struct CellWatch *watch = Ram_HeapSlots->effectWork;
  s32 result = 0;
  s32 cell = MagmaRockInside_GetCell(0, watch->x >> 20, watch->z >> 20) >> 8;
  u32 id;

  goto test;
again:
  result = *table++;
test:
  if (*table != 0xffff) {
    id = *table++;
    if (cell != *table++)
      goto again;
    watch->object = Actor_Get(id);
    result = *table;
  }
  return result;
}

void MagmaRockInside_ResolveBeforeBurst(void) {
  Event_ResolvePendingActions(0);
}

extern s32 gMagmaRockInside_Statues[];
extern s16 gMagmaRockInside_Room4Statues[];

void MagmaRockInside_FlashStatue(struct Actor *object);

// The cell watch's callback: in modes 2 and 3 it finds the object on the
// leader's cell in the room's table (room 4 has its own) and sets it blinking
// (MagmaRockInside_FlashStatue) by frame-count bit 2 in mode 2 or bit 1 in
// mode 3.
void MagmaRockInside_TargetStatue(s32 mode) {
  u16 *table;
  struct CellWatch *watch = Ram_HeapSlots->effectWork;
  s32 scene;

  table = gMagmaRockInside_Statues;
  FieldEvent_ResolvePendingActions(0);
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_MagmaRockInside2) {
  } else if (scene == (s32)&SceneId_MagmaRockInside3) {
  } else if (scene == (s32)&SceneId_MagmaRockInside4) {
    table = gMagmaRockInside_Room4Statues;
  }
  if (mode == 2) {
    struct Actor *object;

    MagmaRockInside_FindStatue(table);
    object = watch->object;
    object->unk5c[7] = 4;
    object->update = (void (*)(void))MagmaRockInside_FlashStatue;
  }
  if (mode == 3) {
    struct Actor *object;

    MagmaRockInside_FindStatue(table);
    object = watch->object;
    object->unk5c[7] = 2;
    object->update = (void (*)(void))MagmaRockInside_FlashStatue;
  }
}

void MagmaRockInside_DriftAndSlow(struct Actor *object) {
  s32 dx, dy, dz;

  object->x += dx = object->bounce;
  object->y += dy = object->gravity;
  object->z += dz = object->unk4c;
  dx -= dx >> 3;
  dy -= dy >> 3;
  dz -= dz >> 3;
  object->bounce = dx;
  object->gravity = dy;
  object->unk4c = dz;
}

extern u32 gMagmaRockInside_EmberScript[];

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);
/* VIEW(s0e20): routine MagmaRockInside_PlayBurst as void
 * MagmaRockInside_PlayBurst(u16 id); the definition has void
 * MagmaRockInside_PlayBurst(s16 value); not tried: MagmaRockInside_PlayBurst's
 * bound declaration was not found. */
extern void Func_02000ce4__s0e20(u16 id) __asm__("MagmaRockInside_PlayBurst");

static __inline__ struct Actor *Import_ActorCreate_(s32 kind, s32 x, s32 y,
                                                    s32 z) {
  return (struct Actor *)Import_ActorCreate(kind, x, y, z);
}

void MagmaRockInside_FireFromStatue(struct Actor *source) {
  struct Actor *flame =
      Import_ActorCreate_(403, source->x, source->y, source->z);
  struct Actor *other;
  s32 position[3];
  struct EffectOptions options;
  s32 step = Trig_Cos(source->facing) << 1;
  s32 cell;
  s32 id;
  s32 bit;

  flame->motionFlags = 0;
  Actor_SelectAnimation(flame, 1);
  Actor_SetSpriteFlags(flame, 0);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  position[0] = flame->x;
  position[1] = flame->y;
  position[2] = flame->z;
next:
  position[0] += step;
  cell = MagmaRockInside_GetCell(2, position[0] >> 20,
                                 (position[2] - position[1]) >> 20) >>
         8;
  if (cell == 212 || cell == 222)
    goto end;
  id = 8;
inner:
  other = ((struct ActorTable *)Ram_HeapSlots->eventWork)->actors[id];
  if (other->script == 0)
    goto skip;
  if (source->x >> 20 == other->x >> 20 && source->y >> 20 == other->y >> 20 &&
      source->z >> 20 == other->z >> 20)
    goto skip;
  if (MagmaRockInside_FlatDistance(&other->x, position) > 8)
    goto skip;
  if (other->y >> 20 != position[1] >> 20)
    goto skip;
  if (*(s16 *)(*(u8 **)((u8 *)other->sprite + 40)) == 272)
    goto end;
  goto hit;
skip:
  id++;
  if (id == 64)
    goto next;
  goto inner;
end:
  flame->acceleration = 0x20000;
  flame->speedLimit = 0x20000;
  flame->update = (void (*)(void))MagmaRockInside_TrailEmbers;
  FieldActor_SetMoveTargetFixed(flame, position[0], position[1], position[2]);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveToFixed(position[0], position[1], position[2], 1);
  FieldActor_WaitForMove(flame);
  flame->update = 0;
  Actor_Release(flame);
  id = 0;
spark:
  bit = gFrameCount;
  bit &= 1;
  if (bit == 0) {
    options.type = 403;
    options.priority = 1;
    options.update =
        (void (*)(union FieldObject *))MagmaRockInside_DriftAndSlow;
    options.script = gMagmaRockInside_EmberScript;
    FieldEffect_Spawn(position[0], position[1], position[2],
                      ((Random16() & 31) - 16) << 13,
                      ((Random16() & 31) - 16) << 13, bit, 0x1320000, &options);
  }
  System_Wait(1);
  id++;
  if (id != 16)
    goto spark;
  System_Wait(24);
  if (cell == 222) {
    MagmaRockInside_DrainLava();
    Event_End();
    return;
  }
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  System_Wait(10);
  Event_End();
  return;
hit:
  flame->acceleration = 0x20000;
  flame->speedLimit = 0x20000;
  flame->update = (void (*)(void))MagmaRockInside_TrailEmbers;
  FieldActor_SetMoveTargetFixed(flame, position[0], position[1], position[2]);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveToFixed(position[0], position[1], position[2], 1);
  FieldActor_WaitForMove(flame);
  flame->update = 0;
  Actor_Release(flame);
  Event_SetFlag(FLAG_SCENE_TEMP_F0);
  Func_02000ce4__s0e20((u16)id);
  Event_ClearFlag(FLAG_SCENE_TEMP_F0);
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  System_Wait(10);
  Event_End();
}

void MagmaRockInside_EndFallingRocks(void);

void MagmaRockInside_FireStatue(void) {
  struct MagmaRockInsideWork *work = Ram_HeapSlots->effectWork;
  struct Actor *object;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_Wait(10);
  object = work->target;
  object->update = NULL;
  FieldActor_SetSpriteColor(object, 7);
  FieldEvent_Wait(3);
  FieldActor_SetSpriteColor(work->target, 0);
  FieldEvent_Wait(1);
  MagmaRockInside_FireFromStatue(work->target);
  Event_End();
}

void MagmaRockInside_BurstStatue(s32 mode, s32 cue) {
  struct MagmaRockInsideWork *work = Ram_HeapSlots->effectWork;

  if (cue == 0)
    MagmaRockInside_StartFallingRocks();
  if (mode == 2) {
    struct Actor *object = work->target;

    MagmaRockInside_DropRocks(object->x, object->y + 0xf0000, object->z, 1);
  }
  if (mode == 3) {
    struct Actor *object = work->target;
    struct Actor *target;

    MagmaRockInside_DropRocks(object->x, object->y + 0xf0000, object->z, 8);
    target = work->target;
    SET_POSITION(target, 0, 0, 0);
  }
  if (cue == 0x3e7)
    MagmaRockInside_EndFallingRocks();
  if (cue == 178)
    Actor_SelectAnimation(work->target, 3);
}

void MagmaRockInside_PushPillarIntoLava(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *target;
  s32 position[3];
  s32 stepX;
  s32 stepZ;
  s32 i;
  s32 angle = (leader->facing + 0x2000) & 0xc000;

  position[0] = (leader->x & 0xfff00000) + 0x80000;
  position[1] = leader->y - 0x100000;
  position[2] = (leader->z & 0xfff00000) + 0x80000;
  FieldVector_AddPolarOffset(0x200000, angle, position);
  target = Actor_FindInCell(position, leader);
  if (target == 0)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SelectAnimation(leader, 6);
  System_Wait(6);
  Event_PlaySound(SE_JUMP);
  Actor_SelectAnimation(leader, 7);
  leader->speedLimit = 0x30000;
  leader->acceleration = 0x20000;
  leader->velocityY = 0x4cccc;
  leader->motionFlags &= 0x7e;
  Actor_SetSpriteFlags(leader, 0);
  stepX = (position[0] - leader->x) / 12;
  stepZ = (position[2] - leader->z) / 12;
  i = 0;
  do {
    leader->x += target->x - position[0] + stepX;
    leader->z += target->z - position[2] + stepZ;
    position[0] = target->x;
    position[2] = target->z;
    System_Wait(1);
    i++;
  } while (i != 12);
  leader->x = target->x;
  leader->y = leader->terrainHeight = target->y + 0x100000;
  leader->z = target->z;
  Actor_SelectAnimation(leader, 6);
  Actor_SetSpriteFlags(leader, 1);
  leader->motionFlags |= 3;
  Event_End();
}

extern u32 gMagmaRockInside_FlameEmberScript[];

void MagmaRockInside_DrainLava(void) {
  s32 srcX;
  s32 srcZ;
  s32 baseX = 0;
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[1];
  struct EffectOptions options;
  struct EffectOptions *effectOptions;
  struct EffectOptions *lastOptions;
  s32 position[3];
  s32 *burstPosition;
  s32 *sparkPosition;
  s32 *roomPosition;
  u32 t;
  s32 scene;
  s32 closing;
  typeof(gMainRoutineWork) *work;
  s32 riseX;
  u32 burstRandom, sparkRandom;
  u32 mask;
  u32 i;
  u32 row;

  /* FAKEMATCH(dead): srcX and srcZ start from a zero only combine sees. */
  srcX = ((u32)layer & 3) >> 2;
  srcZ = ((u32)layer & 7) >> 3;
  // t holds 212 for a set flag, then the frame count.
  t = 0;
  effectOptions = &options;
  effectOptions->type = 403;
  effectOptions->priority = 1;
  effectOptions->script = gMagmaRockInside_FlameEmberScript;
  effectOptions->startScaleX = 0x1cccc;
  effectOptions->startScaleY = 0x1cccc;
  scene = gMainRoutineWork.scene;
  roomPosition = position;
  if (scene == (s32)&SceneId_MagmaRockInside2) {
    baseX = 164 << 17;
    roomPosition[1] = 0x200000;
    roomPosition[2] = 200 << 16;
    srcX = 20;
    srcZ = 69;
    if (Event_GetFlag(FLAG_AREA_TEMP_00))
      t = 212;
  } else if (scene == (s32)&SceneId_MagmaRockInside3) {
    baseX = 232 << 16;
    roomPosition[1] = 0x200000;
    roomPosition[2] = 248 << 16;
    srcX = 14;
    srcZ = 72;
    if (Event_GetFlag(FLAG_AREA_TEMP_01))
      t = 212;
  } else if (scene == (s32)&SceneId_MagmaRockInside4) {
    baseX = 196 << 17;
    roomPosition[1] = 0x200000;
    roomPosition[2] = 134 << 18;
    srcX = 24;
    srcZ = 90;
    if (Event_GetFlag(FLAG_AREA_TEMP_02))
      t = 212;
  }
  i = 0;
burst:
  /* FAKEMATCH(temporary): burstRandom holding the first call's value. */
  burstRandom = Random16();
  mask = 31;
  /* FAKEMATCH(barrier): the empty asm clobbering sfp. */
  asm("" : : : SFP_CLOBBER);
  burstPosition = position;
  burstPosition[0] = baseX + ((burstRandom & mask) << 16);
  Event_PlaySound(SE_CRASH);
  FieldEffect_Spawn(burstPosition[0], burstPosition[1], burstPosition[2], 0,
                    (Random16() & mask) << 12, 0, 0x3a0001, effectOptions);
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  i++;
  Event_Wait(8);
  if (i != 16)
    goto burst;
  Event_PlaySound(SE_BOSS_ROAR);
  effectOptions->priority = 1;
  effectOptions->startScaleX = 0x20000;
  effectOptions->startScaleY = 0x20000;
  effectOptions->targetScaleX = 0x8000;
  effectOptions->targetScaleY = 0x8000;
  baseX -= 0x80000;
  i = 0;
  riseX = baseX;
rise:
  burstPosition[0] = riseX;
  FieldEffect_Spawn(burstPosition[0], burstPosition[1], burstPosition[2], 0, 0,
                    (Random16() & 31) << 10, 0xe0001, effectOptions);
  i++;
  riseX += 0x60000;
  if (i != 8)
    goto rise;
  MapGrid_CopyCellsTo(68, 70, srcX, srcZ, 3, 6);
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_Wait(30);
  MapGrid_CopyCellsTo(71, 70, srcX, srcZ, 3, 6);
  effectOptions->type = 403;
  effectOptions->priority = 1;
  effectOptions->palette = 10;
  effectOptions->script = gMagmaRockInside_EmberScript;
  if (t != 0) {
    MapGrid_CopyCellsTo(68, 70, srcX, srcZ, 3, 6);
    Event_Wait(30);
    burstPosition[2] -= 0x280000;
  } else {
    MapGrid_SetShake(2, 1, 1);
    Event_PlaySound(SE_TREMOR);
    row = 0;
  row:
    i = 0;
  column:
    burstPosition[0] = baseX + ((Random16() % 48) << 16);
    FieldEffect_Spawn(burstPosition[0], burstPosition[1],
                      burstPosition[2] + (row << 18) + 0x80000, 0, 0x10000, 0,
                      0x330000, effectOptions);
    i++;
    if (i != 6)
      goto column;
    wait(3);
    MapGrid_CopyCellsTo(74, 70, srcX, srcZ, 3, (row >> 2) + 6);
    row++;
    if (row != 12)
      goto row;
    MapGrid_CopyCellsTo(74, 70, srcX, srcZ, 3, 8);
    t = 0;
  frame:
    if ((t & 3) == 0) {
      i = 0;
    spark:
      /* FAKEMATCH(temporary): sparkRandom holding the call's value. */
      sparkRandom = Random16();
      sparkPosition = position;
      sparkPosition[0] = baseX + ((sparkRandom % 48) << 16);
      FieldEffect_Spawn(sparkPosition[0], sparkPosition[1],
                        sparkPosition[2] + 0x300000, 0, 0x10000, 0, 0x330000,
                        effectOptions);
      i++;
      if (i != 4)
        goto spark;
    }
    if (t == 0) {
      *(s32 *)&layer->unk08[4] -= 0x200000;
      Map_CenterOnFollowedPoint();
      Event_Wait(1);
    } else if (t <= 63) {
      REG_BLDALPHA = ((16 - (t >> 2)) << 8) | (t >> 2);
      REG_BLDCNT = 0x3f44;
    } else if (t == 64) {
      REG_BLDCNT = 0;
    } else if (t <= 127) {
      *(s32 *)&layer->unk08[4] += 0x8000;
      roomPosition = position;
      roomPosition[2] -= 0x8000;
      MapGrid_CopyCellsTo(71, 76, srcX, srcZ - ((t - 47) >> 5) + 8, 3, 1);
    } else if (t == 128) {
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    }
    t++;
    wait(1);
    if (t != 160)
      goto frame;
    FieldMapGrid_WaitForShakeToSettle();
  }
  Event_PlaySound(SE_288);
  Event_PlaySound(SE_WHOOSH);
  lastOptions = &options;
  lastOptions->startScaleY = 0x1cccc;
  lastOptions->startScaleX = 0x1cccc;
  i = 0;
last:
  /* FAKEMATCH(temporary): sparkRandom holding the call's value. */
  sparkRandom = Random16();
  sparkPosition = position;
  sparkPosition[0] = baseX + ((sparkRandom % 48) << 16);
  i++;
  FieldEffect_Spawn(sparkPosition[0], sparkPosition[1],
                    sparkPosition[2] + 0x300000, 0, 0, 0, 0x3b0000,
                    lastOptions);
  if (i != 6)
    goto last;
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_CopyCellsTo(68, 64, srcX, srcZ, 3, 6);
  FieldMapGrid_WaitForShakeToSettle();
  Event_Wait(30);
  work = &gMainRoutineWork;
  *(s32 *)&layer->unk08[4] = 0;
  closing = work->scene;
  if (closing == (s32)&SceneId_MagmaRockInside2)
    MagmaRockInside_PressGateSwitch1();
  else if (closing == (s32)&SceneId_MagmaRockInside3)
    MagmaRockInside_PressGateSwitch2();
  else if (closing == (s32)&SceneId_MagmaRockInside4)
    MagmaRockInside_PressGateSwitch3();
}

// A Magma Rock room's opening event: the leader is centred on its cell, the
// screen shakes, and by room (2, 3 or 5, the last by entrance) it picks the
// cells to open, the flag that was set (768 to 770, cleared here) and where
// the effects play. The cells are copied open; if that flag was set, ten rows
// of six effects rise along the opening while it widens and the blend fades
// over 64 steps. Then the cells close again and the leader steps back a cell.
void MagmaRockInside_FloodWithLava(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  u32 facing = (leader->facing + 0x2000) & 0xc000;
  s32 position[3];
  struct EffectOptions options;
  s32 baseX = 0;
  s32 dstX = 0;
  s32 dstZ = 0;
  s32 open = 0;
  s32 srcX = 0;
  u8 *flags;
  u32 i = 0;
  u32 j;
  s32 *p;
  s32 rowAbove;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  j = (u32)position;
  ((s32 *)j)[0] = (leader->x & 0xfff00000) + 0x80000;
  ((s32 *)j)[2] = (leader->z & 0xfff00000) + 0x80000;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, ((s32 *)j)[0] >> 16,
                      ((s32 *)j)[2] >> 16);
  FieldEvent_Wait(20);
  Event_PlaySound(SE_584);
  flags = &leader->priorityFlags;
  *flags &= ~64;
  FieldEvent_Wait(20);
  MapGrid_SetShake(1, 2, 1);
  Event_PlaySound(SE_TREMOR);
  FieldEvent_Wait(30);
  if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRockInside2) {
    srcX = 20;
    i = 89;
    dstX = 84;
    dstZ = 26;
    baseX = 160 << 17;
    ((s32 *)j)[1] = 0;
    ((s32 *)j)[2] = 212 << 17;
    if (Event_GetFlag(FLAG_AREA_TEMP_00))
      open = 212;
    Event_ClearFlag(FLAG_AREA_TEMP_00);
    facing = 0;
    ((s32 (*)())MagmaRockInside_SwapCells)(0, 15, 13, 64);
    FieldActor_SetSpritePriority(15, 3);
    for (j = 0; j != 7; j++)
      Actor_SetAnimation(j + 9, 1);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRockInside3) {
    srcX = 28;
    i = 83;
    dstX = 92;
    dstZ = 21;
    baseX = 224 << 17;
    ((s32 *)j)[1] = 0;
    ((s32 *)j)[2] = 172 << 17;
    if (Event_GetFlag(FLAG_AREA_TEMP_01))
      open = 212;
    Event_ClearFlag(FLAG_AREA_TEMP_01);
    ((s32 (*)())MagmaRockInside_SwapCells)(0, 24, 15, 64);
    ((s32 (*)())MagmaRockInside_SwapCells)(0, 9, 16, 64);
    Actor_SetAnimation(11, 4);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRockInside5) {
    if (gMainRoutineWork.entrance <= 8) {
      srcX = 49;
      /* FAKEMATCH(dead): j set again to the position's address, which it
       * already holds. */
      j = (u32)position;
      i = 75;
      dstX = 113;
      dstZ = 13;
      baseX = 198 << 18;
      ((s32 *)j)[1] = 0;
      ((s32 *)j)[2] = 216 << 16;
      if (Event_GetFlag(FLAG_AREA_TEMP_02))
        open = 212;
      Event_ClearFlag(FLAG_AREA_TEMP_02);
    } else {
      srcX = 51;
      i = 104;
      dstX = 115;
      dstZ = 42;
      baseX = 206 << 18;
      ((s32 *)j)[1] = 0;
      ((s32 *)j)[2] = 170 << 18;
      if (Event_GetFlag(FLAG_AREA_TEMP_02))
        open = 212;
      Event_ClearFlag(FLAG_AREA_TEMP_02);
    }
  }
  Event_PlaySound(SE_BOOM);
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  rowAbove = i - 1;
  MapGrid_CopyCellsTo(srcX, i, srcX, rowAbove, 3, 4);
  if (open == 0) {
    FieldMapGrid_WaitForShakeToSettle();
    FieldEvent_Wait(20);
  } else {
    MapGrid_SetShake(2, 1, 1);
    options.type = 403;
    options.priority = 2;
    options.palette = 10;
    options.script = gMagmaRockInside_EmberScript;
    for (i = 0; i != 10; i++) {
      for (j = 0; j != 6; j++) {
        u32 random = Random16();

        p = position;
        p[0] = baseX + ((random % 48) << 16);
        FieldEffect_Spawn(p[0], p[1], p[2] + (i << 18) + 0x170000, 0, 0x10000,
                          0, 0x330000, &options);
      }
      System_Wait(3);
      MapGrid_CopyCellsTo(75, 64, dstX, dstZ, 3, (i >> 1) + 1);
    }
    for (j = 0; j != 64; j++) {
      REG_BLDALPHA = ((j >> 2) << 8) | (16 - (j >> 2));
      REG_BLDCNT = 0x3f44;
      System_Wait(3);
    }
    MapGrid_CopyCellsTo(78, 64, dstX, dstZ, 3, 5);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRockInside2)
    Actor_Get(15)->priorityFlags |= 1;
  Event_PlaySound(SE_288);
  Event_PlaySound(SE_BOOM);
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_CopyCellsTo(72, 63, srcX, rowAbove, 3, 4);
  FieldMapGrid_WaitForShakeToSettle();
  Event_PlaySound(SE_584);
  {
    s32 bit = 64;
    bit |= *flags;
    *flags = bit;
  }
  FieldEvent_Wait(20);
  j = (u32)&gMainRoutineWork.currentOwner;
  Actor_Get(*(s32 *)j)->actionFlags &= ~1;
  /* FAKEMATCH(temporary): the position's address held in a pointer. */
  p = position;
  p[0] = (leader->x & 0xfff00000) + 0x80000;
  p[1] = leader->y;
  p[2] = (leader->z & 0xfff00000) + 0x80000;
  FieldVector_AddPolarOffset(0x100000, (s32)facing ^ 0x8000, p);
  leader->facing = facing;
  Actor_WalkToAndWait(*(s32 *)j, ((s16 *)p)[1], ((s16 *)p)[5]);
  Actor_Get(*(s32 *)j)->actionFlags |= 1;
  Event_End();
}

#define DEFINE_MAGMA_ROCK_INSIDE_BURST_PILLAR(name, n1, n2)                    \
  void name(s32 a, s32 b) {                                                    \
    struct Actor **work = Ram_HeapSlots->effectWork;                           \
                                                                               \
    work[5] = Actor_Get(n1);                                                   \
    MagmaRockInside_BurstStatue(a, b);                                         \
    Event_SetFlag(n2);                                                         \
  }

DEFINE_MAGMA_ROCK_INSIDE_BURST_PILLAR(MagmaRockInside_BurstCrackedPillar1, 9,
                                      0x350)

DEFINE_MAGMA_ROCK_INSIDE_BURST_PILLAR(MagmaRockInside_BurstCrackedPillar2, 11,
                                      0x351)

void MagmaRockInside_SmokeEmber(struct Actor *object) {
  if (++object->timer > 64)
    object->update = 0;
  if ((object->timer & 1) == 0) {
    struct EffectConfig config;

    config.id = 403;
    config.kind = 2;
    config.variant = 10;
    config.data = gMagmaRockInside_EmberScript;
    FieldEffect_Spawn(object->x, object->y, object->z, 0, 0x1000, 0, 0x330000,
                      &config);
  }
}

extern u32 gMagmaRockInside_FlameEmberScript[];

static __inline__ void Arrive(s32 id, s32 x, s32 z) {
  Actor_SetPosition(id, 776, 248);
  Actor_SetDestination(id, x, z);
  Actor_GetById(id)->velocityY = 0xc0000;
  Actor_GetById(id)->gravity = 0x8000;
  Actor_GetById(id)->timer = 0;
  Actor_GetById(id)->update = MagmaRockInside_SmokeEmber;
}

void MagmaRockInside_ReleaseRocks(void) {
  s32 position[3];
  struct EffectOptions options;
  s32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(SE_TREMOR);
  MapGrid_SetShake(3, 3, 1);
  Event_Wait(30);
  FieldCamera_MoveTo(776, CAMERA_KEEP, 272, 1);
  FieldCamera_WaitForArrival();
  options.type = 403;
  options.priority = 2;
  options.palette = 10;
  options.script = gMagmaRockInside_FlameEmberScript;
  options.startScaleY = 0x18000;
  options.startScaleX = 0x18000;
  position[1] = 0;
  i = 0;
loop:
  position[0] = ((Random16() & 31) << 16) + (190 << 18);
  position[2] = ((Random16() & 31) << 16) + (128 << 17);
  FieldEffect_Spawn(position[0], position[1], position[2], 0, 0x10000, 0,
                    0x3b0000, &options);
  if ((i & 7) == 0)
    Event_PlaySound(SE_567);
  if (i == 32) {
    Arrive(13, 744, 216);
  } else if (i == 40) {
    Arrive(20, 776, 328);
  } else if (i == 48) {
    Arrive(14, 856, 232);
  } else if (i == 56) {
    Arrive(19, 696, 312);
  } else if (i == 64) {
    Arrive(15, 728, 280);
  } else if (i == 72) {
    Arrive(18, 824, 280);
  } else if (i == 80) {
    Arrive(16, 824, 312);
  } else if (i == 88) {
    Arrive(17, 776, 232);
  } else if (i == 112) {
    Actor_SetPosition(12, 776, 248);
    Actor_SetDestination(12, 696, 232);
    FieldCamera_MoveTo(696, CAMERA_KEEP, 232, 1);
    Actor_GetById(12)->velocityY = 0xd0000;
    Actor_GetById(12)->gravity = 0x8000;
    Actor_GetById(12)->timer = 0;
    Actor_GetById(12)->update = MagmaRockInside_SmokeEmber;
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  }
  wait(3);
  i++;
  if (i != 128)
    goto loop;
  FieldActor_WaitForMoveById(12);
  FieldMapGrid_WaitForShakeToSettle();
  Event_PlaySound(SE_288);
  Event_Wait(40);
  Event_SetFlag(2552);
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  wait(10);
  Event_End();
}

void MagmaRockInside_TakeFlame(void) {
  u8 *work = (u8 *)Ram_HeapSlots->eventWork;

  *(work + 0xd38) = 1;
  if (!Event_GetFlag(FLAG_AREA_TEMP_04)) {
    FieldActor_SetSpriteColor(Actor_Get(22), 15);
    Actor_SetPosition(22, 632, 312);
    Actor_Get(22)->scaleX = Actor_Get(22)->scaleY = 0x1000;
  }
}

#define DEFINE_MAGMA_ROCK_INSIDE_LIGHT_TORCH(name, n1, n2, n3, n4)             \
  void name(void) {                                                            \
    if (!Event_GetFlag(n1)) {                                                  \
      FieldActor_SetSpriteColor(Actor_Get(n2), 15);                            \
      Actor_SetPosition(n2, n3, n4);                                           \
      Actor_Get(n2)->scaleX = Actor_Get(n2)->scaleY = 0x1000;                  \
    }                                                                          \
  }

DEFINE_MAGMA_ROCK_INSIDE_LIGHT_TORCH(MagmaRockInside_LightTorch, 0x303, 21, 600,
                                     312)

struct Actor;

void MagmaRockInside_GrowFlame(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *object;
  struct Actor *actor;
  struct Actor *other;
  s32 i;

  object = Actor_GetById(((s8 *)work)[0xd30]);
  FieldActor_SetSpriteColor(object, 0);
  Event_PlaySound(SE_WHOOSH);
  i = 0;
  do {
    object->scaleY = object->scaleX += 0x1000;
    wait(5);
    i++;
  } while (i != 16);
  Event_SetFlag(((s8 *)work)[0xd30] + 0x2ee);
  if (Event_GetFlag(FLAG_AREA_TEMP_03) && Event_GetFlag(FLAG_AREA_TEMP_04)) {
    other = Actor_GetById(11);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    MapGrid_SetShake(2, 1, 1);
    Event_PlaySound(SE_RUMBLE);
    FieldEvent_Wait(30);
    FieldCamera_MoveTo(616, CAMERA_KEEP, 296, 1);
    FieldCamera_WaitForArrival();
    FieldEvent_Wait(30);
    Event_PlaySound(SE_557);
    REG_BLDALPHA = 0x1000;
    REG_BLDCNT = 0x3f10;
    {
      struct Actor *prepared = OverlayObject_PrepareObject(
          other->x, other->terrainHeight, other->z + 0x300000, 312);

      other->motionFlags = 0;
      actor = prepared;
    }
    i = 0;
    do {
      other->y -= 0x10000;
      FieldEvent_Wait(5);
      i++;
    } while (i != 32);
    Event_PlaySound(SE_554);
    Actor_SelectAnimation(other, 4);
    Actor_Release(actor);
    FieldEvent_Wait(1);
    REG_BLDCNT = 0;
    MagmaRockInside_SwapCells(0, 38, 18, 0);
    Event_End();
  }
}

void MagmaRockInside_PutOutFlames(void) {
  if (*(s8 *)((u8 *)Ram_HeapSlots->eventWork + 0xd30) == -1) {
    if (!Event_GetFlag(FLAG_AREA_TEMP_04))
      Actor_SetPosition(22, 0, 0);
    if (!Event_GetFlag(FLAG_AREA_TEMP_03))
      Actor_SetPosition(21, 0, 0);
  }
}

void MagmaRockInside_ExamineEmberStone(void) {
  const u8 *message;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  message = MsgBlazeStoneEmber;
  Message_ShowCentered(message, 1);
  FieldEvent_SetMessageId(message + 1);
  Event_OpenMessage(12, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
      0) {
    Party_GiveItem(ITEM_MAGMA_BALL, 0);
    if (FieldPartyInventory_FindMember(460) == -1) {
      Event_End();
      return;
    }
    ItemDisplay_ShowItem(ITEM_MAGMA_BALL, 3);
    Actor_SetAnimation(12, 2);
    Event_SetFlag(0x9f9);
  }
  Event_End();
}

void MagmaRockInside_OpenStairs(void) {
  s32 width = 1;
  s32 height = 2;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_THUMP);
  MapGrid_CopyCellsTo(72, 67, 15, 19, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(73, 67, 15, 19, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(74, 67, 15, 19, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(75, 67, 15, 19, width, height);
  FieldEvent_Wait(15);
  Actor_Get(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_Get(gMainRoutineWork.currentOwner)->speedLimit =
      Actor_Get(gMainRoutineWork.currentOwner)->acceleration = 0x8000;
  Event_PlaySound(SE_MAP_EXIT);
  Actor_WalkTo(gMainRoutineWork.currentOwner, 248, 328);
  FieldEvent_Wait(5);
  FieldEvent_SetExitRequest(1);
  Event_End();
}

void MagmaRockInside_TrackStepCell(void) {
  struct Actor *object = Actor_Get(FieldParty_GetLeaderActor());
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  s32 key = (x << 16) + z;
  s32 last;
  s32 value;
  u8 *work;

  if (object->velocityX != 0 || object->velocityZ != 0)
    return;
  last = gMagmaRockInside_LastStepCell;
  if (last == -1 || key == last)
    return;
  x = last >> 16;
  z = last & 0xffff;
  gMagmaRockInside_LastStepCell = -1;
  MapGrid_CopyCellsLayered(
      73, 72, gMagmaRockInside_StepCellWidth, gMagmaRockInside_StepCellHeight,
      gMagmaRockInside_StepCellX, gMagmaRockInside_StepCellZ);
  if (gMainRoutineWork.scene != (s32)&SceneId_MagmaRockInside8)
    return;
  work = Ram_HeapSlots->eventWork;
  value = MagmaRockInside_GetCell(2, x, z) >> 8;
  if (value != 0) {
    /* FAKEMATCH(temporary): the value plus 200 held in delay before the store.
     */
    s32 delay = value + 200;
    *(s16 *)(work + 362) = delay;
  }
}

void MagmaRockInside_StartStepCells(void) {
  s32 none = -1;
  s32 priority;

  gMagmaRockInside_LastStepCell = none;
  priority = 0x480;
  FieldTask_AddFrame(MagmaRockInside_TrackStepCell, priority);
}

// Saves the 1 by 2 cells 64 columns across from the controlled object at
// (73, 72) and stamps the cells kept at (70, 72) in their place, once per
// cell; Func_02003d40 of Air's Rock is the same with another source.
void MagmaRockInside_StampStepCell(void) {
  struct Actor *object = Actor_Get(FieldParty_GetLeaderActor());
  s32 x = object->x >> 20;
  s32 y = object->y >> 20;
  s32 z = object->z >> 20;
  s32 key = (x << 16) + z;

  if (key != gMagmaRockInside_LastStepCell) {
    gMagmaRockInside_LastStepCell = key;
    gMagmaRockInside_StepCellWidth = 1;
    gMagmaRockInside_StepCellHeight = 2;
    gMagmaRockInside_StepCellX = x + 64;
    gMagmaRockInside_StepCellZ = z - y;
    MapGrid_CopyCellsLayered(gMagmaRockInside_StepCellX,
                             gMagmaRockInside_StepCellZ, 1, 2, 73, 72);
    MapGrid_CopyCellsLayered(
        70, 72, gMagmaRockInside_StepCellWidth, gMagmaRockInside_StepCellHeight,
        gMagmaRockInside_StepCellX, gMagmaRockInside_StepCellZ);
  }
}

// As MagmaRockInside_StampStepCell, stamping the cells kept at (71, 72).
void MagmaRockInside_StampOtherStepCell(void) {
  struct Actor *object = Actor_Get(FieldParty_GetLeaderActor());
  s32 x = object->x >> 20;
  s32 y = object->y >> 20;
  s32 z = object->z >> 20;
  s32 key = (x << 16) + z;

  if (key != gMagmaRockInside_LastStepCell) {
    gMagmaRockInside_LastStepCell = key;
    gMagmaRockInside_StepCellWidth = 1;
    gMagmaRockInside_StepCellHeight = 2;
    gMagmaRockInside_StepCellX = x + 64;
    gMagmaRockInside_StepCellZ = z - y;
    MapGrid_CopyCellsLayered(gMagmaRockInside_StepCellX,
                             gMagmaRockInside_StepCellZ, 1, 2, 73, 72);
    MapGrid_CopyCellsLayered(
        71, 72, gMagmaRockInside_StepCellWidth, gMagmaRockInside_StepCellHeight,
        gMagmaRockInside_StepCellX, gMagmaRockInside_StepCellZ);
  }
}

void MagmaRockInside_OpenFlamePassage(s32 x, s32 z) {
  struct Actor *object;
  s32 i;
  s32 cellZ;

  object = FieldActor_Create(288, x << 16, 0, z << 16);
  REG_BLDALPHA = 16;
  REG_BLDCNT = 0x3f44;
  Scene_SetActorSpritePriority(object, 0);
  Actor_SetSpriteFlags(object, 0);
  ((struct FieldSprite *)object->sprite)->blendMode = 2;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  REG_WINOUT = 0x3f1f;
  REG_DISPCNT |= 0x8000;
  Event_PlaySound(SE_DRIP);
  i = 0;
fadeIn:
  REG_BLDALPHA = (i << 9) | 16;
  System_Wait(1);
  if (++i != 8)
    goto fadeIn;
  i = 0;
fadeOut:
  REG_BLDALPHA = (16 - i) | 0x1000;
  System_Wait(1);
  if (++i != 16)
    goto fadeOut;
  x >>= 4;
  cellZ = (z >> 4) - 1;
  MapGrid_CopyCellsTo(65, 1, x + 64, cellZ, 1, 2);
  MagmaRockInside_SwapCells(0, x, cellZ, 0xff00);
  Actor_Release(object);
  Event_End();
  REG_BLDCNT = 0;
  REG_DISPCNT &= 0x7fff;
}

// Reads what the leader stands on (MagmaRockInside_GetCell, kind 2): on cells
// 1 and 7 it only redraws the marker when flag 0x200 is set; elsewhere it
// clears (MagmaRockInside_StampStepCell), redraws the marker in the row the
// event work's counter gives, sets flag 0x200 and the counter's flag (312 on).
void MagmaRockInside_StepOnMarker(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *leader;
  s32 cell, row = 54;

  leader = Actor_Get(gMainRoutineWork.currentOwner);
  cell = MagmaRockInside_GetCell(2, leader->x >> 20, leader->z >> 20);
  work += 0x16a;
  // From 54 in two steps: as one expression the constants fold into
  // 858 - 4 * counter.
  row -= (*(s16 *)work - 201) << 2;
  cell >>= 8;
  if (cell != 1 && cell != 7) {
    MagmaRockInside_StampStepCell();
    MagmaRockInside_OpenFlamePassage(264, row * 8 + 10);
    if (!Event_GetFlag(FLAG_SCENE_TEMP_00))
      Event_SetFlag(FLAG_SCENE_TEMP_00);
    Event_SetFlag(*(s16 *)work + 312);
  } else if (Event_GetFlag(FLAG_SCENE_TEMP_00)) {
    MagmaRockInside_OpenFlamePassage(264, row * 8 + 10);
  }
}

DEFINE_MAGMA_ROCK_INSIDE_LIGHT_TORCH(MagmaRockInside_LightTabletTorch, 0x305,
                                     11, 136, 360)

void MagmaRockInside_GrowTabletFlame(void) {
  struct Actor *object;
  struct Actor *actor;
  struct Actor *other;
  s32 i;

  object = Actor_Get(((s8 *)Ram_HeapSlots->eventWork)[0xd30]);
  FieldActor_SetSpriteColor(object, 0);
  Event_PlaySound(SE_WHOOSH);
  i = 0;
  do {
    object->scaleY = object->scaleX += 0x1000;
    System_Wait(5);
    i++;
  } while (i != 16);
  Event_SetFlag(FLAG_AREA_TEMP_05);
  other = Actor_Get(9);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  MapGrid_SetShake(2, 1, 1);
  Event_PlaySound(SE_RUMBLE);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(120, CAMERA_KEEP, 344, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(30);
  Event_PlaySound(SE_557);
  REG_BLDALPHA = 0x1000;
  REG_BLDCNT = 0x3f10;
  {
    struct Actor *prepared = OverlayObject_PrepareObject(
        other->x, other->terrainHeight, other->z + 0x300000, 312);

    other->motionFlags = 0;
    actor = prepared;
  }
  i = 0;
  do {
    other->y -= 0x10000;
    FieldEvent_Wait(5);
    i++;
  } while (i != 32);
  Event_PlaySound(SE_554);
  Actor_SelectAnimation(other, 4);
  Actor_Release(actor);
  FieldEvent_Wait(1);
  REG_BLDCNT = 0;
  MagmaRockInside_SwapCells(0, 7, 21, 128);
  Event_End();
}

void MagmaRockInside_PutOutTabletFlame(void) {
  if (*(s8 *)((u8 *)Ram_HeapSlots->eventWork + 0xd30) == -1) {
    if (!Event_GetFlag(FLAG_AREA_TEMP_05))
      Actor_SetPosition(11, 0, 0);
  }
}

void MagmaRockInside_SpawnSpark(struct Actor *source) {
  struct Actor *object = Create(0x2a0, source->x, source->y, source->z);

  object->scaleY = 0x6666;
  object->scaleX = 0x6666;
  object->motionFlags = 0;
  FieldActor_SetSpriteColor(object, 10);
  Scene_SetActorSpritePriority(object, 1);
  Actor_SelectAnimation(object, 1);
  Actor_SetSpriteFlags(object, 0);
  Actor_SetBlendMode(object, 1);
  Actor_StartScript(object, gMagmaRockInside_FlameScript);
}

/* VIEW(magma_rock_inside): tag struct Orbiter as struct
 * Orbiter__magma_rock_inside; module/scene/scene.h has another definition;
 * tried with the definition the rest of the object has, it does not compile in
 * tla-ja: games/tla/src/scene/magma_rock_inside.c:1742: structure has no member
 * named `parent'. */
struct Orbiter__magma_rock_inside {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x38];
  s32 radius;
  u8 unk50[0x12];
  u8 timer;
  u8 unk63;
  u16 angle;
  u16 speed;
  struct Orbiter__magma_rock_inside *parent;
};

s32 MagmaRockInside_OrbitParent(struct Orbiter__magma_rock_inside *object) {
  u16 *angle = &object->angle;
  u16 *speed = &object->speed;
  struct Orbiter__magma_rock_inside *parent = object->parent;
  s32 *position;
  s16 heading;

  object->angle += (s16)*speed >> 1;
  object->x = parent->x;
  object->y = parent->y + 0x80000;
  object->z = parent->z;
  heading = *angle;
  FieldVector_AddPolarOffset(object->radius, heading, position = &object->x);
  if (--object->timer == 0)
    return 0;
  if (object->radius == 0x20000)
    return 1;
  MagmaRockInside_SpawnSpark(object);
  object->radius -= 0x40000;
  object->angle += (s16)*speed >> 1;
  *position = parent->x;
  object->y = parent->y + 0x80000;
  object->z = parent->z;
  FieldVector_AddPolarOffset(object->radius, (s16)*angle, position);
  MagmaRockInside_SpawnSpark(object);
  object->radius -= 0x40000;
  return 1;
}

// Moves around in a circle.
DEFINE_ORBIT_UPDATE(Orbit_Update)

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

DEFINE_ACTOR_AIM_AT_WATCHED(Actor_AimAtWatched)

s32 MagmaRockInside_DriftForSteps(struct Actor *object) {
  u8 *steps;
  s32 v;
  u8 n;

  object->x += object->bounce;
  object->y += object->unk4c;
  object->z += object->gravity;
  steps = (u8 *)object + 98;
  v = *steps + 255;
  *steps = v;
  n = v;
  return n != 0;
}

void MagmaRockInside_TrailSparks(struct Actor *object) {
  struct EffectOptions options;
  s32 position[3];
  u32 frame = gFrameCount & 7;

  if (frame != 0)
    return;
  options.palette = 7;
  options.type = 673;
  options.priority = 1;
  position[0] = object->x + (((Random16() & 31) - 16) << 16);
  position[1] = object->y + ((Random16() & 31) << 16) + 0x80000;
  position[2] = object->z;
  FieldEffect_Spawn(position[0], position[1], position[2] + 0x40000, 0, 0x8000,
                    frame, 0x130000, &options);
}

union ActorView {
  struct Actor actor;
  struct {
    u8 unk00[0x62];
    u8 unk62;
    u8 unk63;
  } bytes;
};

extern u32 gMagmaRockInside_BlazeCasterScript[];
extern u32 gMagmaRockInside_TorchScript[];
extern s32 gMagmaRockInside_SparkScript[];
extern u32 gMagmaRockInside_BlazeWatcherScript[];
extern s32 gMagmaRockInside_OrbitScript[];
extern s32 gMagmaRockInside_BlazeCasterReturnScript[];
/* VIEW(magma_rock_inside): variable gMagmaRockInside_BlazeWatcherReturnScript
 * as const u8 gMagmaRockInside_BlazeWatcherReturnScript[];
 * scene/magma_rock_inside/s46e8 has
 * DEFINE_G_DATA_020052AC(gMagmaRockInside_BlazeWatcherReturnScript, 16, 3)
 * DEFINE_G_DATA_02001540(gMagmaRockInside_RubbleScript, 33)
 * DEFINE_G_DATA_02001540(gMagmaRockInside_DustScript, 44) const s32
 * *gFieldEffect_DurationScripts[] = {; tried as the one declaration extern
 * const u8 gMagmaRockInside_BlazeWatcherReturnScript[], it does not compile in
 * tla-ja: games/tla/src/scene/magma_rock_inside.c:2929: conflicting types for
 * `gMagmaRockInside_BlazeWatcherReturnScript'. */
extern const u8
    gMagmaRockInside_BlazeWatcherReturnScript__magma_rock_inside[] __asm__(
        "gMagmaRockInside_BlazeWatcherReturnScript");
#define gMagmaRockInside_BlazeWatcherReturnScript                              \
  gMagmaRockInside_BlazeWatcherReturnScript__magma_rock_inside

void MagmaRockInside_LearnBlaze(s32 a, s32 b) {
  struct Actor *first = Actor_GetById(a);
  struct Actor *second = Actor_GetById(b);
  struct Actor *object;
  struct Actor *spark;
  struct Actor *riser;
  s32 i;
  u16 noMotion;
  s32 probe;
  const u8 *script;
  // Unused: gives the stack frame its 12 bytes.
  s32 unused[3];

  script = gMagmaRockInside_BlazeWatcherScript;
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Event_Wait(30);
  FieldCamera_SetSpeed(0.5);
  FieldCamera_MoveToFixed(((s16 *)&first->x)[1] << 16,
                          ((s16 *)&first->y)[1] << 16,
                          ((s16 *)&first->z)[1] << 16, 1);
  Event_PlaySound(SE_RUMBLE);
  MapGrid_SetShake(2, 2, 1);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 1);
  PaletteFade_StartInterpolation(60);
  first->motionFlags = 0;
  first->acceleration = 0x3333;
  first->speedLimit = 0x3333;
  Actor_SetScript(a, gMagmaRockInside_BlazeCasterScript);
  Event_PlaySound(SE_194);
  Event_Wait(45);
  Actor_SetSpriteColorById(a, 256);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  i = 0;
burst:
  object = (struct Actor *)(struct Actor *)Import_ActorCreate(
      672, first->x, first->y, first->z);
  Event_PlaySound(SE_GLITTER);
  object->x += ((Random16() & 31) - 16) << 16;
  object->y += ((Random16() & 15) - 8) << 16;
  object->speedLimit = 0x80000;
  object->acceleration = 0x3333;
  object->motionFlags = 0;
  FieldActor_SetSpriteColor(object, 10);
  Scene_SetActorSpritePriority(object, 1);
  Actor_SelectAnimation(object, 7);
  Actor_SetSpriteFlags(object, 0);
  Actor_SetBlendMode(object, 1);
  Actor_StartScript(object, gMagmaRockInside_TorchScript);
  REG_BLDALPHA = 0x1004;
  wait(2);
  REG_BLDALPHA = 0x100a;
  wait(2);
  REG_BLDALPHA = 0x1010;
  wait(2);
  REG_BLDALPHA = 0x100a;
  wait(2);
  i++;
  if (i != 16)
    goto burst;
  REG_BLDALPHA = 0x1008;
  wait(30);
  i = 0;
  /* FAKEMATCH(dead): the never-taken jump into the spiral keeps its refs at
   * weight 1. */
  probe = second->unk57;
  if (probe & 0x100) {
    wait(1);
    goto tail;
  }
spiral:
  spark = FieldActor_Create(672, second->x, second->y, second->z);
  Event_PlaySound(SE_195);
  spark->timer = (Random16() & 0x7fff) + 0x8000;
  spark->unk66 = (((i & 1) << 1) - 1) << ((Random16() & 3) + 9);
  noMotion = 0;
  spark->motionFlags = noMotion;
  spark->unk4c = 0x7a0000;
  spark->watchedObject = second;
  ((union ActorView *)spark)->bytes.unk62 = ((16 - i) << 3) + 15;
  FieldActor_SetSpriteColor(spark, 10);
  Scene_SetActorSpritePriority(spark, 1);
  Actor_SelectAnimation(spark, 7);
  Actor_SetSpriteFlags(spark, 0);
  Actor_SetBlendMode(spark, 1);
  FieldVector_AddPolarOffset(spark->unk4c, spark->timer, &spark->x);
  Actor_StartScript(spark, gMagmaRockInside_SparkScript);
  if (i == 3) {
    Actor_SetSpriteColorById(b, 256);
    second->motionFlags = noMotion;
    second->acceleration = 0x3333;
    second->speedLimit = 0x3333;
    Actor_SetScript(b, script);
  }
tail:
  wait(8);
  i++;
  if (i != 16)
    goto spiral;
  Event_PlaySound(SE_SHIMMER);
  wait(16);
  i = 2;
column:
  riser = FieldActor_Create(672, second->x, second->y + i * 0x10000 - 0x80000,
                            second->z);
  riser->timer = Random16();
  riser->unk66 = 0x800;
  ((union ActorView *)riser)->bytes.unk62 = i;
  ((union ActorView *)riser)->bytes.unk63 = 1;
  riser->bounce = riser->x;
  riser->unk4c = 0;
  riser->gravity = riser->z;
  riser->watchedObject = second;
  riser->motionFlags = (u16)0;
  riser->priorityFlags = 1;
  FieldActor_SetSpriteColor(riser, 10);
  Actor_SelectAnimation(riser, 7);
  Actor_SetSpriteFlags(riser, 0);
  Actor_SetBlendMode(riser, 1);
  Actor_StartScript(riser, gMagmaRockInside_OrbitScript);
  i += 2;
  if (i != 32)
    goto column;
  Event_PlaySound(SE_SHIMMER);
  wait(50);
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  PaletteFade_StartInterpolation(8);
  wait(16);
  MapGrid_SetShake(0, 0, 0);
  Actor_SetSpriteColorById(a, 0);
  Actor_SetSpriteColorById(b, 0);
  Event_PlaySound(SE_574);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  PaletteFade_StartInterpolation(80);
  Actor_SetScript(a, gMagmaRockInside_BlazeCasterReturnScript);
  Actor_SetScript(b, gMagmaRockInside_BlazeWatcherReturnScript);
  second->update = (void (*)(void))MagmaRockInside_TrailSparks;
  wait(120);
  second->update = 0;
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Scene_PlayMusic();
  wait(1);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
#define EVENT_WORK ((struct EventWork *)Ram_HeapSlots->eventWork)

s32 Actor_MoveToObject();

// Reading the Blaze tablet shows its inscription; the first time (flag 0x9fa
// clear) the scene with actor 5 follows.
void MagmaRockInside_ReadBlazeTablet(void) {
  struct Actor *object;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered(MsgBlazeTabletInscription, 1);
  if (Event_GetFlag(0x9fa) != 0) {
    Event_End();
  } else {
    FieldEvent_SetMessageId((s32)MsgBlazeTabletInscription);
    EVENT_WORK->messageId++;
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.7);
    Actor_SetSpeed(5, 0.7);
    Actor_MoveToObject(5, gMainRoutineWork.currentOwner);
    FieldEvent_Wait(1);
    Actor_WalkTo(gMainRoutineWork.currentOwner, 256, 136);
    Actor_WalkTo(5, 272, 136);
    FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
    FieldActor_WaitForMoveById(5);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
    Actor_FaceDirection(5, FACING_WEST, 0);
    FieldEvent_Wait(10);
    Event_ShowMessage(5, 0);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
    FieldEvent_Wait(30);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 256, 152);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Actor_FaceDirection(5, FACING_SOUTH, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimation(5, 3);
    FieldEvent_Wait(30);
    Actor_FaceDirection(5, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Actor_WalkToAndWait(5, 264, 137);
    Actor_FaceDirection(5, FACING_NORTH, 0);
    FieldEvent_Wait(20);
#if (LANGUAGE == LANGUAGE_JAPANESE) || (LANGUAGE == LANGUAGE_ENGLISH)
    Message_ShowCentered(MsgBlazeTabletInscription + 3, 1);
#else
    Message_ShowCentered(MsgBlazeTabletInscription + 2, 1);
#endif
    MapGrid_SetShake(5, 7, 1);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    FieldMapGrid_WaitForShakeToSettle();
    Call2(MagmaRockInside_LearnBlaze, 8, 5);
    Party_LearnAbility(JENNA, ABILITY_BLAZE);
    Event_SetFlag(0x9fa);
    Actor_FaceEachOther(gMainRoutineWork.currentOwner, 5, 0);
    FieldEvent_Wait(10);
    Actor_SetAnimation(5, 3);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
    FieldEvent_Wait(20);
    Actor_SetSpeed(5, 1.2);
    Actor_SetAnimation(5, 2);
    object = Actor_Get(gMainRoutineWork.currentOwner);
    if (object != 0)
      Actor_SetDestination(5, FIELD_AT(object, s16, 10),
                           FIELD_AT(object, s16, 18));
    FieldActor_WaitForMoveById(5);
    Actor_SetPosition(5, 0, 0);
    Actor_SetActionCallback(Actor_Get(8), 9);
    Event_End();
  }
}

void MagmaRockInside_ReadBlazeTabletFromWest(void) {
  if (Event_GetFlag(0x9fa)) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Message_ShowCentered(MsgBlazeTabletInscription, 1);
    Event_End();
  } else {
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 107);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 136);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 136);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    MagmaRockInside_ReadBlazeTablet();
  }
}

#define DEFINE_MAGMA_ROCK_INSIDE_READ_TABLET_FROM(name, n1)                    \
  void name(void) {                                                            \
    if (Event_GetFlag(0x9fa)) {                                                \
      Event_Start();                                                           \
      FieldEvent_ResolvePendingActions(0);                                     \
      Message_ShowCentered(MsgBlazeTabletInscription, 1);                      \
      Event_End();                                                             \
    } else {                                                                   \
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, n1, 136);             \
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 136);            \
      Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);     \
      FieldEvent_Wait(10);                                                     \
      MagmaRockInside_ReadBlazeTablet();                                       \
    }                                                                          \
  }

DEFINE_MAGMA_ROCK_INSIDE_READ_TABLET_FROM(
    MagmaRockInside_ReadBlazeTabletFromEast, 280)

DEFINE_MAGMA_ROCK_INSIDE_READ_TABLET_FROM(
    MagmaRockInside_ReadBlazeTabletFromSouth, 248)

extern struct Trigger gMagmaRockInside_Triggers[];
extern struct Trigger gMagmaRockInside_Room2Triggers[];
extern struct Trigger gMagmaRockInside_Room3Triggers[];
extern struct Trigger gMagmaRockInside_Room4Triggers[];
extern struct Trigger gMagmaRockInside_Room5Triggers[];
extern struct Trigger gMagmaRockInside_Room6Triggers[];
extern struct Trigger gMagmaRockInside_Room7Triggers[];
extern struct Trigger gMagmaRockInside_Room8Triggers[];

const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene != (s32)&SceneId_MagmaRockInside1) {
    if (scene == (s32)&SceneId_MagmaRockInside2)
      return gMagmaRockInside_Room2Triggers;
    if (scene == (s32)&SceneId_MagmaRockInside3)
      return gMagmaRockInside_Room3Triggers;
    if (scene == (s32)&SceneId_MagmaRockInside4)
      return gMagmaRockInside_Room4Triggers;
    if (scene == (s32)&SceneId_MagmaRockInside5)
      return gMagmaRockInside_Room5Triggers;
    if (scene == (s32)&SceneId_MagmaRockInside6)
      return gMagmaRockInside_Room6Triggers;
    if (scene == (s32)&SceneId_MagmaRockInside7)
      return gMagmaRockInside_Room7Triggers;
    if (scene == (s32)&SceneId_MagmaRockInside8)
      return gMagmaRockInside_Room8Triggers;
  }
  return gMagmaRockInside_Triggers;
}

void MagmaRockInside_PressGateSwitch1(void) {
  s32 i;

  Event_SetFlag(FLAG_AREA_TEMP_00);
  MagmaRockInside_SwapCells(0, 15, 13, 0);
  for (i = 0; i != 6; i++)
    Actor_SetAnimation(i + 9, 2);
  Actor_SetAnimation(15, 4);
}

void MagmaRockInside_PressGateSwitch2(void) {
  Event_SetFlag(FLAG_AREA_TEMP_01);
  if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRockInside3) {
    MagmaRockInside_SwapCells(0, 24, 15, 0);
    MagmaRockInside_SwapCells(0, 9, 16, 0);
    Actor_SetAnimation(11, 4);
  } else {
    MagmaRockInside_SwapCells(0, 26, 14, 0);
  }
}

void MagmaRockInside_PressGateSwitch3(void) {
  // Event_SetFlag, event.h's command, keeps 0x302 from being shared with the
  // script calls, which build it again after the scene test; GameFlag_Set
  // called directly differs in 56 instructions.
  Event_SetFlag(FLAG_AREA_TEMP_02);
  if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRockInside4) {
    s16 *script = gMagmaRockInside_PlatformPath6;

    MagmaRockInside_StartLavaPlatform(15, script, 0x302);
    MagmaRockInside_StartLavaPlatform(16, gMagmaRockInside_PlatformPath6,
                                      0x302);
    MagmaRockInside_StartLavaPlatform(17, gMagmaRockInside_PlatformPath7,
                                      0x302);
    MagmaRockInside_StartLavaPlatform(18, gMagmaRockInside_PlatformPath7,
                                      0x302);
    MagmaRockInside_StartLavaPlatform(19, gMagmaRockInside_PlatformPath8,
                                      0x302);
    MagmaRockInside_StartLavaPlatform(20, gMagmaRockInside_PlatformPath8,
                                      0x302);
    MagmaRockInside_StartLavaPlatform(21, gMagmaRockInside_PlatformPath9,
                                      0x302);
    MagmaRockInside_StartLavaPlatform(22, gMagmaRockInside_PlatformPath9,
                                      0x302);
    if (Actor_Get(16)->step == 0) {
      Actor_Get(16)->step = 10;
      Actor_Get(18)->step = 10;
      Actor_Get(20)->step = 10;
      Actor_Get(22)->step = 10;
    }
    Actor_SetAnimation(23, 4);
    Actor_SetAnimation(24, 4);
    Actor_SetAnimation(25, 4);
    Actor_SetAnimation(26, 2);
  }
}

void MagmaRockInside_LayerWithLeader(struct Actor *arg) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *object;
  struct Actor *owner;
  s16 ownerY;
  s16 y;
  u8 *flags;

  do {
  } while (0);
  object = arg;
  owner = Actor_Get(party->currentOwner);
  ownerY = owner->y >> 16;
  y = object->y >> 16;
  flags = &object->priorityFlags;

  if (ownerY != y || (s16)(owner->z >> 16) < (s16)(object->z >> 16)) {
    *flags = 1;
  } else {
    *flags = 0;
    Scene_SetActorSpritePriority(object, 3);
  }
}
#undef BYTE_AT
#undef FIELD_AT
#undef EVENT_WORK

#undef gMagmaRockInside_Room4Statues
#undef gMagmaRockInside_Placements
#undef gMagmaRockInside_Room2Placements
#undef gMagmaRockInside_Room3Placements
#undef gMagmaRockInside_Room4Placements
#undef gMagmaRockInside_Room5Placements
#undef gMagmaRockInside_Room6Placements
#undef gMagmaRockInside_Room7Placements
#undef gMagmaRockInside_Room8Placements
#undef gMagmaRockInside_Room2Triggers
#undef gMagmaRockInside_Room3Triggers
#undef gMagmaRockInside_Room4Triggers
#undef gMagmaRockInside_Room5Triggers
#undef gMagmaRockInside_Room6Triggers
#undef gMagmaRockInside_Room7Triggers
#undef gMagmaRockInside_Room8Triggers
#undef DEFINE_MAGMA_ROCK_INSIDE_BURST_PILLAR

#undef gMagmaRockInside_BlazeWatcherReturnScript
// Magma Rock's inside: its crumbling pillars and traps.

void Tightrope_Initialize(void);

extern s16 gMagmaRockInside_PlatformPath1[];
extern u32 gMagmaRockInside_PlatformPath2[];
extern u16 gMagmaRockInside_PlatformPath3[];
extern u32 gMagmaRockInside_PlatformPath4[];
extern s16 gMagmaRockInside_PlatformPath5[];
extern s16 gMagmaRockInside_PlatformPath10[];
extern s16 gMagmaRockInside_PlatformPath11[];
extern s16 gMagmaRockInside_PlatformPath12[];
extern s32 gMagmaRockInside_Statues[];

void Actor_PlaceBlocks(const u8 *table);
void Tightrope_Place(s32 a, s32 b, s32 c);
s32 MagmaRockInside_SwapCells(u32 layer, s32 x, s32 y, s32 value);

#define CLOSED_BLEND()                                                         \
  {                                                                            \
    REG_BLDALPHA = 0x1000;                                                     \
    REG_BLDCNT = 0x3f44;                                                       \
  }

static __inline__ void Raise(s32 id) {
  struct Actor *object = Actor_GetById(id);

  object->motionFlags = 2;
  object->y = object->terrainHeight = 20 << 16;
  object->priorityFlags |= 128;
}

static __inline__ void Lower(s32 id, s32 height) {
  Actor_GetById(id)->motionFlags = 0;
  Actor_GetById(id)->y = Actor_GetById(id)->terrainHeight = height;
}

s32 Scene_Initialize(void) {
  s32 i;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 516;
  if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRockInside1) {
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
    MagmaRockInside_StartLavaPlatform(8, gMagmaRockInside_PlatformPath1, 768);
    if (!Event_GetFlag(FLAG_AREA_TEMP_00)) {
      CLOSED_BLEND();
      MapGrid_CopyCellsTo(71, 64, 47, 98, 1, 1);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRockInside2) {
    s32 j;

    FieldEffect_StartBlankOverlay(0);
    Actor_GetById(15)->update = MagmaRockInside_LayerWithLeader;
    j = 0;
    do {
      {
        struct Actor *object = Actor_GetById(j + 9);

        object->motionFlags = 0;
        object->y = object->terrainHeight = 0;
      }
    } while (++j != 6);
    if (Event_GetFlag(2544))
      Actor_GetById(15)->z = 132 << 17;
    Actor_PlaceBlocks(gMagmaRockInside_Room2Pillars);
    MagmaRockInside_RaiseActorCells(gMagmaRockInside_Statues);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
    Raise(16);
    if (Event_GetFlag(FLAG_AREA_TEMP_00))
      MagmaRockInside_PressGateSwitch1();
    else
      CLOSED_BLEND();
  } else if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRockInside3) {
    FieldEffect_StartBlankOverlay(0);
    if (Event_GetFlag(2545))
      Actor_GetById(11)->z = 166 << 18;
    Actor_PlaceBlocks(gMagmaRockInside_Room3Pillars);
    Tightrope_Initialize();
    Tightrope_Place(0, 13, 14);
    MagmaRockInside_RaiseActorCells(gMagmaRockInside_Statues);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
    Raise(9);
    MagmaRockInside_StartLavaPlatform(10, gMagmaRockInside_PlatformPath2, 769);
    MagmaRockInside_StartLavaPlatform(12, gMagmaRockInside_PlatformPath3, 769);
    if (Event_GetFlag(FLAG_AREA_TEMP_01))
      MagmaRockInside_PressGateSwitch2();
    else
      CLOSED_BLEND();
  } else if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRockInside4) {
    FieldEffect_StartBlankOverlay(0);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
    MagmaRockInside_RaiseActorCells(gMagmaRockInside_Room4Statues);
    if (gMainRoutineWork.entrance <= 7) {
      MagmaRockInside_StartLavaPlatform(10, gMagmaRockInside_PlatformPath4,
                                        769);
      MagmaRockInside_StartLavaPlatform(12, gMagmaRockInside_PlatformPath5,
                                        769);
      if (Event_GetFlag(FLAG_AREA_TEMP_01)) {
        MagmaRockInside_PressGateSwitch2();
      } else {
        CLOSED_BLEND();
        MapGrid_CopyCellsTo(71, 64, 35, 82, 1, 1);
      }
    } else {
      Actor_GetById(23)->priorityFlags |= 32;
      Actor_GetById(24)->priorityFlags |= 32;
      Actor_GetById(25)->priorityFlags |= 32;
      Actor_SetSpritePriority(23, 3);
      Actor_SetSpritePriority(24, 3);
      Actor_SetSpritePriority(25, 3);
      if (Event_GetFlag(2546))
        Actor_GetById(23)->x = 172 << 17;
      if (Event_GetFlag(2547))
        Actor_GetById(24)->x = 172 << 17;
      if (Event_GetFlag(2548))
        Actor_GetById(25)->x = 172 << 17;
      Actor_PlaceBlocks(gMagmaRockInside_Room4Pillars);
      Lower(26, 0);
      Actor_GetById(15)->unk5c[0] = 3;
      Actor_GetById(16)->unk5c[0] = 3;
      Actor_GetById(17)->unk5c[0] = 3;
      Actor_GetById(18)->unk5c[0] = 3;
      Actor_GetById(19)->unk5c[0] = 3;
      Actor_GetById(20)->unk5c[0] = 3;
      Actor_GetById(21)->unk5c[0] = 3;
      Actor_GetById(22)->unk5c[0] = 3;
      MagmaRockInside_StartLavaPlatform(15, gMagmaRockInside_PlatformPath6,
                                        770);
      MagmaRockInside_StartLavaPlatform(16, gMagmaRockInside_PlatformPath6,
                                        770);
      MagmaRockInside_StartLavaPlatform(17, gMagmaRockInside_PlatformPath7,
                                        770);
      MagmaRockInside_StartLavaPlatform(18, gMagmaRockInside_PlatformPath7,
                                        770);
      MagmaRockInside_StartLavaPlatform(19, gMagmaRockInside_PlatformPath8,
                                        770);
      MagmaRockInside_StartLavaPlatform(20, gMagmaRockInside_PlatformPath8,
                                        770);
      MagmaRockInside_StartLavaPlatform(21, gMagmaRockInside_PlatformPath9,
                                        770);
      MagmaRockInside_StartLavaPlatform(22, gMagmaRockInside_PlatformPath9,
                                        770);
      if (Event_GetFlag(FLAG_AREA_TEMP_02))
        MagmaRockInside_PressGateSwitch3();
      else
        CLOSED_BLEND();
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRockInside5) {
    FieldEffect_StartBlankOverlay(0);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
    HangingVine_Place(10, 1);
    HangingVine_Place(11, 1);
    Raise(8);
    Raise(9);
    if (Event_GetFlag(FLAG_AREA_TEMP_02)) {
      MagmaRockInside_SwapCells(0, 22, 26, 0);
      MagmaRockInside_SwapCells(0, 25, 26, 0);
      Actor_SetSpritePriority(64, 3);
    } else {
      CLOSED_BLEND();
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRockInside6) {
    FieldEffect_StartBlankOverlay(0);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
    if (Event_GetFlag(2549)) {
      MapGrid_CopyCellAttributes(46, 74, 1, 1, 46, 73);
      Actor_GetById(9)->x = 190 << 18;
    }
    if (Event_GetFlag(2550))
      Actor_SetPosition(12, 920, 616);
    Actor_PlaceBlocks(gMagmaRockInside_Room6Pillars);
    MagmaRockInside_StartLavaPlatform(8, gMagmaRockInside_PlatformPath10, 770);
    MagmaRockInside_StartLavaPlatform(10, gMagmaRockInside_PlatformPath11, 770);
    MagmaRockInside_StartLavaPlatform(11, gMagmaRockInside_PlatformPath12, 770);
    if (Event_GetFlag(FLAG_AREA_TEMP_02)) {
      Actor_SetAnimation(9, 4);
      if (Event_GetFlag(2550))
        Actor_SetAnimation(12, 4);
      if (Event_GetFlag(2549))
        MagmaRockInside_SwapCells(2, 46, 8, 0xff00);
      MagmaRockInside_SwapCells(0, 10, 45, 0);
      MagmaRockInside_SwapCells(0, 53, 40, 0);
    } else {
      CLOSED_BLEND();
      MapGrid_CopyCellsTo(71, 64, 50, 102, 1, 1);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRockInside7) {
    i = 0;
  room7:
    {
      struct Actor *object = Actor_GetById(i + 8);

      Actor_SelectAnimation(object, 4);
      object->motionFlags = 0;
      object->y = object->terrainHeight = -0x200000;
    }
    if (++i != 3)
      goto room7;
    if (Event_GetFlag(2551)) {
      Actor_SetAnimation(23, 4);
      Actor_SetPosition(23, 696, 648);
    }
    Actor_PlaceBlocks(gMagmaRockInside_Room7Pillars);
    FieldEffect_StartBlankOverlay(0);
    if (!Event_GetFlag(FLAG_AREA_TEMP_03))
      Actor_SetPosition(21, 0, 0);
    if (!Event_GetFlag(FLAG_AREA_TEMP_04))
      Actor_SetPosition(22, 0, 0);
    if (Event_GetFlag(FLAG_AREA_TEMP_03) && Event_GetFlag(FLAG_AREA_TEMP_04)) {
      Lower(11, -0x200000);
      Actor_SetAnimation(11, 4);
    } else {
      MagmaRockInside_SwapCells(0, 38, 18, 0xff00);
    }
    if (Event_GetFlag(2553))
      Actor_SetAnimation(12, 0);
    for (i = 0; i != 4; i++)
      Actor_SetAnimation(i + 13, 2);
    for (i = 0; i != 11; i++)
      Actor_SetSpriteFlags(Actor_GetById(i + 12), 0);
    if (!Event_GetFlag(2552)) {
      for (i = 0; i != 9; i++)
        Actor_SetPosition(i + 12, 0, 0);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRockInside8) {
    MagmaRockInside_StartStepCells();
    Event_SetFlag(FLAG_RETREAT_BLOCKED);
    i = 0;
  room8:
    if (Event_GetFlag(i + 514)) {
      s32 y = 24 - i * 2;

      MapGrid_CopyCellsTo(65, 1, 80, y, 1, 2);
      Call4((void (*)())MagmaRockInside_SwapCells, 0, 16, y, 0xff00);
    }
    if (++i != 6)
      goto room8;
    Actor_SetSpriteFlags(Actor_GetById(10), 0);
    Actor_SetSpriteFlags(Actor_GetById(11), 0);
    if (Event_GetFlag(FLAG_AREA_TEMP_05)) {
      Lower(9, -0x200000);
      Actor_SetAnimation(9, 4);
    } else {
      MagmaRockInside_SwapCells(0, 7, 21, 0xff00);
      Actor_SetPosition(11, 0, 0);
    }
    Lower(8, 0x100000);
    Actor_SetActionCallback(Actor_GetById(8), 9);
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    if ((scene == (s32)&SceneId_MagmaRockInside2 && entrance == 8) ||
        (scene == (s32)&SceneId_MagmaRockInside7 && entrance == 11)) {
      Event_ClearFlag(FLAG_AREA_TEMP_00);
      Event_ClearFlag(FLAG_AREA_TEMP_01);
      Event_ClearFlag(FLAG_AREA_TEMP_02);
    }
  }
  return 0;
}

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
s32 MapCollision_FindOrAddTag(s32 unused, s32 key, s32 value);

// On entering, sets each listed pillar from its two flags, then lifts the
// leader onto the ground under it.
DEFINE_PILLARS_RESTORE_RAISED(MagmaRockInside_RestoreCrumblingPillars,
                              Actor_GetCellIndex, MapGrid_GetTerrainKind,
                              Actor_DisableById, MapCollision_FindOrAddTag,
                              Actor_SelectAnimation)

DEFINE_TRAPS_CHECK(MagmaRockInside_CheckCrumblingStep, Field_TryJumpForward,
                   Actor_FindAtCell, Actor_SelectAnimation,
                   MagmaRockInside_SpawnRubble,
                   MagmaRockInside_SetLeaderCellShape,
                   MagmaRockInside_SpawnDustRing)

void MagmaRockInside_CrumblePillar(const void *data, s32 id) {
  const struct TrapEntry *entry = data;
  struct Actor *object = Actor_Get(id);
  s32 flag;

  if (entry->actor != 0xffff) {
  next:
    if (entry->actor != id) {
      entry++;
      goto test;
    }
    flag = entry->flag;
    Event_Start();
    if (!Event_GetFlag(flag + 1)) {
      Event_PlaySound(SE_PUSH);
      MapGrid_SetShake(1, 4, 1);
      MagmaRockInside_SpawnRubble(0);
      System_Wait(8);
      Actor_SelectAnimation(object, 2);
      MagmaRockInside_SetActorCellShape(-4, OBJECT_BYTE(object, 34), id);
      MagmaRockInside_SpawnRubble(1);
      System_Wait(16);
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      System_Wait(8);
      FieldActor_SetPositionAndResetMotion(object, 0, 0, 0);
      System_Wait(2);
      Event_PlaySound(SE_THUMP);
      MagmaRockInside_SpawnDustRing();
      System_Wait(20);
      Event_SetFlag(flag);
      Event_SetFlag(flag + 1);
    }
    Event_End();
    return;
  test:
    if (entry->actor != 0xffff)
      goto next;
  }
}

void MagmaRockInside_SetPillarMode3(s32 id) {
  Actor_Get(id)->unk5c[0] = 3;
  Actor_DisableById(id);
}

// Places actors 8, 9 and 10, then walks a 0xffff-ended list of (actor, flag)
// halfword pairs: flag + 1 set parks the actor at (0, 0); otherwise its cell
// gets attributeB 128 and its shape-grid byte is rewritten, and flag set makes
// it copy actor 9's visual attributes.
DEFINE_PILLARS_RESTORE(MagmaRockInside_RestorePillars,
                       MagmaRockInside_SetPillarMode3, Actor_GetCellIndex,
                       MapGrid_GetTerrainKind, Actor_DisableById,
                       MapCollision_FindOrAddTag)

void MagmaRockInside_TakePlaceOf(struct Actor *object, struct Actor *target) {
  FieldActor_SetPositionAndResetMotion(object, target->x, target->y, target->z);
  FieldActor_SetPositionAndResetMotion(target, 0, 0, 0);
}

void MagmaRockInside_StepOnTrap(const u16 *table) {
  struct Actor *leader;
  struct Actor *partner;
  struct Actor *trap;
  s32 position[3];
  s32 id;
  s32 flag;
  s32 second;

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  partner = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  trap = Actor_FindAtCell(position, 0);
  if (trap != 0)
    goto test;
  return;
next:
  id = table[0];
  if (Actor_GetById(id) != trap) {
    table += 2;
    goto test;
  }
  {
    flag = table[1];
    Event_Start();
    if (!Event_GetFlag(flag)) {
      Actor_SelectAnimation(partner, 1);
      MagmaRockInside_TakePlaceOf(partner, trap);
      wait(1);
      Event_PlaySound(SE_CHEST_CLOSE);
      FieldActor_SetSpriteColor(Actor_GetById(8), 7);
      wait(2);
      Actor_SelectAnimation(partner, 0);
      Actor_CopyGraphics(id, 9);
      FieldActor_SetSpriteColor(Actor_GetById(8), 0);
      wait(2);
      FieldActor_SetSpriteColor(Actor_GetById(8), 7);
      wait(4);
      FieldActor_SetSpriteColor(Actor_GetById(8), 0);
      MagmaRockInside_SpawnRubble(0);
      MagmaRockInside_TakePlaceOf(trap, partner);
      wait(1);
      Event_SetFlag(flag);
    } else {
      second = flag + 1;
      if (!Event_GetFlag(second)) {
        Actor_SelectAnimation(partner, 0);
        MagmaRockInside_TakePlaceOf(partner, trap);
        wait(1);
        leader->motionFlags = 0;
        Event_PlaySound(SE_PUSH);
        MapGrid_SetShake(1, 4, 1);
        MagmaRockInside_SpawnRubble(0);
        wait(8);
        Actor_SelectAnimation(partner, 2);
        MagmaRockInside_SetLeaderCellShape(-2, trap->terrainId);
        MagmaRockInside_SpawnRubble(1);
        wait(16);
        MagmaRockInside_SetLeaderCellShape(-2, trap->terrainId);
        wait(4);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        wait(8);
        leader->motionFlags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(partner, 0, 0, 0);
        wait(2);
        Event_PlaySound(SE_THUMP);
        MagmaRockInside_SpawnDustRing();
        wait(20);
        Event_SetFlag(second);
      }
    }
    Event_End();
    return;
  }
test:
  if (*table != 0xffff)
    goto next;
}

#include "../field/field_effect/advance_motion_priority.c"

#undef MagmaRockInside_StartLavaPlatform

#undef MagmaRockInside_StartLavaPlatform
#undef CLOSED_BLEND
// Magma Rock's inside: its falling rocks and rubble, and its tables.

void Actor_StartScript(struct Actor *object, const void *data);

void MagmaRockInside_SpawnRubble(s32 spread) {
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
      Actor_SetSpriteFlags(effect, 0);
      Actor_SelectAnimation(effect, 2);
      Actor_StartScript(effect, gMagmaRockInside_RubbleScript);
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

void MagmaRockInside_SpawnDustRing(void) {
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
      Actor_SetSpriteFlags(effect, 0);
      Actor_SelectAnimation(effect, 2);
      Actor_StartScript(effect, gMagmaRockInside_DustScript);
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

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
s32 MapCollision_FindOrAddTag(s32 layer, s32 shape, s32 height);

DEFINE_ACTOR_SET_CELL_SHAPE(MagmaRockInside_SetActorCellShape,
                            MapGrid_GetTerrainKind, MapCollision_FindOrAddTag)

void MagmaRockInside_SetLeaderCellShape(s32 a, s32 b) {
  MagmaRockInside_SetActorCellShape(a, b, gMainRoutineWork.currentOwner);
}

void BattleEffect_StepVector3(struct Particle *particle, s32 unk1, s32 step);

DEFINE_PARTICLES_UPDATE_FALLING(MagmaRockInside_UpdateFallingRocks,
                                EffectSprite_DrawAtWorldPosition)

// The falling rocks' work: their tiles, 64 rocks and their sprites, in a
// ring that next walks round.
struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct RockSprite sprites[64];
  s16 next;
  u16 tileBase;
  s16 resource;
};

// Spawns count rocks around (x, y, z), each pushed out at a random angle and
// thrown up and out along it.
DEFINE_ROCKS_SPAWN(MagmaRockInside_DropRocks)

void MagmaRockInside_StartFallingRocks(void) {
  struct RockWork *work = HeapSlot_Alloc(220, sizeof(struct RockWork));
  s32 entry;
  s32 tile;
  s32 i;
  struct Rock *rock;
  struct RockSprite *sprite;

  Func_02004c58(Resource_GetTableEntry((s32)&ResourceId_FallingRock), work);
  entry = VramCache_FindFreeEntry();
  tile = Vram_Load(entry, 0x500, work);
  work->tileBase = tile;
  work->resource = entry;
  rock = work->rocks;
  sprite = work->sprites;
  for (i = 63; i >= 0; i--, sprite++, rock++) {
    Func_02004f10(sprite, 8, 8, 0x40000000, tile);
    sprite->unk05_5 = 1;
    sprite->palette = 0;
    sprite->priority = 0;
    sprite->unk1e = 240;
    rock->timer = -1;
  }
  {
    s32 zero = 0;

    work->next = zero;
  }
  FieldTask_AddFrame(MagmaRockInside_UpdateFallingRocks, 0x480);
}

void MagmaRockInside_EndFallingRocks(void) {
  struct MagmaRockInsideSparkWork *work = Ram_HeapSlots->menuRuntime;

  FieldTask_Remove(MagmaRockInside_UpdateFallingRocks);
  VramCache_FreeEntry(work->resource);
  HeapSlot_Release(220);
}

void MagmaRockInside_MeetDjinni(void);
void MagmaRockInside_ResolveBeforeBurst(void);
void MagmaRockInside_FireStatue(void);
void MagmaRockInside_LightTorch(void);
void MagmaRockInside_OpenStairs(void);

extern u32 gFieldEffect_LongScript[];

// clang-format off
DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_ShortScript, 22)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_MediumScript, 44)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_LongScript, 126)

DEFINE_PUSH_LEADER_STEPS(gPushLeaderSteps)

u32 gMagmaRockInside_EmberScript[] = {
    39, 2, 0, 18, 3, 0,
    1, 38, 17,
};

u32 gMagmaRockInside_FlameEmberScript[] = {
    39, 3, 0, 23, 3, 0,
    1, 38, 17,
};

DEFINE_G_DATA_02005074(gMagmaRockInside_PlatformPath1, 776, 680, 744, 680, 744, 632, 776, 632)

DEFINE_G_DATA_020050D4(gMagmaRockInside_PlatformPath2, 216, 568, 216, 584, 136, 584, 136, 568)

u16 gMagmaRockInside_PlatformPath3[] = {
    4, 0, 0, 600, 0, 16, 0, 664,
    1, 0, 4, 0, 0, 728, 0, 16,
    0, 664, 1, 0, 4, 0, 0, 728,
    0, 16, 0, 632, 1, 0, 4, 0,
    0, 760, 0, 16, 0, 632, 1, 0,
    4, 0, 0, 760, 0, 16, 0, 664,
    1, 0, 17, 0,
};

DEFINE_G_DATA_020050D4(gMagmaRockInside_PlatformPath4, 184, 232, 280, 232, 280, 264, 184, 264)

DEFINE_G_DATA_02005074(gMagmaRockInside_PlatformPath5, 472, 296, 424, 296, 424, 264, 472, 264)

DEFINE_G_DATA_02005074(gMagmaRockInside_PlatformPath6, 216, 600, 280, 600, 280, 664, 216, 664)

DEFINE_G_DATA_02005074(gMagmaRockInside_PlatformPath7, 312, 664, 376, 664, 376, 600, 312, 600)

DEFINE_G_DATA_02005074(gMagmaRockInside_PlatformPath8, 440, 600, 504, 600, 504, 664, 440, 664)

DEFINE_G_DATA_02005074(gMagmaRockInside_PlatformPath9, 536, 664, 600, 664, 600, 600, 536, 600)

DEFINE_G_DATA_02005074(gMagmaRockInside_PlatformPath10, 376, 264, 376, 232, 408, 232, 408, 264)

DEFINE_G_DATA_02005074(gMagmaRockInside_PlatformPath11, 216, 680, 280, 680, 280, 712, 216, 712)

DEFINE_G_DATA_02005074(gMagmaRockInside_PlatformPath12, 376, 680, 312, 680, 312, 712, 376, 712)

s16 gMagmaRockInside_Room2Pillars[] = {
    15, -1,
};

s32 gMagmaRockInside_Statues[] = {
    0x320008, -1,
};

s16 gMagmaRockInside_Room3Pillars[] = {
    11, -1,
};

s16 gMagmaRockInside_Room4Statues[] = {
    8, 50, -1, 13, 51, -1, 14, 52,
    -1, -1,
};

s16 gMagmaRockInside_Room4Pillars[] = {
    23, 24, 25, -1,
};

s16 gMagmaRockInside_Room6Pillars[] = {
    9, 12, -1,
};

s16 gMagmaRockInside_Room7Pillars[] = {
    23, -1, 0,
};

u32 gMagmaRockInside_FlameScript[] = {
    0, 6, 38, 17,
};

DEFINE_G_DATA_02005218(gMagmaRockInside_TorchScript)

s32 gMagmaRockInside_SparkScript[] = {
    SC_CALL(MagmaRockInside_OrbitParent),
    SC_FREE,
    SC_END,
};

s32 MagmaRockInside_DriftForSteps(struct Actor *object);

DEFINE_G_DATA_02005244(gMagmaRockInside_OrbitScript, Orbit_Update, Actor_AimAtWatched,
    MagmaRockInside_DriftForSteps)

DEFINE_G_DATA_02005264(gMagmaRockInside_BlazeCasterScript, 24)

DEFINE_G_DATA_02005264(gMagmaRockInside_BlazeWatcherScript, 16)

DEFINE_G_DATA_020052AC(gMagmaRockInside_BlazeCasterReturnScript, 24, 0)

DEFINE_G_DATA_020052AC(gMagmaRockInside_BlazeWatcherReturnScript, 16, 3)

DEFINE_G_DATA_02001540(gMagmaRockInside_RubbleScript, 33)

DEFINE_G_DATA_02001540(gMagmaRockInside_DustScript, 44)

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(MagmaRockInside1),
    SCENE_EXIT(1, MagmaRockInside1, 4),
    SCENE_EXIT(2, MagmaRockInside2, 1),
    SCENE_EXIT(3, MagmaRockInside2, 5),
    SCENE_EXIT(4, MagmaRockInside1, 1),
    SCENE_EXIT(5, MagmaRockInside2, 2),
    SCENE_EXIT(6, MagmaRockInside2, 3),
    SCENE_EXIT(7, MagmaRockInside2, 4),
    SCENE_EXIT(8, MagmaRockInside3, 1),
    SCENE_EXITS(MagmaRockInside2),
    SCENE_EXIT(1, MagmaRockInside1, 2),
    SCENE_EXIT(2, MagmaRockInside1, 5),
    SCENE_EXIT(3, MagmaRockInside1, 6),
    SCENE_EXIT(4, MagmaRockInside1, 7),
    SCENE_EXIT(5, MagmaRockInside1, 3),
    SCENE_EXIT(6, MagmaRockInside2, 7),
    SCENE_EXIT(7, MagmaRockInside2, 6),
    SCENE_EXIT(8, MagmaRock3, 3),
    SCENE_EXITS(MagmaRockInside3),
    SCENE_EXIT(1, MagmaRockInside1, 8),
    SCENE_EXIT(2, MagmaRockInside3, 6),
    SCENE_EXIT(3, MagmaRockInside4, 1),
    SCENE_EXIT(4, MagmaRockInside4, 2),
    SCENE_EXIT(5, MagmaRockInside4, 3),
    SCENE_EXIT(6, MagmaRockInside3, 2),
    SCENE_EXIT(7, MagmaRockInside4, 4),
    SCENE_EXIT(8, MagmaRockInside3, 9),
    SCENE_EXIT(9, MagmaRockInside3, 8),
    SCENE_EXIT(10, MagmaRockInside4, 5),
    SCENE_EXIT(11, MagmaRockInside4, 6),
    SCENE_EXITS(MagmaRockInside4),
    SCENE_EXIT(1, MagmaRockInside3, 3),
    SCENE_EXIT(2, MagmaRockInside3, 4),
    SCENE_EXIT(3, MagmaRockInside3, 5),
    SCENE_EXIT(4, MagmaRockInside3, 7),
    SCENE_EXIT(5, MagmaRockInside3, 10),
    SCENE_EXIT(6, MagmaRockInside3, 11),
    SCENE_EXIT(7, MagmaRockInside6, 7),
    SCENE_EXIT(8, MagmaRockInside5, 7),
    SCENE_EXIT(9, MagmaRockInside5, 8),
    SCENE_EXIT(10, MagmaRockInside5, 1),
    SCENE_EXIT(11, MagmaRockInside6, 1),
    SCENE_EXIT(12, MagmaRockInside6, 2),
    SCENE_EXIT(13, MagmaRockInside6, 5),
    SCENE_EXIT(14, MagmaRockInside6, 6),
    SCENE_EXITS(MagmaRockInside5),
    SCENE_EXIT(1, MagmaRockInside4, 10),
    SCENE_EXIT(2, MagmaRockInside6, 3),
    SCENE_EXIT(3, MagmaRockInside5, 9),
    SCENE_EXIT(4, MagmaRockInside5, 10),
    SCENE_EXIT(5, MagmaRockInside6, 11),
    SCENE_EXIT(6, MagmaRockInside6, 4),
    SCENE_EXIT(7, MagmaRockInside4, 8),
    SCENE_EXIT(8, MagmaRockInside4, 9),
    SCENE_EXIT(9, MagmaRockInside5, 3),
    SCENE_EXIT(10, MagmaRockInside5, 4),
    SCENE_EXITS(MagmaRockInside6),
    SCENE_EXIT(1, MagmaRockInside4, 11),
    SCENE_EXIT(2, MagmaRockInside4, 12),
    SCENE_EXIT(3, MagmaRockInside5, 2),
    SCENE_EXIT(4, MagmaRockInside5, 6),
    SCENE_EXIT(5, MagmaRockInside4, 13),
    SCENE_EXIT(6, MagmaRockInside4, 14),
    SCENE_EXIT(7, MagmaRockInside4, 7),
    SCENE_EXIT(8, MagmaRockInside6, 14),
    SCENE_EXIT(9, MagmaRockInside6, 16),
    SCENE_EXIT(10, MagmaRockInside6, 17),
    SCENE_EXIT(11, MagmaRockInside5, 5),
    SCENE_EXIT(12, MagmaRockInside6, 18),
    SCENE_EXIT(13, MagmaRockInside6, 19),
    SCENE_EXIT(14, MagmaRockInside6, 8),
    SCENE_EXIT(15, MagmaRockInside7, 10),
    SCENE_EXIT(16, MagmaRockInside6, 9),
    SCENE_EXIT(17, MagmaRockInside6, 10),
    SCENE_EXIT(18, MagmaRockInside6, 12),
    SCENE_EXIT(19, MagmaRockInside6, 13),
    SCENE_EXITS(MagmaRockInside7),
    SCENE_EXIT(1, MagmaRockInside8, 1),
    SCENE_EXIT(2, MagmaRockInside7, 5),
    SCENE_EXIT(3, MagmaRockInside7, 6),
    SCENE_EXIT(4, MagmaRockInside7, 7),
    SCENE_EXIT(5, MagmaRockInside7, 2),
    SCENE_EXIT(6, MagmaRockInside7, 3),
    SCENE_EXIT(7, MagmaRockInside7, 4),
    SCENE_EXIT(8, MagmaRockInside7, 9),
    SCENE_EXIT(9, MagmaRockInside7, 8),
    SCENE_EXIT(10, MagmaRockInside6, 15),
    SCENE_EXIT(11, MagmaRock1, 2),
    SCENE_EXITS(MagmaRockInside8),
    SCENE_EXIT(1, MagmaRockInside7, 1),
    SCENE_EXITS_END,
};

struct ActorPlacement gMagmaRockInside_Placements[] = {
    {319, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRockInside_Room2Placements[] = {
    {377, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_WEST, 2, 2},
    {334, CONDITION_ALWAYS, 7, 2, 0, 0, FACING_NORTH, 2, 2},
    {334, CONDITION_ALWAYS, 7, 3, 0, 0, FACING_NORTH, 2, 3},
    {334, CONDITION_ALWAYS, 7, 4, 0, 0, FACING_NORTH, 2, 3},
    {334, CONDITION_ALWAYS, 7, 5, 0, 0, FACING_NORTH, 2, 3},
    {334, CONDITION_ALWAYS, 7, 6, 0, 0, FACING_NORTH, 2, 3},
    {334, CONDITION_ALWAYS, 7, 7, 0, 0, FACING_NORTH, 2, 3},
    {312, CONDITION_ALWAYS, 7, 8, 0, 0, FACING_NORTH, 2, 2},
    {489, CONDITION_ALWAYS, 7, 9, 0, 0, FACING_NORTH, 2, 2},
    {245, 0x6b, 1, PIXELS(296), 0, PIXELS(504), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRockInside_Room3Placements[] = {
    {377, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_WEST, 2, 2},
    {489, CONDITION_ALWAYS, 7, 2, 0, 0, FACING_NORTH, 2, 2},
    {319, CONDITION_ALWAYS, 7, 3, 0, 0, FACING_NORTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 4, 0, 0, FACING_NORTH, 2, 2},
    {319, CONDITION_ALWAYS, 7, 5, 0, 0, FACING_NORTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 6, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 7, 0, 0, FACING_SOUTH, 2, 3},
    {245, 0x68, 1, PIXELS(456), 0, PIXELS(600), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRockInside_Room4Placements[] = {
    {377, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_EAST, 2, 2},
    {404, 0x350, 1, 2, 0, 0, FACING_NORTH, 2, 2},
    {319, CONDITION_ALWAYS, 7, 3, 0, 0, FACING_NORTH, 2, 2},
    {404, 0x351, 1, 4, 0, 0, FACING_NORTH, 2, 2},
    {319, CONDITION_ALWAYS, 7, 5, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 7, 6, 0, 0, FACING_EAST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 7, 0, 0, FACING_WEST, 2, 2},
    {319, CONDITION_ALWAYS, 7, 8, 0, 0, FACING_NORTH, 2, 2},
    {319, CONDITION_ALWAYS, 7, 9, 0, 0, FACING_NORTH, 2, 3},
    {319, CONDITION_ALWAYS, 7, 10, 0, 0, FACING_NORTH, 2, 3},
    {319, CONDITION_ALWAYS, 7, 11, 0, 0, FACING_NORTH, 2, 3},
    {319, CONDITION_ALWAYS, 7, 12, 0, 0, FACING_NORTH, 2, 3},
    {319, CONDITION_ALWAYS, 7, 13, 0, 0, FACING_NORTH, 2, 3},
    {319, CONDITION_ALWAYS, 7, 14, 0, 0, FACING_NORTH, 2, 3},
    {319, CONDITION_ALWAYS, 7, 15, 0, 0, FACING_NORTH, 2, 3},
    {312, CONDITION_ALWAYS, 7, 16, 0, 0, FACING_NORTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 17, 0, 0, FACING_NORTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 18, 0, 0, FACING_NORTH, 2, 2},
    {334, CONDITION_ALWAYS, 7, 19, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRockInside_Room5Placements[] = {
    {489, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_NORTH, 2, 2},
    {489, CONDITION_ALWAYS, 7, 2, 0, 0, FACING_NORTH, 2, 2},
    {373, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(392), FACING_SOUTH, 2, 0},
    {373, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(392), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRockInside_Room6Placements[] = {
    {319, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_NORTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 2, 0, 0, FACING_NORTH, 2, 2},
    {319, CONDITION_ALWAYS, 7, 3, 0, 0, FACING_NORTH, 2, 2},
    {319, CONDITION_ALWAYS, 7, 4, 0, 0, FACING_NORTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 5, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRockInside_Room7Placements[] = {
    {312, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_NORTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 2, 0, 0, FACING_NORTH, 2, 3},
    {312, CONDITION_ALWAYS, 7, 3, 0, 0, FACING_NORTH, 2, 3},
    {312, CONDITION_ALWAYS, 7, 4, 0, 0, FACING_NORTH, 2, 2},
    {292, CONDITION_ALWAYS, 1, 5, 0, 0, FACING_NORTH, 2, 2},
    {325, CONDITION_ALWAYS, 1, 6, 0, 0, FACING_NORTH, 2, 2},
    {325, CONDITION_ALWAYS, 1, 7, 0, 0, FACING_NORTH, 2, 3},
    {325, CONDITION_ALWAYS, 1, 8, 0, 0, FACING_NORTH, 2, 3},
    {325, CONDITION_ALWAYS, 1, 9, 0, 0, FACING_NORTH, 2, 3},
    {364, CONDITION_ALWAYS, 1, 10, 0, 0, FACING_NORTH, 2, 2},
    {364, CONDITION_ALWAYS, 1, 11, 0, 0, FACING_NORTH, 2, 3},
    {364, CONDITION_ALWAYS, 1, 12, 0, 0, FACING_NORTH, 2, 3},
    {364, CONDITION_ALWAYS, 1, 13, 0, 0, FACING_NORTH, 2, 3},
    {365, CONDITION_ALWAYS, 1, 14, 0, 0, FACING_NORTH, 2, 2},
    {365, CONDITION_ALWAYS, 1, 15, 0, 0, FACING_NORTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 16, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRockInside_Room8Placements[] = {
    {323, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_SOUTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 2, 0, 0, FACING_NORTH, 2, 2},
    {365, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_NORTH, 2, 2},
    {365, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_NORTH, 2, 2},
    {5, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gMagmaRockInside_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, 8},
    {STEP_TRIGGER_WALKING, 70, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PushPillarIntoLava)},
    {STEP_TRIGGER_WALKING, 78, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PushPillarIntoLava)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRockInside_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(MagmaRockInside_FloodWithLava)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 17, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_MeetDjinni)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_UnblockMovedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15,
     0x9f0, EVENT(MagmaRockInside_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, 0x9f0,
     EVENT(MagmaRockInside_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_FireStatue)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRockInside_Room3Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(MagmaRockInside_FloodWithLava)},
    {STEP_TRIGGER_WALKING, 70, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PushPillarIntoLava)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(MagmaRockInside_FightFury)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_UnblockMovedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 11,
     0x9f1, EVENT(MagmaRockInside_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, 0x9f1,
     EVENT(MagmaRockInside_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_FireStatue)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRockInside_Room4Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {STEP_TRIGGER_WALKING, 70, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PushPillarIntoLava)},
    {STEP_TRIGGER_WALKING, 71, 0x1301,
     EVENT(MagmaRockInside_PushPillarIntoLava)},
    {STEP_TRIGGER_WALKING, 72, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PushPillarIntoLava)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 73, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PushPillarIntoLava)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 75, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PushPillarIntoLava)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_UnblockMovedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 23,
     0x9f2, EVENT(MagmaRockInside_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 23, 0x9f2,
     EVENT(MagmaRockInside_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 24,
     0x9f3, EVENT(MagmaRockInside_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 24, 0x9f3,
     EVENT(MagmaRockInside_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 25,
     0x9f4, EVENT(MagmaRockInside_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 25, 0x9f4,
     EVENT(MagmaRockInside_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 71, 0x350,
     EVENT(MagmaRockInside_BurstCrackedPillar1)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 72, 0x351,
     EVENT(MagmaRockInside_BurstCrackedPillar2)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50, 0x2f0,
     EVENT(MagmaRockInside_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_BurstCrackedPillar1)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_FireStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 51,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 51,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 51,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_FireStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 52,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 52,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 52,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_FireStatue)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRockInside_Room5Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(MagmaRockInside_FloodWithLava)},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(MagmaRockInside_FloodWithLava)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 11, CONDITION_ALWAYS, 0},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRockInside_Room6Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_EXIT, 18, CONDITION_ALWAYS, 18},
    {TRIGGER_EXIT, 19, CONDITION_ALWAYS, 19},
    {STEP_TRIGGER_WALKING, 70, 0x1302,
     EVENT(MagmaRockInside_PushPillarIntoLava)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_UnblockMovedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 9,
     0x9f5, EVENT(MagmaRockInside_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x9f5,
     EVENT(MagmaRockInside_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12,
     0x9f6, EVENT(MagmaRockInside_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0x9f6,
     EVENT(MagmaRockInside_BlockMovedPillar)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRockInside_Room7Triggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_OpenStairs)},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_STEP, 30, 0x9f8, EVENT(MagmaRockInside_ReleaseRocks)},
    {TRIGGER_EXAMINE, 31, 0x9f9, EVENT(MagmaRockInside_ExamineEmberStone)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_UnblockMovedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 23,
     0x9f7, EVENT(MagmaRockInside_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 23, 0x9f7,
     EVENT(MagmaRockInside_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 17,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_TakeFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 18,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_TakeFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 19,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_TakeFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 20,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_TakeFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 21,
     CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 22,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_LightTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 22,
     0x304, EVENT(MagmaRockInside_GrowFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 21,
     0x303, EVENT(MagmaRockInside_GrowFlame)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), 17, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PutOutFlames)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), 18, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PutOutFlames)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), 19, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PutOutFlames)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), 20, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PutOutFlames)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), 22, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PutOutFlames)},
    {TRIGGER_TABLE_END},
};

s32 gMagmaRockInside_LastStepCell = -1;

struct Trigger gMagmaRockInside_Room8Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_STEP, 80, CONDITION_ALWAYS, EVENT(MagmaRockInside_StampStepCell)},
    {TRIGGER_STEP, 81, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StampOtherStepCell)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), 30, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_ReadBlazeTablet)},
    {EXAMINE_TRIGGER_FACING(FACING_SOUTH), 30, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_ReadBlazeTabletFromWest)},
    {EXAMINE_TRIGGER_FACING(FACING_WEST), 30, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_ReadBlazeTabletFromEast)},
    {EXAMINE_TRIGGER_FACING(FACING_EAST), 30, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_ReadBlazeTabletFromSouth)},
    {TRIGGER_RAISED, 202, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StepOnMarker)},
    {TRIGGER_RAISED, 203, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StepOnMarker)},
    {TRIGGER_RAISED, 204, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StepOnMarker)},
    {TRIGGER_RAISED, 205, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StepOnMarker)},
    {TRIGGER_RAISED, 206, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StepOnMarker)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 10,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_LightTabletTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 11,
     CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 11,
     0x305, EVENT(MagmaRockInside_GrowTabletFlame)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), 10, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PutOutTabletFlame)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), 11, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PutOutTabletFlame)},
    {TRIGGER_TABLE_END},
};

#undef MagmaRockInside_BurstCrackedPillar1
#undef MagmaRockInside_BurstCrackedPillar2
#undef gMagmaRockInside_BlazeWatcherReturnScript
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY
#undef FIELD_AT
#undef gMagmaRockInside_Placements
#undef gMagmaRockInside_Room2Placements
#undef gMagmaRockInside_Room3Placements
#undef gMagmaRockInside_Room4Placements
#undef gMagmaRockInside_Room5Placements
#undef gMagmaRockInside_Room6Placements
#undef gMagmaRockInside_Room7Placements
#undef gMagmaRockInside_Room8Placements
#undef gMagmaRockInside_Room4Statues
#undef DEFINE_MAGMA_ROCK_INSIDE_BURST_PILLAR
#undef DEFINE_MAGMA_ROCK_INSIDE_LIGHT_TORCH
#undef BYTE_AT
#undef gMagmaRockInside_BlazeWatcherReturnScript
#undef EVENT_WORK
#undef DEFINE_MAGMA_ROCK_INSIDE_READ_TABLET_FROM
#undef gMagmaRockInside_Room2Triggers
#undef gMagmaRockInside_Room3Triggers
#undef gMagmaRockInside_Room4Triggers
#undef gMagmaRockInside_Room5Triggers
#undef gMagmaRockInside_Room6Triggers
#undef gMagmaRockInside_Room7Triggers
#undef gMagmaRockInside_Room8Triggers
