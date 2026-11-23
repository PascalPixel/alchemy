/// @file
/// Vale Cave, the optional cave above Vale where Kite, a Jupiter Djinni, hides.
/// Its overlay holds three scenes.
///
/// - The clearing outside (Vale, SceneId_ValeCaveMouth), a grassy hillside
///   reached by the two paths that run north past the west side of Kraden's
///   cottage. The lower path comes in at cells (5..7, 22) and the raised path
///   east of it at (8, 23..25); both lead back down (ValeCave_LeaveRoom).
///   The cave's mouth is a dark opening in the cliff at cell (8, 10), beside
///   stacked crates and two logs. The game still calls the clearing Vale, so
///   arriving here clears FLAG_SHOW_AREA_NAME.
/// - The first room of the cave (SceneId_ValeCaveKite), one map holding three
///   parts that the player meets as separate rooms, joined by passages.
///   Inside the mouth is a pool crossed by hopping from post to post. Reveal
///   shows the post missing from the row, at cell (15, 15)
///   (ValeCave_RevealHiddenTile). Beyond it, a passage in the north-west leads
///   to a pool with two stacks of crates and a log floating by its north wall.
///   Move or a push sinks the eastern stack into the water, where its top
///   becomes a square to walk across on the way to Kite's part. Two Moves sink
///   the western stack beside the log. Isaac then hops onto the log, which
///   carries him across the pool to the passage on its east side
///   (ValeCave_RideLogEast).
/// - The room of logs (SceneId_ValeCaveLogs). Its southern passage leads round
///   to a pool between two ledges, each with a log standing on it, and a third
///   log lying across the floor south of the water. Isaac rolls the two
///   standing logs into the water, hops down, and pushes the lying log north
///   into the pool, where it floats and lets him across.
/// - Kite's part of the first room, reached from the room of logs and from
///   the riding log's pool. The Halt Gem's and the Nut's chests are near the
///   top of a narrow ridge. Walking down the ridge startles Kite, who hops off
///   its south end onto the round floor below, where three mossy rocks stand in
///   a row (ValeCave_KiteRunsOff). Kite then runs round behind the rocks
///   (ValeCave_KiteRunsBehindRocks) and keeps to their far side as Isaac walks
///   round them (ValeCave_KiteKeepsAway). Halt stops it, and
///   talking to it then makes it join the party (ValeCave_TalkToKite,
///   FLAG_KITE).
///
/// No story flag is read here and no message is shown. Each room's progress
/// lives in scene and area flags, and on every arrival
/// ValeCave_ApplyEntryState rebuilds the cells of each puzzle already solved
/// and puts Kite back behind its rocks. The two rooms of the cave proper are
/// one area, so their area flags (the riding log's end, Kite behind the
/// rocks) last while the party goes between them.

#include "ability_ids.h"
#include "djinn_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor.h"
#include "field/create_configured_actor.h"
#include "field/event.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/pushed_actor.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/field_effect_spawn.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/heap_slots.h"
#include "system/main_routine_work.h"
#include "system/system.h"
#include "system/system_call.h"

extern s32 gValeCave_WalkTargets[][2];
extern struct SceneEntrance gScene_FallbackEntrances[];
extern struct SceneEntrance gValeCave_MouthEntrances[];
extern struct SceneEntrance gValeCave_KiteEntrances[];
extern struct SceneEntrance gValeCave_LogsEntrances[];
extern u32 gValeCave_Exits[];
extern struct ActorPlacement gValeCave_MouthActorPlacements[];
extern struct ActorPlacement gValeCave_KiteActorPlacements[];
extern struct ActorPlacement gValeCave_LogsActorPlacements[];
extern struct Trigger gValeCave_TriggersOther[];
extern struct Trigger gValeCave_MouthTriggers[];
extern struct Trigger gValeCave_KiteTriggers[];
extern struct Trigger gValeCave_LogsTriggers[];

/// The clearing's actor placements (gValeCave_MouthActorPlacements), numbered
/// from ACTOR_FIRST_PLACED.
///
/// They are three stacks of crates, drawn as the crates of the Bilibin
/// Barricade in bilibin.c, then two logs, as in Kolima Forest. None of them
/// moves.
enum ValeCaveMouthActorId {
  ACTOR_VALE_CAVE_MOUTH_FIRST_CRATES = ACTOR_FIRST_PLACED,
  ACTOR_VALE_CAVE_MOUTH_FIRST_LOG = ACTOR_FIRST_PLACED + 3,
  ACTOR_VALE_CAVE_MOUTH_SECOND_LOG
};

/// The first room's actor placements (gValeCave_KiteActorPlacements), numbered
/// from ACTOR_FIRST_PLACED.
enum ValeCaveKiteActorId {
  /// Kite, a Jupiter Djinni, placed until it joins (FLAG_KITE).
  ACTOR_KITE = ACTOR_FIRST_PLACED,
  /// The two stacks of crates at the edge of the riding log's pool, which sink
  /// into the water. The western stack sinks under Move, the eastern one under
  /// Move or a push.
  ACTOR_WEST_CRATES,
  ACTOR_EAST_CRATES,
  /// The log floating by the north wall of its pool, which carries Isaac from
  /// one end of the pool to the other. It is drawn as the north-south log of
  /// Colosso's Lumber Water
  /// Crossing Area (colosseum_azart_match.c).
  ACTOR_RIDING_LOG,
  /// Two stacks of crates and two logs, stacked by the passage to the room of
  /// logs as they are at the cave's mouth.
  ACTOR_FIRST_STANDING_CRATES,
  ACTOR_SECOND_STANDING_CRATES,
  ACTOR_FIRST_STANDING_LOG,
  ACTOR_SECOND_STANDING_LOG
};

/// The room of logs' actor placements (gValeCave_LogsActorPlacements), numbered
/// from ACTOR_FIRST_PLACED.
///
/// Its first row places Kite as the first room's does, out of sight of this
/// room. Its three logs are pushed, and are drawn as Kolima Forest's.
enum ValeCaveLogsActorId {
  /// Standing on the ledge west of the pool, rolled east into the water.
  ACTOR_WEST_LOG = ACTOR_FIRST_PLACED + 1,
  /// Standing on the ledge east of the pool, pushed west until it rolls off.
  ACTOR_EAST_LOG,
  /// Lying west to east across the floor south of the pool, pushed north.
  ACTOR_SOUTH_LOG
};

/// The cells the cave's step triggers answer.
enum ValeCaveCellBehavior {
  /// The riding log's west end, cells (23, 34..36), and its east end, cells
  /// (35, 34..36). Only the end the log lies at holds its behavior.
  CELL_BEHAVIOR_VALE_CAVE_RIDING_LOG_WEST_END = 20,
  CELL_BEHAVIOR_VALE_CAVE_RIDING_LOG_EAST_END,
  /// The bank east of the riding log's pool's eastern crates, cell (29, 41).
  CELL_BEHAVIOR_VALE_CAVE_EAST_CRATES_BANK = 24,
  /// In Kite's part, the top of the ridge, cell (49, 11), and its foot,
  /// cell (49, 16).
  CELL_BEHAVIOR_VALE_CAVE_KITE_RIDGE_TOP = 40,
  CELL_BEHAVIOR_VALE_CAVE_KITE_RIDGE_FOOT,
  /// The first of the twelve cell behaviors on the ring of floor round Kite's
  /// mossy rocks (ValeCave_KiteKeepsAway), one for each of
  /// gValeCave_WalkTargets.
  CELL_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE = 45
};

