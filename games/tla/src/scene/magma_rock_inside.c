/// @file
/// Magma Rock's inside, the volcano's heart: the Moai that flood its rooms with
/// lava when Burst wakes them, the platforms that ride the lava and the gates
/// that drain it, the pillars pushed into it, Fury the Djinni, and deep inside
/// the tablet where Jenna learns Blaze and the flames Blaze moves.

#include "../field/actor_set_sprite_priority.c"
#include "sprite_ids.h"
#include "djinn_ids.h"

#include "../field/actor_prepare.c"

#include "types.h"
#include "item_ids.h"
#include "flags.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "field/scene_tables.h"
#include "system/iwram_call.h"
#include "scene_ids.h"
#include "system/frame.h"
#include "field/event_inline.h"
#include "field/map_grid.h"

extern struct MapCell gMapGridCellBuffer[];

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
#include "types.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "field/field_sequence_event.h"
#include "field/field.h"
#include "field/field_coordinate.h"
#include "field/field_bank.h"
#include "field/object.h"
#include "scene/mars_lighthouse_aerie.h"
#include "scene/scene.h"
#include "system/system_bank.h"
#include "scene/shared.h"
#include "system/scene.h"
#include "system/ram_buffer.h"
#include "field/map_grid.h"
#include "field/actor.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "system/io_reg.h"
#include "system/frame.h"
#include "system/field_effect.h"
#include "macros.h"
#include "field/field_sprite.h"
#include "field/call.h"
#include "edition.h"
#include "shared/trial_road.h"
#include "shared/tables.h"
#include "system/iwram_call.h"
#include "system/heap_release.h"
#include "window/item_icon.h"
#include "field/field_effect.h"
#include "field/particle.h"
#include "graphics/resource.h"
#include "shared/tundaria_tower.h"
#include "system/task.h"
#include "field/actor_script.h"
#include "fixed_math.h"

struct Actor *MagmaRockInside_CreateEffectActor(s32 x, s32 y, s32 z, s32 kind) {
  struct Actor *object = FieldActor_Create(kind, x, y, z);

  if (object != NULL) {
    Scene_SetActorSpritePriority(object, 1);
    object->motionFlags = 0;
    object->collisionFlags = ACTOR_NOT_TARGETED;
    Actor_SetSpriteFlags(object, 0);
    FieldActor_SetSpriteColor(object, 15);
    object->priorityFlags |= 0x22;
    return object;
  }
  return NULL;
}

#include "../field/field_effect_move.c"

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

#include "shared/position_distance.h"

#include "../field/actor_find_in_cell.c"

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

s32 MapCell_Get(u32 layer, s32 x, s32 y);

s32 MagmaRockInside_FindStatue(u16 *table);

s32 Position_FlatDistance(s32 *from, s32 *to);

void MagmaRockInside_TrailEmbers(struct Actor *object);
void MagmaRockInside_DrainLava(void);

void MagmaRockInside_DropRocks(s32 x, s32 y, s32 z, s32 count);

void MagmaRockInside_StartFallingRocks(void);

void MagmaRockInside_PressGateSwitch1(void);
void MagmaRockInside_PressGateSwitch2(void);
void MagmaRockInside_PressGateSwitch3(void);

extern s16 gMagmaRockInside_PlatformPathF[];
extern s16 gMagmaRockInside_PlatformPathG[];
extern s16 gMagmaRockInside_PlatformPathH[];
extern s16 gMagmaRockInside_PlatformPathI[];

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

void MagmaRockInside_DecompressTaggedLz(void *entry, void *destination);
void MagmaRockInside_InitRockSprite(struct RockSprite__s4b18 *sprite, s32 width,
                                    s32 height, u32 attributes, s32 tile);

void Particles_UpdateFalling(void);

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
void Actor_PlaySoundInView(struct Actor *object, s32 sound);
extern s32 gMagmaRockInside_StepCellWidth;
extern s32 gMagmaRockInside_StepCellHeight;
extern s32 gMagmaRockInside_StepCellX;
extern s32 gMagmaRockInside_StepCellZ;

extern s32 gPushLeaderSteps[];

#include "shared/block_push_onto.h"

extern struct MapCell gMapGridCellBuffer[];

/// Sets the collision code of each cell in a layer rectangle to value.
/// Layers 3 and above use the shared buffer.
s32 MagmaRockInside_SetCollisionRect(u32 layer, u32 x, u32 y, u32 width,
                                     u32 height, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapCell *cells;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cells = map->layers[layer].cells;
  else
    cells = gMapGridCellBuffer;
  cells += x + (y << 7);
  for (x = 0; x < height; x++) {
    struct MapCell *cell = cells + (x << 7);

    for (y = 0; y < width; y++, cell++)
      cell->collisionCode = value;
  }
  return 0;
}

void MagmaRockInside_FightFury(void) { Field_RequestDjinnBattle(15, 56); }

void MagmaRockInside_MeetDjinni(void) {
  Djinn_JoinParty(17, DJINNI_FIRE19 / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_FIRE19));
}

#include "shared/position_flat_distance.h"

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
s32 MapCollision_FindOrAddTag(s32 layer, s32 shape, s32 height);

#include "shared/map_cell_set_shape.h"

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

#include "shared/map_cell_get.h"

s32 MagmaRockInside_SwapCells(u32 layer, s32 x, s32 y, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapCell *cell;
  s32 old;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapGridCellBuffer;
  cell += x + (y << 7);
  old = (cell->collisionCode << 8) | cell->attributeB;
  cell->collisionCode = value >> 8;
  cell->attributeB = value;
  return old;
}

/// For each id of a list ended by 0xffff: clears the object's mark and, on a
/// cell with attribute bit 6, blocks the cells ahead of and behind it and sets
/// the mark.
void MagmaRockInside_MarkBlockCells(u16 *ids) {
  struct Actor *object;
  u8 *blocked;
  struct MapWork *map;
  struct MapCell **cells;
  struct MapCell *cell;
  s32 x, z;

  while (*ids != 0xffff) {
    object = Actor_Get(*ids);
    blocked = &object->riseEnabled;
    *blocked = 0;
    if (MapCell_Get(0, FIELD_CELL(object->x.fixed),
                    FIELD_CELL(object->z.fixed)) &
        0x40) {
      map = Ram_HeapSlots->mapGrid;
      if (map == NULL)
        return;
      cells = &map->layers[2].cells;
      z = (object->z.fixed - object->y.fixed) >> 20;
      x = FIELD_CELL(object->x.fixed);
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
#include "shared/block_clear_cells.h"

void MagmaRockInside_PausePlatform(struct Actor *object) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  u8 *state = &object->paused;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0 || work->transitionActive == 0)
    *state = 1;
  else
    *state = 0;
}

void Actor_SelectAnimation();
void Actor_StartScript();
s32 GameFlag_Get();

void MagmaRockInside_StartLavaPlatform(s32 id, s32 path, s16 flag) {
  struct Actor *platform;
  s32 risen;

  platform = Actor_GetById(id);
  Actor_SelectAnimation(platform, 4);
  platform->steeringFlags = 0;
  platform->speedLimit = px(1) / 2;
  platform->acceleration = px(1) / 2;
  platform->motionFlags = ACTOR_FALLS;
  platform->priorityFlags |= 0x80;
  Actor_SetSpriteFlags(platform, 0);
  if (Event_GetFlag((u16)flag)) {
    platform->groundHeight = px(16);
    platform->y.fixed = px(16);
    if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
      Actor_StartScript(platform, path);
  } else {
    platform->groundHeight = -px(14);
    platform->y.fixed = -px(14);
  }
  platform->update = (void (*)(void))MagmaRockInside_PausePlatform;
}

s32 MagmaRockInside_SetCellShape(s32 terrain, s32 x, s32 z, s32 height);

#include "shared/actors_raise_cells.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

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

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene != SCENE_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST) {
    if (scene == SCENE_MAGMA_ROCK_INSIDE_2)
      return gMagmaRockInside_Room2Placements;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_FURY)
      return gMagmaRockInside_Room3Placements;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_4)
      return gMagmaRockInside_Room4Placements;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST)
      return gMagmaRockInside_Room5Placements;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_6)
      return gMagmaRockInside_Room6Placements;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE)
      return gMagmaRockInside_Room7Placements;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_BLAZE_TABLET)
      return gMagmaRockInside_Room8Placements;
  }
  return gMagmaRockInside_Placements;
}

void MagmaRockInside_UnblockMovedPillar(void) {
  struct FieldWork *work;
  s16 scene;

  work = Ram_HeapSlots->fieldWork;
  scene = gMainRoutineWork.next;
  if (scene == SCENE_MAGMA_ROCK_INSIDE_2)
    Actor_LiftTargetBlock(gMagmaRockInside_Room2Pillars);
  else if (scene == SCENE_MAGMA_ROCK_INSIDE_FURY)
    Actor_LiftTargetBlock(gMagmaRockInside_Room3Pillars);
  else if (scene == SCENE_MAGMA_ROCK_INSIDE_4)
    Actor_LiftTargetBlock(gMagmaRockInside_Room4Pillars);
  else if (scene == SCENE_MAGMA_ROCK_INSIDE_6)
    Actor_LiftTargetBlock(gMagmaRockInside_Room6Pillars);
  else if (scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE)
    Actor_LiftTargetBlock(gMagmaRockInside_Room7Pillars);
  MagmaRockInside_ClearBlockCells(work->movedBlock);
}

