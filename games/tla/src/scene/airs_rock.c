/// @file
/// Air's Rock, the great purple rock in the desert south of Yampi, climbed
/// from outside over three screens before the way inside opens.
///
/// On the first, where the party arrives, Felix casts Whirlwind on the Wind
/// Stones to blow away the barriers of sand and rock on the paths, then on
/// the huge twister-shaped rock at the foot of the cliff, whose great
/// whirlwind blows the mud off the wall above it so that it can be climbed.
/// The second is the cliff: statues breathe little whirlwinds that carry
/// Felix along and up it, pillars pushed or moved over gaps close them, a
/// puddle Frost turns into a pillar, and a golden door low on the cliff leads
/// into the rock's great hall. The third holds the ledges below the fog,
/// where the party climbs down level by level and rides the statues'
/// whirlwinds back up, the fog Whirlwind lifts from the wall above, and the
/// top, where Whirlwind on the last twister rock calls down lightning on a
/// pile of rocks and opens the way in (airs_rock_inside.c).

#include "system/task.h"
#include "field/field_sequence.h"
#include "field/field_psynergy_state.h"
#include "types.h"
#include "field/actor.h"
#include "field/event_inline.h"
#include "scene/airs_rock.h"
#include "system/ram_buffer.h"
#include "flags.h"
#include "system/main_routine_state.h"
#include "field/field.h"
#include "field/field_sequence_event.h"
#include "system/frame.h"
#include "field/map_grid.h"
#include "scene/scene.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "field/actor_view.h"
#include "scene/dehkan_plateau.h"
#include "graphics/palette_buffers.h"
#include "macros.h"
#include "text/msg_ids.h"
#include "graphics/resource.h"
#include "battle/battle.h"
#include "game/game.h"
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
#include "field/call.h"
#include "system/bg_scroll.h"

/// Set once the first of the cliff's pillars has dropped into its gap, by the
/// golden door, cell (16, 43).
#define FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST 0x85f
/// As FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST, the second, cell (45, 45).
#define FLAG_AIRS_ROCK_PILLAR_PLACED_SECOND 0x860
/// As FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST, the third, cell (21, 48).
#define FLAG_AIRS_ROCK_PILLAR_PLACED_THIRD 0x861
/// As FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST, the fourth, cell (39, 49).
#define FLAG_AIRS_ROCK_PILLAR_PLACED_FOURTH 0x862
/// As FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST, the fifth, cell (49, 18).
#define FLAG_AIRS_ROCK_PILLAR_PLACED_FIFTH 0x887
/// Set once Move has pushed the pillar in the fog into its gap
/// (AirsRock_MoveThirdPillar).
#define FLAG_AIRS_ROCK_THIRD_PILLAR_MOVED 0x879

/// Set while the left and the right of the two pillars on the ledges below the
/// fog stand moved one cell west (AirsRock_MoveFirstPillar,
/// AirsRock_MoveSecondPillar), so that they stand there when the party comes
/// back.
#define FLAG_FIRST_PILLAR_MOVED_WEST FLAG_AREA_TEMP_10
#define FLAG_SECOND_PILLAR_MOVED_WEST FLAG_AREA_TEMP_11
/// Set while Frost holds the cliff's puddle frozen into a pillar.
#define FLAG_PUDDLE_FROZEN FLAG_SCENE_TEMP_00

/// Story flags: the eight barriers the Wind Stones' gusts blow away, in the
/// order of gAirsRock_GustTargets, then the mud blown from the wall, the fog
/// cleared and the way to the top opened.
#define FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN 0x850
#define FLAG_AIRS_ROCK_WALL_CLEARED 0x858
#define FLAG_AIRS_ROCK_FOG_CLEARED 0x87a
#define FLAG_AIRS_ROCK_TOP_OPENED 0x87b

/// What these rooms mark as the collision code (CELL_CODE) of a cell on the
/// second layer.
enum AirsRockCellMark {
  /// A barrier a gust blows away is marked this plus its kind, a row of
  /// gAirsRock_GustTargets; the cell beyond it holds the barrier's number
  /// plus one, which picks its flag.
  CELL_MARK_FIRST_GUST_TARGET = 30,
  /// A statue's whirlwind fades here.
  CELL_MARK_WHIRLWIND_END = 212,
  /// A gust stops here with nothing to blow, and a whirlwind's rider cannot
  /// land here.
  CELL_MARK_BLOCKED = 255
};

/// The kinds of barrier a gust sweeps away, each a row of
/// gAirsRock_GustTargets, by the way the gust crosses it: a bank of sand
/// across an east-west path the gust blows north or south through a cell at a
/// time, or a mound across a north-south path it blows west through a column
/// at a time.
enum AirsRockGustBarrier {
  GUST_BARRIER_BLOWN_NORTH = 0,
  GUST_BARRIER_BLOWN_SOUTH = 2,
  GUST_BARRIER_BLOWN_WEST
};

/// Clears a barrier a gust blew away on an earlier visit: a mound three cells
/// wide or a bank three cells tall, (x, z) its north-west cell.
#define CLEAR_SAND_MOUND(x, z)                                                 \
  (MapGrid_CopyCellsTo(64, 0, (x) + 64, (z) - 1, 3, 3),                        \
   MapGrid_CopyCellAttributes(64, 0, 3, 2, (x) + 64, z),                       \
   MapGrid_CopyCellAttributes(64, 0, 3, 2, x, z))
#define CLEAR_SAND_BANK(x, z)                                                  \
  (MapGrid_CopyCellsTo(64, 0, (x) + 64, (z) - 1, 1, 4),                        \
   MapGrid_CopyCellAttributes(64, 0, 1, 3, (x) + 64, z),                       \
   MapGrid_CopyCellAttributes(64, 0, 1, 3, x, z))

/// The Storm Brand chest's room, numbering gAirsRock_StormBrandChestPlacements.
enum AirsRockStormBrandChestActorId {
  /// The huge twister-shaped rock Whirlwind clears the wall's mud with.
  ACTOR_TWISTER_ROCK_BY_THE_WALL = ACTOR_FIRST_PLACED,
  /// The five Wind Stones, whose gusts Whirlwind strengthens.
  ///@{
  ACTOR_WIND_STONE_A,
  ACTOR_WIND_STONE_B,
  ACTOR_WIND_STONE_C,
  ACTOR_WIND_STONE_D,
  ACTOR_WIND_STONE_E
  ///@}
};

/// The slopes by the way into the rock, numbering gAirsRock_EntrancePlacements.
enum AirsRockEntranceActorId {
  /// The four statues on the slopes that breathe whirlwinds.
  ///@{
  ACTOR_FIRST_SLOPE_STATUE = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_SLOPE_STATUE,
  ACTOR_THIRD_SLOPE_STATUE,
  ACTOR_FOURTH_SLOPE_STATUE,
  ///@}
  /// The pillars that drop into the cliff's gaps, each with its
  /// FLAG_AIRS_ROCK_PILLAR_PLACED_ flag.
  ACTOR_FIRST_LEDGE_PILLAR,
  ACTOR_SECOND_LEDGE_PILLAR,
  /// The puddle Frost freezes into a pillar.
  ACTOR_PUDDLE,
  ACTOR_THIRD_LEDGE_PILLAR,
  ACTOR_FOURTH_LEDGE_PILLAR,
  /// The four statues hung on the cliff that breathe whirlwinds.
  ///@{
  ACTOR_FIRST_CLIFF_STATUE,
  ACTOR_SECOND_CLIFF_STATUE,
  ACTOR_THIRD_CLIFF_STATUE,
  ACTOR_FOURTH_CLIFF_STATUE,
  ///@}
  /// The statue exhaling little whirlwinds to the left.
  ACTOR_BREATHING_STATUE,
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
  /// The left and right of the two pillars on the ledges below the fog.
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

/// The slopes' own ways in, where the leader climbs between the lower slopes
/// and the cliff above them.
enum AirsRockEntranceEntrance {
  /// At the top of the lower slopes' wall, climbing down from the cliff.
  ENTRANCE_DOWN_FROM_THE_CLIFF_WEST = 5,
  ENTRANCE_DOWN_FROM_THE_CLIFF_EAST,
  /// At the foot of the cliff, climbing up from the lower slopes.
  ENTRANCE_UP_THE_CLIFF_WEST = 10,
  ENTRANCE_UP_THE_CLIFF_EAST
};

/// The Fujin Shield chest's room's own ways in: off its ledges onto the paths
/// west of them and back, and up and down its wall into and out of the fog.
enum AirsRockFujinShieldChestEntrance {
  /// On the ledges, back from the lower and the upper path.
  ENTRANCE_LEDGES_FROM_LOWER_PATH = 2,
  ENTRANCE_LEDGES_FROM_UPPER_PATH,
  /// On the paths, from the ledges.
  ENTRANCE_LOWER_PATH,
  ENTRANCE_UPPER_PATH,
  /// On the wall below the fog, climbing down out of it.
  ENTRANCE_BELOW_THE_FOG_WEST,
  ENTRANCE_BELOW_THE_FOG_EAST,
  /// On the wall in the fog, climbing up into it; while the fog lasts, the top
  /// of the wall leads back here.
  ENTRANCE_INTO_THE_FOG_WEST,
  ENTRANCE_INTO_THE_FOG_EAST,
  /// At the top of the wall in the fog, climbing down from above it.
  ENTRANCE_DOWN_INTO_THE_FOG_WEST,
  ENTRANCE_DOWN_INTO_THE_FOG_EAST,
  /// On the wall above the fog, climbing up out of it once it is cleared.
  ENTRANCE_ABOVE_THE_FOG_WEST,
  ENTRANCE_ABOVE_THE_FOG_EAST,
  /// Unused: no exit leads here. The way to the top opens at once
  /// (AirsRock_WhirlwindOpensTheTop).
  ENTRANCE_OPEN_THE_TOP = 99
};
#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_create_silhouette.c"

#include "shared/scene_create_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

/// Clears the actor's sprite flags, as a script's call; always 0.
s32 AirsRock_ClearScriptedActorSpriteFlags(s32 actorId) {
  Actor_SetSpriteFlags(actorId, 0);
  return 0;
}

#include "../field/map_cell_get.c"

#include "shared/map_cell_set.c"

#include "../field/actor_wait_until_landed.c"

#undef EFFECT_SET_SCRIPT

void Actor_PlaceMovedBlock(void);

#include "shared/blocks_mark_cells.c"

/// Frees the cells ahead of and behind a block that Blocks_MarkCells blocked,
/// and clears its mark.
void Blocks_UnmarkCells(struct Actor *block) {
  struct MapState *map;

  if (block->sceneByte != 0) {
    map = Ram_HeapSlots->mapGrid;
    if (map != NULL) {
      struct MapCell **cells = &map->layers[2].cells;
      struct MapCell *cell = *cells;
      s32 x = FIELD_CELL(block->x.fixed);
      s32 row = FIELD_CELL(block->z.fixed - block->y.fixed);

      cell += x + ((row + 1) << 7);
      cell->collisionCode = 0;
      cell = *cells;
      cell += x + ((row - 1) << 7);
      cell->collisionCode = 0;
      block->sceneByte = 0;
    }
  }
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern struct SceneRegion gAirsRock_FujinShieldChestRegions[];

const struct SceneRegion *Scene_GetExitRegions(void) {
  s32 scene = gMainRoutineState.next;

  if (scene == SCENE_AIRS_ROCK_STORM_BRAND_CHEST)
    return gAirsRock_StormBrandChestRegions;
  if (scene == SCENE_AIRS_ROCK_ENTRANCE)
    return gAirsRock_EntranceRegions;
  return gAirsRock_FujinShieldChestRegions;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s32 scene = gMainRoutineState.next;

  if (scene == SCENE_AIRS_ROCK_STORM_BRAND_CHEST)
    return gAirsRock_StormBrandChestPlacements;
  if (scene == SCENE_AIRS_ROCK_ENTRANCE)
    return gAirsRock_EntrancePlacements;
  return gAirsRock_FujinShieldChestPlacements;
}

/// Felix reads a Wind Stone on the first screen: "Beware the Wind Stones."
/// and its warning that the stones magnify the wind.
void AirsRock_ExamineWindStone(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  // "Beware the Wind Stones." "The stones magnify the power of the wind. Use
  // caution when approaching Air's Rock."
  Message_ShowCentered((s32)MsgFieldWindStoneWarning, 1);
  Event_End();
}

s32 TilesetAnim_Resume();

/// Felix walks north into the golden door low on the cliff, beside the first
/// pillar's gap (cell (19, 43)): the door slides open, he walks in and the
/// screen wipes to the great hall inside Air's Rock.
void AirsRock_EnterTheRock(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_CopyCellsTo(64, 64, 83, 35, 1, 2);
  TilesetAnim_Resume(0);
  FieldEvent_Wait(45);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.5);
  Actor_WalkTo(gMainRoutineState.leaderActorId, 312, 680);
  FieldEvent_Wait(5);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldActor_WaitForMoveById(gMainRoutineState.leaderActorId);
  Event_PlaySound(SE_MAP_EXIT);
  FieldEvent_SetExitRequest(AIRS_ROCK_ENTRANCE_TO_AIRS_ROCK_INSIDE_HALL);
  Event_End();
}

/// Felix casts Move on the left of the two pillars on the ledges below the
/// fog. Nothing more happens on screen than the move itself; the game
/// remembers whether it now stands one cell west of where it started
/// (column 31), so that it is still there when the party comes back.
void AirsRock_MoveFirstPillar(void) {
  struct Actor *actor;

  actor = Actor_Get(ACTOR_FIRST_PILLAR);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  if (FIELD_CELL(actor->x.fixed) == 31) {
    Event_SetFlag(FLAG_FIRST_PILLAR_MOVED_WEST);
  } else {
    Event_ClearFlag(FLAG_FIRST_PILLAR_MOVED_WEST);
  }
  Event_End();
}

/// Felix casts Move on the right of the two pillars on the ledges below the
/// fog, from across the gap east of them. With the left pillar moved one cell
/// west, the hand pulls this one west beside it, and the cell where it stood,
/// (33, 54), is open to walk on: Felix jumps the gap west onto it, and stepping
/// south off it he drops to the ledge below (AirsRock_DropOffLedge), where a
/// whirlwind carries him up past the pillars' ledge to a ledge above
/// (AirsRock_RideWhirlwind). The game remembers the pillar there; anywhere
/// else that cell stays closed.
///
/// The ledge's cell is changed in the copy of it kept for the level it is on
/// (AirsRock_SetLedgeCellsByLevel) and, while Felix stands on that level, in
/// the map itself.
void AirsRock_MoveSecondPillar(void) {
  struct Actor *actor;
  struct Actor *leader;
  s32 value;

  actor = Actor_Get(ACTOR_SECOND_PILLAR);
  value = MapCell_GetAttributes(3, 70, 67);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  if (FIELD_CELL(actor->x.fixed) == 32) {
    Event_SetFlag(FLAG_SECOND_PILLAR_MOVED_WEST);
    value &= CELL_ATTRIBUTE_BITS;
    MapCell_SetAttributes(4, 70, 67, value);
    MapCell_SetAttributes(2, 33, 54, value);
  } else {
    Event_ClearFlag(FLAG_SECOND_PILLAR_MOVED_WEST);
    value = CELL_ATTRIBUTES(CELL_MARK_BLOCKED, value & CELL_ATTRIBUTE_BITS);
    MapCell_SetAttributes(4, 70, 67, value);
    MapCell_SetAttributes(2, 33, 54, value);
  }
  leader = Actor_Get(gMainRoutineState.leaderActorId);
  if (CELL_HEIGHT_STEPS(leader->y.fixed) == 26)
    MapGrid_CopyCellAttributes(70, 67, 1, 1, 33, 54);
  Event_End();
}

/// Felix casts Move on the pillar in the fog: pushed one cell east into the
/// gap (cell (11, 17)), it stays there for good, a way across the gap and a
/// shortcut back.
void AirsRock_MoveThirdPillar(void) {
  struct Actor *actor;

  actor = Actor_Get(ACTOR_THIRD_PILLAR);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  if (FIELD_HALF_CELL(actor->x.fixed) == 23) {
    Event_SetFlag(FLAG_AIRS_ROCK_THIRD_PILLAR_MOVED);
  }
  Event_End();
}

/// Felix walks south off the edge of the ledges below the fog, in front of the
/// two pillars (cell (33, 55), on the second level): he hops over the edge,
/// lowers himself a little way down the rock face, lets go and drops to the
/// ledge below.
void AirsRock_DropOffLedge(void) {
  struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);
  FIXED z = leader->z.fixed;
  u32 i;

  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
  System_Wait(10);
  Actor_SelectAnimation(leader, ANIM_STAND);
  Actor_SetSpriteFlags(leader, 0);
  leader->motionFlags |= ACTOR_FALLS;
  leader->velocityY = toFIXED(4);
  FieldActor_SetMoveTargetFX(leader, leader->x.fixed, leader->y.fixed,
                             z + toFIXED(12));
  System_Wait(6);
  Event_PlaySound(SE_FLOOR_SWITCH);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH_SOUTHWEST,
                      0);
  leader->motionFlags = 0;
  for (i = 0; i < 14; i++) {
    leader->y.fixed -= toFIXED(2);
    leader->targetY = leader->y.fixed;
    System_Wait(1);
  }
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  leader->velocityY = toFIXED(3);
  FieldActor_SetMoveTargetFX(leader, leader->x.fixed, leader->y.fixed,
                             z + toFIXED(16));
  FieldActor_WaitForMove(leader);
  Actor_SetSpriteFlags(leader, 1);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Event_NoOp();
}