/// The cells of the room of logs' step triggers.
enum ValeCaveLogsCellBehavior {
  /// The ledge east of the east log, cells (20, 16..17).
  CELL_BEHAVIOR_VALE_CAVE_EAST_LOG_LEDGE = 20,
  /// The ledge west of the west log, cells (11, 16..17).
  CELL_BEHAVIOR_VALE_CAVE_WEST_LOG_LEDGE,
  /// The floor south of the south log, cells (13..16, 23).
  CELL_BEHAVIOR_VALE_CAVE_SOUTH_LOG_FLOOR
};

/// The map columns where the riding log's pool's western and eastern crates
/// sink: the western stack at cell (23, 39), after two Moves east, and the
/// eastern one at cell (27, 41), after one Move or push west.
#define WEST_CRATES_SINK_COLUMN 23
#define EAST_CRATES_SINK_COLUMN 27
/// The map column at the west end of the east log's ledge, cell (17, 16),
/// from which the log rolls off into the pool.
#define LEDGE_EDGE_COLUMN 17
/// The map row the south log can stop on when pushed north.
#define SOUTH_LOG_SOUTH_ROW 20

/// The flag of Kite, set once it is caught.
#define FLAG_KITE (FLAG_DJINNI(DJINNI_KITE))

// The cave's scene flags: each room reuses them for its own puzzle, so the
// first room's two have one name at the riding log's pool and another in
// Kite's part.
/// The riding log's pool: the western crates have sunk into the water.
#define FLAG_VALE_CAVE_WEST_CRATES_SUNK FLAG_SCENE_TEMP_00
/// The riding log's pool: the eastern crates have sunk into the water.
#define FLAG_VALE_CAVE_EAST_CRATES_SUNK FLAG_SCENE_TEMP_01
/// Kite's part: Kite has run off once (ValeCave_KiteRunsOff).
#define FLAG_VALE_CAVE_KITE_RAN_OFF FLAG_SCENE_TEMP_00
/// Kite's part: Kite has run behind the mossy rocks
/// (ValeCave_KiteRunsBehindRocks).
#define FLAG_VALE_CAVE_KITE_RAN_BEHIND_ROCKS FLAG_SCENE_TEMP_01

// The cave's area flags. The first room and the room of logs are one area, so
// these last while the party goes between them; the clearing is another.
/// The riding log is at the east end of its pool.
#define FLAG_VALE_CAVE_RIDING_LOG_EAST FLAG_AREA_TEMP_01
/// Kite has run behind the mossy rocks and keeps to their far side.
#define FLAG_VALE_CAVE_KITE_BEHIND_ROCKS FLAG_AREA_TEMP_02

static void ValeCave_CenterActorOnCell(s32 actorId, s32 column, s32 row);
static void ValeCave_ApplyEntryState(void);

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"
#include "shared/staged_actor.c"

/// In the room of logs, the west log has been rolled into the pool.
#define FLAG_VALE_CAVE_WEST_LOG_PUSHED_OFF_LEDGE FLAG_SCENE_TEMP_02
/// In the room of logs, the east log has been pushed along its ledge and has
/// rolled off into the pool.
#define FLAG_VALE_CAVE_EAST_LOG_PUSHED_OVER_EDGE FLAG_SCENE_TEMP_03
/// In the room of logs, the south log has been pushed north and stopped on
/// the northern of its two rows.
#define FLAG_VALE_CAVE_SOUTH_LOG_ON_NORTH_ROW FLAG_SCENE_TEMP_04
/// In the room of logs, the south log has been pushed north and stopped on
/// the southern of its two rows (SOUTH_LOG_SOUTH_ROW).
#define FLAG_VALE_CAVE_SOUTH_LOG_ON_SOUTH_ROW FLAG_SCENE_TEMP_05

/// Talking to Kite, once Halt has stopped it on the round floor below the
/// ridge, makes the Jupiter Djinni join the party.
static void ValeCave_TalkToKite(void) {
  Djinn_JoinParty(ACTOR_KITE, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_KITE));
}

/// Waits for an actor to land, bounded by a one-second timeout.
static void ValeCave_WaitForActorLanding(struct Actor *actor) {
  s32 framesRemaining = 60;

  while (framesRemaining != 0) {
    wait(1);
    framesRemaining--;
    if (actor->y.fixed <= actor->groundHeight)
      break;
  }
  actor->y.fixed = actor->groundHeight;
}

/// Turns Kite to face the party leader each frame, so that it watches Isaac
/// from behind its rocks.
static UNREAD_RESULT ValeCave_FaceActorTowardLeader(struct Actor *actor) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  FIXED distanceZ = leader->z.fixed - actor->z.fixed;
  FIXED distanceX = leader->x.fixed - actor->x.fixed;

  actor->facing = Trig_Atan(distanceZ, distanceX);
  RETURN_UNREAD(0);
}

/// ValeCave_FaceActorTowardLeader, as an actor's update takes it.
#ifdef UBFIX
#define VALE_CAVE_FACE_LEADER_CALLBACK ValeCave_FaceActorTowardLeader
#else
#define VALE_CAVE_FACE_LEADER_CALLBACK                                         \
  ((void (*)(struct Actor *))ValeCave_FaceActorTowardLeader)
#endif

/// Selects entrance positions and camera bounds for the current cave room.
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_VALE_CAVE_MOUTH) {
    return gValeCave_MouthEntrances;
  }
  if (sceneId == SCENE_VALE_CAVE_KITE) {
    return gValeCave_KiteEntrances;
  }
  if (sceneId == SCENE_VALE_CAVE_LOGS) {
    return gValeCave_LogsEntrances;
  }
  return gScene_FallbackEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

/// Returns the shared packed exits for all cave rooms.
const u32 *Scene_GetExits(void) { return gValeCave_Exits; }

/// Selects the current room's actor placements, which are Kite, and the crates
/// and logs.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_VALE_CAVE_MOUTH) {
    return gValeCave_MouthActorPlacements;
  }
  if (sceneId == SCENE_VALE_CAVE_KITE) {
    return gValeCave_KiteActorPlacements;
  }
  if (sceneId == SCENE_VALE_CAVE_LOGS) {
    return gValeCave_LogsActorPlacements;
  }
  return gValeCave_MouthActorPlacements;
}

/// When Reveal fades in the pool just inside the cave, the post it showed at
/// cell (15, 15) is gone again and the gap in the row of posts is water.
static void ValeCave_HideRevealedTile(void) {
  MapGrid_CopyCells(16, 15, 1, 1, 15, 15);
}

/// Reveal, cast in the pool just inside the cave's mouth, shows the post
/// missing from the row of posts across the water, at cell (15, 15), so that
/// Isaac can hop over it to the west side.
///
/// The post is copied from a spare one at cell (16, 17), and
/// ValeCave_HideRevealedTile puts the water back when Reveal fades.
static void ValeCave_RevealHiddenTile(void) {
  MapGrid_CopyCells(16, 17, 1, 1, 15, 15);
}

