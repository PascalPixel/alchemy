/// @file
/// Colosseum Final 3 (SceneId_ColosseumNavampaMatch) is the site of the last
/// finals match, where Isaac races Navampa of Gondowan through six stages to
/// the arena and fights him there.
///
/// Isaac has won the first two matches, and Navampa is the sixth-ranked
/// warrior.
///
/// It works as Colosseum Final 1 does (colosseum_azart_match.c). The guide,
/// who did not think Isaac would make it this far, leaves the stages to their
/// attendants. The stages run through the Scales of Justice, the
/// Stepping-Stone Stage, the Wall Wedge, the Free-Climb Area, the Moving
/// Sidewalk Stage and another log-rolling area.
///
/// ColosseumNavampaMatch_Initialize restores the stages from their temporary
/// flags (FLAG_PLANT_GROWN to FLAG_LONG_LOG_IN_PLACE, and the stone's column
/// from FLAG_STONE_COLUMN). Then it sets up the visit its entrance stands for.
///
/// - COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_NORTH, the tour
///   (ColosseumNavampaMatch_IntroduceThirdFinals), from the Colosseum's east
///   passage after the second win. The attendants describe their stages and
///   take the friends to cheer, and the guide sends Isaac back to the passage
///   (COLOSSEUM_NAVAMPA_MATCH_TO_COLOSSEUM).
/// - COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH, `Lend a Hand`. Garet, Ivan
///   and Mia in turn may use Psynergy on the stages. Frost freezes the target
///   on the Scales of Justice's eastern ledge so the scales stay tipped. Halt
///   stops the Wall Wedge's operator and with him its pistons. Force knocks the
///   log off the Moving Sidewalk so the belt no longer carries anyone back.
///   Growth grows a plant into a shortcut. Move shifts the weight onto the
///   scales and the stones and the rock on the way. After Mia,
///   COLOSSEUM_NAVAMPA_MATCH_LEND_A_HAND_TO_RACE starts the race.
/// - COLOSSEUM_NAVAMPA_MATCH_RACE_FROM_LEND_A_HAND, the race against Navampa,
///   who follows a recorded path, after the lettering `The 3rd Match` and a
///   count of three. At the arena the first to arrive takes the Claymore and
///   the second the Hunter's Sword. The battle is ENCOUNTER_ZONE_COLOSSO's
///   fifth or sixth group, the Navampa with or without the Claymore.
///
/// Stepping onto the Scales of Justice's western ledge tips the scales (step
/// trigger 15, ColosseumNavampaMatch_StepOnScales). The ledge goes down, with
/// the rider, towards 4 pixels high, and the eastern ledge goes up towards 32.
/// Stepping off (step trigger 16) swings them back. The weight Move sets on
/// the western ledge does the same.
///
/// Win or lose, the third match ends Colosso.
/// COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON and _LOST set FLAG_COLOSSO_OVER, and
/// a win also FLAG_WON_COLOSSO.
///
/// In the event that follows (ColosseumNavampaMatch_PlayPostBattleReunion),
/// Garet, Ivan and Mia run to Isaac in the arena, but he falls asleep,
/// exhausted, and they leave him to sleep. Both exits lead to Babi's Palace,
/// where he wakes long after the finals.

#include "script_operands.h"
#include "version.h"
#include "main_routine_work.h"
#include "io_write_queue.h"
#include "resource_ids.h"
#include "scene_ids.h"
#include "vram_cache.h"
#include "module/import.h"
#include "module/scene/colosseum_navampa_match.h"
#include "module/scene/shared/colosso_solo.h"
#include "module/scene/shared/colosso_marker.h"
#include "module/scene/shared/colosso_setup.h"
#include "module/scene/shared/colosso_occupant.h"
#include "module/field/field_sequence.h"
#include "pushed_actor.h"
#include "module/system/main_routine.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/game/game.h"
#include "module/window/window.h"
#include "module/window/screens.h"
#include "module/menu/menu.h"
#include "module/sound/sound.h"
#include "module/system/system.h"
#include "module/scene/shared/colosso_item_icon.h"
#include "types.h"
#include "event.h"
#include "scene_tables.h"
#include "io_reg.h"
#include "input.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "edition.h"
#include "far_call.h"
#include "sound.h"
#include "system.h"
#include "heap_release.h"
#include "event_inline.h"
#include "map_grid.h"
#include "flags.h"
#include "text/msg_ids.h"
#include "battle_unit.h"
#include "battle_sprite_id.h"
#include "ability_ids.h"
#include "edition.h"
#include "item_ids.h"
#include "actor_script.h"
#include "actor.h"
#include "heap.h"
#include "scene.h"
#include "fixed_math.h"
#include "palette_buffers.h"
#include "shared/colosso_gauge.h"
#include "macros.h"
#include "sounds.h"
#include "sound_driver_symbols.h"

// UB: this calls a void function through an s32 function type, the call the
// bytes show; UBFIX calls it as declared.
#ifdef UBFIX
#define COLOSSEUM_NAVAMPA_MATCH_FACE_DIRECTION Actor_FaceDirectionById
#else
#define COLOSSEUM_NAVAMPA_MATCH_FACE_DIRECTION                                 \
  ((s32 (*)())Actor_FaceDirectionById)
#endif

static void ColosseumNavampaMatch_PlayPostBattleReunion(s32 battleResult);
static void ColosseumNavampaMatch_IntroduceThirdFinals(s32 guideId);
static void Colosso_ShowOpeningLettering(s32 match);
// Unused: the third match ends with the party's reunion in the arena
// (ColosseumNavampaMatch_PlayPostBattleReunion) instead.
UNUSED static void Colosso_ShowMatchResult(s32 result);
static void Colosso_NoOp(void);
static void Colosso_WaitForOpponentToFinish(void);
static s32 Colosso_TakePrize(s32 firstItem, s32 secondItem);
static void Colosso_PushActorAhead(void);

typedef void (*MapTaskEntry)(void);

/// The third finals match's actors, numbered in the order of
/// gColosseumNavampaMatchActorPlacements from ACTOR_FIRST_PLACED.
///
/// The rows of Isaac, Garet, Ivan and Mia take their own actors.
///
/// By the guide's and attendants' explanations, the Scales of Justice, the
/// Stepping-Stone Stage, the Wall Wedge, the Free-Climb Area, the Moving
/// Sidewalk Stage and another log-rolling area lead to the arena.
///
/// ACTOR_SCALES_WEIGHT to ACTOR_LOG_AREA_STONE_C are all drawn as the rocks
/// of Colosseum Final 1's Rock Challenge. The attendant of this match's
/// log-rolling area calls the ones there stones.
enum {
  /// Navampa, racing Isaac to the arena.
  /// ★ Named from the game: "I am Navampa of Gondowan"
  /// (MsgColosseumAmNavampaGondowanSixthRanked)
  ACTOR_OPPONENT = 8,
  /// The puddle Frost freezes on the Scales of Justice's eastern ledge.
  /// ★ Named from what the player sees: a small puddle of water
  /// (SPRITE_PUDDLE) on the eastern ledge of the Scales of Justice (its
  /// render)
  ACTOR_PUDDLE = 9,
  /// The Scales of Justice's two ledges.
  /// ★ Named from the game: "There are two ledges that actually form a
  /// scale." (MsgColossoStageCalledScales)
  ACTOR_SCALES_WEST_LEDGE = 10,
  /// ★ Named from the game: "There are two ledges that actually form a
  /// scale." (MsgColossoStageCalledScales)
  ACTOR_SCALES_EAST_LEDGE = 11,
  /// ★ Named from what the player sees: a stone pillar west of the Scales of
  /// Justice, which Move sets on the western ledge to hold it down (its
  /// render)
  ACTOR_SCALES_WEIGHT = 12,
  /// ★ Named from what the player sees: a stone pillar on the
  /// Stepping-Stone Stage that Move shifts along its row (its render)
  ACTOR_STONE = 13,
  /// East of the Wall Wedge's pistons, by the operator. Move on it needs
  /// nothing more (ColosseumNavampaMatch_IgnoreMove).
  /// ★ Named from what the player sees: a stone pillar east of the Wall
  /// Wedge's pistons (its render)
  ACTOR_ROCK = 14,
  /// The log-rolling area's three stones. Each settles where Move drops it
  /// (ColosseumNavampaMatch_SettleStones, FLAG_FIRST_LOG_AREA_STONE_SETTLED and
  /// the two after it).
  /// ★ Named from the game: "You can get through quickly by moving the logs
  /// and stones." (MsgColossoAnotherLogRollingArea)
  ACTOR_LOG_AREA_STONE_A = 15,
  /// ★ Named from the game: "You can get through quickly by moving the logs
  /// and stones." (MsgColossoAnotherLogRollingArea)
  ACTOR_LOG_AREA_STONE_B = 16,
  /// ★ Named from the game: "You can get through quickly by moving the logs
  /// and stones." (MsgColossoAnotherLogRollingArea)
  ACTOR_LOG_AREA_STONE_C = 17,
  /// The Wall Wedge's five pistons, west to east. Each is drawn by two
  /// actors, ACTOR_FIRST_PISTON + n and ACTOR_FIRST_PISTON + PISTON_COUNT
  /// + n.
  /// ★ Named from the game: "The pistons move back and forth."
  /// (MsgColossoPlaceCalledWall)
  ACTOR_FIRST_PISTON = 18,
  /// ★ Named from the game: "The pistons move back and forth."
  /// (MsgColossoPlaceCalledWall)
  ACTOR_LAST_PISTON = 22,
  /// A young man with fair hair who works the Wall Wedge.
  /// ★ Named from the game: "The operator of the walls will cheer for you on
  /// this stage." (MsgColossoOperatorWallsCheer)
  ACTOR_WALL_OPERATOR = 28,
  /// The plant Growth grows into a shortcut.
  /// ★ Named from what the player sees: a small sprout (SPRITE_SPROUT) on the
  /// ground past the Wall Wedge (its render)
  ACTOR_GROWTH_PLANT = 29,
  /// The log Force knocks off the Moving Sidewalk Stage's conveyor belt.
  /// ★ Named from what the player sees: a log (spr_294.png) at the conveyor
  /// belt of the Moving Sidewalk Stage (its render)
  ACTOR_SIDEWALK_LOG = 30,
  /// The log-rolling area's long log.
  /// ★ Named from the game: "To get across the tank, you'll need to roll the
  /// logs." (MsgColossoAnotherLogRollingArea)
  ACTOR_LONG_LOG = 31,
  /// A soldier who explains the third finals match.
  /// ★ Named from the game: "This is the site of the third finals match."
  /// (MsgColossoSiteThirdFinals)
  ACTOR_GUIDE = 32,
  /// The stages' attendants, young men with fair hair in white tunics.
  /// ★ Named from the game: "This stage is called the Scales of Justice."
  /// (MsgColossoStageCalledScales)
  ACTOR_SCALES_ATTENDANT = 33,
  /// ★ Named from the game: "You can reach the next stage by jumping from
  /// stone to stone." (MsgColossoSteppingStoneStage)
  ACTOR_STEPPING_STONE_ATTENDANT = 34,
  /// ★ Named from the game: "This place is called the Wall Wedge."
  /// (MsgColossoPlaceCalledWall)
  ACTOR_WALL_WEDGE_ATTENDANT = 35,
  /// ★ Named from the game: "This sheer wall has a bumpy surface."
  /// (MsgColossoPlaceNormallyCalledFreeClimb)
  ACTOR_FREE_CLIMB_ATTENDANT = 36,
  /// ★ Named from the game: "This is called the Moving Sidewalk Stage."
  /// (MsgColossoCalledMovingSidewalkStage)
  ACTOR_MOVING_SIDEWALK_ATTENDANT = 37,
  /// ★ Named from the game: "This is another log-rolling area."
  /// (MsgColossoAnotherLogRollingArea)
  ACTOR_LOG_ROLLING_ATTENDANT = 38,
  /// The arena's prizes, drawn as item icons at two blank actors
  /// (Colosso_ShowItemIcon).
  /// ★ Named from what the player sees: the Claymore's icon at the arena's
  /// finish, taken by whoever reaches it first (its render)
  ACTOR_PRIZE_A = 39,
  /// ★ Named from what the player sees: the Hunter's Sword's icon at the
  /// arena's finish, taken by whoever reaches it second (its render)
  ACTOR_PRIZE_B = 40,
  /// The first of the three particles that rise from the sleeping Isaac after
  /// the last match (ColosseumNavampaMatch_SpawnPeriodicParticle).
  /// ★ Named from what the player sees: a "z" (spr_280.png) rising from Isaac
  /// as he sleeps (its render)
  ACTOR_FIRST_SLEEP_PARTICLE = 41
};