void MagmaRockInside_BlockMovedPillar(void) {
  struct Actor *object = gFieldWork->movedBlock;
  /* FAKEMATCH(pin): copy in r5. */
  register struct Actor *copy asm("r5");
  s32 scene;
  s32 x;
  s32 z;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_PlaceMovedBlock();
  scene = gMainRoutineWork.next;
  if (scene == SCENE_MAGMA_ROCK_INSIDE_6) {
    MagmaRockInside_MarkBlockCells(gMagmaRockInside_Room6Pillars);
    if (Map_GetTerrainHeight(2, object->x.fixed, object->z.fixed) !=
        object->y.fixed) {
      object->layer = 2;
      object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      System_Wait(1);
      if (Event_GetFlag(FLAG_AREA_TEMP_02)) {
        object->gravity = px(1) / 16;
        object->priorityFlags |= 2;
        REG_BLDALPHA = 0x1000;
        REG_BLDCNT = 0x3f10;
        copy =
            OverlayObject_PrepareObject(object->x.fixed, object->groundHeight,
                                        object->z.fixed + px(16), 312);
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
      object->layer = 0;
    }
  } else if (scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE) {
    MagmaRockInside_MarkBlockCells(gMagmaRockInside_Room7Pillars);
    if (Map_GetTerrainHeight(2, object->x.fixed, object->z.fixed) !=
        object->y.fixed) {
      object->layer = 2;
      object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      System_Wait(1);
      object->gravity = px(1) / 16;
      object->priorityFlags |= 2;
      REG_BLDALPHA = 0x1000;
      REG_BLDCNT = 0x3f10;
      copy = OverlayObject_PrepareObject(object->x.fixed, object->groundHeight,
                                         object->z.fixed + px(16), 312);
      MagmaRockInside_WaitForLanding(object);
      Event_PlaySound(SE_HEAVY_STOP);
      Actor_Release(copy);
      Actor_SelectAnimation(object, 4);
      System_Wait(1);
      REG_BLDCNT = 0;
      MagmaRockInside_WaitForLanding(object);
      object->motionFlags = 0;
      object->layer = 0;
    }
  }
  x = FIELD_CELL(object->x.fixed);
  z = FIELD_CELL(object->z.fixed);
  scene = gMainRoutineWork.next;
  if (scene == SCENE_MAGMA_ROCK_INSIDE_2) {
    if (z == 16)
      Event_SetFlag(2544);
  } else if (scene == SCENE_MAGMA_ROCK_INSIDE_FURY) {
    if (z == 41)
      Event_SetFlag(2545);
  } else if (scene == SCENE_MAGMA_ROCK_INSIDE_4) {
    if (x == 21) {
      if (z == 43)
        Event_SetFlag(2546);
      if (z == 45)
        Event_SetFlag(2547);
      if (z == 47)
        Event_SetFlag(2548);
    }
  } else if (scene == SCENE_MAGMA_ROCK_INSIDE_6) {
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
  } else if (scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE) {
    if (x == 43)
      Event_SetFlag(2551);
  }
  Event_End();
}

s32 Import_BattleEffectStopMainActor(void);

void MagmaRockInside_PlayBurst(s16 value) {
  struct MainRoutineWork *party;

  FieldPsynergy_LoadResources(ABILITY_BURST, 1);
  party = &gMainRoutineWork;
  Import_FieldPsynergySetupActorPair(party->leaderActorId, value);
  FieldPsynergy_RunHandler(1);
  Import_BattleEffectStopMainActor();
  FieldPsynergy_End();
}

extern u32 gMagmaRockInside_EmberScript[];

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

/// An object's update: every fourth frame it spawns an effect of type 403 with
/// its own script at a random point within 4 pixels of the object along z,
/// moving along z at a random speed; every sixteenth frame it also plays sound
/// 136 at the object.
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
    Actor_PlaySoundInView(object, 136);
  dz = Random16() & 15;
  vz = ((Random16() & 31) - 16) << 12;
  /* FAKEMATCH(temporary): z read into its own local. */
  z = object->z.fixed;
  FieldEffect_Spawn(object->x.fixed, object->y.fixed, z + ((dz - 8) << 15), 0,
                    0, vz, 0x320001, &options);
}

/// Flashes an actor in colour 10 on the frames its mask picks.
void MagmaRockInside_FlashStatue(struct Actor *object) {
  if (gFrameCount & object->riseEnabled)
    FieldActor_SetSpriteColor(object, 10);
  else
    FieldActor_SetSpriteColor(object, 0);
}

// The cell watch's table lookup over (id, cell, value) rows ended by 0xffff:
// for the row of the cell under the watch, sets the watch's object to the row's
// object and returns its value; otherwise returns the value of the last row
// passed (0 for none).

s32 MagmaRockInside_FindStatue(u16 *table) {
  struct CellWatch *watch = Ram_HeapSlots->fieldEffectWork;
  s32 result = 0;
  s32 cell = MapCell_Get(0, watch->x >> 20, watch->z >> 20) >> 8;
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

/// The cell watch's callback: in modes 2 and 3 it finds the object on the
/// leader's cell in the room's table (room 4 has its own) and sets it blinking
/// (MagmaRockInside_FlashStatue) by frame-count bit 2 in mode 2 or bit 1 in
/// mode 3.
void MagmaRockInside_TargetStatue(s32 mode) {
  u16 *table;
  struct CellWatch *watch = Ram_HeapSlots->fieldEffectWork;
  s32 scene;

  table = gMagmaRockInside_Statues;
  FieldEvent_ResolvePendingActions(0);
  scene = gMainRoutineWork.next;
  if (scene == SCENE_MAGMA_ROCK_INSIDE_2) {
  } else if (scene == SCENE_MAGMA_ROCK_INSIDE_FURY) {
  } else if (scene == SCENE_MAGMA_ROCK_INSIDE_4) {
    table = gMagmaRockInside_Room4Statues;
  }
  if (mode == 2) {
    struct Actor *object;

    MagmaRockInside_FindStatue(table);
    object = watch->object;
    object->riseEnabled = 4;
    object->update = (void (*)(void))MagmaRockInside_FlashStatue;
  }
  if (mode == 3) {
    struct Actor *object;

    MagmaRockInside_FindStatue(table);
    object = watch->object;
    object->riseEnabled = 2;
    object->update = (void (*)(void))MagmaRockInside_FlashStatue;
  }
}

void MagmaRockInside_DriftAndSlow(struct Actor *object) {
  s32 dx, dy, dz;

  object->x.fixed += dx = object->bounce;
  object->y.fixed += dy = object->gravity;
  object->z.fixed += dz = object->riseBaseY;
  dx -= dx >> 3;
  dy -= dy >> 3;
  dz -= dz >> 3;
  object->bounce = dx;
  object->gravity = dy;
  object->riseBaseY = dz;
}

extern u32 gMagmaRockInside_EmberScript[];

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);
/* VIEW(s0e20): routine MagmaRockInside_PlayBurst as void
 * MagmaRockInside_PlayBurst(u16 id); the definition has void
 * MagmaRockInside_PlayBurst(s16 value); tried as the definition's void
 * MagmaRockInside_PlayBurst(s16 value), which the object already declares,
 * MagmaRockInside_FireFromStatue moves: 1 of 270 instructions in tla-ja
 * (magma_rock_inside.c). */
extern void
MagmaRockInside_PlayBurst__s0e20(u16 id) __asm__("MagmaRockInside_PlayBurst");

static __inline__ struct Actor *Import_ActorCreate_(s32 kind, s32 x, s32 y,
                                                    s32 z) {
  return (struct Actor *)Import_ActorCreate(kind, x, y, z);
}

void MagmaRockInside_FireFromStatue(struct Actor *source) {
  struct Actor *flame = Import_ActorCreate_(403, source->x.fixed,
                                            source->y.fixed, source->z.fixed);
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
  position[0] = flame->x.fixed;
  position[1] = flame->y.fixed;
  position[2] = flame->z.fixed;
next:
  position[0] += step;
  cell =
      MapCell_Get(2, position[0] >> 20, (position[2] - position[1]) >> 20) >> 8;
  if (cell == 212 || cell == 222)
    goto end;
  id = 8;
inner:
  other = gFieldWork->actors[id];
  if (other->script == 0)
    goto skip;
  if (FIELD_CELL(source->x.fixed) == FIELD_CELL(other->x.fixed) &&
      FIELD_CELL(source->y.fixed) == FIELD_CELL(other->y.fixed) &&
      FIELD_CELL(source->z.fixed) == FIELD_CELL(other->z.fixed))
    goto skip;
  if (Position_FlatDistance(&other->x.fixed, position) > 8)
    goto skip;
  if (FIELD_CELL(other->y.fixed) != position[1] >> 20)
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
  flame->acceleration = px(2);
  flame->speedLimit = px(2);
  flame->update = (void (*)(void))MagmaRockInside_TrailEmbers;
  FieldActor_SetMoveTargetFixed(flame, position[0], position[1], position[2]);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveToFixed(position[0], position[1], position[2], 1);
  FieldActor_WaitForMove(flame);
  flame->update = NULL;
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
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  System_Wait(10);
  Event_End();
  return;
hit:
  flame->acceleration = px(2);
  flame->speedLimit = px(2);
  flame->update = (void (*)(void))MagmaRockInside_TrailEmbers;
  FieldActor_SetMoveTargetFixed(flame, position[0], position[1], position[2]);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveToFixed(position[0], position[1], position[2], 1);
  FieldActor_WaitForMove(flame);
  flame->update = NULL;
  Actor_Release(flame);
  Event_SetFlag(FLAG_SCENE_TEMP_F0);
  MagmaRockInside_PlayBurst__s0e20((u16)id);
  Event_ClearFlag(FLAG_SCENE_TEMP_F0);
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  System_Wait(10);
  Event_End();
}

void MagmaRockInside_EndFallingRocks(void);