/// Sinks a stack of crates into the water over 68 frames, with a white splash
/// of spray round it.
static inline void
ValeCave_SinkActorWithParticles(s32 actorId, s32 baseX, s32 baseZ,
                                struct EffectOptions *particleOptions) {
  u32 frame;

  for (frame = 0; frame < 68; frame++) {
    FIXED particleX = INTtoFIXED(Random_Below(17)) + baseX;
    FIXED particleZ = INTtoFIXED(Random_Below(14)) + baseZ;

    FieldEffect_Spawn(particleX, 0, particleZ, 0, 0, 0,
                      EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
                      particleOptions);
    Actor_Get(actorId)->y.fixed -= toFIXED(1) / 2;
    Event_Wait(1);
  }
}

/// Move on the western stack of crates in the riding log's pool sinks it into
/// the water once a second Move has brought it to cell (23, 39), just south of
/// the floating log. Its top is then a square Isaac hops onto, on his way to
/// the log.
///
/// The game's Move has already slid the stack a cell east. Short of the sink
/// column the event does nothing more.
static void ValeCave_MoveWestCratesIntoWater(void) {
  struct EffectOptions options;
  struct Actor *leftEffect;
  struct Actor *rightEffect;

  Event_Start();
  if (FIELD_CELL(Actor_Get(ACTOR_WEST_CRATES)->x.fixed) ==
      WEST_CRATES_SINK_COLUMN) {
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 360, 664);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 10);
    Actor_Get(ACTOR_WEST_CRATES)->x.fixed += toFIXED(2);
    leftEffect = Actor_PrepareFX(
        Actor_Get(ACTOR_WEST_CRATES)->x.fixed, 0,
        Actor_Get(ACTOR_WEST_CRATES)->z.fixed + toFIXED(52), SPRITE_CRATES);
    rightEffect = Actor_PrepareFX(
        Actor_Get(ACTOR_WEST_CRATES)->x.fixed + toFIXED(16), 0,
        Actor_Get(ACTOR_WEST_CRATES)->z.fixed + toFIXED(52), SPRITE_CRATES);
    Actor_Get(ACTOR_WEST_CRATES)->motionFlags = 0;
    // Surround the sinking crates with one particle per frame.
    options.startScaleX = 3 * FIXED_ONE / 5;
    options.startScaleY = 3 * FIXED_ONE / 5;
    options.palette = SPRITE_COLOR_WHITE;
    Sound_Play(SE_BURST);
    ValeCave_SinkActorWithParticles(ACTOR_WEST_CRATES, toFIXED(368),
                                    toFIXED(624), &options);
    MapGrid_CopyCellAttributes(23, 41, 1, 1, 23, 39);
    Actor_Get(ACTOR_WEST_CRATES)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    GameFlag_Set(FLAG_VALE_CAVE_WEST_CRATES_SUNK);
    Actor_Get(ACTOR_WEST_CRATES)->y.fixed = -toFIXED(8);
    Actor_SetAnimation(ACTOR_WEST_CRATES, 2);
    Actor_Release(leftEffect);
    Actor_Release(rightEffect);
    Event_Wait(30);
  }
  Event_End();
}

/// Move on the eastern stack of crates, at the foot of the rock pillar in the
/// riding log's pool, slides it one cell west into the water at cell (27, 41).
/// It sinks with a splash until only its top shows, a square of floor Isaac
/// can walk across.
static void ValeCave_MoveEastCratesIntoWater(void) {
  struct EffectOptions options;
  struct Actor *leftEffect;
  struct Actor *rightEffect;

  Event_Start();
  if (FIELD_CELL(Actor_Get(ACTOR_EAST_CRATES)->x.fixed) ==
      EAST_CRATES_SINK_COLUMN) {
    leftEffect = Actor_PrepareFX(
        Actor_Get(ACTOR_EAST_CRATES)->x.fixed - toFIXED(8), 0,
        Actor_Get(ACTOR_EAST_CRATES)->z.fixed + toFIXED(52), SPRITE_CRATES);
    rightEffect = Actor_PrepareFX(
        Actor_Get(ACTOR_EAST_CRATES)->x.fixed + toFIXED(8), 0,
        Actor_Get(ACTOR_EAST_CRATES)->z.fixed + toFIXED(52), SPRITE_CRATES);
    // This clears the western crates' motion flags, not these crates', where
    // ValeCave_MoveWestCratesIntoWater clears its own. These sink all the
    // same, since the sinking writes their height directly and nothing pulls
    // them back up, as the recording of this event shows.
    Actor_Get(ACTOR_WEST_CRATES)->motionFlags = 0;
    // Surround the sinking crates with one particle per frame.
    options.startScaleX = 3 * FIXED_ONE / 5;
    options.startScaleY = 3 * FIXED_ONE / 5;
    options.palette = SPRITE_COLOR_WHITE;
    Sound_Play(SE_BURST);
    ValeCave_SinkActorWithParticles(ACTOR_EAST_CRATES, toFIXED(432),
                                    toFIXED(656), &options);
    MapGrid_CopyCellAttributes(31, 39, 2, 1, 27, 41);
    Actor_Get(ACTOR_EAST_CRATES)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    GameFlag_Set(FLAG_VALE_CAVE_EAST_CRATES_SUNK);
    Actor_Get(ACTOR_EAST_CRATES)->y.fixed = -toFIXED(8);
    Actor_SetAnimation(ACTOR_EAST_CRATES, 2);
    Actor_Release(leftEffect);
    Actor_Release(rightEffect);
    Event_Wait(30);
  }
  Event_End();
}

/// Pushing the eastern stack of crates west, from the bank east of it at cell
/// (29, 41), slides it a cell into the water, where it sinks as it does under
/// Move (ValeCave_MoveEastCratesIntoWater).
static void ValeCave_PushEastCratesIntoWater(void) {
  Event_Start();
  PushedActor_AdvancePair();
  Event_End();
  ValeCave_MoveEastCratesIntoWater();
}

/// Walking south off the riding log, once it has carried him to the east side
/// of its pool (cell (35, 36)), Isaac leaps down onto the rock bank below.
///
/// He lands two cells south, at cell (35, 38), and only if nothing stands
/// there.
static s32 ValeCave_LeapOffRidingLog(s32 unusedArgument) {
  struct FieldCoordinate landingPosition[3];
  struct Actor *leader;
  s32 savedMotionFlags;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  savedMotionFlags = leader->motionFlags;
#if TBS_PATCH_1
  landingPosition[0].fixed = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  landingPosition[1].fixed = leader->y.fixed;
  landingPosition[2].fixed = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(40);
#else
  landingPosition[0].fixed = leader->x.fixed;
  landingPosition[1].fixed = leader->y.fixed;
  landingPosition[2].fixed = leader->z.fixed + toFIXED(32);
#endif
  if (FieldActor_CheckMovementCollision(
          leader, (struct FieldPosition *)landingPosition) == 0) {
    Event_Start();
    Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(leader, ANIM_LEADER_LEAP);
    leader->speedLimit = toFIXED(3);
    leader->acceleration = toFIXED(2);
    leader->velocityY = toFIXED(4);
    leader->motionFlags &= ACTOR_JUMP_KEEPS_MOTION;
    Actor_SetSpriteFlags(leader, 0);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, landingPosition[0].part[0].integer,
                        landingPosition[2].part[0].integer);
    Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
    leader->motionFlags = savedMotionFlags;
    Event_End();
    return 1;
  }
  return 0;
}