/// The temporary flags that keep what the friends changed on the stages, for
/// the race, and the scales' state.
///
/// ColosseumNavampaMatch_Initialize restores the stages from them.
enum {
  /// ★ Named from the game: "Growth" (MsgAbilityName)
  FLAG_PLANT_GROWN = FLAG_AREA_TEMP_60, ///< by Growth, into a shortcut
  /// ★ Named from the game: "This place is called the Wall Wedge."
  /// (MsgColossoPlaceCalledWall)
  FLAG_WALL_WEDGE_HALTED =
      FLAG_AREA_TEMP_61, ///< by Halt on ACTOR_WALL_OPERATOR
  /// ★ Named from the game: "This stage is called the Scales of Justice."
  /// (MsgColossoStageCalledScales)
  FLAG_SCALES_FROZEN = FLAG_AREA_TEMP_62, ///< by Frost on ACTOR_PUDDLE
  /// By Force on ACTOR_SIDEWALK_LOG. The belt carries no one any more.
  /// ★ Named from the game: "This is called the Moving Sidewalk Stage."
  /// (MsgColossoCalledMovingSidewalkStage)
  FLAG_SIDEWALK_LOG_KNOCKED_OFF = FLAG_AREA_TEMP_63,
  /// Someone stands on the western ledge, which has sunk
  /// (ColosseumNavampaMatch_StepOnScales).
  /// ★ Named from the game: "This stage is called the Scales of Justice."
  /// (MsgColossoStageCalledScales)
  FLAG_ON_SCALES = FLAG_AREA_TEMP_67,
  /// ★ Named from the game: "This stage is called the Scales of Justice."
  /// (MsgColossoStageCalledScales)
  FLAG_WEIGHT_ON_SCALES =
      FLAG_AREA_TEMP_68, ///< ACTOR_SCALES_WEIGHT is on the western ledge
  /// ★ Named from the game: "To get across the tank, you'll need to roll the
  /// logs." (MsgColossoAnotherLogRollingArea)
  FLAG_LONG_LOG_IN_PLACE =
      FLAG_AREA_TEMP_69, ///< at the east end
                         ///< (ColosseumNavampaMatch_RollLongLog)
  /// The byte of flags that keeps ACTOR_STONE's column.
  /// ★ Named from the game: "You can get through quickly by moving the logs and
  /// stones." (MsgColossoAnotherLogRollingArea)
  FLAG_STONE_COLUMN = FLAG_AREA_TEMP_70,
  /// Someone has reached the arena
  /// (Colosso_TakePrize).
  /// ★ Named from the game: "You must get through several stages before you
  /// reach the arena." (MsgColossoWarriorsEnterFinalsWithoutAny)
  FLAG_ARENA_REACHED = FLAG_SCENE_TEMP_11,
  /// ACTOR_LOG_AREA_STONE_A has settled. The next two flags are B's and C's.
  /// ★ Named from the game: "You can get through quickly by moving the logs and
  /// stones." (MsgColossoAnotherLogRollingArea)
  FLAG_FIRST_LOG_AREA_STONE_SETTLED = FLAG_SCENE_TEMP_14
};

enum { PISTON_COUNT = 5 };

/// Sets the leader speed for the shared solo-party placement routine.
static inline void InitializeActorZero(void) {
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
}

/// Sets a companion speed for the shared solo-party placement routine.
static inline void InitializeSelectedActor(s32 actorId) {
  Actor_SetSpeed(actorId, 1);
}

const struct SceneEntrance *ColosseumNavampaMatch_GetEntrances(void) {
  return gColosseumNavampaMatchEntrances;
}

const struct SceneExitRegion *ColosseumNavampaMatch_GetRegions(void) {
  return 0;
}

const u32 *ColosseumNavampaMatch_GetExits(void) {
  return gColosseumNavampaMatchExits;
}

const struct ActorPlacement *ColosseumNavampaMatch_GetActorPlacements(void) {
  return gColosseumNavampaMatchActorPlacements;
}

/// The Wall Wedge's task cycles its pistons through their four patterns.
///
/// It raises the opening event (4 or 5) when the rider stands in an open gap.
static void ColosseumNavampaMatch_UpdateWallWedge(void) {
  struct FieldWork *work = gFieldWork;
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 row = FIELD_CELL(actor->z.fixed);
  s32 i;
  s32 kind;

  if (gColosseumNavampaMatch_WallWedgeTimer == 0) {
    gColosseumNavampaMatch_WallWedgePatternIndex =
        (gColosseumNavampaMatch_WallWedgePatternIndex + 1) & 3;
    for (i = ACTOR_FIRST_PISTON; i <= ACTOR_LAST_PISTON; i++) {
      kind = gColosseumNavampaMatch_WallWedgePattern
          [gColosseumNavampaMatch_WallWedgePatternIndex]
          [i - ACTOR_FIRST_PISTON];
      Actor_SetAnimation(i, kind);
      Actor_SetAnimation(i + PISTON_COUNT, kind + 8);
      MapGrid_CopyCellAttributeRect(32, 11, 1, 2,
                                    (i - ACTOR_FIRST_PISTON) * 2 + 33, 11);
      if (kind != 7) {
        MapGrid_CopyCellAttributeRect(74, 12, 1, 1,
                                      (i - ACTOR_FIRST_PISTON) * 2 + 33, 11);
      }
    }
    Actor_SetAnimation(
        ACTOR_WALL_OPERATOR,
        gColosseumNavampaMatch_WallWedgePattern
            [gColosseumNavampaMatch_WallWedgePatternIndex][PISTON_COUNT]);
  } else {
    for (i = ACTOR_FIRST_PISTON; i <= ACTOR_LAST_PISTON; i++) {
      kind = gColosseumNavampaMatch_WallWedgePattern
          [gColosseumNavampaMatch_WallWedgePatternIndex]
          [i - ACTOR_FIRST_PISTON];
      if ((u32)(actor->x.fixed - (i << 21) + 0x31ffff) <= 0x13fffe) {
        if (row == 11 && kind == 4)
          work->raisedTrigger = kind;
        if (row == 12 && kind == 5)
          work->raisedTrigger = kind;
      }
    }
  }
  if ((u32)++gColosseumNavampaMatch_WallWedgeTimer > 17) {
    gColosseumNavampaMatch_WallWedgeTimer = 0;
  }
}

// Function from another file; its callers use no declaration (implicit int).

/// Resets the Wall Wedge's pistons to their first pattern.
static void ColosseumNavampaMatch_ResetWallWedge(void) {
  MapTaskEntry entry;

  gColosseumNavampaMatch_WallWedgeTimer = 0;
  entry = ColosseumNavampaMatch_UpdateWallWedge;
  gColosseumNavampaMatch_WallWedgePatternIndex = 0;
  Task_Remove(entry);
  entry();
}

/// Resumes cycling the Wall Wedge's pistons.
static void ColosseumNavampaMatch_StartWallWedge(void) {
  Task_AddFrame(ColosseumNavampaMatch_UpdateWallWedge, TASK_PASS_FRAME);
}

/// Waits, at most two seconds, for the Wall Wedge pattern its attendant's
/// demonstration passes through.
static void ColosseumNavampaMatch_WaitForWallWedgeOpening(void) {
  s32 polls;

  wait(10);

  polls = 0;
  while (gColosseumNavampaMatch_WallWedgePatternIndex != 3 ||
         gColosseumNavampaMatch_WallWedgeTimer != 1) {
    wait(1);
    polls++;
    if (polls > 119) {
      return;
    }
  }
}

/// The Moving Sidewalk Stage's belt carries the party west.
///
/// It's step trigger 11, and until FLAG_SIDEWALK_LOG_KNOCKED_OFF is set the
/// belt carries the controlled actor and the camera west.
static void ColosseumNavampaMatch_RideMovingSidewalk(void) {
  struct FieldWork *work = gFieldWork;
  struct MainRoutineWork *state = &gMainRoutineWork;
  s32 id = state->leaderActorId;
  struct Actor *subject = work->camera;
  struct Actor *actor = Actor_Get(id);
  s32 z = actor->z.part.pixel;

  if ((u32)(z - 183) <= 3) {
    subject->x.fixed += -(px(4) / 5);
    actor->x.fixed += -(px(4) / 5);
  }
}

/// Growth on the plant on Colosseum Final 3's stages makes it grow and open a
/// shortcut (FLAG_PLANT_GROWN).
static void ColosseumNavampaMatch_GrowPlant(void) {
  GameFlag_Set(FLAG_PLANT_GROWN);
  {

    MapGrid_CopyCellAttributes(47, 61, 1, 4, 49, 61);
  }
}

/// Moves a scale ledge vertically to a height in whole pixels, at the shared
/// speed and acceleration.
static inline void ColosseumNavampaMatch_MoveOnScales(struct Actor *ledge,
                                                      s32 height) {
  ledge->acceleration = px(2) / 5;
  ledge->speedLimit = px(4) / 5;
  FieldActor_SetMoveTargetFixed(ledge, ledge->x.fixed, px(height),
                                ledge->z.fixed);
}

/// Frost on the target on the Scales of Justice's eastern ledge freezes it.
///
/// The scales tip, the western ledge down and the eastern ledge up
/// (FLAG_SCALES_FROZEN).
static void ColosseumNavampaMatch_FreezeOnScales(void) {
  struct Actor *actor;

  actor = Actor_Get(ACTOR_PUDDLE);
  actor->scaleX = Q16_ONE;
  actor->scaleY = Q16_ONE;

  actor = Actor_Get(ACTOR_SCALES_EAST_LEDGE);
  ColosseumNavampaMatch_MoveOnScales(actor, 32);

  actor = Actor_Get(ACTOR_SCALES_WEST_LEDGE);
  ColosseumNavampaMatch_MoveOnScales(actor, 4);

  GameFlag_Set(FLAG_SCALES_FROZEN);
  MapGrid_CopyCellAttributes(15, 12, 1, 1, 13, 12);
  MapGrid_CopyCellAttributes(14, 12, 1, 1, 9, 12);
}

/// Stepping onto the Scales of Justice's western ledge sinks it, unless the
/// scales are frozen.
///
/// It sinks with the rider as the eastern ledge rises (FLAG_ON_SCALES).
static void ColosseumNavampaMatch_StepOnScales(void) {
  s32 actorId;
  s32 lowered;
  struct Actor *log;
  struct Actor *rider;
  u8 *flags;

  actorId = gMainRoutineWork.leaderActorId;
  lowered = GameFlag_Get(FLAG_SCALES_FROZEN);
  if (lowered == 0) {
    log = Actor_Get(ACTOR_SCALES_WEST_LEDGE);
    if (log != NULL) {
      Actor_SetDestination(actorId, log->x.part.pixel, log->z.part.pixel);
    }
    Actor_WaitForMoveById(actorId);
    log = Actor_Get(ACTOR_SCALES_EAST_LEDGE);
    log->motionFlags = lowered;
    ColosseumNavampaMatch_MoveOnScales(log, 32);
    log = Actor_Get(ACTOR_SCALES_WEST_LEDGE);
    log->motionFlags = lowered;
    ColosseumNavampaMatch_MoveOnScales(log, 4);
    rider = Actor_Get(actorId);
    flags = &rider->motionFlags;
    *flags = lowered;
    ColosseumNavampaMatch_MoveOnScales(rider, 4);
    Actor_SetSpriteFlags(rider, SPRITE_FLAG_SHADOW);
    Actor_WaitForMoveById(actorId);
    MapGrid_CopyCellAttributes(0, 24, 1, 1, 9, 12);
    wait(2);
    Actor_SetSpriteFlags(rider, SPRITE_FLAG_SHADOW);
    *flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    rider->groundHeight = rider->y.fixed;
    GameFlag_Set(FLAG_ON_SCALES);
  }
}