const struct Trigger *Scene_GetTriggers(void) {
  s32 scene = gMainRoutineState.next;

  if (scene == SCENE_AIRS_ROCK_STORM_BRAND_CHEST)
    return gAirsRock_StormBrandChestTriggers;
  if (scene == SCENE_AIRS_ROCK_ENTRANCE)
    return gAirsRock_EntranceTriggers;
  return gAirsRock_FujinShieldChestTriggers;
}

/// Nothing visible happens of its own as Felix starts casting Whirlwind on a
/// Wind Stone or a twister rock; the game clears what is pending so that the
/// cast can go on to the stone's or rock's own whirlwind
/// (AirsRock_TakeOverWhirlwind).
void AirsRock_StartWhirlwind(void) { Event_EndLastingPsynergy(0); }

/// Nothing visible happens of its own as Felix casts Whirlwind on a Wind
/// Stone or a twister rock; the game tells Whirlwind to leave out its usual
/// little whirlwind, so that what follows is the stone's or the rock's own
/// (AirsRock_BlowGust and the rest).
void AirsRock_TakeOverWhirlwind(void) {
  ((struct FieldEffectState *)Ram_HeapSlots->fieldEffectState)->sceneVariant =
      1;
}

/// The turn a gust's spark makes each frame: a lap every 32 frames.
#define GUST_LAP_STEP (TRIG_TURN / 32)

/// A gust's spark: circles the centre kept in bounce and sceneWord, a lap every
/// 32 frames, its radius growing as it rises.
void AirsRock_SpiralGust(struct Actor *actor) {
  s32 radius = actor->y.fixed;
  s32 step = actor->speedLimit;

  radius >>= 1;
  radius += toFIXED(16);
  actor->x.fixed = actor->bounce +
                   Iwram_MulFX(Trig_Cos((step & 0xff) * GUST_LAP_STEP), radius);
  actor->z.fixed =
      actor->sceneWord +
      Iwram_MulFX(Math_Sin((actor->speedLimit & 0xff) * GUST_LAP_STEP),
                  radius) /
          3;
  actor->y.fixed += actor->gravity;
  actor->speedLimit++;
}

/// The same circle on a smaller radius, twice as wide, blinking blue every
/// other two frames.
void AirsRock_SpiralSpark(struct Actor *actor) {
  s32 radius = actor->y.fixed;
  s32 step = actor->speedLimit;

  radius >>= 2;
  radius += toFIXED(12);
  actor->x.fixed =
      actor->bounce +
      Iwram_MulFX(Trig_Cos((step & 0xff) * GUST_LAP_STEP), radius) * 2;
  actor->z.fixed =
      actor->sceneWord +
      Iwram_MulFX(Math_Sin((actor->speedLimit & 0xff) * GUST_LAP_STEP), radius);
  actor->y.fixed += actor->gravity;
  actor->speedLimit++;
  FieldActor_SetSpriteColor(actor,
                            ((gFrameCount & 2) >> 1) * SPRITE_COLOR_BLUE_TINT);
}

/// A Whirlwind target's update while the gust gathers: it flickers to the
/// colour mode in sceneCounter (a SpriteColorMode), at the rate sceneByte
/// sets.
void AirsRock_FlickerTarget(struct Actor *actor) {
  u32 on = ((gFrameCount & 3) >> actor->sceneByte) & 1;

  FieldActor_SetSpriteColor(actor, (u8)(on * actor->sceneCounter));
}

// The Whirlwind gust in Air's Rock.

extern s16 gAirsRock_GustTargets[][4];