/// Walking onto the narrow ridge that runs south between two channels of water
/// in Kite's part (cell (49, 11)) startles Kite, who stands further down it.
/// The camera follows Kite as it gives a start and hops off the ridge's south
/// end onto the round floor below.
static void ValeCave_KiteRunsOff(void) {
  if (Event_GetFlag(FLAG_VALE_CAVE_KITE_RAN_OFF) == 0) {
    GameFlag_Set(FLAG_VALE_CAVE_KITE_RAN_OFF);
    Event_Start();
    FieldCamera_SetSpeed(1);
    FieldCamera_FollowActor(ACTOR_KITE, 1);
    FieldCamera_WaitForArrival();
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_KITE, FACING_NORTH, 20);
    Actor_SetAttachedEffect(ACTOR_KITE,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_RunRepeatedMotion(ACTOR_KITE, 2);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_KITE, 1);
    Actor_WalkToAndWait(ACTOR_KITE, 792, 248);
    Sound_Play(SE_JUMP);
    Actor_Get(ACTOR_KITE)->velocityY = toFIXED(8);
    Actor_WalkToAndWait(ACTOR_KITE, 792, 280);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_KITE, FACING_NORTH, 20);
    Event_Wait(30);
    Event_End();
  }
}

/// Reaching the foot of the ridge (cell (49, 16)) after Kite has fled makes it
/// run round behind the three mossy rocks on the round floor, where it turns
/// to watch Isaac.
static void ValeCave_KiteRunsBehindRocks(void) {
  if (GameFlag_Get(FLAG_VALE_CAVE_KITE_RAN_OFF) != 0) {
    if (Event_GetFlag(FLAG_VALE_CAVE_KITE_RAN_BEHIND_ROCKS) == 0) {
      GameFlag_Set(FLAG_VALE_CAVE_KITE_RAN_BEHIND_ROCKS);
      GameFlag_Set(FLAG_VALE_CAVE_KITE_BEHIND_ROCKS);
      Event_Start();
      Actor_SetAttachedEffect(ACTOR_KITE,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_RunRepeatedMotion(ACTOR_KITE, 2);
      Event_Wait(20);
      Actor_SetSpeed(ACTOR_KITE, 2);
      Actor_WalkToAndWait(ACTOR_KITE, 760, 280);
      Actor_WalkToAndWait(ACTOR_KITE, 760, 312);
      Actor_WalkToAndWait(ACTOR_KITE, 792, 312);
      Event_Wait(10);
      Actor_FaceDirection(ACTOR_KITE, FACING_NORTH, 20);
      Actor_Get(ACTOR_KITE)->update = VALE_CAVE_FACE_LEADER_CALLBACK;
      Event_End();
    }
  }
}

/// As Isaac walks round the three mossy rocks, Kite keeps to their far side,
/// matching his pace, until Halt stops it.
///
/// Each cell of the ring of floor round the rocks (cells (46..52, 17..20))
/// sends Kite to the point opposite, from gValeCave_WalkTargets.
static void ValeCave_KiteKeepsAway(void) {
  struct FieldWork *work = gFieldWork;
  s32 route;

  if (GameFlag_Get(FLAG_VALE_CAVE_KITE_BEHIND_ROCKS) != 0 &&
      gMainRoutineWork.haltedActor != ACTOR_KITE) {
    route = work->stepTrigger - CELL_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE;
    Actor_Get(ACTOR_KITE)->speedLimit =
        Actor_Get(ACTOR_PARTY_LEADER)->speedLimit;
    Actor_Get(ACTOR_KITE)->acceleration =
        Actor_Get(ACTOR_PARTY_LEADER)->acceleration;
    Actor_WalkTo(ACTOR_KITE, gValeCave_WalkTargets[route][0],
                 gValeCave_WalkTargets[route][1]);
  }
}

/// Halt on Kite stops it where it stands behind the mossy rocks, so that Isaac
/// can walk round and talk to it.
static void ValeCave_HaltKite(void) {
  Event_Start();
  Actor_SetAnimation(ACTOR_KITE, ANIM_STILL);
  Event_End();
}

/// Pushing the log standing on the ledge east of the pool in the room of logs
/// moves it west a cell at a time. At the ledge's west end, cell (17, 16), it
/// rolls off into the pool.
static void ValeCave_PushLogOverEdge(void) {
  struct PushedActorProbe probe;

  Event_Start();
  if (PushedActor_FindClearPosition(&probe) != 0) {
    Actor_MoveAndRedraw(probe);
    // At the ledge's edge the log slides west and drops.
    if (FIELD_CELL(probe.position.x) == LEDGE_EDGE_COLUMN) {
      Actor_SetAnimation(probe.actorSlot, LOG_ANIMATION_ROLL_WEST);
      Actor_Get(probe.actorSlot)->motionFlags = 0;
      Actor_Get(probe.actorSlot)->bounce = 0;
      Actor_SetDestinationOffset(probe.actorSlot, -12, 0);
      Actor_WaitForMoveById(probe.actorSlot);
      Actor_SetAnimation(probe.actorSlot, LOG_ANIMATION_ROLL_WEST);
      Actor_SetSpritePriority(ACTOR_EAST_LOG, 3);
      Actor_Get(probe.actorSlot)->motionFlags =
          ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      Actor_SetDestinationOffset(probe.actorSlot, -6, 0);
      ValeCave_WaitForActorLanding(Actor_Get(probe.actorSlot));
      Actor_SetAnimation(probe.actorSlot, LOG_ANIMATION_DROP_IN);

      Actor_Get(probe.actorSlot)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      // The cells it held on the ledge, and its landing, become open floor.
      PushedActor_FillGridAttributeRectangle(0, FIELD_CELL(probe.position.x),
                                             (FIELD_CELL(probe.position.z) - 2),
                                             1, 4, 0);
      PushedActor_FillGridAttributeRectangle(2, FIELD_CELL(probe.position.x),
                                             (FIELD_CELL(probe.position.z) - 2),
                                             1, 4, 0);
      PushedActor_FillGridAttributeRectangle(2, 16, 18, 1, 2, 0);
      PushedActor_FillGridAttributeRectangle(0, 16, 16, 1, 4, 0);
      GameFlag_Set(FLAG_VALE_CAVE_EAST_LOG_PUSHED_OVER_EDGE);
      Sound_Play(SE_LOG_FALL);
    }
  }
  Event_End();
}

/// Pushing the log standing on the ledge west of the pool in the room of logs,
/// from cell (11, 16), rolls it east off the ledge into the pool.
static void ValeCave_PushLogOffLedge(void) {
  Event_Start();
  Event_Wait(10);
  Actor_SetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 0.5, 0.1);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
  Event_Wait(15);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 8, 0);
  Event_Wait(4);
  Event_PlaySound(SE_STOP_ACTION_SE);
  Sound_Play(SE_HEAVY_MOVE);
  Actor_SetSpeedAndAcceleration(ACTOR_WEST_LOG, 0.5, 0.1);
  Actor_SetAnimation(ACTOR_WEST_LOG, 2);
  Actor_Get(ACTOR_WEST_LOG)->motionFlags = 0;
  Actor_Get(ACTOR_WEST_LOG)->bounce = 0;
  Actor_SetDestinationOffset(ACTOR_WEST_LOG, 12, 0);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_WaitForMoveById(ACTOR_WEST_LOG);
  Sound_Play(SE_STOP_ACTION_SE);
  Sound_Play(SE_HEAVY_STOP);
  Actor_SetAnimation(ACTOR_WEST_LOG, 3);
  Actor_Get(ACTOR_WEST_LOG)->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_SetDestinationOffset(ACTOR_WEST_LOG, 6, 0);
  ValeCave_WaitForActorLanding(Actor_Get(ACTOR_WEST_LOG));
  Actor_SetAnimation(ACTOR_WEST_LOG, LOG_ANIMATION_DROP_IN);
  Actor_SetSpritePriority(ACTOR_WEST_LOG, 3);
  Actor_Get(ACTOR_WEST_LOG)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  PushedActor_FillGridAttributeRectangle(0, 12, 16, 1, 4, CELL_BEHAVIOR_NORMAL);
  PushedActor_FillGridAttributeRectangle(0, 13, 16, 1, 4, CELL_BEHAVIOR_NORMAL);
  GameFlag_Set(FLAG_VALE_CAVE_WEST_LOG_PUSHED_OFF_LEDGE);
  Sound_Play(SE_LOG_FALL);
  Event_End();
}