/// Stepping off the Scales of Justice's western ledge swings the scales back.
static void ColosseumNavampaMatch_StepOffScales(void) {
  struct Actor *actor;

  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);

  actor = Actor_Get(ACTOR_SCALES_EAST_LEDGE);
  actor->motionFlags = 0;
  ColosseumNavampaMatch_MoveOnScales(actor, 4);

  actor = Actor_Get(ACTOR_SCALES_WEST_LEDGE);
  actor->motionFlags = 0;
  ColosseumNavampaMatch_MoveOnScales(actor, 32);

  Actor_WaitForMoveById(ACTOR_SCALES_WEST_LEDGE);
  {
    MapGrid_CopyCellAttributes(0, 25, 1, 1, 9, 12);
  }
  wait(2);
  GameFlag_Clear(FLAG_ON_SCALES);
}

/// Move on the weight by the Scales of Justice can set it on the scales.
///
/// When it reaches the western ledge, it sinks the ledge and the eastern ledge
/// rises (FLAG_WEIGHT_ON_SCALES).
static void ColosseumNavampaMatch_SetWeightOnScales(void) {
  struct Actor *probe;
  struct Actor *log;
  struct Actor *upper;
  struct Actor *lower;
  s32 x;
  s32 z;

  probe = Actor_Get(ACTOR_SCALES_WEIGHT);
  x = FIELD_CELL(probe->x.fixed);
  if (x == 9) {
    z = FIELD_CELL(probe->z.fixed);
    if (z == 12) {
      log = Actor_Get(ACTOR_SCALES_WEIGHT);
      Actor_SetSpriteFlags(log, 0);
      log->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      log->motionFlags = 0;
      ColosseumNavampaMatch_MoveOnScales(log, 4);
      upper = Actor_Get(ACTOR_SCALES_EAST_LEDGE);
      upper->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      ColosseumNavampaMatch_MoveOnScales(upper, 32);
      lower = Actor_Get(ACTOR_SCALES_WEST_LEDGE);
      ColosseumNavampaMatch_MoveOnScales(lower, 4);
      GameFlag_Set(FLAG_WEIGHT_ON_SCALES);
      MapGrid_CopyCellAttributes(15, 12, 1, 1, 13, z);
      MapGrid_CopyCellAttributes(1, 25, 1, 1, x, z);
    }
  }
}

/// Walking into the weight by the Scales of Justice pushes it, perhaps onto the
/// scales.
static void ColosseumNavampaMatch_PushWeight(void) {
  Colosso_PushActorAhead();
  ColosseumNavampaMatch_SetWeightOnScales();
}

/// Move on the stone on Colosseum Final 3's stages, and each push, keeps where
/// it stands.
///
/// Its column is kept in the byte of flags from FLAG_STONE_COLUMN, and its
/// cell is rebuilt.
static void ColosseumNavampaMatch_MoveStone(void) {
  struct Actor *actor;
  s32 x;

  actor = Actor_Get(ACTOR_STONE);
  x = FIELD_CELL(actor->x.fixed);
  FieldGameFlag_SetByte(FLAG_STONE_COLUMN, x);
  MapGrid_CopyCellAttributes(18, 10, 3, 1, 18, 11);
  MapGrid_CopyCellAttributes(17, 11, 1, 1, x, 11);
}

/// Move on ACTOR_ROCK in Colosseum Final 3 needs nothing more.
static void ColosseumNavampaMatch_IgnoreMove(void) {}

/// Walking into a rock on Colosseum Final 3's stages (step trigger 22) pushes
/// it.
static void ColosseumNavampaMatch_PushRock(void) { Colosso_PushActorAhead(); }

/// Move on one of the log-rolling area's three stones, and each push there,
/// settles the stones.
///
/// A stone resting on the floor becomes a step
/// (FLAG_FIRST_LOG_AREA_STONE_SETTLED and the two after it).
static void ColosseumNavampaMatch_SettleStones(void) {
  struct Actor *actor;
  s32 slot;
  s32 x;
  s32 z;
  s32 x2;
  s32 z2;

  for (slot = ACTOR_LOG_AREA_STONE_A; slot <= ACTOR_LOG_AREA_STONE_C; slot++) {
    actor = Actor_Get(slot);
    if (MapGrid_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed) == 0) {
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      actor->motionFlags = 0;
      x = FIELD_CELL(actor->x.fixed);
      z = FIELD_CELL(actor->z.fixed);
      MapGrid_CopyCellAttributes(83, 13, 1, 1, x, z);
      x2 = FIELD_CELL(actor->x.fixed);
      z2 = FIELD_CELL(actor->z.fixed);
      MapGrid_CopyCellAttributes(83, 13, 1, 1, x2, z2 + 52);
      GameFlag_Set(slot - ACTOR_LOG_AREA_STONE_A +
                   FLAG_FIRST_LOG_AREA_STONE_SETTLED);
    }
  }
}

/// A task for Colosseum Final 3's eastern end.
///
/// The cell at column 81, row 12 lets the rider through only while facing
/// south.
static void ColosseumNavampaMatch_UpdateEastEndCollision(void) {
  struct Actor *actor;
  s32 x;
  s32 z;
  s32 behavior;

  actor = Actor_Get(gMainRoutineWork.leaderActorId);
  x = FIELD_CELL(actor->x.fixed);
  behavior = 23;
  z = FIELD_CELL(actor->z.fixed);
  if (x == 81 && z == 12) {
    if ((actor->facing & 0xE000) == FACING_SOUTH) {
      behavior = CELL_BEHAVIOR_CLIMB;
    }
    MapGrid_SetCellBehaviorAt(0, x << 20, z << 20, behavior);
  }
}

/// Rejects blocked cells and cells occupied by one of the log-rolling area's
/// three stones.
static s32 ColosseumNavampaMatch_CheckLongLogCell(s32 x, s32 z) {
  struct Actor *actor;

  if (MapGrid_GetCellBehaviorAt(0, x, z) == CELL_BEHAVIOR_WALL) {
    return -2;
  }
  actor = Actor_Get(ACTOR_LOG_AREA_STONE_A);
  x >>= 20;
  z >>= 20;
  if (FIELD_CELL(actor->x.fixed) == x && FIELD_CELL(actor->z.fixed) == z) {
    return -1;
  }
  actor = Actor_Get(ACTOR_LOG_AREA_STONE_B);
  if (FIELD_CELL(actor->x.fixed) == x && FIELD_CELL(actor->z.fixed) == z) {
    return -1;
  }
  actor = Actor_Get(ACTOR_LOG_AREA_STONE_C);
  if (FIELD_CELL(actor->x.fixed) == x && FIELD_CELL(actor->z.fixed) == z) {
    return -1;
  }
  return 0;
}

/// Checks all four cells covered by the long rolling log.
static s32 ColosseumNavampaMatch_CheckLongLogPath(s32 x, s32 z) {
  if (ColosseumNavampaMatch_CheckLongLogCell(x, z - px(24)) != 0 ||
      ColosseumNavampaMatch_CheckLongLogCell(x, z - px(8)) != 0 ||
      ColosseumNavampaMatch_CheckLongLogCell(x, z + px(8)) != 0 ||
      ColosseumNavampaMatch_CheckLongLogCell(x, z + px(24)) != 0) {
    return -1;
  }
  return 0;
}

/// The long log carries the leader until something stops it.
///
/// Walking into the log-rolling area's long log (step trigger 23) rolls it,
/// with the leader, the way the pad is held. At the east end it drops into
/// place and the path is made (FLAG_LONG_LOG_IN_PLACE).
///
/// Away from the log, a stone ahead is pushed instead.
static void ColosseumNavampaMatch_RollLongLog(void) {
  struct Actor *rider;
  struct Actor *log;
  struct FieldWork *work;
  s32 angle;
  s32 actorId;
  s32 x;
  s32 z;
  s32 steps;
  s32 nextX;
  s32 column;
  s32 acceleration;
  s32 mode;
  s32 step;
  s32 ahead[3];

  rider = Actor_Get(gMainRoutineWork.leaderActorId);
  log = Actor_Get(ACTOR_LONG_LOG);
  steps = 0;
  angle = gColosseumNavampaMatch_FacingAngles[rider->facing >> 13];
  actorId = gMainRoutineWork.leaderActorId;
  SET_VECTOR(ahead, (rider->x.fixed & -0x100000) + px(8), rider->y.fixed,
             (rider->z.fixed & -0x100000) + px(8));
  Vector_AddPolarOffset(px(16), angle, ahead);
  x = log->x.fixed;
  z = log->z.fixed;
  // Only a nearby rider rolls the log; other interactions push the actor
  // ahead. Written with goto: as an if block round the rolling that returns,
  // or with the far case first as an early return, tbs-en does not match.
  if ((ahead[0] - x >= 0 ? ahead[0] - x : x - ahead[0]) > px(8) ||
      (ahead[2] - z >= 0 ? ahead[2] - z : z - ahead[2]) > px(32)) {
    goto far;
  }
  if (gKeysHeld & KEY_LEFT) {
    mode = 2;
    step = -8;
    for (;;) {
      nextX = x - px(16);
      if (ColosseumNavampaMatch_CheckLongLogPath(nextX, z) == 0) {
        steps++;
        x = nextX;
      } else {
        break;
      }
    }
  } else {
    if (!(gKeysHeld & KEY_RIGHT)) {
      return;
    }
    mode = 3;
    step = 8;
    for (;;) {
      nextX = x + px(16);
      if (ColosseumNavampaMatch_CheckLongLogPath(nextX, z) == 0) {
        steps++;
        x = nextX;
      } else {
        break;
      }
    }
  }
  if (steps == 0) {
    return;
  }
  // Clear the old footprint, then move the log and rider together.
  MapGrid_CopyCellAttributes(74, 8, 1, 4, FIELD_CELL(log->x.fixed), 9);
  MapGrid_CopyCellAttributes(120, 60, 8, 5, 74, 60);
  Event_Start();
  Actor_SetAnimation(actorId, ANIM_PUSH);
  Event_Wait(6);
  log->speedLimit = px(1) / 2;
  acceleration = px(1) / 5;
  log->acceleration = acceleration;
  Actor_SelectAnimation(log, mode);
  FieldActor_SetMoveTargetFixed(log, x, 0, z);
  Event_Wait(6);
  Actor_SetAnimation(actorId, ANIM_WALK);
  work = HeapSlot_AllocEwram(HEAP_SLOT_FIELD, sizeof(struct FieldWork));
  Actor_StartCamera(work->camera, (s32)log);
  Actor_SetSpeedFixed(actorId, px(1) / 2, acceleration);
  Sound_Play(SE_HEAVY_MOVE);
  Actor_SelectAnimation(rider, ANIM_WALK);
  FieldActor_SetMoveTargetFixed(rider, px(steps * step) + rider->x.fixed, 0,
                                rider->z.fixed);
  Actor_WaitForMove(rider);
  Actor_SelectAnimation(rider, ANIM_STAND);
  Actor_WaitForMove(log);
  // At the eastern end the log settles at the end of its stage.
  if (x >= px(1328)) {
    GameFlag_Set(FLAG_LONG_LOG_IN_PLACE);
    Actor_SetAnimation(ACTOR_LONG_LOG, LOG_ANIMATION_ROLL_WEST);
    Actor_SetDestinationOffset(ACTOR_LONG_LOG, 18, 6);
    Event_Wait(30);
    Actor_SelectAnimation(log, LOG_ANIMATION_DROP_IN);
    Actor_WaitForMove(log);
    log->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    column = 84;
    MapGrid_CopyCellAttributes(86, 10, 1, 2, column, 10);
    MapGrid_CopyCellAttributes(86, 9, 1, 1, column, 12);
    Sound_Play(SE_STOP_ACTION_SE);
    Sound_Play(SE_LOG_FALL);
  } else {
    Actor_SelectAnimation(log, 1);
    Sound_Play(SE_STOP_ACTION_SE);
    Sound_Play(SE_HEAVY_STOP);
    column = x >> 20;
    MapGrid_CopyCellAttributes(85, 9, 1, 4, column, 9);
    MapGrid_CopyCellAttributes(85, 9, 1, 4, column, 61);
  }
  Event_Wait(15);
  Event_End();
  return;
far:
  Colosso_PushActorAhead();
  ColosseumNavampaMatch_SettleStones();
}

