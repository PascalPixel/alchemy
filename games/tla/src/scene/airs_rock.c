/// @file
/// Air's Rock's slopes, outside the rock in Osenia, in the rooms of this
/// overlay: the Wind Stones Whirlwind strengthens, the walls and ledges of the
/// climb, and the way into the rock itself (airs_rock_inside.c).

#include "system/task.h"
#include "field/field_psynergy_work.h"
#include "types.h"
#include "field/actor.h"
#include "field/event_inline.h"
#include "scene/airs_rock.h"
#include "system/ram_buffer.h"
#include "flags.h"
#include "system/main_routine_work.h"
#include "field/field.h"
#include "field/field_sequence_event.h"
#include "system/frame.h"
#include "field/map_grid.h"
#include "scene/scene.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "field/actor_view.h"
#include "scene/dehkan_plateau.h"

/// Set once Move has left the second room's first pillar on its ledge, cell
/// (16, 43).
#define FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST 0x85f
/// As FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST, the second, cell (45, 45).
#define FLAG_AIRS_ROCK_PILLAR_PLACED_SECOND 0x860
/// As FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST, the third, cell (21, 48).
#define FLAG_AIRS_ROCK_PILLAR_PLACED_THIRD 0x861
/// As FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST, the fourth, cell (39, 49).
#define FLAG_AIRS_ROCK_PILLAR_PLACED_FOURTH 0x862
/// As FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST, the fifth, cell (49, 18).
#define FLAG_AIRS_ROCK_PILLAR_PLACED_FIFTH 0x887
/// Set once Move has pushed the first room's third pillar into place
/// (AirsRock_MoveThirdPillar).
#define FLAG_AIRS_ROCK_THIRD_PILLAR_MOVED 0x879

/// Story flags: the eight barriers the Wind Stones' gusts blow away, in the
/// order of gAirsRock_GustTargets, then the mud blown from the wall, the fog
/// cleared and the way to the top opened.
#define FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN 0x850
#define FLAG_AIRS_ROCK_WALL_CLEARED 0x858
#define FLAG_AIRS_ROCK_FOG_CLEARED 0x87a
#define FLAG_AIRS_ROCK_TOP_OPENED 0x87b
/// The leader rides a statue's whirlwind.
#define FLAG_RIDING_WHIRLWIND FLAG_SCENE_TEMP_50

/// The Storm Brand chest's room, numbering gAirsRock_StormBrandChestPlacements.
enum AirsRockStormBrandChestActorId {
  /// The huge twister-shaped rock Whirlwind clears the wall's mud with.
  ACTOR_TWISTER_ROCK_BY_THE_WALL = ACTOR_FIRST_PLACED,
  /// The five Wind Stones, whose gusts Whirlwind strengthens.
  ACTOR_FIRST_WIND_STONE
};

/// The slopes by the way into the rock, numbering gAirsRock_EntrancePlacements.
enum AirsRockEntranceActorId {
  /// The pillars Move leaves on the ledges, each with its
  /// FLAG_AIRS_ROCK_PILLAR_PLACED_ flag.
  ACTOR_FIRST_LEDGE_PILLAR = ACTOR_FIRST_PLACED + 4,
  ACTOR_SECOND_LEDGE_PILLAR,
  /// The puddle Frost freezes into a pillar.
  ACTOR_PUDDLE,
  ACTOR_THIRD_LEDGE_PILLAR,
  ACTOR_FOURTH_LEDGE_PILLAR,
  /// The statue exhaling little whirlwinds to the left.
  ACTOR_BREATHING_STATUE = ACTOR_FOURTH_LEDGE_PILLAR + 5,
  ACTOR_FIFTH_LEDGE_PILLAR
};

/// The Fujin Shield chest's room, numbering
/// gAirsRock_FujinShieldChestPlacements.
enum AirsRockFujinShieldChestActorId {
  /// The statues whose whirlwinds carry the party up the cliff.
  ///@{
  ACTOR_FIRST_BREATHING_STATUE = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_BREATHING_STATUE,
  ///@}
  /// The pillars AirsRock_MoveFirstPillar and the next two watch.
  ///@{
  ACTOR_FIRST_PILLAR,
  ACTOR_SECOND_PILLAR,
  ///@}
  /// The twister rock whose whirlwind clears the fog.
  ACTOR_TWISTER_ROCK_IN_THE_FOG,
  ACTOR_THIRD_PILLAR,
  /// The twister rock whose whirlwind opens the way to the top.
  ACTOR_TWISTER_ROCK_AT_THE_TOP
};
#include "graphics/resource.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "field/field_event.h"
#include "system/iwram_call.h"
#include "system/io_write.h"
#include "field/field_sprite.h"
#include "system/field_effect.h"
#include "system/vector3.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/input.h"
#include "field/field_sequence_event.h"
#include "field/call.h"
#include "system/bg_scroll.h"
#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_prepare.c"

#include "shared/scene_create_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

s32 AirsRock_ClearScriptedActorSpriteFlags(s32 a0) {
  Actor_SetSpriteFlags(a0, 0);
  return 0;
}

extern u8 gMapGridCellBuffer[];

s32 MapCell_GetAttributes(u32 layer, s32 x, s32 y) {
  struct AirsRockMapWork *map =
      (struct AirsRockMapWork *)Ram_HeapSlots->mapGrid;
  u8 *cell;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapGridCellBuffer;
  cell += (x + (y << 7)) << 2;
  return (cell[2] << 8) | cell[3];
}

s32 MapCell_SetAttributes(u32 layer, s32 x, s32 y, s32 value) {
  struct AirsRockMapWork *map =
      (struct AirsRockMapWork *)Ram_HeapSlots->mapGrid;
  u8 *cell;
  s32 old;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapGridCellBuffer;
  cell += (x + (y << 7)) << 2;
  old = (cell[2] << 8) | cell[3];
  cell[2] = value >> 8;
  cell[3] = value;
  return old;
}

#include "../field/actor_wait_until_idle.c"

#undef EFFECT_SET_SCRIPT

// For each id of a list ended by 0xffff: clears the object's mark and, on a
// cell with attribute bit 6, blocks the cells ahead of and behind it and sets
// the mark.
#include "text/msg_ids.h"

void Actor_PlaceMovedBlock(void);

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
      cell->collisionCode = 255;
      cell = *cells;
      cell += x + ((z - 1) << 7);
      cell->collisionCode = 255;
      *blocked = 1;
    }
    ids++;
  }
}

void Blocks_UnmarkCells(struct Actor *block) {
  struct MapWork *map;

  if (block->riseEnabled != 0) {
    map = Ram_HeapSlots->mapGrid;
    if (map != NULL) {
      struct MapCell **cells = &map->layers[2].cells;
      /* FAKEMATCH(dead): cell's initial value, overwritten unread. */
      struct MapCell *cell = *cells;
      s32 x = FIELD_CELL(block->x.fixed);
      s32 row = FIELD_CELL(block->z.fixed - block->y.fixed);

      cell = *cells;
      cell += x + ((row + 1) << 7);
      cell->collisionCode = 0;
      cell = *cells;
      cell += x + ((row - 1) << 7);
      cell->collisionCode = 0;
      block->riseEnabled = 0;
    }
  }
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern s16 gAirsRock_FujinShieldChestRegions[];

s16 *Scene_GetExitRegions(void) {
  s32 scene = gMainRoutineWork.next;

  if (scene == SCENE_AIRS_ROCK_STORM_BRAND_CHEST)
    return gAirsRock_StormBrandChestRegions;
  if (scene == SCENE_AIRS_ROCK_ENTRANCE)
    return gAirsRock_EntranceRegions;
  return gAirsRock_FujinShieldChestRegions;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s32 scene = gMainRoutineWork.next;

  if (scene == SCENE_AIRS_ROCK_STORM_BRAND_CHEST)
    return gAirsRock_StormBrandChestPlacements;
  if (scene == SCENE_AIRS_ROCK_ENTRANCE)
    return gAirsRock_EntrancePlacements;
  return gAirsRock_FujinShieldChestPlacements;
}

/// Examining a Wind Stone shows its warning.
void AirsRock_ExamineWindStone(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  // "Beware the Wind Stones. The stones magnify the power of the wind...."
  Message_ShowCentered((s32)MsgFieldWindStoneWarning, 1);
  Event_End();
}

s32 TilesetAnim_Resume();

/// Walking into the rock's door: it opens and the leader goes inside.
void AirsRock_EnterTheRock(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_CopyCellsTo(64, 64, 83, 35, 1, 2);
  TilesetAnim_Resume(0);
  FieldEvent_Wait(45);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_WalkTo(gMainRoutineWork.leaderActorId, 312, 680);
  FieldEvent_Wait(5);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Event_PlaySound(SE_MAP_EXIT);
  FieldEvent_SetExitRequest(4);
  Event_End();
}

void AirsRock_MoveFirstPillar(void) {
  struct Actor *object;

  object = Actor_Get(ACTOR_FIRST_PILLAR);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (FIELD_CELL(object->x.fixed) == 0x1f) {
    Event_SetFlag(FLAG_AREA_TEMP_10);
  } else {
    Event_ClearFlag(FLAG_AREA_TEMP_10);
  }
  Event_End();
}

/// Object 11's column sets or clears flag 0x311 and writes the low byte of the
/// cell value MapCell_GetAttributes returns (its high byte 0xff when the object
/// is away) into two places; with the leader 208 pixels up a block copy
/// follows.
void AirsRock_MoveSecondPillar(void) {
  struct Actor *object;
  struct Actor *leader;
  s32 value;

  object = (struct Actor *)((s32)Actor_Get(ACTOR_SECOND_PILLAR));
  value = MapCell_GetAttributes(3, 70, 67);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (FIELD_CELL(object->x.fixed) == 32) {
    Event_SetFlag(FLAG_AREA_TEMP_11);
    value &= 0xff;
    MapCell_SetAttributes(4, 70, 67, value);
    MapCell_SetAttributes(2, 33, 54, value);
  } else {
    Event_ClearFlag(FLAG_AREA_TEMP_11);
    value = (value & 0xff) | 0xff00;
    MapCell_SetAttributes(4, 70, 67, value);
    MapCell_SetAttributes(2, 33, 54, value);
  }
  leader = (struct Actor *)((s32)Actor_Get(gMainRoutineWork.leaderActorId));
  if ((leader->y.fixed >> 19) == 26)
    MapGrid_CopyCellAttributes(70, 67, 1, 1, 33, 54);
  Event_End();
}

void AirsRock_MoveThirdPillar(void) {
  struct Actor *object;

  object = Actor_Get(ACTOR_THIRD_PILLAR);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if ((object->x.fixed >> 19) == 0x17) {
    Event_SetFlag(FLAG_AIRS_ROCK_THIRD_PILLAR_MOVED);
  }
  Event_End();
}

/// The leader jumps up 12 pixels (cue 390, mode 6 then 1), climbs 28 pixels
/// over 14 frames with cue 217 while turned to angle 0x5000, then lands 16
/// pixels on and plays cue 390 again.
void AirsRock_ClimbLedge(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 z = leader->z.fixed;
  u32 i;

  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Actor_SelectAnimation(leader, 6);
  System_Wait(10);
  Actor_SelectAnimation(leader, 1);
  Actor_SetSpriteFlags(leader, 0);
  leader->motionFlags |= ACTOR_FALLS;
  leader->velocityY = px(4);
  FieldActor_SetMoveTargetFixed(leader, leader->x.fixed, leader->y.fixed,
                                z + px(12));
  System_Wait(6);
  Event_PlaySound(SE_FLOOR_SWITCH);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH_SOUTHWEST,
                      0);
  leader->motionFlags = 0;
  for (i = 0; i < 14; i++) {
    leader->y.fixed -= px(2);
    leader->targetY = leader->y.fixed;
    System_Wait(1);
  }
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  leader->velocityY = px(3);
  FieldActor_SetMoveTargetFixed(leader, leader->x.fixed, leader->y.fixed,
                                z + px(16));
  FieldActor_WaitForMove(leader);
  Actor_SetSpriteFlags(leader, 1);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Event_NoOp();
}