/// Felix casts Whirlwind on one of the five Wind Stones on the first screen:
/// a whirlwind rises from the stone, which flickers as it grows, then blows
/// off the way Felix faces, sparkling as it goes. If a barrier of sand or
/// rocks stands in its path it blows it away for good, opening the path;
/// with nothing in its path it fades at the next wall with a whoosh.
///
/// The gust looks ahead along the second layer for the first cell with a
/// mark: a wall (CELL_MARK_BLOCKED) or a barrier (CELL_MARK_FIRST_GUST_TARGET
/// plus its kind). It travels to that cell, sweeps a barrier's cells away as
/// it blows three cells on, and sets the flag counted from
/// FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN by the cell beyond it.
void AirsRock_BlowGust(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct FieldEffectState *effect = Ram_HeapSlots->fieldEffectState;
  struct Actor *gust, *spark;
  struct Vector3 velocity;
  struct EffectOptions glints, dust;
  struct Cell delta, cell;
  s32 newZ, newX, flag = 0, steps = 0, hit, angle, i;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Sound_Play(SE_RUSH);
  gust = FieldActor_Create(SPRITE_WHIRLWIND, leader->x.fixed, leader->y.fixed,
                           leader->z.fixed);
  Actor_SelectAnimation(gust, 2);
  Actor_SetSpriteFlags(gust, 0);
  gust->scaleX = FIXED_ONE * 17 / 10;
  gust->scaleY = FIXED_ONE / 5;
  gust->motionFlags = 0;
  ((struct FieldSprite *)gust->sprite)->priority =
      ((struct FieldSprite *)effect->target->sprite)->priority;
  Actor_SetMoveTarget(gust, effect->target->x.fixed, effect->target->y.fixed,
                      effect->target->z.fixed - toFIXED(1));
  while (gust->scaleY < FIXED_ONE * 13 / 10) {
    gust->scaleY += FIXED_ONE / 10;
    Event_Wait(1);
  }
  Actor_WaitForMove(gust);
  gust->z.fixed = effect->target->z.fixed - toFIXED(1);
  while (gust->scaleY < FIXED_ONE * 18 / 10) {
    if ((gFrameCount & 3) == 0) {
      spark = FieldActor_Create(SPRITE_PARTICLE, effect->target->x.fixed, 0,
                                effect->target->z.fixed);
      spark->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      spark->motionFlags = 0;
      spark->bounce = spark->x.fixed;
      spark->gravity = toFIXED(4) / 5;
      spark->sceneWord = spark->z.fixed;
      spark->speedLimit = Random16() & 0xffff;
      Actor_SelectAnimation(spark, 7);
      Actor_StartScript(spark, gFieldEffect_MediumScript);
      Actor_SetSpriteFlags(spark, 0);
      Actor_SetSpriteColor(spark, SPRITE_COLOR_RBG);
      spark->update = AirsRock_SpiralGust;
    }
    if (gFrameCount & 1) {
      gust->scaleX += FIXED_ONE / 100;
      gust->scaleY += FIXED_ONE / 100;
      Actor_SetSpriteColor(effect->target, SPRITE_COLOR_HIDDEN);
    } else {
      Actor_SetSpriteColor(effect->target, SPRITE_COLOR_GBR);
    }
    Event_Wait(1);
  }
  Actor_SetSpriteColor(effect->target, SPRITE_COLOR_NORMAL);
  Sound_Play(SE_WHOOSH);
  cell.x = FIELD_CELL(effect->target->x.fixed);
  cell.z = FIELD_CELL(effect->target->z.fixed);
  // The leader's facing rounded to the nearest of east, south, west and north.
  angle = FACING_QUARTER(leader->facing + DEGtoANG(45));
  delta.x = FIXEDtoINT(Trig_Cos(angle));
  delta.z = FIXEDtoINT(Trig_Sin(angle));
  do {
    hit = MapCell_GetAttributes(2, cell.x, cell.z);
    hit = CELL_CODE(hit);
    cell.x += delta.x;
    cell.z += delta.z;
    steps += 2;
  } while (hit == 0);
  flag = MapCell_GetAttributes(2, cell.x, cell.z);
  flag = CELL_CODE(flag);
  flag -= 1;
  steps -= 2;
  newX = effect->target->x.part[0].integer + ((steps * delta.x) << 3);
  newZ = effect->target->z.part[0].integer + ((steps * delta.z) << 3);
  ((struct FieldSprite *)gust->sprite)->priority = 1;
  // cell takes the gust's target in pixels: the ROM stores both words in
  // cell's slots before the move, though nothing reads them (a later arm
  // stores its own target there and reads it back).
  cell.x = newX;
  cell.z = newZ;
  gust->speedLimit = toFIXED(4);
  gust->acceleration = toFIXED(4);
  Actor_SetMoveTarget(gust, INTtoFIXED(cell.x), gust->y.fixed,
                      INTtoFIXED(cell.z));
  FieldCamera_SetSpeedAndAcceleration(5, 5);
  FieldCamera_MoveToFX(INTtoFIXED(newX), -1, INTtoFIXED(newZ), 1);
  while (Actor_HasArrived(gust) == 0) {
    if ((gFrameCount & 1) == 0) {
      glints.priority = 1;
      glints.type = SPRITE_GLINT;
      glints.palette = 2;
      velocity.x = Trig_Cos((gFrameCount & 0x1ff) * FACING_STEP) << 1;
      velocity.y = gust->y.fixed + INTtoFIXED(Random16() & 31);
      velocity.z = Trig_Sin((gFrameCount & 0x1ff) * FACING_STEP);
      FieldEffect_Spawn(
          gust->x.fixed, velocity.y, gust->z.fixed, velocity.x, 0, velocity.z,
          EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &glints);
    }
    Event_Wait(1);
  }
  if (hit == CELL_MARK_BLOCKED) {
    Sound_Play(SE_WHOOSH);
  } else {
    cell.x = gust->x.part[0].integer + delta.x * 48;
    cell.z = gust->z.part[0].integer + delta.z * 48;
    gust->speedLimit = toFIXED(1) / 2;
    gust->acceleration = toFIXED(1) / 2;
    Actor_SetMoveTarget(gust, INTtoFIXED(cell.x), gust->y.fixed,
                        INTtoFIXED(cell.z));
    Sound_Play(SE_GUST);
    hit -= CELL_MARK_FIRST_GUST_TARGET;
    while (Actor_HasArrived(gust) == 0) {
      if ((gFrameCount & 1) == 0) {
        dust.priority = 1;
        dust.startScaleX = dust.startScaleY = (Random16() & 0xffff) + FIXED_ONE;
        velocity.x = Trig_Cos((gFrameCount & 0x1ff) * FACING_STEP) << 1;
        velocity.y = toFIXED(1);
        velocity.z = Trig_Sin((gFrameCount & 0x1ff) * FACING_STEP);
        FieldEffect_Spawn(gust->x.fixed, gust->y.fixed, gust->z.fixed,
                          velocity.x, velocity.y, velocity.z,
                          EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY, &dust);
      }
      if ((gFrameCount & 3) == 0) {
        cell.x = gAirsRock_GustTargets[hit][2] + FIELD_CELL(gust->x.fixed);
        cell.z = gAirsRock_GustTargets[hit][3] + FIELD_CELL(gust->z.fixed);
        if (hit & 1) {
          MapGrid_CopyCellsTo(gAirsRock_GustTargets[hit][0],
                              gAirsRock_GustTargets[hit][1], cell.x, cell.z - 1,
                              1, 3);
          MapGrid_CopyCellAttributes(gAirsRock_GustTargets[hit][0],
                                     gAirsRock_GustTargets[hit][1], 1, 2,
                                     cell.x, cell.z);
          MapGrid_CopyCellAttributes(
              gAirsRock_GustTargets[hit][0], gAirsRock_GustTargets[hit][1], 1,
              2, FIELD_CELL(gust->x.fixed), FIELD_CELL(gust->z.fixed));
        } else {
          MapGrid_CopyCellsTo(gAirsRock_GustTargets[hit][0],
                              gAirsRock_GustTargets[hit][1], cell.x,
                              cell.z - (hit >> 1), 1, 1);
          MapGrid_CopyCellAttributes(gAirsRock_GustTargets[hit][0],
                                     gAirsRock_GustTargets[hit][1], 1, 1,
                                     cell.x, cell.z);
          MapGrid_CopyCellAttributes(
              gAirsRock_GustTargets[hit][0], gAirsRock_GustTargets[hit][1], 1,
              1, FIELD_CELL(gust->x.fixed), FIELD_CELL(gust->z.fixed));
        }
        MapGrid_CenterOnFollowedPoint();
      }
      Event_Wait(1);
    }
    // Blown south, the sweep clears the row behind the gust, so the row it
    // stops on is cleared last.
    if (hit == GUST_BARRIER_BLOWN_SOUTH) {
      MapGrid_CopyCellsTo(gAirsRock_GustTargets[2][0],
                          gAirsRock_GustTargets[2][1], cell.x, cell.z, 1, 1);
      MapGrid_CenterOnFollowedPoint();
    }
    Sound_Play(SE_STOP_EXIT_SE);
    Event_SetFlag(flag + FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN);
  }
  gust->acceleration = 0;
  gust->speedLimit = 0;
  gust->targetZ = 0;
  gust->targetY = 0;
  gust->targetX = 0;
  for (i = 0; i != 8; i++) {
    gust->scaleX -= FIXED_ONE * 3 / 20;
    gust->scaleY -= FIXED_ONE / 4;
    gust->y.fixed += toFIXED(7) / 2;
    Event_Wait(1);
  }
  Actor_Release(gust);
  Event_Wait(30);
  FieldCamera_MoveToActorById(gMainRoutineState.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  Event_End();
}

/// The line scroll of the great whirlwind or the fog, a halfword a line; three
/// a line while the fog lifts.
s16 gAirsRock_WaveLines[DISPLAY_HEIGHT * 3] BSS_DATA ALIGNED(16);

/// The great whirlwind's sway: BG0's line scroll, by HBlank DMA.
#define WAVE_SCROLL AirsRock_ScrollWave
#define WAVE_SCROLL_LINES gAirsRock_WaveLines
#include "../field/field_effect_wave_scroll.c"

/// A cloud of the wall's mud swept round the great whirlwind: west of x 320 it
/// speeds up eastward and grows, past it westward and shrinks, passing in
/// front of the whirlwind going east and behind it going west, and it spins.
void AirsRock_SwirlDust(struct Actor *actor) {
  FIXED scale;

  actor->y.fixed += actor->gravity;
  actor->x.fixed += actor->bounce;
  actor->z.fixed += actor->sceneWord;
  if (FIELD_HALF_CELL(actor->x.fixed) <= 39) {
    actor->bounce += toFIXED(3) / 2;
    scale = actor->scaleX + (FIXED_ONE / 20);
  } else {
    actor->bounce -= toFIXED(3) / 2;
    scale = actor->scaleX - (FIXED_ONE / 50);
  }
  actor->scaleX = scale;
  actor->scaleY = scale;
  if (actor->bounce > 0)
    Scene_SetActorSpritePriority(actor, 1);
  else
    Scene_SetActorSpritePriority(actor, 2);
  ((struct FieldSprite *)actor->sprite)->rotation += TRIG_TURN * 3 / 32;
}

/// The bytes of one row of the whirlwind's tiles.
#define WHIRLWIND_TILE_ROW_SIZE (WHIRLWIND_ROW_TILES * TILE_SIZE_4BPP)
/// Where each row of the whirlwind's map is copied into its screen block row,
/// in bytes: the word copy drops the odd bytes, so the picture's rows start at
/// column 6.
#define WHIRLWIND_MAP_ROW_OFFSET 15

/// Felix casts Whirlwind on the huge twister-shaped rock below the cliff on
/// the first screen: it glows, the view pans north to the wall caked in
/// mud, and a great whirlwind rises and blows the mud away, baring the
/// holds that climb it. The rock turns grey, spent.
void AirsRock_WhirlwindClearsWall(void) {
  struct MainRoutineState *state;
  u16 *buffer = Heap_Alloc(2 * WHIRLWIND_TILE_ROW_SIZE);
  u16 *cell;
  struct Actor *spark;
  u32 i;
  FIXED x;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_WHIRLWIND_TILES), buffer);
  Dma_Set(buffer, BG_CHAR_BLOCK(3) + WHIRLWIND_TOP_TILE * TILE_SIZE_4BPP,
          DMA_COPY_WORDS(WHIRLWIND_TILE_ROW_SIZE / 4), REG_DMA3);
  Dma_Set((u8 *)buffer + WHIRLWIND_TILE_ROW_SIZE,
          BG_CHAR_BLOCK(3) + WHIRLWIND_BOTTOM_TILE * TILE_SIZE_4BPP,
          DMA_COPY_WORDS(WHIRLWIND_TILE_ROW_SIZE / 4), REG_DMA3);
  Heap_Release(buffer);
  FieldActor_SetSpritePriority(gMainRoutineState.leaderActorId, 1);
  FieldActor_SetSpritePriority(ACTOR_TWISTER_ROCK_BY_THE_WALL, 1);
  Io_Write16(BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(3) | BGCNT_SCREEN_BASE(4),
             &REG_BG0CNT);
  REG_BG1CNT &= ~BGCNT_PRIORITY(3);
  REG_BG1CNT |= BGCNT_PRIORITY(2);
  Dma_Fill16(WHIRLWIND_FILL_TILE, BG_SCREEN_BLOCK(4), 1280);
  buffer = Heap_Alloc(WHIRLWIND_MAP_HEIGHT * WHIRLWIND_MAP_WIDTH * sizeof(u16));
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_WHIRLWIND_MAP), buffer);
  cell = buffer;
  for (i = 0; i != WHIRLWIND_MAP_HEIGHT * WHIRLWIND_MAP_WIDTH; i++) {
    *cell -= WHIRLWIND_MAP_FIRST_TILE - WHIRLWIND_TOP_TILE;
    cell++;
  }
  cell = buffer;
  for (i = 0; i != WHIRLWIND_MAP_HEIGHT; i++) {
    Dma_Set(cell,
            BG_SCREEN_BLOCK(4) + WHIRLWIND_MAP_ROW_OFFSET +
                i * BG_SCREEN_WIDTH * sizeof(u16),
            DMA_COPY_WORDS(WHIRLWIND_MAP_WIDTH / 2), REG_DMA3);
    cell += WHIRLWIND_MAP_WIDTH;
  }
  Heap_Release(buffer);
  Event_PlaySound(SE_GLITTER);
  Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->sceneCounter =
      SPRITE_COLOR_BLUE_TINT;
  Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->sceneByte = 1;
  Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->update = AirsRock_FlickerTarget;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = FieldActor_Create(
          SPRITE_PARTICLE, Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->x.fixed,
          Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->y.fixed,
          Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->z.fixed);
      ((struct FieldSprite *)spark->sprite)->priority =
          ((struct FieldSprite *)Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)
               ->sprite)
              ->priority;
      spark->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      spark->motionFlags = 0;
      spark->bounce = spark->x.fixed;
      spark->gravity = toFIXED(3) / 2;
      spark->sceneWord = spark->z.fixed;
      spark->speedLimit = Random16() & 0xffff;
      Actor_SelectAnimation(spark, 7);
      Actor_StartScript(spark, gFieldEffect_MediumScript);
      Actor_SetSpriteFlags(spark, 0);
      FieldActor_SetSpriteColor(spark, SPRITE_COLOR_RBG);
      spark->update = AirsRock_SpiralSpark;
    }
    FieldEvent_Wait(1);
  }
  EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(330, CAMERA_KEEP, 73, 1);
  FieldCamera_WaitForArrival();
  Task_AddFrame(AirsRock_ScrollWave, TASK_PASS_VBLANK);
  Event_PlaySound(SE_WAVE_HIGH);
  REG_BLDALPHA = BLEND_ALPHA(0, 15);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_DISPCNT |= DISPCNT_BG0_ON;
  MapGrid_SetShake(1, 1, 1);
  for (i = 0; i != 64; i++) {
    REG_BLDALPHA = ((BLEND_WEIGHT_MAX - 1 - (i >> 2)) << 8) | (i >> 2);
    Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->sceneCounter = Random16() & 7;
    FieldEvent_Wait(1);
  }
  FieldEvent_Wait(60);
  REG_BLDALPHA = BLEND_ALPHA(15, 0);
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3;
  MapGrid_SetShake(3, 3, 1);
  {
    u32 j = 0;

    do {
      j++;
      if ((j & 3) == 0) {
        struct Vector3 velocity;
        struct EffectOptions options;

        options.priority = 1;
        options.update = AirsRock_SwirlDust;
        velocity.x = 0;
        velocity.y = 0;
        velocity.z = -toFIXED(2);
        x = INTtoFIXED(Random16() % 144) + toFIXED(256);
        FieldEffect_Spawn(x, 0, INTtoFIXED(Random16() % 144), velocity.x,
                          velocity.y, velocity.z,
                          EFFECT_USE_UPDATE | EFFECT_USE_PRIORITY |
                              EFFECT_SCRIPT_MEDIUM,
                          &options);
      }
      REG_BLDALPHA =
          (((j >> 4) & 0xf) << 8) | (BLEND_WEIGHT_MAX - ((j >> 4) & 0xf));
      Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->sceneCounter = Random16() & 7;
      FieldEvent_Wait(1);
    } while (j != 255);
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
  Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->sceneCounter = SPRITE_COLOR_GREY;
  for (i = 0; i != 65; i++) {
    REG_BLDALPHA = ((i >> 2) << 8) | (BLEND_WEIGHT_MAX - (i >> 2));
    FieldEvent_Wait(1);
  }
  FieldTask_Remove(AirsRock_ScrollWave);
  Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->sceneByte = 0;
  System_Wait(30);
  Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->update = NULL;
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL),
                            SPRITE_COLOR_GREY);
  FieldMapGrid_WaitForShakeToSettle();
  FieldEvent_Wait(60);
  REG_BLDCNT = 0;
  Window_LoadGraphics();
  DebugText_Clear();
  state = &gMainRoutineState;
  WindowColor_Apply(state->windowHue, state->windowBrightness);
  REG_BG1CNT &= ~BGCNT_PRIORITY(3);
  REG_BG1CNT |= BGCNT_PRIORITY(1);
  {
    s32 *ownerId = &state->leaderActorId;

    Actor_Get(*ownerId)->priorityFlags &= ~ACTOR_PRIORITY_IGNORES_RISES;
    Actor_Get(*ownerId)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    {
      u8 *flags = &Actor_Get(ACTOR_TWISTER_ROCK_BY_THE_WALL)->priorityFlags;
      u8 v = *flags | ACTOR_PRIORITY_AUTOMATIC;

      *flags = v;
    }
    EventCamera_FollowActor(*ownerId, 1);
  }
  FieldCamera_WaitForArrival();
  Event_SetFlag(FLAG_AIRS_ROCK_WALL_CLEARED);
  Event_End();
}