/// Displays the activated moving sidewalk and plays its arrival sounds.
static s32
ColosseumNavampaMatch_ShowActivatedSidewalkLayer(struct Actor *interpreter) {
  TilesetAnim_Pause(1);
  TilesetAnim_Resume(2);
  Sound_Play(SE_STOP_ACTION_SE);
  Sound_Play(SE_FLOOR_SWITCH);
  return 0;
}

/// Force on the Moving Sidewalk Stage's log makes it slide away.
///
/// The belt carries no one any more (FLAG_SIDEWALK_LOG_KNOCKED_OFF).
static void ColosseumNavampaMatch_ForceSidewalkLog(void) {
  struct Actor *effect;
  s32 moveRate;

  effect = Actor_Get(ACTOR_SIDEWALK_LOG);
  effect->motionFlags = 0;
  moveRate = px(8) / 5;
  effect->acceleration = moveRate;
  effect->speedLimit = moveRate;
  Actor_SelectAnimation(effect, 2);
  Actor_StartScript(effect, gColosseumNavampaMatch_SidewalkLogScript);
  GameFlag_Set(FLAG_SIDEWALK_LOG_KNOCKED_OFF);
}

/// Halt on the Wall Wedge's operator stops the pistons at their next open
/// pattern (FLAG_WALL_WEDGE_HALTED).
static void ColosseumNavampaMatch_HaltWall(void) {
  FieldPsynergy_HaltActor(ACTOR_WALL_OPERATOR);
  GameFlag_Set(FLAG_WALL_WEDGE_HALTED);
  wait(10);
  while (gColosseumNavampaMatch_WallWedgePatternIndex != 1 &&
         gColosseumNavampaMatch_WallWedgePatternIndex != 3)
    wait(1);
  wait(1);
  Task_Remove(ColosseumNavampaMatch_UpdateWallWedge);
}

/// Raised by the Wall Wedge (4) as the rider passes through the pistons'
/// northern opening.
static void ColosseumNavampaMatch_PassNorthWallOpening(void) {
  struct Actor *actor;
#ifndef UBFIX
  s16 *gameWords;
#endif
  s32 *actorId;
  s32 z;

#ifdef UBFIX
  actorId = &gMainRoutineWork.leaderActorId;
#else
  // The original word-indexed address takes three more instructions here.
  gameWords = (s16 *)&gMainRoutineWork;
  actorId = (s32 *)&gameWords[offsetof(struct MainRoutineWork, leaderActorId) /
                              sizeof(s16)];
#endif
  actor = Actor_Get(*actorId);
  actor->acceleration = px(1);
  actor->speedLimit = px(2);
  Actor_SetAttachedEffect(*actorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SelectAnimation(actor, ANIM_LEADER_RUN);
  z = actor->z.fixed & FIELD_CELL_MASK;
  FieldActor_SetMoveTargetFixed(actor, actor->x.fixed, actor->y.fixed,
                                z + px(24));
  Actor_WaitForMove(actor);
}

/// Raised by the Wall Wedge (5) as the rider passes through the pistons'
/// southern opening.
static void ColosseumNavampaMatch_PassSouthWallOpening(void) {
  struct Actor *actor;
#ifndef UBFIX
  s16 *gameWords;
#endif
  s32 *actorId;
  s32 z;

#ifdef UBFIX
  actorId = &gMainRoutineWork.leaderActorId;
#else
  // The original word-indexed address takes three more instructions here.
  gameWords = (s16 *)&gMainRoutineWork;
  actorId = (s32 *)&gameWords[offsetof(struct MainRoutineWork, leaderActorId) /
                              sizeof(s16)];
#endif
  actor = Actor_Get(*actorId);
  if (actor->x.fixed > px(664)) {
    actor->x.fixed = px(664);
  }
  actor->acceleration = px(1);
  actor->speedLimit = px(2);
  Actor_SelectAnimation(actor, ANIM_LEADER_RUN);
  z = actor->z.fixed & FIELD_CELL_MASK;
  FieldActor_SetMoveTargetFixed(actor, actor->x.fixed, actor->y.fixed,
                                z + px(12));
  Actor_WaitForMove(actor);
  Actor_SetAttachedEffect(*actorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_JumpDownLedge(*actorId, 6, 0);
}

/// Unused: an empty function nothing refers to.
UNUSED static void ColosseumNavampaMatch_UnusedNoOp(void) {}

// XXX (no evidence): which lines this question was written for; in the
// Japanese catalog, as in the English one, the lines three apart per character
// from MsgColossoDidntThinkBattles are a spectator's thought and the stage
// announcements, and no catalog older than the released ones survives.
/// Unused: talking to a character at the third finals site would have handed
/// the player control of that character.
///
/// Nothing calls it. Arriving at COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH
/// it would ask a yes or no question and, on yes, make the character it names
/// the one the player moves alone, as the friends' turns do.
UNUSED static void
ColosseumNavampaMatch_UnusedAskToControlCharacter(s32 characterId) {
  struct FieldWork *work;
  s32 actorId;
  s32 messageOffset;
  s32 entrance;

  work = gFieldWork;
  actorId = gMainRoutineWork.leaderActorId;
  entrance = gMainRoutineWork.entrance;
  if (entrance == COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Event_Start();
    messageOffset = characterId * 3;
    Event_SetMessageId(messageOffset + MsgColossoDidntThinkBattles);
    Event_OpenMessage(characterId, 0);
    if (Event_AskYesNoWithActorAnimation(actorId, 0) == 0) {
      Event_SetMessageId(messageOffset + MsgColossoDidntThinkBattles + 1);
      Event_ShowMessage(characterId, 0);
      work->transition =
          SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
      work->transitionFrames = 15;
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
      Colosso_ControlCharacterAlone(characterId);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
    } else {
      Event_SetMessageId(messageOffset + MsgColossoDidntThinkBattles + 2);
      Event_ShowMessage(characterId, 0);
    }
    Event_End();
  }
}

/// Reaching Colosseum Final 3's arena, the leader takes a prize and fights
/// Navampa.
///
/// The prize is the Claymore if he is first, the Hunter's Sword if second. He
/// and Navampa face off and fight, and the battle's group follows who took the
/// Claymore.
///
/// The arena's step trigger row waits on FLAG_COLOSSEUM_ARENA_CLOSED being
/// clear, which nothing in the game sets (colosseum_azart_match.c's
/// ColosseumNavampaMatch_ReachArena).
static void ColosseumNavampaMatch_ReachArena(void) {
  s32 place;
  s32 i;
  s32 last;

  last = 4;
  Colosso_NoOp();
  Event_Start();
  place = Colosso_TakePrize(ITEM_CLAYMORE, ITEM_HUNTERS_SWORD);
  Colosso_WaitForOpponentToFinish();
  for (i = 0; i < 10; i++) {
    Actor_WaitForScriptEndById(ACTOR_OPPONENT);
  }
  Actor_SetSpeed(ACTOR_OPPONENT, 1);
  Actor_WalkTo(ACTOR_OPPONENT, 1528, 192);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 1496, 192);
  Actor_SetAnimation(ACTOR_OPPONENT, ANIM_STAND);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_OPPONENT, 0);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_OPPONENT, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 2);
  Actor_SetSpeed(ACTOR_OPPONENT, 2);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 1504, 192);
  Actor_WalkToAndWait(ACTOR_OPPONENT, 1520, 192);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
  Actor_SetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_FIGHTING_STANCE);
  Event_Wait(10);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_COLOSSO,
                                        last - place + 1);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination((s32)&SceneId_ColosseumNavampaMatch,
                                  COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON);
  Event_SetDefeatDestination((s32)&SceneId_ColosseumNavampaMatch,
                             COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_LOST);
  GameFlag_Set(FLAG_KEEP_MUSIC_ONCE);
}

/// Walking into something on Colosseum Final 3's stages (step trigger 90)
/// makes the leader jump forward where he can.
///
/// He does not jump when it is an actor (Field_TryJumpForward).
static void ColosseumNavampaMatch_JumpAtLogAreaEnd(void) {
  if (Colosso_FindActorAhead() == 0) {
    Field_TryJumpForward();
  }
}

/// Walking into something by the Scales of Justice (step trigger 91) pushes
/// the weight, and checks anything else.
static void ColosseumNavampaMatch_PushWeightOrCheckAhead(void) {
  if (Colosso_FindActorAhead() == 0) {
    Field_TryJumpForward();
  } else {
    ColosseumNavampaMatch_PushWeight();
  }
}

const struct Trigger *ColosseumNavampaMatch_GetTriggers(void) {
  return gColosseumNavampaMatchTriggers;
}

