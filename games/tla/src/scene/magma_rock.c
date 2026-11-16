/// @file
/// Magma Rock, outside and on its slopes, where the party climbs to the Magma
/// Ball the diggers of Loho need for their cannon, in three rooms that share
/// this overlay (SceneId_MagmaRockSlide to SceneId_MagmaRockStatue).
#include "resource_ids.h"
#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_prepare.c"

#include "types.h"
#include "flags.h"
#include "system/ram_buffer.h"
#include "system/iwram_call.h"
#include "field/field_sequence.h"
#include "field/actor.h"
#include "field/event_inline.h"
#include "scene/scene.h"
#include "system/main_routine_work.h"
#include "field/field_coordinate.h"
#include "system/scene.h"
#include "field/map_grid.h"
#include "field/field.h"
#include "field/event_commands.h"
#include "scene/magma_rock.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "scene_ids.h"
#include "system/frame.h"
#include "field/event.h"
#include "field/field_sequence_event.h"
#include "field/flag_call.h"
#include "system/field_effect.h"
#include "macros.h"
#include "field/call.h"
#include "fixed_math.h"
#include "scene/falling_rocks.h"
#include "field/sprite.h"
#include "field/object.h"
#include "scene/mars_lighthouse_star_room.h"
#include "field/field_sprite.h"
#include "graphics/resource.h"
#include "system/heap_release.h"
#include "field/field_effect.h"
#include "field/field_psynergy_work.h"
#include "field/particle.h"
#include "system/task.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"

// Each pushed pillar's flag keeps it where it was pushed: two on the slide,
// one on each of the Salamander Tail room's lower, middle and upper levels and
// two on its top.
#define FLAG_SLIDE_PILLAR_A_PUSHED 0x8d5
#define FLAG_SLIDE_PILLAR_B_PUSHED 0x8d6
#define FLAG_LOWER_PILLAR_PUSHED 0x8d7
#define FLAG_MIDDLE_PILLAR_PUSHED 0x8d8
#define FLAG_UPPER_PILLAR_PUSHED 0x8d9
#define FLAG_TOP_PILLAR_A_PUSHED 0x8da
#define FLAG_TOP_PILLAR_B_PUSHED 0x8db
/// The statue's blast opened the way into Magma Rock (MagmaRock_ScatterEmbers).
#define FLAG_WAY_IN_BLASTED 0x8dc
// Set as Burst breaks each cracked pillar, until the scene is left.
#define FLAG_CRACKED_PILLAR_A_BURST FLAG_SCENE_TEMP_00
#define FLAG_CRACKED_PILLAR_B_BURST FLAG_SCENE_TEMP_01
#define FLAG_CRACKED_PILLAR_C_BURST FLAG_SCENE_TEMP_02
#define FLAG_CRACKED_PILLAR_D_BURST FLAG_SCENE_TEMP_03
#define FLAG_CRACKED_PILLAR_E_BURST FLAG_SCENE_TEMP_04

/// The slide's cracked pillars, lettered as the events that break them.
/// The slide: its two pushed pillars, the statue whose flame burns on it, the
/// pair of statues a magma rock rolls between, and its cracked pillars,
/// lettered as the events that break them.
enum MagmaRockSlideActorId {
  ACTOR_SLIDE_PILLAR_A = ACTOR_FIRST_PLACED,
  ACTOR_SLIDE_STATUE,
  ACTOR_SLIDE_PILLAR_B,
  ACTOR_CRACKED_PILLAR_E,
  ACTOR_SLIDE_PAIR_EAST_STATUE = ACTOR_FIRST_PLACED + 5,
  ACTOR_SLIDE_PAIR_WEST_STATUE,
  ACTOR_CRACKED_PILLAR_A,
  ACTOR_CRACKED_PILLAR_B,
  ACTOR_CRACKED_PILLAR_C,
  ACTOR_CRACKED_PILLAR_D,
  ACTOR_SLIDE_STATUE_FLAME = ACTOR_FIRST_PLACED + 12, ///< sprite 379
  ACTOR_SLIDE_MAGMA_ROCK ///< a flame puff, rolling between the pair
};

/// The Salamander Tail room's lower level: the cracked pillars the first two
/// events break there.
enum MagmaRockSalamanderTailLowerActorId {
  ACTOR_LOWER_CRACKED_PILLAR_B = ACTOR_FIRST_PLACED + 2,
  ACTOR_LOWER_TIGHTROPE_WEST_POST,
  ACTOR_LOWER_TIGHTROPE_EAST_POST,
  ACTOR_LOWER_CRACKED_PILLAR_A = ACTOR_FIRST_PLACED + 6,
  ACTOR_LOWER_PILLAR
};

/// Its middle level: three pairs of statues raised to three heights, a magma
/// rock rolling between each pair, two statues' flames and a pushed pillar.
enum MagmaRockSalamanderTailMiddleActorId {
  ACTOR_STATUE_PAIR_A_EAST = ACTOR_FIRST_PLACED,
  ACTOR_STATUE_PAIR_A_WEST,
  ACTOR_STATUE_PAIR_B_EAST,
  ACTOR_STATUE_PAIR_B_WEST,
  ACTOR_STATUE_PAIR_C_EAST,
  ACTOR_STATUE_PAIR_C_WEST,
  ACTOR_MIDDLE_FLAME_A = ACTOR_FIRST_PLACED + 7,
  ACTOR_MIDDLE_PILLAR,
  ACTOR_MIDDLE_FLAME_B = ACTOR_FIRST_PLACED + 10,
  ACTOR_MAGMA_ROCK_A,
  ACTOR_MAGMA_ROCK_B,
  ACTOR_MAGMA_ROCK_C
};

/// Its upper level: a pushed pillar, two statues' flames and the rope's posts.
enum MagmaRockSalamanderTailUpperActorId {
  ACTOR_UPPER_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_UPPER_FLAME_A = ACTOR_FIRST_PLACED + 2,
  ACTOR_UPPER_FLAME_B = ACTOR_FIRST_PLACED + 4,
  ACTOR_UPPER_ROPE_POST_A,
  ACTOR_UPPER_ROPE_POST_B
};

/// Its top: two pushed pillars and a statue's flame.
enum MagmaRockSalamanderTailTopActorId {
  ACTOR_TOP_PILLAR_A = ACTOR_FIRST_PLACED,
  ACTOR_TOP_PILLAR_B,
  ACTOR_TOP_FLAME = ACTOR_FIRST_PLACED + 5
};
#include "shared/tables.h"

struct Actor *MagmaRock_CreateEffectActor(s32 x, s32 y, s32 z, s32 kind) {
  struct Actor *object = (FieldActor_Create(kind, x, y, z));

  if (object != NULL) {
    Scene_SetActorSpritePriority(object, 1);
    object->motionFlags = 0;
    object->collisionFlags = ACTOR_NOT_TARGETED;
    Actor_SetSpriteFlags(object, 0);
    FieldActor_SetSpriteColor(object, SPRITE_COLOR_HIDDEN);
    object->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES | ACTOR_PRIORITY_UNDERFOOT;
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

#include "shared/distance.c"

#include "../field/actor_find_in_cell.c"

#undef EFFECT_SPAWN

extern s32 gPushLeaderSteps[];

#include "shared/block_push_onto.h"

#include "../field/map_cell_fill_byte.c"

// Magma Rock's outside, the volcano north of Loho the party climbs to its
// summit: the Moai statues Burst wakes, which fire magma rocks or lift the
// leader on a column of flame, the cracked columns they break, the pillars
// Move pushes and the crumbling ones, Growth's vines and the slides, and at
// the top the Psynergy rock that blasts the way in.

void MagmaRock_FightFury(void) {
  Field_RequestDjinnBattle(ACTOR_FIRST_PLACED, DJINNI_FURY);
}

#include "shared/position_flat_distance.h"

extern u8 gMapShapeGrid[];

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
s32 MapCollision_FindOrAddTag(s32 layer, s32 shape, s32 height);

#include "shared/map_cell_set_shape.h"

#include "../field/map_cell_get.c"

#include "shared/actors_raise_cells.h"

/// For each id of a list ended by 0xffff: clears the object's mark and, on a
/// cell with attribute bit 6, blocks the cells ahead of and behind it and sets
/// the mark.
void Blocks_MarkCells(u16 *ids) {
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
    if (MapCell_GetAttributes(0, FIELD_CELL(object->x.fixed),
                              FIELD_CELL(object->z.fixed)) &
        0x40) {
      map = Ram_HeapSlots->mapGrid;
      if (map == NULL)
        return;
      cells = &map->layers[2].cells;
      z = FIELD_CELL(object->z.fixed - object->y.fixed);
      x = FIELD_CELL(object->x.fixed);
      cell = *cells;
      cell += x + ((z + 1) << 7);
      cell->collisionCode = CELL_BEHAVIOR_WALL;
      cell = *cells;
      cell += x + ((z - 1) << 7);
      cell->collisionCode = CELL_BEHAVIOR_WALL;
      *blocked = 1;
    }
    ids++;
  }
}

// Clears the object's mark and the cells ahead of and behind it that the mark
// blocked.

#include "shared/block_clear_cells.h"

// For each id of a list ended by 0xffff: settles the object on its ground,
// copies the cell under it, and blocks the cells ahead of and behind it on a
// cell with attribute bit 6.

/// For each id of a list ended by 0xffff: settles the object on its ground,
/// copies the cell under it, and blocks the cells ahead of and behind it on a
/// cell with attribute bit 6.
void MagmaRock_SettleBlocks(u16 *ids) {
  while (*ids != 0xffff) {
    struct Actor *object = Actor_Get(*ids);
    u8 *terrainId = &object->layer;
    s32 height =
        Map_GetTerrainHeight(*terrainId, object->x.fixed, object->z.fixed);
    u8 *blocked;
    struct MapWork *map;
    struct MapCell **cells;
    struct MapCell *cell;
    s32 x, z;

    object->riseBaseY =
        255 & MapCell_GetAttributes(0, FIELD_CELL(object->x.fixed),
                                    FIELD_CELL(object->z.fixed));
    object->motionFlags = 0;
    object->collisionFlags = 0;
    Actor_SetSpriteFlags(object, 0);
    MagmaRockInside_SetCellShape(*terrainId, FIELD_CELL(object->x.fixed),
                                 FIELD_CELL(object->z.fixed),
                                 (height >> 19) + 4);
    blocked = &object->riseEnabled;
    *blocked = 0;
    if (MapCell_GetAttributes(0, FIELD_CELL(object->x.fixed),
                              FIELD_CELL(object->z.fixed)) &
        64) {
      map = Ram_HeapSlots->mapGrid;
      if (map == NULL)
        return;
      cells = &map->layers[2].cells;
      z = FIELD_CELL(object->z.fixed - object->y.fixed);
      x = FIELD_CELL(object->x.fixed);
      cell = *cells;
      cell += x + ((z + 1) << 7);
      cell->collisionCode = CELL_BEHAVIOR_WALL;
      cell = *cells;
      cell += x + ((z - 1) << 7);
      cell->collisionCode = CELL_BEHAVIOR_WALL;
      *blocked = 1;
    }
    ids++;
  }
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern s16 gMagmaRock_Regions[];
extern s16 gMagmaRock_SalamanderTailRegions[];
extern s16 gMagmaRock_StatueRegions[];

const struct SceneEntrance *Scene_GetExitRegions(void) {
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST)
    return (const struct SceneEntrance *)gMagmaRock_SalamanderTailRegions;
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_STATUE)
    return (const struct SceneEntrance *)gMagmaRock_StatueRegions;
  return (const struct SceneEntrance *)gMagmaRock_Regions;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gMagmaRock_Placements[];
extern struct ActorPlacement gMagmaRock_SalamanderTailLowerPlacements[];
extern struct ActorPlacement gMagmaRock_SalamanderTailMiddlePlacements[];
extern struct ActorPlacement gMagmaRock_SalamanderTailUpperPlacements[];
extern struct ActorPlacement gMagmaRock_SalamanderTailTopPlacements[];
extern struct ActorPlacement gMagmaRock_StatuePlacements[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
    if (gMainRoutineWork.entrance <= 3)
      return gMagmaRock_SalamanderTailLowerPlacements;
    if (gMainRoutineWork.entrance <= 8)
      return gMagmaRock_SalamanderTailMiddlePlacements;
    if (gMainRoutineWork.entrance <= 12)
      return gMagmaRock_SalamanderTailUpperPlacements;
    return gMagmaRock_SalamanderTailTopPlacements;
  }
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_STATUE)
    return gMagmaRock_StatuePlacements;
  return gMagmaRock_Placements;
}