/// Felix is caught by one of the little whirlwinds the cliff's statues
/// breathe: he spins round on top of it as it carries him along, and where
/// it fades he jumps off. On a whirlwind blowing up the cliff, he has a
/// moment to hold Up, Left or Right and choose which way to leap onto the
/// ledge at its top; otherwise he drops off where it ends.
void AirsRock_RideWhirlwind(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct Actor *ride = &leader->watchedActor->actor;
  struct Actor *statue, *camera;
  s32 rise;
  FIXED dx, dz;
  s32 moved, i, height;
  s32 angle;
  u16 facing;

  Event_Start();
  Event_EndLastingPsynergy(0);
  leader->motionFlags = ACTOR_FLOATS;
  Actor_SetSpriteFlags(leader, 0);
  GameFlag_Set(FLAG_RIDING_WHIRLWIND);
  Sound_Play(SE_KIRIN);
  rise = 0;
  while (ride->sceneByte != 0) {
    leader->x.fixed = ride->x.fixed;
    leader->y.fixed = ride->y.fixed + rise;
    leader->z.fixed = ride->z.fixed;
    if (rise < toFIXED(12))
      rise += toFIXED(1) / 10;
    leader->facing = (u8)gFrameCount * FACING_STEP;
    leader->priorityFlags ^= ACTOR_PRIORITY_UNDERFOOT << (gFrameCount & 1);
    wait(1);
  }
  Sound_Play(SE_STOP_ACTION_SE);
  facing = ride->facing;
  if (facing == FACING_NORTH) {
    statue = &ride->watchedActor->actor;
    dx = 0;
    dz = 0;
    leader->priorityFlags = ACTOR_PRIORITY_OVERHEAD;
    leader->x.fixed =
        FIELD_HALF_CELL_ORIGIN((FIELD_HALF_CELL(leader->x.fixed) & ~1) + 1);
    leader->y.fixed =
        FIELD_HALF_CELL_ORIGIN((FIELD_HALF_CELL(ride->y.fixed) & ~1) + 3);
    leader->z.fixed =
        FIELD_HALF_CELL_ORIGIN((FIELD_HALF_CELL(leader->z.fixed) & ~1) + 1);
    for (i = 0; i != 60; i++) {
      moved = 0;
      if (gInput.held & KEY_UP) {
        dx = 0;
        dz = -toFIXED(16);
        leader->facing = FACING_NORTH;
        moved = 1;
      }
      if (gInput.held & KEY_RIGHT) {
        dx = toFIXED(16);
        dz = 0;
        leader->facing = FACING_EAST;
        moved = 1;
      }
      if (gInput.held & KEY_LEFT) {
        dx = -toFIXED(16);
        leader->facing = FACING_WEST;
        dz = 0;
        moved = 1;
      }
      if (moved && CELL_CODE(MapCell_GetAttributes(
                       2, FIELD_CELL(ride->x.fixed + dx),
                       FIELD_CELL(ride->z.fixed - ride->y.fixed + dz))) !=
                       CELL_MARK_BLOCKED)
        goto found;
      wait(1);
    }
    dx = 0;
    leader->facing = FACING_NORTH;
    dz = -toFIXED(16);
    if ((u16)CELL_CODE(MapCell_GetAttributes(
            2, FIELD_CELL(ride->x.fixed),
            FIELD_CELL(ride->z.fixed - ride->y.fixed + dz))) ==
        CELL_MARK_BLOCKED) {
      dx = toFIXED(16);
      dz = 0;
      leader->facing = FACING_EAST;
      if ((u16)CELL_CODE(MapCell_GetAttributes(
              2, FIELD_CELL(ride->x.fixed + dx),
              FIELD_CELL(ride->z.fixed - ride->y.fixed))) ==
          CELL_MARK_BLOCKED) {
        dx = -toFIXED(16);
        leader->facing = FACING_WEST;
      }
    }
  found:
    // The statue's sceneCounter holds the layer of the ledge he lands on.
    height = Map_GetTerrainHeight(statue->sceneCounter, ride->x.fixed + dx,
                                  ride->z.fixed + dz);
    leader->y.fixed = height;
    leader->groundHeight = height;
    Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(leader, ANIM_LEADER_LEAP);
    leader->speedLimit = toFIXED(2);
    leader->acceleration = toFIXED(1) / 2;
    leader->velocityY = toFIXED(4);
    leader->motionFlags = ACTOR_FALLS;
    leader->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    leader->layer = statue->sceneCounter;
    FieldActor_SetMoveTargetFX(leader, leader->x.fixed + dx, leader->y.fixed,
                               leader->z.fixed + dz);
    Actor_WaitUntilLanded(leader);
    FieldActor_WaitForMove(leader);
    Actor_SetSpriteFlags(leader, 1);
    leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  } else {
    angle = ride->facing;
    leader->x.fixed =
        FIELD_HALF_CELL_ORIGIN((FIELD_HALF_CELL(ride->x.fixed) & ~1) +
                               (angle = FACING_EIGHTH_INDEX(angle)) - 1);
    leader->z.fixed = FIELD_HALF_CELL_ORIGIN(
        (FIELD_HALF_CELL(ride->z.fixed) & ~1) - (angle & 2) + 1);
    Actor_SetSpriteFlags(leader, 1);
    leader->velocityY = toFIXED(6);
    leader->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    camera = FieldCamera_GetActor();
    camera->speedLimit = toFIXED(8);
    camera->acceleration = toFIXED(1);
    camera->motionFlags = 0;
    FieldActor_SetMoveTargetFX(
        camera, leader->x.fixed,
        Map_GetTerrainHeight(0, leader->x.fixed, leader->z.fixed),
        leader->z.fixed);
    Actor_WaitUntilLanded(leader);
    FieldActor_WaitForMove(camera);
  }
  Actor_GetById(gMainRoutineState.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_AUTOMATIC;
  Actor_GetById(gMainRoutineState.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  Event_ClearFlag(FLAG_RIDING_WHIRLWIND);
  EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  wait(10);
  Event_End();
}

#include "shared/position_flat_distance.h"

#include "shared/position_distance_above.h"

extern u32 gAirsRock_WhirlwindRisesScript[];
extern u32 gAirsRock_WhirlwindFadesScript[];

/// How near a whirlwind passes to pick up the leader across the wall he
/// climbs, in pixels; across the ground it is WHIRLWIND_REACH.
#define WHIRLWIND_REACH_ON_WALL 16

/// A statue's whirlwind's update: it travels its way, picks up a leader it
/// touches and fades on a wall or another statue.
void AirsRock_MoveWhirlwind(struct Actor *shot) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct FieldState *state = Ram_HeapSlots->fieldState;
  FIXED speed = toFIXED(3) / 2;
  s32 cue = SE_SAND_EMERGE;
  struct Actor *other, *launcher;
  s32 i;
  s32 *leaderPosition;
  s32 *shotPosition;
  struct Actor **actors;

  if (state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
      state->item != 0 ||
      gMainRoutineState.movementMode == LEADER_MOVEMENT_SAND) {
    Actor_SetAnimationSpeed(shot, 0);
    return;
  }
  Actor_SetAnimationSpeed(shot, ANIM_STEP_NORMAL);
  if (CELL_CODE(
          MapCell_GetAttributes(2, FIELD_CELL(shot->x.fixed),
                                FIELD_CELL(shot->z.fixed - shot->y.fixed))) ==
      CELL_MARK_WHIRLWIND_END) {
  end:
    Actor_PlaySoundInView(shot, cue);
    shot->sceneByte = 0;
    shot->update = 0;
    if (shot->facing == FACING_NORTH)
      Actor_StartScript(shot, gAirsRock_WhirlwindRisesScript);
    else
      Actor_StartScript(shot, gAirsRock_WhirlwindFadesScript);
    return;
  }
  if (shot->sceneCounter == 0) {
    // i first says whether the whirlwind is beside the leader; the loop below
    // reuses it as the actor index.
    i = 0;
    leaderPosition = &leader->x.fixed;
    shotPosition = &shot->x.fixed;
    if (gMainRoutineState.movementMode == LEADER_MOVEMENT_KEYS) {
      if (Position_DistanceAbove(leaderPosition, shotPosition) <=
              WHIRLWIND_REACH_ON_WALL &&
          FIELD_CELL(leader->z.fixed) == FIELD_CELL(shot->z.fixed))
        i = 1;
    } else {
      if (Position_FlatDistance(leaderPosition, shotPosition) <=
              WHIRLWIND_REACH &&
          FIELD_CELL(leader->y.fixed) == FIELD_CELL(shot->y.fixed))
        i = 1;
    }
    if (i && !GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS)) {
      leader->facing = shot->facing;
      leader->watchedActor = (union FieldObject *)shot;
      leader->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      ((struct FieldSprite *)leader->sprite)->priority =
          ((struct FieldSprite *)shot->sprite)->priority;
      state->raisedTrigger = RAISED_RIDE_WHIRLWIND;
      gMainRoutineState.movementMode = LEADER_MOVEMENT_WALK;
      shot->sceneCounter = 1;
      shot->sceneByte = 1;
    }
  }
  actors = &state->actors[ACTOR_FIRST_PLACED];
  for (i = ACTOR_FIRST_PLACED; i != FIELD_TREASURE_ACTOR; i++) {
    other = *actors++;
    if (other->script != 0) {
      launcher = &shot->watchedActor->actor;
      if (FIELD_CELL(launcher->x.fixed) != FIELD_CELL(other->x.fixed) ||
          FIELD_CELL(launcher->y.fixed) != FIELD_CELL(other->y.fixed) ||
          FIELD_CELL(launcher->z.fixed) != FIELD_CELL(other->z.fixed)) {
        if (Position_FlatDistance(&other->x.fixed, &shot->x.fixed) <=
                WHIRLWIND_REACH &&
            FIELD_CELL(other->y.fixed) == FIELD_CELL(shot->y.fixed))
          goto end;
      }
    }
  }
  if (shot->facing == FACING_NORTH) {
    shot->x.fixed += Iwram_MulFX(Trig_Cos(shot->facing), speed);
    shot->y.fixed -= Iwram_MulFX(Math_Sin(shot->facing), speed);
  } else {
    shot->x.fixed += Iwram_MulFX(Trig_Cos(shot->facing), speed);
    shot->z.fixed += Iwram_MulFX(Math_Sin(shot->facing), speed);
  }
}

/// A statue's update: while no Psynergy, menu or item is in use it counts
/// frames, and every WHIRLWIND_BREATH_FRAMES breathes a whirlwind
/// (AirsRock_MoveWhirlwind) 8 pixels ahead of it with a puff of dust, with a
/// gust's sound unless the leader is riding one.
void AirsRock_StatueBreathesWhirlwind(struct Actor *actor) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *shot;
  struct Vector3 position;
  struct EffectOptions options;

  if (state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
      state->item != 0)
    return;
  if (actor->timer == WHIRLWIND_BREATH_FRAMES) {
    if (!Event_GetFlag(FLAG_RIDING_WHIRLWIND))
      Actor_PlaySoundInView(actor, SE_FLAME_FLARE);
    if (actor->facing == FACING_NORTH) {
      position.x = actor->x.fixed;
      position.z = actor->z.fixed;
    } else {
      position.x =
          actor->x.fixed + Math_MulFX(toFIXED(8), Trig_Cos(actor->facing));
      position.z =
          actor->z.fixed + Math_MulFX(toFIXED(8), Math_Sin(actor->facing));
    }
    shot = FieldActor_Create(SPRITE_WHIRLWIND, position.x, actor->y.fixed,
                             position.z);
    Actor_SelectAnimation(shot, 2);
    Actor_SetSpriteFlags(shot, 0);
    ((struct FieldSprite *)shot->sprite)->priority = 1;
    shot->motionFlags = 0;
    shot->sceneCounter = 0;
    shot->sceneByte = 0;
    shot->facing = actor->facing;
    shot->update = AirsRock_MoveWhirlwind;
    shot->watchedActor = (union FieldObject *)actor;
    actor->timer = 0;
    options.priority = 1;
    options.palette = 7;
    FieldEffect_Spawn(position.x, actor->y.fixed, position.z - toFIXED(6), 0, 0,
                      0, EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
  }
  actor->timer++;
}

/// Nothing visible happens as Felix starts to push one of the cliff's
/// pillars, or casts Move on one; the game takes it off the cells it was
/// closing so that it can slide.
void AirsRock_LiftPillar(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  Actor_LiftTargetBlock(gAirsRock_EntrancePillars);
  Blocks_UnmarkCells(state->movedBlock);
}