/// Sets up Colosseum Final 3 for the visit its entrance stands for.
///
/// It restores obstacle progress, then goes by the arrival:
///
/// - the race (COLOSSEUM_NAVAMPA_MATCH_RACE_FROM_LEND_A_HAND),
/// - the friends' turns to lend a hand
///   (COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH),
/// - the guide's tour (COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_NORTH),
/// - the end of Colosso after a win (COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON),
/// - or after a loss (COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_LOST).
s32 ColosseumNavampaMatch_Initialize(void) {
  struct Actor *actor;
  s32 i;
  s32 height;
  s32 column;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  // Restore the scale ledges, movable weight and supporting cell behaviors.
  Task_AddFrame(ColosseumNavampaMatch_UpdateEastEndCollision, TASK_PASS_FRAME);
  MapGrid_CopyCellAttributes(74, 60, 8, 6, 120, 60);

  Actor_SetSpriteFlags(Actor_Get(ACTOR_PUDDLE), 0);

  actor = Actor_Get(ACTOR_SCALES_WEST_LEDGE);
  Actor_SetSpriteFlags(actor, 0);
  actor->motionFlags = 0;
  actor->y.fixed = px(32);

  actor = Actor_Get(ACTOR_SCALES_EAST_LEDGE);
  Actor_SetSpriteFlags(actor, 0);
  actor->motionFlags = 0;
  actor->y.fixed = px(4);

  if (GameFlag_Get(FLAG_SCALES_FROZEN) != 0) {
    Actor_SetAnimation(ACTOR_PUDDLE, PUDDLE_ANIMATION_PILLAR);
    actor = Actor_Get(ACTOR_SCALES_WEST_LEDGE);
    actor->y.fixed = px(4);
    actor = Actor_Get(ACTOR_SCALES_EAST_LEDGE);
    actor->y.fixed = px(32);
    MapGrid_CopyCellAttributes(15, 12, 1, 1, 13, 12);
  } else {
    actor = Actor_Get(ACTOR_PUDDLE);
    actor->scaleX = 3 * Q16_ONE / 2;
    actor->scaleY = 3 * Q16_ONE / 2;
    // The cells ColosseumNavampaMatch_StepOnScales copies as it sets the flag,
    // redrawn on arrival.
    if (GameFlag_Get(FLAG_ON_SCALES) != 0) {
      MapGrid_CopyCellAttributes(0, 24, 1, 1, 9, 12);
    } else {
      MapGrid_CopyCellAttributes(0, 25, 1, 1, 9, 12);
    }
  }

  // The Scales of Justice are two pans on poles. With the weight, a rock, on
  // the western ledge they have tipped: the west pan hangs low and the east
  // one high, where otherwise the west is high and the east low. The two
  // cells under the pans take the ground that goes with it.
  if (GameFlag_Get(FLAG_WEIGHT_ON_SCALES) != 0) {
    MapGrid_CopyCellAttributes(15, 12, 1, 1, 13, 12);
    MapGrid_CopyCellAttributes(1, 25, 1, 1, 9, 12);
    actor = Actor_Get(ACTOR_SCALES_WEIGHT);
    Actor_SetSpriteFlags(actor, 0);
    actor->motionFlags = 0;
    actor->y.fixed = px(2);
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    actor = Actor_Get(ACTOR_SCALES_WEST_LEDGE);
    actor->y.fixed = px(4);
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    actor = Actor_Get(ACTOR_SCALES_EAST_LEDGE);
    actor->y.fixed = px(32);
  }

  column = FieldGameFlag_GetByte(FLAG_STONE_COLUMN);
  if (column == 0) {
    column = 19;
  }
  actor = Actor_Get(ACTOR_STONE);
  actor->x.fixed = (column << 20) + (px(8));
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  MapGrid_CopyCellAttributes(18, 10, 3, 1, 18, 11);
  MapGrid_CopyCellAttributes(17, 11, 1, 1, column, 11);

  for (i = ACTOR_LOG_AREA_STONE_A; i <= ACTOR_LOG_AREA_STONE_C; i++) {
    actor = Actor_Get(i);
    height = MapGrid_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
    if (actor->y.fixed == 0 && height == 0) {
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      actor->motionFlags = height;
      MapGrid_CopyCellAttributes(83, 13, 1, 1, FIELD_CELL(actor->x.fixed),
                                 FIELD_CELL(actor->z.fixed));
      MapGrid_CopyCellAttributes(83, 13, 1, 1, FIELD_CELL(actor->x.fixed),
                                 (FIELD_CELL(actor->z.fixed)) + 52);
    }
  }

  // Halt preserves a stationary wall; otherwise restart its pattern cycle.
  if (GameFlag_Get(FLAG_WALL_WEDGE_HALTED) != 0) {
    for (i = ACTOR_FIRST_PISTON; i <= ACTOR_LAST_PISTON; i++) {
      actor = Actor_Get(i);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      Actor_SelectAnimation(actor, 2);
      actor = Actor_Get(i + PISTON_COUNT);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      actor->motionFlags = 0;
      actor->y.fixed = px(32);
      Actor_SelectAnimation(actor, 10);
      MapGrid_CopyCellAttributes(74, 12, 1, 1, i * 2 - 3, 11);
    }
    Actor_SetAnimation(ACTOR_WALL_OPERATOR, 10);
    FieldPsynergy_HaltActor(ACTOR_WALL_OPERATOR);
  } else {
    for (i = ACTOR_FIRST_PISTON; i <= ACTOR_LAST_PISTON; i++) {
      actor = Actor_Get(i);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      actor = Actor_Get(i + PISTON_COUNT);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      actor->motionFlags = 0;
      actor->y.fixed = px(32);
    }
    Task_AddFrame(ColosseumNavampaMatch_UpdateWallWedge, TASK_PASS_FRAME);
  }

  if (GameFlag_Get(FLAG_PLANT_GROWN) != 0) {
    Actor_SetAnimation(ACTOR_GROWTH_PLANT, ANIM_SHAKE_HEAD);
    MapGrid_CopyCellAttributes(47, 61, 1, 4, 49, 61);
  }

  if (GameFlag_Get(FLAG_SIDEWALK_LOG_KNOCKED_OFF) != 0) {
    TilesetAnim_Pause(1);
    actor = Actor_Get(ACTOR_SIDEWALK_LOG);
    actor->motionFlags = 0;
    actor->x.fixed = px(1130);
    actor->z.fixed = px(184);
    Actor_SetSpriteFlags(actor, 0);
    Actor_SelectAnimation(actor, 3);
    Actor_StartScript(actor, gColosseumNavampaMatch_SidewalkLogOffBelt);
  } else {
    TilesetAnim_Pause(2);
  }

  if (GameFlag_Get(FLAG_LONG_LOG_IN_PLACE) != 0) {
    actor = Actor_Get(ACTOR_LONG_LOG);
    Actor_SelectAnimation(actor, LOG_ANIMATION_DROP_IN);
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(86, 10, 1, 2, 84, 10);
    MapGrid_CopyCellAttributes(86, 9, 1, 1, 84, 12);
  } else {
    actor = Actor_Get(ACTOR_LONG_LOG);
    MapGrid_CopyCellAttributes(85, 9, 1, 4, FIELD_CELL(actor->x.fixed), 9);
    MapGrid_CopyCellAttributes(85, 9, 1, 4, FIELD_CELL(actor->x.fixed), 61);
  }

  actor = Actor_Get(ACTOR_PUDDLE);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  actor = Actor_Get(ACTOR_SCALES_WEST_LEDGE);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  actor = Actor_Get(ACTOR_SCALES_EAST_LEDGE);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_FIGHTING_STANCE);
  gMainRoutineWork.movementMode = LEADER_MOVEMENT_WALK;
  Colosso_ShowItemIcon(ACTOR_PRIZE_A, ITEM_CLAYMORE);
  Colosso_ShowItemIcon(ACTOR_PRIZE_B, ITEM_HUNTERS_SWORD);
  Actor_SetSpriteColorById(ACTOR_OPPONENT, SPRITE_COLOR_RBG);

  // Entrances: the race (1), the friends' turns, the tour, and the end
  // of Colosso after a win (4) or a loss (5).
  switch (gMainRoutineWork.entrance) {
  case COLOSSEUM_NAVAMPA_MATCH_RACE_FROM_LEND_A_HAND:
    Colosso_StartRaceGauge(ACTOR_PARTY_LEADER, ACTOR_OPPONENT, 6, px(1512),
                           px(192), ACTOR_PRIZE_A, ACTOR_PRIZE_B);
    MapGrid_CopyCellsLayered(127, 0, 1, 2, 5, 2);
    Actor_ReleaseById(ACTOR_GUIDE);
    Actor_ReleaseById(ACTOR_SCALES_ATTENDANT);
    Actor_ReleaseById(ACTOR_STEPPING_STONE_ATTENDANT);
    Actor_ReleaseById(ACTOR_WALL_WEDGE_ATTENDANT);
    Actor_ReleaseById(ACTOR_FREE_CLIMB_ATTENDANT);
    Actor_ReleaseById(ACTOR_MOVING_SIDEWALK_ATTENDANT);
    Actor_ReleaseById(ACTOR_LOG_ROLLING_ATTENDANT);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      Sound_Play(SOUND_FADE_OUT_BGM);
      Colosso_ControlCharacterAlone(ISAAC);
      Colosso_PlaceFriendsAtCheeringSpots();
      Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_PARTY_LEADER);
      Colosso_ShowOpeningLettering(3);
    }
    Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_PARTY_LEADER);
    Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_PARTY_LEADER);
    Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_PARTY_LEADER);
    Colosso_StartOpponent((s32)&ResourceId_OpponentPathC);
    break;

  case COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH:
    Task_AddFrame(Colosso_RaiseTakePlaceOnPsynergy, TASK_PASS_FRAME);
    Actor_ReleaseById(ACTOR_PRIZE_A);
    Actor_ReleaseById(ACTOR_PRIZE_B);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
      break;
    }
    Colosso_PlaceFriendsAtCheeringSpots();
    Colosso_ControlCharacterAlone(GARET);
    Colosso_ShowOpeningLettering(0);
    break;

  case COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_NORTH:
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
      break;
    }
    ColosseumNavampaMatch_IntroduceThirdFinals(ACTOR_GUIDE);
    Colosso_ClearCheeringSpots();
    break;

  case COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON:
    ColosseumNavampaMatch_PlayPostBattleReunion(1);
    Import_EventSetExitRequest(COLOSSEUM_NAVAMPA_MATCH_TO_BABI_PALACE);
    GameFlag_Set(FLAG_COLOSSO_OVER);
    GameFlag_Set(FLAG_WON_COLOSSO);
    break;

  case COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_LOST:
    ColosseumNavampaMatch_PlayPostBattleReunion(-1);
    Import_EventSetExitRequest(COLOSSEUM_NAVAMPA_MATCH_TO_BABI_PALACE_LOST);
    GameFlag_Set(FLAG_COLOSSO_OVER);
    break;
  }
  return 0;
}

/// Advances one of the particles that rise from the sleeping Isaac after the
/// last match.
///
/// Its timer and work hold signed 1/256-pixel steps.
static s32
ColosseumNavampaMatch_AdvanceParticleMotion(struct Actor *interpreter) {
  struct Actor *particle = interpreter;
  particle->x.fixed += particle->timer * 256;
  particle->y.fixed += (s16)particle->userValue * 256;
  particle->scaleX += Q16_ONE / 40;
  particle->scaleY += Q16_ONE / 40;
  particle->timer += 5;
  particle->userValue = (s16)particle->userValue - 1;
  return 0;
}

/// Shows one of the three particles that rise from the sleeping Isaac, each at
/// its point in a three-second cycle.
static void ColosseumNavampaMatch_SpawnPeriodicParticle(void) {
  struct Actor *particle;
  struct Actor *source;
  s32 x;
  s32 y;
  s32 kind;
  s32 count;

  particle = Actor_Get(ACTOR_PARTY_LEADER);
  count = gColossoParticleCount + 1;
  kind = ACTOR_FIRST_SLEEP_PARTICLE;
  x = particle->x.fixed;
  y = particle->y.fixed;
  gColossoParticleCount = count;
  switch (count % 180) {
  case 10:
    break;
  case 20:
    kind = ACTOR_FIRST_SLEEP_PARTICLE + 1;
    break;
  case 30:
    kind = ACTOR_FIRST_SLEEP_PARTICLE + 2;
    break;
  default:
    return;
  }
  particle = Actor_Get(kind);
  if (particle == 0) {
    return;
  }
  source = Actor_Get(ACTOR_PARTY_LEADER);
  if (source != 0) {
    Actor_SetPositionFixed(kind, source->x.fixed, source->z.fixed);
  }
  Actor_SetSpriteFlags(Actor_Get(kind), 0);
  particle->motionFlags = 0;
  particle->scaleX = 2 * Q16_ONE / 5;
  particle->scaleY = 2 * Q16_ONE / 5;
  {
    s32 height = px(4);
    particle->x.fixed = x + height;
    height += y;
    particle->y.fixed = height;
    particle->targetY = height;
  }
  particle->timer = 25;
  particle->userValue = 128;
  Actor_SetScript(kind, gColossoParticleKinds);
}