void Actor_LiftTargetBlock(s16 *table);

void MagmaRock_UnblockMovedPillar(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;

  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_SLIDE) {
    MapGrid_CopyCellAttributes(98, 18, 4, 3, 78, 21);
    MapGrid_CopyCellAttributes(98, 24, 4, 3, 78, 24);
    Actor_LiftTargetBlock(gMagmaRock_SlidePillars);
  } else if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
    if (gMainRoutineWork.entrance <= 3) {
      Actor_LiftTargetBlock(gMagmaRock_SalamanderTailLowerPillars);
    } else if (gMainRoutineWork.entrance <= 7) {
      Actor_LiftTargetBlock(gMagmaRock_SalamanderTailMiddlePillars);
    } else if (gMainRoutineWork.entrance <= 11) {
      MapGrid_CopyCellAttributes(79, 46, 2, 1, 79, 36);
      Actor_LiftTargetBlock(gMagmaRock_SalamanderTailUpperPillars);
    } else {
      if (work->child->z.part[0].integer == 600)
        MapGrid_CopyCellAttributes(104, 38, 1, 1, 103, 38);
      else
        MapGrid_CopyCellAttributes(94, 44, 5, 1, 99, 38);
      Actor_LiftTargetBlock(gMagmaRock_SalamanderTailTopPillars);
    }
  }
  MagmaRockInside_ClearBlockCells(event->movedBlock);
}

void Event_Start();
void Actor_PlaceMovedBlock();
s32 MagmaRock_EndEvent();

/// Rebuilds the room's opened passages on entry and sets the flag of the cell
/// the leader stands on.
s32 MagmaRock_BlockMovedPillar(void) {
  struct Actor *leader = gFieldWork->movedBlock;
  s32 x, z;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_PlaceMovedBlock();
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_SLIDE) {
    MapGrid_CopyCellAttributes(98, 21, 4, 3, 78, 21);
    MapGrid_CopyCellAttributes(98, 27, 4, 3, 78, 24);
    Blocks_MarkCells(gMagmaRock_SlidePillars);
  } else if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
    if (gMainRoutineWork.entrance <= 3) {
      Blocks_MarkCells(gMagmaRock_SalamanderTailLowerPillars);
    } else if (gMainRoutineWork.entrance <= 8) {
      Blocks_MarkCells(gMagmaRock_SalamanderTailMiddlePillars);
    } else if (gMainRoutineWork.entrance <= 12) {
      MapGrid_CopyCellAttributes(79, 37, 2, 1, 79, 36);
      Blocks_MarkCells(gMagmaRock_SalamanderTailUpperPillars);
    } else {
      MapGrid_CopyCellAttributes(103, 39, 1, 1, 103, 38);
      MapGrid_CopyCellAttributes(99, 42, 5, 1, 99, 38);
      Blocks_MarkCells(gMagmaRock_SalamanderTailTopPillars);
    }
  }
  x = FIELD_CELL(leader->x.fixed);
  z = FIELD_CELL(leader->z.fixed);
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_SLIDE) {
    if (x == 15 && z == 25)
      Event_SetFlag(FLAG_SLIDE_PILLAR_B_PUSHED);
    else if (x == 16 && z == 22)
      Event_SetFlag(FLAG_SLIDE_PILLAR_A_PUSHED);
  } else if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
    if (gMainRoutineWork.entrance <= 3) {
      if (x == 27 && z == 33)
        Event_SetFlag(FLAG_LOWER_PILLAR_PUSHED);
    } else if (gMainRoutineWork.entrance <= 8) {
      if (x == 55 && z == 35)
        Event_SetFlag(FLAG_MIDDLE_PILLAR_PUSHED);
    } else if (gMainRoutineWork.entrance <= 12) {
      if (x == 16 && z == 36)
        Event_SetFlag(FLAG_UPPER_PILLAR_PUSHED);
    } else if (x == 40 && z == 37) {
      Event_SetFlag(FLAG_TOP_PILLAR_A_PUSHED);
    } else if (x == 35 && z == 38) {
      Event_SetFlag(FLAG_TOP_PILLAR_B_PUSHED);
    }
  }
  return MagmaRock_EndEvent();
}

void Import_FieldPsynergySetupActorPair(s32 owner, u16 value);

void MagmaRock_PlayBurst(u16 value) {
  struct MainRoutineWork *party;

  FieldPsynergy_LoadResources(ABILITY_BURST, 1);
  party = &gMainRoutineWork;
  Import_FieldPsynergySetupActorPair(party->leaderActorId, value);
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  FieldPsynergy_End();
}

extern s32 gMagmaRock_FlameEmberScript[];

void MagmaRock_ThrowEmbers(struct Actor *object) {
  struct Actor *linked = object->watchedActor;
  s16 *timer = &object->timer;

  if (*timer < 50) {
    u32 phase = gFrameCount & 3;

    if (phase == 0) {
      struct EffectOptions params;
      s32 dx;
      s32 dy;

      params.type = SPRITE_FLAME_PUFF;
      params.priority = 1;
      params.script = gMagmaRock_FlameEmberScript;
      {
        s32 mask = 15;

        dx = mask & Random16();
      }
      dy = Random16() & 31;
      FieldEffect_Spawn(
          object->x.fixed + ((dx - 8) << 16),
          object->y.fixed + (dy << 16) - toFIXED(8), object->z.fixed, 0, phase,
          phase, EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | 1,
          &params);
    }
    if (gFrameCount & 4)
      FieldActor_SetSpriteColor(object, SPRITE_COLOR_RED_TINT);
    else
      FieldActor_SetSpriteColor(object, SPRITE_COLOR_NORMAL);
  } else if (*timer < 100) {
    if (gFrameCount & 4)
      FieldActor_SetSpriteColor(object, SPRITE_COLOR_RED_TINT);
    else
      FieldActor_SetSpriteColor(object, SPRITE_COLOR_NORMAL);
  } else if (*timer < 140) {
    if (gFrameCount & 2)
      FieldActor_SetSpriteColor(object, SPRITE_COLOR_RED_TINT);
    else
      FieldActor_SetSpriteColor(object, SPRITE_COLOR_NORMAL);
  } else if (*timer < 144) {
    FieldActor_SetSpriteColor(object, SPRITE_COLOR_WHITE);
  } else {
    FieldActor_SetSpriteColor(object, SPRITE_COLOR_NORMAL);
    object->update = NULL;
    linked->x.fixed = object->x.fixed;
    linked->y.fixed = object->y.fixed;
    linked->z.fixed = object->z.fixed;
    linked->update = MagmaRock_RollMagmaRock;
  }
  (*timer)++;
}

void Actor_PlaySoundInView(struct Actor *object, s32 value);

/// Every fourth frame, spawns flame puff at the object with a random z offset
/// and z velocity.
void MagmaRock_TrailEmbers(struct Actor *object) {
  u32 frame = gFrameCount;

  if ((frame & 3) == 0) {
    struct EffectOptions config;
    s32 dz;
    s32 velocityZ;
    FIXED z;

    config.type = SPRITE_FLAME_PUFF;
    config.priority = 2;
    config.script = gMagmaRock_EmberScript;
    if ((frame & 15) == 0 && !Event_GetFlag(FLAG_SCENE_TEMP_60))
      Actor_PlaySoundInView(object, SE_WHOOSH);
    dz = ((Random16() & 15) - 8) << 15;
    velocityZ = ((Random16() & 31) - 16) << 12;
    z = object->z.fixed;
    FieldEffect_Spawn(
        object->x.fixed, object->y.fixed, z + dz, 0, 0, velocityZ,
        EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | 1, &config);
  }
}

FIXED Trig_Cos(s32 angle);

void MagmaRock_RollMagmaRock(struct Actor *object) {
  FIXED step;
  struct Actor *partner;
  struct Actor *leader;

  step = Trig_Cos(object->facing);
  partner = Actor_Get(object->userValue + object->rollSide);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  object->x.fixed += step * 2;
  MagmaRock_TrailEmbers(object);
  if (leader->x.fixed - toFIXED(8) < object->x.fixed + toFIXED(6) &&
      object->x.fixed - toFIXED(6) < leader->x.fixed + toFIXED(8) &&
      leader->z.fixed - toFIXED(8) < object->z.fixed + toFIXED(6) &&
      object->z.fixed - toFIXED(6) < leader->z.fixed + toFIXED(8) &&
      leader->y.fixed < object->y.fixed + toFIXED(8) &&
      object->y.fixed - toFIXED(8) < leader->y.fixed + toFIXED(15)) {
    gFieldWork->raisedTrigger = 200;
    object->update = NULL;
    object->x.fixed = 0;
    object->y.fixed = 0;
    object->z.fixed = 0;
  }
  if (Position_FlatDistance(&object->x.fixed, &partner->x.fixed) <= 8 &&
      FIELD_CELL(object->y.fixed) == FIELD_CELL(partner->y.fixed)) {
    object->x.fixed = 0;
    object->update = NULL;
    object->facing ^= 0x9000;
    object->y.fixed = 0;
    object->z.fixed = 0;
    object->rollSide ^= 1;
    partner->timer = 0;
    partner->update = (void *)MagmaRock_ThrowEmbers;
    partner->watchedActor = object;
  }
}

// Unless flag 0x109 is set, places the object at the watched object first +
// side, gives it MagmaRock_RollMagmaRock and faces it along the pair first and
// first + 1.

void MagmaRock_PlaceVolley(u16 id, s16 first, s16 side) {
  struct Actor *object = Actor_Get(id);
  struct Actor *a;
  struct Actor *b;
  u16 left;
  u16 right;
  s32 linked;
  s32 next;
  u8 *flags;
  u8 facing;
  s16 *pairFirst;

  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    return;
  left = first;
  right = side;
  linked = left + right;
  object->x.fixed = Actor_Get(linked)->x.fixed;
  object->y.fixed = Actor_Get(linked)->y.fixed;
  object->z.fixed = Actor_Get(linked)->z.fixed;
  Scene_SetActorSpritePriority(object, 2);
  pairFirst = &object->userValue;
  *pairFirst = first;
  facing = side ^ 1;
  object->rollSide = facing;
  object->update = (void *)MagmaRock_RollMagmaRock;
  next = left + 1;
  flags = &object->motionFlags;
  if (right != 0) {
    a = Actor_Get(left);
    b = Actor_Get(next);
    if (a->x.fixed < b->x.fixed)
      object->facing = FACING_WEST;
    else
      object->facing = 0;
  } else {
    a = Actor_Get(left);
    b = Actor_Get(next);
    if (a->x.fixed > b->x.fixed)
      object->facing = FACING_WEST;
    else
      object->facing = 0;
  }
  *flags = 0;
}

void MagmaRock_FlashStatue(struct Actor *object) {
  if (gFrameCount & object->flashMask)
    FieldActor_SetSpriteColor(object, SPRITE_COLOR_RED_TINT);
  else
    FieldActor_SetSpriteColor(object, SPRITE_COLOR_NORMAL);
}

// Magma Rock's statues: the magma they fire, the flame columns that lift the
// leader, the cracked columns, slides, pillars, vines and the final blast.

void MagmaRock_WarnStatue(struct Actor *object) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  if (object->timer > 528) {
    if (object->timer & 2)
      FieldActor_SetSpriteColor(object, SPRITE_COLOR_RED_TINT);
    else
      FieldActor_SetSpriteColor(object, SPRITE_COLOR_NORMAL);
  } else if (object->timer > 400) {
    if (object->timer & 16)
      FieldActor_SetSpriteColor(object, SPRITE_COLOR_RED_TINT);
    else
      FieldActor_SetSpriteColor(object, SPRITE_COLOR_NORMAL);
  } else {
    object->update = MagmaRock_BurnStatue;
    object->timer = 1;
  }
  if (work->psynergy == 0 && work->fieldMenu == 0 && work->pauseMenu == 0 &&
      work->item == 0 && work->transitionActive != 0)
    object->timer--;
}

s32 MagmaRock_FindStatue(u16 *table) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;
  s32 result = 0;
  s32 code =
      MapCell_GetAttributes(0, FIELD_CELL(work->x), FIELD_CELL(work->z)) >> 8;
  s32 id;

  for (; *table != 0xffff; result = *table++) {
    id = *table++;
    if (code == *table++) {
      work->child = Actor_Get(id);
      result = *table;
      break;
    }
  }
  return result;
}