const struct Trigger *Scene_GetTriggers(void) {
  s32 scene = gMainRoutineWork.next;

  if (scene == SCENE_AIRS_ROCK_STORM_BRAND_CHEST)
    return gAirsRock_StormBrandChestTriggers;
  if (scene == SCENE_AIRS_ROCK_ENTRANCE)
    return gAirsRock_EntranceTriggers;
  return gAirsRock_FujinShieldChestTriggers;
}

/// Whirlwind on a Wind Stone or the twister rock: the scene's own effect runs
/// in place of Whirlwind's (AirsRock_TakeOverWhirlwind).
void AirsRock_StartWhirlwind(void) { Event_ResolvePendingActions(0); }

void AirsRock_TakeOverWhirlwind(void) {
  ((struct FieldEffectWork *)Ram_HeapSlots->fieldEffectWork)->childOption = 1;
}

/// Circles the object around the centre in words 68 and 76, the radius
/// growing with its height, one step of 256 per lap.
void AirsRock_SpiralGust(struct Actor *object) {
  s32 radius = object->y.fixed;
  s32 step = object->speedLimit;

  radius >>= 1;
  radius += px(16);
  object->x.fixed =
      object->bounce + Iwram_MulQ16(Trig_Cos((step & 255) << 11), radius);
  object->z.fixed =
      object->riseBaseY +
      Iwram_MulQ16(Math_Sin((object->speedLimit & 255) << 11), radius) / 3;
  object->y.fixed += object->gravity;
  object->speedLimit++;
}

/// The same circle on a smaller radius, twice as wide, blinking the part
/// attribute every other frame.
void AirsRock_SpiralSpark(struct Actor *object) {
  s32 radius = object->y.fixed;
  s32 step = object->speedLimit;

  radius >>= 2;
  radius += px(12);
  object->x.fixed =
      object->bounce + Iwram_MulQ16(Trig_Cos((step & 255) << 11), radius) * 2;
  object->z.fixed =
      object->riseBaseY +
      Iwram_MulQ16(Math_Sin((object->speedLimit & 255) << 11), radius);
  object->y.fixed += object->gravity;
  object->speedLimit++;
  FieldActor_SetSpriteColor(object, ((gFrameCount & 2) >> 1) * 9);
}

void AirsRock_FlickerTarget(struct Actor *object) {
  u8 *bytes = (u8 *)object;
  u32 on = ((gFrameCount & 3) >> bytes[99]) & 1;

  FieldActor_SetSpriteColor(object, (u8)(on * bytes[98]));
}

// The Whirlwind gust in Air's Rock.

extern s16 gAirsRock_GustTargets[][4];

/// Whirlwind on a target in Air's Rock: a gust (object type 280) rises from the
/// target and grows, shedding sparks every fourth frame while the target
/// flickers; then it travels in the leader's facing, two cells a step, until
/// MapCell_GetAttributes finds a cell with something on it, throwing leaves as
/// it goes. With nothing there (255) it ends with cue 136; otherwise it stops
/// one and a half cells short and blows the thing found along, copying its
/// cells each fourth frame, and sets that thing's flag
/// (FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN + hit - 1) with cue 298. Then the gust
/// shrinks and rises away over 8 frames.
void AirsRock_BlowGust(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct FieldEffectWork *effect = Ram_HeapSlots->fieldEffectWork;
  struct Actor *gust, *spark;
  struct Vector3 velocity;
  struct EffectOptions leaves, dust;
  struct Cell delta, cell;
  s32 cellZ, cellX, newZ, newX, flag = 0, steps = 0, hit, angle, i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_RUSH);
  gust =
      FieldActor_Create(280, leader->x.fixed, leader->y.fixed, leader->z.fixed);
  Actor_SelectAnimation(gust, 2);
  Actor_SetSpriteFlags(gust, 0);
  gust->scaleX = Q16_ONE * 17 / 10;
  gust->scaleY = Q16_ONE / 5;
  ((struct MotionView *)gust)->flags = flag;
  ((struct FieldSprite *)gust->sprite)->priority =
      ((struct FieldSprite *)effect->child->sprite)->priority;
  Actor_SetMoveTarget(gust, effect->child->x.fixed, effect->child->y.fixed,
                      effect->child->z.fixed - px(1));
  while (gust->scaleY <= 0x14ccb) {
    gust->scaleY += Q16_ONE / 10;
    Event_Wait(1);
  }
  Actor_WaitForMove(gust);
  gust->z.fixed = effect->child->z.fixed - px(1);
  while (gust->scaleY <= 0x1cccb) {
    if ((gFrameCount & 3) == 0) {
      spark = (struct Actor *)Import_ActorCreate(0x2a0, effect->child->x.fixed,
                                                 0, effect->child->z.fixed);
      spark->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      spark->motionFlags = 0;
      spark->bounce = spark->x.fixed;
      spark->gravity = px(4) / 5;
      spark->riseBaseY = spark->z.fixed;
      spark->speedLimit = Random16() & 0xffff;
      Actor_SelectAnimation(spark, 7);
      Actor_StartScript(spark, gFieldEffect_MediumScript);
      Actor_SetSpriteFlags(spark, 0);
      Actor_SetSpriteColor(spark, 2);
      spark->update = AirsRock_SpiralGust;
    }
    if (gFrameCount & 1) {
      gust->scaleX += 655;
      gust->scaleY += 655;
      Actor_SetSpriteColor(effect->child, 15);
    } else {
      Actor_SetSpriteColor(effect->child, 4);
    }
    Event_Wait(1);
  }
  Actor_SetSpriteColor(effect->child, 0);
  Sound_Play(SE_WHOOSH);
  cell.x = FIELD_CELL(effect->child->x.fixed);
  cell.z = FIELD_CELL(effect->child->z.fixed);
  angle = (leader->facing + 0x2000) & 0xc000;
  delta.x = Trig_Cos(angle) >> 16;
  delta.z = Trig_Sin(angle) >> 16;
  do {
    hit = MapCell_GetAttributes(2, cell.x, cell.z);
    hit >>= 8;
    cell.x += delta.x;
    cell.z += delta.z;
    steps += 2;
  } while (hit == 0);
  flag = MapCell_GetAttributes(2, cell.x, cell.z);
  flag >>= 8;
  flag -= 1;
  steps -= 2;
  newX = effect->child->x.part[0].pixel + ((steps * delta.x) << 3);
  newZ = effect->child->z.part[0].pixel + ((steps * delta.z) << 3);
  ((struct FieldSprite *)gust->sprite)->priority = 1;
  cell.x = newX;
  cell.z = newZ;
  gust->speedLimit = px(4);
  gust->acceleration = px(4);
  Actor_SetMoveTarget(gust, px(cell.x), gust->y.fixed, px(cell.z));
  FieldCamera_SetSpeedAndAcceleration(5, 5);
  FieldCamera_MoveToFixed((s32)px(newX), (s32)(-1), (s32)px(newZ), (s32)(1));
  while (Actor_HasArrived(gust) == 0) {
    if ((gFrameCount & 1) == 0) {
      leaves.priority = 1;
      leaves.type = 674;
      leaves.palette = 2;
      velocity.x = Trig_Cos((gFrameCount & 0x1ff) << 12) << 1;
      velocity.y = gust->y.fixed + px(Random16() & 31);
      velocity.z = Trig_Sin((gFrameCount & 0x1ff) << 12);
      FieldEffect_Spawn(
          gust->x.fixed, velocity.y, gust->z.fixed, velocity.x, 0, velocity.z,
          EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &leaves);
    }
    Event_Wait(1);
  }
  if (hit == 255) {
    Sound_Play(SE_WHOOSH);
  } else {
    cell.x = gust->x.part[0].pixel + delta.x * 48;
    cell.z = gust->z.part[0].pixel + delta.z * 48;
    gust->speedLimit = px(1) / 2;
    gust->acceleration = px(1) / 2;
    Actor_SetMoveTarget(gust, px(cell.x), gust->y.fixed, px(cell.z));
    Sound_Play(SE_GUST);
    hit -= 30;
    while (Actor_HasArrived(gust) == 0) {
      if ((gFrameCount & 1) == 0) {
        dust.priority = 1;
        dust.startScaleX = dust.startScaleY = (Random16() & 0xffff) + Q16_ONE;
        velocity.x = Trig_Cos((gFrameCount & 0x1ff) << 12) << 1;
        velocity.y = px(1);
        velocity.z = Trig_Sin((gFrameCount & 0x1ff) << 12);
        FieldEffect_Spawn(gust->x.fixed, gust->y.fixed, gust->z.fixed,
                          velocity.x, velocity.y, velocity.z,
                          EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY, &dust);
      }
      if ((gFrameCount & 3) == 0) {
        s32 sourceX;
        cell.x = gAirsRock_GustTargets[hit][2] + FIELD_CELL(gust->x.fixed);
        cell.z = gAirsRock_GustTargets[hit][3] + FIELD_CELL(gust->z.fixed);
        cellX = cell.x;
        cellZ = cell.z;
        if (hit & 1) {
          sourceX = (u16)gAirsRock_GustTargets[hit][0];
          MapGrid_CopyCellsTo((s16)sourceX, gAirsRock_GustTargets[hit][1],
                              cellX, cellZ - 1, 1, 3);
          MapGrid_CopyCellAttributes(gAirsRock_GustTargets[hit][0],
                                     gAirsRock_GustTargets[hit][1], 1, 2, cellX,
                                     cellZ);
          MapGrid_CopyCellAttributes(
              gAirsRock_GustTargets[hit][0], gAirsRock_GustTargets[hit][1], 1,
              2, FIELD_CELL(gust->x.fixed), FIELD_CELL(gust->z.fixed));
        } else {
          sourceX = (u16)gAirsRock_GustTargets[hit][0];
          MapGrid_CopyCellsTo((s16)sourceX, gAirsRock_GustTargets[hit][1],
                              cellX, cell.z - (hit >> 1), 1, 1);
          MapGrid_CopyCellAttributes(gAirsRock_GustTargets[hit][0],
                                     gAirsRock_GustTargets[hit][1], 1, 1, cellX,
                                     cellZ);
          MapGrid_CopyCellAttributes(
              gAirsRock_GustTargets[hit][0], gAirsRock_GustTargets[hit][1], 1,
              1, FIELD_CELL(gust->x.fixed), FIELD_CELL(gust->z.fixed));
        }
        MapGrid_CenterOnFollowedPoint();
      }
      Event_Wait(1);
    }
    if (hit == 2) {
      MapGrid_CopyCellsTo(gAirsRock_GustTargets[2][0],
                          gAirsRock_GustTargets[2][1], cell.x, cell.z, 1, 1);
      MapGrid_CenterOnFollowedPoint();
    }
    Sound_Play(SE_STOP_MUSIC_PLAYER_5);
    Event_SetFlag(flag + FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN);
  }
  gust->acceleration = 0;
  gust->speedLimit = 0;
  gust->targetZ = 0;
  gust->targetY = 0;
  gust->targetX = 0;
  for (i = 0; i != 8; i++) {
    gust->scaleX -= Q16_ONE * 3 / 20;
    gust->scaleY -= Q16_ONE / 4;
    gust->y.fixed += px(7) / 2;
    Event_Wait(1);
  }
  Actor_Release(gust);
  Event_Wait(30);
  FieldCamera_MoveToActorById(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  Event_End();
}

