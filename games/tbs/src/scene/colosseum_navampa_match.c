/// @file
/// Colosseum Final 3 (SceneId_ColosseumNavampaMatch), the site of the last
/// finals match, where Isaac, having won the first two, races Navampa of
/// Gondowan, the sixth-ranked warrior, through six stages to the arena and
/// fights him there.
///
/// It works as Colosseum Final 1 does (colosseum_azart_match.c): the guide, who
/// did not think Isaac would make it this far, leaves the stages to their
/// attendants, and the stages run through the Scales of Justice, the
/// Stepping-Stone Stage, the Wall Wedge, the Free-Climb Area, the Moving
/// Sidewalk Stage and another log-rolling area.
/// ColosseumNavampaMatch_Initialize restores the stages from their temporary
/// flags (FLAG_PLANT_GROWN to FLAG_LONG_LOG_IN_PLACE, and the stone's column
/// from FLAG_STONE_COLUMN) and sets up the visit its entrance
/// stands for:
///
/// - COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_NORTH, the tour
///   (ColosseumNavampaMatch_IntroduceThirdFinals), from the Colosseum's east
///   passage after the second win; the attendants describe their stages and
///   take the friends to cheer, and the guide sends Isaac back to the passage
///   (COLOSSEUM_NAVAMPA_MATCH_TO_COLOSSEUM).
/// - COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH, `Lend a Hand`: Garet, Ivan
///   and Mia in turn may use Psynergy on the stages. Frost freezes the target
///   on the Scales of Justice's eastern ledge so the scales stay tipped, Halt
///   stops the Wall Wedge's operator and with him its pistons, Force knocks the
///   log off the Moving Sidewalk so the belt no longer carries anyone back,
///   Growth grows a plant into a shortcut, and Move shifts the weight onto the
///   scales and the stones and the rock on the way. After Mia,
///   COLOSSEUM_NAVAMPA_MATCH_LEND_A_HAND_TO_RACE starts the race.
/// - COLOSSEUM_NAVAMPA_MATCH_RACE_FROM_LEND_A_HAND, the race against Navampa,
///   who follows a recorded path, after the lettering `Match 3` and a count of
///   three. At the arena the first to arrive takes the Claymore and the second
///   the Hunter's Sword, and the battle is ENCOUNTER_ZONE_COLOSSO's fifth or
///   sixth group, the Navampa with or without the Claymore.
///
/// Stepping onto the Scales of Justice's western ledge (step trigger 15,
/// ColosseumNavampaMatch_StepOnScales) sends it, with the rider, down towards 4
/// pixels high and the eastern ledge up towards 32, and stepping off (step
/// trigger 16) swings them back; the weight Move sets on the western ledge does
/// the same.
///
/// Win or lose, the third match ends Colosso:
/// COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON and _LOST set FLAG_COLOSSO_OVER, and
/// a win also FLAG_WON_COLOSSO. In the event that follows
/// (ColosseumNavampaMatch_PlayPostBattleReunion), Garet, Ivan and Mia run to
/// Isaac in the arena, but he falls asleep, exhausted, and they leave him to
/// sleep; both exits lead to Babi's Palace, where he wakes long after the
/// finals.

#include "script_operands.h"
#include "version.h"
#include "adventure.h"
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
#include "module/overworld/overworld.h"
#include "module/overworld/event_commands.h"
#include "module/field/field.h"
#include "module/game/game.h"
#include "module/window/window.h"
#include "module/window/screens.h"
#include "module/menu/menu.h"
#include "module/sound/sound.h"
#include "module/runtime/runtime.h"
#include "module/scene/shared/colosso_item_icon.h"
#include "types.h"
#include "event.h"
#include "scene_tables.h"
#include "colosso_log_rolling_stage.h"
#include "io_reg.h"
#include "keystate.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "event_inline.h"
#include "map_grid.h"
#include "flags.h"
#include "text/msg_ids.h"
#include "battle_unit.h"
#include "resource.h"
#include "ability_ids.h"
#include "edition.h"
#include "item_ids.h"
#include "actor_script.h"
#include "actor.h"
#include "heap.h"
#include "scene.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "colosso_gauge.h"
#include "macros.h"
#include "songs.h"
#include "audio_engine_symbols.h"

// UB: each of these calls a void function through an s32 function type, the
// call the bytes show; UBFIX calls it as declared. Each use is tagged.
#ifdef UBFIX
#define COLOSSEUM_NAVAMPA_MATCH_FACE_DIRECTION Actor_ArmCallback
#define COLOSSEUM_NAVAMPA_MATCH_INITIALIZE_MODE_TASK                           \
  ColosseumNavampaMatch_StartLetteringAnimation
#define COLOSSEUM_NAVAMPA_MATCH_APPLY_MATCHING_SLOT_VALUES                     \
  Scene_GiveAndEquipItem
#else
#define COLOSSEUM_NAVAMPA_MATCH_FACE_DIRECTION ((s32 (*)())Actor_ArmCallback)
#define COLOSSEUM_NAVAMPA_MATCH_INITIALIZE_MODE_TASK                           \
  ((s32 (*)())ColosseumNavampaMatch_StartLetteringAnimation)
#define COLOSSEUM_NAVAMPA_MATCH_APPLY_MATCHING_SLOT_VALUES                     \
  ((s32 (*)())Scene_GiveAndEquipItem)
#endif

static void ColosseumNavampaMatch_PlayPostBattleReunion(s32 battleResult);
static void ColosseumNavampaMatch_IntroduceThirdFinals(s32 guideId);
static void ColosseumNavampaMatch_ShowRoundStartLettering(s32 mode);
static void ColosseumNavampaMatch_EmptyHook(void);
static void ColosseumNavampaMatch_WaitForOpponentToFinish(void);
static s32 ColosseumNavampaMatch_CollectFinishEquipment(s32 firstItem,
                                                        s32 secondItem);
static void ColosseumNavampaMatch_PushActorAhead(void);

typedef void (*MapTaskEntry)(void);

/// The third finals match's actors, numbered in the order of
/// gColosseumNavampaMatchActorPlacements from ACTOR_FIRST_PLACED; the
/// rows of Isaac, Garet, Ivan and Mia take their own actors. By the
/// guide's and attendants' explanations, the Scales of Justice, the
/// Stepping-Stone Stage, the Wall Wedge, the Free-Climb Area, the Moving
/// Sidewalk Stage and another log-rolling area lead to the arena.
/// ACTOR_SCALES_WEIGHT to ACTOR_LOG_AREA_STONE_C are all drawn as the rocks
/// of Colosseum Final 1's Rock Challenge; the attendant of this match's
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
  /// East of the Wall Wedge's pistons, by the operator; Move on it needs
  /// nothing more (ColosseumNavampaMatch_IgnoreMove).
  /// ★ Named from what the player sees: a stone pillar east of the Wall
  /// Wedge's pistons (its render)
  ACTOR_ROCK = 14,
  /// The log-rolling area's three stones; each settles where Move drops it
  /// (ColosseumNavampaMatch_SettleStones, FLAG_FIRST_LOG_AREA_STONE_SETTLED and
  /// the two
  /// after it).
  /// ★ Named from the game: "You can get through quickly by moving the logs
  /// and stones." (MsgColossoAnotherLogRollingArea)
  ACTOR_LOG_AREA_STONE_A = 15,
  /// ★ Named from the game: "You can get through quickly by moving the logs
  /// and stones." (MsgColossoAnotherLogRollingArea)
  ACTOR_LOG_AREA_STONE_B = 16,
  /// ★ Named from the game: "You can get through quickly by moving the logs
  /// and stones." (MsgColossoAnotherLogRollingArea)
  ACTOR_LOG_AREA_STONE_C = 17,
  /// The Wall Wedge's five pistons, west to east; each is drawn by two
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
/// the race (ColosseumNavampaMatch_Initialize restores them), and the scales'
/// state.
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
  /// By Force on ACTOR_SIDEWALK_LOG: the belt carries no one any more.
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
  /// (ColosseumNavampaMatch_CollectFinishEquipment).
  /// ★ Named from the game: "You must get through several stages before you
  /// reach the arena." (MsgColossoWarriorsEnterFinalsWithoutAny)
  FLAG_ARENA_REACHED = FLAG_SCENE_TEMP_11,
  /// ACTOR_LOG_AREA_STONE_A has settled; the next two flags are B's and C's.
  /// ★ Named from the game: "You can get through quickly by moving the logs and
  /// stones." (MsgColossoAnotherLogRollingArea)
  FLAG_FIRST_LOG_AREA_STONE_SETTLED = FLAG_SCENE_TEMP_14
};

enum { PISTON_COUNT = 5 };