/// Once Felix has pushed or moved one of the cliff's pillars: if it now
/// stands over a gap it drops into it with a thump, closing the gap so the
/// party can walk over it, and it stays there for good.
void AirsRock_PlacePillar(void) {
  struct Actor *actor = gFieldState->movedBlock;
  s32 x, z;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_PlaceMovedBlock();
  Blocks_MarkCells(((u8 *)gAirsRock_EntrancePillars));
  if (Map_GetTerrainHeight(2, actor->x.fixed, actor->z.fixed) !=
      actor->y.fixed) {
    actor->layer = 2;
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    Actor_WaitUntilLanded(actor);
    Actor_WaitUntilLanded(actor);
    Event_PlaySound(SE_THUMP);
    actor->motionFlags = 0;
    actor->layer = 0;
  }
  x = FIELD_CELL(actor->x.fixed);
  z = FIELD_CELL(actor->z.fixed);
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

/// Felix casts Frost on the puddle on the cliff and it freezes into a pillar
/// of ice.
void AirsRock_FreezePuddle(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FlagBlock_ApplyFlags((s32)((u8 *)gAirsRock_FrozenPuddle));
  Event_End();
}

extern s32 gAirsRock_RubbleScript[];

/// A crumbling rock's clouds of dust, every fourth at its own size and the
/// rest twice as big, and the pieces of rubble it scatters.
#define CRUMBLE_DUST_CLOUDS 12
#define CRUMBLE_PIECES 6

/// A rock crumbles at x, z into rubble that scatters, shaking the screen.
void AirsRock_CrumbleRock(s32 x, s32 z) {
  struct Actor *first, *rock;
  struct EffectOptions options;
  struct Vector3 scatter;
  s32 i;
  FIXED radius = toFIXED(64);

  first = FieldActor_Create(SPRITE_ROCK, x, 0, z);
  options.priority = 1;
  options.startScaleX = FIXED_ONE * 2;
  options.startScaleY = FIXED_ONE * 2;
  for (i = 0; i != CRUMBLE_DUST_CLOUDS; i++) {
    scatter.x = x + INTtoFIXED(Random16() & 63) - toFIXED(32);
    scatter.y = (Random16() & 15) << 12;
    scatter.z = z + INTtoFIXED(Random16() & 63) - toFIXED(48);
    if (i & 3)
      FieldEffect_Spawn(scatter.x, 0, scatter.z, 0, scatter.y, 0,
                        EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
                            EFFECT_SCRIPT_MEDIUM,
                        &options);
    else
      FieldEffect_Spawn(scatter.x, 0, scatter.z, 0, scatter.y, 0,
                        EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, &options);
  }
  for (i = 0; i != CRUMBLE_PIECES; i++) {
    rock = first;
    if (i != 0) {
      rock = FieldActor_Create(SPRITE_ROCK, x, 0, z);
      Sprite_ShareVramCacheEntry(rock->sprite, first->sprite);
    }
    Actor_SetSpriteFlags(rock, 0);
    Actor_StartScript(rock, gAirsRock_RubbleScript);
    Actor_SelectAnimation(rock, 2);
    rock->velocityY = INTtoFIXED(Random16() & 3) + toFIXED(4);
    rock->gravity = toFIXED(1) / 5;
    rock->bounce = FIXED_ONE * 2 / 5;
    rock->scaleX = FIXED_ONE * 4 / 5;
    rock->scaleY = FIXED_ONE * 4 / 5;
    rock->speedLimit = toFIXED(4) / 5;
    scatter.y = Random16() & 7;
    scatter.x = Iwram_MulFX(Trig_Cos(scatter.y * FACING_STEP), radius);
    scatter.z = Iwram_MulFX(Math_Sin(scatter.y * FACING_STEP), radius);
    ((struct FieldSprite *)rock->sprite)->priority = 1;
    FieldActor_SetMoveTargetFX(rock, rock->x.fixed + scatter.x, rock->y.fixed,
                               rock->z.fixed + scatter.z);
  }
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
}

/// Felix casts Whirlwind on the twister rock at the top of Air's Rock: it
/// glows, the light turns yellow, lightning strikes the pile of rocks in the
/// hollow below it twice and they crumble with a crash and a flash
/// (AirsRock_CrumbleRock), clearing the way to the doorway into the rock. The
/// rock turns grey, spent.
void AirsRock_WhirlwindOpensTheTop(void) {
  u16 *buffer = Heap_Alloc(LIGHTNING_TILES * TILE_SIZE_4BPP);
  u16 *cell;
  struct Actor *spark;
  u32 i;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  Io_Write16(BGCNT_SCREEN_BASE(LIGHTNING_SCREEN_BLOCK), &REG_BG0CNT);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_LIGHTNING_TILES), buffer);
  Dma_Set(buffer, (void *)BG_CHAR_BLOCK(0),
          DMA_COPY_WORDS(LIGHTNING_TILES * TILE_SIZE_4BPP / 4), REG_DMA3);
  Heap_Release(buffer);
  Dma_Fill16(0, (void *)BG_SCREEN_BLOCK(LIGHTNING_SCREEN_BLOCK),
             BG_SCREEN_SIZE);
  buffer = Heap_Alloc(LIGHTNING_MAP_ROWS * LIGHTNING_MAP_COLUMNS * sizeof(u16));
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_LIGHTNING_MAP), buffer);
  cell = buffer;
  // The map's tile numbers less 512.
  for (i = 0; i != LIGHTNING_MAP_ROWS * LIGHTNING_MAP_COLUMNS; i++) {
    *cell &= 0xfcff;
    cell++;
  }
  cell = buffer;
  for (i = 0; i != LIGHTNING_MAP_ROWS; i++) {
    Dma_Set(
        cell,
        (void *)(BG_SCREEN_BLOCK(LIGHTNING_SCREEN_BLOCK) + i * MAP_ROW_BYTES),
        DMA_COPY_WORDS(LIGHTNING_MAP_COLUMNS * sizeof(u16) / 4), REG_DMA3);
    cell += LIGHTNING_MAP_COLUMNS;
  }
  Heap_Release(buffer);
  Event_PlaySound(SE_GLITTER);
  Actor_Get(ACTOR_TWISTER_ROCK_AT_THE_TOP)->sceneCounter =
      SPRITE_COLOR_BLUE_TINT;
  Actor_Get(ACTOR_TWISTER_ROCK_AT_THE_TOP)->sceneByte = 1;
  Actor_Get(ACTOR_TWISTER_ROCK_AT_THE_TOP)->update = AirsRock_FlickerTarget;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = (FieldActor_Create(
          SPRITE_PARTICLE, Actor_Get(ACTOR_TWISTER_ROCK_AT_THE_TOP)->x.fixed,
          Actor_Get(ACTOR_TWISTER_ROCK_AT_THE_TOP)->y.fixed,
          Actor_Get(ACTOR_TWISTER_ROCK_AT_THE_TOP)->z.fixed));
      ((struct FieldSprite *)spark->sprite)->priority =
          ((struct FieldSprite *)Actor_Get(ACTOR_TWISTER_ROCK_AT_THE_TOP)
               ->sprite)
              ->priority;
      spark->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      spark->motionFlags = 0;
      spark->bounce = spark->x.fixed;
      spark->gravity = toFIXED(3) / 2;
      spark->sceneWord = spark->z.fixed;
      spark->speedLimit = Random16() & 0xffff;
      Actor_SelectAnimation(spark, 7);
      Actor_StartScript(spark, gFieldEffect_MediumScript);
      Actor_SetSpriteFlags(spark, 0);
      FieldActor_SetSpriteColor(spark, SPRITE_COLOR_RBG);
      spark->update = AirsRock_SpiralSpark;
    }
    FieldEvent_Wait(1);
  }
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_Interpolate(60);
  EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(536, CAMERA_KEEP, 216, 1);
  FieldCamera_WaitForArrival();
  Actor_Get(ACTOR_TWISTER_ROCK_AT_THE_TOP)->update = 0;
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_TWISTER_ROCK_AT_THE_TOP),
                            SPRITE_COLOR_GREY);
  REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, BLEND_WEIGHT_MAX);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  gBgScroll[0].x = -120;
  gBgScroll[0].y = 16;
  System_Wait(30);
  Event_PlaySound(SE_ITEM_BREAK);
  PaletteFade_ApplySource(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_ALL);
  PaletteFade_Interpolate(1);
  Actor_Get(gMainRoutineState.leaderActorId)->facing = FACING_SOUTH;
  System_Wait(1);
  PaletteFade_ApplySource(RGB_WHITE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_Interpolate(8);
  REG_DISPCNT |= DISPCNT_BG0_ON;
  MapGrid_CopyCellsTo(52, 12, 29, 9, 9, 9);
  MapGrid_CopyCellsTo(52, 76, 29, 73, 9, 9);
  AirsRock_CrumbleRock(toFIXED(552), toFIXED(228));
  for (i = 0; i != BLEND_WEIGHT_MAX * 2; i++) {
    REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX - (i >> 1), BLEND_WEIGHT_MAX);
    System_Wait(1);
  }
  gBgScroll[0].x = -84;
  gBgScroll[0].y = 0;
  System_Wait(1);
  Event_PlaySound(SE_ITEM_BREAK);
  PaletteFade_ApplySource(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_ALL);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  PaletteFade_ApplySource(RGB_WHITE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_Interpolate(8);
  REG_DISPCNT |= DISPCNT_BG0_ON;
  MapGrid_CopyCellsTo(52, 22, 29, 9, 9, 9);
  MapGrid_CopyCellsTo(52, 86, 29, 73, 9, 9);
  AirsRock_CrumbleRock(toFIXED(520), toFIXED(242));
  for (i = 0; i != BLEND_WEIGHT_MAX * 2; i++) {
    REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX - (i >> 1), BLEND_WEIGHT_MAX);
    System_Wait(1);
  }
  Event_PlaySound(SE_CRASH);
  PaletteFade_ApplySource(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_ALL);
  PaletteFade_Interpolate(1);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  FieldEvent_Wait(20);
  AirsRock_CrumbleRock(toFIXED(512), toFIXED(198));
  AirsRock_CrumbleRock(toFIXED(562), toFIXED(242));
  AirsRock_CrumbleRock(toFIXED(521), toFIXED(223));
  MapGrid_CopyCellsTo(62, 2, 29, 9, 9, 9);
  MapGrid_CopyCellAttributes(62, 2, 9, 9, 29, 9);
  MapGrid_CopyCellsTo(62, 66, 29, 73, 9, 9);
  PaletteFade_ApplySource(RGB_WHITE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_Interpolate(60);
  System_Wait(60);
  PaletteFade_ApplySource(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_Interpolate(60);
  System_Wait(70);
  Window_LoadGraphics();
  DebugText_Clear();
  FieldWindowColor_Apply(gMainRoutineState.windowHue,
                         gMainRoutineState.windowBrightness);
  REG_BLDCNT = 0;
  REG_DISPCNT |= DISPCNT_BG0_ON;
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  Event_SetFlag(FLAG_AIRS_ROCK_TOP_OPENED);
  Event_End();
}

/// The fog's blend weight at its thickest; it breathes down to half that.
#define FOG_WEIGHT 8
/// A fog line's ripple: a sine shifted down to at most 8 pixels either way.
#define FOG_RIPPLE_SHIFT 13

/// The fog over the wall drifts, thickening and thinning between FOG_WEIGHT
/// and half that while its lines ripple.
void AirsRock_DriftFog(void) {
  struct MapLayer *layer = MapGrid_GetLayer(2);
  s16 *line = gAirsRock_WaveLines;
  s32 i;
  volatile u16 *channel;

  REG_BLDALPHA = BLEND_ALPHA(gAirsRock_FogAlpha, BLEND_WEIGHT_MAX);
  if ((*Ram_FrameCount & 15) == 0) {
    gAirsRock_FogAlpha += gAirsRock_FogAlphaStep;
    if (gAirsRock_FogAlpha == FOG_WEIGHT ||
        gAirsRock_FogAlpha == FOG_WEIGHT / 2)
      gAirsRock_FogAlphaStep *= -1;
  }
  for (i = 0; i != DISPLAY_HEIGHT; i++) {
    *line = FIXEDtoINT(layer->x) +
            (Math_Sin((i + (u8)*Ram_FrameCount + FIXEDtoINT(layer->y)) << 9) >>
             FOG_RIPPLE_SHIFT);
    line++;
  }
  line = gAirsRock_WaveLines;
  REG_BG1HOFS = line[0];
  channel = REG_DMA0;
  channel[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
  channel[5] &= ~DMA_ENABLE;
  channel[5];
  Dma_Set(line + 1, (void *)&REG_BG1HOFS, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)channel);
}

/// As the fog lifts, the great whirlwind sways and scatters the more the
/// higher it stands above its foot, while the fog ripples and thickens and
/// thins as in AirsRock_DriftFog.
void AirsRock_LiftFog(void) {
  struct MapLayer *layer = MapGrid_GetLayer(2);
  s16 *line = gAirsRock_WaveLines;
  s32 i;
  FIXED wave;
  s32 spread, value;
  s16 *table;
  volatile u16 *channel;

  for (i = 0; i != DISPLAY_HEIGHT; i++) {
    spread = (DISPLAY_HEIGHT - i) >> 2;
    wave = Math_Sin((i + (u8)gFrameCount) << 8);
    // Swayed by up to half the line's height above the foot, less 4, and
    // jittered by up to a quarter of the lines below; a line swung past
    // WAVE_SCROLL_MAX or WAVE_SCROLL_MIN is pulled back by as much.
    value =
        (Iwram_MulFX(wave, (WAVE_SCROLL_CALM_LINE - i) * (FIXED_ONE / 64)) >>
         11) -
        4;
    line[0] = value;
    line[0] += Random16() % (spread * 2) - spread;
    if (line[0] > WAVE_SCROLL_MAX)
      line[0] -= WAVE_SCROLL_MAX;
    else if (line[0] < WAVE_SCROLL_MIN)
      line[0] -= WAVE_SCROLL_MIN;
    line[1] = 0;
    value = wave + FIXEDtoINT(layer->y);
    line[2] = FIXEDtoINT(layer->x) + (value >> FOG_RIPPLE_SHIFT);
    line += 3;
  }
  REG_BLDALPHA = BLEND_ALPHA(gAirsRock_LiftingFogAlpha, BLEND_WEIGHT_MAX);
  if ((gFrameCount & 15) == 0) {
    gAirsRock_LiftingFogAlpha += gAirsRock_LiftingFogAlphaStep;
    if (gAirsRock_LiftingFogAlpha == FOG_WEIGHT ||
        gAirsRock_LiftingFogAlpha == FOG_WEIGHT / 2)
      gAirsRock_LiftingFogAlphaStep *= -1;
  }
  table = gAirsRock_WaveLines;
  *(volatile u16 *)REG_BG0HOFS = table[0];
  table += 2;
  REG_BG1HOFS = table[0];
  channel = REG_DMA0;
  channel[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
  channel[5] &= ~DMA_ENABLE;
  channel[5];
  Dma_Set(table + 1, REG_BG0HOFS, DMA_HBLANK_HALFWORD_PER_LINE + 2,
          (volatile u32 *)channel);
}

/// Felix casts Whirlwind on the twister rock at the foot of the foggy wall:
/// a great whirlwind rises from it and the fog lifts (AirsRock_LiftFog) for
/// good, so that the wall can be climbed out of it. The rock turns grey,
/// spent.
void AirsRock_WhirlwindClearsFog(void) {
  u16 *buffer = Heap_Alloc(2 * WHIRLWIND_TILE_ROW_SIZE);
  struct MapLayer *fog = MapGrid_GetLayer(2);
  u16 *cell;
  struct Actor *spark;
  s32 i;
  s32 z;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_WHIRLWIND_TILES), buffer);
  Dma_Set(buffer, BG_CHAR_BLOCK(3) + WHIRLWIND_TOP_TILE * TILE_SIZE_4BPP,
          DMA_COPY_WORDS(WHIRLWIND_TILE_ROW_SIZE / 4), REG_DMA3);
  Dma_Set((u8 *)buffer + WHIRLWIND_TILE_ROW_SIZE,
          BG_CHAR_BLOCK(3) + WHIRLWIND_BOTTOM_TILE * TILE_SIZE_4BPP,
          DMA_COPY_WORDS(WHIRLWIND_TILE_ROW_SIZE / 4), REG_DMA3);
  Heap_Release(buffer);
  Io_Write16(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(3) | BGCNT_SCREEN_BASE(4),
             &REG_BG0CNT);
  Dma_Fill16(WHIRLWIND_FILL_TILE, BG_SCREEN_BLOCK(4), 1280);
  buffer = Heap_Alloc(WHIRLWIND_MAP_HEIGHT * WHIRLWIND_MAP_WIDTH * sizeof(u16));
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_WHIRLWIND_MAP), buffer);
  cell = buffer;
  for (i = 0; i != WHIRLWIND_MAP_HEIGHT * WHIRLWIND_MAP_WIDTH; i++) {
    *cell -= WHIRLWIND_MAP_FIRST_TILE - WHIRLWIND_TOP_TILE;
    cell++;
  }
  cell = buffer;
  for (i = 0; i != WHIRLWIND_MAP_HEIGHT; i++) {
    Dma_Set(cell,
            BG_SCREEN_BLOCK(4) + WHIRLWIND_MAP_ROW_OFFSET +
                i * BG_SCREEN_WIDTH * sizeof(u16),
            DMA_COPY_WORDS(WHIRLWIND_MAP_WIDTH / 2), REG_DMA3);
    cell += WHIRLWIND_MAP_WIDTH;
  }
  Heap_Release(buffer);
  Event_PlaySound(SE_GLITTER);
  Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->sceneCounter =
      SPRITE_COLOR_BLUE_TINT;
  Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->sceneByte = 1;
  Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->update = AirsRock_FlickerTarget;
  for (i = 0; i != 45; i++) {
    s32 low = gFrameCount & 3;

    if (low == 0) {
      spark = (FieldActor_Create(
          SPRITE_PARTICLE, Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->x.fixed,
          Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->y.fixed,
          Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->z.fixed));
      ((struct FieldSprite *)spark->sprite)->priority =
          ((struct FieldSprite *)Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)
               ->sprite)
              ->priority;
      spark->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      spark->motionFlags = 0;
      spark->bounce = spark->x.fixed;
      spark->gravity = toFIXED(3) / 2;
      spark->sceneWord = spark->z.fixed;
      spark->speedLimit = Random16() & 0xffff;
      Actor_SelectAnimation(spark, 7);
      Actor_StartScript(spark, gFieldEffect_MediumScript);
      Actor_SetSpriteFlags(spark, 0);
      FieldActor_SetSpriteColor(spark, SPRITE_COLOR_RBG);
      spark->update = AirsRock_SpiralSpark;
    }
    FieldEvent_Wait(1);
  }
  EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(152, CAMERA_KEEP, 144, 1);
  FieldCamera_WaitForArrival();
  FieldTask_Remove(AirsRock_DriftFog);
  REG_BG1CNT &= ~BGCNT_PRIORITY(3);
  REG_BG1CNT |= BGCNT_PRIORITY(0);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_ALL);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  PaletteFade_ApplySource(RGB_WHITE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_Interpolate(8);
  Task_AddFrame(AirsRock_LiftFog, TASK_PASS_VBLANK);
  Event_PlaySound(SE_WAVE_HIGH);
  REG_DISPCNT |= DISPCNT_BG0_ON;
  MapGrid_SetShake(3, 3, 1);
  z = -toFIXED(1) / 2;
  for (i = toFIXED(1) / 4; i != toFIXED(9) / 2;
       i += toFIXED(1) / 64, z -= toFIXED(1) / 64) {
    fog->speedX = i;
    fog->speedY = z;
    Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->sceneCounter = Random16() & 7;
    System_Wait(1);
  }
  FieldTask_Remove(AirsRock_LiftFog);
  Task_AddFrame(AirsRock_ScrollWave, TASK_PASS_VBLANK);
  REG_BLDALPHA = BLEND_ALPHA(FOG_WEIGHT, 0);
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  // The fog thins a weight every 32 frames, to none.
  for (i = 0; i != (FOG_WEIGHT << 5) + 1; i++) {
    REG_BLDALPHA = BLEND_ALPHA(FOG_WEIGHT - (i >> 5), BLEND_WEIGHT_MAX);
    Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->sceneCounter = Random16() & 7;
    FieldEvent_Wait(1);
  }
  REG_DISPCNT &= ~DISPCNT_BG1_ON;
  System_Wait(30);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 0);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->sceneCounter = SPRITE_COLOR_GREY;
  for (i = 0; i != 129; i++) {
    REG_BLDALPHA = ((i >> 3) << 8) | (BLEND_WEIGHT_MAX - (i >> 3));
    FieldEvent_Wait(1);
  }
  FieldTask_Remove(AirsRock_ScrollWave);
  Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->sceneByte = 0;
  System_Wait(30);
  Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->update = NULL;
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG),
                            SPRITE_COLOR_GREY);
  FieldMapGrid_WaitForShakeToSettle();
  FieldEvent_Wait(60);
  REG_BLDCNT = 0;
  Window_LoadGraphics();
  DebugText_Clear();
  FieldWindowColor_Apply(gMainRoutineState.windowHue,
                         gMainRoutineState.windowBrightness);
  REG_BG1CNT &= ~BGCNT_PRIORITY(3);
  REG_BG1CNT |= BGCNT_PRIORITY(1);
  Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags &=
      ~ACTOR_PRIORITY_IGNORES_RISES;
  Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_TWISTER_ROCK_IN_THE_FOG)->priorityFlags |=
      ACTOR_PRIORITY_AUTOMATIC;
  EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  Event_SetFlag(FLAG_AIRS_ROCK_FOG_CLEARED);
  Event_End();
}