s32 MagmaRock_LookUpCellStatue(s32 x, s32 z, u16 *table) {
  s32 result = 0;
  s32 code = MapCell_GetAttributes(0, FIELD_CELL(x), FIELD_CELL(z)) >> 8;

  while (*table != 0xffff) {
    table++;
    if (code == *table) {
      result = table[1];
      break;
    }
    table++;
    result = *table++;
  }
  return result;
}

/// Picks the cell table for the room and entrance, finds the target in it and,
/// in modes 2 and 3, sets its flash mask to 4 or 2 and makes it blink
/// with MagmaRock_FlashStatue.
void MagmaRock_TargetStatue(s32 mode) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;
  s16 *table = gMagmaRock_SlideStatues;
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->next == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
    if (party->entrance <= 3)
      table = gMagmaRock_SalamanderTailLowerStatues;
    else if (party->entrance <= 8)
      table = gMagmaRock_SalamanderTailMiddleStatues;
    else if (party->entrance <= 12)
      table = gMagmaRock_SalamanderTailUpperStatues;
    else
      table = gMagmaRock_SalamanderTailTopStatues;
  } else if (party->next == SCENE_MAGMA_ROCK_STATUE) {
    table = gMagmaRock_StatueStatues;
  }
  if (mode == 2) {
    struct Actor *target;

    MagmaRock_FindStatue(table);
    target = work->child;
    target->flashMask = 4;
    target->update = (void (*)(void))MagmaRock_FlashStatue;
  }
  if (mode == 3) {
    struct Actor *target;

    MagmaRock_FindStatue(table);
    target = work->child;
    target->flashMask = 2;
    target->update = (void (*)(void))MagmaRock_FlashStatue;
  }
}

/// Adds the velocity to the position and decays it by one eighth each
/// update.
void MagmaRock_DriftAndSlow(struct Actor *object) {
  object->x.fixed += object->bounce;
  object->y.fixed += object->gravity;
  object->z.fixed += object->riseBaseY;
  object->bounce -= object->bounce >> 3;
  object->gravity -= object->gravity >> 3;
  object->riseBaseY -= object->riseBaseY >> 3;
}

s32 Position_FlatDistance(FIXED *a, FIXED *b);
void Actor_Release(struct Actor *object);
s32 MagmaRock_EndEvent(void);
void FieldCamera_WaitForArrival(void);

s32 MagmaRock_FireFromStatue(struct Actor *source) {
  FIXED position[3];
  struct EffectOptions options;
  struct Actor *object;
  struct Actor *other;
  s32 step;
  s32 parity;
  s32 i;
  s32 attributes;

  {
    FIXED x = source->x.fixed;
    FIXED y = source->y.fixed;
    FIXED z = source->z.fixed;

    object = FieldActor_Create(SPRITE_FLAME_PUFF, x, y, z);
  }
  step = Trig_Cos(source->facing) << 1;
  object->motionFlags = 0;
  Actor_SelectAnimation(object, 1);
  Actor_SetSpriteFlags(object, 0);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  position[0] = object->x.fixed;
  position[1] = object->y.fixed;
  position[2] = object->z.fixed;
  for (;;) {
    position[0] += step;
    attributes = MapCell_GetAttributes(2, FIELD_CELL(position[0]),
                                       FIELD_CELL(position[2] - position[1])) >>
                 8;
    if (attributes == 212 || attributes == 222)
      goto stop;
    for (i = 8; i != 64; i++) {
      other = gFieldWork->actors[i];
      if (other->script == NULL)
        continue;
      if (FIELD_CELL(source->x.fixed) == FIELD_CELL(other->x.fixed) &&
          FIELD_CELL(source->y.fixed) == FIELD_CELL(other->y.fixed) &&
          FIELD_CELL(source->z.fixed) == FIELD_CELL(other->z.fixed))
        continue;
      if (Position_FlatDistance(&other->x.fixed, position) > 8)
        continue;
      if (FIELD_CELL(other->y.fixed) != FIELD_CELL(position[1]))
        continue;
      if (((struct Sprite *)other->sprite)->entries[0]->animationId == 0x110)
        goto stop;
      goto hit;
    }
  }

stop:
  object->acceleration = toFIXED(2);
  object->speedLimit = toFIXED(2);
  object->update = MagmaRock_TrailEmbers;
  FieldActor_SetMoveTargetFX(object, position[0], position[1], position[2]);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveToFX(position[0], position[1], position[2], 1);
  FieldActor_WaitForMove(object);
  object->update = NULL;
  Actor_Release(object);
  for (i = 0; i != 16; i++) {
    parity = gFrameCount & 1;

    if (parity == 0) {
      s32 dx;
      s32 dz;

      options.type = SPRITE_FLAME_PUFF;
      options.priority = 1;
      options.update = (void (*)(union FieldObject *))MagmaRock_DriftAndSlow;
      options.script = gMagmaRock_EmberScript;
      dx = Random16() & 31;
      dz = Random16() & 31;
      FieldEffect_Spawn(position[0], position[1], position[2], (dx - 16) << 13,
                        (dz - 16) << 13, parity,
                        EFFECT_USE_UPDATE | EFFECT_USE_SCRIPT |
                            EFFECT_USE_TYPE | EFFECT_USE_PRIORITY,
                        &options);
    }
    System_Wait(1);
  }
  System_Wait(24);
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  System_Wait(10);
  return MagmaRock_EndEvent();

hit:
  object->acceleration = toFIXED(2);
  object->speedLimit = toFIXED(2);
  object->update = MagmaRock_TrailEmbers;
  FieldActor_SetMoveTargetFX(object, position[0], position[1], position[2]);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveToFX(position[0], position[1], position[2], 1);
  FieldActor_WaitForMove(object);
  object->update = NULL;
  Actor_Release(object);
  Event_SetFlag(FLAG_SCENE_TEMP_F0);
  MagmaRock_PlayBurst(i);
  Event_ClearFlag(FLAG_SCENE_TEMP_F0);
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  System_Wait(10);
  return MagmaRock_EndEvent();
}

void MagmaRock_FireStatue(void) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;
  struct Actor *object;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_Wait(10);
  object = work->child;
  object->update = NULL;
  FieldActor_SetSpriteColor(object, SPRITE_COLOR_WHITE);
  FieldEvent_Wait(3);
  FieldActor_SetSpriteColor(work->child, SPRITE_COLOR_NORMAL);
  FieldEvent_Wait(1);
  MagmaRock_FireFromStatue(work->child);
  MagmaRock_EndEvent();
}

void MagmaRock_IgniteStatue(void) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;
  struct Actor *object;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  object = work->child;
  object->timer = 0x400;
  object->update = MagmaRock_WarnStatue;
  MagmaRock_EndEvent();
}

void MagmaRock_BurstStatue(s32 mode, s32 cue) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;

  if (cue == 0)
    MagmaRock_StartFallingRocks();
  if (mode == 2) {
    struct Actor *object = work->child;

    MagmaRock_DropRocks(object->x.fixed, object->y.fixed + toFIXED(15),
                        object->z.fixed, 1);
  }
  if (mode == 3) {
    struct Actor *object = work->child;
    struct Actor *target;

    MagmaRock_DropRocks(object->x.fixed, object->y.fixed + toFIXED(15),
                        object->z.fixed, 8);
    target = work->child;
    SET_ACTOR_POSITION(target, 0, 0, 0);
  }
  if (cue == PSYNERGY_EVENT_LAST_FRAME)
    MagmaRock_EndFallingRocks();
  if (cue == 178)
    Actor_SelectAnimation(work->child, 3);
}

// One of the four parts MagmaRock_BurnStatue builds: rises and falls with its
// owner's action counter.

void Actor_Release(struct Actor *part);

/// One of the four parts MagmaRock_BurnStatue builds: rises and falls with its
/// owner's action counter.
void MagmaRock_RaiseFlamePart(struct Actor *part) {
  struct Actor *owner = part->watchedActor;

  if (owner->timer > 35 && owner->timer < part->flameLevel * 2 + 36) {
    part->y.fixed = owner->y.fixed + toFIXED(32);
    part->y.fixed += (owner->timer << 19) + (toFIXED(-280));
  } else if (part->flameLevel * 2 + 36 < owner->timer && owner->timer < 332) {
    part->y.fixed = owner->y.fixed + (part->flameLevel << 20) + toFIXED(32);
  } else if (owner->timer > 332 && owner->timer < part->flameLevel * 2 + 333) {
    part->y.fixed = owner->y.fixed + (part->flameLevel << 20) + toFIXED(32);
    part->y.fixed -= (owner->timer << 19) - (toFIXED(2656));
  } else if (part->flameLevel * 2 + 333 < owner->timer) {
    Actor_Release(part);
  }
}

// The fifth part of MagmaRock_BurnStatue's group: follows its owner like the
// others, throws flame puff every eighth frame and marks the ground beneath.

/// The fifth part of MagmaRock_BurnStatue's group: follows its owner like the
/// others, throws flame puff every eighth frame and marks the ground beneath.
void MagmaRock_RaiseFlameTop(union FieldObject *object) {
  struct Actor *watched = (struct Actor *)object->effect.countdown;
  struct EffectOptions config;
  s32 step;
  s32 mask;

  if (watched->timer >= 35 && watched->timer <= 42) {
    object->effect.y = watched->y.fixed + toFIXED(24);
    object->effect.y =
        object->effect.y + (watched->timer << 19) + (toFIXED(-272));
  } else {
    if (watched->timer >= 9 && watched->timer <= 331) {
      object->effect.y =
          watched->y.fixed + (object->effect.riseCounter << 20) + toFIXED(24);
    } else if (watched->timer > 332 &&
               watched->timer < (object->effect.riseCounter << 1) + 333) {
      object->effect.y =
          watched->y.fixed + (object->effect.riseCounter << 20) + toFIXED(24);
      object->effect.y -= (watched->timer << 19) - (toFIXED(2656));
    } else if ((object->effect.riseCounter << 1) + 333 < watched->timer) {
      object->effect.y = object->actor.groundHeight;
      object->effect.update = 0;
    }
  }

  mask = 7;
  step = gFrameCount & mask;
  if (step == 0) {
    config.type = SPRITE_FLAME_PUFF;
    config.palette = SPRITE_COLOR_RED_TINT;
    config.priority = 2;
    config.update = (void (*)(union FieldObject *))MagmaRock_DriftAndSlow;
    config.script = gMagmaRock_EmberScript;
    FieldEffect_Spawn(object->effect.x + toFIXED(1),
                      object->effect.y - toFIXED(4), object->effect.z,
                      ((Random16() & mask) - 3) << 14, -0x20000, step,
                      EFFECT_USE_UPDATE | EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                          EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                      &config);
  }
  MagmaRockInside_SetCellShape(watched->layer, FIELD_CELL(watched->x.fixed),
                               FIELD_CELL(watched->z.fixed),
                               (object->effect.y >> 19) + 1);
}

void Actor_PlaySoundInView(struct Actor *object, s32 a);

struct SpriteAnimation;

/// The sprite as Sprite_Create fills it, laid out as vram_block_93.c
/// declares it.
struct ResourceSprite {
  u32 imageAttributes[3];
  s32 scale;
  u8 slot;
  u8 retainCacheEntry : 1;
  u8 useOwnScale : 1;
  u8 unk11 : 6;
  u16 rotation;
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 lastNo;
  u8 dirty;
  u8 flags;
  u8 count;
  u32 shadowAttributes[3];
  struct SpriteAnimation *entries[4];
};