/// Pushing the log that lies across the floor south of the pool in the room of
/// logs north tips it into the pool's south end, where it floats with a
/// splash and makes a way across.
static void ValeCave_PushLogNorth(void) {
  struct PushedActorProbe probe;

  Event_Start();
  if (PushedActor_FindClearPosition(&probe) != 0) {
    Actor_MoveAndRedraw(probe);
    Actor_SetAnimation(ACTOR_SOUTH_LOG, 3);
    Actor_SetSpeedAndAcceleration(ACTOR_SOUTH_LOG, 0.25, 0.5);
    Actor_SetDestinationOffset(ACTOR_SOUTH_LOG, 0, -16);
    Event_Wait(45);
    Sound_Play(SE_LOG_FALL);
    Actor_SetAnimation(ACTOR_SOUTH_LOG, 8);

    Actor_Get(ACTOR_SOUTH_LOG)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    PushedActor_FillGridAttributeRectangle(
        0, 13, (FIELD_CELL(probe.position.z)) - 1, 4, 2, 0);
    if (FIELD_CELL(probe.position.z) == SOUTH_LOG_SOUTH_ROW) {
      GameFlag_Set(FLAG_VALE_CAVE_SOUTH_LOG_ON_SOUTH_ROW);
    } else {
      GameFlag_Set(FLAG_VALE_CAVE_SOUTH_LOG_ON_NORTH_ROW);
      MapGrid_CopyCellAttributes(14, 17, 2, 1, 14, 16);
      MapGrid_CopyCellAttributes(14, 13, 1, 1, 14, 15);
    }
  }
  Event_End();
}

/// Selects the current room's step, Psynergy and examine triggers.
const struct Trigger *Scene_GetTriggers(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_VALE_CAVE_MOUTH) {
    return gValeCave_MouthTriggers;
  }
  if (sceneId == SCENE_VALE_CAVE_KITE) {
    return gValeCave_KiteTriggers;
  }
  if (sceneId == SCENE_VALE_CAVE_LOGS) {
    return gValeCave_LogsTriggers;
  }
  return gValeCave_TriggersOther;
}

/// Arriving in any of the three rooms chooses the entry transition and restores
/// the room's puzzle state.
s32 Scene_Initialize(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gMainRoutineWork.next == SCENE_VALE_CAVE_MOUTH) {
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    wait(1);
    Actor_SetSpritePriority(ACTOR_VALE_CAVE_MOUTH_FIRST_LOG, 3);
    Actor_SetSpritePriority(ACTOR_VALE_CAVE_MOUTH_SECOND_LOG, 3);
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  }
  ValeCave_ApplyEntryState();
  return 0;
}

/// In the clearing outside Vale Cave, Isaac walks into the cave's mouth (cell
/// (8, 10)) or down either path back to Kraden's cottage: the lower path at
/// cells (5..7, 22) or the raised one east of it at (8, 23..25).
///
/// Each plays the exit sound first. The cell's behavior is the exit's number.
static void ValeCave_LeaveRoom(void) {
  s32 exitNumber = gFieldWork->stepTrigger;

  Sound_Play(SE_MAP_EXIT);
  Import_EventSetExitRequest(exitNumber);
}

/// Puts the riding log at the end of its pool where it was left, and makes that
/// end the one Isaac can step onto.
///
/// The passage's cells are copied back from their spare copy at cells (0..12,
/// 34..36), and the far end is then covered with water.
static void ValeCave_UpdateRidingLogPassage(void) {
  MapGrid_CopyCellAttributes(0, 34, 13, 3, 23, 34);

  if (GameFlag_Get(FLAG_VALE_CAVE_RIDING_LOG_EAST) != 0) {
    ValeCave_CenterActorOnCell(ACTOR_RIDING_LOG, 35, 35);
    MapGrid_CopyCellAttributes(24, 34, 1, 3, 23, 34);
  } else {
    ValeCave_CenterActorOnCell(ACTOR_RIDING_LOG, 23, 35);
    MapGrid_CopyCellAttributes(24, 34, 1, 3, 35, 34);
  }
}

#include "shared/actor_pair_move.c"

/// Walking east onto the riding log at the west end of its pool (cell (23,
/// 35)) has it float east along the north wall with Isaac on it, to the pool's
/// east side, where a post leads to the passage into the room of logs.
static void ValeCave_RideLogEast(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_RIDING_LOG, 112, 0);
  Actor_PlacePairAtOffset(ACTOR_RIDING_LOG, 80, 0);
  GameFlag_Set(FLAG_VALE_CAVE_RIDING_LOG_EAST);
  wait(2);
  ValeCave_UpdateRidingLogPassage();
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
}

/// Walking west onto the riding log at the east end of its pool (cell (35,
/// 35)) has it float back west along the north wall with Isaac on it.
static void ValeCave_RideLogWest(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_RIDING_LOG, -112, 0);
  Actor_PlacePairAtOffset(ACTOR_RIDING_LOG, -80, 0);
  GameFlag_Clear(FLAG_VALE_CAVE_RIDING_LOG_EAST);
  wait(2);
  ValeCave_UpdateRidingLogPassage();
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
}

/// Centers an actor on a map cell and gives it underfoot priority.
static void ValeCave_CenterActorOnCell(s32 actorId, s32 column, s32 row) {
  struct Actor *actor = Actor_Get(actorId);

  if (actor != NULL) {
    Actor_SetSpritePriority(actorId, 3);
    actor->layer = 2;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actor->x.fixed = FIELD_CELL_CENTER(column);
    actor->z.fixed = FIELD_CELL_CENTER(row);
  }
}