extern s32 gAirsRock_LeaderLevel;

/// The climb's levels are this many pixels apart.
#define CLIMB_LEVEL_HEIGHT 112

/// Nothing visible happens: as Felix climbs or drops between the four levels
/// of the ledges below the fog, the game swaps in the cells of the ledges in
/// front of the two pillars for the level he is on, since the ledges of all
/// four levels are drawn over the same rows of the map. On the second level
/// they hold the edge he drops off (AirsRock_DropOffLedge).
void AirsRock_SetLedgeCellsByLevel(void) {
  struct Actor *actor = Actor_Get(FieldParty_GetLeaderActor());
  s32 level;

  actor->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  level = FIXEDtoINT(actor->y.fixed + toFIXED(32)) / CLIMB_LEVEL_HEIGHT;
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

/// The leader's actor.
#define LEADER() Actor_GetById(gMainRoutineState.leaderActorId)

/// The fog's picture repeats every FOG_WRAP map cells (64 pixels) across and
/// down.
#define FOG_WRAP 4

/// Sets a statue breathing whirlwinds (AirsRock_StatueBreathesWhirlwind), its
/// count started at `frames` of WHIRLWIND_BREATH_FRAMES.
#define START_BREATHING(id, frames)                                            \
  (Actor_GetById(id)->timer = (frames),                                        \
   Actor_GetById(id)->update = AirsRock_StatueBreathesWhirlwind,               \
   Actor_SetSpriteFlags(Actor_GetById(id), 0))

/// The same for a statue hung on the cliff at the given height.
#define HANG_BREATHING(id, height, frames)                                     \
  (Actor_GetById(id)->motionFlags = 0, Actor_GetById(id)->y.fixed = (height),  \
   START_BREATHING(id, frames))

s32 Scene_Initialize(void) {
  struct Actor *leader;
  volatile struct DmaRegisters *dma;

  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  gMainRoutineState.retreatScene = SCENE_AIRS_ROCK_STORM_BRAND_CHEST;
  gMainRoutineState.retreatEntrance =
      AIRS_ROCK_STORM_BRAND_CHEST_FROM_WORLD_MAP;
  LEADER()->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  if (gMainRoutineState.next == SCENE_AIRS_ROCK_STORM_BRAND_CHEST) {
    switch (gMainRoutineState.entrance) {
    case AIRS_ROCK_STORM_BRAND_CHEST_FROM_AIRS_ROCK_ENTRANCE_WEST:
    case AIRS_ROCK_STORM_BRAND_CHEST_FROM_AIRS_ROCK_ENTRANCE_EAST_A:
    case AIRS_ROCK_STORM_BRAND_CHEST_FROM_AIRS_ROCK_ENTRANCE_EAST_B:
      Leader_Lift(toFIXED(88));
    }
    // The barriers already blown away stay gone.
    if (GameFlag_Get(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN))
      CLEAR_SAND_MOUND(49, 12);
    if (GameFlag_Get(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN + 1))
      CLEAR_SAND_BANK(6, 40);
    if (GameFlag_Get(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN + 2))
      CLEAR_SAND_BANK(39, 39);
    if (GameFlag_Get(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN + 3))
      CLEAR_SAND_BANK(23, 44);
    if (GameFlag_Get(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN + 4)) {
      // The bank at (6, 52), the one a gust blows south through: its edits
      // start a row lower than the other banks'.
      MapGrid_CopyCellsTo(64, 0, 70, 52, 1, 4);
      MapGrid_CopyCellAttributes(64, 0, 1, 3, 70, 52);
      MapGrid_CopyCellAttributes(64, 0, 1, 3, 6, 53);
    }
    if (GameFlag_Get(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN + 5))
      CLEAR_SAND_MOUND(15, 50);
    if (GameFlag_Get(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN + 6))
      CLEAR_SAND_BANK(39, 49);
    if (GameFlag_Get(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN + 7))
      CLEAR_SAND_MOUND(47, 53);
    if (GameFlag_Get(FLAG_AIRS_ROCK_WALL_CLEARED)) {
      MapGrid_CopyCellsTo(68, 0, 79, 0, 11, 10);
      MapGrid_CopyCellAttributes(38, 9, 1, 1, 16, 9);
      MapGrid_CopyCellAttributes(38, 9, 1, 1, 24, 9);
      Actor_SetSpriteColor(Actor_GetById(ACTOR_TWISTER_ROCK_BY_THE_WALL),
                           SPRITE_COLOR_GREY);
    }
  }
  if (gMainRoutineState.next == SCENE_AIRS_ROCK_ENTRANCE) {
    switch (gMainRoutineState.entrance) {
    case AIRS_ROCK_ENTRANCE_FROM_AIRS_ROCK_STORM_BRAND_CHEST_EAST_B:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->z.fixed += toFIXED(16);
      Leader_Lift(-toFIXED(72));
      goto breathing;
    case AIRS_ROCK_ENTRANCE_FROM_AIRS_ROCK_STORM_BRAND_CHEST_WEST:
    case AIRS_ROCK_ENTRANCE_FROM_AIRS_ROCK_STORM_BRAND_CHEST_EAST_A:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->z.fixed += toFIXED(16);
      Leader_Lift(-toFIXED(40));
      goto breathing;
    case ENTRANCE_DOWN_FROM_THE_CLIFF_WEST:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        LEADER()->x.fixed += toFIXED(48);
        LEADER()->y.fixed += toFIXED(16);
      }
    case ENTRANCE_DOWN_FROM_THE_CLIFF_EAST:
      Leader_Lift(toFIXED(56));
    case AIRS_ROCK_ENTRANCE_FROM_AIRS_ROCK_INSIDE_HALL:
    breathing:
      START_BREATHING(ACTOR_FIRST_SLOPE_STATUE, 60);
      START_BREATHING(ACTOR_SECOND_SLOPE_STATUE, 180);
      START_BREATHING(ACTOR_THIRD_SLOPE_STATUE, 0);
      START_BREATHING(ACTOR_FOURTH_SLOPE_STATUE, 60);
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
    case ENTRANCE_UP_THE_CLIFF_EAST:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        LEADER()->x.fixed += toFIXED(16);
        LEADER()->y.fixed -= toFIXED(16);
      }
    case ENTRANCE_UP_THE_CLIFF_WEST:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->z.fixed += toFIXED(16);
      Leader_Lift(-toFIXED(56));
    case AIRS_ROCK_ENTRANCE_FROM_AIRS_ROCK_FUJIN_SHIELD_CHEST_B:
      if (GameFlag_Get(FLAG_AIRS_ROCK_PILLAR_PLACED_FIFTH))
        Actor_SetPosition(ACTOR_FIFTH_LEDGE_PILLAR, 792, 296);
      Actor_PlaceBlocks(gAirsRock_EntrancePillars);
      Blocks_MarkCells(gAirsRock_EntrancePillars);
      HANG_BREATHING(ACTOR_FIRST_CLIFF_STATUE, toFIXED(16), 60);
      HANG_BREATHING(ACTOR_THIRD_CLIFF_STATUE, toFIXED(80), 90);
      HANG_BREATHING(ACTOR_SECOND_CLIFF_STATUE, -toFIXED(48), 100);
      HANG_BREATHING(ACTOR_FOURTH_CLIFF_STATUE, toFIXED(144), 180);
      Actor_GetById(ACTOR_BREATHING_STATUE)->motionFlags = 0;
      Actor_GetById(ACTOR_BREATHING_STATUE)->y.fixed = -toFIXED(144);
      Actor_SetSpritePriority(ACTOR_BREATHING_STATUE, 2);
      Actor_GetById(ACTOR_BREATHING_STATUE)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
      Actor_GetById(ACTOR_BREATHING_STATUE)->timer = 180;
      Actor_GetById(ACTOR_BREATHING_STATUE)->sceneCounter = 1;
      Actor_GetById(ACTOR_BREATHING_STATUE)->update =
          AirsRock_StatueBreathesWhirlwind;
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_BREATHING_STATUE), 0);
      break;
    }
  }
  if (gMainRoutineState.next == SCENE_AIRS_ROCK_FUJIN_SHIELD_CHEST) {
    switch (gMainRoutineState.entrance) {
    case AIRS_ROCK_FUJIN_SHIELD_CHEST_FROM_AIRS_ROCK_ENTRANCE:
    case ENTRANCE_LEDGES_FROM_LOWER_PATH:
    case ENTRANCE_LEDGES_FROM_UPPER_PATH:
      Task_AddFrame(AirsRock_SetLedgeCellsByLevel, TASK_PASS_FRAME);
      Actor_GetById(ACTOR_FIRST_BREATHING_STATUE)->motionFlags = 0;
      Actor_GetById(ACTOR_FIRST_BREATHING_STATUE)->y.fixed = -toFIXED(8);
      Actor_SetSpritePriority(ACTOR_FIRST_BREATHING_STATUE, 2);
      Actor_GetById(ACTOR_FIRST_BREATHING_STATUE)->timer = 60;
      Actor_GetById(ACTOR_FIRST_BREATHING_STATUE)->sceneCounter = 0;
      Actor_GetById(ACTOR_FIRST_BREATHING_STATUE)->update =
          AirsRock_StatueBreathesWhirlwind;
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_BREATHING_STATUE), 0);
      Actor_GetById(ACTOR_SECOND_BREATHING_STATUE)->motionFlags = 0;
      Actor_GetById(ACTOR_SECOND_BREATHING_STATUE)->y.fixed = -toFIXED(8);
      Actor_SetSpritePriority(ACTOR_SECOND_BREATHING_STATUE, 2);
      Actor_GetById(ACTOR_SECOND_BREATHING_STATUE)->timer = 180;
      Actor_GetById(ACTOR_SECOND_BREATHING_STATUE)->sceneCounter = 0;
      Actor_GetById(ACTOR_SECOND_BREATHING_STATUE)->update =
          AirsRock_StatueBreathesWhirlwind;
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_SECOND_BREATHING_STATUE), 0);
      Actor_GetById(ACTOR_FIRST_PILLAR)->layer = 2;
      Actor_SetSpritePriority(ACTOR_FIRST_PILLAR, 1);
      Actor_GetById(ACTOR_FIRST_PILLAR)->y.fixed = toFIXED(208);
      Actor_GetById(ACTOR_FIRST_PILLAR)->groundHeight =
          Actor_GetById(ACTOR_FIRST_PILLAR)->y.fixed;
      Actor_GetById(ACTOR_SECOND_PILLAR)->layer = 2;
      Actor_SetSpritePriority(ACTOR_SECOND_PILLAR, 1);
      Actor_GetById(ACTOR_SECOND_PILLAR)->y.fixed = toFIXED(208);
      Actor_GetById(ACTOR_SECOND_PILLAR)->groundHeight =
          Actor_GetById(ACTOR_SECOND_PILLAR)->y.fixed;
      if (GameFlag_Get(FLAG_FIRST_PILLAR_MOVED_WEST))
        Actor_SetPosition(ACTOR_FIRST_PILLAR, 504, 872);
      if (GameFlag_Get(FLAG_SECOND_PILLAR_MOVED_WEST))
        Actor_SetPosition(ACTOR_SECOND_PILLAR, 520, 872);
      AirsRock_MoveSecondPillar();
      MapGrid_GetLayer(2)->offsetX = toFIXED(1408);
      MapGrid_GetLayer(2)->offsetY = toFIXED(576);
      MapGrid_GetLayer(2)->scaleX = FIXED_ONE / 4;
      MapGrid_GetLayer(2)->scaleY = FIXED_ONE / 4;
      MapGrid_CenterOnFollowedPoint();
      System_Wait(1);
      break;
    case ENTRANCE_BELOW_THE_FOG_WEST:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->y.fixed += toFIXED(24);
    case ENTRANCE_BELOW_THE_FOG_EAST:
      Leader_Lift(toFIXED(72));
    case ENTRANCE_LOWER_PATH:
    case ENTRANCE_UPPER_PATH:
      Actor_SetSpritePriority(gMainRoutineState.leaderActorId, 1);
      break;
    case ENTRANCE_INTO_THE_FOG_WEST:
    case ENTRANCE_INTO_THE_FOG_EAST:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->z.fixed += toFIXED(16);
      Leader_Lift(-toFIXED(24));
      goto fog;
    case ENTRANCE_DOWN_INTO_THE_FOG_WEST:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->x.fixed += toFIXED(48);
    case ENTRANCE_DOWN_INTO_THE_FOG_EAST:
      Leader_Lift(toFIXED(144));
    fog:
      REG_BG1CNT &= ~BGCNT_PRIORITY(3);
      REG_BG1CNT |= BGCNT_PRIORITY(1);
      REG_BG2CNT &= ~BGCNT_PRIORITY(3);
      REG_BG2CNT |= BGCNT_PRIORITY(2);
      REG_BG3CNT &= ~BGCNT_PRIORITY(3);
      REG_BG3CNT |= BGCNT_PRIORITY(3);
      Actor_GetById(ACTOR_TWISTER_ROCK_IN_THE_FOG)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
      FieldEffect_StartBlankOverlay(0);
      if (GameFlag_Get(FLAG_AIRS_ROCK_THIRD_PILLAR_MOVED))
        Actor_SetPosition(ACTOR_THIRD_PILLAR, 184, 280);
      if (!GameFlag_Get(FLAG_AIRS_ROCK_FOG_CLEARED)) {
        gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        REG_BLDALPHA = BLEND_ALPHA(FOG_WEIGHT, BLEND_WEIGHT_MAX);
        REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
        dma = (volatile struct DmaRegisters *)REG_DMA0;
        while ((s32)dma->control < 0)
          ;
        Task_AddFrame(AirsRock_DriftFog, TASK_PASS_VBLANK);
        MapGrid_GetLayer(2)->speedX = toFIXED(0.25);
        MapGrid_GetLayer(2)->speedY = -toFIXED(0.5);
        MapGrid_GetLayer(2)->maskX = FOG_WRAP - 1;
        MapGrid_GetLayer(2)->maskY = (FOG_WRAP - 1) * MAPGRID_SIZE;
      } else {
        REG_BLDALPHA = BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
        REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
        REG_DISPCNT &= ~DISPCNT_BG1_ON;
        Actor_SetSpriteColor(Actor_GetById(ACTOR_TWISTER_ROCK_IN_THE_FOG),
                             SPRITE_COLOR_GREY);
      }
      break;
    case ENTRANCE_ABOVE_THE_FOG_WEST:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->y.fixed -= toFIXED(32);
    case ENTRANCE_ABOVE_THE_FOG_EAST:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->z.fixed += toFIXED(16);
      Leader_Lift(-toFIXED(48));
    case AIRS_ROCK_FUJIN_SHIELD_CHEST_FROM_AIRS_ROCK_INSIDE_VIAL_CHEST:
      MapGrid_GetLayer(2)->offsetX = toFIXED(1152);
      MapGrid_GetLayer(2)->offsetY = toFIXED(800);
      MapGrid_GetLayer(2)->scaleX = FIXED_ONE * 13 / 16;
      MapGrid_GetLayer(2)->scaleY = FIXED_ONE * 13 / 16;
      if (!GameFlag_Get(FLAG_AIRS_ROCK_TOP_OPENED)) {
        MapGrid_CopyCellsTo(29, 9, 62, 2, 9, 9);
        MapGrid_CopyCellAttributes(29, 9, 9, 9, 62, 2);
        MapGrid_CopyCellsTo(29, 73, 62, 66, 9, 9);
        MapGrid_CopyCellsTo(52, 2, 29, 9, 9, 9);
        MapGrid_CopyCellAttributes(52, 2, 9, 9, 29, 9);
        MapGrid_CopyCellsTo(52, 66, 29, 73, 9, 9);
      } else {
        Actor_SetSpriteColor(Actor_GetById(ACTOR_TWISTER_ROCK_AT_THE_TOP),
                             SPRITE_COLOR_GREY);
      }
      Actor_SetSpritePriority(ACTOR_TWISTER_ROCK_AT_THE_TOP, 1);
      MapGrid_CenterOnFollowedPoint();
      System_Wait(1);
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        leader = LEADER();
        leader->layer = 0;
        leader->y.fixed = leader->groundHeight = Map_GetTerrainHeight(
            0, leader->x.fixed, leader->z.fixed - leader->y.fixed);
        EventCamera_FollowActor(gMainRoutineState.leaderActorId, 0);
      }
      break;
    case ENTRANCE_OPEN_THE_TOP:
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
  if (gMainRoutineState.next == SCENE_AIRS_ROCK_FUJIN_SHIELD_CHEST) {
    AirsRock_SetLedgeCellsByLevel();
  }
  return 0;
}