void MagmaRock_BurnStatue(struct Actor *object) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 type = SPRITE_STEAM_VENT;

  if ((gFrameCount & 15) == 0)
    Actor_PlaySoundInView(object, SE_WHOOSH);
  FieldActor_SetSpriteColor(object, SPRITE_COLOR_RED_TINT);
  if (object->timer <= 31) {
    u32 phase = gFrameCount & 3;

    if (phase == 0) {
      struct EffectOptions config;
      s32 dx;
      s32 dz;

      config.type = SPRITE_FLAME_PUFF;
      config.palette = SPRITE_COLOR_RED_TINT;
      config.priority = 2;
      config.update = (void (*)(union FieldObject *))MagmaRock_DriftAndSlow;
      config.script = gMagmaRock_EmberScript;
      dx = Random16() & 15;
      dz = Random16() & 15;
      FieldEffect_Spawn(
          object->x.fixed + toFIXED(1), object->y.fixed + toFIXED(24),
          object->z.fixed, (dx - 8) << 14, 0, (dz - 8) << 14,
          EFFECT_USE_UPDATE | EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
              EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
          &config);
    }
  } else if (object->timer == 33) {
    struct Actor *parts[4];
    struct Actor *fifthPart;
    s32 id;
    s32 i;

    Actor_PlaySoundInView(object, SE_CLATTER);
    parts[0] =
        (FieldActor_Create(type, object->x.fixed + toFIXED(1),
                           object->y.fixed + toFIXED(32), object->z.fixed));
    for (i = 1; i != 4; i++) {
      struct ResourceSprite *first = parts[0]->sprite;
      struct Actor *part = (FieldActor_Create(type, 0, 0, 0));
      struct ResourceSprite *sprite;

      parts[i] = part;
      sprite = part->sprite;
      sprite->slot = first->slot;
      sprite->retainCacheEntry = 1;
      part->x.fixed = parts[0]->x.fixed;
      part->y.fixed = parts[0]->y.fixed;
      part->z.fixed = parts[0]->z.fixed;
      part->motionFlags = 0;
      Actor_SetSpriteFlags(part, 0);
      Actor_SelectAnimation(parts[i], 3);
      {
        struct Actor *flame = parts[i];

        flame->flameLevel = 3 - i;
        flame->watchedActor = object;
        flame->update = MagmaRock_RaiseFlamePart;
        FieldActor_SetSpriteColor(flame, SPRITE_COLOR_BGR);
      }
    }
    {
      struct Actor *part = parts[0];

      part->motionFlags = 0;
      Actor_SetSpriteFlags(part, 0);
      Actor_SelectAnimation(parts[0], 3);
      Scene_SetActorSpritePriority(parts[0], 3);
      {
        struct Actor *flame = parts[0];

        flame->flameLevel = 3;
        flame->watchedActor = object;
        flame->update = MagmaRock_RaiseFlamePart;
        FieldActor_SetSpriteColor(flame, SPRITE_COLOR_BGR);
      }
    }
    id = 0;
    if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_SLIDE) {
      id = MagmaRock_LookUpCellStatue(object->x.fixed, object->z.fixed,
                                      gMagmaRock_SlideStatues);
    } else if (gMainRoutineWork.next ==
               SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
      s16 entrance = gMainRoutineWork.entrance;
      FIXED x = object->x.fixed;
      FIXED z = object->z.fixed;

      if (entrance <= 3)
        id = MagmaRock_LookUpCellStatue(x, z,
                                        gMagmaRock_SalamanderTailLowerStatues);
      else if (entrance <= 8)
        id = MagmaRock_LookUpCellStatue(x, z,
                                        gMagmaRock_SalamanderTailMiddleStatues);
      else if (entrance <= 12)
        id = MagmaRock_LookUpCellStatue(x, z,
                                        gMagmaRock_SalamanderTailUpperStatues);
      else
        id = MagmaRock_LookUpCellStatue(x, z,
                                        gMagmaRock_SalamanderTailTopStatues);
    }
    fifthPart = Actor_Get(id);
    fifthPart->flameLevel = 4;
    fifthPart->watchedActor = object;
    fifthPart->update = MagmaRock_RaiseFlameTop;
    object->timer++;
  } else if (object->timer > 364) {
    FieldActor_SetSpriteColor(object, SPRITE_COLOR_NORMAL);
    object->update = NULL;
  }
  if (work->psynergy == 0 && work->fieldMenu == 0 && work->pauseMenu == 0 &&
      work->item == 0 && (u16)work->transitionActive != 0)
    object->timer++;
}

/// Burst on a cracked pillar of the slide or the Salamander Tail room's lower
/// level: the pillar breaks (MagmaRock_BurstStatue) and its flag is set.
#define DEFINE_MAGMA_ROCK_BURST_COLUMN(name, n1, n2, n3)                       \
  void name(s32 mode, s32 cue) {                                               \
    struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;             \
    struct MainRoutineWork *party = &gMainRoutineWork;                         \
                                                                               \
    {                                                                          \
      s32 scene = party->next;                                                 \
                                                                               \
      if (scene == SCENE_MAGMA_ROCK_SLIDE)                                     \
        work->child = Actor_Get(n1);                                           \
      else if (scene == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST)                \
        work->child = Actor_Get(n2);                                           \
      MagmaRock_BurstStatue(mode, cue);                                        \
    }                                                                          \
    Event_SetFlag(n3);                                                         \
  }

DEFINE_MAGMA_ROCK_BURST_COLUMN(MagmaRock_BurstCrackedPillarA,
                               ACTOR_CRACKED_PILLAR_A,
                               ACTOR_LOWER_CRACKED_PILLAR_A,
                               FLAG_CRACKED_PILLAR_A_BURST)

DEFINE_MAGMA_ROCK_BURST_COLUMN(MagmaRock_BurstCrackedPillarB,
                               ACTOR_CRACKED_PILLAR_B,
                               ACTOR_LOWER_CRACKED_PILLAR_B,
                               FLAG_CRACKED_PILLAR_B_BURST)

#define DEFINE_MAGMA_ROCK_BURST_STATUE_COLUMN(name, n1, n2)                    \
  void name(s32 mode, s32 cue) {                                               \
    struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;             \
                                                                               \
    work->child = Actor_Get(n1);                                               \
    MagmaRock_BurstStatue(mode, cue);                                          \
    Event_SetFlag(n2);                                                         \
  }

DEFINE_MAGMA_ROCK_BURST_STATUE_COLUMN(MagmaRock_BurstCrackedPillarC,
                                      ACTOR_CRACKED_PILLAR_C,
                                      FLAG_CRACKED_PILLAR_C_BURST)

DEFINE_MAGMA_ROCK_BURST_STATUE_COLUMN(MagmaRock_BurstCrackedPillarD,
                                      ACTOR_CRACKED_PILLAR_D,
                                      FLAG_CRACKED_PILLAR_D_BURST)

DEFINE_MAGMA_ROCK_BURST_STATUE_COLUMN(MagmaRock_BurstCrackedPillarE,
                                      ACTOR_CRACKED_PILLAR_E,
                                      FLAG_CRACKED_PILLAR_E_BURST)

void MagmaRock_ClearSlideFlag(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->movementMode != 2)
    Event_ClearFlag(FLAG_SCENE_TEMP_F8);
}

void MagmaRock_SlideDown(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->movementMode != 2) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Actor_Get(party->leaderActorId)->motionFlags = 0;
    Actor_WalkTo(party->leaderActorId, 264, 440);
    FieldEvent_Wait(5);
    Event_PlaySound(SE_MAP_EXIT);
    FieldEvent_SetExitRequest(2);
    MagmaRock_EndEvent();
  }
  Event_SetFlag(FLAG_SCENE_TEMP_F8);
}

void MagmaRock_ResolveBeforeBurst(void) { Event_ResolvePendingActions(0); }

void MagmaRock_CheckCrumblingPillars(void) {
  MagmaRock_StepOnCrumblingPillar(gMagmaRock_CrumblingPillars);
}

void MagmaRock_SetLedgeBehavior(void) {
  Field_SetCellBehaviorAt(0, toFIXED(264), toFIXED(552), 30);
  Event_SetFlag(FLAG_SCENE_TEMP_12);
}

void MagmaRock_ClearLedgeBehavior(void) {
  Field_SetCellBehaviorAt(0, toFIXED(264), toFIXED(552), 0);
  Event_ClearFlag(FLAG_SCENE_TEMP_12);
}

void Actor_SetAnimationSpeedById(s32 id, s32 action);
void FieldCamera_WaitForArrival(void);

void MagmaRock_AmplifyBurst(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct EffectOptions options;
  s32 position[3];
  struct EffectOptions *opt;
  s32 *offset;
  s32 i;
  u32 phase;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetFlag(FLAG_SCENE_TEMP_60);
  Event_PlaySound(SE_IMPACT);
  FieldActor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId),
                            SPRITE_COLOR_GREY);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 10);
  Actor_SetAnimationSpeedById(gMainRoutineWork.leaderActorId, 32);
  opt = &options;
  opt->priority = 1;
  opt->palette = SPRITE_COLOR_BLACK;
  opt->startScaleX = FIXED_ONE * 4 / 5;
  opt->startScaleY = FIXED_ONE * 4 / 5;
  i = 0;
first:
  {
    s32 r = Random16();

    offset = position;
    offset[0] = ((r & 15) << 16) - toFIXED(8);
  }
  offset[1] = (Random16() & 15) << 16;
  FieldEffect_Spawn(
      leader->x.fixed + offset[0], leader->y.fixed + offset[1], leader->z.fixed,
      0, 0x13333, 0,
      EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, opt);
  wait(5);
  i++;
  if (i != 8)
    goto first;
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  gMainRoutineWork.movementMode = LEADER_MOVEMENT_WALK;
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  leader->gravity = toFIXED(4) / 5;
  leader->bounce = 0;
  Event_Wait(1);
  leader->facing = FACING_NORTH;
  i = 60;
second:
  phase = gFrameCount & 1;
  if (phase == 0) {
    offset[0] = ((Random16() & 15) << 16) - toFIXED(8);
    offset[1] = (Random16() & 15) << 16;
    FieldEffect_Spawn(
        leader->x.fixed + offset[0], leader->y.fixed + offset[1],
        leader->z.fixed, 0, 0x13333, phase,
        EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, opt);
  }
  wait(1);
  if (leader->velocityY != 0) {
    i--;
    if (i != 0)
      goto second;
  }
  Event_PlaySound(SE_THUMP);
  Actor_SetSpriteFlags(leader, 1);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 22);
  MapGrid_SetShake(5, 8, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  FieldCamera_MoveToActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_SHAKE_HEAD);
  FieldActor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId),
                            SPRITE_COLOR_NORMAL);
  Actor_WaitForAnimationChange(gMainRoutineWork.leaderActorId);
  Event_ClearFlag(FLAG_SCENE_TEMP_60);
  leader->gravity = toFIXED(1);
  leader->bounce = FIXED_ONE / 4;
  MagmaRock_EndEvent();
}

void MagmaRockInside_ClearBlockCells(struct Actor *object);
void MagmaRockInside_PushBlockOnto(void);

void MagmaRock_PushPillarEast(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 position[3];
  s32 *cell = position;
  struct Actor *object;
  u8 *terrainId;
  s32 x;
  s32 z;
  s32 height;

  cell[0] = leader->x.fixed + toFIXED(16);
  cell[1] = leader->y.fixed;
  cell[2] = leader->z.fixed;
  object = Actor_FindInCell(cell, leader);
  MagmaRockInside_ClearBlockCells(object);
  terrainId = &object->layer;
  x = FIELD_CELL(object->x.fixed);
  z = FIELD_CELL(object->z.fixed);
  height = object->groundHeight >> 19;
  MagmaRockInside_SetCellShape(*terrainId, x, z, height);
  MapGrid_CopyCellAttributes(79, 46, 2, 1, 79, 36);
  MagmaRockInside_PushBlockOnto();
  *terrainId = 0;
  MapGrid_CopyCellAttributes(79, 37, 2, 1, 79, 36);
  MagmaRockInside_SetCellShape(*terrainId, FIELD_CELL(object->x.fixed),
                               FIELD_CELL(object->z.fixed),
                               (object->groundHeight >> 19) + 4);
  Blocks_MarkCells(gMagmaRock_SalamanderTailUpperPillars);
  cell[0] = FIELD_CELL(object->x.fixed);
  cell[2] = FIELD_CELL(object->z.fixed);
  if (cell[0] == 16 && cell[2] == 36)
    Event_SetFlag(FLAG_UPPER_PILLAR_PUSHED);
}