/// Sets the leader speed for the shared solo-party placement routine.
static inline void InitializeActorZero(void) {
  Actor_SetSpeedParameters(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
}

/// Sets a companion speed for the shared solo-party placement routine.
static inline void InitializeSelectedActor(s32 actorId) {
  Actor_SetSpeedParameters(actorId, PIXELS(1), PIXELS(1) / 2);
}

extern s32 gColossoParticleCount;

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

/// The Wall Wedge's task: cycles its pistons through their four patterns,
/// and raises the opening event (4 or 5) when the rider stands in an open
/// gap.
static void ColosseumNavampaMatch_UpdateWallWedge(void) {
  struct FieldWork *work = gFieldWork;
  struct Actor *actor = Actor_GetById(gAdventure.leaderActorId);
  s32 row = actor->z.fixed >> 20;
  s32 i;
  s32 kind;

  if (gColosseumNavampaMatch_WallWedgeTimer == 0) {
    gColosseumNavampaMatch_WallWedgePatternIndex =
        (gColosseumNavampaMatch_WallWedgePatternIndex + 1) & 3;
    for (i = ACTOR_FIRST_PISTON; i <= ACTOR_LAST_PISTON; i++) {
      kind = gColosseumNavampaMatch_WallWedgePattern
          [gColosseumNavampaMatch_WallWedgePatternIndex]
          [i - ACTOR_FIRST_PISTON];
      Import_ActorSetAnimation(i, kind);
      Import_ActorSetAnimation(i + PISTON_COUNT, kind + 8);
      MapGrid_CopyCellAttributeRect(32, 11, 1, 2,
                                    (i - ACTOR_FIRST_PISTON) * 2 + 33, 11);
      if (kind != 7) {
        MapGrid_CopyCellAttributeRect(74, 12, 1, 1,
                                      (i - ACTOR_FIRST_PISTON) * 2 + 33, 11);
      }
    }
    Import_ActorSetAnimation(
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

  WaitFrames(10);

  polls = 0;
  while (gColosseumNavampaMatch_WallWedgePatternIndex != 3 ||
         gColosseumNavampaMatch_WallWedgeTimer != 1) {
    WaitFrames(1);
    polls++;
    if (polls > 119) {
      return;
    }
  }
}

/// The Moving Sidewalk Stage's belt carries the party west. It's step trigger
/// 11, and until FLAG_SIDEWALK_LOG_KNOCKED_OFF is set the belt carries the
/// controlled actor and the camera west.
static void ColosseumNavampaMatch_RideMovingSidewalk(void) {
  struct FieldWork *work = gFieldWork;
  struct Adventure *state = &gAdventure;
  s32 id = state->leaderActorId;
  struct Actor *subject = work->camera;
  struct Actor *actor = Actor_GetById(id);
  s32 z = actor->z.part.pixel;

  if ((u32)(z - 183) <= 3) {
    subject->x.fixed += -(PIXELS(4) / 5);
    actor->x.fixed += -(PIXELS(4) / 5);
  }
}

/// Growth on the plant on Colosseum Final 3's stages: it grows and opens a
/// shortcut (FLAG_PLANT_GROWN).
static void ColosseumNavampaMatch_GrowPlant(void) {
  GameFlag_Set(FLAG_PLANT_GROWN);
  {

    MapGrid_CopyCellAttributes(47, 61, 1, 4, 49, 61);
  }
}

/// Moves a scale ledge vertically at the shared speed and acceleration.
static inline void ColosseumNavampaMatch_MoveOnScales(struct Actor *ledge,
                                                      s32 height) {
  ledge->acceleration = PIXELS(2) / 5;
  ledge->speedLimit = PIXELS(4) / 5;
  FieldActor_SetMoveTarget(ledge, ledge->x.fixed, height, ledge->z.fixed);
}

/// Frost on the target on the Scales of Justice's eastern ledge: it freezes and
/// the scales tip, the western ledge down and the eastern ledge up
/// (FLAG_SCALES_FROZEN).
static void ColosseumNavampaMatch_FreezeOnScales(void) {
  struct Actor *actor;

  actor = Actor_GetById(ACTOR_PUDDLE);
  actor->scaleX = Q16_ONE;
  actor->scaleY = Q16_ONE;

  actor = Actor_GetById(ACTOR_SCALES_EAST_LEDGE);
  ColosseumNavampaMatch_MoveOnScales(actor, PIXELS(32));

  actor = Actor_GetById(ACTOR_SCALES_WEST_LEDGE);
  ColosseumNavampaMatch_MoveOnScales(actor, PIXELS(4));

  GameFlag_Set(FLAG_SCALES_FROZEN);
  MapGrid_CopyCellAttributes(15, 12, 1, 1, 13, 12);
  MapGrid_CopyCellAttributes(14, 12, 1, 1, 9, 12);
}

/// Stepping onto the Scales of Justice's western ledge, unless the scales are
/// frozen: it sinks with the rider as the eastern ledge rises
/// (FLAG_ON_SCALES).
static void ColosseumNavampaMatch_StepOnScales(void) {
  s32 actorId;
  s32 lowered;
  struct Actor *log;
  struct Actor *rider;
  u8 *flags;

  actorId = gAdventure.leaderActorId;
  lowered = GameFlag_Get(FLAG_SCALES_FROZEN);
  if (lowered == 0) {
    log = Actor_GetById(ACTOR_SCALES_WEST_LEDGE);
    if (log != NULL) {
      Actor_SetDestination(actorId, log->x.part.pixel, log->z.part.pixel);
    }
    Actor_WaitForMoveById(actorId);
    log = Actor_GetById(ACTOR_SCALES_EAST_LEDGE);
    log->motionFlags = lowered;
    ColosseumNavampaMatch_MoveOnScales(log, PIXELS(32));
    log = Actor_GetById(ACTOR_SCALES_WEST_LEDGE);
    log->motionFlags = lowered;
    ColosseumNavampaMatch_MoveOnScales(log, PIXELS(4));
    rider = Actor_GetById(actorId);
    flags = &rider->motionFlags;
    *flags = lowered;
    ColosseumNavampaMatch_MoveOnScales(rider, PIXELS(4));
    Actor_SetSpriteFlags(rider, SPRITE_FLAG_SHADOW);
    Actor_WaitForMoveById(actorId);
    MapGrid_CopyCellAttributes(0, 24, 1, 1, 9, 12);
    WaitFrames(2);
    Actor_SetSpriteFlags(rider, SPRITE_FLAG_SHADOW);
    *flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    rider->groundHeight = rider->y.fixed;
    GameFlag_Set(FLAG_ON_SCALES);
  }
}

/// Stepping off the Scales of Justice's western ledge: the scales swing back.
static void ColosseumNavampaMatch_StepOffScales(void) {
  struct Actor *actor;

  Import_ActorSetAnimation(gAdventure.leaderActorId, 1);

  actor = Actor_GetById(ACTOR_SCALES_EAST_LEDGE);
  actor->motionFlags = 0;
  ColosseumNavampaMatch_MoveOnScales(actor, PIXELS(4));

  actor = Actor_GetById(ACTOR_SCALES_WEST_LEDGE);
  actor->motionFlags = 0;
  ColosseumNavampaMatch_MoveOnScales(actor, PIXELS(32));

  Actor_WaitForMoveById(ACTOR_SCALES_WEST_LEDGE);
  {
    MapGrid_CopyCellAttributes(0, 25, 1, 1, 9, 12);
  }
  WaitFrames(2);
  GameFlag_Clear(FLAG_ON_SCALES);
}

/// Move on the weight by the Scales of Justice: when it reaches the western
/// ledge, it sinks the ledge and the eastern ledge rises
/// (FLAG_WEIGHT_ON_SCALES).
static void ColosseumNavampaMatch_SetWeightOnScales(void) {
  struct Actor *probe;
  struct Actor *log;
  struct Actor *upper;
  struct Actor *lower;
  s32 x;
  s32 z;

  probe = Actor_GetById(ACTOR_SCALES_WEIGHT);
  x = probe->x.fixed >> 20;
  if (x == 9) {
    z = probe->z.fixed >> 20;
    if (z == 12) {
      log = Actor_GetById(ACTOR_SCALES_WEIGHT);
      Actor_SetSpriteFlags(log, 0);
      log->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      log->motionFlags = 0;
      ColosseumNavampaMatch_MoveOnScales(log, PIXELS(4));
      upper = Actor_GetById(ACTOR_SCALES_EAST_LEDGE);
      upper->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      ColosseumNavampaMatch_MoveOnScales(upper, PIXELS(32));
      lower = Actor_GetById(ACTOR_SCALES_WEST_LEDGE);
      ColosseumNavampaMatch_MoveOnScales(lower, PIXELS(4));
      GameFlag_Set(FLAG_WEIGHT_ON_SCALES);
      MapGrid_CopyCellAttributes(15, 12, 1, 1, 13, z);
      MapGrid_CopyCellAttributes(1, 25, 1, 1, x, z);
    }
  }
}

/// Walking into the weight by the Scales of Justice pushes it, perhaps onto the
/// scales.
static void ColosseumNavampaMatch_PushWeight(void) {
  ColosseumNavampaMatch_PushActorAhead();
  ColosseumNavampaMatch_SetWeightOnScales();
}

/// Move on the stone on Colosseum Final 3's stages, and after each push: its
/// column is kept (the byte of flags from FLAG_STONE_COLUMN) and its cell
/// rebuilt.
static void ColosseumNavampaMatch_MoveStone(void) {
  struct Actor *actor;
  s32 x;

  actor = Actor_GetById(ACTOR_STONE);
  x = actor->x.fixed >> 20;
  FieldGameFlag_SetByte(FLAG_STONE_COLUMN, x);
  MapGrid_CopyCellAttributes(18, 10, 3, 1, 18, 11);
  MapGrid_CopyCellAttributes(17, 11, 1, 1, x, 11);
}

/// Move on ACTOR_ROCK in Colosseum Final 3 needs nothing more.
static void ColosseumNavampaMatch_IgnoreMove(void) {}

/// Walking into a rock on Colosseum Final 3's stages (step trigger 22) pushes
/// it.
static void ColosseumNavampaMatch_PushRock(void) {
  ColosseumNavampaMatch_PushActorAhead();
}

/// Move on one of the log-rolling area's three stones, and each push there: a
/// stone resting on the floor becomes a step (FLAG_FIRST_LOG_AREA_STONE_SETTLED
/// and the two after it).
static void ColosseumNavampaMatch_SettleStones(void) {
  struct Actor *actor;
  s32 slot;
  s32 x;
  s32 z;
  s32 x2;
  s32 z2;

  for (slot = ACTOR_LOG_AREA_STONE_A; slot <= ACTOR_LOG_AREA_STONE_C; slot++) {
    actor = Actor_GetById(slot);
    if (MapGrid_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed) == 0) {
      actor->priorityFlags = 2;
      actor->motionFlags = 0;
      x = actor->x.fixed >> 20;
      z = actor->z.fixed >> 20;
      MapGrid_CopyCellAttributes(83, 13, 1, 1, x, z);
      x2 = actor->x.fixed >> 20;
      z2 = actor->z.fixed >> 20;
      MapGrid_CopyCellAttributes(83, 13, 1, 1, x2, z2 + 52);
      GameFlag_Set(slot - ACTOR_LOG_AREA_STONE_A +
                   FLAG_FIRST_LOG_AREA_STONE_SETTLED);
    }
  }
}

/// A task for Colosseum Final 3's eastern end: the cell at column 81, row
/// 12 lets the rider through only while facing south.
static void ColosseumNavampaMatch_UpdateEastEndCollision(void) {
  struct Actor *actor;
  s32 x;
  s32 z;
  s32 behavior;

  actor = Actor_GetById(gAdventure.leaderActorId);
  x = actor->x.fixed >> 20;
  behavior = 23;
  z = actor->z.fixed >> 20;
  if (x == 81 && z == 12) {
    if ((actor->facing & 0xE000) == 0x4000) {
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
  actor = Actor_GetById(ACTOR_LOG_AREA_STONE_A);
  x >>= 20;
  z >>= 20;
  if (actor->x.fixed >> 20 == x && actor->z.fixed >> 20 == z) {
    return -1;
  }
  actor = Actor_GetById(ACTOR_LOG_AREA_STONE_B);
  if (actor->x.fixed >> 20 == x && actor->z.fixed >> 20 == z) {
    return -1;
  }
  actor = Actor_GetById(ACTOR_LOG_AREA_STONE_C);
  if (actor->x.fixed >> 20 == x && actor->z.fixed >> 20 == z) {
    return -1;
  }
  return 0;
}

/// Checks all four cells covered by the long rolling log.
static s32 ColosseumNavampaMatch_CheckLongLogPath(s32 x, s32 z) {
  if (ColosseumNavampaMatch_CheckLongLogCell(x, z - PIXELS(24)) != 0 ||
      ColosseumNavampaMatch_CheckLongLogCell(x, z - PIXELS(8)) != 0 ||
      ColosseumNavampaMatch_CheckLongLogCell(x, z + PIXELS(8)) != 0 ||
      ColosseumNavampaMatch_CheckLongLogCell(x, z + PIXELS(24)) != 0) {
    return -1;
  }
  return 0;
}

/// The long log carries the leader until something stops it. Walking into the
/// log-rolling area's long log (step trigger 23) rolls it, with the leader, the
/// way the pad is held; at the east end it drops into place and the path is
/// made (FLAG_LONG_LOG_IN_PLACE). Away from the log, a stone ahead is pushed
/// instead.
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

  rider = Actor_GetById(gAdventure.leaderActorId);
  log = Actor_GetById(ACTOR_LONG_LOG);
  steps = 0;
  angle = gColosseumNavampaMatch_FacingAngles[rider->facing >> 13];
  actorId = gAdventure.leaderActorId;
  SET_VECTOR(ahead, (rider->x.fixed & -0x100000) + PIXELS(8), rider->y.fixed,
             (rider->z.fixed & -0x100000) + PIXELS(8));
  Vector_AddPolarOffset(PIXELS(16), angle, ahead);
  x = log->x.fixed;
  z = log->z.fixed;
  // Only a nearby rider rolls the log; other interactions push the actor
  // ahead.
  if ((ahead[0] - x >= 0 ? ahead[0] - x : x - ahead[0]) > PIXELS(8) ||
      (ahead[2] - z >= 0 ? ahead[2] - z : z - ahead[2]) > PIXELS(32)) {
    goto far;
  }
  if (gKeysHeld & KEY_LEFT) {
    mode = 2;
    step = -8;
    for (;;) {
      nextX = x - PIXELS(16);
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
      nextX = x + PIXELS(16);
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
  MapGrid_CopyCellAttributes(74, 8, 1, 4, log->x.fixed >> 20, 9);
  MapGrid_CopyCellAttributes(120, 60, 8, 5, 74, 60);
  Event_Start();
  Import_ActorSetAnimation(actorId, 8);
  Event_WaitFrames(6);
  log->speedLimit = PIXELS(1) / 2;
  acceleration = PIXELS(1) / 5;
  log->acceleration = acceleration;
  Actor_SelectAnimation(log, mode);
  FieldActor_SetMoveTarget(log, x, 0, z);
  Event_WaitFrames(6);
  Import_ActorSetAnimation(actorId, ANIM_WALK);
  work = HeapSlot_AllocEwram(HEAP_SLOT_FIELD, sizeof(struct FieldWork));
  Actor_StartCamera(work->camera, (s32)log);
  Actor_SetSpeed(actorId, PIXELS(1) / 2, acceleration);
  Event_PlaySong(SE_HEAVY_MOVE);
  Actor_SelectAnimation(rider, 2);
  FieldActor_SetMoveTarget(rider, (steps * step * PIXELS(1)) + rider->x.fixed,
                           0, rider->z.fixed);
  Actor_WaitForMove(rider);
  Actor_SelectAnimation(rider, 1);
  Actor_WaitForMove(log);
  // At the eastern end the log settles at the end of its stage.
  if (x >= PIXELS(1328)) {
    GameFlag_Set(FLAG_LONG_LOG_IN_PLACE);
    Import_ActorSetAnimation(ACTOR_LONG_LOG, ANIM_NOD);
    Actor_SetDestinationOffset(ACTOR_LONG_LOG, 18, 6);
    Event_WaitFrames(30);
    Actor_SelectAnimation(log, 8);
    Actor_WaitForMove(log);
    log->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    column = 84;
    MapGrid_CopyCellAttributes(86, 10, 1, 2, column, 10);
    MapGrid_CopyCellAttributes(86, 9, 1, 1, column, 12);
    Event_PlaySong(SE_STOP_ACTION_SE);
    Event_PlaySong(SE_LOG_FALL);
  } else {
    Actor_SelectAnimation(log, 1);
    Event_PlaySong(SE_STOP_ACTION_SE);
    Event_PlaySong(SE_HEAVY_STOP);
    column = x >> 20;
    MapGrid_CopyCellAttributes(85, 9, 1, 4, column, 9);
    MapGrid_CopyCellAttributes(85, 9, 1, 4, column, 61);
  }
  Event_WaitFrames(15);
  Event_End();
  return;
far:
  ColosseumNavampaMatch_PushActorAhead();
  ColosseumNavampaMatch_SettleStones();
}

/// Displays the activated moving sidewalk and plays its arrival sounds.
static s32
ColosseumNavampaMatch_ShowActivatedSidewalkLayer(struct Actor *interpreter) {
  TilesetAnim_Pause(1);
  TilesetAnim_Resume(2);
  Sound_PlaySong(SE_STOP_ACTION_SE);
  Sound_PlaySong(SE_FLOOR_SWITCH);
  return 0;
}

/// Force on the Moving Sidewalk Stage's log: it slides away and the belt
/// carries no one any more (FLAG_SIDEWALK_LOG_KNOCKED_OFF).
static void ColosseumNavampaMatch_ForceSidewalkLog(void) {
  struct Actor *effect;
  s32 moveRate;

  effect = Actor_GetById(ACTOR_SIDEWALK_LOG);
  effect->motionFlags = 0;
  moveRate = PIXELS(8) / 5;
  effect->acceleration = moveRate;
  effect->speedLimit = moveRate;
  Actor_SelectAnimation(effect, 2);
  Actor_StartScript(effect, gColosseumNavampaMatch_SidewalkLogScript);
  GameFlag_Set(FLAG_SIDEWALK_LOG_KNOCKED_OFF);
}

/// Halt on the Wall Wedge's operator: the pistons stop at their next open
/// pattern (FLAG_WALL_WEDGE_HALTED).
static void ColosseumNavampaMatch_HaltWall(void) {
  FieldPsynergy_HaltActor(ACTOR_WALL_OPERATOR);
  GameFlag_Set(FLAG_WALL_WEDGE_HALTED);
  WaitFrames(10);
  while (gColosseumNavampaMatch_WallWedgePatternIndex != 1 &&
         gColosseumNavampaMatch_WallWedgePatternIndex != 3)
    WaitFrames(1);
  WaitFrames(1);
  Task_Remove(ColosseumNavampaMatch_UpdateWallWedge);
}

/// Raised by the Wall Wedge (4): the rider passes through the pistons'
/// northern opening.
static void ColosseumNavampaMatch_PassNorthWallOpening(void) {
  struct Actor *actor;
#ifndef UBFIX
  s16 *gameWords;
#endif
  s32 *actorId;
  s32 z;

#ifdef UBFIX
  actorId = &gAdventure.leaderActorId;
#else
  // The original word-indexed address takes three more instructions here.
  gameWords = (s16 *)&gAdventure;
  actorId = (s32 *)&gameWords[offsetof(struct Adventure, leaderActorId) /
                              sizeof(s16)];
#endif
  actor = Actor_GetById(*actorId);
  actor->acceleration = PIXELS(1);
  actor->speedLimit = PIXELS(2);
  FieldEffect_SetAttached(*actorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SelectAnimation(actor, 5);
  z = actor->z.fixed & 0xFFF00000;
  FieldActor_SetMoveTarget(actor, actor->x.fixed, actor->y.fixed,
                           z + PIXELS(24));
  Actor_WaitForMove(actor);
}

/// Raised by the Wall Wedge (5): the rider passes through the pistons'
/// southern opening.
static void ColosseumNavampaMatch_PassSouthWallOpening(void) {
  struct Actor *actor;
#ifndef UBFIX
  s16 *gameWords;
#endif
  s32 *actorId;
  s32 z;

#ifdef UBFIX
  actorId = &gAdventure.leaderActorId;
#else
  // The original word-indexed address takes three more instructions here.
  gameWords = (s16 *)&gAdventure;
  actorId = (s32 *)&gameWords[offsetof(struct Adventure, leaderActorId) /
                              sizeof(s16)];
#endif
  actor = Actor_GetById(*actorId);
  if (actor->x.fixed > PIXELS(664)) {
    actor->x.fixed = PIXELS(664);
  }
  actor->acceleration = PIXELS(1);
  actor->speedLimit = PIXELS(2);
  Actor_SelectAnimation(actor, 5);
  z = actor->z.fixed & 0xFFF00000;
  FieldActor_SetMoveTarget(actor, actor->x.fixed, actor->y.fixed,
                           z + PIXELS(12));
  Actor_WaitForMove(actor);
  FieldEffect_SetAttached(*actorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_JumpDownLedge(*actorId, 6, 0);
}

UNUSED static void ColosseumNavampaMatch_UnusedEventHook(void) {}

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
UNUSED static void ColosseumNavampaMatch_UnusedWarriorTalk(s32 warriorId) {
  struct FieldWork *work;
  s32 actorId;
  s32 messageOffset;
  s32 entrance;

  work = gFieldWork;
  actorId = gAdventure.leaderActorId;
  entrance = gAdventure.entrance;
  if (entrance == COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Event_Start();
    messageOffset = warriorId * 3;
    Event_SetMessageId(messageOffset + MsgColossoDidntThinkBattles);
    FieldMessage_ShowAtActor(warriorId, 0);
    if (Event_AskYesNoWithActorAnimation(actorId, 0) == 0) {
      Event_SetMessageId(messageOffset + MsgColossoDidntThinkBattles + 1);
      Event_ShowMessageAndWait(warriorId, 0);
      work->transition =
          SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
      work->transitionFrames = 15;
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
      Colosso_ControlCharacterAlone(warriorId);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
    } else {
      Event_SetMessageId(messageOffset + MsgColossoDidntThinkBattles + 2);
      Event_ShowMessageAndWait(warriorId, 0);
    }
    Event_End();
  }
}

/// Reaching Colosseum Final 3's arena: the leader takes a prize (the Claymore
/// if first, the Hunter's Sword if second), and he and Navampa face off and
/// fight; the battle's group follows who took the Claymore. The arena's
/// step trigger row waits on FLAG_COLOSSEUM_ARENA_CLOSED being clear, which
/// nothing in the game sets (colosseum_azart_match.c's
/// ColosseumNavampaMatch_ReachArena).
static void ColosseumNavampaMatch_ReachArena(void) {
  s32 place;
  s32 i;
  s32 last;

  last = 4;
  ColosseumNavampaMatch_EmptyHook();
  Event_Start();
  place = ColosseumNavampaMatch_CollectFinishEquipment(ITEM_CLAYMORE,
                                                       ITEM_HUNTERS_SWORD);
  ColosseumNavampaMatch_WaitForOpponentToFinish();
  for (i = 0; i < 10; i++) {
    Actor_RefreshSelectorById(ACTOR_OPPONENT);
  }
  Actor_SetSpeed(ACTOR_OPPONENT, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(ACTOR_OPPONENT, 1528, 192);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 1496, 192);
  Import_ActorSetAnimation(ACTOR_OPPONENT, ANIM_STAND);
  Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_OPPONENT, 0);
  Event_WaitFrames(10);
  Import_ActorSetAnimation(ACTOR_OPPONENT, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1));
  Actor_SetSpeed(ACTOR_OPPONENT, PIXELS(2), PIXELS(1));
  Actor_WalkTo(ACTOR_PARTY_LEADER, 1504, 192);
  Actor_WalkToAndWait(ACTOR_OPPONENT, 1520, 192);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
  Import_ActorSetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_FIGHTING_STANCE);
  Event_WaitFrames(10);
  /* FAKEMATCH(temporary): last, set to 4 at the top, keeps the + 1 unfolded;
   * as 4 - place + 1, 1 of 106 instructions differ in tbs-en. */
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_COLOSSO,
                                        last - place + 1);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination((s32)&SceneId_ColosseumNavampaMatch,
                                  COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON);
  Event_SetDefeatDestination((s32)&SceneId_ColosseumNavampaMatch,
                             COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_LOST);
  GameFlag_Set(FLAG_KEEP_MUSIC_ONCE);
}

/// Walking into something on Colosseum Final 3's stages (step trigger 90):
/// unless it is an actor, the leader jumps forward where he can
/// (Field_TryJumpForward).
static void ColosseumNavampaMatch_JumpAtLogAreaEnd(void) {
  if (Colosso_FindActorAhead() == 0) {
    Field_TryJumpForward();
  }
}

/// Walking into something by the Scales of Justice (step trigger 91): the
/// weight is pushed, anything else is checked.
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

/// Sets up Colosseum Final 3: restores obstacle progress, then by the arrival:
/// the race (COLOSSEUM_NAVAMPA_MATCH_RACE_FROM_LEND_A_HAND), the friends' turns
/// to lend a hand (COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH), the guide's
/// tour (COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_NORTH), or the end of Colosso
/// after a win (COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON) or a loss
/// (COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_LOST).
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

  Actor_SetSpriteFlags(Actor_GetById(ACTOR_PUDDLE), 0);

  actor = Actor_GetById(ACTOR_SCALES_WEST_LEDGE);
  Actor_SetSpriteFlags(actor, 0);
  actor->motionFlags = 0;
  actor->y.fixed = PIXELS(32);

  actor = Actor_GetById(ACTOR_SCALES_EAST_LEDGE);
  Actor_SetSpriteFlags(actor, 0);
  actor->motionFlags = 0;
  actor->y.fixed = PIXELS(4);

  if (GameFlag_Get(FLAG_SCALES_FROZEN) != 0) {
    Import_ActorSetAnimation(ACTOR_PUDDLE, 5);
    actor = Actor_GetById(ACTOR_SCALES_WEST_LEDGE);
    actor->y.fixed = PIXELS(4);
    actor = Actor_GetById(ACTOR_SCALES_EAST_LEDGE);
    actor->y.fixed = PIXELS(32);
    MapGrid_CopyCellAttributes(15, 12, 1, 1, 13, 12);
  } else {
    actor = Actor_GetById(ACTOR_PUDDLE);
    actor->scaleX = 3 * Q16_ONE / 2;
    actor->scaleY = 3 * Q16_ONE / 2;
    if (GameFlag_Get(FLAG_ON_SCALES) != 0) {
      MapGrid_CopyCellAttributes(0, 24, 1, 1, 9, 12);
    } else {
      MapGrid_CopyCellAttributes(0, 25, 1, 1, 9, 12);
    }
  }

  if (GameFlag_Get(FLAG_WEIGHT_ON_SCALES) != 0) {
    MapGrid_CopyCellAttributes(15, 12, 1, 1, 13, 12);
    MapGrid_CopyCellAttributes(1, 25, 1, 1, 9, 12);
    actor = Actor_GetById(ACTOR_SCALES_WEIGHT);
    Actor_SetSpriteFlags(actor, 0);
    actor->motionFlags = 0;
    actor->y.fixed = PIXELS(2);
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    actor = Actor_GetById(ACTOR_SCALES_WEST_LEDGE);
    actor->y.fixed = PIXELS(4);
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    actor = Actor_GetById(ACTOR_SCALES_EAST_LEDGE);
    actor->y.fixed = PIXELS(32);
  }

  column = FieldGameFlag_GetByte(FLAG_STONE_COLUMN);
  if (column == 0) {
    column = 19;
  }
  actor = Actor_GetById(ACTOR_STONE);
  actor->x.fixed = (column << 20) + (PIXELS(8));
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  MapGrid_CopyCellAttributes(18, 10, 3, 1, 18, 11);
  MapGrid_CopyCellAttributes(17, 11, 1, 1, column, 11);

  for (i = ACTOR_LOG_AREA_STONE_A; i <= ACTOR_LOG_AREA_STONE_C; i++) {
    actor = Actor_GetById(i);
    height = MapGrid_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
    if (actor->y.fixed == 0 && height == 0) {
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      actor->motionFlags = height;
      MapGrid_CopyCellAttributes(83, 13, 1, 1, actor->x.fixed >> 20,
                                 actor->z.fixed >> 20);
      MapGrid_CopyCellAttributes(83, 13, 1, 1, actor->x.fixed >> 20,
                                 (actor->z.fixed >> 20) + 52);
    }
  }

  // Halt preserves a stationary wall; otherwise restart its pattern cycle.
  if (GameFlag_Get(FLAG_WALL_WEDGE_HALTED) != 0) {
    for (i = ACTOR_FIRST_PISTON; i <= ACTOR_LAST_PISTON; i++) {
      actor = Actor_GetById(i);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      Actor_SelectAnimation(actor, 2);
      actor = Actor_GetById(i + PISTON_COUNT);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      actor->motionFlags = 0;
      actor->y.fixed = PIXELS(32);
      Actor_SelectAnimation(actor, 10);
      MapGrid_CopyCellAttributes(74, 12, 1, 1, i * 2 - 3, 11);
    }
    Import_ActorSetAnimation(ACTOR_WALL_OPERATOR, 10);
    FieldPsynergy_HaltActor(ACTOR_WALL_OPERATOR);
  } else {
    for (i = ACTOR_FIRST_PISTON; i <= ACTOR_LAST_PISTON; i++) {
      actor = Actor_GetById(i);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      actor = Actor_GetById(i + PISTON_COUNT);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      actor->motionFlags = 0;
      actor->y.fixed = PIXELS(32);
    }
    Task_AddFrame(ColosseumNavampaMatch_UpdateWallWedge, TASK_PASS_FRAME);
  }

  if (GameFlag_Get(FLAG_PLANT_GROWN) != 0) {
    Import_ActorSetAnimation(ACTOR_GROWTH_PLANT, ANIM_SHAKE_HEAD);
    MapGrid_CopyCellAttributes(47, 61, 1, 4, 49, 61);
  }

  if (GameFlag_Get(FLAG_SIDEWALK_LOG_KNOCKED_OFF) != 0) {
    TilesetAnim_Pause(1);
    actor = Actor_GetById(ACTOR_SIDEWALK_LOG);
    actor->motionFlags = 0;
    actor->x.fixed = PIXELS(1130);
    actor->z.fixed = PIXELS(184);
    Actor_SetSpriteFlags(actor, 0);
    Actor_SelectAnimation(actor, 3);
    Actor_StartScript(actor, gColosseumNavampaMatch_SidewalkLogOffBelt);
  } else {
    TilesetAnim_Pause(2);
  }

  if (GameFlag_Get(FLAG_LONG_LOG_IN_PLACE) != 0) {
    actor = Actor_GetById(ACTOR_LONG_LOG);
    Actor_SelectAnimation(actor, 8);
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(86, 10, 1, 2, 84, 10);
    MapGrid_CopyCellAttributes(86, 9, 1, 1, 84, 12);
  } else {
    actor = Actor_GetById(ACTOR_LONG_LOG);
    MapGrid_CopyCellAttributes(85, 9, 1, 4, actor->x.fixed >> 20, 9);
    MapGrid_CopyCellAttributes(85, 9, 1, 4, actor->x.fixed >> 20, 61);
  }

  actor = Actor_GetById(ACTOR_PUDDLE);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  actor = Actor_GetById(ACTOR_SCALES_WEST_LEDGE);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  actor = Actor_GetById(ACTOR_SCALES_EAST_LEDGE);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  Import_ActorSetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_FIGHTING_STANCE);
  gAdventure.movementMode = LEADER_MOVEMENT_WALK;
  Colosso_ShowItemIcon(ACTOR_PRIZE_A, ITEM_CLAYMORE);
  Colosso_ShowItemIcon(ACTOR_PRIZE_B, ITEM_HUNTERS_SWORD);
  Actor_ConfigureSpriteColor(ACTOR_OPPONENT, SPRITE_COLOR_RBG);

  // Entrances: the race (1), the friends' turns, the tour, and the end
  // of Colosso after a win (4) or a loss (5).
  switch (gAdventure.entrance) {
  case COLOSSEUM_NAVAMPA_MATCH_RACE_FROM_LEND_A_HAND:
    Colosso_StartRaceGauge(ACTOR_PARTY_LEADER, ACTOR_OPPONENT, 6, PIXELS(1512),
                           PIXELS(192), ACTOR_PRIZE_A, ACTOR_PRIZE_B);
    MapGrid_CopyCellsLayered(127, 0, 1, 2, 5, 2);
    Actor_ReleaseById(ACTOR_GUIDE);
    Actor_ReleaseById(ACTOR_SCALES_ATTENDANT);
    Actor_ReleaseById(ACTOR_STEPPING_STONE_ATTENDANT);
    Actor_ReleaseById(ACTOR_WALL_WEDGE_ATTENDANT);
    Actor_ReleaseById(ACTOR_FREE_CLIMB_ATTENDANT);
    Actor_ReleaseById(ACTOR_MOVING_SIDEWALK_ATTENDANT);
    Actor_ReleaseById(ACTOR_LOG_ROLLING_ATTENDANT);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      Sound_PlaySong(SONG_FADE_OUT_BGM);
      Colosso_ControlCharacterAlone(ISAAC);
      Colosso_PlaceFriendsAtCheeringSpots();
      Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_PARTY_LEADER);
      ColosseumNavampaMatch_ShowRoundStartLettering(3);
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
    ColosseumNavampaMatch_ShowRoundStartLettering(0);
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
/// last match; timer and work hold signed 1/256-pixel steps.
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

  particle = Actor_GetById(ACTOR_PARTY_LEADER);
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
  particle = Actor_GetById(kind);
  if (particle == 0) {
    return;
  }
  source = Actor_GetById(ACTOR_PARTY_LEADER);
  if (source != 0) {
    Import_ActorSetPosition(kind, source->x.fixed, source->z.fixed);
  }
  Actor_SetSpriteFlags(Actor_GetById(kind), 0);
  particle->motionFlags = 0;
  particle->scaleX = 2 * Q16_ONE / 5;
  particle->scaleY = 2 * Q16_ONE / 5;
  {
    s32 height = PIXELS(4);
    particle->x.fixed = x + height;
    height += y;
    particle->y.fixed = height;
    particle->targetY = height;
  }
  particle->timer = 25;
  particle->userValue = 128;
  Actor_SetScript(kind, gColossoParticleKinds);
}

/// After the last match (COLOSSEUM_NAVAMPA_MATCH_AFTER_MATCH_WON or _LOST):
/// Garet, Ivan and Mia run to Isaac in the arena and call his name, but he
/// falls asleep, exhausted, and they leave him to sleep.
static void ColosseumNavampaMatch_PlayPostBattleReunion(s32 battleResult) {
  struct Actor *actor;
  const s32 *leaderScaleScript;
  TaskFunc particleFunc;

  // Place the opponent and companions around the resting leader.
  Actor_ReleaseById(ACTOR_PRIZE_A);
  Actor_ReleaseById(ACTOR_PRIZE_B);
  Party_RestoreHpPp(1);
  Sound_PlaySong(SONG_FADE_OUT_BGM);
  Event_Start();
  Actor_SetPosition(ACTOR_OPPONENT, PIXELS(1544), PIXELS(192));
  if (battleResult < 0) {
    Import_ActorSetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_ARMS_RAISED);
  } else {
    Import_ActorSetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_DEFEATED);
  }
  Actor_SetScript(ACTOR_OPPONENT, gColosseumNavampaMatch_OpponentAfterBattle);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(1504), PIXELS(192));
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  {
    u16 shown = 0;

    actor->facing = shown;
  }
  Actor_SetScript(ACTOR_PARTY_LEADER, gColosseumNavampaMatch_LeaderActionA);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CROUCH);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(1), PIXELS(1) / 2);
  Actor_SetPosition(ACTOR_GARET, PIXELS(1464), PIXELS(184));
  Actor_SetPosition(ACTOR_IVAN, PIXELS(1464), PIXELS(200));
  Actor_SetPosition(ACTOR_MIA, PIXELS(1448), PIXELS(192));
  actor = Actor_GetById(ACTOR_GARET);
  {
    u16 shown = 0;

    actor->facing = shown;
  }
  actor = Actor_GetById(ACTOR_IVAN);
  {
    u16 shown = 0;

    actor->facing = shown;
  }
  actor = Actor_GetById(ACTOR_MIA);
  {
    u16 shown = 0;

    actor->facing = shown;
  }
  WaitFrames(1);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  PaletteFade_ApplyTarget(PALETTE_TINT_GRAYSCALE, PALETTES_BG);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetMessageId(MsgColossoIsaac);
  Event_WaitFrames(60);
  leaderScaleScript = gColosseumNavampaMatch_LeaderScaleScript;
  Actor_SetScript(ACTOR_PARTY_LEADER, leaderScaleScript);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  actor->scaleX = Q16_ONE;
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  actor->scaleY = Q16_ONE;
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 36);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  actor->x.fixed += PIXELS(3);
  Event_WaitFrames(10);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(actor, 0);
  Event_WaitFrames(20);
  Actor_SetScript(ACTOR_PARTY_LEADER, gColosseumNavampaMatch_LeaderActionB);
  // Let each companion react before the leader settles back to sleep.
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(20);
  Actor_WalkToAndWait(ACTOR_GARET, 1504, 176);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 10);
  FieldEffect_SpawnAttached(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                            20);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_IVAN);
  Event_WaitFrames(30);
  Actor_WalkToAndWait(ACTOR_IVAN, 1488, 176);
  Actor_WalkTo(ACTOR_GARET, 1520, 184);
  Actor_WalkToAndWait(ACTOR_IVAN, 1504, 176);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_WaitFrames(10);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_GARET);
  Event_WaitFrames(30);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_MIA);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_MIA);
  Actor_WalkToAndWait(ACTOR_MIA, 1488, 184);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_PARTY_LEADER);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_PARTY_LEADER);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_GARET);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_IVAN);
  Event_WaitFrames(40);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_WaitFrames(10);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_MIA);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_MIA);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_SetScript(ACTOR_PARTY_LEADER, gColosseumNavampaMatch_LeaderActionC);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_PARTY_LEADER);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_PARTY_LEADER);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_PARTY_LEADER);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_SetScript(ACTOR_PARTY_LEADER, leaderScaleScript);
  Event_WaitFrames(60);
  particleFunc = ColosseumNavampaMatch_SpawnPeriodicParticle;
  // Time the small reunion effects to the companions' reactions.
  gColossoParticleCount = 9;
  Task_AddFrame(particleFunc, TASK_PASS_FRAME);
  Event_WaitFrames(5);
  Task_Remove(particleFunc);
  Event_WaitFrames(55);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Task_AddFrame(particleFunc, TASK_PASS_FRAME);
  Event_WaitFrames(20);
  Task_Remove(particleFunc);
  Event_WaitFrames(40);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  gColossoParticleCount = 9;
  Task_AddFrame(particleFunc, TASK_PASS_FRAME);
  Event_WaitFrames(35);
  Task_Remove(particleFunc);
  Event_WaitFrames(25);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  gColossoParticleCount = 9;
  Task_AddFrame(particleFunc, TASK_PASS_FRAME);
  Event_WaitFrames(35);
  Task_Remove(particleFunc);
  Event_WaitFrames(25);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_IVAN);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_MIA);
  Event_WaitFrames(60);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_PARTY_LEADER);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_PARTY_LEADER);
  gColossoParticleCount = 9;
  Task_AddFrame(particleFunc, TASK_PASS_FRAME);
  Event_WaitFrames(35);
  Task_Remove(particleFunc);
  Event_WaitFrames(25);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
  Actor_StartRepeatedMotion(ACTOR_GARET, 3);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 3);
  Actor_RunRepeatedMotion(ACTOR_MIA, 3);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_IVAN);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_IVAN);
  gColossoParticleCount = 9;
  Task_AddFrame(particleFunc, TASK_PASS_FRAME);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(60);
  // The companions begin to leave, then discuss the sleeping leader off-screen.
  Actor_WalkTo(ACTOR_MIA, 1464, 200);
  Event_WaitFrames(5);
  Actor_WalkTo(ACTOR_IVAN, 1368, 184);
  Event_WaitFrames(3);
  Actor_WalkToAndWait(ACTOR_GARET, 1512, 184);
  Actor_WalkTo(ACTOR_GARET, 1368, 184);
  Actor_WaitForMoveById(ACTOR_MIA);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_STAND);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_PARTY_LEADER);
  Event_WaitFrames(60);
  Actor_WalkToAndWait(ACTOR_MIA, 1432, 200);
  Actor_WalkTo(ACTOR_MIA, 1368, 184);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(30);
  Actor_SetPosition(ACTOR_GARET, PIXELS(1512), PIXELS(176));
  Actor_SetPosition(ACTOR_IVAN, PIXELS(1464), PIXELS(192));
  Actor_SetPosition(ACTOR_MIA, PIXELS(1560), PIXELS(200));
  ActorSystem_SuspendProjectionAndShowSprites();
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(1);
  Event_SetMessageId(MsgColossoIsaacFellAsleep);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_WaitFrames(60);
  Event_End();
}