#undef WAVE_SCROLL
#undef WAVE_SCROLL_LINES
#undef LEADER
#undef FOG_WRAP
#undef START_BREATHING
#undef HANG_BREATHING
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY

// clang-format off
#include "shared/field_effect_duration_scripts.c"

/// The five pillars on the entrance's ledges that Move pushes, ended by -1.
s16 gAirsRock_EntrancePillars[] = {
    ACTOR_FIRST_LEDGE_PILLAR,  ACTOR_SECOND_LEDGE_PILLAR, ACTOR_THIRD_LEDGE_PILLAR,
    ACTOR_FOURTH_LEDGE_PILLAR, ACTOR_FIFTH_LEDGE_PILLAR,  -1,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s32 gAirsRock_WindStoneScript[] = {
    SC_SET(SW_SCALE_Y, FIXED_ONE * 3 / 2),
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
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 50),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 50),
    SC_LOOP(32, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

/// The puddle Frost freezes into a pillar, and the flag set while it stands
/// (FlagBlock_PlaceAll).
s16 gAirsRock_FrozenPuddle[] = {
    ACTOR_PUDDLE, FLAG_PUDDLE_FROZEN, -1, 0,
};

#include "shared/scene_entrances_default.c"

struct SceneRegion gAirsRock_StormBrandChestRegions[] = {
    {256, 96, 144, 272, 112, 160, CONDITION_ALWAYS, 2},
    {608, 96, 144, 624, 112, 160, CONDITION_ALWAYS, 3},
    {864, 96, 144, 880, 112, 160, CONDITION_ALWAYS, 4},
    {SCENE_TABLE_END},
};

struct SceneRegion gAirsRock_EntranceRegions[] = {
    {256, -192, 816, 272, -176, 832, CONDITION_ALWAYS, 1},
    {608, -192, 816, 624, -176, 832, CONDITION_ALWAYS, 2},
    {864, -192, 816, 880, -176, 832, CONDITION_ALWAYS, 3},
    {624, 176, 704, 640, 192, 720, CONDITION_ALWAYS, 5},
    {784, 160, 688, 800, 176, 704, CONDITION_ALWAYS, 6},
    {624, -200, 320, 640, -184, 336, CONDITION_ALWAYS, 7},
    {784, -216, 304, 800, -200, 320, CONDITION_ALWAYS, 8},
    {SCENE_TABLE_END},
};

struct SceneRegion gAirsRock_FujinShieldChestRegions[] = {
    {144, -96, 304, 160, -80, 320, CONDITION_ALWAYS, 6},
    {208, -96, 304, 224, -80, 320, CONDITION_ALWAYS, 7},
    {144, -80, 304, 160, -64, 320, CONDITION_ALWAYS, 8},
    {208, -80, 304, 224, -64, 320, CONDITION_ALWAYS, 9},
    {144, 160, 192, 160, 176, 208, CONDITION_ALWAYS, 10},
    {208, 160, 192, 224, 176, 208, CONDITION_ALWAYS, 11},
    {144, 160, 192, 160, 176, 208, CONDITION_ALWAYS, 10},
    {208, 160, 192, 224, 176, 208, CONDITION_ALWAYS, 11},
    {528, -96, 304, 544, -80, 320, CONDITION_ALWAYS, 12},
    {592, -96, 304, 608, -80, 320, CONDITION_ALWAYS, 13},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(AirsRockStormBrandChest),
    SCENE_EXIT_UNLESS(AIRS_ROCK_STORM_BRAND_CHEST_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_AIRS_ROCK_STORM_BRAND_CHEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(AIRS_ROCK_STORM_BRAND_CHEST_TO_AIRS_ROCK_ENTRANCE_WEST, AirsRockEntrance, AIRS_ROCK_ENTRANCE_FROM_AIRS_ROCK_STORM_BRAND_CHEST_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(AIRS_ROCK_STORM_BRAND_CHEST_TO_AIRS_ROCK_ENTRANCE_EAST_A, AirsRockEntrance, AIRS_ROCK_ENTRANCE_FROM_AIRS_ROCK_STORM_BRAND_CHEST_EAST_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(AIRS_ROCK_STORM_BRAND_CHEST_TO_AIRS_ROCK_ENTRANCE_EAST_B, AirsRockEntrance, AIRS_ROCK_ENTRANCE_FROM_AIRS_ROCK_STORM_BRAND_CHEST_EAST_B, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockEntrance),
    SCENE_EXIT_UNLESS(AIRS_ROCK_ENTRANCE_TO_AIRS_ROCK_STORM_BRAND_CHEST_WEST, AirsRockStormBrandChest, AIRS_ROCK_STORM_BRAND_CHEST_FROM_AIRS_ROCK_ENTRANCE_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(AIRS_ROCK_ENTRANCE_TO_AIRS_ROCK_STORM_BRAND_CHEST_EAST_A, AirsRockStormBrandChest, AIRS_ROCK_STORM_BRAND_CHEST_FROM_AIRS_ROCK_ENTRANCE_EAST_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(AIRS_ROCK_ENTRANCE_TO_AIRS_ROCK_STORM_BRAND_CHEST_EAST_B, AirsRockStormBrandChest, AIRS_ROCK_STORM_BRAND_CHEST_FROM_AIRS_ROCK_ENTRANCE_EAST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(AIRS_ROCK_ENTRANCE_TO_AIRS_ROCK_INSIDE_HALL, AirsRockInsideHall, AIRS_ROCK_INSIDE_HALL_FROM_AIRS_ROCK_ENTRANCE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, AirsRockEntrance, ENTRANCE_UP_THE_CLIFF_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, AirsRockEntrance, ENTRANCE_UP_THE_CLIFF_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, AirsRockEntrance, ENTRANCE_DOWN_FROM_THE_CLIFF_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, AirsRockEntrance, ENTRANCE_DOWN_FROM_THE_CLIFF_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(AIRS_ROCK_ENTRANCE_TO_AIRS_ROCK_FUJIN_SHIELD_CHEST_A, AirsRockFujinShieldChest, AIRS_ROCK_FUJIN_SHIELD_CHEST_FROM_AIRS_ROCK_ENTRANCE, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockFujinShieldChest),
    SCENE_EXIT_UNLESS(AIRS_ROCK_FUJIN_SHIELD_CHEST_TO_AIRS_ROCK_ENTRANCE, AirsRockEntrance, AIRS_ROCK_ENTRANCE_FROM_AIRS_ROCK_FUJIN_SHIELD_CHEST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockFujinShieldChest, ENTRANCE_LOWER_PATH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AirsRockFujinShieldChest, ENTRANCE_UPPER_PATH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AirsRockFujinShieldChest, ENTRANCE_LEDGES_FROM_LOWER_PATH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, AirsRockFujinShieldChest, ENTRANCE_LEDGES_FROM_UPPER_PATH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, AirsRockFujinShieldChest, ENTRANCE_INTO_THE_FOG_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, AirsRockFujinShieldChest, ENTRANCE_INTO_THE_FOG_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, AirsRockFujinShieldChest, ENTRANCE_BELOW_THE_FOG_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, AirsRockFujinShieldChest, ENTRANCE_BELOW_THE_FOG_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, AirsRockFujinShieldChest, ENTRANCE_INTO_THE_FOG_WEST, FLAG_AIRS_ROCK_FOG_CLEARED),
    SCENE_EXIT_UNLESS(11, AirsRockFujinShieldChest, ENTRANCE_INTO_THE_FOG_EAST, FLAG_AIRS_ROCK_FOG_CLEARED),
    SCENE_EXIT_UNLESS(10, AirsRockFujinShieldChest, ENTRANCE_ABOVE_THE_FOG_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, AirsRockFujinShieldChest, ENTRANCE_ABOVE_THE_FOG_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, AirsRockFujinShieldChest, ENTRANCE_DOWN_INTO_THE_FOG_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, AirsRockFujinShieldChest, ENTRANCE_DOWN_INTO_THE_FOG_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(AIRS_ROCK_FUJIN_SHIELD_CHEST_TO_AIRS_ROCK_INSIDE_VIAL_CHEST, AirsRockInsideVialChest, AIRS_ROCK_INSIDE_VIAL_CHEST_FROM_AIRS_ROCK_FUJIN_SHIELD_CHEST, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gAirsRock_StormBrandChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TWISTER_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 328, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STONE, CONDITION_ALWAYS, (s32)gAirsRock_WindStoneScript, 920, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STONE, CONDITION_ALWAYS, (s32)gAirsRock_WindStoneScript, 104, 0, 760, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STONE, CONDITION_ALWAYS, (s32)gAirsRock_WindStoneScript, 376, 0, 808, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STONE, CONDITION_ALWAYS, (s32)gAirsRock_WindStoneScript, 632, 0, 888, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STONE, CONDITION_ALWAYS, (s32)gAirsRock_WindStoneScript, 872, 0, 856, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRock_EntrancePlacements[] = {
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 9, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 536, 0, 296, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 680, 0, 296, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 536, 0, 296, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 0, 296, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 712, 0, 312, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRock_FujinShieldChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 536, 0, 904, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 712, 0, 904, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 872, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 872, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TWISTER_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 152, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 280, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TWISTER_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 536, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gAirsRock_StormBrandChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXAMINE, 20, CONDITION_ALWAYS, EVENT(AirsRock_ExamineWindStone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_TWISTER_ROCK_BY_THE_WALL, FLAG_AIRS_ROCK_WALL_CLEARED, EVENT(AirsRock_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TWISTER_ROCK_BY_THE_WALL, FLAG_AIRS_ROCK_WALL_CLEARED, EVENT(AirsRock_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_TWISTER_ROCK_BY_THE_WALL, FLAG_AIRS_ROCK_WALL_CLEARED, EVENT(AirsRock_WhirlwindClearsWall)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_WIND_STONE_A, CONDITION_ALWAYS, EVENT(AirsRock_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_WIND_STONE_A, CONDITION_ALWAYS, EVENT(AirsRock_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WIND_STONE_A, CONDITION_ALWAYS, EVENT(AirsRock_BlowGust)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_WIND_STONE_B, CONDITION_ALWAYS, EVENT(AirsRock_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_WIND_STONE_B, CONDITION_ALWAYS, EVENT(AirsRock_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WIND_STONE_B, CONDITION_ALWAYS, EVENT(AirsRock_BlowGust)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_WIND_STONE_C, CONDITION_ALWAYS, EVENT(AirsRock_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_WIND_STONE_C, CONDITION_ALWAYS, EVENT(AirsRock_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WIND_STONE_C, CONDITION_ALWAYS, EVENT(AirsRock_BlowGust)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_WIND_STONE_D, CONDITION_ALWAYS, EVENT(AirsRock_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_WIND_STONE_D, CONDITION_ALWAYS, EVENT(AirsRock_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WIND_STONE_D, CONDITION_ALWAYS, EVENT(AirsRock_BlowGust)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_WIND_STONE_E, CONDITION_ALWAYS, EVENT(AirsRock_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_WIND_STONE_E, CONDITION_ALWAYS, EVENT(AirsRock_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WIND_STONE_E, CONDITION_ALWAYS, EVENT(AirsRock_BlowGust)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRock_EntranceTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(AirsRock_EnterTheRock)},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 9},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AirsRock_LiftPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AirsRock_PlacePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_LEDGE_PILLAR, FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST, EVENT(AirsRock_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_LEDGE_PILLAR, FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST, EVENT(AirsRock_PlacePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SECOND_LEDGE_PILLAR, FLAG_AIRS_ROCK_PILLAR_PLACED_SECOND, EVENT(AirsRock_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_LEDGE_PILLAR, FLAG_AIRS_ROCK_PILLAR_PLACED_SECOND, EVENT(AirsRock_PlacePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_THIRD_LEDGE_PILLAR, FLAG_AIRS_ROCK_PILLAR_PLACED_THIRD, EVENT(AirsRock_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_LEDGE_PILLAR, FLAG_AIRS_ROCK_PILLAR_PLACED_THIRD, EVENT(AirsRock_PlacePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FOURTH_LEDGE_PILLAR, FLAG_AIRS_ROCK_PILLAR_PLACED_FOURTH, EVENT(AirsRock_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FOURTH_LEDGE_PILLAR, FLAG_AIRS_ROCK_PILLAR_PLACED_FOURTH, EVENT(AirsRock_PlacePillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_PUDDLE_FROZEN, EVENT(AirsRock_FreezePuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIFTH_LEDGE_PILLAR, FLAG_AIRS_ROCK_PILLAR_PLACED_FIFTH, EVENT(AirsRock_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIFTH_LEDGE_PILLAR, FLAG_AIRS_ROCK_PILLAR_PLACED_FIFTH, EVENT(AirsRock_PlacePillar)},
    {TRIGGER_RAISED, RAISED_RIDE_WHIRLWIND, CONDITION_ALWAYS, EVENT(AirsRock_RideWhirlwind)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRock_FujinShieldChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 14},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 20, CONDITION_ALWAYS, EVENT(AirsRock_DropOffLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR, CONDITION_ALWAYS, EVENT(AirsRock_MoveFirstPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_PILLAR, CONDITION_ALWAYS, EVENT(AirsRock_MoveSecondPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_PILLAR, FLAG_AIRS_ROCK_THIRD_PILLAR_MOVED, EVENT(AirsRock_MoveThirdPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_TWISTER_ROCK_IN_THE_FOG, FLAG_AIRS_ROCK_FOG_CLEARED, EVENT(AirsRock_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TWISTER_ROCK_IN_THE_FOG, FLAG_AIRS_ROCK_FOG_CLEARED, EVENT(AirsRock_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_TWISTER_ROCK_IN_THE_FOG, FLAG_AIRS_ROCK_FOG_CLEARED, EVENT(AirsRock_WhirlwindClearsFog)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_TWISTER_ROCK_AT_THE_TOP, FLAG_AIRS_ROCK_TOP_OPENED, EVENT(AirsRock_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TWISTER_ROCK_AT_THE_TOP, FLAG_AIRS_ROCK_TOP_OPENED, EVENT(AirsRock_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_TWISTER_ROCK_AT_THE_TOP, FLAG_AIRS_ROCK_TOP_OPENED, EVENT(AirsRock_WhirlwindOpensTheTop)},
    {TRIGGER_RAISED, RAISED_RIDE_WHIRLWIND, CONDITION_ALWAYS, EVENT(AirsRock_RideWhirlwind)},
    {TRIGGER_TABLE_END},
};

/// The kinds of barrier a gust sweeps away (enum AirsRockGustBarrier), a row
/// each: the map cell the cleared ground's picture is copied from, and how far
/// the barrier's pictures lie from the cells the gust crosses, 64 cells east.
/// An odd kind clears a column three cells tall, an even one a cell; no
/// barrier is of kind 1.
s16 gAirsRock_GustTargets[][4] = {
    {64, 0, 64, 0}, {64, 0, 64, 0}, {64, 0, 64, 0}, {64, 0, 64, 0},
};
// clang-format on

/// The fog's blend weight, in sixteenths, and its step every sixteenth frame:
/// it thickens and thins between FOG_WEIGHT / 2 and FOG_WEIGHT
/// (AirsRock_DriftFog).
s8 gAirsRock_FogAlpha = 6;
s8 gAirsRock_FogAlphaStep = 1;

/// As gAirsRock_FogAlpha, while the fog lifts (AirsRock_LiftFog).
s8 gAirsRock_LiftingFogAlpha = 6;
s8 gAirsRock_LiftingFogAlphaStep = 1;

/// The ledges' level the leader stood on when their cells were last swapped; -1
/// before.
s32 gAirsRock_LeaderLevel = -1;