void MagmaRock_PushPillarWest(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 position[3];
  struct Actor *object;
  u8 *terrainId;
  s32 x;
  s32 z;
  s32 height;

  position[0] = leader->x.fixed - toFIXED(16);
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  object = Actor_FindInCell(position, leader);
  MagmaRockInside_ClearBlockCells(object);
  terrainId = &object->layer;
  x = FIELD_CELL(object->x.fixed);
  z = FIELD_CELL(object->z.fixed);
  height = object->groundHeight >> 19;
  MagmaRockInside_SetCellShape(*terrainId, x, z, height);
  MapGrid_CopyCellAttributes(94, 44, 5, 1, 99, 38);
  MagmaRockInside_PushBlockOnto();
  *terrainId = 0;
  MapGrid_CopyCellAttributes(99, 42, 5, 1, 99, 38);
  MagmaRockInside_SetCellShape(*terrainId, FIELD_CELL(object->x.fixed),
                               FIELD_CELL(object->z.fixed),
                               (object->groundHeight >> 19) + 4);
  Blocks_MarkCells(gMagmaRock_SalamanderTailTopPillars);
  position[0] = FIELD_CELL(object->x.fixed);
  position[2] = FIELD_CELL(object->z.fixed);
  if (position[0] == 35 && position[2] == 38)
    Event_SetFlag(FLAG_TOP_PILLAR_B_PUSHED);
}

void MagmaRock_GrowVineA(void) {
  MapGrid_CopyCellAttributes(121, 48, 1, 3, 123, 48);
  Event_SetFlag(FLAG_SCENE_TEMP_50);
}

void MagmaRock_GrowVineB(void) {
  MapGrid_CopyCellAttributes(121, 48, 1, 3, 125, 48);
  Event_SetFlag(FLAG_SCENE_TEMP_51);
}

void MagmaRock_GrowVineC(void) {
  MapGrid_CopyCellAttributes(108, 56, 1, 3, 109, 56);
  Event_SetFlag(FLAG_SCENE_TEMP_52);
}

void MagmaRock_ScatterEmbers(s32 x, s32 z) {
  struct EffectOptions config;
  s32 position[3];
  s32 i;
  s32 kind;

  config.startScaleX = FIXED_ONE * 2;
  config.startScaleY = FIXED_ONE * 2;
  config.type = SPRITE_FLAME_PUFF;
  kind = 1; // Holds one constant: the native loads 1 before the data address.
  config.script = gMagmaRock_EmberScript;
  config.priority = kind;
  i = 0;
loop:
  position[0] = x + ((Random16() & 63) << 16) - FIXED_ONE * 32;
  position[1] = (Random16() & 15) << 12;
  position[2] = z + ((Random16() & 63) << 16) - FIXED_ONE * 32;
  FieldEffect_Spawn(position[0], 0, position[2], 0, position[1], 0,
                    EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                        EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | 1,
                    &config);
  i++;
  if (i != 4)
    goto loop;
  Event_SetFlag(FLAG_WAY_IN_BLASTED);
}

void MagmaRock_FlashTarget(struct Actor *object) {
  if (gFrameCount & object->flashMask)
    FieldActor_SetSpriteColor(object, SPRITE_COLOR_WHITE);
  else
    FieldActor_SetSpriteColor(object, SPRITE_COLOR_NORMAL);
}

/// MagmaRock_BurstStatue's counterpart: on cue 0 it hands the target to
/// MagmaRock_FlashTarget; modes 2 and 3 run MagmaRock_ScatterEmbers at the
/// target's cell, and mode 3 also clears its position and copies the cells at
/// columns 32 and 96.
void MagmaRock_BlastOpenWall(s32 mode, s32 cue) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;

  if (cue == 0) {
    struct Actor *object;

    MagmaRock_StartFallingRocks();
    object = work->child;
    object->flashMask = 4;
    object->update = (void *)MagmaRock_FlashTarget;
  }
  if (mode == 2) {
    MagmaRock_ScatterEmbers(work->child->x.fixed, work->child->z.fixed);
    MagmaRock_DropRocks(work->child->x.fixed,
                        work->child->y.fixed + toFIXED(15),
                        work->child->z.fixed, 1);
  }
  if (mode == 3) {
    struct Actor *target;
    s32 last = 8;

    MagmaRock_ScatterEmbers(work->child->x.fixed, work->child->z.fixed);
    MagmaRock_ScatterEmbers(work->child->x.fixed, work->child->z.fixed);
    MagmaRock_ScatterEmbers(work->child->x.fixed,
                            work->child->z.fixed - toFIXED(32));
    MagmaRock_DropRocks(work->child->x.fixed,
                        work->child->y.fixed + toFIXED(15),
                        work->child->z.fixed, 16);
    target = work->child;
    SET_ACTOR_POSITION(target, 0, 0, 0);
    MapGrid_CopyCellAttributes(0, 32, 5, 8, 42, 17);
    MapGrid_CopyCellAttributes(0, 96, 5, 8, 42, 81);
    MapGrid_CopyCellsTo(0, 32, 42, 17, 5, last);
    MapGrid_CopyCellsTo(0, 96, 42, 81, 5, last);
  }
  if (cue == PSYNERGY_EVENT_LAST_FRAME)
    MagmaRock_EndFallingRocks();
}

extern struct Trigger gMagmaRock_Triggers[];
extern struct Trigger gMagmaRock_SalamanderTailLowerTriggers[];
extern struct Trigger gMagmaRock_SalamanderTailMiddleTriggers[];
extern struct Trigger gMagmaRock_SalamanderTailUpperTriggers[];
extern struct Trigger gMagmaRock_SalamanderTailTopTriggers[];
extern struct Trigger gMagmaRock_StatueTriggers[];

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
    if (gMainRoutineWork.entrance <= 3)
      return gMagmaRock_SalamanderTailLowerTriggers;
    if (gMainRoutineWork.entrance <= 8)
      return gMagmaRock_SalamanderTailMiddleTriggers;
    if (gMainRoutineWork.entrance <= 12)
      return gMagmaRock_SalamanderTailUpperTriggers;
    return gMagmaRock_SalamanderTailTopTriggers;
  }
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_STATUE)
    return gMagmaRock_StatueTriggers;
  return gMagmaRock_Triggers;
}

#include "../field/field_effect_advance_motion.c"

#undef DEFINE_MAGMA_ROCK_BURST_COLUMN
#undef DEFINE_MAGMA_ROCK_BURST_STATUE_COLUMN
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY
// Magma Rock's rooms set up, the leader launched and landed, and the
// crumbling pillars and traps.

FIXED Trig_Cos(s32 angle);
void FieldEffect_AdvanceMotion();
void MapGrid_CenterOnFollowedPoint(void);

void MagmaRock_LaunchLeader(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct EffectOptions options;
  u32 i;
  s32 angle;
  FIXED velocity[3];

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  MapGrid_CenterOnFollowedPoint();
  System_Wait(1);
  leader->y.fixed += toFIXED(130);
  leader->gravity = toFIXED(1) / 2;
  leader->bounce = 0;
  leader->motionFlags = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(SE_DUST_PUFF);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  FieldEvent_Wait(24);
  options.palette = SPRITE_COLOR_WHITE;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = FIXED_ONE * 4 / 5;
  options.startScaleY = FIXED_ONE * 4 / 5;
  i = 0;
loop:
  angle = i << 12;
  velocity[0] = Trig_Cos(angle);
  velocity[1] = 0;
  velocity[2] = Math_Sin(angle);
  velocity[0] += velocity[0] >> 1;
  velocity[0] += ((Random16() * 3) << 14 >> 16) - 0x6000;
  velocity[2] += ((Random16() * 3) << 13 >> 16) - 0x3000;
  FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                    velocity[0], velocity[1], velocity[2],
                    EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                        EFFECT_USE_PALETTE | 1,
                    &options);
  i++;
  if (i <= 16)
    goto loop;
  Event_PlaySound(SE_THUMP);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 22);
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldMapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  leader->gravity = toFIXED(1);
  leader->bounce = FIXED_ONE / 4;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  MagmaRock_EndEvent();
}

#include "shared/leader_lift.h"

s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);

void MagmaRock_LandLeader(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.leaderActorId);

  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    s32 height;

    System_Wait(1);
    height =
        Map_GetTerrainHeight(object->layer, object->x.fixed, object->z.fixed);
    object->velocityY = 0;
    object->groundHeight = height;
    object->y.fixed = height;
    object->targetY = toFIXED(-32768);
    EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 0);
  }
}

struct LayeredSprite {
  u8 unk00[4];
  u32 attributes;
  u8 unk08[4];
  s32 scale;
  u8 slot;
  u8 retainCacheEntry : 1;
  u8 useOwnScale : 1;
  u8 unk11 : 6;
  u16 rotation;
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 lastNo;
  u8 dirty;
  u8 flags;
  u8 count;
  struct AnimationSpritePart shadow;
  struct SpriteAnimation *entries[4];
};

static inline void AttachPart(struct Actor *part, struct Actor *anchor,
                              u8 order) {
  part->flameLevel = order;
  part->watchedActor = anchor;
  part->update = MagmaRock_RaiseFlamePart;
  FieldActor_SetSpriteColor(part, SPRITE_COLOR_BGR);
}

static inline void BuildParts(struct Actor *anchor) {
  struct Actor *parts[4];
  struct Actor *part;
  struct LayeredSprite *first;
  struct LayeredSprite *sprite;
  u32 i;

  parts[0] = ((struct Actor *)Import_ActorCreate(
      SPRITE_STEAM_VENT, anchor->x.fixed + toFIXED(1),
      anchor->y.fixed + toFIXED(32), anchor->z.fixed));
  i = 1;
next:
  first = parts[0]->sprite;
  part = ((struct Actor *)Import_ActorCreate(SPRITE_STEAM_VENT, 0, 0, 0));
  parts[i] = part;
  sprite = part->sprite;
  sprite->slot = first->slot;
  sprite->retainCacheEntry = 1;
  part->x.fixed = parts[0]->x.fixed;
  part->y.fixed = parts[0]->y.fixed;
  part->z.fixed = parts[0]->z.fixed;
  part->motionFlags = 0;
  Actor_SetSpriteFlags(part, 0);
  Actor_SelectAnimation(parts[i], 3);
  AttachPart(parts[i], anchor, i - 1);
  if (++i != 4)
    goto next;
  part = parts[0];
  part->motionFlags = 0;
  Actor_SetSpriteFlags(part, 0);
  Actor_SelectAnimation(parts[0], 3);
  Scene_SetActorSpritePriority(parts[0], 3);
  AttachPart(parts[0], anchor, 3);
}

void MagmaRock_RebuildFlameParts(u16 id) {
  if (Actor_Get(id)->update != MagmaRock_RaiseFlameTop)
    return;
  BuildParts(Actor_Get(id)->watchedActor);
}

void MagmaRock_SettleBlocks();
/// The overlay's import veneer to Rope_PlaceFar.
void MagmaRock_PlaceRope();