/// Restores, on entry, Kite behind its mossy rocks and the crates and logs of
/// the puzzles already solved.
static void ValeCave_ApplyEntryState(void) {
  s32 southLogOnSouthernRow;

  // Each room reuses the temporary flags for its own puzzle state.
  if (gMainRoutineWork.next == SCENE_VALE_CAVE_KITE) {
    switch (gMainRoutineWork.entrance) {
    case VALE_CAVE_KITE_FROM_VALE_CAVE_MOUTH:
    case VALE_CAVE_KITE_WEST_FROM_EAST:
    case VALE_CAVE_KITE_WEST_FROM_SOUTH:
    case VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_WEST:
      Actor_SetSpritePriority(ACTOR_SECOND_STANDING_LOG, 3);
      Actor_SetSpritePriority(ACTOR_SECOND_STANDING_CRATES, 3);
      // A still actor of sprite 223 (crate.png, a wooden crate) that shows
      // nothing. XXX (no evidence): what it and the chest below were meant to
      // show; the sprites alone hold neither, and neither actor is solid or a
      // target.
      Actor_CreateConfigured(120, 0, 232, SPRITE_CRATE);
      break;
    case VALE_CAVE_KITE_EAST_FROM_WEST:
    case VALE_CAVE_KITE_EAST_FROM_SOUTH:
    case VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_EAST:
      if (GameFlag_Get(FLAG_KITE) != 0) {
        break;
      }
      if (GameFlag_Get(FLAG_VALE_CAVE_KITE_BEHIND_ROCKS) == 0) {
        break;
      }
      GameFlag_Set(FLAG_VALE_CAVE_KITE_RAN_OFF);
      if (gMainRoutineWork.entrance == VALE_CAVE_KITE_EAST_FROM_WEST) {
        GameFlag_Set(FLAG_VALE_CAVE_KITE_RAN_BEHIND_ROCKS);
      }
      wait(1);
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
        break;
      }
      // Kite waits behind the mossy rocks, watching the leader.
      Actor_SetPosition(ACTOR_KITE, 792, 280);
      Actor_Get(ACTOR_KITE)->update = VALE_CAVE_FACE_LEADER_CALLBACK;
      break;
    case VALE_CAVE_KITE_SOUTH_FROM_WEST:
    case VALE_CAVE_KITE_SOUTH_FROM_EAST:
    case VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_SOUTH:
      // The riding log's part: a still chest actor that shows nothing, the
      // riding log's passage, and the cells of crates already sunk.
      Actor_CreateConfigured(642, 0, 552, SPRITE_CHEST);
      MapGrid_CopyCellAttributes(23, 34, 13, 3, 0, 34);
      ValeCave_UpdateRidingLogPassage();
      // The cells ValeCave_MoveWestCratesIntoWater copies as it sets the flag,
      // redrawn on arrival.
      if (GameFlag_Get(FLAG_VALE_CAVE_WEST_CRATES_SUNK) != 0) {
        MapGrid_CopyCellAttributes(23, 41, 1, 1, 23, 39);
      }
      // The cells ValeCave_MoveEastCratesIntoWater copies as it sets the flag,
      // redrawn on arrival.
      if (GameFlag_Get(FLAG_VALE_CAVE_EAST_CRATES_SUNK) != 0) {
        MapGrid_CopyCellAttributes(31, 39, 2, 1, 27, 41);
      }
      break;
    }
  } else if (gMainRoutineWork.next == SCENE_VALE_CAVE_LOGS) {
    switch (gMainRoutineWork.entrance) {
    case VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_WEST:
    case VALE_CAVE_LOGS_NORTH_FROM_SOUTH:
    case VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_EAST:
      // Each log already pushed leaves open floor; else its rectangle
      // blocks its cells again.
      if (GameFlag_Get(FLAG_VALE_CAVE_WEST_LOG_PUSHED_OFF_LEDGE) != 0) {
        PushedActor_FillGridAttributeRectangle(0, 12, 16, 1, 4,
                                               CELL_BEHAVIOR_NORMAL);
        PushedActor_FillGridAttributeRectangle(0, 13, 16, 1, 4,
                                               CELL_BEHAVIOR_NORMAL);
      } else {
        Scene_RedrawActorRectangle(ACTOR_WEST_LOG);
      }
      if (GameFlag_Get(FLAG_VALE_CAVE_EAST_LOG_PUSHED_OVER_EDGE) != 0) {
        PushedActor_FillGridAttributeRectangle(2, 16, 16, 1, 4,
                                               CELL_BEHAVIOR_NORMAL);
        PushedActor_FillGridAttributeRectangle(0, 16, 16, 1, 4,
                                               CELL_BEHAVIOR_NORMAL);
      } else {
        Scene_RedrawActorRectangle(ACTOR_EAST_LOG);
      }
      southLogOnSouthernRow =
          GameFlag_Get(FLAG_VALE_CAVE_SOUTH_LOG_ON_SOUTH_ROW);
      if (southLogOnSouthernRow != 0) {
        PushedActor_FillGridAttributeRectangle(0, 13, 19, 4, 2,
                                               CELL_BEHAVIOR_NORMAL);
      } else if (GameFlag_Get(FLAG_VALE_CAVE_SOUTH_LOG_ON_NORTH_ROW) != 0) {
        PushedActor_FillGridAttributeRectangle(0, 13, 15, 4, 2,
                                               southLogOnSouthernRow);
        MapGrid_CopyCellAttributes(14, 17, 2, 1, 14, 16);
        MapGrid_CopyCellAttributes(14, 13, 1, 1, 14, 15);
      } else {
        Scene_RedrawActorRectangle(ACTOR_SOUTH_LOG);
        Actor_SetSpritePriority(ACTOR_SOUTH_LOG, 3);
      }
      break;
    }
  }
}

/// Stepping onto either end of the riding log starts its floating animation
/// over from the first frame.
static void ValeCave_TouchRidingLog(void) {
  Actor_SetAnimation(ACTOR_RIDING_LOG, 1);
  Actor_SetAnimation(ACTOR_RIDING_LOG, 2);
}

#include "shared/field_effect_duration_scripts.c"

#include "shared/pushed_actor_tables.c"
// clang-format off