/// The scroll of the swirling sand, by HBlank lines.
#define WAVE_SCROLL AirsRock_ScrollWave
#define WAVE_SCROLL_LINES gAirsRock_WaveLines
#include "../field/field_effect_wave_scroll.c"

/// Moves the object: left of x 320 it speeds up rightward and grows, past it
/// it speeds up leftward and shrinks; its priority follows its direction and
/// its sprite spins.
void AirsRock_SwirlLeaf(struct Actor *object) {
  s32 scale;

  object->y.fixed += object->gravity;
  object->x.fixed += object->bounce;
  object->z.fixed += object->riseBaseY;
  if ((object->x.fixed >> 19) <= 39) {
    object->bounce += Q16_ONE * 3 / 2;
    scale = object->scaleX + (Q16_ONE / 20);
  } else {
    object->bounce -= Q16_ONE * 3 / 2;
    scale = object->scaleX - (Q16_ONE / 50);
  }
  object->scaleX = scale;
  object->scaleY = scale;
  if (object->bounce > 0)
    Scene_SetActorSpritePriority(object, 1);
  else
    Scene_SetActorSpritePriority(object, 2);
  ((struct FieldSprite *)object->sprite)->rotation += 0x1800;
}

struct SpriteParts {
  u8 unk00[9];
  u8 unk09Low : 2;
  u8 part : 2;
};

/// Whirlwind on the twister rock of the first slope: the mud is blown from the
/// wall so it can be climbed.
void AirsRock_WhirlwindClearsWall(void) {
  struct MainRoutineWork *work;
  u16 *buffer = Heap_Alloc(0x680);
  u16 *cell;
  struct Actor *spark;
  u32 i;
  s32 x;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  REG_DISPCNT &= ~0x100;
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_WhirlwindTiles),
                      buffer);
  Dma_Set(buffer, BG_SCREEN_BLOCK(29), DMA_COPY_WORDS(208), REG_DMA3);
  Dma_Set((u8 *)buffer + 0x340, (void *)0x600ec00, DMA_COPY_WORDS(208),
          REG_DMA3);
  Heap_Release(buffer);
  FieldActor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  FieldActor_SetSpritePriority(ACTOR_TWISTER_ROCK_BY_THE_WALL, 1);
  {
    s32 control = 0x40d;

    REG_BG0CNT = control;
  }
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 2;
  Dma_Fill16(0x10, (void *)0x6002000, 0x500);
  buffer = Heap_Alloc(0x240);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_WhirlwindMap),
                      buffer);
  i = 0;
  cell = buffer;
offset:
  *cell += 0xff40;
  i++;
  cell++;
  if (i != 0x120)
    goto offset;
  cell = buffer;
  i = 0;
row:
  Dma_Set(cell, (void *)(0x600200f + (i << 6)), DMA_COPY_WORDS(8), REG_DMA3);
  i++;
  cell += 16;
  if (i != 18)
    goto row;
  Heap_Release(buffer);
  Event_PlaySound(SE_GLITTER);
  Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->riseCounter = 9;
  Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->riseEnabled = 1;
  Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->update = AirsRock_FlickerTarget;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = FieldActor_Create(
          0x2a0, Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->x.fixed,
          Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->y.fixed,
          Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->z.fixed);
      {
        s32 priority =
            ((s8 *)Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->sprite)[9] & 12;

        cell = spark->sprite;
        ((struct SpriteParts *)cell)->part = priority >> 2;
      }
      spark->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      spark->motionFlags = 0;
      spark->bounce = spark->x.fixed;
      spark->gravity = px(3) / 2;
      spark->riseBaseY = spark->z.fixed;
      spark->speedLimit = Random16() & 0xffff;
      Actor_SelectAnimation(spark, 7);
      Actor_StartScript(spark, gFieldEffect_MediumScript);
      Actor_SetSpriteFlags(spark, 0);
      FieldActor_SetSpriteColor(spark, 2);
      spark->update = AirsRock_SpiralSpark;
    }
    FieldEvent_Wait(1);
  }
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(330, CAMERA_KEEP, 73, 1);
  FieldCamera_WaitForArrival();
  Task_AddFrame(AirsRock_ScrollWave, TASK_PASS_FRAME);
  Event_PlaySound(SE_163);
  REG_BLDALPHA = BLEND_ALPHA(0, 15);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_DISPCNT |= 0x100;
  MapGrid_SetShake(1, 1, 1);
  i = 0;
glow:
  REG_BLDALPHA = ((15 - (i >> 2)) << 8) | (i >> 2);
  Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->riseCounter = Random16() & 7;
  i++;
  FieldEvent_Wait(1);
  if (i != 64)
    goto glow;
  FieldEvent_Wait(60);
  REG_BLDALPHA = BLEND_ALPHA(15, 0);
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3;
  MapGrid_SetShake(3, 3, 1);
  {
    u32 j = 0;

  burst:
    j++;
    if ((j & 3) == 0) {
      struct Vector3 velocity;
      struct EffectOptions options;

      options.priority = 1;
      options.update = AirsRock_SwirlLeaf;
      velocity.x = 0;
      velocity.y = 0;
      velocity.z = -px(2);
      x = px(Random16() % 144) + px(256);
      FieldEffect_Spawn(x, 0, px(Random16() % 144), velocity.x, velocity.y,
                        velocity.z, EFFECT_USE_UPDATE | EFFECT_USE_PRIORITY | 1,
                        &options);
    }
    REG_BLDALPHA = (((j >> 4) & 15) << 8) | (16 - ((j >> 4) & 15));
    Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->riseCounter = Random16() & 7;
    FieldEvent_Wait(1);
    if (j != 255)
      goto burst;
  }
  MapGrid_CopyCellsTo(68, 0, 79, 0, 11, 10);
  MapGrid_CopyCellAttributes(38, 9, 1, 1, 16, 9);
  MapGrid_CopyCellAttributes(38, 9, 1, 1, 24, 9);
  MapGrid_CenterOnFollowedPoint();
  FieldEvent_Wait(1);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(15, 0);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->riseCounter = 6;
  i = 0;
fade:
  REG_BLDALPHA = ((i >> 2) << 8) | (16 - (i >> 2));
  FieldEvent_Wait(1);
  i++;
  if (i != 65)
    goto fade;
  FieldTask_Remove(AirsRock_ScrollWave);
  {
    s32 zero = 0;

    Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->riseEnabled = zero;
    System_Wait(30);
    Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->update = (void (*)(void))zero;
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL), 6);
    FieldMapGrid_WaitForShakeToSettle();
    FieldEvent_Wait(60);
    REG_BLDCNT = zero;
  }
  Window_LoadGraphics();
  DebugText_Clear();
  work = &gMainRoutineWork;
  WindowColor_Apply(work->windowHue, work->windowBrightness);
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 1;
  {
    s32 *ownerId = &work->leaderActorId;

    Actor_Get(*ownerId)->priorityFlags &= ~ACTOR_PRIORITY_IGNORES_RISES;
    Actor_Get(*ownerId)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    {
      u8 *flags = &Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->priorityFlags;
      u8 v = *flags | 1;

      *flags = v;
    }
    EventCamera_FollowActor(*ownerId, 1);
  }
  FieldCamera_WaitForArrival();
  Event_SetFlag(FLAG_AIRS_ROCK_WALL_CLEARED);
  Event_End();
}