void MagmaRockInside_FireStatue(void) {
  struct MagmaRockInsideWork *work = Ram_HeapSlots->fieldEffectWork;
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
  struct MagmaRockInsideWork *work = Ram_HeapSlots->fieldEffectWork;

  if (cue == 0)
    MagmaRockInside_StartFallingRocks();
  if (mode == 2) {
    struct Actor *object = work->target;

    MagmaRockInside_DropRocks(object->x.fixed, object->y.fixed + px(15),
                              object->z.fixed, 1);
  }
  if (mode == 3) {
    struct Actor *object = work->target;
    struct Actor *target;

    MagmaRockInside_DropRocks(object->x.fixed, object->y.fixed + px(15),
                              object->z.fixed, 8);
    target = work->target;
    SET_ACTOR_POSITION(target, 0, 0, 0);
  }
  if (cue == 0x3e7)
    MagmaRockInside_EndFallingRocks();
  if (cue == 178)
    Actor_SelectAnimation(work->target, 3);
}

void MagmaRockInside_PushPillarIntoLava(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *target;
  s32 position[3];
  s32 stepX;
  s32 stepZ;
  s32 i;
  s32 angle = (leader->facing + 0x2000) & 0xc000;

  position[0] = (leader->x.fixed & 0xfff00000) + 0x80000;
  position[1] = leader->y.fixed - px(16);
  position[2] = (leader->z.fixed & 0xfff00000) + 0x80000;
  FieldVector_AddPolarOffset(32, angle, position);
  target = Actor_FindInCell(position, leader);
  if (target == 0)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SelectAnimation(leader, 6);
  System_Wait(6);
  Event_PlaySound(SE_JUMP);
  Actor_SelectAnimation(leader, 7);
  leader->speedLimit = px(3);
  leader->acceleration = px(2);
  leader->velocityY = 0x4cccc;
  leader->motionFlags &= ~(ACTOR_FOLLOWS_TERRAIN | ACTOR_MOTION_SPARE);
  Actor_SetSpriteFlags(leader, 0);
  stepX = (position[0] - leader->x.fixed) / 12;
  stepZ = (position[2] - leader->z.fixed) / 12;
  i = 0;
  do {
    leader->x.fixed += target->x.fixed - position[0] + stepX;
    leader->z.fixed += target->z.fixed - position[2] + stepZ;
    position[0] = target->x.fixed;
    position[2] = target->z.fixed;
    System_Wait(1);
    i++;
  } while (i != 12);
  leader->x.fixed = target->x.fixed;
  leader->y.fixed = leader->groundHeight = target->y.fixed + px(16);
  leader->z.fixed = target->z.fixed;
  Actor_SelectAnimation(leader, 6);
  Actor_SetSpriteFlags(leader, 1);
  leader->motionFlags |= ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Event_End();
}

extern u32 gMagmaRockInside_FlameEmberScript[];

void MagmaRockInside_DrainLava(void) {
  s32 srcX;
  s32 srcZ;
  s32 baseX = 0;
  struct MapLayer *layer = &Ram_HeapSlots->mapGrid->layers[1];
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
  scene = gMainRoutineWork.next;
  roomPosition = position;
  if (scene == SCENE_MAGMA_ROCK_INSIDE_2) {
    baseX = 164 << 17;
    roomPosition[1] = 0x200000;
    roomPosition[2] = 200 << 16;
    srcX = 20;
    srcZ = 69;
    if (Event_GetFlag(FLAG_AREA_TEMP_00))
      t = 212;
  } else if (scene == SCENE_MAGMA_ROCK_INSIDE_FURY) {
    baseX = 232 << 16;
    roomPosition[1] = 0x200000;
    roomPosition[2] = 248 << 16;
    srcX = 14;
    srcZ = 72;
    if (Event_GetFlag(FLAG_AREA_TEMP_01))
      t = 212;
  } else if (scene == SCENE_MAGMA_ROCK_INSIDE_4) {
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
                      burstPosition[2] + (row << 18) + px(8), 0, px(1), 0,
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
      sparkRandom = Random16();
      sparkPosition = position;
      sparkPosition[0] = baseX + ((sparkRandom % 48) << 16);
      FieldEffect_Spawn(sparkPosition[0], sparkPosition[1],
                        sparkPosition[2] + px(48), 0, px(1), 0, 0x330000,
                        effectOptions);
      i++;
      if (i != 4)
        goto spark;
    }
    if (t == 0) {
      layer->offsetY -= 0x200000;
      MapGrid_CenterOnFollowedPoint();
      Event_Wait(1);
    } else if (t <= 63) {
      REG_BLDALPHA = ((16 - (t >> 2)) << 8) | (t >> 2);
      REG_BLDCNT = 0x3f44;
    } else if (t == 64) {
      REG_BLDCNT = 0;
    } else if (t <= 127) {
      layer->offsetY += 0x8000;
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
  Event_PlaySound(SE_STOP_ACTION_SE);
  Event_PlaySound(SE_WHOOSH);
  lastOptions = &options;
  lastOptions->startScaleY = 0x1cccc;
  lastOptions->startScaleX = 0x1cccc;
  i = 0;
last:
  sparkRandom = Random16();
  sparkPosition = position;
  sparkPosition[0] = baseX + ((sparkRandom % 48) << 16);
  i++;
  FieldEffect_Spawn(sparkPosition[0], sparkPosition[1],
                    sparkPosition[2] + px(48), 0, 0, 0, 0x3b0000, lastOptions);
  if (i != 6)
    goto last;
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_CopyCellsTo(68, 64, srcX, srcZ, 3, 6);
  FieldMapGrid_WaitForShakeToSettle();
  Event_Wait(30);
  work = &gMainRoutineWork;
  layer->offsetY = 0;
  closing = work->next;
  if (closing == SCENE_MAGMA_ROCK_INSIDE_2)
    MagmaRockInside_PressGateSwitch1();
  else if (closing == SCENE_MAGMA_ROCK_INSIDE_FURY)
    MagmaRockInside_PressGateSwitch2();
  else if (closing == SCENE_MAGMA_ROCK_INSIDE_4)
    MagmaRockInside_PressGateSwitch3();
}

/// A Magma Rock room's opening event: the leader is centred on its cell, the
/// screen shakes, and by room (2, 3 or 5, the last by entrance) it picks the
/// cells to open, the flag that was set (768 to 770, cleared here) and where
/// the effects play. The cells are copied open; if that flag was set, ten rows
/// of six effects rise along the opening while it widens and the blend fades
/// over 64 steps. Then the cells close again and the leader steps back a cell.
void MagmaRockInside_FloodWithLava(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
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
  ((s32 *)j)[0] = (leader->x.fixed & 0xfff00000) + 0x80000;
  ((s32 *)j)[2] = (leader->z.fixed & 0xfff00000) + 0x80000;
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, ((s32 *)j)[0] >> 16,
                      ((s32 *)j)[2] >> 16);
  FieldEvent_Wait(20);
  Event_PlaySound(SE_BUTTON_PRESS);
  flags = &leader->priorityFlags;
  *flags &= ~64;
  FieldEvent_Wait(20);
  MapGrid_SetShake(1, 2, 1);
  Event_PlaySound(SE_TREMOR);
  FieldEvent_Wait(30);
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_2) {
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
    facing = FACING_EAST;
    ((s32 (*)())MagmaRockInside_SwapCells)(0, 15, 13, 64);
    FieldActor_SetSpritePriority(15, 3);
    for (j = 0; j != 7; j++)
      Actor_SetAnimation(j + 9, 1);
  } else if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_FURY) {
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
  } else if (gMainRoutineWork.next ==
             SCENE_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST) {
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
        FieldEffect_Spawn(p[0], p[1], p[2] + (i << 18) + px(23), 0, px(1), 0,
                          0x330000, &options);
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
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_2)
    Actor_Get(15)->priorityFlags |= 1;
  Event_PlaySound(SE_STOP_ACTION_SE);
  Event_PlaySound(SE_BOOM);
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_CopyCellsTo(72, 63, srcX, rowAbove, 3, 4);
  FieldMapGrid_WaitForShakeToSettle();
  Event_PlaySound(SE_BUTTON_PRESS);
  {
    s32 bit = 64;
    bit |= *flags;
    *flags = bit;
  }
  FieldEvent_Wait(20);
  j = (u32)&gMainRoutineWork.leaderActorId;
  Actor_Get(*(s32 *)j)->steeringFlags &= ~1;
  /* FAKEMATCH(temporary): the position's address held in a pointer. */
  p = position;
  p[0] = (leader->x.fixed & 0xfff00000) + 0x80000;
  p[1] = leader->y.fixed;
  p[2] = (leader->z.fixed & 0xfff00000) + 0x80000;
  FieldVector_AddPolarOffset(16, (s32)facing ^ 0x8000, p);
  leader->facing = facing;
  Actor_WalkToAndWait(*(s32 *)j, ((s16 *)p)[1], ((s16 *)p)[5]);
  Actor_Get(*(s32 *)j)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_End();
}

#define DEFINE_MAGMA_ROCK_INSIDE_BURST_PILLAR(name, n1, n2)                    \
  void name(s32 a, s32 b) {                                                    \
    struct Actor **work = Ram_HeapSlots->fieldEffectWork;                      \
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
    object->update = NULL;
  if ((object->timer & 1) == 0) {
    struct EffectConfig config;

    config.id = 403;
    config.kind = 2;
    config.variant = 10;
    config.data = gMagmaRockInside_EmberScript;
    FieldEffect_Spawn(object->x.fixed, object->y.fixed, object->z.fixed, 0,
                      0x1000, 0, 0x330000, &config);
  }
}

extern u32 gMagmaRockInside_FlameEmberScript[];