s32 Scene_Initialize(void) {
  struct MainRoutineWork *work;
  s32 scene;
  s32 *owner, *leader;
  s16 *entrance;
  struct Actor *actor;
  // other serves in turn as a table, a step and a size.
  FIXED other, rise;
  FIXED offset;
  s32 priorityBit, destinationX, destinationColumn, width;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  work = &gMainRoutineWork;
  work->retreatScene = SCENE_MAGMA_ROCK_SLIDE;
  work->retreatEntrance = 1;
  scene = work->next;
  if (scene == SCENE_MAGMA_ROCK_SLIDE) {
    priorityBit = 32;
    Actor_GetById(work->leaderActorId)->priorityFlags |= priorityBit;
    Actor_GetById(ACTOR_SLIDE_PILLAR_A)->priorityFlags |= priorityBit;
    Actor_GetById(ACTOR_SLIDE_PILLAR_B)->priorityFlags |= priorityBit;
    MagmaRockInside_RaiseActorCells(gMagmaRock_SlideStatues);
    if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
      MagmaRock_PlaceVolley(ACTOR_SLIDE_MAGMA_ROCK,
                            ACTOR_SLIDE_PAIR_EAST_STATUE, 0);
    if (Event_GetFlag(FLAG_SLIDE_PILLAR_A_PUSHED))
      Actor_SetPosition(ACTOR_SLIDE_PILLAR_A, 264, 360);
    if (Event_GetFlag(FLAG_SLIDE_PILLAR_B_PUSHED))
      Actor_SetPosition(ACTOR_SLIDE_PILLAR_B, 248, 408);
    other = (s32)gMagmaRock_SlidePillars;
    destinationX = 78;
    Actor_PlaceBlocks(other);
    MapGrid_CopyCellAttributes(98, 21, 4, 3, destinationX, 21);
    MapGrid_CopyCellAttributes(98, 27, 4, 3, destinationX, 24);
    Blocks_MarkCells(other);
    if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
      actor = Actor_GetById(ACTOR_SLIDE_STATUE_FLAME);
      actor->x.fixed += toFIXED(1);
      actor->motionFlags = 0;
      actor->collisionFlags = 0;
      actor->groundHeight = actor->y.fixed += toFIXED(24);
      Actor_SetSpriteFlags(actor, 0);
    }
    MagmaRock_RebuildFlameParts(ACTOR_SLIDE_STATUE_FLAME);
    switch (work->entrance) {
    case 3:
    case 4:
      offset = toFIXED(16);
      goto move;
    }
    goto settle;
  } else if (scene == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
    owner = &work->leaderActorId;
    Actor_GetById(*owner)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
    entrance = &work->entrance;
    if (work->entrance <= 3) {
      s32 id;

      MagmaRock_RestoreCrumblingPillars(gMagmaRock_CrumblingPillars);
      MagmaRockInside_RaiseActorCells(gMagmaRock_SalamanderTailLowerStatues);
      Tightrope_Initialize();
      MagmaRock_PlaceTightrope(0, ACTOR_LOWER_TIGHTROPE_WEST_POST,
                               ACTOR_LOWER_TIGHTROPE_EAST_POST);
      if (Event_GetFlag(FLAG_LOWER_PILLAR_PUSHED))
        Actor_SetPosition(ACTOR_LOWER_PILLAR, 440, 536);
      Actor_PlaceBlocks(gMagmaRock_SalamanderTailLowerPillars);
      id = *entrance;
      if (id == 1) {
        if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
          actor = Actor_GetById(*owner);
          actor->z.fixed += toFIXED(16);
        }
        Leader_Lift(-0x100000);
      } else if (id == 2) {
        if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
          actor = Actor_GetById(*owner);
          actor->z.fixed += toFIXED(16);
        }
        Leader_Lift(-0x600000);
      }
    } else {
      if (*entrance <= 8) {
        s32 id;

        if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
          Actor_GetById(ACTOR_STATUE_PAIR_A_EAST)->motionFlags = 0;
          rise = 0x500000;
          Actor_GetById(ACTOR_STATUE_PAIR_A_EAST)->y.fixed += rise;
          Actor_GetById(ACTOR_STATUE_PAIR_A_WEST)->motionFlags = 0;
          Actor_GetById(ACTOR_STATUE_PAIR_A_WEST)->y.fixed += rise;
          MagmaRock_PlaceVolley(ACTOR_MAGMA_ROCK_A, ACTOR_STATUE_PAIR_A_EAST,
                                0);
          Actor_GetById(ACTOR_STATUE_PAIR_B_EAST)->motionFlags = 0;
          rise = 0xa00000;
          Actor_GetById(ACTOR_STATUE_PAIR_B_EAST)->y.fixed += rise;
          Actor_GetById(ACTOR_STATUE_PAIR_B_WEST)->motionFlags = 0;
          Actor_GetById(ACTOR_STATUE_PAIR_B_WEST)->y.fixed += rise;
          MagmaRock_PlaceVolley(ACTOR_MAGMA_ROCK_B, ACTOR_STATUE_PAIR_B_EAST,
                                1);
          Actor_GetById(ACTOR_STATUE_PAIR_C_EAST)->motionFlags = 0;
          rise = 0xf00000;
          Actor_GetById(ACTOR_STATUE_PAIR_C_EAST)->y.fixed += rise;
          Actor_GetById(ACTOR_STATUE_PAIR_C_WEST)->motionFlags = 0;
          Actor_GetById(ACTOR_STATUE_PAIR_C_WEST)->y.fixed += rise;
          MagmaRock_PlaceVolley(ACTOR_MAGMA_ROCK_C, ACTOR_STATUE_PAIR_C_EAST,
                                0);
        }
        if (Event_GetFlag(FLAG_MIDDLE_PILLAR_PUSHED))
          Actor_SetPosition(ACTOR_MIDDLE_PILLAR, 888, 568);
        Actor_PlaceBlocks(gMagmaRock_SalamanderTailMiddlePillars);
        MagmaRockInside_RaiseActorCells(gMagmaRock_SalamanderTailMiddleStatues);
        if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
          actor = Actor_GetById(ACTOR_MIDDLE_FLAME_A);
          other = 0x10000;
          actor->x.fixed += other;
          actor->motionFlags = 0;
          actor->collisionFlags = 0;
          rise = 0x180000;
          actor->groundHeight = actor->y.fixed += rise;
          Actor_SetSpriteFlags(actor, 0);
          actor = Actor_GetById(ACTOR_MIDDLE_FLAME_B);
          actor->x.fixed += other;
          actor->motionFlags = 0;
          actor->collisionFlags = 0;
          actor->groundHeight = actor->y.fixed += rise;
          Actor_SetSpriteFlags(actor, 0);
        }
        MagmaRock_RebuildFlameParts(ACTOR_MIDDLE_FLAME_A);
        MagmaRock_RebuildFlameParts(ACTOR_MIDDLE_FLAME_B);
        id = *entrance;
        if (id == 5) {
          Leader_Lift(0x1200000);
        } else if (id == 6) {
          if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
            MagmaRock_LaunchLeader();
        } else if (id == 7) {
          if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
            actor = Actor_GetById(*owner);
            actor->z.fixed += toFIXED(16);
          }
          Leader_Lift(-0x300000);
        } else if (id == 8) {
          offset = toFIXED(104);
          goto move;
        }
      } else if (*entrance <= 12) {
        if (Event_GetFlag(FLAG_UPPER_PILLAR_PUSHED))
          Actor_SetPosition(ACTOR_UPPER_PILLAR, 264, 584);
        rise = (s32)gMagmaRock_SalamanderTailUpperPillars;
        Actor_PlaceBlocks(rise);
        Blocks_MarkCells(rise);
        Rope_Initialize();
        Call4((void (*)())MagmaRock_PlaceRope, 0, 0x250,
              ACTOR_UPPER_ROPE_POST_B, ACTOR_UPPER_ROPE_POST_A);
        MagmaRockInside_RaiseActorCells(gMagmaRock_SalamanderTailUpperStatues);
        if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
          actor = Actor_GetById(ACTOR_UPPER_FLAME_A);
          other = 0x10000;
          actor->x.fixed += other;
          actor->motionFlags = 0;
          actor->collisionFlags = 0;
          rise = 0x180000;
          actor->groundHeight = actor->y.fixed += rise;
          Actor_SetSpriteFlags(actor, 0);
          actor = Actor_GetById(ACTOR_UPPER_FLAME_B);
          actor->x.fixed += other;
          actor->motionFlags = 0;
          actor->collisionFlags = 0;
          actor->groundHeight = actor->y.fixed += rise;
          Actor_SetSpriteFlags(actor, 0);
        }
        MagmaRock_RebuildFlameParts(ACTOR_UPPER_FLAME_A);
        MagmaRock_RebuildFlameParts(ACTOR_UPPER_FLAME_B);
        if (*entrance == 9 || *entrance == 10) {
          offset = toFIXED(48);
          goto move;
        }
      } else {
        MagmaRockInside_RaiseActorCells(gMagmaRock_SalamanderTailTopStatues);
        if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
          actor = Actor_GetById(ACTOR_TOP_FLAME);
          actor->x.fixed += toFIXED(1);
          actor->motionFlags = 0;
          actor->collisionFlags = 0;
          actor->groundHeight = actor->y.fixed += toFIXED(24);
          Actor_SetSpriteFlags(actor, 0);
        }
        MagmaRock_RebuildFlameParts(ACTOR_TOP_FLAME);
        if (Event_GetFlag(FLAG_TOP_PILLAR_A_PUSHED))
          Actor_SetPosition(ACTOR_TOP_PILLAR_A, 648, 600);
        if (Event_GetFlag(FLAG_TOP_PILLAR_B_PUSHED))
          Actor_SetPosition(ACTOR_TOP_PILLAR_B, 568, 616);
        MagmaRock_SettleBlocks(gMagmaRock_SalamanderTailTopPillars);
        if (Event_GetFlag(FLAG_SCENE_TEMP_50))
          MapGrid_CopyCellAttributes(121, 48, 1, 3, 123, 48);
        if (Event_GetFlag(FLAG_SCENE_TEMP_51))
          MapGrid_CopyCellAttributes(121, 48, 1, 3, 125, 48);
        if (Event_GetFlag(FLAG_SCENE_TEMP_52))
          MapGrid_CopyCellAttributes(108, 56, 1, 3, 109, 56);
        if (*entrance == 14) {
          Leader_Lift(0x300000);
        } else if (*entrance == 16 || *entrance == 17) {
          if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
            actor = Actor_GetById(work->leaderActorId);
            actor->z.fixed += toFIXED(16);
          }
          offset = -toFIXED(32);
        move:
          Leader_Lift(offset);
        }
      }
    settle:
      MagmaRock_LandLeader();
    }
  } else if (scene == SCENE_MAGMA_ROCK_STATUE) {
    if (Event_GetFlag(FLAG_WAY_IN_BLASTED)) {
      Actor_SetPosition(8, 0, 0);
      destinationColumn = 42;
      MapGrid_CopyCellAttributes(0, 32, 5, 8, destinationColumn, 17);
      MapGrid_CopyCellAttributes(0, 96, 5, 8, destinationColumn, 81);
      width = 5;
      other = 8;
      MapGrid_CopyCellsTo(0, 32, 42, 17, width, other);
      MapGrid_CopyCellsTo(0, 96, 42, 81, width, other);
    }
    leader = &work->leaderActorId;
    Actor_GetById(*leader)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
    MagmaRockInside_RaiseActorCells(gMagmaRock_StatueStatues);
    if (work->entrance <= 2) {
      if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
        actor = Actor_GetById(*leader);
        actor->z.fixed += toFIXED(16);
      }
      Leader_Lift(-0x200000);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

// On entering, sets each listed pillar from its two flags, then lifts the
// leader onto the ground under it.

extern u8 gMapShapeGrid[];

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);

void MagmaRock_RestoreCrumblingPillars(const u16 *table) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *object;
  s32 id;
  s32 flag;
  s32 nextFlag;
  struct MapCell *cells;
  u8 *shapes;
  struct MapCell *cell;
  s32 index;
  struct Actor *leader;
  s32 ground;

  if (*table == 0xffff)
    goto done;
next:
  id = table[0];
  object = Actor_Get(id);
  flag = table[1];
  nextFlag = flag + 1;
  cells = map->layers[object->layer].cells;
  shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);
  if (Event_GetFlag(nextFlag)) {
    Actor_SetPosition(id, 0, 0);
  } else {
    s32 shape;
    u8 *slot;
    s32 height;

    index = Actor_GetCellIndex(object);
    cell = &cells[index];
    slot = shapes + index;
    shape =
        MapGrid_GetTerrainKind(object->layer, object->x.fixed, object->z.fixed);
    height =
        Map_GetTerrainHeight(object->layer, object->x.fixed, object->z.fixed);
    Actor_DisableById(id);
    cell->attributeB = 128;
    *slot = MapCollision_FindOrAddTag(object->layer, shape, (height >> 19) + 4);
    if (Event_GetFlag(flag))
      Actor_SelectAnimation(object, 0);
  }
  table += 2;
  if (*table != 0xffff)
    goto next;
done:
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  ground =
      Map_GetTerrainHeight(leader->layer, leader->x.fixed, leader->z.fixed);
  if (leader->y.fixed < ground) {
    leader->groundHeight = ground;
    leader->y.fixed = ground;
  }
}

void MagmaRock_SpawnDustRing(void);

void MagmaRock_StepOnCrumblingPillar(u16 *list) {
  struct Actor *leader;
  struct Actor *target;
  FIXED position[3];

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  target = Actor_FindAtCell(position, 0);
  if (target != NULL)
    goto test;
  return;
body:
  {
    s32 id = list[0];

    if (Actor_GetById(id) != target) {
      list += 2;
      goto test;
    }
    {
      s32 flag = list[1];

      Event_Start();
      if (!Event_GetFlag(flag)) {
        Event_PlaySound(SE_CHEST_CLOSE);
        FieldActor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_WHITE);
        wait(2);
        Actor_SelectAnimation(target, 0);
        FieldActor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_NORMAL);
        wait(2);
        FieldActor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_WHITE);
        wait(4);
        FieldActor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_NORMAL);
        MagmaRock_SpawnRubble(0);
        Event_SetFlag(flag);
      } else if (!Event_GetFlag(flag + 1)) {
        leader->motionFlags = 0;
        Event_PlaySound(SE_PUSH);
        MapGrid_SetShake(1, 4, 1);
        MagmaRock_SpawnRubble(0);
        wait(8);
        Actor_SelectAnimation(target, 2);
        MagmaRock_SetLeaderCellShape(-2, target->layer);
        MagmaRock_SpawnRubble(1);
        wait(16);
        MagmaRock_SetLeaderCellShape(-2, target->layer);
        wait(4);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        wait(8);
        leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
        wait(5);
        Actor_SetPositionAndResetMotion(target, 0, 0, 0);
        wait(2);
        Event_PlaySound(SE_THUMP);
        MagmaRock_SpawnDustRing();
        wait(20);
        Event_SetFlag(flag + 1);
      }
    }
    MagmaRock_EndEvent();
    return;
  }