/// After the last match, Garet, Ivan and Mia run to Isaac in the arena, but he
/// falls asleep.
///
/// The entrance is COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON or _LOST. They call
/// his name, but he falls asleep, exhausted, and they leave him to sleep.
static void ColosseumNavampaMatch_PlayPostBattleReunion(s32 battleResult) {
  struct Actor *actor;
  const s32 *leaderScaleScript;
  TaskFunc particleFunc;

  // Place the opponent and companions around the resting leader.
  Actor_ReleaseById(ACTOR_PRIZE_A);
  Actor_ReleaseById(ACTOR_PRIZE_B);
  Party_RestoreHpPp(1);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Event_Start();
  Actor_SetPosition(ACTOR_OPPONENT, 1544, 192);
  if (battleResult < 0) {
    Actor_SetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_ARMS_RAISED);
  } else {
    Actor_SetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_DEFEATED);
  }
  Actor_SetScript(ACTOR_OPPONENT, gColosseumNavampaMatch_OpponentAfterBattle);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 1504, 192);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  {
    u16 shown = 0;

    actor->facing = shown;
  }
  Actor_SetScript(ACTOR_PARTY_LEADER, gColosseumNavampaMatch_LeaderActionA);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CROUCH);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_SetSpeed(ACTOR_IVAN, 1);
  Actor_SetSpeed(ACTOR_MIA, 1);
  Actor_SetPosition(ACTOR_GARET, 1464, 184);
  Actor_SetPosition(ACTOR_IVAN, 1464, 200);
  Actor_SetPosition(ACTOR_MIA, 1448, 192);
  actor = Actor_Get(ACTOR_GARET);
  {
    u16 shown = 0;

    actor->facing = shown;
  }
  actor = Actor_Get(ACTOR_IVAN);
  {
    u16 shown = 0;

    actor->facing = shown;
  }
  actor = Actor_Get(ACTOR_MIA);
  {
    u16 shown = 0;

    actor->facing = shown;
  }
  wait(1);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  PaletteFade_ApplyTarget(PALETTE_TINT_GRAYSCALE, PALETTES_BG);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetMessageId(MsgColossoIsaac);
  Event_Wait(60);
  leaderScaleScript = gColosseumNavampaMatch_LeaderScaleScript;
  Actor_SetScript(ACTOR_PARTY_LEADER, leaderScaleScript);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->scaleX = Q16_ONE;
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->scaleY = Q16_ONE;
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_LEADER_FALL);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->x.fixed += px(3);
  Event_Wait(10);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(actor, 0);
  Event_Wait(20);
  Actor_SetScript(ACTOR_PARTY_LEADER, gColosseumNavampaMatch_LeaderActionB);
  // Let each companion react before the leader settles back to sleep.
  // "Isaac!!!"
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(20);
  Actor_WalkToAndWait(ACTOR_GARET, 1504, 176);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "Isaac..."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_IVAN);
  Event_Wait(30);
  Actor_WalkToAndWait(ACTOR_IVAN, 1488, 176);
  Actor_WalkTo(ACTOR_GARET, 1520, 184);
  Actor_WalkToAndWait(ACTOR_IVAN, 1504, 176);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(10);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_GARET);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(30);
  // "Isaac..."
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_MIA);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_MIA);
  Actor_WalkToAndWait(ACTOR_MIA, 1488, 184);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_PARTY_LEADER);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_PARTY_LEADER);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_GARET);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_IVAN);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(10);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_MIA);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_MIA);
  Event_Wait(20);
  // "Isaac..."
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_SetScript(ACTOR_PARTY_LEADER, gColosseumNavampaMatch_LeaderActionC);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_PARTY_LEADER);
  Event_Wait(20);
  // "Isaac..."
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_PARTY_LEADER);
  Event_Wait(20);
  // "Isaac..."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_PARTY_LEADER);
  Event_Wait(20);
  // "Isaac..."
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_SetScript(ACTOR_PARTY_LEADER, leaderScaleScript);
  Event_Wait(60);
  particleFunc = ColosseumNavampaMatch_SpawnPeriodicParticle;
  // Time the small reunion effects to the companions' reactions.
  gColossoParticleCount = 9;
  Task_AddFrame(particleFunc, TASK_PASS_FRAME);
  Event_Wait(5);
  Task_Remove(particleFunc);
  Event_Wait(55);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Task_AddFrame(particleFunc, TASK_PASS_FRAME);
  Event_Wait(20);
  Task_Remove(particleFunc);
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  gColossoParticleCount = 9;
  Task_AddFrame(particleFunc, TASK_PASS_FRAME);
  Event_Wait(35);
  Task_Remove(particleFunc);
  Event_Wait(25);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  gColossoParticleCount = 9;
  Task_AddFrame(particleFunc, TASK_PASS_FRAME);
  Event_Wait(35);
  Task_Remove(particleFunc);
  Event_Wait(25);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_IVAN);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_MIA);
  Event_Wait(60);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_PARTY_LEADER);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_PARTY_LEADER);
  gColossoParticleCount = 9;
  Task_AddFrame(particleFunc, TASK_PASS_FRAME);
  Event_Wait(35);
  Task_Remove(particleFunc);
  Event_Wait(25);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
  Actor_StartRepeatedMotion(ACTOR_GARET, 3);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 3);
  Actor_RunRepeatedMotion(ACTOR_MIA, 3);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_IVAN);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_IVAN);
  gColossoParticleCount = 9;
  Task_AddFrame(particleFunc, TASK_PASS_FRAME);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Event_Wait(60);
  // The companions begin to leave, then discuss the sleeping leader off-screen.
  Actor_WalkTo(ACTOR_MIA, 1464, 200);
  Event_Wait(5);
  Actor_WalkTo(ACTOR_IVAN, 1368, 184);
  Event_Wait(3);
  Actor_WalkToAndWait(ACTOR_GARET, 1512, 184);
  Actor_WalkTo(ACTOR_GARET, 1368, 184);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_SetAnimation(ACTOR_MIA, ANIM_STAND);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_PARTY_LEADER);
  Event_Wait(60);
  Actor_WalkToAndWait(ACTOR_MIA, 1432, 200);
  Actor_WalkTo(ACTOR_MIA, 1368, 184);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(30);
  Actor_SetPosition(ACTOR_GARET, 1512, 176);
  Actor_SetPosition(ACTOR_IVAN, 1464, 192);
  Actor_SetPosition(ACTOR_MIA, 1560, 200);
  ActorSystem_SuspendProjectionAndShowSprites();
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(1);
  Event_SetMessageId(MsgColossoIsaacFellAsleep);
  // "Isaac fell asleep..."
  Event_ShowMessage(ACTOR_GARET, 0);
  // "He must be exhausted."
  Event_ShowMessage(ACTOR_IVAN, 0);
  // "Let's leave him to sleep for now."
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(60);
  Event_End();
}

/// The guide gives the party the tour of Colosseum Final 3
/// (COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_NORTH).
///
/// He welcomes the party to the third finals match's stages and leaves them
/// to their attendants. Then Garet, Ivan and Mia walk back into Isaac.
static void ColosseumNavampaMatch_IntroduceThirdFinals(s32 guideId) {
  s32 guideZ;
  s32 guideX;
  struct Actor *actor;

  actor = Actor_Get(guideId);
  guideX = actor->x.part.pixel;
  guideZ = actor->z.part.pixel;
  Event_Start();
  Actor_SetSpeed(guideId, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_SetSpeed(ACTOR_IVAN, 1);
  Actor_SetSpeed(ACTOR_MIA, 1);
  Actor_SetPositionFixed(ACTOR_PARTY_LEADER, px(guideX), (px(guideZ) - px(48)));
  Actor_SetPositionFixed(ACTOR_GARET, px(guideX) - px(16), px(guideZ) - px(40));
  Actor_SetPositionFixed(ACTOR_IVAN, px(guideX) + px(16), px(guideZ) - px(40));
  Actor_SetPositionFixed(ACTOR_MIA, px(guideX), (px(guideZ) - px(32)));
  Actor_SetPositionFixed(guideId, px(guideX), (px(guideZ) - px(80)));
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->facing = FACING_NORTH;
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetMessageId(MsgColossoSiteThirdFinals);
  Actor_SetAnimationAndWait(guideId, ANIM_NOD);
  // "This is the site of the third finals match."
  Event_ShowMessage(guideId, 0);
  Actor_RunRepeatedMotion(guideId, 2);
  // "By now, you know that each stage has its own attendant."
  Event_ShowMessage(guideId, 0);
  Actor_RunRepeatedMotion(guideId, 2);
  // "Ask the attendants descriptions of the stages."
  Event_ShowMessage(guideId, 0);
  Actor_RunRepeatedMotion(guideId, 2);
  // "Honestly, Isaac, I didn't think you'd make it this far! I'll..."
  Event_ShowMessage(guideId, 0);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(6);
  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetDestination(ACTOR_GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_SetAnimation(ACTOR_IVAN, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetDestination(ACTOR_IVAN, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_SetAnimation(ACTOR_MIA, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetDestination(ACTOR_MIA, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_WalkToAndWait(guideId, (guideX - 16), (guideZ - 64));
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Actor_WalkToAndWait(guideId, (guideX - 16), (guideZ - 16));
  Actor_WalkToAndWait(guideId, guideX, guideZ);
  Actor_FaceDirection(guideId, FACING_NORTH, 10);
  Event_End();
}

/// The Scales of Justice's attendant explains stage 1 of Colosseum Final 3.
///
/// The first time, a pointer shows the two ledges and how one rises as the
/// other falls. Then comes the objective, and a place for a friend to cheer.
///
/// With a friend alone on the stages
/// (COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) he asks whether they are
/// done cheering instead.
static void ColosseumNavampaMatch_TalkToScalesAttendant(s32 guideId) {
  struct Actor *westPan;
  struct Actor *eastPan;
  struct Actor *westPanAgain;
  struct Actor *eastPanAgain;
  s32 promptResult;

  if (gMainRoutineWork.entrance ==
      COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(guideId);
    return;
  }
  Event_Start();
  promptResult = Colosso_GreetAtStage(guideId, 1);
  if (promptResult == COLOSSO_GUIDE_DESCRIBE) {
    // Point out the ledges, demonstrate both heights, then restore the stage.
    Event_SetMessageId(MsgColossoStageCalledScales);
    FieldCamera_SetSpeed(3);
    FieldCamera_MoveTo(152, CAMERA_KEEP, 200, 1);
    FieldCamera_WaitForMove();
    Event_Wait(30);
    // "This stage is called the Scales of Justice."
    Event_ShowMessage(guideId, 0);
    Colosso_ShowPointer(104, 68, 0);
    Event_Wait(60);
    Colosso_MovePointer(168, 96, 10);
    Event_Wait(70);
    Event_ShowMessage(guideId, 0);
    Colosso_HidePointer();
    wait(2);
    westPan = Actor_Get(ACTOR_SCALES_WEST_LEDGE);
    westPan->motionFlags = 0;
    ColosseumNavampaMatch_MoveOnScales(westPan, 4);
    eastPan = Actor_Get(ACTOR_SCALES_EAST_LEDGE);
    eastPan->motionFlags = 0;
    ColosseumNavampaMatch_MoveOnScales(eastPan, 32);
    Actor_WaitForMove(eastPan);
    Event_Wait(45);
    westPanAgain = Actor_Get(ACTOR_SCALES_WEST_LEDGE);
    westPanAgain->motionFlags = 0;
    ColosseumNavampaMatch_MoveOnScales(westPanAgain, 32);
    eastPanAgain = Actor_Get(ACTOR_SCALES_EAST_LEDGE);
    eastPanAgain->motionFlags = 0;
    ColosseumNavampaMatch_MoveOnScales(eastPanAgain, 4);
    Actor_WaitForMove(eastPanAgain);
    Event_Wait(15);
    Event_ShowMessage(guideId, 0);
    Colosso_ShowPointer(104, 68, 0);
    Event_Wait(30);
    Colosso_MovePointer(168, 96, 10);
    Event_Wait(40);
    Colosso_MovePointer(104, 68, 10);
    Event_Wait(70);
    Event_ShowMessage(guideId, 0);
    Colosso_HidePointer();
    wait(2);
    FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Colosso_RepeatStageNumber(guideId, 1);
  } else if (promptResult == COLOSSO_GUIDE_OFFER_CHEERING) {
    Event_SetMessageId(MsgColossoObjectiveGetAcross);
    // "Here your objective is to get across the gaps as quickly as..."
    Event_ShowMessage(guideId, 0);
  }
  Colosso_OfferCheeringSpot(promptResult, guideId, 1);
  Event_End();
}

/// The Stepping-Stone Stage's attendant explains stage 2 of Colosseum Final 3.
///
/// The first time, Isaac appears and finds the way. Then comes the objective,
/// and a place for a friend to cheer.
static void ColosseumNavampaMatch_TalkToSteppingStoneAttendant(s32 guideId) {
  s32 promptResult;
  s32 x;

  if (gMainRoutineWork.entrance ==
      COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(guideId);
    return;
  }
  Event_Start();
  promptResult = Colosso_GreetAtStage(guideId, 2);
  if (promptResult == COLOSSO_GUIDE_DESCRIBE) {
    Event_SetMessageId(MsgColossoSteppingStoneStage);
    FieldCamera_SetSpeed(3);
    FieldCamera_MoveTo(376, CAMERA_KEEP, 152, 1);
    x = 280;
    FieldCamera_WaitForMove();
    Event_Wait(30);
    // "This is the Stepping- Stone Stage."
    Event_ShowMessage(guideId, 0);
    Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, x, 200);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, x, 152);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 296, 152);
    Event_Wait(10);
    Field_TryJumpForward();
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 15);
    Field_TryJumpForward();
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 15);
    Field_TryJumpForward();
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 15);
    Event_ShowMessage(guideId, 0);
    Colosso_ShowPointer(96, 40, 0);
    Colosso_MovePointer(128, 40, 10);
    Event_Wait(30);
    Colosso_MovePointer(160, 40, 10);
    Event_Wait(30);
    Colosso_MovePointer(160, 72, 10);
    Event_Wait(30);
    Event_ShowMessage(guideId, 0);
    Colosso_HidePointer();
    Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
    FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Colosso_RepeatStageNumber(guideId, 2);
  } else if (promptResult == COLOSSO_GUIDE_OFFER_CHEERING) {
    Event_SetMessageId(MsgColossoYourGoalInStageSimple);
    // "Your goal in this stage is simple: find the shortest path."
    Event_ShowMessage(guideId, 0);
  }
  Colosso_OfferCheeringSpot(promptResult, guideId, 2);
  Event_End();
}