static __inline__ void Arrive(s32 id, s32 x, s32 z) {
  Actor_SetPosition(id, 776, 248);
  Actor_SetDestination(id, x, z);
  Actor_GetById(id)->velocityY = px(12);
  Actor_GetById(id)->gravity = px(1) / 2;
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
  FieldEffect_Spawn(position[0], position[1], position[2], 0, px(1), 0,
                    0x3b0000, &options);
  if ((i & 7) == 0)
    Event_PlaySound(SE_POUND);
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
    Actor_GetById(12)->velocityY = px(13);
    Actor_GetById(12)->gravity = px(1) / 2;
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
  Event_PlaySound(SE_STOP_ACTION_SE);
  Event_Wait(40);
  Event_SetFlag(2552);
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  wait(10);
  Event_End();
}

void MagmaRockInside_TakeFlame(void) {
  u8 *work = (u8 *)Ram_HeapSlots->fieldWork;

  *(work + 0xd38) = 1;
  if (!Event_GetFlag(FLAG_AREA_TEMP_04)) {
    FieldActor_SetSpriteColor(Actor_Get(22), 15);
    Actor_SetPosition(22, 632, 312);
    Actor_Get(22)->scaleX = Actor_Get(22)->scaleY = Q16_ONE / 16;
  }
}

#define DEFINE_MAGMA_ROCK_INSIDE_LIGHT_TORCH(name, n1, n2, n3, n4)             \
  void name(void) {                                                            \
    if (!Event_GetFlag(n1)) {                                                  \
      FieldActor_SetSpriteColor(Actor_Get(n2), 15);                            \
      Actor_SetPosition(n2, n3, n4);                                           \
      Actor_Get(n2)->scaleX = Actor_Get(n2)->scaleY = Q16_ONE / 16;            \
    }                                                                          \
  }

DEFINE_MAGMA_ROCK_INSIDE_LIGHT_TORCH(MagmaRockInside_LightTorch, 0x303, 21, 600,
                                     312)

struct Actor;

void MagmaRockInside_GrowFlame(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *object;
  struct Actor *actor;
  struct Actor *other;
  s32 i;

  object = Actor_GetById(work->unkd30);
  FieldActor_SetSpriteColor(object, 0);
  Event_PlaySound(SE_WHOOSH);
  i = 0;
  do {
    object->scaleY = object->scaleX += Q16_ONE / 16;
    wait(5);
    i++;
  } while (i != 16);
  Event_SetFlag(work->unkd30 + 0x2ee);
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
          other->x.fixed, other->groundHeight, other->z.fixed + px(48), 312);

      other->motionFlags = 0;
      actor = prepared;
    }
    i = 0;
    do {
      other->y.fixed -= px(1);
      FieldEvent_Wait(5);
      i++;
    } while (i != 32);
    Event_PlaySound(SE_FLAME_GROW);
    Actor_SelectAnimation(other, 4);
    Actor_Release(actor);
    FieldEvent_Wait(1);
    REG_BLDCNT = 0;
    MagmaRockInside_SwapCells(0, 38, 18, 0);
    Event_End();
  }
}

void MagmaRockInside_PutOutFlames(void) {
  if (gFieldWork->unkd30 == -1) {
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
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId,
                                            0) == 0) {
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
  Actor_Get(gMainRoutineWork.leaderActorId)->motionFlags = 0;
  Actor_Get(gMainRoutineWork.leaderActorId)->speedLimit =
      Actor_Get(gMainRoutineWork.leaderActorId)->acceleration = px(1) / 2;
  Event_PlaySound(SE_MAP_EXIT);
  Actor_WalkTo(gMainRoutineWork.leaderActorId, 248, 328);
  FieldEvent_Wait(5);
  FieldEvent_SetExitRequest(1);
  Event_End();
}

void MagmaRockInside_TrackStepCell(void) {
  struct Actor *object = Actor_Get(FieldParty_GetLeaderActor());
  s32 x = FIELD_CELL(object->x.fixed);
  s32 z = FIELD_CELL(object->z.fixed);
  s32 key = (x << 16) + z;
  s32 last;
  s32 value;
  s32 trigger;
  struct FieldWork *work;

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
  if (gMainRoutineWork.next != SCENE_MAGMA_ROCK_INSIDE_BLAZE_TABLET)
    return;
  work = Ram_HeapSlots->fieldWork;
  value = MapCell_Get(2, x, z) >> 8;
  if (value != 0) {
    trigger = value + 200;
    work->raisedTrigger = trigger;
  }
}

void MagmaRockInside_StartStepCells(void) {
  s32 none = -1;
  s32 priority;

  gMagmaRockInside_LastStepCell = none;
  priority = 0x480;
  FieldTask_AddFrame(MagmaRockInside_TrackStepCell, priority);
}

/// Saves the 1 by 2 cells 64 columns across from the controlled object at
/// (73, 72) and stamps the cells kept at (70, 72) in their place, once per
/// cell; AirsRockInside_StampWideCellUnderLeader is the same with another
/// source.
void MagmaRockInside_StampStepCell(void) {
  struct Actor *object = Actor_Get(FieldParty_GetLeaderActor());
  s32 x = FIELD_CELL(object->x.fixed);
  s32 y = FIELD_CELL(object->y.fixed);
  s32 z = FIELD_CELL(object->z.fixed);
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

/// As MagmaRockInside_StampStepCell, stamping the cells kept at (71, 72).
void MagmaRockInside_StampOtherStepCell(void) {
  struct Actor *object = Actor_Get(FieldParty_GetLeaderActor());
  s32 x = FIELD_CELL(object->x.fixed);
  s32 y = FIELD_CELL(object->y.fixed);
  s32 z = FIELD_CELL(object->z.fixed);
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

/// Reads what the leader stands on (MapCell_Get, kind 2): on cells
/// 1 and 7 it only redraws the marker when flag 0x200 is set; elsewhere it
/// clears (MagmaRockInside_StampStepCell), redraws the marker in the row the
/// event work's counter gives, sets flag 0x200 and the counter's flag (312 on).
void MagmaRockInside_StepOnMarker(void) {
  u8 *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader;
  s32 cell, row = 54;

  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  cell =
      MapCell_Get(2, FIELD_CELL(leader->x.fixed), FIELD_CELL(leader->z.fixed));
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

  object = Actor_Get(gFieldWork->unkd30);
  FieldActor_SetSpriteColor(object, 0);
  Event_PlaySound(SE_WHOOSH);
  i = 0;
  do {
    object->scaleY = object->scaleX += Q16_ONE / 16;
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
        other->x.fixed, other->groundHeight, other->z.fixed + px(48), 312);

    other->motionFlags = 0;
    actor = prepared;
  }
  i = 0;
  do {
    other->y.fixed -= px(1);
    FieldEvent_Wait(5);
    i++;
  } while (i != 32);
  Event_PlaySound(SE_FLAME_GROW);
  Actor_SelectAnimation(other, 4);
  Actor_Release(actor);
  FieldEvent_Wait(1);
  REG_BLDCNT = 0;
  MagmaRockInside_SwapCells(0, 7, 21, 128);
  Event_End();
}

void MagmaRockInside_PutOutTabletFlame(void) {
  if (gFieldWork->unkd30 == -1) {
    if (!Event_GetFlag(FLAG_AREA_TEMP_05))
      Actor_SetPosition(11, 0, 0);
  }
}

void MagmaRockInside_SpawnSpark(struct Actor *source) {
  struct Actor *object =
      Create(0x2a0, source->x.fixed, source->y.fixed, source->z.fixed);

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
 * Orbiter__magma_rock_inside; scene/scene.h has another definition; tried with
 * the definition the rest of the object has, it does not compile in tla-ja
 * (magma_rock_inside.c): games/tla/src/scene/magma_rock_inside.c:1757:
 * structure has no member named `parent'. */
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
  object->y = parent->y + px(8);
  object->z = parent->z;
  heading = *angle;
  FieldVector_AddPolarOffsetFixed(object->radius, heading,
                                  position = &object->x);
  if (--object->timer == 0)
    return 0;
  if (object->radius == 0x20000)
    return 1;
  MagmaRockInside_SpawnSpark(object);
  object->radius -= 0x40000;
  object->angle += (s16)*speed >> 1;
  *position = parent->x;
  object->y = parent->y + px(8);
  object->z = parent->z;
  FieldVector_AddPolarOffsetFixed(object->radius, (s16)*angle, position);
  MagmaRockInside_SpawnSpark(object);
  object->radius -= 0x40000;
  return 1;
}

// Moves around in a circle.
#include "shared/orbit_update.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

#include "shared/actor_aim_at_watched.h"

s32 MagmaRockInside_DriftForSteps(struct Actor *object) {
  u8 *steps;
  s32 v;
  u8 n;

  object->x.fixed += object->bounce;
  object->y.fixed += object->riseBaseY;
  object->z.fixed += object->gravity;
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
  position[0] = object->x.fixed + (((Random16() & 31) - 16) << 16);
  position[1] = object->y.fixed + ((Random16() & 31) << 16) + 0x80000;
  position[2] = object->z.fixed;
  FieldEffect_Spawn(position[0], position[1], position[2] + px(4), 0, px(1) / 2,
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
 * *gFieldEffect_DurationScripts[] = {; tried as the definition's extern s32
 * gMagmaRockInside_BlazeWatcherReturnScript[], it does not compile in tla-ja
 * (magma_rock_inside.c): games/tla/src/scene/magma_rock_inside.c:1993:
 * `gMagmaRockInside_BlazeWatcherReturnScript' undeclared (first use in this
 * function). */
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
  FieldCamera_MoveToFixed(first->x.part[0].pixel << 16,
                          first->y.part[0].pixel << 16,
                          first->z.part[0].pixel << 16, 1);
  Event_PlaySound(SE_RUMBLE);
  MapGrid_SetShake(2, 2, 1);
  PaletteFade_SetSource(0x10000, 0);
  PaletteFade_SetTarget(0x10003, 1);
  PaletteFade_StartInterpolation(60);
  first->motionFlags = 0;
  first->acceleration = 0x3333;
  first->speedLimit = 0x3333;
  Actor_SetScript(a, gMagmaRockInside_BlazeCasterScript);
  Event_PlaySound(SE_TABLET_RISE);
  Event_Wait(45);
  Actor_SetSpriteColorById(a, 256);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  i = 0;
burst:
  object = (struct Actor *)(struct Actor *)Import_ActorCreate(
      672, first->x.fixed, first->y.fixed, first->z.fixed);
  Event_PlaySound(SE_GLITTER);
  object->x.fixed += ((Random16() & 31) - 16) << 16;
  object->y.fixed += ((Random16() & 15) - 8) << 16;
  object->speedLimit = px(8);
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
  for (i = 0; i != 16; i++) {
    spark = FieldActor_Create(SPRITE_PARTICLE, second->x.fixed, second->y.fixed,
                              second->z.fixed);
    Event_PlaySound(SE_SPARKS);
    spark->timer = (Random16() & 0x7fff) + 0x8000;
    spark->userValue = (((i & 1) << 1) - 1) << ((Random16() & 3) + 9);
    noMotion = 0;
    spark->motionFlags = noMotion;
    spark->riseBaseY = px(122);
    spark->watchedActor = second;
    ((union ActorView *)spark)->bytes.unk62 = ((16 - i) << 3) + 15;
    FieldActor_SetSpriteColor(spark, 10);
    Scene_SetActorSpritePriority(spark, 1);
    Actor_SelectAnimation(spark, 7);
    Actor_SetSpriteFlags(spark, 0);
    Actor_SetBlendMode(spark, 1);
    FieldVector_AddPolarOffsetFixed(spark->riseBaseY, spark->timer,
                                    &spark->x.fixed);
    Actor_StartScript(spark, gMagmaRockInside_SparkScript);
    if (i == 3) {
      Actor_SetSpriteColorById(b, 256);
      second->motionFlags = noMotion;
      second->acceleration = 0x3333;
      second->speedLimit = 0x3333;
      Actor_SetScript(b, script);
    }
    wait(8);
  }
  Event_PlaySound(SE_SHIMMER);
  wait(16);
  i = 2;
column:
  riser =
      FieldActor_Create(SPRITE_PARTICLE, second->x.fixed,
                        second->y.fixed + i * px(1) - px(8), second->z.fixed);
  riser->timer = Random16();
  riser->userValue = 0x800;
  ((union ActorView *)riser)->bytes.unk62 = i;
  ((union ActorView *)riser)->bytes.unk63 = 1;
  riser->bounce = riser->x.fixed;
  riser->riseBaseY = 0;
  riser->gravity = riser->z.fixed;
  riser->watchedActor = second;
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
  PaletteFade_SetTarget(0x7fff, 0);
  PaletteFade_StartInterpolation(8);
  wait(16);
  MapGrid_SetShake(0, 0, 0);
  Actor_SetSpriteColorById(a, 0);
  Actor_SetSpriteColorById(b, 0);
  Event_PlaySound(SE_574);
  PaletteFade_SetTarget(0x10000, 0);
  PaletteFade_StartInterpolation(80);
  Actor_SetScript(a, gMagmaRockInside_BlazeCasterReturnScript);
  Actor_SetScript(b, gMagmaRockInside_BlazeWatcherReturnScript);
  second->update = (void (*)(void))MagmaRockInside_TrailSparks;
  wait(120);
  second->update = NULL;
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Scene_PlayMusic();
  wait(1);
}