/// The tour of Colosseum Final 3
/// (COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_NORTH): the guide welcomes the party
/// to the third finals match's stages and leaves them to their attendants, then
/// Garet, Ivan and Mia walk back into Isaac.
static void ColosseumNavampaMatch_IntroduceThirdFinals(s32 guideId) {
  s32 guideZ;
  s32 guideX;
  struct Actor *actor;

  actor = Actor_GetById(guideId);
  guideX = actor->x.part.pixel;
  guideZ = actor->z.part.pixel;
  Event_Start();
  Actor_SetSpeed(guideId, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(1), PIXELS(1) / 2);
  Import_ActorSetPosition(ACTOR_PARTY_LEADER, PIXELS(guideX),
                          (PIXELS(guideZ) - PIXELS(48)));
  Import_ActorSetPosition(ACTOR_GARET, PIXELS(guideX) - PIXELS(16),
                          PIXELS(guideZ) - PIXELS(40));
  Import_ActorSetPosition(ACTOR_IVAN, PIXELS(guideX) + PIXELS(16),
                          PIXELS(guideZ) - PIXELS(40));
  Import_ActorSetPosition(ACTOR_MIA, PIXELS(guideX),
                          (PIXELS(guideZ) - PIXELS(32)));
  Import_ActorSetPosition(guideId, PIXELS(guideX),
                          (PIXELS(guideZ) - PIXELS(80)));
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  actor->facing = FACING_NORTH;
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetMessageId(MsgColossoSiteThirdFinals);
  Actor_SetAnimationAndWait(guideId, ANIM_NOD);
  Event_ShowMessageAndWait(guideId, 0);
  Actor_RunRepeatedMotion(guideId, 2);
  Event_ShowMessageAndWait(guideId, 0);
  Actor_RunRepeatedMotion(guideId, 2);
  Event_ShowMessageAndWait(guideId, 0);
  Actor_RunRepeatedMotion(guideId, 2);
  Event_ShowMessageAndWait(guideId, 0);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(6);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_ResetAndSetPosition(ACTOR_GARET, actor->x.part.pixel,
                              actor->z.part.pixel);
  }
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_ResetAndSetPosition(ACTOR_IVAN, actor->x.part.pixel,
                              actor->z.part.pixel);
  }
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_WALK);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_ResetAndSetPosition(ACTOR_MIA, actor->x.part.pixel,
                              actor->z.part.pixel);
  }
  Actor_SetPositionAndReset(guideId, (guideX - 16), (guideZ - 64));
  Import_ActorSetPosition(ACTOR_GARET, 0, 0);
  Import_ActorSetPosition(ACTOR_IVAN, 0, 0);
  Import_ActorSetPosition(ACTOR_MIA, 0, 0);
  Actor_SetPositionAndReset(guideId, (guideX - 16), (guideZ - 16));
  Actor_SetPositionAndReset(guideId, guideX, guideZ);
  Actor_ArmCallback(guideId, FACING_NORTH, 10);
  Event_End();
}