/// The Wall Wedge's attendant explains stage 3 of Colosseum Final 3.
///
/// The first time, Isaac appears and times his way past the pistons. Then the
/// attendant says that the walls' operator will cheer for him, and offers a
/// place for a friend to cheer.
static void ColosseumNavampaMatch_TalkToWallWedgeAttendant(s32 guideId) {
  struct FieldWork *work;
  s32 promptResult;

  if (gMainRoutineWork.entrance ==
      COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(guideId);
  } else {
    Event_Start();
    promptResult = Colosso_GreetAtStage(guideId, 3);
    if (promptResult == COLOSSO_GUIDE_DESCRIBE) {
      // Show the pattern cycle before demonstrating a timed passage.
      work = gFieldWork;
      Event_SetMessageId(MsgColossoPlaceCalledWall);
      ColosseumNavampaMatch_ResetWallWedge();
      FieldCamera_SetSpeed(3);
      FieldCamera_MoveTo(616, CAMERA_KEEP, 184, 1);
      FieldCamera_WaitForMove();
      Event_Wait(30);
      Event_ShowMessage(guideId, 0);
      ColosseumNavampaMatch_StartWallWedge();
      Event_Wait(60);
      Event_ShowMessage(guideId, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 504, 200);
      COLOSSEUM_NAVAMPA_MATCH_FACE_DIRECTION(ACTOR_PARTY_LEADER, 0, 0);
      ColosseumNavampaMatch_WaitForWallWedgeOpening();
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
      Colosso_WalkActorTo(ACTOR_PARTY_LEADER, 680, 200);
      if (work->raisedTrigger != 5) {
        do {
          wait(1);
          if (++promptResult > 239) {
            break;
          }
        } while (work->raisedTrigger != 5);
      }
      ColosseumNavampaMatch_PassSouthWallOpening();
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                      60);
      Event_ShowMessage(guideId, 0);
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(guideId, 3);
      work->raisedTrigger = 0;
    } else if (promptResult == COLOSSO_GUIDE_OFFER_CHEERING) {
      Event_SetMessageId(MsgColossoOperatorWallsCheer);
      // "The operator of the walls will cheer for you on this stage."
      Event_ShowMessage(guideId, 0);
    }
    Colosso_OfferCheeringSpot(promptResult, guideId, 3);
    Event_End();
  }
}

/// The Free-Climb Area's attendant explains stage 4 of Colosseum Final 3.
///
/// The first time, Isaac appears and climbs across. Then comes the objective,
/// and a place for a friend to cheer.
static void ColosseumNavampaMatch_TalkToFreeClimbAttendant(s32 guideId) {
  s32 result;
  struct Actor *actor;
  s32 x;
  s32 y;
  s32 raisedHeight;
  s32 shiftedX;

  if (gMainRoutineWork.entrance ==
      COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(guideId);
  } else {
    Event_Start();
    result = Colosso_GreetAtStage(guideId, 4);
    if (result == COLOSSO_GUIDE_DESCRIBE) {
      Event_SetMessageId(MsgColossoPlaceNormallyCalledFreeClimb);
      FieldCamera_SetSpeed(3);
      FieldCamera_MoveTo(856, CAMERA_KEEP, 168, 1);
      FieldCamera_WaitForMove();
      Event_Wait(30);
      // "This place is normally called the Free- Climb Area."
      Event_ShowMessage(guideId, 0);
      // "This sheer wall has a bumpy surface."
      Event_ShowMessage(guideId, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 816, 200);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 840, 200);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
      Field_ClimbUp();
      FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
      actor = Actor_Get(ACTOR_PARTY_LEADER);
      y = actor->y.fixed;
      x = actor->x.fixed;
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB);
      raisedHeight = px(6) + y;
      FieldActor_SetMoveTargetFixed(actor, x, raisedHeight, actor->z.fixed);
      Actor_WaitForMove(actor);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB_EAST);
      shiftedX = px(64) + x;
      FieldActor_SetMoveTargetFixed(actor, shiftedX, raisedHeight,
                                    actor->z.fixed);
      Actor_WaitForMove(actor);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB);
      FieldActor_SetMoveTargetFixed(actor, shiftedX, y + px(54),
                                    actor->z.fixed);
      Actor_WaitForMove(actor);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB_WEST);
      FieldActor_SetMoveTargetFixed(actor, x + px(48), y + px(54),
                                    actor->z.fixed);
      Actor_WaitForMove(actor);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLING);
      Event_ShowMessage(guideId, 0);
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(guideId, 4);
    } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
      Event_SetMessageId(MsgColossoClearStageMustAbleChange);
      // "To clear this stage, you must be able to change directions..."
      Event_ShowMessage(guideId, 0);
    }
    Colosso_OfferCheeringSpot(result, guideId, 4);
    Event_End();
  }
}

/// Starts a scripted pixel destination while preserving the actor's
/// height.
static void ColosseumNavampaMatch_PositionActor(s32 selector, s32 x, s32 z) {
  struct Actor *actor;

  actor = Actor_Lookup(selector);
  if (actor != 0) {
    Actor_StartIdle(actor);
    Actor_SelectAnimation(actor, 5);
    Actor_SetMoveTarget(actor, px(x), actor->y.fixed, px(z));
  }
}

/// Moves to a scripted pixel destination, waits, then restores the idle mode.
static void ColosseumNavampaMatch_PositionAndActivateActor(s32 selector, s32 x,
                                                           s32 z) {
  struct Actor *actor;

  actor = Actor_Lookup(selector);
  if (actor != 0) {
    Actor_StartIdle(actor);
    Actor_SelectAnimation(actor, 5);
    Actor_SetMoveTarget(actor, px(x), actor->y.fixed, px(z));
    Actor_WaitForMove(actor);
    Actor_SelectAnimation(actor, 1);
  }
}

/// The Moving Sidewalk Stage's attendant explains stage 5 of Colosseum Final
/// 3.
///
/// The first time, Isaac appears and walks against the belt. Then comes the
/// objective, and a place for a friend to cheer.
static void ColosseumNavampaMatch_TalkToMovingSidewalkAttendant(s32 guideId) {
  s32 i;
  s32 result;
  struct Actor *actor;

  if (gMainRoutineWork.entrance ==
      COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(guideId);
  } else {
    Event_Start();
    result = Colosso_GreetAtStage(guideId, 5);
    if (result == COLOSSO_GUIDE_DESCRIBE) {
      // Walk against the sidewalk, then show how its current pushes the rider.
      Event_SetMessageId(MsgColossoCalledMovingSidewalkStage);
      FieldCamera_SetSpeed(3);
      FieldCamera_MoveTo(1080, CAMERA_KEEP, 168, 1);
      FieldCamera_WaitForMove();
      Event_Wait(30);
      // "This is called the Moving Sidewalk Stage."
      Event_ShowMessage(guideId, 0);
      // "The conveyor belt moves opposite the direction you need to go."
      Event_ShowMessage(guideId, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 984, 184);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
      ColosseumNavampaMatch_PositionAndActivateActor(ACTOR_PARTY_LEADER, 992,
                                                     184);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.3);
      ColosseumNavampaMatch_PositionActor(ACTOR_PARTY_LEADER, 1120, 184);
      Event_Wait(120);
      Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                      ATTACHED_EFFECT_ATTACH);
      Event_Wait(120);
      Actor_ResetMotionFields(ACTOR_PARTY_LEADER);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
#if !TBS_PATCH_3
      Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                      ATTACHED_EFFECT_REMOVE);
#endif
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      actor = Actor_Get(ACTOR_PARTY_LEADER);
      for (i = 119; i >= 0; i--) {
        if (actor->x.fixed > px(992)) {
          actor->x.fixed -= px(6) / 5;
        }
        wait(1);
      }
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                      60);
      ColosseumNavampaMatch_PositionActor(ACTOR_PARTY_LEADER, 1120, 184);
      Event_ShowMessage(guideId, 0);
      Actor_ResetMotionFields(ACTOR_PARTY_LEADER);
      gMainRoutineWork.movementMode = LEADER_MOVEMENT_CLIMB;
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(guideId, 5);
    } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
      Event_SetMessageId(MsgColossoInStageMustTryOutpace);
      // "In this stage, you must try to outpace the conveyor belt."
      Event_ShowMessage(guideId, 0);
    }
    Colosso_OfferCheeringSpot(result, guideId, 5);
    Event_End();
  }
}

/// The log-rolling area's attendant explains stage 6 of Colosseum Final 3.
///
/// The first time, a pointer traces the log's route. Then comes the
/// objective, and a place for a friend to cheer.
static void ColosseumNavampaMatch_TalkToLogRollingAttendant(s32 actorId) {
  s32 state;

  if (gMainRoutineWork.entrance ==
      COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(actorId);
    return;
  }

  Event_Start();
  state = Colosso_GreetAtStage(actorId, 6);

  if (state == COLOSSO_GUIDE_DESCRIBE) {
    Event_SetMessageId(MsgColossoAnotherLogRollingArea);
    FieldCamera_SetSpeed(3);
    FieldCamera_MoveTo(1288, CAMERA_KEEP, 152, 1);
    FieldCamera_WaitForMove();
    Event_Wait(30);
    // "This is another log-rolling area."
    Event_ShowMessage(actorId, 0);
    Colosso_ShowPointer(180, 88, 0);
    Event_Wait(60);
    Event_ShowMessage(actorId, 0);
    Colosso_MovePointer(32, 84, 10);
    Event_Wait(30);
    Event_ShowMessage(actorId, 0);
    Colosso_MovePointer(96, 84, 30);
    Event_Wait(60);
    Event_ShowMessage(actorId, 0);
    Colosso_HidePointer();
    Event_Wait(2);
    FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Colosso_RepeatStageNumber(actorId, 6);
  } else if (state == COLOSSO_GUIDE_OFFER_CHEERING) {
    Event_SetMessageId(MsgColossoHereMustFigureOutHow);
    // "Here you must figure out how the logs and stones need to be..."
    Event_ShowMessage(actorId, 0);
  }

  Colosso_OfferCheeringSpot(state, actorId, 6);
  Event_End();
}