s32 Actor_MoveToObject();

/// Reading the Blaze tablet shows its inscription; the first time (flag 0x9fa
/// clear) the scene with actor 5 follows.
void MagmaRockInside_ReadBlazeTablet(void) {
  struct Actor *object;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered(MsgBlazeTabletInscription, 1);
  if (Event_GetFlag(0x9fa) != 0) {
    Event_End();
  } else {
    FieldEvent_SetMessageId((s32)MsgBlazeTabletInscription);
    gFieldWork->message++;
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.7);
    Actor_SetSpeed(ACTOR_JENNA, 0.7);
    Actor_MoveToObject(ACTOR_JENNA, gMainRoutineWork.leaderActorId);
    FieldEvent_Wait(1);
    Actor_WalkTo(gMainRoutineWork.leaderActorId, 256, 136);
    Actor_WalkTo(ACTOR_JENNA, 272, 136);
    FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
    FieldActor_WaitForMoveById(ACTOR_JENNA);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
    FieldEvent_Wait(10);
    // "Flame's strength? Hey, that must mean it's finally my turn!"
    Event_ShowMessage(ACTOR_JENNA, 0);
    Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_NOD);
    FieldEvent_Wait(30);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 256, 152);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
    FieldEvent_Wait(30);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Actor_WalkToAndWait(ACTOR_JENNA, 264, 137);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
    FieldEvent_Wait(20);
#if (LANGUAGE == LANGUAGE_JAPANESE) || (LANGUAGE == LANGUAGE_ENGLISH)
    Message_ShowCentered(MsgBlazeTabletInscription + 3, 1);
#else
    Message_ShowCentered(MsgBlazeTabletInscription + 2, 1);
#endif
    MapGrid_SetShake(5, 7, 1);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    FieldMapGrid_WaitForShakeToSettle();
    MagmaRockInside_LearnBlaze(8, 5);
    Party_LearnAbility(JENNA, ABILITY_BLAZE);
    Event_SetFlag(0x9fa);
    Actor_FaceEachOther(gMainRoutineWork.leaderActorId, ACTOR_JENNA, 0);
    FieldEvent_Wait(10);
    Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
    Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_NOD);
    FieldEvent_Wait(20);
    Actor_SetSpeed(ACTOR_JENNA, 1.2);
    Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
    object = Actor_Get(gMainRoutineWork.leaderActorId);
    if (object != 0)
      Actor_SetDestination(ACTOR_JENNA, object->x.part[0].pixel,
                           object->z.part[0].pixel);
    FieldActor_WaitForMoveById(ACTOR_JENNA);
    Actor_SetPosition(ACTOR_JENNA, 0, 0);
    Actor_SetMovementType(Actor_Get(8), 9);
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
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 248, 107);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 248, 136);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 264, 136);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
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
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, n1, 136);            \
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 264, 136);           \
      Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);    \
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

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene != SCENE_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST) {
    if (scene == SCENE_MAGMA_ROCK_INSIDE_2)
      return gMagmaRockInside_Room2Triggers;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_FURY)
      return gMagmaRockInside_Room3Triggers;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_4)
      return gMagmaRockInside_Room4Triggers;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST)
      return gMagmaRockInside_Room5Triggers;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_6)
      return gMagmaRockInside_Room6Triggers;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE)
      return gMagmaRockInside_Room7Triggers;
    if (scene == SCENE_MAGMA_ROCK_INSIDE_BLAZE_TABLET)
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
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_FURY) {
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
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_4) {
    s16 *script = gMagmaRockInside_PlatformPathF;

    MagmaRockInside_StartLavaPlatform(15, script, 0x302);
    MagmaRockInside_StartLavaPlatform(16, gMagmaRockInside_PlatformPathF,
                                      0x302);
    MagmaRockInside_StartLavaPlatform(17, gMagmaRockInside_PlatformPathG,
                                      0x302);
    MagmaRockInside_StartLavaPlatform(18, gMagmaRockInside_PlatformPathG,
                                      0x302);
    MagmaRockInside_StartLavaPlatform(19, gMagmaRockInside_PlatformPathH,
                                      0x302);
    MagmaRockInside_StartLavaPlatform(20, gMagmaRockInside_PlatformPathH,
                                      0x302);
    MagmaRockInside_StartLavaPlatform(21, gMagmaRockInside_PlatformPathI,
                                      0x302);
    MagmaRockInside_StartLavaPlatform(22, gMagmaRockInside_PlatformPathI,
                                      0x302);
    if (Actor_Get(16)->scriptCursor == 0) {
      Actor_Get(16)->scriptCursor = 10;
      Actor_Get(18)->scriptCursor = 10;
      Actor_Get(20)->scriptCursor = 10;
      Actor_Get(22)->scriptCursor = 10;
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
  owner = Actor_Get(party->leaderActorId);
  ownerY = FIELD_PIXEL(owner->y.fixed);
  y = FIELD_PIXEL(object->y.fixed);
  flags = &object->priorityFlags;

  if (ownerY != y || (s16)(FIELD_PIXEL(owner->z.fixed)) <
                         (s16)(FIELD_PIXEL(object->z.fixed))) {
    *flags = 1;
  } else {
    *flags = 0;
    Scene_SetActorSpritePriority(object, 3);
  }
}
#undef BYTE_AT

#undef DEFINE_MAGMA_ROCK_INSIDE_BURST_PILLAR

#undef gMagmaRockInside_BlazeWatcherReturnScript
// Magma Rock's inside: its crumbling pillars and traps.

void Tightrope_Initialize(void);

extern s16 gMagmaRockInside_PlatformPathA[];
extern u32 gMagmaRockInside_PlatformPathB[];
extern u16 gMagmaRockInside_PlatformPathC[];
extern u32 gMagmaRockInside_PlatformPathD[];
extern s16 gMagmaRockInside_PlatformPathE[];
extern s16 gMagmaRockInside_PlatformPathJ[];
extern s16 gMagmaRockInside_PlatformPathK[];
extern s16 gMagmaRockInside_PlatformPathL[];
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

  object->motionFlags = ACTOR_FALLS;
  object->y.fixed = object->groundHeight = 20 << 16;
  object->priorityFlags |= 128;
}