/// The Scales of Justice's attendant, stage 1 of Colosseum Final 3: the first
/// time, a pointer shows the two ledges and how one rises as the other falls;
/// then the objective, and a place for a friend to cheer. With a friend alone
/// on the stages (COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) he asks whether
/// they are done cheering instead.
static void ColosseumNavampaMatch_TalkToScalesAttendant(s32 guideId) {
  struct Actor *westPan;
  struct Actor *eastPan;
  struct Actor *westPanAgain;
  struct Actor *eastPanAgain;
  s32 promptResult;

  if (gAdventure.entrance == COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(guideId);
    return;
  }
  Event_Start();
  promptResult = Colosso_GreetAtStage(guideId, 1);
  if (promptResult == COLOSSO_GUIDE_DESCRIBE) {
    // Point out the ledges, demonstrate both heights, then restore the stage.
    Event_SetMessageId(MsgColossoStageCalledScales);
    FieldCamera_SetSpeedParameters(PIXELS(3), PIXELS(3) / 8);
    FieldCamera_MoveWithinMapBounds(PIXELS(152), -1, PIXELS(200), 1);
    FieldCamera_WaitForArrival();
    Event_WaitFrames(30);
    Event_ShowMessageAndWait(guideId, 0);
    Colosso_ShowPointer(104, 68, 0);
    Event_WaitFrames(60);
    Colosso_MovePointer(168, 96, 10);
    Event_WaitFrames(70);
    Event_ShowMessageAndWait(guideId, 0);
    Colosso_HidePointer();
    WaitFrames(2);
    westPan = Actor_GetById(ACTOR_SCALES_WEST_LEDGE);
    westPan->motionFlags = 0;
    ColosseumNavampaMatch_MoveOnScales(westPan, PIXELS(4));
    eastPan = Actor_GetById(ACTOR_SCALES_EAST_LEDGE);
    eastPan->motionFlags = 0;
    ColosseumNavampaMatch_MoveOnScales(eastPan, PIXELS(32));
    Actor_WaitForMove(eastPan);
    Event_WaitFrames(45);
    westPanAgain = Actor_GetById(ACTOR_SCALES_WEST_LEDGE);
    westPanAgain->motionFlags = 0;
    ColosseumNavampaMatch_MoveOnScales(westPanAgain, PIXELS(32));
    eastPanAgain = Actor_GetById(ACTOR_SCALES_EAST_LEDGE);
    eastPanAgain->motionFlags = 0;
    ColosseumNavampaMatch_MoveOnScales(eastPanAgain, PIXELS(4));
    Actor_WaitForMove(eastPanAgain);
    Event_WaitFrames(15);
    Event_ShowMessageAndWait(guideId, 0);
    Colosso_ShowPointer(104, 68, 0);
    Event_WaitFrames(30);
    Colosso_MovePointer(168, 96, 10);
    Event_WaitFrames(40);
    Colosso_MovePointer(104, 68, 10);
    Event_WaitFrames(70);
    Event_ShowMessageAndWait(guideId, 0);
    Colosso_HidePointer();
    WaitFrames(2);
    FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Colosso_RepeatStageNumber(guideId, 1);
  } else if (promptResult == COLOSSO_GUIDE_OFFER_CHEERING) {
    Event_SetMessageId(MsgColossoObjectiveGetAcross);
    Event_ShowMessageAndWait(guideId, 0);
  }
  Colosso_OfferCheeringSpot(promptResult, guideId, 1);
  Event_End();
}