// This scene's exits for colosso_solo.h.
#define COLOSSO_SOLO_TO_RACE COLOSSEUM_NAVAMPA_MATCH_LEND_A_HAND_TO_RACE
#define COLOSSO_SOLO_TO_COLOSSEUM COLOSSEUM_NAVAMPA_MATCH_TO_COLOSSEUM
#include "shared/colosso_solo.h"
#include "shared/item_equip.h"
#include "shared/colosso_task.h"

#include "shared/colosso_lettering.h"

#include "shared/colosso_marker.h"
#include "shared/colosso_item_icon.h"

#include "shared/colosso_hex_string.h"
#include "shared/colosso_finish.h"

#include "shared/colosso_path_opponent.h"
#include "shared/colosso_setup.h"
#include "shared/colosso_map_state.h"
#include "shared/actor_find_at_position.h"

#include "shared/colosso_occupant.h"

/// Closes the arena's step trigger. Nothing in the game sets it.
/// ★ Named from the game: "This is the battle arena. After getting here, the
/// warriors do battle." (MsgColossoWarriorsEnterFinalsWithoutAny)
#define FLAG_COLOSSEUM_ARENA_CLOSED FLAG_SCENE_TEMP_10
/// The chest holding a Smoke Bomb has been opened.
/// ★ Named from the game: "Smoke Bomb" (MsgItemName)
#define FLAG_COLOSSEUM_NAVAMPA_MATCH_SMOKE_BOMB_CHEST_OPENED FLAG_AREA_TEMP_6A
/// The chest holding a Nut has been opened.
/// ★ Named from the game: "Nut" (MsgItemName)
#define FLAG_COLOSSEUM_NAVAMPA_MATCH_NUT_CHEST_OPENED FLAG_AREA_TEMP_6B
/// The chest holding a Sleep Bomb has been opened.
/// ★ Named from the game: "Sleep Bomb" (MsgItemName)
#define FLAG_COLOSSEUM_NAVAMPA_MATCH_SLEEP_BOMB_CHEST_OPENED FLAG_AREA_TEMP_6C

/// The Moving Sidewalk Stage's log where Force has knocked it off the belt,
/// bobbing a pixel up and down.
const s32 gColosseumNavampaMatch_SidewalkLogOffBelt[] = {
    SC_SET(SW_Y, px(1)),         SC_WAIT(10), SC_SET(SW_Y, 0), SC_WAIT(10),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The Wall Wedge's four patterns.
///
/// Each holds each piston's animation, west to east, then the operator's.
const s8 gColosseumNavampaMatch_WallWedgePattern[][6] = {
    {7, 5, 7, 5, 7, 11},
    {4, 6, 4, 6, 4, 10},
    {5, 7, 5, 7, 5, 12},
    {6, 4, 6, 4, 6, 10},
};

// clang-format off
const u16 gColosseumNavampaMatch_FacingAngles[] = {
    0, 0, FACING_SOUTH, FACING_WEST, FACING_WEST, FACING_WEST, FACING_NORTH, 0,
};

const s32 gColosseumNavampaMatch_SidewalkLogScript[] = {
    SC_MOVE(1088, 0, 184),
    SC_SYNC,
    SC_MOVE(1130, 0, 184),
    SC_SYNC,
    SC_CALL(ColosseumNavampaMatch_ShowActivatedSidewalkLayer),
    SC_LABEL(1),
    SC_SET(SW_Y, px(1)),
    SC_WAIT(10),
    SC_SET(SW_Y, 0),
    SC_WAIT(10),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

#include "shared/colosso_lettering_tables.h"

char gHexDigits[20] = "0123456789ABCDEF";

#include "shared/colosso_prize_scripts.h"

u32 gPushedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),
    DIRECTION_STEP(16, 0),
    DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),
    DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

struct SceneEntrance gColosseumNavampaMatchEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { COLOSSEUM_NAVAMPA_MATCH_RACE_FROM_LEND_A_HAND, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH, CONDITION_ALWAYS, 136, 0, 392, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_LOST, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gColosseumNavampaMatchExits[] = {
    SCENE_EXITS(ColosseumNavampaMatch),
    SCENE_EXIT(COLOSSEUM_NAVAMPA_MATCH_LEND_A_HAND_TO_RACE, ColosseumNavampaMatch, COLOSSEUM_NAVAMPA_MATCH_RACE_FROM_LEND_A_HAND),
    SCENE_EXIT(COLOSSEUM_NAVAMPA_MATCH_TO_COLOSSEUM, Colosseum, COLOSSEUM_FROM_COLOSSEUM_NAVAMPA_MATCH),
    SCENE_EXIT(COLOSSEUM_NAVAMPA_MATCH_TO_BABI_PALACE, BabiPalace, BABI_PALACE_FROM_COLOSSEUM_NAVAMPA_MATCH),
    SCENE_EXIT(COLOSSEUM_NAVAMPA_MATCH_TO_BABI_PALACE_LOST, BabiPalace, BABI_PALACE_FROM_COLOSSEUM_NAVAMPA_MATCH),
    SCENE_EXITS_END,
};

struct ActorPlacement gColosseumNavampaMatchActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_PARTY_LEADER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        88, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
        152, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
        216, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        88, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        312, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        712, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1240, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1272, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1304, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, 7,
        536, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, 7,
        568, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, 7,
        600, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, 7,
        632, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, 7,
        664, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, 7,
        536, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, 7,
        568, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, 7,
        600, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, 7,
        632, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PISTON, CONDITION_ALWAYS, 7,
        664, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        720, 0, 216, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        792, 0, 194, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STANDING_LOG, CONDITION_ALWAYS, 7,
        1080, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        1208, 0, 176, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        136, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        152, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        376, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        600, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        856, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1080, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1304, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1512, 0, 176, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1512, 0, 208, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LETTER_Z, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_LETTER_Z, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_LETTER_Z, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    { SCENE_TABLE_END },
};

s32 gColosseumNavampaMatch_WallWedgePatternIndex = 0;

s32 gColosseumNavampaMatch_WallWedgeTimer = 0;

struct Trigger gColosseumNavampaMatchTriggers[] = {
    { STEP_TRIGGER_WALKING, 90, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_JumpAtLogAreaEnd) },
    { STEP_TRIGGER_WALKING, 91, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_PushWeightOrCheckAhead) },
    { TRIGGER_STEP, 11, FLAG_SIDEWALK_LOG_KNOCKED_OFF, EVENT(ColosseumNavampaMatch_RideMovingSidewalk) },
    { STEP_TRIGGER_WALKING, 20, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_PushWeight) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SCALES_WEIGHT, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_SetWeightOnScales) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_MoveStone) },
    { STEP_TRIGGER_WALKING, 22, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_PushRock) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROCK, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_IgnoreMove) },
    { STEP_TRIGGER_WALKING, 23, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_RollLongLog) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LOG_AREA_STONE_A, FLAG_FIRST_LOG_AREA_STONE_SETTLED, EVENT(ColosseumNavampaMatch_SettleStones) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LOG_AREA_STONE_B, FLAG_FIRST_LOG_AREA_STONE_SETTLED + 1, EVENT(ColosseumNavampaMatch_SettleStones) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LOG_AREA_STONE_C, FLAG_FIRST_LOG_AREA_STONE_SETTLED + 2, EVENT(ColosseumNavampaMatch_SettleStones) },
    { TRIGGER_STEP, 15, FLAG_ON_SCALES, EVENT(ColosseumNavampaMatch_StepOnScales) },
    { TRIGGER_STEP, 16, IF_FLAG_SET(FLAG_ON_SCALES), EVENT(ColosseumNavampaMatch_StepOffScales) },
    { TRIGGER_STEP, 50, FLAG_COLOSSEUM_ARENA_CLOSED, EVENT(ColosseumNavampaMatch_ReachArena) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_COLOSSEUM_NAVAMPA_MATCH_SMOKE_BOMB_CHEST_OPENED, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 102, FLAG_COLOSSEUM_NAVAMPA_MATCH_NUT_CHEST_OPENED, FIND_ITEM(ITEM_NUT) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST) | TRIGGER_FLAG_FACING | FACING_WEST, 103, FLAG_COLOSSEUM_NAVAMPA_MATCH_SLEEP_BOMB_CHEST_OPENED, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_ACTS), ACTOR_PUDDLE, FLAG_SCALES_FROZEN, EVENT(ColosseumNavampaMatch_FreezeOnScales) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_GROWTH_PLANT, FLAG_PLANT_GROWN, EVENT(ColosseumNavampaMatch_GrowPlant) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_WALL_OPERATOR, FLAG_WALL_WEDGE_HALTED, EVENT(ColosseumNavampaMatch_HaltWall) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_FORCE, PSYNERGY_PHASE_ACTS), ACTOR_SIDEWALK_LOG, FLAG_SIDEWALK_LOG_KNOCKED_OFF, EVENT(ColosseumNavampaMatch_ForceSidewalkLog) },
    { TRIGGER_RAISED, 4, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_PassNorthWallOpening) },
    { TRIGGER_RAISED, 5, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_PassSouthWallOpening) },
    { TRIGGER_TALK, ACTOR_GUIDE, CONDITION_ALWAYS, EVENT(Colosso_AskReadyToRace) },
    { TRIGGER_TALK, ACTOR_SCALES_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_TalkToScalesAttendant) },
    { TRIGGER_TALK, ACTOR_STEPPING_STONE_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_TalkToSteppingStoneAttendant) },
    { TRIGGER_TALK, ACTOR_WALL_WEDGE_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_TalkToWallWedgeAttendant) },
    { TRIGGER_TALK, ACTOR_FREE_CLIMB_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_TalkToFreeClimbAttendant) },
    { TRIGGER_TALK, ACTOR_MOVING_SIDEWALK_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_TalkToMovingSidewalkAttendant) },
    { TRIGGER_TALK, ACTOR_LOG_ROLLING_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumNavampaMatch_TalkToLogRollingAttendant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgColossoRiverThisYearsFinalsAreIncredibleThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCALES_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogTheStreetVendorsCornOnThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STEPPING_STONE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogICantSeeAnythingWhenIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WALL_WEDGE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogItsNotVeryExcitingWorkBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FREE_CLIMB_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogStandingForLongPeriodsOfTime) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOVING_SIDEWALK_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogIWasUpAllNightTrying) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LOG_ROLLING_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogSomeTouristToldMeILook) },
    { TRIGGER_RAISED, 99, CONDITION_ALWAYS, EVENT(Colosso_TakePlaceForMatch) },
    { TRIGGER_TABLE_END },
};

/// Navampa after the last battle
/// (ColosseumNavampaMatch_PlayPostBattleReunion).
///
/// His sprite grows taller and narrower and back, over and over.
s32 gColosseumNavampaMatch_OpponentAfterBattle[] = {
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(10),
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

s32 gColosseumNavampaMatch_LeaderActionA[] = {
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(6),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(12),
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

s32 gColosseumNavampaMatch_LeaderActionB[] = {
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 64),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 64),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 64),
    SC_WAIT(16),
    SC_ADD(SW_SCALE_Y, -(3 * Q16_ONE / 64)),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -(3 * Q16_ONE / 64)),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -(3 * Q16_ONE / 64)),
    SC_WAIT(32),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

s32 gColosseumNavampaMatch_LeaderActionC[] = {
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 64),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 64),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 64),
    SC_WAIT(26),
    SC_ADD(SW_SCALE_Y, -(3 * Q16_ONE / 64)),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -(3 * Q16_ONE / 64)),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -(3 * Q16_ONE / 64)),
    SC_WAIT(52),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

s32 gColosseumNavampaMatch_LeaderScaleScript[] = {
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_END,
};

s32 gColossoParticleKinds[] = {
    SC_CALL(ColosseumNavampaMatch_AdvanceParticleMotion),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s16 gColosso_LetteringSlot = -1;

#include "shared/colosso_lettering_scripts.h"
// clang-format on

s16 gColosso_MarkerSlot = -1;

#include "shared/colosso_finish_scripts.h"