/// The twelve points round Kite's mossy rocks, x then z in pixels,
/// one for each cell behavior from CELL_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE on.
s32 gValeCave_WalkTargets[][2] = {
    760, 296,
    760, 280,
    776, 280,
    792, 280,
    808, 280,
    824, 280,
    824, 296,
    824, 312,
    808, 312,
    792, 312,
    776, 312,
    760, 312,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

#include "shared/fallback_entrances.c"

struct SceneEntrance gValeCave_MouthEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, 16, 0, 336, 368, 0 },
    { VALE_CAVE_MOUTH_FROM_VALE_KRADENS_COTTAGE_WEST, CONDITION_ALWAYS, 104, 0, 344, FACING_NORTH, 0, 16, 0, 336, 368, 0 },
    { VALE_CAVE_MOUTH_FROM_VALE_CAVE_KITE, CONDITION_ALWAYS, 136, 0, 184, FACING_SOUTH, 0, 16, 0, 336, 368, 0 },
    { VALE_CAVE_MOUTH_FROM_VALE_KRADENS_COTTAGE_EAST, CONDITION_ALWAYS, 136, 0, 376, FACING_NORTH, 0, 16, 0, 336, 368, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gValeCave_KiteEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_CAVE_KITE_FROM_VALE_CAVE_MOUTH, CONDITION_ALWAYS, 328, 0, 392, FACING_NORTH, 0, 48, 32, 544, 416, 0 },
    { VALE_CAVE_KITE_WEST_FROM_EAST, CONDITION_ALWAYS, 312, 0, 160, FACING_SOUTH, 0, 48, 32, 544, 416, 0 },
    { VALE_CAVE_KITE_WEST_FROM_SOUTH, CONDITION_ALWAYS, 120, 0, 120, FACING_SOUTH, 0, 48, 32, 544, 416, 0 },
    { VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_WEST, CONDITION_ALWAYS, 504, 0, 136, FACING_SOUTH, 0, 48, 32, 544, 416, 0 },
    { VALE_CAVE_KITE_EAST_FROM_WEST, CONDITION_ALWAYS, 792, 0, 360, FACING_NORTH, 0, 624, 16, 944, 384, 0 },
    { VALE_CAVE_KITE_EAST_FROM_SOUTH, CONDITION_ALWAYS, 680, 0, 248, FACING_SOUTH, 0, 624, 16, 944, 384, 0 },
    { VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_EAST, CONDITION_ALWAYS, 904, 0, 232, FACING_SOUTH, 0, 624, 16, 944, 384, 0 },
    { VALE_CAVE_KITE_SOUTH_FROM_WEST, CONDITION_ALWAYS, 408, 0, 872, FACING_NORTH, 0, 312, 472, 704, 896, 0 },
    { VALE_CAVE_KITE_SOUTH_FROM_EAST, CONDITION_ALWAYS, 488, 0, 744, FACING_NORTH, 0, 312, 472, 704, 896, 0 },
    { VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_SOUTH, CONDITION_ALWAYS, 632, 0, 552, FACING_SOUTH, 0, 312, 472, 704, 896, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gValeCave_LogsEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 200, 0, 432, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_WEST, CONDITION_ALWAYS, 200, 0, 432, FACING_NORTH, 0, 64, 24, 416, 464, 0 },
    { VALE_CAVE_LOGS_NORTH_FROM_SOUTH, CONDITION_ALWAYS, 344, 0, 120, FACING_SOUTH, 0, 64, 24, 416, 464, 0 },
    { VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_EAST, CONDITION_ALWAYS, 120, 0, 264, FACING_NORTH, 0, 64, 24, 416, 464, 0 },
    { VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_SOUTH, CONDITION_ALWAYS, 536, 0, 776, FACING_NORTH, 0, 400, 440, 896, 800, 0 },
    { VALE_CAVE_LOGS_SOUTH_FROM_NORTH, CONDITION_ALWAYS, 840, 0, 776, FACING_NORTH, 0, 400, 440, 896, 800, 0 },
    { SCENE_TABLE_END },
};

u32 gValeCave_Exits[] = {
    SCENE_EXITS(ValeCaveMouth),
    SCENE_EXIT(VALE_CAVE_MOUTH_TO_VALE_KRADENS_COTTAGE_WEST, ValeKradensCottage, VALE_KRADENS_COTTAGE_FROM_VALE_CAVE_MOUTH_WEST),
    SCENE_EXIT(VALE_CAVE_MOUTH_TO_VALE_CAVE_KITE, ValeCaveKite, VALE_CAVE_KITE_FROM_VALE_CAVE_MOUTH),
    SCENE_EXIT(VALE_CAVE_MOUTH_TO_VALE_KRADENS_COTTAGE_EAST, ValeKradensCottage, VALE_KRADENS_COTTAGE_FROM_VALE_CAVE_MOUTH_EAST),
    SCENE_EXITS(ValeCaveKite),
    SCENE_EXIT(VALE_CAVE_KITE_TO_VALE_CAVE_MOUTH, ValeCaveMouth, VALE_CAVE_MOUTH_FROM_VALE_CAVE_KITE),
    SCENE_EXIT(VALE_CAVE_KITE_WEST_TO_EAST, ValeCaveKite, VALE_CAVE_KITE_EAST_FROM_WEST),
    SCENE_EXIT(VALE_CAVE_KITE_WEST_TO_SOUTH, ValeCaveKite, VALE_CAVE_KITE_SOUTH_FROM_WEST),
    SCENE_EXIT(VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_WEST, ValeCaveLogs, VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_WEST),
    SCENE_EXIT(VALE_CAVE_KITE_EAST_TO_WEST, ValeCaveKite, VALE_CAVE_KITE_WEST_FROM_EAST),
    SCENE_EXIT(VALE_CAVE_KITE_EAST_TO_SOUTH, ValeCaveKite, VALE_CAVE_KITE_SOUTH_FROM_EAST),
    SCENE_EXIT(VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_EAST, ValeCaveLogs, VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_EAST),
    SCENE_EXIT(VALE_CAVE_KITE_SOUTH_TO_WEST, ValeCaveKite, VALE_CAVE_KITE_WEST_FROM_SOUTH),
    SCENE_EXIT(VALE_CAVE_KITE_SOUTH_TO_EAST, ValeCaveKite, VALE_CAVE_KITE_EAST_FROM_SOUTH),
    SCENE_EXIT(VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_SOUTH, ValeCaveLogs, VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_SOUTH),
    SCENE_EXITS(ValeCaveLogs),
    SCENE_EXIT(VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_WEST, ValeCaveKite, VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_WEST),
    SCENE_EXIT(VALE_CAVE_LOGS_NORTH_TO_SOUTH, ValeCaveLogs, VALE_CAVE_LOGS_SOUTH_FROM_NORTH),
    SCENE_EXIT(VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_EAST, ValeCaveKite, VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_EAST),
    SCENE_EXIT(VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_SOUTH, ValeCaveKite, VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_SOUTH),
    SCENE_EXIT(VALE_CAVE_LOGS_SOUTH_TO_NORTH, ValeCaveLogs, VALE_CAVE_LOGS_NORTH_FROM_SOUTH),
    SCENE_EXITS_END,
};

struct ActorPlacement gValeCave_MouthActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_CRATES, CONDITION_ALWAYS, 7,
        232, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CRATES, CONDITION_ALWAYS, 7,
        232, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CRATES, CONDITION_ALWAYS, 7,
        248, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        192, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        208, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gValeCave_KiteActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_KITE, MOVEMENT_TYPE_STAND,
        792, 0, 232, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CRATES, CONDITION_ALWAYS, 7,
        344, 0, 632, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CRATES, CONDITION_ALWAYS, 7,
        456, 0, 664, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        376, 0, 576, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CRATES, CONDITION_ALWAYS, 7,
        422, 0, 206, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CRATES, CONDITION_ALWAYS, 7,
        440, 0, 206, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        384, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        400, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    { SCENE_TABLE_END },
};

struct ActorPlacement gValeCave_LogsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_KITE, MOVEMENT_TYPE_STAND,
        792, 0, 232, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        200, 0, 288, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        312, 0, 288, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        240, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct Trigger gValeCave_TriggersOther[] = {
    { TRIGGER_TABLE_END },
};