/// The Stepping-Stone Stage's attendant, stage 2 of Colosseum Final 3: the
/// first time, Isaac appears and finds the way; then the objective, and a
/// place for a friend to cheer.
static void ColosseumNavampaMatch_TalkToSteppingStoneAttendant(s32 guideId) {
  s32 promptResult;
  s32 x;

  if (gAdventure.entrance == COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(guideId);
    return;
  }
  Event_Start();
  promptResult = Colosso_GreetAtStage(guideId, 2);
  if (promptResult == COLOSSO_GUIDE_DESCRIBE) {
    Event_SetMessageId(MsgColossoSteppingStoneStage);
    FieldCamera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
    FieldCamera_MoveTo(PIXELS(376), -1, PIXELS(152), 1);
    x = 280;
    FieldCamera_WaitForArrival();
    Event_WaitFrames(30);
    Event_ShowMessage(guideId, 0);
    Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, x, 200);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, x, 152);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 296, 152);
    Event_WaitFrames(10);
    Field_TryJumpForward();
    FieldCamera_MoveTo(-1, -1, -1, 0);
    Actor_ArmCallback(ACTOR_PARTY_LEADER, FACING_NORTH, 15);
    Field_TryJumpForward();
    FieldCamera_MoveTo(-1, -1, -1, 0);
    Actor_ArmCallback(ACTOR_PARTY_LEADER, FACING_EAST, 15);
    Field_TryJumpForward();
    FieldCamera_MoveTo(-1, -1, -1, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 15);
    Event_ShowMessage(guideId, 0);
    Colosso_ShowPointer(96, 40, 0);
    Colosso_MovePointer(128, 40, 10);
    Event_WaitFrames(30);
    Colosso_MovePointer(160, 40, 10);
    Event_WaitFrames(30);
    Colosso_MovePointer(160, 72, 10);
    Event_WaitFrames(30);
    Event_ShowMessage(guideId, 0);
    Colosso_HidePointer();
    Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
    FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Colosso_RepeatStageNumber(guideId, 2);
  } else if (promptResult == COLOSSO_GUIDE_OFFER_CHEERING) {
    Event_SetMessageId(MsgColossoYourGoalInStageSimple);
    Event_ShowMessage(guideId, 0);
  }
  Colosso_OfferCheeringSpot(promptResult, guideId, 2);
  Event_End();
}