/// The leader rides a statue's whirlwind along its path until it fades, landing
/// where it ends.
void AirsRock_RideWhirlwind(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *ride = leader->watchedActor;
  struct Actor *rider, *dust;
  u8 *riding, *motion;
  s32 rise, dx, dz, moved, i, height;
  s32 angle;
  u16 facing;
  s32 still;

  Event_Start();
  Event_ResolvePendingActions(0);
  motion = &leader->motionFlags;
  *motion = 4;
  Actor_SetSpriteFlags(leader, 0);
  GameFlag_Set(FLAG_RIDING_WHIRLWIND);
  Sound_Play(SE_KIRIN);
  riding = (u8 *)ride + 99;
  rise = 0;
  while (*riding != 0) {
    leader->x.fixed = ride->x.fixed;
    leader->y.fixed = ride->y.fixed + rise;
    leader->z.fixed = ride->z.fixed;
    if (rise <= 0xbffff)
      rise += 0x1999;
    leader->facing = (u8)gFrameCount << 12;
    leader->priorityFlags ^= 2 << (gFrameCount & 1);
    wait(1);
  }
  Sound_Play(SE_STOP_ACTION_SE);
  facing = ride->facing;
  if (facing == 0xc000) {
    rider = ride->watchedActor;
    dx = 0;
    dz = 0;
    leader->priorityFlags = ACTOR_PRIORITY_OVERHEAD;
    leader->x.fixed = (((leader->x.fixed >> 19) & ~1) + 1) << 19;
    i = 0;
    leader->y.fixed = (((ride->y.fixed >> 19) & ~1) + 3) << 19;
    leader->z.fixed = (((leader->z.fixed >> 19) & ~1) + 1) << 19;
  waiting:
    moved = 0;
    if (gInput.held & KEY_UP) {
      dx = 0;
      dz = -px(16);
      leader->facing = FACING_NORTH;
      moved = 1;
    }
    if (gInput.held & KEY_RIGHT) {
      dx = px(16);
      dz = 0;
      leader->facing = dz;
      moved = 1;
    }
    if (gInput.held & KEY_LEFT) {
      dx = -px(16);
      leader->facing = FACING_WEST;
      dz = 0;
      moved = 1;
    }
    if (moved &&
        MapCell_GetAttributes(2, FIELD_CELL(ride->x.fixed + dx),
                              (ride->z.fixed - ride->y.fixed + dz) >> 20) >>
                8 !=
            255)
      goto found;
    wait(1);
    if (++i != 60)
      goto waiting;
    dx = 0;
    leader->facing = FACING_NORTH;
    dz = -px(16);
    if ((u32)(MapCell_GetAttributes(2, FIELD_CELL(ride->x.fixed),
                                    (ride->z.fixed - ride->y.fixed + dz) >> 20)
              << 8) >>
            16 !=
        255)
      goto found;
    dx = px(16);
    dz = 0;
    leader->facing = dz;
    if ((u32)(MapCell_GetAttributes(2, FIELD_CELL(ride->x.fixed + dx),
                                    (ride->z.fixed - ride->y.fixed) >> 20)
              << 8) >>
            16 !=
        255)
      goto found;
    dx = -px(16);
    leader->facing = FACING_WEST;
  found:
    height = Map_GetTerrainHeight(rider->riseCounter, ride->x.fixed + dx,
                                  ride->z.fixed + dz);
    leader->y.fixed = height;
    leader->groundHeight = height;
    Actor_SelectAnimation(leader, 6);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(leader, 7);
    leader->speedLimit = px(2);
    leader->acceleration = px(1) / 2;
    leader->velocityY = px(4);
    *motion = 2;
    leader->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    leader->layer = rider->riseCounter;
    FieldActor_SetMoveTargetFixed(leader, leader->x.fixed + dx, leader->y.fixed,
                                  leader->z.fixed + dz);
    OverlayObject_WaitUntilIdle(leader);
    FieldActor_WaitForMove(leader);
    Actor_SetSpriteFlags(leader, 1);
    *motion = 3;
  } else {
    angle = ride->facing;
    leader->x.fixed = (((ride->x.fixed >> 19) & ~1) + (angle >>= 13) - 1) << 19;
    leader->z.fixed = (((ride->z.fixed >> 19) & ~1) - (angle & 2) + 1) << 19;
    still = 0;
    Actor_SetSpriteFlags(leader, 1);
    leader->velocityY = px(6);
    leader->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    *motion = 3;
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    dust = FieldCamera_GetActor();
    dust->speedLimit = px(8);
    dust->acceleration = px(1);
    dust->motionFlags = still;
    FieldActor_SetMoveTargetFixed(
        dust, leader->x.fixed,
        Map_GetTerrainHeight(0, leader->x.fixed, leader->z.fixed),
        leader->z.fixed);
    OverlayObject_WaitUntilIdle(leader);
    FieldActor_WaitForMove(dust);
  }
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_AUTOMATIC;
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  Event_ClearFlag(FLAG_RIDING_WHIRLWIND);
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  wait(10);
  Event_End();
}

#include "shared/position_flat_distance.h"

#include "shared/position_distance_above.h"

extern u32 gAirsRock_WhirlwindRisesScript[];
extern u32 gAirsRock_WhirlwindFadesScript[];

/// A statue's whirlwind's update: it travels its way, picks up a leader it
/// touches and fades on a wall or another statue.
void AirsRock_MoveWhirlwind(struct Actor *shot) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 speed = px(3) / 2;
  s32 cue = 539;
  struct Actor *other, *launcher;
  s32 i;
  s32 *leaderPosition;
  s32 *shotPosition;
  struct Actor **objects;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0 || gMainRoutineWork.movementMode == 5) {
    Actor_SetAnimationSpeed(shot, 0);
    return;
  }
  Actor_SetAnimationSpeed(shot, 16);
  if (MapCell_GetAttributes(2, FIELD_CELL(shot->x.fixed),
                            (shot->z.fixed - shot->y.fixed) >> 20) >>
          8 ==
      212) {
  end:
    Actor_PlaySoundInView(shot, cue);
    shot->riseEnabled = 0;
    shot->update = 0;
    if (shot->facing == 0xc000)
      Actor_StartScript(shot, gAirsRock_WhirlwindRisesScript);
    else
      Actor_StartScript(shot, gAirsRock_WhirlwindFadesScript);
    return;
  }
  if (shot->riseCounter == 0) {
    // i first says whether the shot is beside the leader; the loop below
    // reuses it as the object index.
    i = 0;
    leaderPosition = &leader->x.fixed;
    shotPosition = &shot->x.fixed;
    if (gMainRoutineWork.movementMode == 2) {
      if (Position_DistanceAbove(leaderPosition, shotPosition) <= 16 &&
          FIELD_CELL(leader->z.fixed) == FIELD_CELL(shot->z.fixed))
        i = 1;
    } else {
      if (Position_FlatDistance(leaderPosition, shotPosition) <= 8 &&
          FIELD_CELL(leader->y.fixed) == FIELD_CELL(shot->y.fixed))
        i = 1;
    }
    if (i && !GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS)) {
      leader->facing = shot->facing;
      leader->watchedActor = shot;
      leader->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      ((struct FieldSprite *)leader->sprite)->priority =
          ((struct FieldSprite *)shot->sprite)->priority;
      work->raisedTrigger = 200;
      gMainRoutineWork.movementMode = 0;
      shot->riseCounter = 1;
      shot->riseEnabled = 1;
    }
  }
  objects = &work->actors[8];
  for (i = 8; i != 64; i++) {
    other = *objects++;
    if (other->script != 0) {
      launcher = shot->watchedActor;
      if (FIELD_CELL(launcher->x.fixed) != FIELD_CELL(other->x.fixed) ||
          FIELD_CELL(launcher->y.fixed) != FIELD_CELL(other->y.fixed) ||
          FIELD_CELL(launcher->z.fixed) != FIELD_CELL(other->z.fixed)) {
        if (Position_FlatDistance(&other->x.fixed, &shot->x.fixed) <= 8 &&
            FIELD_CELL(other->y.fixed) == FIELD_CELL(shot->y.fixed))
          goto end;
      }
    }
  }
  if (shot->facing == 0xc000) {
    shot->x.fixed += Iwram_MulQ16(Trig_Cos(shot->facing), speed);
    shot->y.fixed -= Iwram_MulQ16(Math_Sin(shot->facing), speed);
  } else {
    shot->x.fixed += Iwram_MulQ16(Trig_Cos(shot->facing), speed);
    shot->z.fixed += Iwram_MulQ16(Math_Sin(shot->facing), speed);
  }
}

/// A launcher's update, the Air's Rock Inside launcher with a new shot each
/// time: while none of the event work's four busy counters is set it counts
/// frames in its action parameter, and at 240 launches a shot 8 pixels ahead
/// along its angle with a dust effect below it.
void AirsRock_StatueBreathesWhirlwind(struct Actor *object) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *shot;
  struct Vector3 position;
  struct EffectOptions options;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0)
    return;
  if (object->timer == 240) {
    if (!Event_GetFlag(FLAG_RIDING_WHIRLWIND))
      Actor_PlaySoundInView(object, 202);
    if (object->facing == 0xc000) {
      position.x = object->x.fixed;
      position.z = object->z.fixed;
    } else {
      position.x =
          object->x.fixed + Math_MulQ16(px(8), Trig_Cos(object->facing));
      position.z =
          object->z.fixed + Math_MulQ16(px(8), Math_Sin(object->facing));
    }
    shot = FieldActor_Create(280, position.x, object->y.fixed, position.z);
    Actor_SelectAnimation(shot, 2);
    Actor_SetSpriteFlags(shot, 0);
    ((struct FieldSprite *)shot->sprite)->priority = 1;
    shot->motionFlags = 0;
    shot->riseCounter = 0;
    shot->riseEnabled = 0;
    shot->facing = object->facing;
    shot->update = AirsRock_MoveWhirlwind;
    shot->watchedActor = object;
    object->timer = 0;
    options.priority = 1;
    options.palette = 7;
    FieldEffect_Spawn(position.x, object->y.fixed, position.z - px(6), 0, 0, 0,
                      EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
  }
  object->timer++;
}

/// Before a push or Move on a pillar: lifts it and frees the cells beside it.
void AirsRock_LiftPillar(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  Actor_LiftTargetBlock(gAirsRock_EntrancePillars);
  Blocks_UnmarkCells(work->movedBlock);
}

/// After it: a pillar left over a gap drops into it with a thump, closing the
/// gap, and sets its flag.
void AirsRock_PlacePillar(void) {
  struct Actor *object = gFieldWork->movedBlock;
  s32 x, z;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_PlaceMovedBlock();
  Blocks_MarkCells(((u8 *)gAirsRock_EntrancePillars));
  if (Map_GetTerrainHeight(2, object->x.fixed, object->z.fixed) !=
      object->y.fixed) {
    object->layer = 2;
    object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    OverlayObject_WaitUntilIdle(object);
    OverlayObject_WaitUntilIdle(object);
    Event_PlaySound(SE_THUMP);
    object->motionFlags = 0;
    object->layer = 0;
  }
  x = FIELD_CELL(object->x.fixed);
  z = FIELD_CELL(object->z.fixed);
  if (x == 16 && z == 43)
    Event_SetFlag(FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST);
  else if (x == 45 && z == 45)
    Event_SetFlag(FLAG_AIRS_ROCK_PILLAR_PLACED_SECOND);
  else if (x == 21 && z == 48)
    Event_SetFlag(FLAG_AIRS_ROCK_PILLAR_PLACED_THIRD);
  else if (x == 39 && z == 49)
    Event_SetFlag(FLAG_AIRS_ROCK_PILLAR_PLACED_FOURTH);
  else if (x == 49 && z == 18)
    Event_SetFlag(FLAG_AIRS_ROCK_PILLAR_PLACED_FIFTH);
  Event_End();
}

s32 FlagBlock_ApplyFlags();

/// Frost on the puddle freezes it into a pillar.
void AirsRock_FreezePuddle(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FlagBlock_ApplyFlags((s32)((u8 *)gAirsRock_FrozenPuddle));
  Event_End();
}

extern s32 gAirsRock_RubbleScript[];