struct Trigger gValeCave_MouthTriggers[] = {
    { TRIGGER_STEP, VALE_CAVE_MOUTH_TO_VALE_KRADENS_COTTAGE_WEST, CONDITION_ALWAYS, EVENT(ValeCave_LeaveRoom) },
    { TRIGGER_STEP, VALE_CAVE_MOUTH_TO_VALE_CAVE_KITE, CONDITION_ALWAYS, EVENT(ValeCave_LeaveRoom) },
    { TRIGGER_STEP, VALE_CAVE_MOUTH_TO_VALE_KRADENS_COTTAGE_EAST, CONDITION_ALWAYS, EVENT(ValeCave_LeaveRoom) },
    { TRIGGER_TABLE_END },
};

struct Trigger gValeCave_KiteTriggers[] = {
    { TRIGGER_EXIT, VALE_CAVE_KITE_TO_VALE_CAVE_MOUTH, CONDITION_ALWAYS, VALE_CAVE_KITE_TO_VALE_CAVE_MOUTH },
    { TRIGGER_EXIT, VALE_CAVE_KITE_WEST_TO_EAST, CONDITION_ALWAYS, VALE_CAVE_KITE_WEST_TO_EAST },
    { TRIGGER_EXIT, VALE_CAVE_KITE_WEST_TO_SOUTH, CONDITION_ALWAYS, VALE_CAVE_KITE_WEST_TO_SOUTH },
    { TRIGGER_EXIT, VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_WEST, CONDITION_ALWAYS, VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_WEST },
    { TRIGGER_EXIT, VALE_CAVE_KITE_EAST_TO_WEST, CONDITION_ALWAYS, VALE_CAVE_KITE_EAST_TO_WEST },
    { TRIGGER_EXIT, VALE_CAVE_KITE_EAST_TO_SOUTH, CONDITION_ALWAYS, VALE_CAVE_KITE_EAST_TO_SOUTH },
    { TRIGGER_EXIT, VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_EAST, CONDITION_ALWAYS, VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_EAST },
    { TRIGGER_EXIT, VALE_CAVE_KITE_SOUTH_TO_WEST, CONDITION_ALWAYS, VALE_CAVE_KITE_SOUTH_TO_WEST },
    { TRIGGER_EXIT, VALE_CAVE_KITE_SOUTH_TO_EAST, CONDITION_ALWAYS, VALE_CAVE_KITE_SOUTH_TO_EAST },
    { TRIGGER_EXIT, VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_SOUTH, CONDITION_ALWAYS, VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_SOUTH },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_VALE_CAVE_RIDING_LOG_WEST_END, CONDITION_ALWAYS, EVENT(ValeCave_RideLogEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_VALE_CAVE_RIDING_LOG_EAST_END, CONDITION_ALWAYS, EVENT(ValeCave_RideLogWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_VALE_CAVE_RIDING_LOG_EAST_END, CONDITION_ALWAYS, EVENT(ValeCave_LeapOffRidingLog) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_CAVE_RIDING_LOG_WEST_END, CONDITION_ALWAYS, EVENT(ValeCave_TouchRidingLog) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_CAVE_RIDING_LOG_EAST_END, CONDITION_ALWAYS, EVENT(ValeCave_TouchRidingLog) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_VALE_CAVE_EAST_CRATES_BANK, FLAG_VALE_CAVE_EAST_CRATES_SUNK, EVENT(ValeCave_PushEastCratesIntoWater) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_CAVE_KITE_RIDGE_TOP, FLAG_KITE, EVENT(ValeCave_KiteRunsOff) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_CAVE_KITE_RIDGE_FOOT, FLAG_KITE, EVENT(ValeCave_KiteRunsBehindRocks) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE, FLAG_KITE, EVENT(ValeCave_KiteKeepsAway) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE + 1, FLAG_KITE, EVENT(ValeCave_KiteKeepsAway) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE + 2, FLAG_KITE, EVENT(ValeCave_KiteKeepsAway) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE + 3, FLAG_KITE, EVENT(ValeCave_KiteKeepsAway) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE + 4, FLAG_KITE, EVENT(ValeCave_KiteKeepsAway) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE + 5, FLAG_KITE, EVENT(ValeCave_KiteKeepsAway) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE + 6, FLAG_KITE, EVENT(ValeCave_KiteKeepsAway) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE + 7, FLAG_KITE, EVENT(ValeCave_KiteKeepsAway) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE + 8, FLAG_KITE, EVENT(ValeCave_KiteKeepsAway) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE + 9, FLAG_KITE, EVENT(ValeCave_KiteKeepsAway) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE + 10, FLAG_KITE, EVENT(ValeCave_KiteKeepsAway) },
    { TRIGGER_STEP, CELL_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE + 11, FLAG_KITE, EVENT(ValeCave_KiteKeepsAway) },
    { TRIGGER_TALK_HALTED, ACTOR_KITE, CONDITION_ALWAYS, EVENT(ValeCave_TalkToKite) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_VALE_CAVE_HALT_GEM, FIND_ITEM(ITEM_HALT_GEM) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_VALE_CAVE_NUT, FIND_ITEM(ITEM_NUT) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(ValeCave_RevealHiddenTile) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(ValeCave_HideRevealedTile) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_CRATES, FLAG_VALE_CAVE_WEST_CRATES_SUNK, EVENT(ValeCave_MoveWestCratesIntoWater) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_CRATES, FLAG_VALE_CAVE_EAST_CRATES_SUNK, EVENT(ValeCave_MoveEastCratesIntoWater) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_KITE, CONDITION_ALWAYS, EVENT(ValeCave_HaltKite) },
    { TRIGGER_TABLE_END },
};
// clang-format on

struct Trigger gValeCave_LogsTriggers[] = {
    {TRIGGER_EXIT, VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_WEST, CONDITION_ALWAYS,
     VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_WEST},
    {TRIGGER_EXIT, VALE_CAVE_LOGS_NORTH_TO_SOUTH, CONDITION_ALWAYS,
     VALE_CAVE_LOGS_NORTH_TO_SOUTH},
    {TRIGGER_EXIT, VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_EAST, CONDITION_ALWAYS,
     VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_EAST},
    {TRIGGER_EXIT, VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_SOUTH, CONDITION_ALWAYS,
     VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_SOUTH},
    {TRIGGER_EXIT, VALE_CAVE_LOGS_SOUTH_TO_NORTH, CONDITION_ALWAYS,
     VALE_CAVE_LOGS_SOUTH_TO_NORTH},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     CELL_BEHAVIOR_VALE_CAVE_EAST_LOG_LEDGE, CONDITION_ALWAYS,
     EVENT(ValeCave_PushLogOverEdge)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST),
     CELL_BEHAVIOR_VALE_CAVE_WEST_LOG_LEDGE, CONDITION_ALWAYS,
     EVENT(ValeCave_PushLogOffLedge)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     CELL_BEHAVIOR_VALE_CAVE_SOUTH_LOG_FLOOR, CONDITION_ALWAYS,
     EVENT(ValeCave_PushLogNorth)},
    {TRIGGER_TABLE_END},
};