/// The Wall Wedge's attendant, stage 3 of Colosseum Final 3: the first time,
/// Isaac appears and times his way past the pistons; then that the walls'
/// operator will cheer for him, and a place for a friend to cheer.
static void ColosseumNavampaMatch_TalkToWallWedgeAttendant(s32 guideId) {
  struct FieldWork *work;
  s32 promptResult;

  if (gAdventure.entrance == COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(guideId);
  } else {
    Event_Start();
    promptResult = Colosso_GreetAtStage(guideId, 3);
    if (promptResult == COLOSSO_GUIDE_DESCRIBE) {
      // Show the pattern cycle before demonstrating a timed passage.
      work = gFieldWork;
      Event_SetMessageId(MsgColossoPlaceCalledWall);
      ColosseumNavampaMatch_ResetWallWedge();
      FieldCamera_SetSpeedParameters(PIXELS(3), PIXELS(3) / 8);
      FieldCamera_MoveWithinMapBounds(PIXELS(616), -1, PIXELS(184), 1);
      FieldCamera_WaitForArrival();
      Event_WaitFrames(30);
      Event_ShowMessage(guideId, 0);
      ColosseumNavampaMatch_StartWallWedge();
      Event_WaitFrames(60);
      Event_ShowMessageAndWait(guideId, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 504, 200);
      /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
       * show, which sets r0 last; as a direct call, 1 of 136 instructions
       * differ in tbs-en. */
      COLOSSEUM_NAVAMPA_MATCH_FACE_DIRECTION(ACTOR_PARTY_LEADER, 0, 0);
      ColosseumNavampaMatch_WaitForWallWedgeOpening();
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
      Colosso_WalkActorTo(ACTOR_PARTY_LEADER, 680, 200);
      if (work->raisedTrigger != 5) {
        do {
          WaitFrames(1);
          if (++promptResult > 239) {
            break;
          }
        } while (work->raisedTrigger != 5);
      }
      ColosseumNavampaMatch_PassSouthWallOpening();
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                      60);
      Event_ShowMessageAndWait(guideId, 0);
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(guideId, 3);
      work->raisedTrigger = 0;
    } else if (promptResult == COLOSSO_GUIDE_OFFER_CHEERING) {
      Event_SetMessageId(MsgColossoOperatorWallsCheer);
      Event_ShowMessageAndWait(guideId, 0);
    }
    Colosso_OfferCheeringSpot(promptResult, guideId, 3);
    Event_End();
  }
}

/// The Free-Climb Area's attendant, stage 4 of Colosseum Final 3: the first
/// time, Isaac appears and climbs across; then the objective, and a place for a
/// friend to cheer.
static void ColosseumNavampaMatch_TalkToFreeClimbAttendant(s32 guideId) {
  s32 result;
  struct Actor *actor;
  s32 x;
  s32 y;
  s32 raisedHeight;
  s32 shiftedX;

  if (gAdventure.entrance == COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(guideId);
  } else {
    Event_Start();
    result = Colosso_GreetAtStage(guideId, 4);
    if (result == COLOSSO_GUIDE_DESCRIBE) {
      Event_SetMessageId(MsgColossoPlaceNormallyCalledFreeClimb);
      FieldCamera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
      FieldCamera_MoveTo(PIXELS(856), -1, PIXELS(168), 1);
      FieldCamera_WaitForArrival();
      Event_WaitFrames(30);
      Event_ShowMessage(guideId, 0);
      Event_ShowMessage(guideId, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 816, 200);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 840, 200);
      Actor_ArmCallback(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
      Field_ClimbUp();
      FieldCamera_MoveTo(-1, -1, -1, 0);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
      actor = Actor_GetById(ACTOR_PARTY_LEADER);
      y = actor->y.fixed;
      x = actor->x.fixed;
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
      Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB);
      raisedHeight = PIXELS(6) + y;
      FieldActor_SetMoveTarget(actor, x, raisedHeight, actor->z.fixed);
      Actor_WaitForMove(actor);
      Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB_EAST);
      shiftedX = PIXELS(64) + x;
      FieldActor_SetMoveTarget(actor, shiftedX, raisedHeight, actor->z.fixed);
      Actor_WaitForMove(actor);
      Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB);
      FieldActor_SetMoveTarget(actor, shiftedX, y + PIXELS(54), actor->z.fixed);
      Actor_WaitForMove(actor);
      Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB_WEST);
      FieldActor_SetMoveTarget(actor, x + PIXELS(48), y + PIXELS(54),
                               actor->z.fixed);
      Actor_WaitForMove(actor);
      Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLING);
      Event_ShowMessage(guideId, 0);
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(guideId, 4);
    } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
      Event_SetMessageId(MsgColossoClearStageMustAbleChange);
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
    Actor_SetMoveTarget(actor, x << 16, actor->y.fixed, z << 16);
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
    Actor_SetMoveTarget(actor, x << 16, actor->y.fixed, z << 16);
    Actor_WaitForMove(actor);
    Actor_SelectAnimation(actor, 1);
  }
}

/// The Moving Sidewalk Stage's attendant, stage 5 of Colosseum Final 3: the
/// first time, Isaac appears and walks against the belt; then the objective,
/// and a place for a friend to cheer.
static void ColosseumNavampaMatch_TalkToMovingSidewalkAttendant(s32 guideId) {
  s32 i;
  s32 result;
  struct Actor *actor;

  if (gAdventure.entrance == COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(guideId);
  } else {
    Event_Start();
    result = Colosso_GreetAtStage(guideId, 5);
    if (result == COLOSSO_GUIDE_DESCRIBE) {
      // Walk against the sidewalk, then show how its current pushes the rider.
      Event_SetMessageId(MsgColossoCalledMovingSidewalkStage);
      FieldCamera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
      FieldCamera_MoveTo(PIXELS(1080), -1, PIXELS(168), 1);
      FieldCamera_WaitForArrival();
      Event_WaitFrames(30);
      Event_ShowMessage(guideId, 0);
      Event_ShowMessage(guideId, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 984, 184);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
      ColosseumNavampaMatch_PositionAndActivateActor(ACTOR_PARTY_LEADER, 992,
                                                     184);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 10, PIXELS(3) / 20);
      ColosseumNavampaMatch_PositionActor(ACTOR_PARTY_LEADER, 1120, 184);
      Event_WaitFrames(120);
      Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                      ATTACHED_EFFECT_ATTACH);
      Event_WaitFrames(120);
      Actor_ResetMotionFields(ACTOR_PARTY_LEADER);
      Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
#if !TBS_PATCH_3
      Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                      ATTACHED_EFFECT_REMOVE);
#endif
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      actor = Actor_GetById(ACTOR_PARTY_LEADER);
      for (i = 119; i >= 0; i--) {
        if (actor->x.fixed > PIXELS(992)) {
          actor->x.fixed -= PIXELS(6) / 5;
        }
        WaitFrames(1);
      }
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                      60);
      ColosseumNavampaMatch_PositionActor(ACTOR_PARTY_LEADER, 1120, 184);
      Event_ShowMessage(guideId, 0);
      Actor_ResetMotionFields(ACTOR_PARTY_LEADER);
      gAdventure.movementMode = LEADER_MOVEMENT_CLIMB;
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(guideId, 5);
    } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
      Event_SetMessageId(MsgColossoInStageMustTryOutpace);
      Event_ShowMessage(guideId, 0);
    }
    Colosso_OfferCheeringSpot(result, guideId, 5);
    Event_End();
  }
}