/// A rock crumbles at x, z into rubble that scatters, shaking the screen.
void AirsRock_CrumbleRock(s32 x, s32 z) {
  struct Actor *first, *rock;
  struct EffectOptions options;
  struct Vector3 scatter;
  s32 i;
  s32 radius = px(64);
  s32 shakeA = -1;
  s32 shakeC = 0xe666;

  first = FieldActor_Create(255, x, 0, z);
  options.priority = 1;
  options.startScaleX = Q16_ONE * 2;
  options.startScaleY = Q16_ONE * 2;
  for (i = 0; i != 12; i++) {
    scatter.x = x + px(Random16() & 63) - px(32);
    scatter.y = (Random16() & 15) << 12;
    scatter.z = z + px(Random16() & 63) - px(48);
    if (i & 3)
      FieldEffect_Spawn(scatter.x, 0, scatter.z, 0, scatter.y, 0,
                        EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | 1,
                        &options);
    else
      FieldEffect_Spawn(scatter.x, 0, scatter.z, 0, scatter.y, 0,
                        EFFECT_USE_PRIORITY | 1, &options);
  }
  for (i = 0; i != 6; i++) {
    rock = first;
    if (i != 0) {
      rock = FieldActor_Create(255, x, 0, z);
      Sprite_ShareVramCacheEntry(rock->sprite, first->sprite);
    }
    Actor_SetSpriteFlags(rock, 0);
    Actor_StartScript(rock, gAirsRock_RubbleScript);
    Actor_SelectAnimation(rock, 2);
    rock->velocityY = px(Random16() & 3) + px(4);
    rock->gravity = px(1) / 5;
    rock->bounce = Q16_ONE * 2 / 5;
    rock->scaleX = Q16_ONE * 4 / 5;
    rock->scaleY = Q16_ONE * 4 / 5;
    rock->speedLimit = px(4) / 5;
    scatter.y = Random16() & 7;
    scatter.x = Iwram_MulQ16(Trig_Cos(scatter.y << 12), radius);
    scatter.z = Iwram_MulQ16(Math_Sin(scatter.y << 12), radius);
    ((struct FieldSprite *)rock->sprite)->priority = 1;
    FieldActor_SetMoveTargetFixed(rock, rock->x.fixed + scatter.x,
                                  rock->y.fixed, rock->z.fixed + scatter.z);
  }
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShakeFixed(shakeA, shakeA, shakeC);
}

/// Whirlwind on the purple twister rock (actor 14): it glows, the rocks in the
/// way crumble with a crash (AirsRock_CrumbleRock) and the way to the top
/// opens; sets flag FLAG_AIRS_ROCK_TOP_OPENED.
void AirsRock_WhirlwindOpensTheTop(void) {
  u16 *buffer = Heap_Alloc(0x800);
  u16 *cell;
  struct Actor *spark;
  u32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  Io_Write16(BGCNT_SCREEN_BASE(4), &REG_BG0CNT);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_LightningTiles),
                      buffer);
  Dma_Set(buffer, (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(512), REG_DMA3);
  Heap_Release(buffer);
  Dma_Fill16(0, (void *)BG_SCREEN_BLOCK(4), 0x800);
  buffer = Heap_Alloc(0x80);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_LightningMap),
                      buffer);
  i = 0;
  cell = buffer;
mask:
  *cell &= 0xfcff;
  i++;
  cell++;
  if (i != 64)
    goto mask;
  cell = buffer;
  i = 0;
row:
  Dma_Set(cell, (void *)(BG_SCREEN_BLOCK(4) + (i << 6)), DMA_COPY_WORDS(2),
          REG_DMA3);
  i++;
  cell += 4;
  if (i != 16)
    goto row;
  Heap_Release(buffer);
  Event_PlaySound(SE_GLITTER);
  Actor_Get(ACTOR_TWISTER_ROCK_AT_THE_TOP)->riseCounter = 9;
  Actor_Get(ACTOR_TWISTER_ROCK_AT_THE_TOP)->riseEnabled = 1;
  Actor_Get(ACTOR_TWISTER_ROCK_AT_THE_TOP)->update = AirsRock_FlickerTarget;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = (FieldActor_Create(
          0x2a0, Actor_Get(ACTOR_TWISTER_ROCK_AT_THE_TOP)->x.fixed,
          Actor_Get(ACTOR_TWISTER_ROCK_AT_THE_TOP)->y.fixed,
          Actor_Get(ACTOR_TWISTER_ROCK_AT_THE_TOP)->z.fixed));
      ((struct FieldSprite *)spark->sprite)->priority =
          ((struct FieldSprite *)Actor_Get(ACTOR_TWISTER_ROCK_AT_THE_TOP)
               ->sprite)
              ->priority;
      spark->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      spark->motionFlags = 0;
      spark->bounce = spark->x.fixed;
      spark->gravity = px(3) / 2;
      spark->riseBaseY = spark->z.fixed;
      spark->speedLimit = Random16() & 0xffff;
      Actor_SelectAnimation(spark, 7);
      Actor_StartScript(spark, gFieldEffect_MediumScript);
      Actor_SetSpriteFlags(spark, 0);
      FieldActor_SetSpriteColor(spark, 2);
      spark->update = AirsRock_SpiralSpark;
    }
    FieldEvent_Wait(1);
  }
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x10003, 0);
  PaletteFade_Interpolate(60);
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(536, CAMERA_KEEP, 216, 1);
  FieldCamera_WaitForArrival();
  Actor_Get(ACTOR_TWISTER_ROCK_AT_THE_TOP)->update = 0;
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_TWISTER_ROCK_AT_THE_TOP), 6);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  gBgScroll[0].x = 0xff88;
  gBgScroll[0].y = 0x10;
  System_Wait(30);
  Event_PlaySound(SE_ITEM_BREAK);
  PaletteFade_ApplySource(0x10003, 0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  Actor_Get(gMainRoutineWork.leaderActorId)->facing = FACING_SOUTH;
  System_Wait(1);
  PaletteFade_ApplySource(0x7fff, 0);
  PaletteFade_ApplyTarget(0x10003, 0);
  PaletteFade_Interpolate(8);
  REG_DISPCNT |= DISPCNT_BG0_ON;
  MapGrid_CopyCellsTo(52, 12, 29, 9, 9, 9);
  MapGrid_CopyCellsTo(52, 76, 29, 73, 9, 9);
  AirsRock_CrumbleRock(0x2280000, 0xe40000);
  i = 0;
fadeFirstCrumble:
  REG_BLDALPHA = BLEND_ALPHA(16 - (i >> 1), 16);
  System_Wait(1);
  i++;
  if (i != 32)
    goto fadeFirstCrumble;
  gBgScroll[0].x = 0xffac;
  gBgScroll[0].y = 0;
  System_Wait(1);
  Event_PlaySound(SE_ITEM_BREAK);
  PaletteFade_ApplySource(0x10003, 0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  PaletteFade_ApplySource(0x7fff, 0);
  PaletteFade_ApplyTarget(0x10003, 0);
  PaletteFade_Interpolate(8);
  REG_DISPCNT |= DISPCNT_BG0_ON;
  MapGrid_CopyCellsTo(52, 22, 29, 9, 9, 9);
  MapGrid_CopyCellsTo(52, 86, 29, 73, 9, 9);
  AirsRock_CrumbleRock(0x2080000, 0xf20000);
  i = 0;
fadeSecondCrumble:
  REG_BLDALPHA = BLEND_ALPHA(16 - (i >> 1), 16);
  System_Wait(1);
  i++;
  if (i != 32)
    goto fadeSecondCrumble;
  Event_PlaySound(SE_CRASH);
  PaletteFade_ApplySource(0x10003, 0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  FieldEvent_Wait(20);
  AirsRock_CrumbleRock(0x2000000, 0xc60000);
  AirsRock_CrumbleRock(0x2320000, 0xf20000);
  AirsRock_CrumbleRock(0x2090000, 0xdf0000);
  MapGrid_CopyCellsTo(62, 2, 29, 9, 9, 9);
  MapGrid_CopyCellAttributes(62, 2, 9, 9, 29, 9);
  MapGrid_CopyCellsTo(62, 66, 29, 73, 9, 9);
  PaletteFade_ApplySource(0x7fff, 0);
  PaletteFade_ApplyTarget(0x10003, 0);
  PaletteFade_Interpolate(60);
  System_Wait(60);
  PaletteFade_ApplySource(0x10003, 0);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(60);
  System_Wait(70);
  Window_LoadGraphics();
  DebugText_Clear();
  FieldWindowColor_Apply(gMainRoutineWork.windowHue,
                         gMainRoutineWork.windowBrightness);
  REG_BLDCNT = 0;
  REG_DISPCNT |= DISPCNT_BG0_ON;
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  Event_SetFlag(FLAG_AIRS_ROCK_TOP_OPENED);
  Event_End();
}

/// The top's fog: its blend breathes between 4 and 8 sixteenths while its lines
/// ripple.
void AirsRock_DriftFog(void) {
  s32 *scroll = (s32 *)((u8 *)Ram_HeapSlots->mapGrid + 0x178);
  s16 *line = gAirsRock_WaveLines;
  s32 i;
  volatile u16 *channel;

  REG_BLDALPHA = BLEND_ALPHA(gAirsRock_FogAlpha, BLEND_WEIGHT_MAX);
  if ((*Ram_FrameCount & 15) == 0) {
    gAirsRock_FogAlpha += gAirsRock_FogAlphaStep;
    if (gAirsRock_FogAlpha == 8 || gAirsRock_FogAlpha == 4)
      gAirsRock_FogAlphaStep *= -1;
  }
  for (i = 0; i != 160; i++) {
    *line =
        (scroll[0] >> 16) +
        (Math_Sin((i + (u8)*Ram_FrameCount + (scroll[1] >> 16)) << 9) >> 13);
    line++;
  }
  line = gAirsRock_WaveLines;
  REG_BG1HOFS = line[0];
  channel = REG_DMA0;
  channel[5] &= 0xc5ff;
  channel[5] &= 0x7fff;
  channel[5];
  Dma_Set(line + 1, (void *)&REG_BG1HOFS, 0xa2600001, (volatile u32 *)channel);
}

void AirsRock_LiftFog(void) {
  s32 *scroll = (s32 *)((u8 *)Ram_HeapSlots->mapGrid + 0x178);
  s16 *line = gAirsRock_WaveLines;
  s32 i, wave, spread, value;
  s16 *table;
  volatile u16 *channel;

  for (i = 0; i != 160; i++) {
    spread = (160 - i) >> 2;
    wave = Math_Sin((i + (u8)gFrameCount) << 8);
    value = (Iwram_MulQ16(wave, (144 - i) << 10) >> 11) - 4;
    line[0] = value;
    line[0] += Random16() % (spread * 2) - spread;
    if (line[0] > 56)
      line[0] -= 56;
    else if (line[0] < -64)
      line[0] += 64;
    line[1] = 0;
    value = wave + (scroll[1] >> 16);
    line[2] = (scroll[0] >> 16) + (value >> 13);
    line += 3;
  }
  REG_BLDALPHA = BLEND_ALPHA(gAirsRock_LiftingFogAlpha, BLEND_WEIGHT_MAX);
  if ((gFrameCount & 15) == 0) {
    gAirsRock_LiftingFogAlpha += gAirsRock_LiftingFogAlphaStep;
    if (gAirsRock_LiftingFogAlpha == 8 || gAirsRock_LiftingFogAlpha == 4)
      gAirsRock_LiftingFogAlphaStep *= -1;
  }
  table = gAirsRock_WaveLines;
  *(volatile u16 *)REG_BG0HOFS = table[0];
  table += 2;
  REG_BG1HOFS = table[0];
  channel = REG_DMA0;
  channel[5] &= 0xc5ff;
  channel[5] &= 0x7fff;
  channel[5];
  Dma_Set(table + 1, REG_BG0HOFS, 0xa2600003, (volatile u32 *)channel);
}

/// Whirlwind on the stone in the fog: the fog lifts (AirsRock_LiftFog) and
/// stays clear.
void AirsRock_WhirlwindClearsFog(void) {
  u16 *buffer = Heap_Alloc(0x680);
  s32 *beam = (s32 *)((u8 *)Ram_HeapSlots->mapGrid + 0x178);
  u16 *cell;
  struct Actor *spark;
  s32 i;
  s32 z;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  REG_DISPCNT &= ~0x100;
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_WhirlwindTiles),
                      buffer);
  Dma_Set(buffer, BG_SCREEN_BLOCK(29), DMA_COPY_WORDS(208), REG_DMA3);
  Dma_Set((u8 *)buffer + 0x340, (void *)0x600ec00, DMA_COPY_WORDS(208),
          REG_DMA3);
  Heap_Release(buffer);
  {
    s32 control = 0x40e;

    REG_BG0CNT = control;
  }
  // A once-only loop: its loop notes order the fill's address ahead of its
  // value.
  do {
    Dma_Fill16(0x10, (void *)0x6002000, 0x500);
  } while (0);
  buffer = Heap_Alloc(0x240);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_WhirlwindMap),
                      buffer);
  cell = buffer;
  for (i = 0; i != 0x120; i++) {
    *cell += 0xff40;
    cell++;
  }
  cell = buffer;
  for (i = 0; i != 18; i++) {
    Dma_Set(cell, (void *)(0x600200f + (i << 6)), DMA_COPY_WORDS(8), REG_DMA3);
    cell += 16;
  }
  Heap_Release(buffer);
  Event_PlaySound(SE_GLITTER);
  Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->riseCounter = 9;
  Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->riseEnabled = 1;
  Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->update = AirsRock_FlickerTarget;
  for (i = 0; i != 45; i++) {
    s32 low = gFrameCount & 3;

    if (low == 0) {
      spark = (FieldActor_Create(
          0x2a0, Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->x.fixed,
          Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->y.fixed,
          Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->z.fixed));
      {
        s32 priority =
            ((s8 *)Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->sprite)[9] & 12;

        cell = spark->sprite;
        ((struct SpriteParts *)cell)->part = priority >> 2;
      }
      spark->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      spark->motionFlags = 0;
      spark->bounce = spark->x.fixed;
      spark->gravity = px(3) / 2;
      spark->riseBaseY = spark->z.fixed;
      spark->speedLimit = Random16() & 0xffff;
      Actor_SelectAnimation(spark, 7);
      Actor_StartScript(spark, gFieldEffect_MediumScript);
      Actor_SetSpriteFlags(spark, 0);
      FieldActor_SetSpriteColor(spark, 2);
      spark->update = AirsRock_SpiralSpark;
    }
    FieldEvent_Wait(1);
  }
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(152, CAMERA_KEEP, 144, 1);
  FieldCamera_WaitForArrival();
  FieldTask_Remove(AirsRock_DriftFog);
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 0;
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  PaletteFade_ApplySource(0x7fff, 0);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(8);
  Task_AddFrame(AirsRock_LiftFog, TASK_PASS_FRAME);
  Event_PlaySound(SE_163);
  REG_DISPCNT |= 0x100;
  MapGrid_SetShake(3, 3, 1);
  z = 0xffff8000;
  for (i = 0x4000; i != 0x48000; i += 0x400, z += 0xfffffc00) {
    beam[6] = i;
    beam[7] = z;
    Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->riseCounter = Random16() & 7;
    System_Wait(1);
  }
  FieldTask_Remove(AirsRock_LiftFog);
  Task_AddFrame(AirsRock_ScrollWave, TASK_PASS_FRAME);
  REG_BLDALPHA = BLEND_ALPHA(8, 0);
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  for (i = 0; i != 257; i++) {
    REG_BLDALPHA = BLEND_ALPHA(8 - (i >> 5), BLEND_WEIGHT_MAX);
    Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->riseCounter = Random16() & 7;
    FieldEvent_Wait(1);
  }
  REG_DISPCNT &= ~0x200;
  System_Wait(30);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  REG_BLDALPHA = BLEND_ALPHA(16, 0);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->riseCounter = 6;
  for (i = 0; i != 129; i++) {
    REG_BLDALPHA = ((i >> 3) << 8) | (0x10 - (i >> 3));
    FieldEvent_Wait(1);
  }
  FieldTask_Remove(AirsRock_ScrollWave);
  {
    s32 zero = 0;

    Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->riseEnabled = zero;
    System_Wait(30);
    Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->update = (void (*)(void))zero;
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG), 6);
    FieldMapGrid_WaitForShakeToSettle();
    FieldEvent_Wait(60);
    REG_BLDCNT = zero;
  }
  Window_LoadGraphics();
  DebugText_Clear();
  FieldWindowColor_Apply(gMainRoutineWork.windowHue,
                         gMainRoutineWork.windowBrightness);
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 1;
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags &=
      ~ACTOR_PRIORITY_IGNORES_RISES;
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->priorityFlags |=
      ACTOR_PRIORITY_AUTOMATIC;
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  Event_SetFlag(FLAG_AIRS_ROCK_FOG_CLEARED);
  Event_End();
}