static __inline__ void Lower(s32 id, s32 height) {
  Actor_GetById(id)->motionFlags = 0;
  Actor_GetById(id)->y.fixed = Actor_GetById(id)->groundHeight = height;
}

s32 Scene_Initialize(void) {
  s32 i;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_LUCKY_MEDAL_CHEST) {
    Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |= 64;
    MagmaRockInside_StartLavaPlatform(8, gMagmaRockInside_PlatformPathA, 768);
    if (!Event_GetFlag(FLAG_AREA_TEMP_00)) {
      CLOSED_BLEND();
      MapGrid_CopyCellsTo(71, 64, 47, 98, 1, 1);
    }
  } else if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_2) {
    s32 j;

    FieldEffect_StartBlankOverlay(0);
    Actor_GetById(15)->update = MagmaRockInside_LayerWithLeader;
    j = 0;
    do {
      {
        struct Actor *object = Actor_GetById(j + 9);

        object->motionFlags = 0;
        object->y.fixed = object->groundHeight = 0;
      }
    } while (++j != 6);
    if (Event_GetFlag(2544))
      Actor_GetById(15)->z.fixed = 132 << 17;
    Actor_PlaceBlocks(gMagmaRockInside_Room2Pillars);
    MagmaRockInside_RaiseActorCells(gMagmaRockInside_Statues);
    Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |= 64;
    Raise(16);
    if (Event_GetFlag(FLAG_AREA_TEMP_00))
      MagmaRockInside_PressGateSwitch1();
    else
      CLOSED_BLEND();
  } else if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_FURY) {
    FieldEffect_StartBlankOverlay(0);
    if (Event_GetFlag(2545))
      Actor_GetById(11)->z.fixed = 166 << 18;
    Actor_PlaceBlocks(gMagmaRockInside_Room3Pillars);
    Tightrope_Initialize();
    Tightrope_Place(0, 13, 14);
    MagmaRockInside_RaiseActorCells(gMagmaRockInside_Statues);
    Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |= 64;
    Raise(9);
    MagmaRockInside_StartLavaPlatform(10, gMagmaRockInside_PlatformPathB, 769);
    MagmaRockInside_StartLavaPlatform(12, gMagmaRockInside_PlatformPathC, 769);
    if (Event_GetFlag(FLAG_AREA_TEMP_01))
      MagmaRockInside_PressGateSwitch2();
    else
      CLOSED_BLEND();
  } else if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_4) {
    FieldEffect_StartBlankOverlay(0);
    Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |= 64;
    MagmaRockInside_RaiseActorCells(gMagmaRockInside_Room4Statues);
    if (gMainRoutineWork.entrance <= 7) {
      MagmaRockInside_StartLavaPlatform(10, gMagmaRockInside_PlatformPathD,
                                        769);
      MagmaRockInside_StartLavaPlatform(12, gMagmaRockInside_PlatformPathE,
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
        Actor_GetById(23)->x.fixed = 172 << 17;
      if (Event_GetFlag(2547))
        Actor_GetById(24)->x.fixed = 172 << 17;
      if (Event_GetFlag(2548))
        Actor_GetById(25)->x.fixed = 172 << 17;
      Actor_PlaceBlocks(gMagmaRockInside_Room4Pillars);
      Lower(26, 0);
      Actor_GetById(15)->retainSpriteCache = 3;
      Actor_GetById(16)->retainSpriteCache = 3;
      Actor_GetById(17)->retainSpriteCache = 3;
      Actor_GetById(18)->retainSpriteCache = 3;
      Actor_GetById(19)->retainSpriteCache = 3;
      Actor_GetById(20)->retainSpriteCache = 3;
      Actor_GetById(21)->retainSpriteCache = 3;
      Actor_GetById(22)->retainSpriteCache = 3;
      MagmaRockInside_StartLavaPlatform(15, gMagmaRockInside_PlatformPathF,
                                        770);
      MagmaRockInside_StartLavaPlatform(16, gMagmaRockInside_PlatformPathF,
                                        770);
      MagmaRockInside_StartLavaPlatform(17, gMagmaRockInside_PlatformPathG,
                                        770);
      MagmaRockInside_StartLavaPlatform(18, gMagmaRockInside_PlatformPathG,
                                        770);
      MagmaRockInside_StartLavaPlatform(19, gMagmaRockInside_PlatformPathH,
                                        770);
      MagmaRockInside_StartLavaPlatform(20, gMagmaRockInside_PlatformPathH,
                                        770);
      MagmaRockInside_StartLavaPlatform(21, gMagmaRockInside_PlatformPathI,
                                        770);
      MagmaRockInside_StartLavaPlatform(22, gMagmaRockInside_PlatformPathI,
                                        770);
      if (Event_GetFlag(FLAG_AREA_TEMP_02))
        MagmaRockInside_PressGateSwitch3();
      else
        CLOSED_BLEND();
    }
  } else if (gMainRoutineWork.next ==
             SCENE_MAGMA_ROCK_INSIDE_GOLEM_CORE_CHEST) {
    FieldEffect_StartBlankOverlay(0);
    Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |= 64;
    HangingVine_Place(10, 1);
    HangingVine_Place(11, 1);
    Raise(8);
    Raise(9);
    if (Event_GetFlag(FLAG_AREA_TEMP_02)) {
      MagmaRockInside_SwapCells(0, 22, 26, 0);
      MagmaRockInside_SwapCells(0, 25, 26, 0);
      Actor_SetSpritePriority(FIELD_TREASURE_ACTOR, 3);
    } else {
      CLOSED_BLEND();
    }
  } else if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_6) {
    FieldEffect_StartBlankOverlay(0);
    Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |= 64;
    if (Event_GetFlag(2549)) {
      MapGrid_CopyCellAttributes(46, 74, 1, 1, 46, 73);
      Actor_GetById(9)->x.fixed = 190 << 18;
    }
    if (Event_GetFlag(2550))
      Actor_SetPosition(12, 920, 616);
    Actor_PlaceBlocks(gMagmaRockInside_Room6Pillars);
    MagmaRockInside_StartLavaPlatform(8, gMagmaRockInside_PlatformPathJ, 770);
    MagmaRockInside_StartLavaPlatform(10, gMagmaRockInside_PlatformPathK, 770);
    MagmaRockInside_StartLavaPlatform(11, gMagmaRockInside_PlatformPathL, 770);
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
  } else if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE) {
    i = 0;
  room7:
    {
      struct Actor *object = Actor_GetById(i + 8);

      Actor_SelectAnimation(object, 4);
      object->motionFlags = 0;
      object->y.fixed = object->groundHeight = -px(32);
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
  } else if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_INSIDE_BLAZE_TABLET) {
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
    Actor_SetMovementType(Actor_GetById(8), 9);
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  s32 scene = gMainRoutineWork.next;
  s32 entrance = gMainRoutineWork.entrance;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    if ((scene == SCENE_MAGMA_ROCK_INSIDE_2 && entrance == 8) ||
        (scene == SCENE_MAGMA_ROCK_INSIDE_EMBER_STONE && entrance == 11)) {
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
#include "shared/pillars_restore_raised.h"

#include "shared/traps_check.h"

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
  Actor_Get(id)->retainSpriteCache = 3;
  Actor_DisableById(id);
}

// Places actors 8, 9 and 10, then walks a 0xffff-ended list of (actor, flag)
// halfword pairs: flag + 1 set parks the actor at (0, 0); otherwise its cell
// gets attributeB 128 and its shape-grid byte is rewritten, and flag set makes
// it copy actor 9's visual attributes.
#include "shared/pillars_restore.h"

void MagmaRockInside_TakePlaceOf(struct Actor *object, struct Actor *target) {
  FieldActor_SetPositionAndResetMotion(object, target->x.fixed, target->y.fixed,
                                       target->z.fixed);
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
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  partner = Actor_GetById(8);
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
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
        MagmaRockInside_SetLeaderCellShape(-2, trap->layer);
        MagmaRockInside_SpawnRubble(1);
        wait(16);
        MagmaRockInside_SetLeaderCellShape(-2, trap->layer);
        wait(4);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        wait(8);
        leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
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

#include "../field/field_effect_advance_motion_priority.c"

#undef CLOSED_BLEND
// Magma Rock's inside: its falling rocks and rubble, and its tables.

void Actor_StartScript(struct Actor *object, const void *data);

void MagmaRockInside_SpawnRubble(s32 spread) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;
  s32 y;

  do {
    y = leader->y.fixed - ((Random_Below(14)) << 16);
    effect = (struct FieldEffect *)FieldActor_Create(
        285, (i << 17) + leader->x.fixed - px(8), y - 0x20000, leader->z.fixed);
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
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct FieldEffect *effect;
  u32 frame;
  u32 i;

  if (leader == 0)
    return;
  frame = 0;
  i = 0;
  do {
    effect = (struct FieldEffect *)FieldActor_Create(
        285, leader->x.fixed, leader->y.fixed, leader->z.fixed);
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

void MagmaRockInside_SetActorCellShape(s32 level, s32 layer, s32 owner) {
  struct Actor *object = Actor_GetById(owner);
  u8 *shape = Ram_MapShapeGrid + (Ram_HeapSlots->mapGrid->layers[layer].cells -
                                  (struct MapCell *)Ram_MapCellBuffer);
  s32 value =
      MapGrid_GetTerrainKind(object->layer, object->x.fixed, object->z.fixed);
  s32 height =
      Map_GetTerrainHeight(object->layer, object->x.fixed, object->z.fixed) >>
      19;

  height += level;

  shape += object->x.fixed / px(16) + object->z.fixed / px(16) * 128;
  *shape = MapCollision_FindOrAddTag(object->layer, value, height);
}

void MagmaRockInside_SetLeaderCellShape(s32 a, s32 b) {
  MagmaRockInside_SetActorCellShape(a, b, gMainRoutineWork.leaderActorId);
}

void BattleEffect_StepVector3(struct Particle *particle, s32 unk1, s32 step);

#include "shared/particles_update_falling.h"

/// The falling rocks' work: their tiles, 64 rocks and their sprites, in a
/// ring that next walks round.
struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct RockSprite sprites[64];
  s16 next;
  u16 tileBase;
  s16 resource;
};

/// Spawns count rocks around (x, y, z), each pushed out at a random angle and
/// thrown up and out along it.
void MagmaRockInside_DropRocks(s32 x, s32 y, s32 z, s32 count) {
  struct RockWork *work;
  s32 i;

  work = Ram_HeapSlots->menuWork;
  /* A goto loop carries no loop notes, so the offsets are rebuilt on every
   */
  /* pass, as the bytes have them; a for loop hoists them. */
  if (count > 0) {
    i = count;
  loop:;
    {
      s32 angle = Random16();
      struct Rock *rock = &work->rocks[work->next];

      rock->timer = 0;
      rock->position[0] = x;
      rock->position[1] = y;
      rock->position[2] = z;
      FieldVector_AddPolarOffsetFixed((Random16() << 3) + 0x80000, angle,
                                      rock->position);
      rock->velocity[0] = 0;
      rock->velocity[1] = px(5);
      rock->velocity[2] = 0;
      FieldVector_AddPolarOffsetFixed((Random16() << 1) + 0x20000, angle,
                                      rock->velocity);
      work->next++;
      work->next &= 63;
    }
    i--;
    if (i != 0)
      goto loop;
  }
}

void MagmaRockInside_StartFallingRocks(void) {
  struct RockWork *work =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct RockWork));
  s32 entry;
  s32 tile;
  s32 i;
  struct Rock *rock;
  struct RockSprite *sprite;

  MagmaRockInside_DecompressTaggedLz(
      Resource_GetTableEntry((s32)&ResourceId_FallingRock), work);
  entry = VramCache_FindFreeEntry();
  tile = Vram_Load(entry, 0x500, work);
  work->tileBase = tile;
  work->resource = entry;
  rock = work->rocks;
  sprite = work->sprites;
  for (i = 63; i >= 0; i--, sprite++, rock++) {
    MagmaRockInside_InitRockSprite(sprite, 8, 8, 0x40000000, tile);
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
  FieldTask_AddFrame(Particles_UpdateFalling, TASK_PASS_VBLANK);
}

void MagmaRockInside_EndFallingRocks(void) {
  struct MagmaRockInsideSparkWork *work = Ram_HeapSlots->menuWork;

  FieldTask_Remove(Particles_UpdateFalling);
  VramCache_FreeEntry(work->resource);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

void MagmaRockInside_MeetDjinni(void);
void MagmaRockInside_ResolveBeforeBurst(void);
void MagmaRockInside_FireStatue(void);
void MagmaRockInside_LightTorch(void);
void MagmaRockInside_OpenStairs(void);

extern u32 gFieldEffect_LongScript[];

// clang-format off
u32 gFieldEffect_ShortScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(22, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_MediumScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(44, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_LongScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(126, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

s32 gPushLeaderSteps[] = {                                                               
      PUSH_STEP(16, 0),  PUSH_STEP(16, 0),  PUSH_STEP(16, 0),                  
      PUSH_STEP(0, 16),  PUSH_STEP(0, 16),  PUSH_STEP(0, 16),                  
      PUSH_STEP(0, 16),  PUSH_STEP(-16, 0), PUSH_STEP(-16, 0),                 
      PUSH_STEP(-16, 0), PUSH_STEP(-16, 0), PUSH_STEP(0, -16),                 
      PUSH_STEP(0, -16), PUSH_STEP(0, -16), PUSH_STEP(0, -16),                 
      PUSH_STEP(16, 0),                                                        
  };

u32 gMagmaRockInside_EmberScript[] = {
    SC_ANIMATION(2),
    SC_WAIT(18),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gMagmaRockInside_FlameEmberScript[] = {
    SC_ANIMATION(3),
    SC_WAIT(23),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

s16 gMagmaRockInside_PlatformPathA[] = {                                                               
      4, 0,  0, 776, 0, 16, 0, 680, 1,  0,  4,  0,  0, 744, 0,  16,               
      0, 680, 1, 0,  4, 0,  0, 744, 0,  16, 0,  632, 1, 0,  4,  0,                
      0, 776, 0, 16, 0, 632, 1, 0,  13, 0,  -1, 0,  0, 0,  17, 0,                
  };

u32 gMagmaRockInside_PlatformPathB[] = {                                                               
      SC_MOVE(216, 16, 568),                             
      SC_SYNC,                                                                 
      SC_MOVE(216, 16, 584),                             
      SC_SYNC,                                                                 
      SC_MOVE(136, 16, 584),                             
      SC_SYNC,                                                                 
      SC_MOVE(136, 16, 568),                             
      SC_SYNC,                                                                 
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
      SC_END,                                                                  
  };

u16 gMagmaRockInside_PlatformPathC[] = {
    4, 0, 0, 600, 0, 16, 0, 664,
    1, 0, 4, 0, 0, 728, 0, 16,
    0, 664, 1, 0, 4, 0, 0, 728,
    0, 16, 0, 632, 1, 0, 4, 0,
    0, 760, 0, 16, 0, 632, 1, 0,
    4, 0, 0, 760, 0, 16, 0, 664,
    1, 0, 17, 0,
};

u32 gMagmaRockInside_PlatformPathD[] = {                                                               
      SC_MOVE(184, 16, 232),                             
      SC_SYNC,                                                                 
      SC_MOVE(280, 16, 232),                             
      SC_SYNC,                                                                 
      SC_MOVE(280, 16, 264),                             
      SC_SYNC,                                                                 
      SC_MOVE(184, 16, 264),                             
      SC_SYNC,                                                                 
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
      SC_END,                                                                  
  };

s16 gMagmaRockInside_PlatformPathE[] = {                                                               
      4, 0,  0, 472, 0, 16, 0, 296, 1,  0,  4,  0,  0, 424, 0,  16,               
      0, 296, 1, 0,  4, 0,  0, 424, 0,  16, 0,  264, 1, 0,  4,  0,                
      0, 472, 0, 16, 0, 264, 1, 0,  13, 0,  -1, 0,  0, 0,  17, 0,                
  };

s16 gMagmaRockInside_PlatformPathF[] = {                                                               
      4, 0,  0, 216, 0, 16, 0, 600, 1,  0,  4,  0,  0, 280, 0,  16,               
      0, 600, 1, 0,  4, 0,  0, 280, 0,  16, 0,  664, 1, 0,  4,  0,                
      0, 216, 0, 16, 0, 664, 1, 0,  13, 0,  -1, 0,  0, 0,  17, 0,                
  };

s16 gMagmaRockInside_PlatformPathG[] = {                                                               
      4, 0,  0, 312, 0, 16, 0, 664, 1,  0,  4,  0,  0, 376, 0,  16,               
      0, 664, 1, 0,  4, 0,  0, 376, 0,  16, 0,  600, 1, 0,  4,  0,                
      0, 312, 0, 16, 0, 600, 1, 0,  13, 0,  -1, 0,  0, 0,  17, 0,                
  };

s16 gMagmaRockInside_PlatformPathH[] = {                                                               
      4, 0,  0, 440, 0, 16, 0, 600, 1,  0,  4,  0,  0, 504, 0,  16,               
      0, 600, 1, 0,  4, 0,  0, 504, 0,  16, 0,  664, 1, 0,  4,  0,                
      0, 440, 0, 16, 0, 664, 1, 0,  13, 0,  -1, 0,  0, 0,  17, 0,                
  };

s16 gMagmaRockInside_PlatformPathI[] = {                                                               
      4, 0,  0, 536, 0, 16, 0, 664, 1,  0,  4,  0,  0, 600, 0,  16,               
      0, 664, 1, 0,  4, 0,  0, 600, 0,  16, 0,  600, 1, 0,  4,  0,                
      0, 536, 0, 16, 0, 600, 1, 0,  13, 0,  -1, 0,  0, 0,  17, 0,                
  };

s16 gMagmaRockInside_PlatformPathJ[] = {                                                               
      4, 0,  0, 376, 0, 16, 0, 264, 1,  0,  4,  0,  0, 376, 0,  16,               
      0, 232, 1, 0,  4, 0,  0, 408, 0,  16, 0,  232, 1, 0,  4,  0,                
      0, 408, 0, 16, 0, 264, 1, 0,  13, 0,  -1, 0,  0, 0,  17, 0,                
  };

s16 gMagmaRockInside_PlatformPathK[] = {                                                               
      4, 0,  0, 216, 0, 16, 0, 680, 1,  0,  4,  0,  0, 280, 0,  16,               
      0, 680, 1, 0,  4, 0,  0, 280, 0,  16, 0,  712, 1, 0,  4,  0,                
      0, 216, 0, 16, 0, 712, 1, 0,  13, 0,  -1, 0,  0, 0,  17, 0,                
  };

s16 gMagmaRockInside_PlatformPathL[] = {                                                               
      4, 0,  0, 376, 0, 16, 0, 680, 1,  0,  4,  0,  0, 312, 0,  16,               
      0, 680, 1, 0,  4, 0,  0, 312, 0,  16, 0,  712, 1, 0,  4,  0,                
      0, 376, 0, 16, 0, 712, 1, 0,  13, 0,  -1, 0,  0, 0,  17, 0,                
  };

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
    SC_WAIT(6),
    SC_RELEASE,
    SC_END,
};

u32 gMagmaRockInside_TorchScript[] = {                                                               
      SC_MOVE_BY(0, 160, 0),                                           
      SC_SYNC,                                                                 
      SC_RELEASE,                                                                 
      SC_END,                                                                  
  };

s32 gMagmaRockInside_SparkScript[] = {
    SC_CALL(MagmaRockInside_OrbitParent),
    SC_RELEASE,
    SC_END,
};

s32 MagmaRockInside_DriftForSteps(struct Actor *object);

s32 gMagmaRockInside_OrbitScript[] = {                                                               
      SC_CALL(Orbit_Update), SC_CALL(Actor_AimAtWatched), SC_CALL(MagmaRockInside_DriftForSteps), SC_RELEASE, SC_END,         
  };

u32 gMagmaRockInside_BlazeCasterScript[] = {                                                               
      SC_MOVE_BY(0, 24, 0),                                            
      SC_SYNC,                                                                 
      SC_SET(SW_MOTION, 4),                                                    
      SC_END,                                                                  
  };

u32 gMagmaRockInside_BlazeWatcherScript[] = {                                                               
      SC_MOVE_BY(0, 16, 0),                                            
      SC_SYNC,                                                                 
      SC_SET(SW_MOTION, 4),                                                    
      SC_END,                                                                  
  };

s32 gMagmaRockInside_BlazeCasterReturnScript[] = {                                                               
      SC_SET(SW_MOTION, 0),                                                    
      SC_MOVE_BY(0, -24, 0),                                           
      SC_SYNC,                                                                 
      SC_SET(SW_MOTION, 0),                                                   
      SC_END,                                                                  
  };

s32 gMagmaRockInside_BlazeWatcherReturnScript[] = {                                                               
      SC_SET(SW_MOTION, 0),                                                    
      SC_MOVE_BY(0, -16, 0),                                           
      SC_SYNC,                                                                 
      SC_SET(SW_MOTION, 3),                                                   
      SC_END,                                                                  
  };

u32 gMagmaRockInside_RubbleScript[] = {                                                               
      SC_WAIT(33), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),       
      SC_RELEASE,                                                                 
  };

u32 gMagmaRockInside_DustScript[] = {                                                               
      SC_WAIT(44), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),       
      SC_RELEASE,                                                                 
  };

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
    SCENE_EXITS(MagmaRockInsideLuckyMedalChest),
    SCENE_EXIT(1, MagmaRockInsideLuckyMedalChest, 4),
    SCENE_EXIT(2, MagmaRockInside2, 1),
    SCENE_EXIT(3, MagmaRockInside2, 5),
    SCENE_EXIT(4, MagmaRockInsideLuckyMedalChest, 1),
    SCENE_EXIT(5, MagmaRockInside2, 2),
    SCENE_EXIT(6, MagmaRockInside2, 3),
    SCENE_EXIT(7, MagmaRockInside2, 4),
    SCENE_EXIT(8, MagmaRockInsideFury, 1),
    SCENE_EXITS(MagmaRockInside2),
    SCENE_EXIT(1, MagmaRockInsideLuckyMedalChest, 2),
    SCENE_EXIT(2, MagmaRockInsideLuckyMedalChest, 5),
    SCENE_EXIT(3, MagmaRockInsideLuckyMedalChest, 6),
    SCENE_EXIT(4, MagmaRockInsideLuckyMedalChest, 7),
    SCENE_EXIT(5, MagmaRockInsideLuckyMedalChest, 3),
    SCENE_EXIT(6, MagmaRockInside2, 7),
    SCENE_EXIT(7, MagmaRockInside2, 6),
    SCENE_EXIT(8, MagmaRockStatue, 3),
    SCENE_EXITS(MagmaRockInsideFury),
    SCENE_EXIT(1, MagmaRockInsideLuckyMedalChest, 8),
    SCENE_EXIT(2, MagmaRockInsideFury, 6),
    SCENE_EXIT(3, MagmaRockInside4, 1),
    SCENE_EXIT(4, MagmaRockInside4, 2),
    SCENE_EXIT(5, MagmaRockInside4, 3),
    SCENE_EXIT(6, MagmaRockInsideFury, 2),
    SCENE_EXIT(7, MagmaRockInside4, 4),
    SCENE_EXIT(8, MagmaRockInsideFury, 9),
    SCENE_EXIT(9, MagmaRockInsideFury, 8),
    SCENE_EXIT(10, MagmaRockInside4, 5),
    SCENE_EXIT(11, MagmaRockInside4, 6),
    SCENE_EXITS(MagmaRockInside4),
    SCENE_EXIT(1, MagmaRockInsideFury, 3),
    SCENE_EXIT(2, MagmaRockInsideFury, 4),
    SCENE_EXIT(3, MagmaRockInsideFury, 5),
    SCENE_EXIT(4, MagmaRockInsideFury, 7),
    SCENE_EXIT(5, MagmaRockInsideFury, 10),
    SCENE_EXIT(6, MagmaRockInsideFury, 11),
    SCENE_EXIT(7, MagmaRockInside6, 7),
    SCENE_EXIT(8, MagmaRockInsideGolemCoreChest, 7),
    SCENE_EXIT(9, MagmaRockInsideGolemCoreChest, 8),
    SCENE_EXIT(10, MagmaRockInsideGolemCoreChest, 1),
    SCENE_EXIT(11, MagmaRockInside6, 1),
    SCENE_EXIT(12, MagmaRockInside6, 2),
    SCENE_EXIT(13, MagmaRockInside6, 5),
    SCENE_EXIT(14, MagmaRockInside6, 6),
    SCENE_EXITS(MagmaRockInsideGolemCoreChest),
    SCENE_EXIT(1, MagmaRockInside4, 10),
    SCENE_EXIT(2, MagmaRockInside6, 3),
    SCENE_EXIT(3, MagmaRockInsideGolemCoreChest, 9),
    SCENE_EXIT(4, MagmaRockInsideGolemCoreChest, 10),
    SCENE_EXIT(5, MagmaRockInside6, 11),
    SCENE_EXIT(6, MagmaRockInside6, 4),
    SCENE_EXIT(7, MagmaRockInside4, 8),
    SCENE_EXIT(8, MagmaRockInside4, 9),
    SCENE_EXIT(9, MagmaRockInsideGolemCoreChest, 3),
    SCENE_EXIT(10, MagmaRockInsideGolemCoreChest, 4),
    SCENE_EXITS(MagmaRockInside6),
    SCENE_EXIT(1, MagmaRockInside4, 11),
    SCENE_EXIT(2, MagmaRockInside4, 12),
    SCENE_EXIT(3, MagmaRockInsideGolemCoreChest, 2),
    SCENE_EXIT(4, MagmaRockInsideGolemCoreChest, 6),
    SCENE_EXIT(5, MagmaRockInside4, 13),
    SCENE_EXIT(6, MagmaRockInside4, 14),
    SCENE_EXIT(7, MagmaRockInside4, 7),
    SCENE_EXIT(8, MagmaRockInside6, 14),
    SCENE_EXIT(9, MagmaRockInside6, 16),
    SCENE_EXIT(10, MagmaRockInside6, 17),
    SCENE_EXIT(11, MagmaRockInsideGolemCoreChest, 5),
    SCENE_EXIT(12, MagmaRockInside6, 18),
    SCENE_EXIT(13, MagmaRockInside6, 19),
    SCENE_EXIT(14, MagmaRockInside6, 8),
    SCENE_EXIT(15, MagmaRockInsideEmberStone, 10),
    SCENE_EXIT(16, MagmaRockInside6, 9),
    SCENE_EXIT(17, MagmaRockInside6, 10),
    SCENE_EXIT(18, MagmaRockInside6, 12),
    SCENE_EXIT(19, MagmaRockInside6, 13),
    SCENE_EXITS(MagmaRockInsideEmberStone),
    SCENE_EXIT(1, MagmaRockInsideBlazeTablet, 1),
    SCENE_EXIT(2, MagmaRockInsideEmberStone, 5),
    SCENE_EXIT(3, MagmaRockInsideEmberStone, 6),
    SCENE_EXIT(4, MagmaRockInsideEmberStone, 7),
    SCENE_EXIT(5, MagmaRockInsideEmberStone, 2),
    SCENE_EXIT(6, MagmaRockInsideEmberStone, 3),
    SCENE_EXIT(7, MagmaRockInsideEmberStone, 4),
    SCENE_EXIT(8, MagmaRockInsideEmberStone, 9),
    SCENE_EXIT(9, MagmaRockInsideEmberStone, 8),
    SCENE_EXIT(10, MagmaRockInside6, 15),
    SCENE_EXIT(11, MagmaRockSlide, 2),
    SCENE_EXITS(MagmaRockInsideBlazeTablet),
    SCENE_EXIT(1, MagmaRockInsideEmberStone, 1),
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
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, 0x6b, MOVEMENT_TYPE_STAND, 296, 0, 504, FACING_SOUTH, TALK_FACING_PARTY, 0),
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
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, 0x68, MOVEMENT_TYPE_STAND, 456, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
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
    ACTOR_PLACEMENT(373, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(373, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
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
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
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

#undef gMagmaRockInside_BlazeWatcherReturnScript
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY
#undef DEFINE_MAGMA_ROCK_INSIDE_BURST_PILLAR
#undef DEFINE_MAGMA_ROCK_INSIDE_LIGHT_TORCH
#undef BYTE_AT
#undef DEFINE_MAGMA_ROCK_INSIDE_READ_TABLET_FROM