test:
  if (list[0] != 0xffff)
    goto body;
}

void MagmaRock_CrumblePillar(struct MagmaRockCrumblingPillar *entry, s32 id) {
  struct Actor *target;
  s32 flag;
  s32 nextFlag;

  target = Actor_Get(id);
  if (entry->id == 0xffff)
    return;
body:
  if (entry->id != id) {
    entry++;
    goto test;
  }
  flag = entry->flag;
  Event_Start();
  nextFlag = flag + 1;
  if (Event_GetFlag(nextFlag) == 0) {
    Event_PlaySound(SE_PUSH);
    MapGrid_SetShake(1, 4, 1);
    MagmaRock_SpawnRubble(0);
    System_Wait(8);
    Actor_SelectAnimation(target, 2);
    MagmaRock_SetActorCellShape(-4, target->layer, id);
    MagmaRock_SpawnRubble(1);
    System_Wait(16);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    System_Wait(8);
    FieldActor_SetPositionAndResetMotion(target, 0, 0, 0);
    System_Wait(2);
    Event_PlaySound(SE_THUMP);
    MagmaRock_SpawnDustRing();
    System_Wait(20);
    Event_SetFlag(flag);
    Event_SetFlag(nextFlag);
  }
  MagmaRock_EndEvent();
  return;
test:
  if (entry->id != 0xffff)
    goto body;
}

void MagmaRockInside_SetPillarMode3(s32 id) {
  Actor_Get(id)->retainSpriteCache = 3;
  Actor_DisableById(id);
}

// Places the listed actors on their cells.

void Actor_MoveToObject(s32 id, s32 owner);

// Places actors 8, 9 and 10, then walks a 0xffff-ended list of (actor, flag)
// halfword pairs: flag + 1 set parks the actor at (0, 0); otherwise its cell
// gets attributeB 128 and its shape-grid byte is rewritten, and flag set makes
// it copy actor 9's visual attributes.
#include "shared/pillars_restore.h"

void MagmaRock_TakePlaceOf(struct Actor *object, struct Actor *source) {
  FieldActor_SetPositionAndResetMotion(object, source->x.fixed, source->y.fixed,
                                       source->z.fixed);
  FieldActor_SetPositionAndResetMotion(source, 0, 0, 0);
}

void MagmaRock_StepOnTrap(u16 *list) {
  struct Actor *leader;
  struct Actor *actor8;
  struct Actor *target;
  FIXED position[3];

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  actor8 = Actor_GetById(8);
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  target = Actor_FindAtCell(position, 0);
  if (target == 0)
    return;
  while (*list != 0xffff) {
    u32 id = list[0];

    if (Actor_GetById(id) != target) {
      list += 2;
      continue;
    }
    {
      s32 flag = list[1];

      Event_Start();
      if (!Event_GetFlag(flag)) {
        Actor_SelectAnimation(actor8, 1);
        MagmaRock_TakePlaceOf(actor8, target);
        wait(1);
        Event_PlaySound(SE_CHEST_CLOSE);
        FieldActor_SetSpriteColor(Actor_GetById(8), SPRITE_COLOR_WHITE);
        wait(2);
        Actor_SelectAnimation(actor8, 0);
        Actor_CopyGraphics(id, 9);
        FieldActor_SetSpriteColor(Actor_GetById(8), SPRITE_COLOR_NORMAL);
        wait(2);
        FieldActor_SetSpriteColor(Actor_GetById(8), SPRITE_COLOR_WHITE);
        wait(4);
        FieldActor_SetSpriteColor(Actor_GetById(8), SPRITE_COLOR_NORMAL);
        MagmaRock_SpawnRubble(0);
        MagmaRock_TakePlaceOf(target, actor8);
        wait(1);
        Event_SetFlag(flag);
      } else if (!Event_GetFlag(flag + 1)) {
        u8 *flags;

        Actor_SelectAnimation(actor8, 0);
        MagmaRock_TakePlaceOf(actor8, target);
        wait(1);
        flags = &leader->motionFlags;
        *flags = 0;
        Event_PlaySound(SE_PUSH);
        MapGrid_SetShake(1, 4, 1);
        MagmaRock_SpawnRubble(0);
        wait(8);
        Actor_SelectAnimation(actor8, 2);
        MagmaRock_SetLeaderCellShape(-2, target->layer);
        MagmaRock_SpawnRubble(1);
        wait(16);
        MagmaRock_SetLeaderCellShape(-2, target->layer);
        wait(4);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        wait(8);
        *flags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(actor8, 0, 0, 0);
        wait(2);
        Event_PlaySound(SE_THUMP);
        MagmaRock_SpawnDustRing();
        wait(20);
        Event_SetFlag(flag + 1);
      }
      MagmaRock_EndEvent();
      return;
    }
  }
}

#include "../field/field_effect_advance_motion_priority.c"

// Magma Rock's rubble and falling rocks, and its tables.

void Actor_StartScript(struct Actor *object, const s32 *script);

void MagmaRock_SpawnRubble(s32 spread) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;
  FIXED y;

  do {
    y = leader->y.fixed - ((Random_Below(14)) << 16);
    effect = (struct FieldEffect *)FieldActor_Create(
        SPRITE_DUST_CLOUD, (i << 17) + leader->x.fixed - toFIXED(8),
        y - toFIXED(2), leader->z.fixed);
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags(effect, 0);
      Actor_SelectAnimation(effect, 2);
      Actor_StartScript(effect, gMagmaRock_RubbleScript);
      effect->scaleY = effect->scaleX = FIXED_ONE * 7 / 10;
      effect->sprite->priority = 1;
      if (spread != 0) {
        effect->velocity.x = Iwram_MulFX(Trig_Cos(i << 13), 0x13333);
        effect->velocity.z = Math_Sin(i << 13);
      } else {
        effect->velocity.x = spread;
        effect->velocity.z = spread;
      }
      effect->velocity.y = -(((Random16() << 14) >> 16) + 0x6000);
      effect->spin = ((Random16() << 9) >> 16) - 0x100;
      effect->scaleRateX = -FIXED_ONE / 32;
      effect->scaleRateY = -0x600;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
    }
    i++;
  } while (i <= 7);
}

void MagmaRock_SpawnDustRing(void) {
  struct Actor *leader;
  struct FieldEffect *effect;
  s32 state;
  u32 i;
  s32 angle;

  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  if (leader == 0)
    return;
  state = 0;
  for (i = 0; i <= 16; i++) {
    effect = (struct FieldEffect *)FieldActor_Create(
        SPRITE_DUST_CLOUD, leader->x.fixed, leader->y.fixed, leader->z.fixed);
    if (effect != 0) {
      state = Sprite_ShareVramCacheEntry(effect->sprite, state);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags((struct Actor *)effect, 0);
      Actor_SelectAnimation(effect, 2);
      Actor_StartScript(effect, gMagmaRock_DustScript);
      angle = i << 12;
      effect->velocity.x = Trig_Cos(angle);
      effect->velocity.y = 0;
      effect->velocity.z = Math_Sin(angle);
      effect->velocity.x += effect->velocity.x >> 1;
      effect->velocity.x += (Random_Below(0xc000)) - 0x6000;
      effect->velocity.z += (Random_Below(0x6000)) - 0x3000;
      effect->spin = (Random_Below(0x1000)) - 0x800;
      effect->scaleRateX = 0;
      effect->scaleRateY = 0;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
      effect->sprite->priority =
          ((struct FieldSprite *)leader->sprite)->priority;
    }
  }
}

s32 MapGrid_GetTerrainKind(s32 terrainId, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

/// Actor_SetCellShape as Izumo's ruins and Magma Rock compile it.
void MagmaRock_SetActorCellShape(s32 heightOffset, s32 layer, s32 id) {
  struct Actor *actor = Actor_Get(id);
  s32 cell = Ram_HeapSlots->mapGrid->layers[layer].cells -
             (struct MapCell *)Ram_MapCellBuffer;
  FIXED z = actor->z.fixed;
  u8 *shapes = gMapShapeGrid + cell;
  s32 value;
  s32 height;

  value = MapGrid_GetTerrainKind(actor->layer, actor->x.fixed, z);
  height =
      Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed) >> 19;
  height += heightOffset;
  shapes += actor->x.fixed / toFIXED(16) + (actor->z.fixed / toFIXED(16) << 7);
  *shapes = MapCollision_FindOrAddTag(actor->layer, value, height);
}

void MagmaRock_SetLeaderCellShape(s32 a, s32 b) {
  MagmaRock_SetActorCellShape(a, b, gMainRoutineWork.leaderActorId);
}

void BattleEffect_StepVector3(struct Particle *particle, s32 unk1, s32 step);

#include "shared/particles_update_falling.h"

struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct EffectSprite sprites[64];
  s16 next;
  u16 tileBase;
  s16 resource;
};

void MagmaRock_DropRocks(s32 x, s32 y, s32 z, s32 count) {
  struct RockWork *work;
  s32 i;

  work = Ram_HeapSlots->menuWork;
  if (count > 0) {
    i = count;
  next:
    {
      struct Rock *rock;
      s32 angle = Random16();

      rock = &work->rocks[work->next];
      rock->timer = 0;
      rock->position[0] = x;
      rock->position[1] = y;
      rock->position[2] = z;
      FieldVector_AddPolarOffsetFX(Random16() * 8 + toFIXED(8), angle,
                                   rock->position);
      rock->velocity[0] = 0;
      rock->velocity[1] = toFIXED(5);
      rock->velocity[2] = 0;
      FieldVector_AddPolarOffsetFX(Random16() * 2 + toFIXED(2), angle,
                                   rock->velocity);
      {
        s32 mask = 63;

        work->next = (work->next + 1) & mask;
      }
      i--;
    }
    if (i != 0)
      goto next;
  }
}

// The falling rocks' setup: loads their picture, sets up 64 small sprites,
// marks each rock idle and starts the task that spawns them.

void Import_DecompressTaggedLz(void *entry, void *destination);

void MagmaRock_StartFallingRocks(void) {
  struct RockWork *work =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct RockWork));
  s32 entry;
  s32 tile;
  s32 i;
  struct Rock *rock;
  struct EffectSprite *sprite;

  Import_DecompressTaggedLz(Resource_GetTableEntry(RESOURCE_FALLING_ROCK),
                            work);
  entry = VramCache_FindFreeEntry();
  tile = Vram_Load(entry, 0x500, work);
  work->tileBase = tile;
  work->resource = entry;
  rock = work->rocks;
  sprite = work->sprites;
  for (i = 63; i >= 0; i--, sprite++, rock++) {
    EffectSprite_Init(sprite, 8, 8, 0x40000000, tile);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = SPRITE_COLOR_NORMAL;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    rock->timer = -1;
  }
  {
    s32 zero = 0;

    work->next = zero;
  }
  Task_AddFrame(Particles_UpdateFalling, TASK_PASS_VBLANK);
}