extern s32 gAirsRock_LeaderLevel;

/// Sets the ledge's cells for the height the leader climbs to, one of four
/// levels.
void AirsRock_SetLedgeCellsByLevel(void) {
  struct Actor *actor = Actor_Get(FieldParty_GetLeaderActor());
  s32 level;

  actor->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  level = ((actor->y.fixed + px(32)) >> 16) / 112;
  if (gAirsRock_LeaderLevel != level) {
    switch (level) {
    case 3:
      MapGrid_CopyCellAttributes(64, 64, 21, 3, 27, 54);
      break;
    case 2:
      MapGrid_CopyCellAttributes(64, 67, 21, 3, 27, 54);
      break;
    case 1:
      MapGrid_CopyCellAttributes(64, 70, 21, 3, 27, 54);
      break;
    case 0:
      MapGrid_CopyCellAttributes(64, 73, 21, 3, 27, 54);
      break;
    }
    gAirsRock_LeaderLevel = level;
  }
}

#include "shared/leader_lift.h"

#define LEADER() Actor_GetById(gMainRoutineWork.leaderActorId)
#define BEAM ((s32 *)((u8 *)Ram_HeapSlots->mapGrid + 0x178))

/// A statue that breathes whirlwinds (AirsRock_StatueBreathesWhirlwind), from
/// the given count of frames.
#define SWAY(id, phase)                                                        \
  (Actor_GetById(id)->timer = (phase),                                         \
   Actor_GetById(id)->update = AirsRock_StatueBreathesWhirlwind,               \
   Actor_SetSpriteFlags(Actor_GetById(id), 0))

/// The same for a statue that hangs at the given height.
#define HANG(id, height, phase)                                                \
  (Actor_GetById(id)->motionFlags = 0, Actor_GetById(id)->y.fixed = (height),  \
   SWAY(id, phase))