/// The log-rolling area's attendant, stage 6 of Colosseum Final 3: the first
/// time, a pointer traces the log's route; then the objective, and a place for
/// a friend to cheer.
static void ColosseumNavampaMatch_TalkToLogRollingAttendant(s32 actorId) {
  s32 state;

  if (gAdventure.entrance == COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH) {
    Colosso_AskDoneCheering(actorId);
    return;
  }

  Event_Start();
  state = Colosso_GreetAtStage(actorId, 6);

  if (state == COLOSSO_GUIDE_DESCRIBE) {
    Event_SetMessageId(MsgColossoAnotherLogRollingArea);
    FieldCamera_SetSpeedParameters(PIXELS(3), PIXELS(3) / 8);
    FieldCamera_MoveWithinMapBounds(PIXELS(1288), -1, PIXELS(152), 1);
    FieldCamera_WaitForArrival();
    Event_WaitFrames(30);
    Event_ShowMessageAndWait(actorId, 0);
    Colosso_ShowPointer(180, 88, 0);
    Event_WaitFrames(60);
    Event_ShowMessageAndWait(actorId, 0);
    Colosso_MovePointer(32, 84, 10);
    Event_WaitFrames(30);
    Event_ShowMessageAndWait(actorId, 0);
    Colosso_MovePointer(96, 84, 30);
    Event_WaitFrames(60);
    Event_ShowMessageAndWait(actorId, 0);
    Colosso_HidePointer();
    Event_WaitFrames(2);
    FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Colosso_RepeatStageNumber(actorId, 6);
  } else if (state == COLOSSO_GUIDE_OFFER_CHEERING) {
    Event_SetMessageId(MsgColossoHereMustFigureOutHow);
    Event_ShowMessageAndWait(actorId, 0);
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

/// Chooses the lettering animation script and resets its task state.
static void ColosseumNavampaMatch_StartLetteringAnimation(u32 mode,
                                                          u32 parameter) {
  s32 handler;

  gColosso_LetteringMode = mode;
  gColosso_LetteringParameter = (parameter << 4);

  {
    s32 priority = TASK_PASS_FRAME;
    TaskFunc task = Colosso_UpdateLettering;
    Task_AddFrame(task, priority);
  }

  handler = (s32)gColossoLetteringScriptTitle;
  if (mode == COLOSSO_SHAPE_DIGIT) {
    handler = (s32)gColossoLetteringScriptDigit;
  }
  if (mode == COLOSSO_SHAPE_GO) {
    handler = (s32)gColossoLetteringScriptGo;
  }
  if (mode == COLOSSO_SHAPE_RESULT) {
    if (parameter != 0) {
      handler = (s32)gColossoLetteringScriptLose;
    } else {
      handler = (s32)gColossoLetteringScriptWon;
    }
  }

  gColosso_LetteringTimer = 0;
  gColosso_LetteringScript = (s16 *)handler;
  gColosso_ModeMoveTarget = 0;
  gColosso_ModeMoveDuration = 0;
  gColosso_LetteringPosition = 0;
}

/// The lettering a visit opens with: `Lend a Hand` for the friends' turns
/// (mode 0), or for the race `Match 3`, then 3, 2, 1 and `Go!`.
static void ColosseumNavampaMatch_ShowRoundStartLettering(s32 mode) {
  if (mode == 0) {
    Event_Start();
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_WaitFrames(30);
    Sound_PlaySong(MUS_LEVEL_UP);
    Colosso_LoadLettering(COLOSSO_PICTURE_LEND_A_HAND);
    ColosseumNavampaMatch_StartLetteringAnimation(COLOSSO_SHAPE_TITLE, 0);
    Event_WaitFrames(120);
    Event_End();
    return;
  }

  Sound_PlaySong(SE_COLOSSO_LETTERING);
  Event_Start();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  {
    s16 *base = gColossoLetteringScriptTitle;
    *(s16 *)((u8 *)base + 30) = (mode * 60);
  }
  Event_WaitFrames(30);
  Sound_PlaySong(mode + 0x5a);
  Colosso_LoadLettering(mode);
  ColosseumNavampaMatch_StartLetteringAnimation(COLOSSO_SHAPE_TITLE, 0);
  Event_WaitFrames(120);

  while (Sound_GetBgmRestoreDelay() != 0) {
    WaitFrames(1);
  }

  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  Colosso_LoadLettering(COLOSSO_PICTURE_COUNT_3_2);
  ColosseumNavampaMatch_StartLetteringAnimation(COLOSSO_SHAPE_DIGIT, 0);
  Sound_PlaySong(SE_COLOSSO_COUNTDOWN);
  Event_WaitFrames(60);
  ColosseumNavampaMatch_StartLetteringAnimation(COLOSSO_SHAPE_DIGIT, 1);
  Sound_PlaySong(SE_COLOSSO_COUNTDOWN);
  Event_WaitFrames(60);
  Colosso_LoadLettering(COLOSSO_PICTURE_COUNT_1);
  ColosseumNavampaMatch_StartLetteringAnimation(COLOSSO_SHAPE_DIGIT, 0);
  Sound_PlaySong(SE_COLOSSO_COUNTDOWN);
  Event_WaitFrames(60);
  Colosso_LoadLettering(COLOSSO_PICTURE_GO);
  ColosseumNavampaMatch_StartLetteringAnimation(COLOSSO_SHAPE_GO, 0);
  Sound_PlaySong(SE_COLOSSO_GO);
  Scene_PlayMusic();
  Event_End();
  GameFlag_Set(FLAG_IN_TOWN);
}

/// Unused: a lettering countdown before the match, for a signed number of
/// seconds; nothing calls it.
UNUSED static void ColosseumNavampaMatch_UnusedCountdown(s32 countdown) {
  s32 actorId;

  Sound_PlaySong(SE_COLOSSO_LETTERING);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  gColossoLetteringScriptWon[13] = countdown * 60;
  STORE_CONST(gColossoLetteringScriptLose[13]) =
      (countdown < 0 ? -countdown : countdown) * 60;
  if (countdown < 0) {
    Event_WaitFrames(30);
    Sound_PlaySong(MUS_INN);
    Colosso_LoadLettering(COLOSSO_PICTURE_WON_LOSE_FOR_LOSS);
    /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
     * show, which sets r1 before r0; as a direct call, 1 of 72 instructions
     * differ in tbs-en. */
    COLOSSEUM_NAVAMPA_MATCH_INITIALIZE_MODE_TASK(3, 1);
    Event_WaitFrames(-countdown * 60 + 60);
    actorId = ACTOR_PARTY_LEADER;
  } else {
    Event_WaitFrames(30);
    Sound_PlaySong(countdown + 90);
    Colosso_LoadLettering(COLOSSO_PICTURE_WON_LOSE);
    /* FAKEMATCH(wrapper): the same cast as the first arm, which sets r1 before
     * r0; as a direct call, 1 of 72 instructions differ in tbs-en. */
    COLOSSEUM_NAVAMPA_MATCH_INITIALIZE_MODE_TASK(3, 0);
    Event_WaitFrames(countdown * 60 + 60);
    actorId = ACTOR_OPPONENT;
  }
  FieldEffect_SpawnAttached(actorId, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  while (Sound_GetBgmRestoreDelay() != 0) {
    WaitFrames(1);
  }
  Sound_PlaySong(MUS_SILENCE);
  Event_WaitFrames(30);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}

#include "shared/colosso_marker.h"
#include "shared/colosso_item_icon.h"

/// Unused: writes eight hexadecimal digits followed by a terminator; nothing
/// calls it.
UNUSED static void WriteU32AsHex(u8 *hexText, u32 value) {
  s32 digitIndex;

  hexText += 8;
  *hexText = 0;
  hexText--;
  for (digitIndex = 7; digitIndex >= 0; digitIndex--) {
    *hexText = gHexDigits[value & 15];
    value >>= 4;
    hexText--;
  }
}

static void ColosseumNavampaMatch_EmptyHook(void) {}

/// Unused: marks the opponent path recorder as finished; nothing calls it.
UNUSED static void ColosseumNavampaMatch_MarkOpponentFinished(void) {
  struct PathRecorder *recorder = (struct PathRecorder *)gSceneLocalState;

  recorder->mode = PATH_RECORDER_FINISHED;
}

/// Waits until the opponent path recorder reaches its finished mode.
static void ColosseumNavampaMatch_WaitForOpponentToFinish(void) {
  struct PathRecorder *recorder = (struct PathRecorder *)gSceneLocalState;

  while (recorder->mode != PATH_RECORDER_FINISHED) {
    WaitFrames(1);
  }
}

/// Occasionally scatters an effect around the prize being raised at the
/// arena.
static void ColosseumNavampaMatch_SpawnRandomFinishEffect(struct Actor *actor) {
  struct Actor *source = (struct Actor *)actor;

  s32 position[3];
  u32 randomValue;

  if (source->velocityY >= -255 && source->velocityY <= 255) {
    source->motionFlags = 0;
  }
  randomValue = Import_RandomNext();
  if (randomValue * 100 >> 16 <= 9) {
    struct Actor *effect;
    s32 angle;
    s32 radius;

    SET_VECTOR(position, source->x.fixed, source->y.fixed, source->z.fixed);
    angle = Import_RandomNext();
    radius = Import_RandomNext();
    Vector_AddPolarOffset(angle << 4, radius, position);
    {
      s32 x = position[0];
      s32 y = position[1];
      s32 z = position[2];

      effect = FieldActor_Create(SPRITE_SPARKLE, x, y, z);
    }
    if (effect != 0) {
      effect->motionFlags = 0;
      Actor_SetSpriteFlags(effect, 0);
      Actor_StartScript(effect, gColosseumNavampaMatch_PrizeEffectScript);
      Actor_SelectAnimation(effect, 1);
      Actor_SelectAnimation(effect, 0);
    }
  }
}

/// Presents the prize above the warrior who took it, with the treasure fanfare,
/// and clears the link.
static s32
ColosseumNavampaMatch_PlaceFinishItemAboveActor(struct Actor *interpreter) {
  struct Actor *source = interpreter;

  struct Actor *effect = Actor_GetById(source->timer);

  FieldActor_SetMoveTarget(effect, source->x.fixed,
                           source->y.fixed + PIXELS(36), source->z.fixed);
  effect->motionFlags = 0;
  Actor_StartScript(effect, gColosseumNavampaMatch_PrizeScript);
  Sound_PlaySong(MUS_TREASURE_FOUND);
  source->timer = 0;
  return 0;
}

/// Moves to the available finish item, equips it and announces it.
static s32 ColosseumNavampaMatch_CollectFinishEquipment(s32 firstItem,
                                                        s32 secondItem) {
  struct ColossoGaugeWork *gauge = (struct ColossoGaugeWork *)gColossoWork;
  struct Actor *actor;
  s32 opponentFinishedFirst;
  s32 x;
  s32 z;
  u16 *itemActorId;
  s16 *linkedItemActorId;

  opponentFinishedFirst = GameFlag_Get(FLAG_ARENA_REACHED);
  actor = Actor_GetById(gAdventure.leaderActorId);
  if (gauge->finishX < actor->x.fixed) {
    x = gauge->finishX + PIXELS(12);
  } else {
    x = gauge->finishX - PIXELS(12);
  }
  if (opponentFinishedFirst != 0) {
    z = gauge->finishZ + PIXELS(16);
    itemActorId = &gauge->finishItemActorIdB;
  } else {
    z = gauge->finishZ - PIXELS(16);
    itemActorId = &gauge->finishItemActorIdA;
  }
  linkedItemActorId = &actor->timer;
  *linkedItemActorId = *itemActorId;
  actor->acceleration = PIXELS(1) / 4;
  actor->speedLimit = PIXELS(1);
  FieldActor_SetMoveTarget(actor, x, 0, z);
  GameFlag_Set(FLAG_ARENA_REACHED);
  Actor_StartScript(actor, gColosseumNavampaMatch_PlaceScript);
  while (*linkedItemActorId != 0) {
    WaitFrames(1);
  }
  if (opponentFinishedFirst == 0) {
    /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
     * show, which sets r1 before r0; as a direct call, 1 of 110 instructions
     * differ in tbs-en. */
    COLOSSEUM_NAVAMPA_MATCH_APPLY_MATCHING_SLOT_VALUES(ISAAC, firstItem);
    MessageArgument_Push(firstItem, 2);
  } else {
    /* FAKEMATCH(wrapper): the same cast as the first arm, which sets r1 before
     * r0; as a direct call, 1 of 110 instructions differ in tbs-en. */
    COLOSSEUM_NAVAMPA_MATCH_APPLY_MATCHING_SLOT_VALUES(ISAAC, secondItem);
    MessageArgument_Push(secondItem, 2);
  }
  MessageArgument_Push(gAdventure.leaderActorId, 1);
  FieldMessage_Show(MsgColossoIsaacGotItem, 3);
  Actor_WaitForScriptEnd(actor);
  return opponentFinishedFirst;
}

#include "shared/colosso_path_opponent.h"
#include "shared/colosso_setup.h"
#include "shared/colosso_map_state.h"
#include "shared/actor_findslot.h"

/// Pushes a neighboring actor one cell when its destination and overhead
/// are clear.
static void ColosseumNavampaMatch_PushActorAhead(void) {
  struct Actor *subject;
  struct Actor *target;
  struct Actor *blocker;
  u32 step;
  u32 direction;
  struct FieldPosition position;
  s32 zero;
  s32 subjectHandle;

  subjectHandle = gAdventure.leaderActorId;
  subject = Actor_GetById(subjectHandle);

  direction = subject->facing >> 12;

  step = gPushedActor_DirectionSteps[direction];
  position.x = subject->x.fixed + (step & 0xffff0000);
  position.y = subject->y.fixed;
  step <<= 16;
  position.z = subject->z.fixed + step;

  target = Actor_FindAtPosition(&position, subject);
  if (target == 0) {
    return;
  }

  step = gPushedActor_DirectionSteps[direction];
  position.x = target->x.fixed + (step & 0xffff0000);
  position.y = target->y.fixed;
  step <<= 16;
  position.z = target->z.fixed + step;

  blocker = Actor_FindAtPosition(&position, target);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0) {
    return;
  }

  position.x = target->x.fixed;
  position.y = target->y.fixed + PIXELS(16);
  position.z = target->z.fixed;

  blocker = Actor_FindAtPosition(&position, target);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0) {
    return;
  }

  target->layer = 2;
  zero = 0;

  step = gPushedActor_DirectionSteps[direction];
  position.x = target->x.fixed + (step & 0xffff0000);
  position.y = target->y.fixed;
  step <<= 16;
  position.z = target->z.fixed + step;

  if (FieldActor_CheckMovementCollision(target, &position) > 0) {
    return;
  }

  Actor_SelectAnimation(subject, 8);
  WaitFrames(15);

  target->speedLimit = PIXELS(1) / 5;
  target->acceleration = PIXELS(1) / 5;
  FieldActor_SetMoveTarget(target, position.x, position.y, position.z);

  subject->speedLimit = PIXELS(1) / 5;
  subject->acceleration = PIXELS(1) / 5;
  FieldActor_SetMoveTarget(subject, position.x, position.y, position.z);

  Sound_PlaySong(SE_PUSH_STEP);
  Actor_WaitForMove(target);
  Sound_PlaySong(SE_STOP_ACTION_SE);

  target->x.fixed = position.x;
  target->z.fixed = position.z;
  target->velocityX = zero;
  target->velocityZ = zero;

  Actor_SelectAnimation(subject, 1);
}

#include "shared/colosso_occupant.h"

/// Closes the arena's step trigger; nothing in the game sets it.
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
    SC_SET(SW_Y, PIXELS(1)),     SC_WAIT(10), SC_SET(SW_Y, 0), SC_WAIT(10),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The Wall Wedge's four patterns: each piston's animation, west to east, then
/// the operator's.
const s8 gColosseumNavampaMatch_WallWedgePattern[][6] = {
    {7, 5, 7, 5, 7, 11},
    {4, 6, 4, 6, 4, 10},
    {5, 7, 5, 7, 5, 12},
    {6, 4, 6, 4, 6, 10},
};

// clang-format off
const u16 gColosseumNavampaMatch_FacingAngles[] = {
    0, 0, 0x4000, 0x8000, 0x8000, 0x8000, 0xc000, 0,
};

const s32 gColosseumNavampaMatch_SidewalkLogScript[] = {
    SC_MOVE(PIXELS(1088), 0, PIXELS(184)),
    SC_SYNC,
    SC_MOVE(PIXELS(1130), 0, PIXELS(184)),
    SC_SYNC,
    SC_CALL(ColosseumNavampaMatch_ShowActivatedSidewalkLayer),
    SC_LABEL(1),
    SC_SET(SW_Y, PIXELS(1)),
    SC_WAIT(10),
    SC_SET(SW_Y, 0),
    SC_WAIT(10),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

const u8 gColosso_LetteringPaletteOffsets[] = {
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
    0, 32, 32, 32, 96, 64, 64, 64, 128, 0,
#else
    0, 32, 32, 32, 96, 64, 64, 160, 128, 0,
#endif
};

const s16 gColossoLetteringScriptDigit[] = {
    0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 30, 0x2000, 0,
    30, 0x7fff, 30, -1,
};

const s16 gColossoLetteringScriptLose[] = {
    0x1000, 128, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 60, 0x1000, 384,
    6, 0x7fff, 6, 0x1000, 240, 6, 0x7fff, 6,
    0x1000, 368, 6, 0x7fff, 6, 0x1000, 224, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, 352, 6,
    0x7fff, 6, 0x1000, 208, 6, 0x7fff, 6, 0x1000,
    336, 6, 0x7fff, 6, 0x1000, 192, 6, 0x7fff,
    6, 0x1000, 320, 6, 0x7fff, 6, -1,
};

char gHexDigits[20] = "0123456789ABCDEF";

s32 gColosseumNavampaMatch_PrizeEffectScript[] = {
    SC_ADD(SW_Y, -(PIXELS(1) / 4)),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_RELEASE,
};

s32 gColosseumNavampaMatch_PrizeScript[] = {
    SC_SET(SW_HOOK, (s32)ColosseumNavampaMatch_SpawnRandomFinishEffect),
    SC_LABEL(1),
    SC_WAIT(10),
    SC_SET(SW_SCALE_X, 11 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 11 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 9 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 9 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 11 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 11 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 9 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 9 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(2),
    SC_WAIT(50),
    SC_LOOP(2, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_SET(SW_HOOK, 0),
    SC_END,
};

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(x, z) ((u32)(x) << 16 | (u16)(z))

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
    { 0x0, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(104), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(152), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(216), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(168), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(312), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(712), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1240), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1272), 0, PIXELS(168), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1304), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(536), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(568), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(600), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(632), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(664), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(536), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(568), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(600), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(632), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(664), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(720), 0, PIXELS(216), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(792), 0, PIXELS(194), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x126, CONDITION_ALWAYS, 7,
        PIXELS(1080), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(1208), 0, PIXELS(176), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(408), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(312), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(376), 0, PIXELS(312), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(600), 0, PIXELS(312), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(856), 0, PIXELS(312), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1080), 0, PIXELS(312), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1304), 0, PIXELS(312), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1512), 0, PIXELS(176), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1512), 0, PIXELS(208), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x118, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { 0x118, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x118, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
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
    { TRIGGERS_END },
};