void MagmaRock_EndFallingRocks(void) {
  struct RockWork *work = Ram_HeapSlots->menuWork;

  FieldTask_Remove(Particles_UpdateFalling);
  VramCache_FreeEntry(work->resource);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

void MagmaRock_UnblockMovedPillar(void);

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

u32 gMagmaRock_EmberScript[] = {
    SC_ANIMATION(2),
    SC_WAIT(18),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

s32 gMagmaRock_FlameEmberScript[] = {
    SC_ANIMATION(3),
    SC_WAIT(23),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

s16 gMagmaRock_SlidePillars[] = {
    8, 10, -1,
};

s16 gMagmaRock_SlideStatues[] = {
    9, 50, 20, 12, 30, -1, -1,
};

s16 gMagmaRock_SalamanderTailLowerPillars[] = {
    8, 15, -1,
};

s16 gMagmaRock_SalamanderTailLowerStatues[] = {
    13, 30, -1, -1,
};

s16 gMagmaRock_CrumblingPillars[] = {
    9, 512, 10, 514, -1,
};

s16 gMagmaRock_SalamanderTailMiddleStatues[] = {
    14, 50, 15, 17, 51, 18, -1,
};

s16 gMagmaRock_SalamanderTailMiddlePillars[] = {
    16, -1,
};

s16 gMagmaRock_SalamanderTailUpperStatues[] = {
    9, 50, 10, 11, 51, 12, -1,
};

s16 gMagmaRock_SalamanderTailUpperPillars[] = {
    8, -1,
};

s16 gMagmaRock_SalamanderTailTopPillars[] = {
    8, 9, -1,
};

s16 gMagmaRock_SalamanderTailTopStatues[] = {
    11, 30, -1, 12, 50, 13, 15, 31,
    -1, -1,
};

s16 gMagmaRock_StatueStatues[] = {
    9, 50, -1, -1, 0,
};

u32 gMagmaRock_RubbleScript[] = {                                                               
      SC_WAIT(33), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),       
      SC_RELEASE,                                                                 
  };

u32 gMagmaRock_DustScript[] = {                                                               
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

s16 gMagmaRock_Regions[] = {
    240, 328, 352, 256, 344, 368, -1, 3,
    448, 328, 352, 464, 344, 368, -1, 4,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

s16 gMagmaRock_SalamanderTailRegions[] = {
    272, 56, 576, 288, 72, 592, -1, 1,
    352, 40, 560, 368, 56, 576, -1, 2,
    752, 512, 536, 768, 528, 552, -1, 5,
    784, 80, 592, 800, 96, 608, -1, 7,
    896, 512, 528, 912, 528, 544, -1, 8,
    272, 48, 576, 288, 64, 592, -1, 9,
    352, 32, 560, 368, 48, 576, -1, 10,
    784, 56, 592, 800, 72, 608, -1, 14,
    752, 0xfea8, 672, 768, 0xfeb8, 688, -1, 16,
    976, 0xfea8, 672, 992, 0xfeb8, 688, -1, 17,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

s16 gMagmaRock_StatueRegions[] = {
    240, -56, 448, 256, -40, 464, -1, 1,
    384, -56, 448, 400, -40, 464, -1, 2,
    416, -128, 448, 432, -112, 464, -1, 4,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(MagmaRockSlide),
    SCENE_EXIT(MAGMA_ROCK_SLIDE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_MAGMA_ROCK_SLIDE),
    SCENE_EXIT(MAGMA_ROCK_SLIDE_TO_MAGMA_ROCK_INSIDE_EMBER_STONE, MagmaRockInsideEmberStone, MAGMA_ROCK_INSIDE_EMBER_STONE_FROM_MAGMA_ROCK_SLIDE),
    SCENE_EXIT(MAGMA_ROCK_SLIDE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_WEST, MagmaRockSalamanderTailChest, MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_SLIDE_WEST),
    SCENE_EXIT(MAGMA_ROCK_SLIDE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST, MagmaRockSalamanderTailChest, MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_SLIDE_EAST),
    SCENE_EXITS(MagmaRockSalamanderTailChest),
    SCENE_EXIT(1, MagmaRockSalamanderTailChest, 9),
    SCENE_EXIT(2, MagmaRockSalamanderTailChest, 10),
    SCENE_EXIT(3, MagmaRockSalamanderTailChest, 4),
    SCENE_EXIT(4, MagmaRockSalamanderTailChest, 3),
    SCENE_EXIT(MAGMA_ROCK_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_STATUE_WEST, MagmaRockStatue, MAGMA_ROCK_STATUE_FROM_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_WEST),
    SCENE_EXIT(MAGMA_ROCK_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_STATUE_EAST_A, MagmaRockStatue, MAGMA_ROCK_STATUE_FROM_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST_B),
    SCENE_EXIT(7, MagmaRockSalamanderTailChest, 14),
    SCENE_EXIT(MAGMA_ROCK_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_STATUE_EAST_B, MagmaRockStatue, MAGMA_ROCK_STATUE_FROM_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST_B),
    SCENE_EXIT(9, MagmaRockSalamanderTailChest, 1),
    SCENE_EXIT(10, MagmaRockSalamanderTailChest, 2),
    SCENE_EXIT(11, MagmaRockSalamanderTailChest, 13),
    SCENE_EXIT(12, MagmaRockSalamanderTailChest, 15),
    SCENE_EXIT(13, MagmaRockSalamanderTailChest, 11),
    SCENE_EXIT(14, MagmaRockSalamanderTailChest, 7),
    SCENE_EXIT(15, MagmaRockSalamanderTailChest, 12),
    SCENE_EXIT(MAGMA_ROCK_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_SLIDE_WEST, MagmaRockSlide, MAGMA_ROCK_SLIDE_FROM_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_WEST),
    SCENE_EXIT(MAGMA_ROCK_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_SLIDE_EAST, MagmaRockSlide, MAGMA_ROCK_SLIDE_FROM_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST),
    SCENE_EXITS(MagmaRockStatue),
    SCENE_EXIT(MAGMA_ROCK_STATUE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_WEST, MagmaRockSalamanderTailChest, MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_STATUE_WEST),
    SCENE_EXIT(MAGMA_ROCK_STATUE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST_B, MagmaRockSalamanderTailChest, MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_STATUE_EAST_B),
    SCENE_EXIT(MAGMA_ROCK_STATUE_TO_MAGMA_ROCK_INSIDE_ENTRANCE, MagmaRockInsideEntrance, MAGMA_ROCK_INSIDE_ENTRANCE_FROM_MAGMA_ROCK_STATUE),
    SCENE_EXIT(MAGMA_ROCK_STATUE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST_A, MagmaRockSalamanderTailChest, MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_STATUE_EAST_A),
    SCENE_EXITS_END,
};

struct ActorPlacement gMagmaRock_Placements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 2},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, 1, 5, 0, 0, FACING_WEST, 2, 2},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, 7, 6, 0, 0, FACING_EAST, 2, 2},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, 7, 7, 0, 0, FACING_WEST, 2, 2},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, 1, 8, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, 1, 9, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, 1, 10, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, 1, 11, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, 1, 12, 0, 0, FACING_NORTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_NORTH, 2, 2},
    ACTOR_PLACEMENT(SPRITE_FLAME_PUFF, CONDITION_ALWAYS, 7, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRock_SalamanderTailLowerPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_SOUTH, 2, 2},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, 1, 5, 0, 0, FACING_SOUTH, 2, 3},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, 1, 6, 0, 0, FACING_EAST, 2, 2},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, 1, 7, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, 1, 8, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRock_SalamanderTailMiddlePlacements[] = {
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, 7, 9, 0, 0, FACING_EAST, 2, 2},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, 7, 10, 0, 0, FACING_WEST, 2, 2},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, 7, 9, 0, 0, FACING_EAST, 2, 2},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, 7, 10, 0, 0, FACING_WEST, 2, 2},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, 7, 9, 0, 0, FACING_EAST, 2, 2},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, 7, 10, 0, 0, FACING_WEST, 2, 2},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, 1, 11, 0, 0, FACING_SOUTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 11, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, 1, 12, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, 1, 13, 0, 0, FACING_SOUTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 13, 0, 0, FACING_NORTH, 2, 2},
    ACTOR_PLACEMENT(SPRITE_FLAME_PUFF, CONDITION_ALWAYS, 7, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLAME_PUFF, CONDITION_ALWAYS, 7, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLAME_PUFF, CONDITION_ALWAYS, 7, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRock_SalamanderTailUpperPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, 1, 14, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, 1, 15, 0, 0, FACING_SOUTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 15, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, 1, 16, 0, 0, FACING_SOUTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 16, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, 1, 17, 0, 0, FACING_SOUTH, 2, 2},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, 1, 18, 0, 0, FACING_SOUTH, 2, 3},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRock_SalamanderTailTopPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, 1, 19, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, 1, 20, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, 1, 21, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, 7, 22, 0, 0, FACING_WEST, 2, 2},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, 1, 23, 0, 0, FACING_SOUTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 23, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, 1, 24, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, 7, 25, 0, 0, FACING_WEST, 2, 2},
    {SPRITE_SPROUT, CONDITION_ALWAYS, 7, 26, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_SPROUT, CONDITION_ALWAYS, 7, 27, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_SPROUT, CONDITION_ALWAYS, 7, 28, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRock_StatuePlacements[] = {
    {384, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_NORTH, 2, 2},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, 7, 2, 0, 0, FACING_WEST, 2, 2},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gMagmaRock_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0xca02, 2, 0x2f8, EVENT(MagmaRock_SlideDown)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(MagmaRock_ClearSlideFlag)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), 19, CONDITION_ALWAYS, 0},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     FLAG_SLIDE_PILLAR_A_PUSHED, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, FLAG_SLIDE_PILLAR_A_PUSHED,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 10,
     FLAG_SLIDE_PILLAR_B_PUSHED, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, FLAG_SLIDE_PILLAR_B_PUSHED,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 31,
     FLAG_CRACKED_PILLAR_A_BURST, EVENT(MagmaRock_BurstCrackedPillarA)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 32,
     FLAG_CRACKED_PILLAR_B_BURST, EVENT(MagmaRock_BurstCrackedPillarB)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 33,
     FLAG_CRACKED_PILLAR_C_BURST, EVENT(MagmaRock_BurstCrackedPillarC)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 34,
     FLAG_CRACKED_PILLAR_D_BURST, EVENT(MagmaRock_BurstCrackedPillarD)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 35,
     FLAG_CRACKED_PILLAR_E_BURST, EVENT(MagmaRock_BurstCrackedPillarE)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30, 0x2f0,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_BurstStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_FireStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRock_SalamanderTailLowerTriggers[] = {
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {STEP_TRIGGER_WALKING, 20, CONDITION_ALWAYS,
     EVENT(MagmaRock_CheckCrumblingPillars)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15,
     FLAG_LOWER_PILLAR_PUSHED, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, FLAG_LOWER_PILLAR_PUSHED,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 31,
     FLAG_CRACKED_PILLAR_A_BURST, EVENT(MagmaRock_BurstCrackedPillarA)},
    {TRIGGER_STEP, 50, 0x212, EVENT(MagmaRock_SetLedgeBehavior)},
    {TRIGGER_STEP, 51, CONDITION_ALWAYS, EVENT(MagmaRock_ClearLedgeBehavior)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30, 0x2f0,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_BurstStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_FireStatue)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRock_SalamanderTailMiddleTriggers[] = {
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 16,
     FLAG_MIDDLE_PILLAR_PUSHED, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, FLAG_MIDDLE_PILLAR_PUSHED,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 51,
     CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 51,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 51,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(MagmaRock_AmplifyBurst)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRock_SalamanderTailUpperTriggers[] = {
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 40, 0x8,
     EVENT(MagmaRock_PushPillarEast)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     FLAG_UPPER_PILLAR_PUSHED, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, FLAG_UPPER_PILLAR_PUSHED,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 14, 0x250, 0},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 51,
     CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 51,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 51,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRock_SalamanderTailTopTriggers[] = {
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 40, CONDITION_ALWAYS,
     EVENT(MagmaRock_PushPillarWest)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     FLAG_TOP_PILLAR_A_PUSHED, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, FLAG_TOP_PILLAR_A_PUSHED,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 9,
     FLAG_TOP_PILLAR_B_PUSHED, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, FLAG_TOP_PILLAR_B_PUSHED,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30, 0x2f0,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_BurstStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_FireStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 31,
     CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 31, 0x2f0,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 31,
     CONDITION_ALWAYS, EVENT(MagmaRock_BurstStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 31,
     CONDITION_ALWAYS, EVENT(MagmaRock_FireStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 32,
     FLAG_CRACKED_PILLAR_B_BURST, EVENT(MagmaRock_BurstCrackedPillarB)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 16, 0x250, EVENT(MagmaRock_GrowVineA)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 17, 0x251, EVENT(MagmaRock_GrowVineB)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 18, 0x252, EVENT(MagmaRock_GrowVineC)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRock_StatueTriggers[] = {
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50, 0x2f0,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_BlastOpenWall)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_FireStatue)},
    {TRIGGER_TABLE_END},
};