s32 Scene_Initialize(void) {
  struct Actor *leader;
  s32 *beam;
  volatile struct DmaRegisters *dma;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  gMainRoutineWork.retreatScene = SCENE_AIRS_ROCK_STORM_BRAND_CHEST;
  gMainRoutineWork.retreatEntrance = 1;
  LEADER()->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  if (gMainRoutineWork.next == SCENE_AIRS_ROCK_STORM_BRAND_CHEST) {
    switch (gMainRoutineWork.entrance) {
    case 2:
    case 3:
    case 4:
      Leader_Lift(0x580000);
    }
    if (GameFlag_Get(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN)) {
      MapGrid_CopyCellsTo(64, 0, 113, 11, 3, 3);
      MapGrid_CopyCellAttributes(64, 0, 3, 2, 113, 12);
      MapGrid_CopyCellAttributes(64, 0, 3, 2, 49, 12);
    }
    if (GameFlag_Get(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN + 1)) {
      MapGrid_CopyCellsTo(64, 0, 70, 39, 1, 4);
      MapGrid_CopyCellAttributes(64, 0, 1, 3, 70, 40);
      MapGrid_CopyCellAttributes(64, 0, 1, 3, 6, 40);
    }
    if (GameFlag_Get(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN + 2)) {
      MapGrid_CopyCellsTo(64, 0, 103, 38, 1, 4);
      MapGrid_CopyCellAttributes(64, 0, 1, 3, 103, 39);
      MapGrid_CopyCellAttributes(64, 0, 1, 3, 39, 39);
    }
    if (GameFlag_Get(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN + 3)) {
      MapGrid_CopyCellsTo(64, 0, 87, 43, 1, 4);
      MapGrid_CopyCellAttributes(64, 0, 1, 3, 87, 44);
      MapGrid_CopyCellAttributes(64, 0, 1, 3, 23, 44);
    }
    if (GameFlag_Get(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN + 4)) {
      MapGrid_CopyCellsTo(64, 0, 70, 52, 1, 4);
      MapGrid_CopyCellAttributes(64, 0, 1, 3, 70, 52);
      MapGrid_CopyCellAttributes(64, 0, 1, 3, 6, 53);
    }
    if (GameFlag_Get(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN + 5)) {
      MapGrid_CopyCellsTo(64, 0, 79, 49, 3, 3);
      MapGrid_CopyCellAttributes(64, 0, 3, 2, 79, 50);
      MapGrid_CopyCellAttributes(64, 0, 3, 2, 15, 50);
    }
    if (GameFlag_Get(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN + 6)) {
      MapGrid_CopyCellsTo(64, 0, 103, 48, 1, 4);
      MapGrid_CopyCellAttributes(64, 0, 1, 3, 103, 49);
      MapGrid_CopyCellAttributes(64, 0, 1, 3, 39, 49);
    }
    if (GameFlag_Get(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN + 7)) {
      MapGrid_CopyCellsTo(64, 0, 111, 52, 3, 3);
      MapGrid_CopyCellAttributes(64, 0, 3, 2, 111, 53);
      MapGrid_CopyCellAttributes(64, 0, 3, 2, 47, 53);
    }
    if (GameFlag_Get(FLAG_AIRS_ROCK_WALL_CLEARED)) {
      MapGrid_CopyCellsTo(68, 0, 79, 0, 11, 10);
      MapGrid_CopyCellAttributes(38, 9, 1, 1, 16, 9);
      MapGrid_CopyCellAttributes(38, 9, 1, 1, 24, 9);
      Actor_SetSpriteColor(Actor_GetById(ACTOR_TWISTER_ROCK_BY_THE_WALL), 6);
    }
  }
  if (gMainRoutineWork.next == SCENE_AIRS_ROCK_ENTRANCE) {
    switch (gMainRoutineWork.entrance) {
    case 3:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->z.fixed += px(16);
      Leader_Lift(0xffb80000);
      goto swaying;
    case 1:
    case 2:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->z.fixed += px(16);
      Leader_Lift(0xffd80000);
      goto swaying;
    case 5:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        LEADER()->x.fixed += px(48);
        LEADER()->y.fixed += px(16);
      }
    case 6:
      Leader_Lift(0x380000);
    case 4:
    swaying:
      SWAY(8, 60);
      SWAY(9, 180);
      SWAY(10, 0);
      SWAY(11, 60);
      if (GameFlag_Get(FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST))
        Actor_SetPosition(ACTOR_FIRST_LEDGE_PILLAR, 264, 696);
      if (GameFlag_Get(FLAG_AIRS_ROCK_PILLAR_PLACED_SECOND))
        Actor_SetPosition(ACTOR_SECOND_LEDGE_PILLAR, 728, 728);
      if (GameFlag_Get(FLAG_AIRS_ROCK_PILLAR_PLACED_THIRD))
        Actor_SetPosition(ACTOR_THIRD_LEDGE_PILLAR, 344, 776);
      if (GameFlag_Get(FLAG_AIRS_ROCK_PILLAR_PLACED_FOURTH))
        Actor_SetPosition(ACTOR_FOURTH_LEDGE_PILLAR, 632, 792);
      Actor_PlaceBlocks(gAirsRock_EntrancePillars);
      Blocks_MarkCells(gAirsRock_EntrancePillars);
      FlagBlock_PlaceAll(gAirsRock_FrozenPuddle);
      break;
    case 11:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        LEADER()->x.fixed += px(16);
        LEADER()->y.fixed -= px(16);
      }
    case 10:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->z.fixed += px(16);
      Leader_Lift(0xffc80000);
    case 12:
      if (GameFlag_Get(FLAG_AIRS_ROCK_PILLAR_PLACED_FIFTH))
        Actor_SetPosition(ACTOR_FIFTH_LEDGE_PILLAR, 792, 296);
      Actor_PlaceBlocks(gAirsRock_EntrancePillars);
      Blocks_MarkCells(gAirsRock_EntrancePillars);
      HANG(17, 0x100000, 60);
      HANG(19, 0x500000, 90);
      HANG(18, 0xffd00000, 100);
      HANG(20, 0x900000, 180);
      Actor_GetById(ACTOR_BREATHING_STATUE)->motionFlags = 0;
      Actor_GetById(ACTOR_BREATHING_STATUE)->y.fixed = -px(144);
      Actor_SetSpritePriority(ACTOR_BREATHING_STATUE, 2);
      Actor_GetById(ACTOR_BREATHING_STATUE)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
      Actor_GetById(ACTOR_BREATHING_STATUE)->timer = 180;
      Actor_GetById(ACTOR_BREATHING_STATUE)->riseCounter = 1;
      Actor_GetById(ACTOR_BREATHING_STATUE)->update =
          AirsRock_StatueBreathesWhirlwind;
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_BREATHING_STATUE), 0);
      break;
    }
  }
  if (gMainRoutineWork.next == SCENE_AIRS_ROCK_FUJIN_SHIELD_CHEST) {
    switch (gMainRoutineWork.entrance) {
    case 1:
    case 2:
    case 3:
      Task_AddFrame(AirsRock_SetLedgeCellsByLevel, TASK_PASS_VBLANK);
      Actor_GetById(ACTOR_FIRST_BREATHING_STATUE)->motionFlags = 0;
      Actor_GetById(ACTOR_FIRST_BREATHING_STATUE)->y.fixed = -px(8);
      Actor_SetSpritePriority(ACTOR_FIRST_BREATHING_STATUE, 2);
      Actor_GetById(ACTOR_FIRST_BREATHING_STATUE)->timer = 60;
      Actor_GetById(ACTOR_FIRST_BREATHING_STATUE)->riseCounter = 0;
      Actor_GetById(ACTOR_FIRST_BREATHING_STATUE)->update =
          AirsRock_StatueBreathesWhirlwind;
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_BREATHING_STATUE), 0);
      Actor_GetById(ACTOR_SECOND_BREATHING_STATUE)->motionFlags = 0;
      Actor_GetById(ACTOR_SECOND_BREATHING_STATUE)->y.fixed = -px(8);
      Actor_SetSpritePriority(ACTOR_SECOND_BREATHING_STATUE, 2);
      Actor_GetById(ACTOR_SECOND_BREATHING_STATUE)->timer = 180;
      Actor_GetById(ACTOR_SECOND_BREATHING_STATUE)->riseCounter = 0;
      Actor_GetById(ACTOR_SECOND_BREATHING_STATUE)->update =
          AirsRock_StatueBreathesWhirlwind;
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_SECOND_BREATHING_STATUE), 0);
      Actor_GetById(ACTOR_FIRST_PILLAR)->layer = 2;
      Actor_SetSpritePriority(ACTOR_FIRST_PILLAR, 1);
      Actor_GetById(ACTOR_FIRST_PILLAR)->y.fixed = px(208);
      Actor_GetById(ACTOR_FIRST_PILLAR)->groundHeight =
          Actor_GetById(ACTOR_FIRST_PILLAR)->y.fixed;
      Actor_GetById(ACTOR_SECOND_PILLAR)->layer = 2;
      Actor_SetSpritePriority(ACTOR_SECOND_PILLAR, 1);
      Actor_GetById(ACTOR_SECOND_PILLAR)->y.fixed = px(208);
      Actor_GetById(ACTOR_SECOND_PILLAR)->groundHeight =
          Actor_GetById(ACTOR_SECOND_PILLAR)->y.fixed;
      if (GameFlag_Get(FLAG_AREA_TEMP_10))
        Actor_SetPosition(ACTOR_FIRST_PILLAR, 504, 872);
      if (GameFlag_Get(FLAG_AREA_TEMP_11))
        Actor_SetPosition(ACTOR_SECOND_PILLAR, 520, 872);
      AirsRock_MoveSecondPillar();
      beam = (s32 *)Ram_HeapSlots->mapGrid;
      beam = (s32 *)((u8 *)beam + 0x178);
      beam[2] = px(1408);
      beam[3] = px(576);
      beam[4] = 0x4000;
      beam[5] = 0x4000;
      MapGrid_CenterOnFollowedPoint();
      System_Wait(1);
      break;
    case 6:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->y.fixed += px(24);
    case 7:
      Leader_Lift(0x480000);
    case 4:
    case 5:
      Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
      break;
    case 8:
    case 9:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->z.fixed += px(16);
      Leader_Lift(0xffe80000);
      goto beam;
    case 10:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->x.fixed += px(48);
    case 11:
      Leader_Lift(0x900000);
    beam:
      REG_BG1CNT &= ~3;
      REG_BG1CNT |= 1;
      REG_BG2CNT &= ~3;
      REG_BG2CNT |= 2;
      REG_BG3CNT &= ~3;
      REG_BG3CNT |= 3;
      Actor_GetById(ACTOR_TWISTER_ROCK_IN_THE_FOG)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
      FieldEffect_StartBlankOverlay(0);
      if (GameFlag_Get(FLAG_AIRS_ROCK_THIRD_PILLAR_MOVED))
        Actor_SetPosition(ACTOR_THIRD_PILLAR, 184, 280);
      if (!GameFlag_Get(FLAG_AIRS_ROCK_FOG_CLEARED)) {
        gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        REG_BLDALPHA = BLEND_ALPHA(8, 16);
        REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
        dma = (volatile struct DmaRegisters *)0x040000b0;
        while ((s32)dma->control < 0)
          ;
        Task_AddFrame(AirsRock_DriftFog, TASK_PASS_FRAME);
        beam = (s32 *)Ram_HeapSlots->mapGrid;
        beam = (s32 *)((u8 *)beam + 0x178);
        beam[6] = 0x4000;
        beam[7] = 0xffff8000;
        {
          s16 *scale = (s16 *)beam;
          s32 value = 3;

          scale[20] = value;
          value = 0x180;
          scale[21] = value;
        }
      } else {
        REG_BLDALPHA = BLEND_ALPHA(0, 16);
        REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
        REG_DISPCNT &= ~0x200;
        Actor_SetSpriteColor(Actor_GetById(ACTOR_TWISTER_ROCK_IN_THE_FOG), 6);
      }
      break;
    case 12:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->y.fixed -= px(32);
    case 13:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->z.fixed += px(16);
      Leader_Lift(0xffd00000);
    case 14:
      beam = (s32 *)Ram_HeapSlots->mapGrid;
      beam = (s32 *)((u8 *)beam + 0x178);
      beam[2] = px(1152);
      beam[3] = px(800);
      beam[4] = 0xd000;
      beam[5] = 0xd000;
      if (!GameFlag_Get(FLAG_AIRS_ROCK_TOP_OPENED)) {
        MapGrid_CopyCellsTo(29, 9, 62, 2, 9, 9);
        MapGrid_CopyCellAttributes(29, 9, 9, 9, 62, 2);
        MapGrid_CopyCellsTo(29, 73, 62, 66, 9, 9);
        MapGrid_CopyCellsTo(52, 2, 29, 9, 9, 9);
        MapGrid_CopyCellAttributes(52, 2, 9, 9, 29, 9);
        MapGrid_CopyCellsTo(52, 66, 29, 73, 9, 9);
      } else {
        Actor_SetSpriteColor(Actor_GetById(ACTOR_TWISTER_ROCK_AT_THE_TOP), 6);
      }
      Actor_SetSpritePriority(ACTOR_TWISTER_ROCK_AT_THE_TOP, 1);
      MapGrid_CenterOnFollowedPoint();
      System_Wait(1);
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        leader = LEADER();
        leader->layer = 0;
        leader->y.fixed = leader->groundHeight = Map_GetTerrainHeight(
            0, leader->x.fixed, leader->z.fixed - leader->y.fixed);
        EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 0);
      }
      break;
    case 99:
      MapGrid_CopyCellsTo(29, 9, 62, 2, 9, 9);
      MapGrid_CopyCellAttributes(29, 9, 9, 9, 62, 2);
      MapGrid_CopyCellsTo(29, 73, 62, 66, 9, 9);
      MapGrid_CopyCellsTo(52, 2, 29, 9, 9, 9);
      MapGrid_CopyCellAttributes(52, 2, 9, 9, 29, 9);
      MapGrid_CopyCellsTo(52, 66, 29, 73, 9, 9);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      AirsRock_WhirlwindOpensTheTop();
      break;
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.next == (s32)MsgCurePoison2) {
    AirsRock_SetLedgeCellsByLevel();
  }
  return 0;
}