/// Navampa after the last battle (ColosseumNavampaMatch_PlayPostBattleReunion): his
/// sprite grows taller and narrower and back, over and over.
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

s16 gColossoLetteringScriptTitle[] = {
#if LANGUAGE == LANGUAGE_JAPANESE
    0x4000, -188, 2048, 0x1000, 256, 1, 0x2000, 16,
#else
    0x4000, -188, 2048, 0x1000, 384, 1, 0x2000, 16,
#endif
    1, 0x7fff, 14, 0x3000, 0, 20, 0x7fff, 60,
    0x3000, 2048, 20, 0x7fff, 60, -1,
};

s16 gColossoLetteringScriptGo[] = {
#if LANGUAGE == LANGUAGE_FRENCH
    0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 384,
    6, 0x2000, 16, 6, 0x7fff, 6, 0x1000, 512,
    6, 0x7fff, 6, 0x1000, 384, 6, 0x7fff, 6,
    0x1000, 512, 6, 0x7fff, 6, 0x1000, 384, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, 512, 6,
    0x7fff, 6, 0x1000, 384, 6, 0x7fff, 6, 0x1000,
    512, 6, 0x7fff, 6, 0x1000, 384, 6, 0x7fff,
    6, 0x1000, 512, 6, 0x7fff, 6, -1,
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
    0x1000, 384, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 6, 0x1000, 384,
    6, 0x7fff, 6, 0x1000, 256, 6, 0x7fff, 6,
    0x1000, 384, 6, 0x7fff, 6, 0x1000, 256, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, 384, 6,
    0x7fff, 6, 0x1000, 256, 6, 0x7fff, 6, 0x1000,
    384, 6, 0x7fff, 6, 0x1000, 256, 6, 0x7fff,
    6, 0x1000, 384, 6, 0x7fff, 6, -1,
#else
    0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 6, 0x1000, 384,
    6, 0x7fff, 6, 0x1000, 256, 6, 0x7fff, 6,
    0x1000, 384, 6, 0x7fff, 6, 0x1000, 256, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, 384, 6,
    0x7fff, 6, 0x1000, 256, 6, 0x7fff, 6, 0x1000,
    384, 6, 0x7fff, 6, 0x1000, 256, 6, 0x7fff,
    6, 0x1000, 384, 6, 0x7fff, 6, -1,
#endif
};

s16 gColossoLetteringScriptWon[] = {
    0x1000, 128, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 60, 0x1000, 384,
    6, 0x7fff, 6, 0x1000, 272, 6, 0x7fff, 6,
    0x1000, 400, 6, 0x7fff, 6, 0x1000, 288, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, 416, 6,
    0x7fff, 6, 0x1000, 304, 6, 0x7fff, 6, 0x1000,
    432, 6, 0x7fff, 6, 0x1000, 320, 6, 0x7fff,
    6, 0x1000, 448, 6, 0x7fff, 6, -1,
};
// clang-format on

s16 gColosso_MarkerSlot = -1;

s32 gColosso_OpponentFinishScript[] = {
    SC_ANIMATION(2),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(6),
    SC_CALL(ColosseumNavampaMatch_PlaceFinishItemAboveActor),
    SC_WAIT(3),
    SC_ANIMATION(10),
    SC_WAIT(150),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ANIMATION(1),
    SC_END,
};

s32 gColosseumNavampaMatch_PlaceScript[] = {
    SC_ANIMATION(2),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(6),
    SC_CALL(ColosseumNavampaMatch_PlaceFinishItemAboveActor),
    SC_WAIT(3),
    SC_ANIMATION(28),
    SC_WAIT(150),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ANIMATION(1),
    SC_END,
};