#undef WAVE_SCROLL
#undef WAVE_SCROLL_LINES
#undef LEADER
#undef BEAM
#undef SWAY
#undef HANG
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY

void AirsRock_MoveSecondPillar(void);

// clang-format off
u32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

s16 gAirsRock_EntrancePillars[] = {
    12, 13, 15, 16, 22, -1,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s32 gAirsRock_WindStoneScript[] = {
    SC_SET(SW_SCALE_Y, 0x18000),
    SC_ANIMATION(2),
    SC_CALL((s32)AirsRock_ClearScriptedActorSpriteFlags),
    SC_END,
};

u32 gAirsRock_WhirlwindFadesScript[] = {
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_RELEASE,
};

u32 gAirsRock_WhirlwindRisesScript[] = {
    SC_WAIT(60),
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_RELEASE,
};

s32 gAirsRock_RubbleScript[] = {
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, -1310),
    SC_ADD(SW_SCALE_Y, -1310),
    SC_LOOP(32, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

s16 gAirsRock_FrozenPuddle[] = {
    14, 512, -1, 0,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

s16 gAirsRock_StormBrandChestRegions[] = {
    256, 96, 144, 272, 112, 160, -1, 2,
    608, 96, 144, 624, 112, 160, -1, 3,
    864, 96, 144, 880, 112, 160, -1, 4,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

s16 gAirsRock_EntranceRegions[] = {
    256, -192, 816, 272, -176, 832, -1, 1,
    608, -192, 816, 624, -176, 832, -1, 2,
    864, -192, 816, 880, -176, 832, -1, 3,
    624, 176, 704, 640, 192, 720, -1, 5,
    784, 160, 688, 800, 176, 704, -1, 6,
    624, -200, 320, 640, -184, 336, -1, 7,
    784, -216, 304, 800, -200, 320, -1, 8,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

s16 gAirsRock_FujinShieldChestRegions[] = {
    144, -96, 304, 160, -80, 320, -1, 6,
    208, -96, 304, 224, -80, 320, -1, 7,
    144, -80, 304, 160, -64, 320, -1, 8,
    208, -80, 304, 224, -64, 320, -1, 9,
    144, 160, 192, 160, 176, 208, -1, 10,
    208, 160, 192, 224, 176, 208, -1, 11,
    144, 160, 192, 160, 176, 208, -1, 10,
    208, 160, 192, 224, 176, 208, -1, 11,
    528, -96, 304, 544, -80, 320, -1, 12,
    592, -96, 304, 608, -80, 320, -1, 13,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(AirsRockStormBrandChest),
    SCENE_EXIT_UNLESS(1, WorldMap, 16, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockEntrance, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AirsRockEntrance, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AirsRockEntrance, 3, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockEntrance),
    SCENE_EXIT_UNLESS(1, AirsRockStormBrandChest, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockStormBrandChest, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AirsRockStormBrandChest, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AirsRockInsideHall, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, AirsRockEntrance, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, AirsRockEntrance, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, AirsRockEntrance, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, AirsRockEntrance, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, AirsRockFujinShieldChest, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockFujinShieldChest),
    SCENE_EXIT_UNLESS(1, AirsRockEntrance, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockFujinShieldChest, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AirsRockFujinShieldChest, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AirsRockFujinShieldChest, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, AirsRockFujinShieldChest, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, AirsRockFujinShieldChest, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, AirsRockFujinShieldChest, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, AirsRockFujinShieldChest, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, AirsRockFujinShieldChest, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, AirsRockFujinShieldChest, 8, FLAG_AIRS_ROCK_FOG_CLEARED),
    SCENE_EXIT_UNLESS(11, AirsRockFujinShieldChest, 9, FLAG_AIRS_ROCK_FOG_CLEARED),
    SCENE_EXIT_UNLESS(10, AirsRockFujinShieldChest, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, AirsRockFujinShieldChest, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, AirsRockFujinShieldChest, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, AirsRockFujinShieldChest, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, AirsRockInsideVialChest, 1, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gAirsRock_StormBrandChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TWISTER_ROCK, CONDITION_ALWAYS, 7, 328, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, (s32)gAirsRock_WindStoneScript, 920, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, (s32)gAirsRock_WindStoneScript, 104, 0, 760, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, (s32)gAirsRock_WindStoneScript, 376, 0, 808, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, (s32)gAirsRock_WindStoneScript, 632, 0, 888, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, (s32)gAirsRock_WindStoneScript, 872, 0, 856, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRock_EntrancePlacements[] = {
    {377, CONDITION_ALWAYS, 1, 0x1, 0, 0, FACING_EAST, 2, 2},
    {377, CONDITION_ALWAYS, 1, 0x2, 0, 0, FACING_EAST, 2, 2},
    {377, CONDITION_ALWAYS, 1, 0x3, 0, 0, FACING_WEST, 2, 2},
    {377, CONDITION_ALWAYS, 1, 0x4, 0, 0, FACING_WEST, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x6, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x7, 0, 0, FACING_SOUTH, 2, 2},
    {290, CONDITION_ALWAYS, 1, 0x8, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x9, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0xa, 0, 0, FACING_SOUTH, 2, 2},
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, 7, 536, 0, 296, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, 7, 680, 0, 296, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, 7, 536, 0, 296, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, 7, 648, 0, 296, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, 7, 712, 0, 312, FACING_NORTH, TALK_FACING_KEEP, 0),
    {272, CONDITION_ALWAYS, 1, 0xb, 0, 0, FACING_SOUTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRock_FujinShieldChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, 7, 536, 0, 904, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, 7, 712, 0, 904, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 872, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 872, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TWISTER_ROCK, CONDITION_ALWAYS, 7, 152, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 280, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TWISTER_ROCK, CONDITION_ALWAYS, 7, 536, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gAirsRock_StormBrandChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXAMINE, 20, CONDITION_ALWAYS, EVENT(AirsRock_ExamineWindStone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 8, FLAG_AIRS_ROCK_WALL_CLEARED, EVENT(AirsRock_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 8, FLAG_AIRS_ROCK_WALL_CLEARED, EVENT(AirsRock_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 8, FLAG_AIRS_ROCK_WALL_CLEARED, EVENT(AirsRock_WhirlwindClearsWall)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 9, CONDITION_ALWAYS, EVENT(AirsRock_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 9, CONDITION_ALWAYS, EVENT(AirsRock_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 9, CONDITION_ALWAYS, EVENT(AirsRock_BlowGust)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 10, CONDITION_ALWAYS, EVENT(AirsRock_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 10, CONDITION_ALWAYS, EVENT(AirsRock_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, CONDITION_ALWAYS, EVENT(AirsRock_BlowGust)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 11, CONDITION_ALWAYS, EVENT(AirsRock_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 11, CONDITION_ALWAYS, EVENT(AirsRock_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 11, CONDITION_ALWAYS, EVENT(AirsRock_BlowGust)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 12, CONDITION_ALWAYS, EVENT(AirsRock_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 12, CONDITION_ALWAYS, EVENT(AirsRock_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 12, CONDITION_ALWAYS, EVENT(AirsRock_BlowGust)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 13, CONDITION_ALWAYS, EVENT(AirsRock_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 13, CONDITION_ALWAYS, EVENT(AirsRock_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 13, CONDITION_ALWAYS, EVENT(AirsRock_BlowGust)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRock_EntranceTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(AirsRock_EnterTheRock)},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 9},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AirsRock_LiftPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AirsRock_PlacePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12, FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST, EVENT(AirsRock_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST, EVENT(AirsRock_PlacePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 13, FLAG_AIRS_ROCK_PILLAR_PLACED_SECOND, EVENT(AirsRock_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, FLAG_AIRS_ROCK_PILLAR_PLACED_SECOND, EVENT(AirsRock_PlacePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15, FLAG_AIRS_ROCK_PILLAR_PLACED_THIRD, EVENT(AirsRock_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, FLAG_AIRS_ROCK_PILLAR_PLACED_THIRD, EVENT(AirsRock_PlacePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 16, FLAG_AIRS_ROCK_PILLAR_PLACED_FOURTH, EVENT(AirsRock_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, FLAG_AIRS_ROCK_PILLAR_PLACED_FOURTH, EVENT(AirsRock_PlacePillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 14, FLAG_SCENE_TEMP_00, EVENT(AirsRock_FreezePuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 22, FLAG_AIRS_ROCK_PILLAR_PLACED_FIFTH, EVENT(AirsRock_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 22, FLAG_AIRS_ROCK_PILLAR_PLACED_FIFTH, EVENT(AirsRock_PlacePillar)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(AirsRock_RideWhirlwind)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRock_FujinShieldChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 14},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 20, CONDITION_ALWAYS, EVENT(AirsRock_ClimbLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, EVENT(AirsRock_MoveFirstPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS, EVENT(AirsRock_MoveSecondPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, FLAG_AIRS_ROCK_THIRD_PILLAR_MOVED, EVENT(AirsRock_MoveThirdPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 12, FLAG_AIRS_ROCK_FOG_CLEARED, EVENT(AirsRock_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 12, FLAG_AIRS_ROCK_FOG_CLEARED, EVENT(AirsRock_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 12, FLAG_AIRS_ROCK_FOG_CLEARED, EVENT(AirsRock_WhirlwindClearsFog)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 14, FLAG_AIRS_ROCK_TOP_OPENED, EVENT(AirsRock_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 14, FLAG_AIRS_ROCK_TOP_OPENED, EVENT(AirsRock_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 14, FLAG_AIRS_ROCK_TOP_OPENED, EVENT(AirsRock_WhirlwindOpensTheTop)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(AirsRock_RideWhirlwind)},
    {TRIGGER_TABLE_END},
};

s16 gAirsRock_GustTargets[][4] = {
    {64, 0, 64, 0}, {64, 0, 64, 0}, {64, 0, 64, 0}, {64, 0, 64, 0},
};
// clang-format on

s8 gAirsRock_FogAlpha = 6;
s8 gAirsRock_FogAlphaStep = 1;

s8 gAirsRock_LiftingFogAlpha = 6;
s8 gAirsRock_LiftingFogAlphaStep = 1;

s32 gAirsRock_LeaderLevel = -1;
