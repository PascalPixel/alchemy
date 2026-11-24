/// @file
/// Mogall Forest (SceneId_MogallForestSign to SceneId_MogallForestSidePaths),
/// the endless woods between Fuchin Temple and Xian. Master Nyunpa teaches the
/// party Force to find the way through: apes hide in the great hollow purple
/// stumps, and Force on a stump brings its ape out to show the way on, or to
/// attack. Each area is a few screens of clearings joined by paths, and a
/// wrong path leads into the side paths and back.
///
/// - The first area (SceneId_MogallForestSign) is entered from the World Map
///   at the north of its first clearing. The first time, an ape peeks out of
///   the stump in the middle and dives back in
///   (FLAG_MOGALL_FOREST_FIRST_APE_SEEN). A broken sign by the path reads
///   `North: Fuchin Temple South: Mogall Prairie...` Force on the stump sends
///   its ape off south. Two clearings on, by the river, the stump on the west
///   of the south bank holds an ape that attacks. Logs on the south bank are
///   pushed about until the last one rolls into the river, a stepping place up
///   to the raised meadow north of it, where the ape in the big stump points
///   east to the second area. A Nut hangs in a tree on the river's east bank.
/// - The second area (SceneId_MogallForestQuartz). In the clearing of two
///   stumps the west one's ape leads south and the east one's east, to a pond
///   with an Apple in a chest. South, by the river, Move puts a rock into a
///   ring on the ground so that the log above it can be pushed on, and another
///   log rolls into the stream. The ape in the stump on the island in the
///   stream first leaps to a stump on the west bank, then leads west to the
///   third area. Quartz, a Venus Djinni, waits at the bottom of the clearing to
///   be fought.
/// - The third area (SceneId_MogallForestKillerApe). Of three stumps in a row
///   the east one's ape attacks, the middle one's leads south and the west
///   one's west, to an Elven Shirt in a chest that a rock moved with Move and
///   a log pushed east let the party reach. South, two logs on an island roll
///   into the river and a rock settles with Move, and the ape in the stump on
///   its own island leaps to a lower stump, then leads south. There a lone
///   stump stands on an island between two log bridges. Walking up to it, the
///   ape bursts out and runs off south over the bridge
///   (FLAG_MOGALL_FOREST_LED_TO_KILLER_APE). Following it, at the top of the
///   bridge the Killer Ape bursts out and attacks
///   (ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, slot 1), and coming back
///   from the battle at MOGALL_FOREST_KILLER_APE_AFTER_KILLER_APE_ATTACKS marks
///   it beaten (FLAG_MOGALL_FOREST_KILLER_APE_BEATEN). The way south leads out
///   to the World Map and Xian. Entering that clearing at
///   MOGALL_FOREST_KILLER_APE_BRIDGES_FROM_RIVER or
///   MOGALL_FOREST_KILLER_APE_FROM_WORLD_MAP once Force is learned and the
///   Killer Ape beaten sets FLAG_NYUNPA_ENLIGHTENED. Back at Fuchin Temple the
///   monks then say that Nyunpa has reached enlightenment and passed away.
/// - SceneId_MogallForestSidePaths, also named Mogall Forest: small clearings
///   that the wrong paths lead into, whose far sides lead back. A Psynergy
///   Stone lies there.
///
/// The apes that attack, one in the first area and one in the third, start a
/// battle with an Ape (ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, slot 0).
/// An ape stays gone once out, and logs pushed into the river stay there, for
/// as long as the party is in the forest (area flags from FLAG_AREA_TEMP_00).
///
/// One overlay serves all four scenes. Scene_GetEntrances,
/// _GetActorPlacements and _GetTriggers choose each scene's tables.
/// Scene_Initialize puts back the apes already out, the rolled logs and
/// the settled rocks, scene by scene and entrance by entrance.
///
/// The third area's east ape is also the ape at the lone stump and the Killer
/// Ape. The editions differ in how it is kept out of sight once it has come
/// out (the #if blocks below and in Scene_Initialize).
///
/// The apes and rocks are told apart by colour mode (enum SpriteColorMode). An
/// ape in hiding is SPRITE_COLOR_HIDDEN. A guide that comes out is drawn green,
/// its red and green swapped (SPRITE_COLOR_GRB), and an ape that attacks keeps
/// SPRITE_APE's own tan. The Killer Ape is the same sprite a fifth larger and
/// blue, its colours rotated (SPRITE_COLOR_BRG). The rocks Move pushes are
/// SPRITE_TURNING_ROCK drawn in greys (SPRITE_COLOR_GREY).

#include "ability_ids.h"
#include "djinn_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor.h"
#include "field/event.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/pushed_actor.h"
#include "field/scene_tables.h"
#include "field/script_operands.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/actor_orbit.h"
#include "scene/catch_nut.h"
#include "scene/field_effect_spawn.h"
#include "sound/sound.h"
#include "system/heap_slots.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "system/system_call.h"
#include "text/msg_ids.h"

extern s32 gMogallForest_ActorScript[];
extern struct SceneEntrance gMogallForestSign_Entrances[];
extern struct SceneEntrance gMogallForestQuartz_Entrances[];
extern struct SceneEntrance gMogallForestKillerApe_Entrances[];
extern struct SceneEntrance gMogallForestSidePaths_Entrances[];
extern u32 gMogallForest_Exits[];
extern struct ActorPlacement gMogallForestSign_ActorPlacements[];
extern struct ActorPlacement gMogallForestQuartz_ActorPlacements[];
extern struct ActorPlacement gMogallForestKillerApe_ActorPlacements[];
extern struct ActorPlacement gMogallForestSidePaths_ActorPlacements[];
extern struct Trigger gMogallForestSign_Triggers[];
extern struct Trigger gMogallForestQuartz_Triggers[];
extern struct Trigger gMogallForestKillerApe_Triggers[];
extern struct Trigger gMogallForestSidePaths_Triggers[];

/// Actors of the first area (SceneId_MogallForestSign), numbered in the
/// order of gMogallForestSign_ActorPlacements.
enum MogallForestSignActorId {
  /// A log lying north to south at (488, 464), pushed as the river log is but
  /// with nothing happening where it stops.
  ACTOR_FIRST_AREA_LOG = ACTOR_FIRST_PLACED,
  /// The log lying west to east on the river's south bank, which rolls into
  /// the river.
  ACTOR_FIRST_AREA_RIVER_LOG,
  /// The ape in the stump in the middle of the clearing entered from the World
  /// Map, the one that peeks out on the first arrival.
  ACTOR_FIRST_AREA_NORTH_APE,
  /// The ape in the stump on the west of the river's south bank.
  ACTOR_FIRST_AREA_ATTACKING_APE,
  /// The ape in the big stump on the raised meadow north of the river.
  ACTOR_FIRST_AREA_EAST_APE,
  /// The stumps themselves: blank actors on the apes' spots, the targets of
  /// Force.
  ACTOR_FIRST_AREA_NORTH_APE_STUMP,
  ACTOR_FIRST_AREA_ATTACKING_APE_STUMP,
  ACTOR_FIRST_AREA_EAST_APE_STUMP,
  /// A Nut in a tree on the river's east bank, caught with Catch.
  ACTOR_NUT
};

/// Actors of the second area (SceneId_MogallForestQuartz), numbered in
/// the order of gMogallForestQuartz_ActorPlacements.
enum MogallForestQuartzActorId {
  /// The upper log lying west to east by the river, held by the rock below it.
  ACTOR_SECOND_AREA_LOG = ACTOR_FIRST_PLACED,
  /// A log lying west to east at (592, 456), south of ACTOR_SECOND_AREA_LOG,
  /// pushed with nothing happening where it stops.
  ACTOR_SECOND_AREA_SOUTH_LOG,
  /// The log lying north to south by the stream, which rolls into it.
  ACTOR_SECOND_AREA_RIVER_LOG,
  /// The small rock below the upper log, moved with Move.
  ///
  /// It is drawn as the rocks of Altmiller Cave's color puzzle
  /// (MsgAltmillerRotatedRock), drawn plain as those are while Reveal is off.
  ACTOR_SECOND_AREA_ROCK,
  /// The apes in the two stumps side by side in the clearing of stumps.
  ACTOR_SECOND_AREA_WEST_APE,
  ACTOR_SECOND_AREA_EAST_APE,
  /// The ape in the stump on the island in the stream, which moves to a stump
  /// on the stream's west bank.
  ACTOR_SECOND_AREA_SOUTH_APE,
  ACTOR_SECOND_AREA_WEST_APE_STUMP,
  ACTOR_SECOND_AREA_EAST_APE_STUMP,
  ACTOR_SECOND_AREA_SOUTH_APE_STUMP,
  /// Quartz, a Venus Djinni, at the bottom of the river clearing until the
  /// party has it.
  ACTOR_SECOND_AREA_QUARTZ
};

/// Actors of the third area (SceneId_MogallForestKillerApe), numbered in
/// the order of gMogallForestKillerApe_ActorPlacements.
enum MogallForestKillerApeActorId {
  /// The log lying north to south beside the Elven Shirt's chest.
  ACTOR_THIRD_AREA_LOG = ACTOR_FIRST_PLACED,
  /// The upper of the two logs lying north to south on the island in the
  /// river, which roll east into it.
  ACTOR_THIRD_AREA_NORTH_RIVER_LOG,
  /// A log lying north to south at (552, 480), west of the two river logs,
  /// pushed with nothing happening where it stops.
  ACTOR_THIRD_AREA_WEST_LOG,
  ACTOR_THIRD_AREA_SOUTH_RIVER_LOG,
  /// A log lying west to east at (544, 536), south of
  /// ACTOR_THIRD_AREA_WEST_LOG, pushed with nothing happening where it stops.
  ACTOR_THIRD_AREA_SOUTHWEST_LOG,
  /// Two rocks moved with Move, as in the second area: the one beside the
  /// Elven Shirt's log and the one beside the upper log on the river island.
  ACTOR_THIRD_AREA_NORTH_ROCK,
  ACTOR_THIRD_AREA_SOUTH_ROCK,
  /// The apes in the three stumps in a row: the east one, the middle and the
  /// west.
  ACTOR_THIRD_AREA_ATTACKING_APE,
  ACTOR_THIRD_AREA_MIDDLE_APE,
  ACTOR_THIRD_AREA_WEST_APE,
  /// The ape in the stump on its own island in the river clearing, which moves
  /// to a lower stump. It is also the ape at the lone stump and the Killer Ape.
  ACTOR_THIRD_AREA_EAST_APE,
  ACTOR_THIRD_AREA_ATTACKING_APE_STUMP,
  ACTOR_THIRD_AREA_MIDDLE_APE_STUMP,
  ACTOR_THIRD_AREA_WEST_APE_STUMP,
  ACTOR_THIRD_AREA_EAST_APE_STUMP
};

/// The animations of a river log's sprite (SPRITE_LOG_NORTH_SOUTH and
/// SPRITE_LOG_WEST_EAST), named by when these scenes play them.
enum MogallForestLogAnimation {
  MOGALL_FOREST_LOG_ROLLING = 3,    ///< as it rolls over the bank
  MOGALL_FOREST_LOG_IN_RIVER = 4,   ///< lying in the river, set on arrival
  MOGALL_FOREST_LOG_STEPPED_ON = 7, ///< as the leader jumps onto it
  MOGALL_FOREST_LOG_FALLING_IN = 8  ///< as it drops into the water
};

/// Kept until the party leaves the forest.
///
/// There is one for each ape once it has come out (or, for two, moved on to a
/// second hiding place) and one for each rolled log.
enum {
  FLAG_MOGALL_FOREST_FIRST_AREA_NORTH_APE_OUT = FLAG_AREA_TEMP_00,
  /// The ape that leaps at the leader instead of pointing the way.
  FLAG_MOGALL_FOREST_FIRST_AREA_ATTACKING_APE_OUT,
  FLAG_MOGALL_FOREST_FIRST_AREA_EAST_APE_OUT,
  FLAG_MOGALL_FOREST_SECOND_AREA_WEST_APE_OUT,
  FLAG_MOGALL_FOREST_SECOND_AREA_EAST_APE_OUT,
  FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_MOVED,
  FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_OUT,
  /// The ape that leaps at the leader instead of pointing the way.
  FLAG_MOGALL_FOREST_THIRD_AREA_ATTACKING_APE_OUT,
  FLAG_MOGALL_FOREST_THIRD_AREA_MIDDLE_APE_OUT,
  FLAG_MOGALL_FOREST_THIRD_AREA_WEST_APE_OUT,
  FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_MOVED,
  FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT,
  FLAG_MOGALL_FOREST_FIRST_AREA_LOG_ROLLED = FLAG_AREA_TEMP_10,
  FLAG_MOGALL_FOREST_SECOND_AREA_LOG_ROLLED,
  FLAG_MOGALL_FOREST_THIRD_AREA_NORTH_LOG_ROLLED,
  FLAG_MOGALL_FOREST_THIRD_AREA_SOUTH_LOG_ROLLED,
  FLAG_MOGALL_FOREST_PSYNERGY_STONE = FLAG_AREA_TEMP_50
};

/// The cell behaviors the trigger tables answer.
enum MogallForestCellBehavior {
  /// Under each log the party pushes by walking into it.
  CELL_BEHAVIOR_MOGALL_FOREST_LOG = 20,
  /// Beside the lone stump on the island between the two log bridges, cells
  /// (12, 22) and (12, 23).
  CELL_BEHAVIOR_MOGALL_FOREST_BESIDE_LONE_STUMP = 22,
  /// At the top of the south log bridge below the lone stump, cell (8, 27).
  CELL_BEHAVIOR_MOGALL_FOREST_BELOW_LONE_STUMP = 23,
  /// Where walking on jumps across water: from the first area's river log up
  /// to the meadow north of it, and onto the third area's lower river log
  /// from the bank east of it.
  CELL_BEHAVIOR_MOGALL_FOREST_JUMP = 25,
  /// The edge of the first area's meadow above the river log, where walking
  /// south jumps down onto it.
  CELL_BEHAVIOR_MOGALL_FOREST_JUMP_SOUTH = 26,
  /// The broken sign at the first clearing, cells (9, 3) and (9, 4).
  CELL_BEHAVIOR_MOGALL_FOREST_BROKEN_SIGN = 27
};

/// Scene flag, kept while the party stays in the area.
enum { FLAG_MOGALL_FOREST_SOUTH_ROCK_SETTLED = FLAG_SCENE_TEMP_14 };

/// The third area's east ape: in the first patch it is placed only until it
/// has come out, and from the first patch on the hiding place that shares its
/// sprite too.
#if TBS_PATCH_LEVEL == 1
#define MOGALL_FOREST_EAST_ACTOR_CONDITION                                     \
  FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT
#else
#define MOGALL_FOREST_EAST_ACTOR_CONDITION CONDITION_ALWAYS
#endif

/// The hiding place that shares the ape's sprite: always placed before patch 1,
/// from it only until the ape has come out.
#if !TBS_PATCH_1
#define MOGALL_FOREST_EAST_SHARED_SPRITE_CONDITION CONDITION_ALWAYS
#else
#define MOGALL_FOREST_EAST_SHARED_SPRITE_CONDITION                             \
  FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT
#endif

#include "shared/staged_actor.c"

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

/// Talking to Quartz, the Venus Djinni at the bottom of the second area's river
/// clearing, starts the battle with it.
static void MogallForest_StartDjinnBattle(void) {
  Field_RequestDjinnBattle(ACTOR_SECOND_AREA_QUARTZ, DJINNI_QUARTZ);
}

/// The leader springs and leaps to a spot (in 16.16 pixels). Returns 0, and
/// does nothing, when the spot is blocked.
static s32 MogallForest_TryJumpLeaderTo(struct FieldPosition *position) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  s32 savedMotionFlags = leader->motionFlags;
  s32 collision = FieldActor_CheckMovementCollision(leader, position);

  if (collision == 0) {
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
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, FIXEDtoINT(position->x),
                        FIXEDtoINT(position->z));
    Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
    leader->motionFlags = savedMotionFlags;
    Event_End();
    return 1;
  }
  return 0;
}

/// An actor script call: draws the actor in the colour mode its timer holds.
static s32 MogallForest_SetColorFromTimer(struct Actor *actor) {
  Actor_SetSpriteColor(actor, actor->timer);
  return 0;
}

/// Clears the actor's sprite flags when its script requests it.
static s32 MogallForest_ClearActorSpriteFlags(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

/// Hops an actor to a destination in whole pixels with a vertical speed in
/// 16.16 fixed point; scenes call it through MogallForest_HopActorTo.
static void MogallForest_HopActorToWithSpeedFX(s32 actorId, s32 targetX,
                                               s32 targetZ,
                                               FIXED verticalSpeed) {
  struct Actor *actor;

  actor = Actor_Get(actorId);
  Actor_SetSpritePriority(actorId, 1);
  Actor_SetSpeed(actorId, 3);
  Sound_Play(SE_JUMP);
  actor->velocityY = verticalSpeed;
  actor->gravity = toFIXED(1) / 2;
  actor->bounce = 0;
  Actor_SetSpriteFlags(actor, 0);
  Actor_MoveToAndWait(actorId, targetX, targetZ);
  Actor_SetPosition(actorId, targetX, targetZ);
  Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  actor->gravity = toFIXED(1);
}

/// Hops an actor with a vertical speed in pixels a frame:
/// `MogallForest_HopActorTo(ACTOR_FIRST_AREA_NORTH_APE, 88, 120, 6)`.
#define MogallForest_HopActorTo(actorId, targetX, targetZ, verticalSpeed)      \
  MogallForest_HopActorToWithSpeedFX(actorId, targetX, targetZ,                \
                                     toFIXED(verticalSpeed))

/// With a thump, scatters a ring of puffs of dust round an actor: the puff an
/// ape makes popping out of a stump or diving back in.
static void MogallForest_SpawnPuffRing(s32 id) {
  struct Vector3 velocity;
  struct EffectOptions options;
  struct Actor *actor;
  u32 direction;

  actor = Actor_Get(id);
  Sound_Play(SE_THUMP);
  options.priority = 1;
  // Seventeen puffs a sixteenth of a turn apart, the first and last together,
  // spread a third wider east to west.
  for (direction = 0; direction <= TRIG_TURN / FACING_STEP; direction++) {
    velocity.x = Trig_Cos(direction * FACING_STEP);
    velocity.y = 0;
    velocity.z = Trig_Sin(direction * FACING_STEP);
    velocity.x += velocity.x / 3;
    FieldEffect_Spawn(actor->x.fixed, toFIXED(16), actor->z.fixed, velocity.x,
                      velocity.y + toFIXED(1) / 10, velocity.z,
                      EFFECT_USE_PRIORITY, &options);
  }
}

/// Pans the camera to an ape's stump and, half a second later, brings the ape
/// out in a puff of dust, drawn in the colour mode given.
static void MogallForest_ApeComesOut(s32 actorId, s32 color) {
  FieldCamera_SetSpeedParameters(toFIXED(2), toFIXED(1) / 4);
  FieldCamera_MoveToActorById(actorId, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(30);
  MogallForest_SpawnPuffRing(actorId);
  Actor_SetSpriteColorById(actorId, color);
}

/// The ape turns to the leader, hops on the spot and gestures the way on.
static inline void MogallForest_ApePointsTheWay(s32 guideId) {
  FieldCamera_FollowActor(guideId, 1);
  Actor_TurnToFaceEachOther(guideId, ACTOR_PARTY_LEADER, 0);
  Event_Wait(20);
  Actor_StartRepeatedMotion(guideId, 2);
  Actor_SetAttachedEffect(guideId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_MOGALL_FOREST_SIGN) {
    return gMogallForestSign_Entrances;
  }
  if (scene == SCENE_MOGALL_FOREST_QUARTZ) {
    return gMogallForestQuartz_Entrances;
  }
  if (scene == SCENE_MOGALL_FOREST_KILLER_APE) {
    return gMogallForestKillerApe_Entrances;
  }
  return gMogallForestSidePaths_Entrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

/// Returns the exits, one table for all four scenes.
const u32 *Scene_GetExits(void) { return gMogallForest_Exits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_MOGALL_FOREST_SIGN) {
    return gMogallForestSign_ActorPlacements;
  }
  if (scene == SCENE_MOGALL_FOREST_QUARTZ) {
    return gMogallForestQuartz_ActorPlacements;
  }
  if (scene == SCENE_MOGALL_FOREST_KILLER_APE) {
    return gMogallForestKillerApe_ActorPlacements;
  }
  return gMogallForestSidePaths_ActorPlacements;
}

/// The map rows and columns where a pushed log reaches the water and rolls in,
/// or is out of the way.
enum {
  /// The first area's river log, at the river's south bank.
  FIRST_AREA_RIVER_BANK_ROW = 26,
  /// The second area's upper log, pushed a cell south into the place of the
  /// rock below it.
  SECOND_AREA_LOG_OPENS_WAY_ROW = 23,
  /// The second area's river log, at the stream's east bank.
  SECOND_AREA_RIVER_BANK_COLUMN = 35,
  /// The third area's upper river log, at the island's east bank.
  THIRD_AREA_NORTH_RIVER_BANK_COLUMN = 42,
  /// The third area's lower river log, at the island's east bank.
  THIRD_AREA_SOUTH_RIVER_BANK_COLUMN = 40
};

/// Isaac pushes a log on the first area's river bank a cell by walking into
/// it. The log lying west to east, pushed north to the river (row 26), rolls
/// in and lies there, a stepping place up to the meadow on the far side.
///
/// Rolling in, the log gives the cells (31, 25) to (34, 26) the jump north on
/// their north row, copied from a spare stretch of the map at (38, 27).
static void MogallForest_FirstArea_PushLog(void) {
  struct PushedActorProbe probe;

  Event_Start();
  if (PushedActor_FindClearPosition(&probe) != 0) {
    Actor_MoveAndRedraw(probe);
    if (probe.actorSlot == ACTOR_FIRST_AREA_RIVER_LOG &&
        FIELD_CELL(probe.position.z) == FIRST_AREA_RIVER_BANK_ROW) {
      GameFlag_Set(FLAG_MOGALL_FOREST_FIRST_AREA_LOG_ROLLED);
      Actor_SetAnimation(ACTOR_FIRST_AREA_RIVER_LOG, MOGALL_FOREST_LOG_ROLLING);
      Actor_SetSpeedAndAcceleration(ACTOR_FIRST_AREA_RIVER_LOG, 0.25, 0.5);
      Actor_SetDestinationOffsetById(ACTOR_FIRST_AREA_RIVER_LOG, 0, -16);
      Event_Wait(45);
      Actor_SetAnimation(ACTOR_FIRST_AREA_RIVER_LOG,
                         MOGALL_FOREST_LOG_FALLING_IN);
      Sound_Play(SE_LOG_FALL);
      Actor_SetSpritePriority(ACTOR_FIRST_AREA_RIVER_LOG, 1);
      Actor_Get(ACTOR_FIRST_AREA_RIVER_LOG)->priorityFlags =
          ACTOR_PRIORITY_UNDERFOOT;
      MapGrid_CopyCellAttributes(38, 27, 4, 2, 31, 25);
    }
  }
  Event_End();
}

/// Walking north along the log lying in the first area's river (cells (31, 25)
/// to (34, 25)) makes Isaac leap up onto the raised meadow north of it, by the
/// big stump.
static void MogallForest_FirstArea_JumpNorth(void) {
  struct FieldPosition position;
#if TBS_PATCH_1
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

  position.x = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  position.y = leader->y.fixed;
  position.z = (leader->z.fixed & FIELD_CELL_MASK) - toFIXED(24);
#else
  position.x = Actor_Get(ACTOR_PARTY_LEADER)->x.fixed;
  position.y = Actor_Get(ACTOR_PARTY_LEADER)->y.fixed;
  position.z = Actor_Get(ACTOR_PARTY_LEADER)->z.fixed - toFIXED(32);
#endif
  MogallForest_TryJumpLeaderTo(&position);
}

/// Walking south off the edge of the first area's raised meadow (cells (31, 23)
/// to (34, 23)) makes Isaac leap down onto the log lying in the river, which
/// dips under him.
///
/// The ground and the leader sink a pixel, then another, and rise back.
static void MogallForest_FirstArea_JumpSouth(void) {
  struct FieldPosition position;
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  s32 savedMotionFlags = leader->motionFlags;

#if TBS_PATCH_1
  position.x = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  position.y = leader->y.fixed;
  position.z = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(40);
#else
  position.x = Actor_Get(ACTOR_PARTY_LEADER)->x.fixed;
  position.y = Actor_Get(ACTOR_PARTY_LEADER)->y.fixed;
  position.z = Actor_Get(ACTOR_PARTY_LEADER)->z.fixed + toFIXED(32);
#endif
  if (MogallForest_TryJumpLeaderTo(&position) != 0) {
    Event_Start();
    leader->motionFlags = 0;
    Actor_SetAnimation(ACTOR_FIRST_AREA_RIVER_LOG,
                       MOGALL_FOREST_LOG_STEPPED_ON);
    leader->y.fixed -= toFIXED(1);
    leader->groundHeight -= toFIXED(1);
    wait(2);
    leader->y.fixed -= toFIXED(1);
    leader->groundHeight -= toFIXED(1);
    wait(10);
    leader->y.fixed += toFIXED(1);
    leader->groundHeight += toFIXED(1);
    wait(4);
    leader->y.fixed += toFIXED(1);
    leader->groundHeight += toFIXED(1);
    leader->motionFlags = savedMotionFlags;
    Event_End();
  }
}

/// Isaac reads the broken sign by the path just inside the forest, a broken
/// post a step from where he arrives from the World Map (cells (9, 3) and
/// (9, 4)):
/// `The broken sign reads: North: Fuchin Temple South: Mogall Prairie...`
static void MogallForest_ReadBrokenSign(void) {
  Event_Start();
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  FieldMessage_Show(MSGNO(MsgMogallForestBrokenSign), 1);
  Event_End();
}

/// Force on the stump in the middle of the first clearing brings a green ape
/// out of it. The ape turns to Isaac, points the way, and hops off south, the
/// way on to the river.
static void MogallForest_FirstArea_NorthApeShowsWay(void) {
  struct Actor *ape;

  ape = Actor_Get(ACTOR_FIRST_AREA_NORTH_APE);

  Event_Start();

  MogallForest_ApeComesOut(ACTOR_FIRST_AREA_NORTH_APE, SPRITE_COLOR_GRB);
  MogallForest_HopActorTo(ACTOR_FIRST_AREA_NORTH_APE, 88, 120, 6);

  FieldEffect_Spawn(ape->x.fixed, ape->y.fixed, ape->z.fixed + toFIXED(24), 0,
                    0, 0, EFFECT_SCRIPT_MEDIUM, 0);

  MogallForest_ApePointsTheWay(ACTOR_FIRST_AREA_NORTH_APE);

  MogallForest_HopActorTo(ACTOR_FIRST_AREA_NORTH_APE, 88, 152, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_NORTH_APE, 0);
  Event_Wait(10);

  MogallForest_HopActorTo(ACTOR_FIRST_AREA_NORTH_APE, 120, 192, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_NORTH_APE, 0);
  Event_Wait(10);

  MogallForest_HopActorTo(ACTOR_FIRST_AREA_NORTH_APE, 120, 240, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_NORTH_APE, 0);
  Event_Wait(10);

  GameFlag_Set(FLAG_MOGALL_FOREST_FIRST_AREA_NORTH_APE_OUT);
  Actor_SetPosition(ACTOR_FIRST_AREA_NORTH_APE_STUMP, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_AREA_NORTH_APE, 0, 0);

  Event_End();
}

/// Force on the stump on the west of the river's south bank, in the first area,
/// brings out an ape that cries out in anger, leaps at Isaac and starts a
/// battle with it.
static void MogallForest_FirstArea_ApeAttacks(void) {
  struct Actor *ape;

  ape = Actor_Get(ACTOR_FIRST_AREA_ATTACKING_APE);

  Event_Start();

  MogallForest_ApeComesOut(ACTOR_FIRST_AREA_ATTACKING_APE, SPRITE_COLOR_NORMAL);
  MogallForest_HopActorTo(ACTOR_FIRST_AREA_ATTACKING_APE, 408, 456, 6);

  FieldEffect_Spawn(ape->x.fixed, ape->y.fixed, ape->z.fixed + toFIXED(24), 0,
                    0, 0, EFFECT_SCRIPT_MEDIUM, 0);

  FieldCamera_FollowActor(ACTOR_FIRST_AREA_ATTACKING_APE, 1);
  Actor_TurnToFaceEachOther(ACTOR_FIRST_AREA_ATTACKING_APE, ACTOR_PARTY_LEADER,
                            0);
  Event_Wait(30);
  Actor_StartRepeatedMotion(ACTOR_FIRST_AREA_ATTACKING_APE, 2);
  Actor_ShowEmote(ACTOR_FIRST_AREA_ATTACKING_APE,
                  EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Sound_Play(SE_BEAST_CRY);
  Event_Wait(60);

  MogallForest_HopActorTo(ACTOR_FIRST_AREA_ATTACKING_APE,
                          Actor_Get(ACTOR_PARTY_LEADER)->x.part[0].integer,
                          Actor_Get(ACTOR_PARTY_LEADER)->z.part[0].integer, 4);

  Event_Wait(10);
  GameFlag_Set(FLAG_MOGALL_FOREST_FIRST_AREA_ATTACKING_APE_OUT);
  Actor_SetPosition(ACTOR_FIRST_AREA_ATTACKING_APE_STUMP, 0, 0);

  Battle_SetType(BATTLE_TYPE_SCRIPTED);

  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 0);

  Event_End();
}

/// Force on the big stump on the meadow north of the first area's river brings
/// a green ape out of it. The ape points the way and hops off east, the way to
/// the second area.
static void MogallForest_FirstArea_EastApeShowsWay(void) {
  struct Actor *ape;

  ape = Actor_Get(ACTOR_FIRST_AREA_EAST_APE);

  Event_Start();

  MogallForest_ApeComesOut(ACTOR_FIRST_AREA_EAST_APE, SPRITE_COLOR_GRB);
  MogallForest_HopActorTo(ACTOR_FIRST_AREA_EAST_APE, 536, 344, 7);

  FieldEffect_Spawn(ape->x.fixed, ape->y.fixed, ape->z.fixed + toFIXED(16), 0,
                    0, 0, EFFECT_SCRIPT_MEDIUM, 0);

  MogallForest_ApePointsTheWay(ACTOR_FIRST_AREA_EAST_APE);

  MogallForest_HopActorTo(ACTOR_FIRST_AREA_EAST_APE, 584, 344, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_EAST_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_FIRST_AREA_EAST_APE, 632, 344, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_EAST_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_FIRST_AREA_EAST_APE, 680, 344, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_EAST_APE, 0);
  Event_Wait(6);

  GameFlag_Set(FLAG_MOGALL_FOREST_FIRST_AREA_EAST_APE_OUT);
  Actor_SetPosition(ACTOR_FIRST_AREA_EAST_APE_STUMP, 0, 0);

  Event_End();
}

/// Catch on the Nut in the tree on the east bank of the first area's river
/// brings it down to Isaac: `Isaac got a Nut.`
static void MogallForest_CatchNut(void) {
  Scene_CatchNut(ACTOR_NUT, FLAG_MOGALL_FOREST_NUT_CAUGHT);
}

/// Isaac pushes a log in the second area's river clearing a cell by walking
/// into it. Once Move has put the rock below it into its ring, the upper log
/// can go a cell south and the way on is open. The log by the stream, pushed
/// west to its bank, rolls into the stream and lies there as a way across.
///
/// The upper log's push copies the cells (35, 67) to (38, 67) onto the row
/// below them. The rolled log copies its cells in the stream from a spare
/// stretch of the map at (44, 30).
static void MogallForest_SecondArea_PushLog(void) {
  struct PushedActorProbe probe;
  s32 destinationColumn;
  s32 destinationRow;

  Event_Start();

  if (PushedActor_FindClearPosition(&probe) != 0) {
    Actor_MoveAndRedraw(probe);

    if (probe.actorSlot == ACTOR_SECOND_AREA_LOG &&
        (FIELD_CELL(probe.position.z)) == SECOND_AREA_LOG_OPENS_WAY_ROW) {
      destinationColumn = 35;
      destinationRow = 68;
      MapGrid_CopyCellAttributes(35, 67, 4, 1, destinationColumn,
                                 destinationRow);
    } else if (probe.actorSlot == ACTOR_SECOND_AREA_RIVER_LOG &&
               (FIELD_CELL(probe.position.x)) ==
                   SECOND_AREA_RIVER_BANK_COLUMN) {
      GameFlag_Set(FLAG_MOGALL_FOREST_SECOND_AREA_LOG_ROLLED);
      Actor_SetAnimation(ACTOR_SECOND_AREA_RIVER_LOG,
                         MOGALL_FOREST_LOG_ROLLING);
      Actor_SetDestinationOffsetById(ACTOR_SECOND_AREA_RIVER_LOG, -16, 6);
      Event_Wait(30);
      Actor_SetAnimation(ACTOR_SECOND_AREA_RIVER_LOG,
                         MOGALL_FOREST_LOG_FALLING_IN);
      Sound_Play(SE_LOG_FALL);

      Actor_Get(ACTOR_SECOND_AREA_RIVER_LOG)->priorityFlags =
          ACTOR_PRIORITY_UNDERFOOT;

      destinationColumn = 34;
      destinationRow = 30;
      MapGrid_CopyCellAttributes(44, 30, 2, 4, destinationColumn,
                                 destinationRow);
      PushedActor_FillGridAttributeRectangle(2, 35, 30, 1, 4, 0);
    }
  }

  Event_End();
}

/// Makes the rock's new cell solid and clears the four cells round it, one of
/// which it came from.
static inline void MogallForest_UpdateRockCells(s32 column, s32 row) {
  PushedActor_FillGridAttributeRectangle(2, column, row, 1, 1,
                                         CELL_BEHAVIOR_WALL);
  PushedActor_FillGridAttributeRectangle(2, column + 1, row, 1, 1,
                                         CELL_BEHAVIOR_NORMAL);
  PushedActor_FillGridAttributeRectangle(2, column - 1, row, 1, 1,
                                         CELL_BEHAVIOR_NORMAL);
  PushedActor_FillGridAttributeRectangle(2, column, row + 1, 1, 1,
                                         CELL_BEHAVIOR_NORMAL);
  PushedActor_FillGridAttributeRectangle(2, column, row - 1, 1, 1,
                                         CELL_BEHAVIOR_NORMAL);
}

/// Move on the small grey rock below the upper log in the second area's river
/// clearing pushes it a cell. Pushed south into the ring on the ground (cell
/// (36, 24)), it sinks 2 pixels into place and stays, and the log can be
/// pushed.
static void MogallForest_SecondArea_UpdateRock(void) {
  s32 column;
  s32 row;

  Event_Start();

  column = FIELD_CELL(Actor_Get(ACTOR_SECOND_AREA_ROCK)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_SECOND_AREA_ROCK)->z.fixed);

  MogallForest_UpdateRockCells(column, row);

  if (column == 36 && row == 24) {
    struct Actor *rock = Actor_Get(ACTOR_SECOND_AREA_ROCK);

    rock->motionFlags = 0;
    rock->groundHeight = -toFIXED(2);
    rock->y.fixed = -toFIXED(2);
  }

  Event_End();
}

/// Force on the west one of the two stumps side by side in the second area
/// brings a green ape out of it. The ape points the way and hops off south,
/// the way on toward the river.
static void MogallForest_SecondArea_WestApeShowsWay(void) {
  struct Actor *ape;

  ape = Actor_Get(ACTOR_SECOND_AREA_WEST_APE);
  Event_Start();
  MogallForest_ApeComesOut(ACTOR_SECOND_AREA_WEST_APE, SPRITE_COLOR_GRB);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_WEST_APE, 392, 104, 7);
  Event_Wait(10);
  FieldEffect_Spawn(ape->x.fixed, ape->y.fixed, (ape->z.fixed + toFIXED(4)), 0,
                    0, 0, EFFECT_SCRIPT_MEDIUM, 0);
  MogallForest_ApePointsTheWay(ACTOR_SECOND_AREA_WEST_APE);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_WEST_APE, 424, 120, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_WEST_APE, 0);
  Event_Wait(6);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_WEST_APE, 424, 168, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_WEST_APE, 0);
  Event_Wait(6);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_WEST_APE, 424, 208, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_WEST_APE, 0);
  Event_Wait(6);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_WEST_APE, 424, 232, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_WEST_APE, 0);
  Event_Wait(6);
  Actor_SetPosition(ACTOR_SECOND_AREA_WEST_APE, 0, 0);
  GameFlag_Set(FLAG_MOGALL_FOREST_SECOND_AREA_WEST_APE_OUT);
  Actor_SetPosition(ACTOR_SECOND_AREA_WEST_APE_STUMP, 0, 0);
  Event_End();
}

/// Force on the east one of the two stumps side by side in the second area
/// brings a green ape out of it. The ape points the way and hops off east,
/// toward the pond with the Apple's chest.
static void MogallForest_SecondArea_EastApeShowsWay(void) {
  struct Actor *ape;

  ape = Actor_Get(ACTOR_SECOND_AREA_EAST_APE);

  Event_Start();

  MogallForest_ApeComesOut(ACTOR_SECOND_AREA_EAST_APE, SPRITE_COLOR_GRB);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_EAST_APE, 456, 104, 7);
  Event_Wait(10);

  FieldEffect_Spawn(ape->x.fixed, ape->y.fixed, ape->z.fixed + toFIXED(4), 0, 0,
                    0, EFFECT_SCRIPT_MEDIUM, 0);

  MogallForest_ApePointsTheWay(ACTOR_SECOND_AREA_EAST_APE);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_EAST_APE, 472, 136, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_EAST_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_EAST_APE, 504, 136, 3.2);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_EAST_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_EAST_APE, 552, 136, 3.5);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_EAST_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_EAST_APE, 584, 136, 3.5);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_EAST_APE, 0);
  Event_Wait(6);

  Actor_SetPosition(ACTOR_SECOND_AREA_EAST_APE, 0, 0);
  GameFlag_Set(FLAG_MOGALL_FOREST_SECOND_AREA_EAST_APE_OUT);
  Actor_SetPosition(ACTOR_SECOND_AREA_EAST_APE_STUMP, 0, 0);

  Event_End();
}

/// Force on the stump on the island in the second area's stream brings a green
/// ape out, which leaps west across the stream into the stump on the bank and
/// hides again.
static void MogallForest_SecondArea_SouthApeHidesAgain(void) {
  Event_Start();
  MogallForest_ApeComesOut(ACTOR_SECOND_AREA_SOUTH_APE, SPRITE_COLOR_GRB);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 424, 480, 7.6);
  Event_Wait(2);
  MogallForest_SpawnPuffRing(ACTOR_SECOND_AREA_SOUTH_APE);
  Actor_SetSpriteColorById(ACTOR_SECOND_AREA_SOUTH_APE, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SECOND_AREA_SOUTH_APE), 0);
  Event_Wait(30);
  GameFlag_Set(FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_MOVED);
  Actor_SetPosition(ACTOR_SECOND_AREA_SOUTH_APE_STUMP, 424, 480);
  Event_End();
}

/// Force on the stump on the west bank of the second area's stream, where the
/// ape from the island hid, brings it out again. It points the way and hops off
/// west, the way to the third area.
static void MogallForest_SecondArea_SouthApeShowsWay(void) {
  struct Actor *ape;

  ape = Actor_Get(ACTOR_SECOND_AREA_SOUTH_APE);

  Event_Start();

  MogallForest_ApeComesOut(ACTOR_SECOND_AREA_SOUTH_APE, SPRITE_COLOR_GRB);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 392, 504, 6);
  Event_Wait(10);

  FieldEffect_Spawn(ape->x.fixed, ape->y.fixed, ape->z.fixed + toFIXED(4), 0, 0,
                    0, EFFECT_SCRIPT_MEDIUM, 0);

  MogallForest_ApePointsTheWay(ACTOR_SECOND_AREA_SOUTH_APE);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 360, 528, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_SOUTH_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 328, 528, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_SOUTH_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 288, 528, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_SOUTH_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 256, 528, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_SOUTH_APE, 0);
  Event_Wait(6);

  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  Actor_SetPosition(ACTOR_SECOND_AREA_SOUTH_APE, 0, 0);
  Event_Wait(30);
  GameFlag_Set(FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_OUT);
  Actor_SetPosition(ACTOR_SECOND_AREA_SOUTH_APE_STUMP, 0, 0);

  Event_End();
}

/// Isaac pushes a log in the third area a cell by walking into it: the log by
/// the Elven Shirt's chest, or the logs on the island in the river clearing.
/// The two lying north to south on the island, pushed east to its bank, roll
/// into the river and lie there as ways across.
///
/// At every push the chest's log and the upper river log copy cells onto
/// their new column from spare stretches of the map, in rows 49 to 52 and 68
/// to 71. A rolled log copies its cells in the river from (26, 20).
static void MogallForest_ThirdArea_PushLog(void) {
  struct PushedActorProbe probe;
  s32 landed;

  Event_Start();
  landed = 0;
  if (PushedActor_FindClearPosition(&probe) != 0) {
    Actor_MoveAndRedraw(probe);
    if (probe.actorSlot == ACTOR_THIRD_AREA_NORTH_RIVER_LOG ||
        probe.actorSlot == ACTOR_THIRD_AREA_SOUTH_RIVER_LOG) {
      if (probe.actorSlot == ACTOR_THIRD_AREA_NORTH_RIVER_LOG) {
        MapGrid_CopyCellAttributes(38, 68, 1, 4, FIELD_CELL(probe.position.x),
                                   68);
        if (FIELD_CELL(probe.position.x) ==
            THIRD_AREA_NORTH_RIVER_BANK_COLUMN) {
          MapGrid_CopyCellAttributes(26, 20, 2, 4, FIELD_CELL(probe.position.x),
                                     23);
          Actor_SetSpritePriority(ACTOR_THIRD_AREA_NORTH_RIVER_LOG, 1);
          landed = 1;
          GameFlag_Set(FLAG_MOGALL_FOREST_THIRD_AREA_NORTH_LOG_ROLLED);
        }
      } else {
        if (FIELD_CELL(probe.position.x) ==
            THIRD_AREA_SOUTH_RIVER_BANK_COLUMN) {
          MapGrid_CopyCellAttributes(26, 20, 2, 4, FIELD_CELL(probe.position.x),
                                     32);
          Actor_SetSpritePriority(ACTOR_THIRD_AREA_SOUTH_RIVER_LOG, 1);
          landed = 1;
          GameFlag_Set(FLAG_MOGALL_FOREST_THIRD_AREA_SOUTH_LOG_ROLLED);
        }
      }
      if (landed == 0) {
        Event_End();
        return;
      }
      Actor_SetAnimation(probe.actorSlot, MOGALL_FOREST_LOG_ROLLING);
      Actor_SetDestinationOffsetById(probe.actorSlot, 18, 6);
      Event_Wait(30);
      Actor_SetAnimation(probe.actorSlot, MOGALL_FOREST_LOG_FALLING_IN);
      Sound_Play(SE_LOG_FALL);
      Actor_Get(probe.actorSlot)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    } else {
      if (probe.actorSlot == ACTOR_THIRD_AREA_LOG)
        MapGrid_CopyCellAttributes(42, 49, 1, 4, FIELD_CELL(probe.position.x),
                                   49);
    }
  }
  Event_End();
}

/// Walking west off the bank east of the third area's river channel (cells
/// (43, 32) to (43, 35)) makes Isaac leap onto the lower log once it lies in
/// the channel, which dips under him.
static void MogallForest_ThirdArea_JumpWest(void) {
  struct Actor *leader;
  u8 savedMotionFlags;
  struct FieldPosition position;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  savedMotionFlags = leader->motionFlags;

  position.x = Actor_Get(ACTOR_PARTY_LEADER)->x.fixed - toFIXED(32);
  position.y = Actor_Get(ACTOR_PARTY_LEADER)->y.fixed;
  position.z = Actor_Get(ACTOR_PARTY_LEADER)->z.fixed;

  if (MogallForest_TryJumpLeaderTo(&position) != 0) {
    Event_Start();

    leader->motionFlags = 0;
    Actor_SetAnimation(ACTOR_THIRD_AREA_SOUTH_RIVER_LOG,
                       MOGALL_FOREST_LOG_STEPPED_ON);

    leader->y.fixed -= toFIXED(1);
    leader->groundHeight -= toFIXED(1);
    wait(2);

    leader->y.fixed -= toFIXED(1);
    leader->groundHeight -= toFIXED(1);
    wait(10);

    leader->y.fixed += toFIXED(1);
    leader->groundHeight += toFIXED(1);
    wait(4);

    leader->y.fixed += toFIXED(1);
    leader->groundHeight += toFIXED(1);

    leader->motionFlags = savedMotionFlags;
    Event_End();
  }
}

/// Move on the rock beside the log by the Elven Shirt's chest pushes it a cell.
/// In the ring among the stumps by the stream (cell (45, 6)) it settles into
/// place and stays, out of the way of the log.
static void MogallForest_ThirdArea_UpdateNorthRock(void) {
  s32 column;
  s32 row;

  Event_Start();

  column = FIELD_CELL(Actor_Get(ACTOR_THIRD_AREA_NORTH_ROCK)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_THIRD_AREA_NORTH_ROCK)->z.fixed);

  MogallForest_UpdateRockCells(column, row);

  if (column == 45 && row == 6) {
    struct Actor *rock = Actor_Get(ACTOR_THIRD_AREA_NORTH_ROCK);

    rock->motionFlags = 0;
    rock->groundHeight = -toFIXED(2);
    rock->y.fixed = -toFIXED(2);
  }

  Event_End();
}

/// Move on the rock beside the upper log on the third area's river island
/// pushes it a cell. Pushed south into the ring on row 27, it settles into
/// place and stays, and the cell (43, 23) across the channel is made solid.
///
/// Scene_Initialize runs it too, to settle the rock again on arrival.
static void MogallForest_ThirdArea_UpdateSouthRock(void) {
  struct Actor *rock;

  Event_Start();
  MogallForest_UpdateRockCells(
      FIELD_CELL(Actor_Get(ACTOR_THIRD_AREA_SOUTH_ROCK)->x.fixed),
      FIELD_CELL(Actor_Get(ACTOR_THIRD_AREA_SOUTH_ROCK)->z.fixed));
  if (FIELD_CELL(Actor_Get(ACTOR_THIRD_AREA_SOUTH_ROCK)->z.fixed) == 27) {
    rock = Actor_Get(ACTOR_THIRD_AREA_SOUTH_ROCK);
    rock->motionFlags = 0;
    rock->groundHeight = -toFIXED(2);
    rock->y.fixed = -toFIXED(2);
    GameFlag_Set(FLAG_MOGALL_FOREST_SOUTH_ROCK_SETTLED);
    PushedActor_FillGridAttributeRectangle(2, 43, 23, 1, 1, CELL_BEHAVIOR_WALL);
  }
  Event_End();
}

/// Force on the east one of the three stumps in a row in the third area brings
/// out an ape that cries out in anger, leaps at Isaac and starts a battle with
/// it.
static void MogallForest_ThirdArea_ApeAttacks(void) {
  struct Actor *ape;
  s32 leaderX;
  struct Actor *leaderForZ;

  ape = Actor_Get(ACTOR_THIRD_AREA_ATTACKING_APE);
  Event_Start();
  MogallForest_ApeComesOut(ACTOR_THIRD_AREA_ATTACKING_APE, SPRITE_COLOR_NORMAL);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_ATTACKING_APE, 472, 104, 8);
  Event_Wait(10);
  FieldEffect_Spawn(ape->x.fixed, ape->y.fixed, (ape->z.fixed + toFIXED(8)), 0,
                    0, 0, EFFECT_SCRIPT_MEDIUM, 0);
  FieldCamera_FollowActor(ACTOR_THIRD_AREA_ATTACKING_APE, 1);
  Actor_TurnToFaceEachOther(ACTOR_THIRD_AREA_ATTACKING_APE, ACTOR_PARTY_LEADER,
                            0);
  Event_Wait(30);
  Actor_StartRepeatedMotion(ACTOR_THIRD_AREA_ATTACKING_APE, 2);
  Actor_ShowEmote(ACTOR_THIRD_AREA_ATTACKING_APE,
                  EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Sound_Play(SE_BEAST_CRY);
  Event_Wait(60);
  leaderX = Actor_Get(ACTOR_PARTY_LEADER)->x.part[0].integer;
  leaderForZ = Actor_Get(ACTOR_PARTY_LEADER);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_ATTACKING_APE, leaderX,
                          leaderForZ->z.part[0].integer, 6);
  Event_Wait(10);
  GameFlag_Set(FLAG_MOGALL_FOREST_THIRD_AREA_ATTACKING_APE_OUT);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 0);
  Event_End();
}

/// Force on the middle one of the three stumps in a row in the third area
/// brings a green ape out of it. The ape points the way and hops off south,
/// the way on to the river.
static void MogallForest_ThirdArea_MiddleApeShowsWay(void) {
  struct Actor *ape;

  ape = Actor_Get(ACTOR_THIRD_AREA_MIDDLE_APE);

  Event_Start();

  MogallForest_ApeComesOut(ACTOR_THIRD_AREA_MIDDLE_APE, SPRITE_COLOR_GRB);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_MIDDLE_APE, 456, 152, 6);
  Event_Wait(10);

  FieldEffect_Spawn(ape->x.fixed, ape->y.fixed, ape->z.fixed + toFIXED(4), 0, 0,
                    0, EFFECT_SCRIPT_MEDIUM, 0);

  MogallForest_ApePointsTheWay(ACTOR_THIRD_AREA_MIDDLE_APE);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_MIDDLE_APE, 448, 192, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_MIDDLE_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_MIDDLE_APE, 424, 208, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_MIDDLE_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_MIDDLE_APE, 424, 224, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_MIDDLE_APE, 0);
  Event_Wait(6);

  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  Actor_SetPosition(ACTOR_THIRD_AREA_MIDDLE_APE, 0, 0);
  Event_Wait(30);
  GameFlag_Set(FLAG_MOGALL_FOREST_THIRD_AREA_MIDDLE_APE_OUT);
  Actor_SetPosition(ACTOR_THIRD_AREA_MIDDLE_APE_STUMP, 0, 0);

  Event_End();
}

/// Force on the west one of the three stumps in a row in the third area brings
/// a green ape out of it. The ape points the way and hops off west, toward the
/// Elven Shirt's chest.
static void MogallForest_ThirdArea_WestApeShowsWay(void) {
  struct Actor *ape;

  ape = Actor_Get(ACTOR_THIRD_AREA_WEST_APE);

  Event_Start();

  MogallForest_ApeComesOut(ACTOR_THIRD_AREA_WEST_APE, SPRITE_COLOR_GRB);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_WEST_APE, 392, 104, 6);
  Event_Wait(10);

  FieldEffect_Spawn(ape->x.fixed, ape->y.fixed, ape->z.fixed + toFIXED(4), 0, 0,
                    0, EFFECT_SCRIPT_MEDIUM, 0);

  MogallForest_ApePointsTheWay(ACTOR_THIRD_AREA_WEST_APE);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_WEST_APE, 376, 152, 6);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_WEST_APE, 0);
  Event_Wait(10);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_WEST_APE, 328, 160, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_WEST_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_WEST_APE, 296, 160, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_WEST_APE, 0);
  Event_Wait(6);

  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  Actor_SetPosition(ACTOR_THIRD_AREA_WEST_APE, 0, 0);
  Event_Wait(30);
  GameFlag_Set(FLAG_MOGALL_FOREST_THIRD_AREA_WEST_APE_OUT);
  Actor_SetPosition(ACTOR_THIRD_AREA_WEST_APE_STUMP, 0, 0);

  Event_End();
}

/// Force on the stump on its own island in the third area's river clearing
/// brings a green ape out, which leaps south into a lower stump and hides
/// again, the camera following it.
static void MogallForest_ThirdArea_EastApeHidesAgain(void) {
  Event_Start();
  MogallForest_ApeComesOut(ACTOR_THIRD_AREA_EAST_APE, SPRITE_COLOR_GRB);
  FieldCamera_MoveTo(744, CAMERA_KEEP, 504, 1);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 744, 504, 9);
  MogallForest_SpawnPuffRing(ACTOR_THIRD_AREA_EAST_APE);
  Actor_SetSpriteColorById(ACTOR_THIRD_AREA_EAST_APE, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_THIRD_AREA_EAST_APE), 0);
  Event_Wait(30);
  GameFlag_Set(FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_MOVED);
  Actor_SetPosition(ACTOR_THIRD_AREA_EAST_APE_STUMP, 744, 504);
  Event_End();
}

/// Force on the lower stump in the third area's river clearing, where the ape
/// from the island hid, brings it out again. It points the way and hops off
/// south, the way to the lone stump.
static void MogallForest_ThirdArea_EastApeShowsWay(void) {
  struct Actor *ape;

  ape = Actor_Get(ACTOR_THIRD_AREA_EAST_APE);

  Event_Start();

  MogallForest_ApeComesOut(ACTOR_THIRD_AREA_EAST_APE, SPRITE_COLOR_GRB);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 712, 536, 6);
  Event_Wait(10);

  FieldEffect_Spawn(ape->x.fixed, ape->y.fixed, ape->z.fixed + toFIXED(4), 0, 0,
                    0, EFFECT_SCRIPT_MEDIUM, 0);

  MogallForest_ApePointsTheWay(ACTOR_THIRD_AREA_EAST_APE);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 712, 568, 6);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
  Event_Wait(10);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 712, 600, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 736, 640, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 736, 704, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
  Event_Wait(6);

  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  Actor_SetPosition(ACTOR_THIRD_AREA_EAST_APE, 0, 0);
  Event_Wait(30);
  GameFlag_Set(FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT);

  Event_End();
}

/// Walking up to the lone stump on the island between the two log bridges, on
/// its east side (cells (12, 22) and (12, 23)), the ape that led the party
/// there bursts out of it and hops away south over the bridge. It happens once.
static void MogallForest_ApeLeadsToKillerApe(void) {
  struct Actor *ape;

  ape = Actor_Get(ACTOR_THIRD_AREA_EAST_APE);
  Event_Start();
  Actor_SetPosition(ACTOR_THIRD_AREA_EAST_APE, 136, 360);
  MogallForest_ApeComesOut(ACTOR_THIRD_AREA_EAST_APE, SPRITE_COLOR_GRB);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 136, 408, 8);
  Event_Wait(10);
  FieldEffect_Spawn(ape->x.fixed, ape->y.fixed, (ape->z.fixed + toFIXED(4)), 0,
                    0, 0, EFFECT_SCRIPT_MEDIUM, 0);
  Actor_FaceDirection(ACTOR_THIRD_AREA_EAST_APE, FACING_NORTH, 40);
  Actor_SetAttachedEffect(ACTOR_THIRD_AREA_EAST_APE,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_RunRepeatedMotion(ACTOR_THIRD_AREA_EAST_APE, 2);
  FieldCamera_FollowActor(ACTOR_THIRD_AREA_EAST_APE, 1);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 136, 440, 6);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
  Event_Wait(10);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 136, 472, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
  Event_Wait(6);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 136, 504, 3);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
  Event_Wait(6);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  Actor_SetPosition(ACTOR_THIRD_AREA_EAST_APE, 0, 0);
  Event_Wait(60);
  GameFlag_Set(FLAG_MOGALL_FOREST_LED_TO_KILLER_APE);
  Event_End();
}

/// Stepping onto the top of the south bridge below the lone stump (cell (8,
/// 27)) until the Killer Ape is beaten: the camera turns to the stump and the
/// ground shakes twice with puffs of dust round it. Then the Killer Ape, a big
/// blue ape, bursts out, roars, leaps at Isaac and its battle begins.
static void MogallForest_KillerApeAttacks(void) {
  struct Vector3 velocity;
  struct Actor *actor;
  u32 directionIndex;
  s32 x;
  FIXED z;

  actor = Actor_Get(ACTOR_THIRD_AREA_EAST_APE);
  Event_Start();
  Actor_SetSpriteColorById(ACTOR_THIRD_AREA_EAST_APE, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_THIRD_AREA_EAST_APE), 0);
  Actor_SetPosition(ACTOR_THIRD_AREA_EAST_APE, 136, 360);
  FieldCamera_SetSpeed(0.5);
  FieldCamera_MoveTo(136, CAMERA_KEEP, 392, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(60);
  MapGrid_SetShake(5, 5, 1);
  MogallForest_SpawnPuffRing(ACTOR_THIRD_AREA_EAST_APE);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Event_Wait(40);
  MapGrid_SetShake(5, 5, 1);
  MogallForest_SpawnPuffRing(ACTOR_THIRD_AREA_EAST_APE);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  Event_Wait(40);
  MogallForest_SpawnPuffRing(ACTOR_THIRD_AREA_EAST_APE);
  actor->scaleX = 6 * FIXED_ONE / 5;
  actor->scaleY = 6 * FIXED_ONE / 5;
  Actor_SetSpriteColorById(ACTOR_THIRD_AREA_EAST_APE, SPRITE_COLOR_BRG);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 136, 392, 15);
  Event_Wait(15);
  // The larger reveal uses a wider, level ring of puffs.
  MapGrid_SetShake(5, 5, 1);
  for (directionIndex = 0; directionIndex <= TRIG_TURN / FACING_STEP;
       directionIndex++) {
    velocity.x = Trig_Cos(directionIndex * FACING_STEP);
    velocity.y = 0;
    z = Trig_Sin(directionIndex * FACING_STEP);
    x = velocity.x;
    velocity.z = z;
    x += x / 2;
    velocity.x = x;
    FieldEffect_Spawn(actor->x.fixed, 0, actor->z.fixed, x, velocity.y, z,
                      EFFECT_SCRIPT_MEDIUM, NULL);
  }
  Event_Wait(30);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  Sound_Play(SE_BOSS_ROAR);
  Actor_RunRepeatedMotion(ACTOR_THIRD_AREA_EAST_APE, 2);
  Event_Wait(20);
  // It leaps at Isaac, and the battle begins.
  {
    s32 leaderX = Actor_Get(ACTOR_PARTY_LEADER)->x.part[0].integer;

    MogallForest_HopActorTo(
        ACTOR_THIRD_AREA_EAST_APE, leaderX,
        Actor_Get(ACTOR_PARTY_LEADER)->z.part[0].integer - 16, 8);
  }
  Event_Wait(10);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination(
      SCENE_MOGALL_FOREST_KILLER_APE,
      MOGALL_FOREST_KILLER_APE_AFTER_KILLER_APE_ATTACKS);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 1);
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_MOGALL_FOREST_SIGN) {
    return gMogallForestSign_Triggers;
  }
  if (scene == SCENE_MOGALL_FOREST_QUARTZ) {
    return gMogallForestQuartz_Triggers;
  }
  if (scene == SCENE_MOGALL_FOREST_KILLER_APE) {
    return gMogallForestKillerApe_Triggers;
  }
  return gMogallForestSidePaths_Triggers;
}

/// Sets up the clearing entered. On the first arrival in the forest it plays
/// the ape peeking out of the first stump; otherwise it puts back the apes
/// already out, the logs in the river and the settled rocks.
s32 Scene_Initialize(void) {
  struct Actor *actor;
  s32 sceneId;
  s32 entrance;

  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  sceneId = gMainRoutineState.next;
  if (sceneId == SCENE_MOGALL_FOREST_SIGN) {
    entrance = gMainRoutineState.entrance;
    switch (entrance) {
    case MOGALL_FOREST_SIGN_FROM_WORLD_MAP:
    case MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_WEST:
    case MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_NORTH:
    case MOGALL_FOREST_SIGN_ENTRANCE_FROM_SOUTHWEST_CLEARING:
      if (Event_GetFlag(FLAG_MOGALL_FOREST_FIRST_APE_SEEN) == 0) {
        Event_Start();
        wait(1);
        Actor_SetSpriteColorById(ACTOR_FIRST_AREA_NORTH_APE, SPRITE_COLOR_GRB);
        Actor_SetPosition(ACTOR_FIRST_AREA_NORTH_APE, 92, 120);
        Actor_FaceDirection(ACTOR_FIRST_AREA_NORTH_APE, FACING_NORTH_NORTHEAST,
                            0);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.4);
        Actor_WalkTo(ACTOR_PARTY_LEADER, 136, 64);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
        Event_Wait(30);
        Actor_ShowEmote(ACTOR_FIRST_AREA_NORTH_APE,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
        Actor_RunRepeatedMotion(ACTOR_FIRST_AREA_NORTH_APE, 2);
        Event_Wait(30);
        MogallForest_HopActorTo(ACTOR_FIRST_AREA_NORTH_APE, 136, 116, 7);
        MogallForest_SpawnPuffRing(ACTOR_FIRST_AREA_NORTH_APE);
        Actor_SetSpriteColorById(ACTOR_FIRST_AREA_NORTH_APE,
                                 SPRITE_COLOR_HIDDEN);
        Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_AREA_NORTH_APE), 0);
        GameFlag_Set(FLAG_MOGALL_FOREST_FIRST_APE_SEEN);
        Event_Wait(60);
        Event_End();
      }
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
        break;
      }
      if (GameFlag_Get(FLAG_MOGALL_FOREST_FIRST_AREA_NORTH_APE_OUT) != 0) {
        break;
      }
      Actor_SetSpriteColorById(ACTOR_FIRST_AREA_NORTH_APE, SPRITE_COLOR_HIDDEN);
      Actor_SetPosition(ACTOR_FIRST_AREA_NORTH_APE, 136, 116);
      break;

    case MOGALL_FOREST_SIGN_RIVER_FROM_SOUTHWEST_CLEARING:
    case MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_EAST:
    case MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_QUARTZ:
      actor = Actor_Get(ACTOR_PARTY_LEADER);
      if (actor != NULL) {
        Actor_SetPositionFX(ACTOR_NUT, actor->x.fixed, actor->z.fixed);
      }
      Actor_Get(ACTOR_NUT)->update = 0;
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
        Actor_Get(ACTOR_NUT)->y.fixed = toFIXED(32);
      }
      wait(1);
      Actor_SetPosition(ACTOR_NUT, 632, 440);
      if (GameFlag_Get(FLAG_MOGALL_FOREST_NUT_CAUGHT) == 0) {
        Scene_FloatNut(ACTOR_NUT);
      }
      Actor_SetSpriteColorById(ACTOR_FIRST_AREA_ATTACKING_APE,
                               SPRITE_COLOR_HIDDEN);
      Actor_SetSpriteColorById(ACTOR_FIRST_AREA_EAST_APE, SPRITE_COLOR_HIDDEN);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_AREA_ATTACKING_APE), 0);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_AREA_EAST_APE), 0);
      Scene_RedrawActorRectangle(ACTOR_FIRST_AREA_LOG);
      if (GameFlag_Get(FLAG_MOGALL_FOREST_FIRST_AREA_LOG_ROLLED) == 0) {
        Scene_RedrawActorRectangle(ACTOR_FIRST_AREA_RIVER_LOG);
        break;
      }
      wait(1);
      Actor_SetPosition(ACTOR_FIRST_AREA_RIVER_LOG, 528, 408);
      Actor_SetAnimation(ACTOR_FIRST_AREA_RIVER_LOG,
                         MOGALL_FOREST_LOG_IN_RIVER);
      MapGrid_CopyCellAttributes(38, 27, 4, 2, 31, 25);
      Actor_Get(ACTOR_FIRST_AREA_RIVER_LOG)->priorityFlags =
          ACTOR_PRIORITY_UNDERFOOT;
      break;
    }
  } else if (sceneId == SCENE_MOGALL_FOREST_QUARTZ) {
    entrance = gMainRoutineState.entrance;
    switch (entrance) {
    case MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIDE_PATHS_NORTH:
    case MOGALL_FOREST_QUARTZ_STUMPS_FROM_NORTHWEST_CLEARING:
    case MOGALL_FOREST_QUARTZ_STUMPS_FROM_POND:
    case MOGALL_FOREST_QUARTZ_STUMPS_FROM_NORTHEAST_CLEARING:
      if (GameFlag_Get(FLAG_MOGALL_FOREST_SECOND_AREA_WEST_APE_OUT) == 0) {
        Actor_SetSpriteColorById(ACTOR_SECOND_AREA_WEST_APE,
                                 SPRITE_COLOR_HIDDEN);
        Actor_SetSpriteFlags(Actor_Get(ACTOR_SECOND_AREA_WEST_APE), 0);
      }
      if (GameFlag_Get(FLAG_MOGALL_FOREST_SECOND_AREA_EAST_APE_OUT) != 0) {
        break;
      }
      Actor_SetSpriteColorById(ACTOR_SECOND_AREA_EAST_APE, SPRITE_COLOR_HIDDEN);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_SECOND_AREA_EAST_APE), 0);
      break;

    case MOGALL_FOREST_QUARTZ_RIVER_FROM_NORTHEAST_CLEARING:
    case MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_KILLER_APE:
    case MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIDE_PATHS_SOUTH:
      if (GameFlag_Get(FLAG_MOGALL_FOREST_SECOND_AREA_LOG_ROLLED) == 0) {
        Scene_RedrawActorRectangle(ACTOR_SECOND_AREA_RIVER_LOG);
      } else {
        wait(1);
        Actor_SetPosition(ACTOR_SECOND_AREA_RIVER_LOG, 552, 510);
        Actor_SetAnimation(ACTOR_SECOND_AREA_RIVER_LOG,
                           MOGALL_FOREST_LOG_IN_RIVER);
        Actor_Get(ACTOR_SECOND_AREA_RIVER_LOG)->priorityFlags =
            ACTOR_PRIORITY_UNDERFOOT;
        MapGrid_CopyCellAttributes(44, 30, 2, 4, 34, 30);
#if TBS_PATCH_1
        PushedActor_FillGridAttributeRectangle(0, 35, 29, 1, 4, 0);
#endif
      }
      Scene_RedrawActorRectangle(ACTOR_SECOND_AREA_LOG);
      Scene_RedrawActorRectangle(ACTOR_SECOND_AREA_SOUTH_LOG);
      PushedActor_FillGridAttributeRectangle(
          2, FIELD_CELL(Actor_Get(ACTOR_SECOND_AREA_ROCK)->x.fixed),
          FIELD_CELL(Actor_Get(ACTOR_SECOND_AREA_ROCK)->z.fixed), 1, 1,
          CELL_BEHAVIOR_WALL);
      wait(1);
      Actor_SetSpriteColorById(ACTOR_SECOND_AREA_ROCK, SPRITE_COLOR_GREY);
      Actor_Get(ACTOR_SECOND_AREA_LOG)->collisionFlags |= ACTOR_NOT_TARGETED;
      if (GameFlag_Get(FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_OUT) != 0) {
        break;
      }
      Actor_SetSpriteColorById(ACTOR_SECOND_AREA_SOUTH_APE,
                               SPRITE_COLOR_HIDDEN);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_SECOND_AREA_SOUTH_APE), 0);
      if (GameFlag_Get(FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_MOVED) == 0) {
        break;
      }
      Actor_SetPosition(ACTOR_SECOND_AREA_SOUTH_APE, 424, 480);
      Actor_SetPosition(ACTOR_SECOND_AREA_SOUTH_APE_STUMP, 424, 480);
      break;
    }
  } else if (sceneId == SCENE_MOGALL_FOREST_KILLER_APE) {
    entrance = gMainRoutineState.entrance;
    switch (entrance) {
    case MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_WEST:
    case MOGALL_FOREST_KILLER_APE_STUMPS_FROM_CHEST:
    case MOGALL_FOREST_KILLER_APE_STUMPS_FROM_NORTHWEST_CLEARING:
    case MOGALL_FOREST_KILLER_APE_STUMPS_FROM_RIVER:
      wait(1);
      if (GameFlag_Get(FLAG_MOGALL_FOREST_THIRD_AREA_ATTACKING_APE_OUT) == 0) {
        Actor_SetSpriteColorById(ACTOR_THIRD_AREA_ATTACKING_APE,
                                 SPRITE_COLOR_HIDDEN);
        Actor_SetSpriteFlags(Actor_Get(ACTOR_THIRD_AREA_ATTACKING_APE), 0);
        Actor_SetSpriteFlags(Actor_Get(ACTOR_THIRD_AREA_ATTACKING_APE_STUMP),
                             0);
      }
      if (GameFlag_Get(FLAG_MOGALL_FOREST_THIRD_AREA_MIDDLE_APE_OUT) == 0) {
        Actor_SetSpriteColorById(ACTOR_THIRD_AREA_MIDDLE_APE,
                                 SPRITE_COLOR_HIDDEN);
        Actor_SetSpriteFlags(Actor_Get(ACTOR_THIRD_AREA_MIDDLE_APE), 0);
        Actor_SetSpriteFlags(Actor_Get(ACTOR_THIRD_AREA_MIDDLE_APE_STUMP), 0);
      }
      if (GameFlag_Get(FLAG_MOGALL_FOREST_THIRD_AREA_WEST_APE_OUT) != 0) {
        break;
      }
      Actor_SetSpriteColorById(ACTOR_THIRD_AREA_WEST_APE, SPRITE_COLOR_HIDDEN);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_THIRD_AREA_WEST_APE), 0);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_THIRD_AREA_WEST_APE_STUMP), 0);
      break;

    case MOGALL_FOREST_KILLER_APE_CHEST_FROM_STUMPS:
      PushedActor_FillGridAttributeRectangle(
          2, FIELD_CELL(Actor_Get(ACTOR_THIRD_AREA_NORTH_ROCK)->x.fixed),
          FIELD_CELL(Actor_Get(ACTOR_THIRD_AREA_NORTH_ROCK)->z.fixed), 1, 1,
          CELL_BEHAVIOR_WALL);
      Actor_SetSpriteColorById(ACTOR_THIRD_AREA_NORTH_ROCK, SPRITE_COLOR_GREY);
      wait(1);
      Actor_Get(ACTOR_THIRD_AREA_LOG)->collisionFlags |= ACTOR_NOT_TARGETED;
      Scene_RedrawActorRectangle(ACTOR_THIRD_AREA_LOG);
      break;

    case MOGALL_FOREST_KILLER_APE_RIVER_FROM_STUMPS:
    case MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_EAST:
    case MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_SOUTH:
    case MOGALL_FOREST_KILLER_APE_RIVER_FROM_BRIDGES:
      // XXX (no evidence): what these two crates are for; they are hidden, not
      // solid and never a target, so the player neither sees nor meets them.
      Actor_CreateConfigured(734, 0, 370, SPRITE_CRATE);
      Actor_CreateConfigured(754, 0, 370, SPRITE_CRATE);
      Scene_RedrawActorRectangle(ACTOR_THIRD_AREA_WEST_LOG);
      Scene_RedrawActorRectangle(ACTOR_THIRD_AREA_SOUTHWEST_LOG);
      if (GameFlag_Get(FLAG_MOGALL_FOREST_THIRD_AREA_NORTH_LOG_ROLLED) == 0) {
        Scene_RedrawActorRectangle(ACTOR_THIRD_AREA_NORTH_RIVER_LOG);
      } else {
        wait(1);
        Actor_SetAnimation(ACTOR_THIRD_AREA_NORTH_RIVER_LOG,
                           MOGALL_FOREST_LOG_IN_RIVER);
        Actor_SetPosition(ACTOR_THIRD_AREA_NORTH_RIVER_LOG, 698, 398);
        Actor_Get(ACTOR_THIRD_AREA_NORTH_RIVER_LOG)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        MapGrid_CopyCellAttributes(26, 20, 2, 4, 42, 23);
        GameFlag_Set(FLAG_MOGALL_FOREST_SOUTH_ROCK_SETTLED);
        Actor_SetPosition(ACTOR_THIRD_AREA_SOUTH_ROCK, 632, 440);
        Actor_SetSpriteFlags(Actor_Get(ACTOR_THIRD_AREA_SOUTH_ROCK), 0);
      }
      if (GameFlag_Get(FLAG_MOGALL_FOREST_THIRD_AREA_SOUTH_LOG_ROLLED) == 0) {
        Scene_RedrawActorRectangle(ACTOR_THIRD_AREA_SOUTH_RIVER_LOG);
      } else {
        wait(1);
        Actor_SetAnimation(ACTOR_THIRD_AREA_SOUTH_RIVER_LOG,
                           MOGALL_FOREST_LOG_IN_RIVER);
        Actor_SetPosition(ACTOR_THIRD_AREA_SOUTH_RIVER_LOG, 666, 550);
        Actor_Get(ACTOR_THIRD_AREA_SOUTH_RIVER_LOG)->priorityFlags =
            ACTOR_PRIORITY_UNDERFOOT;
        MapGrid_CopyCellAttributes(26, 20, 2, 4, 40, 32);
      }
      PushedActor_FillGridAttributeRectangle(
          2, FIELD_CELL(Actor_Get(ACTOR_THIRD_AREA_SOUTH_ROCK)->x.fixed),
          FIELD_CELL(Actor_Get(ACTOR_THIRD_AREA_SOUTH_ROCK)->z.fixed), 1, 1,
          CELL_BEHAVIOR_WALL);
      Actor_SetSpriteColorById(ACTOR_THIRD_AREA_SOUTH_ROCK, SPRITE_COLOR_GREY);
      wait(1);
      Actor_Get(ACTOR_THIRD_AREA_NORTH_RIVER_LOG)->collisionFlags |=
          ACTOR_NOT_TARGETED;
      if (GameFlag_Get(FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT) == 0) {
        Actor_SetSpriteColorById(ACTOR_THIRD_AREA_EAST_APE,
                                 SPRITE_COLOR_HIDDEN);
        Actor_SetSpriteFlags(Actor_Get(ACTOR_THIRD_AREA_EAST_APE), 0);
        if (GameFlag_Get(FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_MOVED) != 0) {
          Actor_SetPosition(ACTOR_THIRD_AREA_EAST_APE_STUMP, 744, 504);
          Actor_SetPosition(ACTOR_THIRD_AREA_EAST_APE, 744, 504);
        }
#if TBS_PATCH_2
      } else {
        Actor_SetPosition(ACTOR_THIRD_AREA_EAST_APE, 0, 0);
#endif
      }
      MogallForest_ThirdArea_UpdateSouthRock();
      break;

    case MOGALL_FOREST_KILLER_APE_BRIDGES_FROM_RIVER:
    case MOGALL_FOREST_KILLER_APE_FROM_WORLD_MAP:
      Actor_SetScript(ACTOR_THIRD_AREA_EAST_APE, gMogallForest_ActorScript);
      if (GameFlag_Get(FLAG_NYUNPA_TAUGHT_FORCE) == 0) {
        break;
      }
      if (GameFlag_Get(FLAG_MOGALL_FOREST_KILLER_APE_BEATEN) == 0) {
        break;
      }
      GameFlag_Set(FLAG_NYUNPA_ENLIGHTENED);
      break;

    case MOGALL_FOREST_KILLER_APE_AFTER_KILLER_APE_ATTACKS:
      GameFlag_Set(FLAG_MOGALL_FOREST_KILLER_APE_BEATEN);
      break;
    }
  }
  return 0;
}

#include "shared/actor_orbit.c"

#include "shared/pushed_actor_tables.c"
// clang-format off

#include "shared/field_effect_duration_scripts.c"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// The script of each rock moved with Move, which draws it in greys.
s32 gMogallForest_RockScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_SET(SW_TIMER, SPRITE_COLOR_GREY),
    SC_CALL(MogallForest_SetColorFromTimer),
    SC_END,
};

/// The script of each ape, at four fifths of its size, hidden while it hides,
/// and with its shadow off.
s32 gMogallForest_ActorScript[] = {
    SC_SET(SW_SCALE_X, 4 * FIXED_ONE / 5),
    SC_SET(SW_SCALE_Y, 4 * FIXED_ONE / 5),
    SC_SET(SW_TIMER, SPRITE_COLOR_HIDDEN),
    SC_CALL(MogallForest_SetColorFromTimer),
    SC_CALL(MogallForest_ClearActorSpriteFlags),
    SC_END,
};

/// The script of each stump's blank actor, the target of Force, which stays
/// still with its sprite hidden.
s32 gMogallForest_HidingPlaceScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_Y, 0),
    SC_SET(SW_TARGET_Y, ACTOR_NO_TARGET),
    SC_CALL(MogallForest_ClearActorSpriteFlags),
    SC_WAIT(2),
    SC_END,
};

struct SceneEntrance gMogallForestSign_Entrances[] = {
    { MOGALL_FOREST_SIGN_FROM_WORLD_MAP, CONDITION_ALWAYS, 136, 0, 24, FACING_SOUTH, 0, 0, 0, 272, 224, 0 },
    { MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_WEST, CONDITION_ALWAYS, 24, 0, 128, FACING_EAST, 0, 0, 0, 272, 224, 0 },
    { MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_NORTH, CONDITION_ALWAYS, 248, 0, 128, FACING_WEST, 0, 0, 0, 272, 224, 0 },
    { MOGALL_FOREST_SIGN_ENTRANCE_FROM_SOUTHWEST_CLEARING, CONDITION_ALWAYS, 136, 0, 200, FACING_NORTH, 0, 0, 0, 272, 224, 0 },
    { MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_FROM_ENTRANCE, CONDITION_ALWAYS, 72, 0, 264, FACING_SOUTH, 0, 0, 240, 272, 464, 0 },
    { MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_FROM_RIVER, CONDITION_ALWAYS, 200, 0, 440, FACING_NORTH, 0, 0, 240, 272, 464, 0 },
    { MOGALL_FOREST_SIGN_RIVER_FROM_SOUTHWEST_CLEARING, CONDITION_ALWAYS, 416, 0, 264, FACING_SOUTH, 0, 288, 240, 672, 624, 0 },
    { MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_EAST, CONDITION_ALWAYS, 560, 0, 264, FACING_SOUTH, 0, 288, 240, 672, 624, 0 },
    { MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_QUARTZ, CONDITION_ALWAYS, 648, 0, 344, FACING_WEST, 0, 288, 240, 672, 624, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gMogallForestQuartz_Entrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIGN, CONDITION_ALWAYS, 24, 0, 128, FACING_EAST, 0, 0, 0, 272, 224, 0 },
    { MOGALL_FOREST_QUARTZ_NORTHWEST_CLEARING_FROM_STUMPS, CONDITION_ALWAYS, 248, 0, 128, FACING_WEST, 0, 0, 0, 272, 224, 0 },
    { MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIDE_PATHS_NORTH, CONDITION_ALWAYS, 424, 0, 24, FACING_SOUTH, 0, 288, 0, 560, 224, 0 },
    { MOGALL_FOREST_QUARTZ_STUMPS_FROM_NORTHWEST_CLEARING, CONDITION_ALWAYS, 312, 0, 136, FACING_EAST, 0, 288, 0, 560, 224, 0 },
    { MOGALL_FOREST_QUARTZ_STUMPS_FROM_POND, CONDITION_ALWAYS, 536, 0, 120, FACING_WEST, 0, 288, 0, 560, 224, 0 },
    { MOGALL_FOREST_QUARTZ_STUMPS_FROM_NORTHEAST_CLEARING, CONDITION_ALWAYS, 424, 0, 200, FACING_NORTH, 0, 288, 0, 560, 224, 0 },
    { MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_FROM_STUMPS, CONDITION_ALWAYS, 744, 0, 24, FACING_SOUTH, 0, 576, 0, 848, 224, 0 },
    { MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_FROM_RIVER, CONDITION_ALWAYS, 744, 0, 200, FACING_NORTH, 0, 576, 0, 848, 224, 0 },
    { MOGALL_FOREST_QUARTZ_POND_FROM_STUMPS, CONDITION_ALWAYS, 24, 0, 368, FACING_EAST, 0, 0, 240, 272, 464, 0 },
    { MOGALL_FOREST_QUARTZ_RIVER_FROM_NORTHEAST_CLEARING, CONDITION_ALWAYS, 488, 0, 264, FACING_SOUTH, 0, 288, 240, 736, 624, 0 },
    { MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_KILLER_APE, CONDITION_ALWAYS, 312, 0, 528, FACING_EAST, 0, 288, 240, 736, 624, 0 },
    { MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIDE_PATHS_SOUTH, CONDITION_ALWAYS, 360, 0, 600, FACING_NORTH, 0, 288, 240, 736, 624, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gMogallForestKillerApe_Entrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MOGALL_FOREST_KILLER_APE_NORTHWEST_CLEARING_FROM_STUMPS, CONDITION_ALWAYS, 24, 0, 128, FACING_EAST, 0, 0, 0, 272, 224, 0 },
    { MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_QUARTZ, CONDITION_ALWAYS, 248, 0, 128, FACING_WEST, 0, 0, 0, 272, 224, 0 },
    { MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_WEST, CONDITION_ALWAYS, 424, 0, 24, FACING_SOUTH, 0, 288, 0, 560, 224, 0 },
    { MOGALL_FOREST_KILLER_APE_STUMPS_FROM_CHEST, CONDITION_ALWAYS, 312, 0, 160, FACING_EAST, 0, 288, 0, 560, 224, 0 },
    { MOGALL_FOREST_KILLER_APE_STUMPS_FROM_NORTHWEST_CLEARING, CONDITION_ALWAYS, 536, 0, 160, FACING_WEST, 0, 288, 0, 560, 224, 0 },
    { MOGALL_FOREST_KILLER_APE_STUMPS_FROM_RIVER, CONDITION_ALWAYS, 424, 0, 200, FACING_NORTH, 0, 288, 0, 560, 224, 0 },
    { MOGALL_FOREST_KILLER_APE_CHEST_FROM_STUMPS, CONDITION_ALWAYS, 824, 0, 136, FACING_WEST, 0, 576, 0, 848, 224, 0 },
    { MOGALL_FOREST_KILLER_APE_RIVER_FROM_STUMPS, CONDITION_ALWAYS, 728, 0, 264, FACING_SOUTH, 0, 400, 240, 848, 640, 0 },
    { MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_EAST, CONDITION_ALWAYS, 824, 0, 512, FACING_WEST, 0, 400, 240, 848, 640, 0 },
    { MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_SOUTH, CONDITION_ALWAYS, 824, 0, 608, FACING_WEST, 0, 400, 240, 848, 640, 0 },
    { MOGALL_FOREST_KILLER_APE_RIVER_FROM_BRIDGES, CONDITION_ALWAYS, 736, 0, 616, FACING_NORTH, 0, 400, 240, 848, 640, 0 },
    { MOGALL_FOREST_KILLER_APE_BRIDGES_FROM_RIVER, CONDITION_ALWAYS, 264, 0, 264, FACING_SOUTH, 0, 0, 240, 384, 496, 0 },
    { MOGALL_FOREST_KILLER_APE_FROM_WORLD_MAP, CONDITION_ALWAYS, 136, 0, 472, FACING_NORTH, 0, 0, 240, 384, 496, 0 },
    { MOGALL_FOREST_KILLER_APE_AFTER_KILLER_APE_ATTACKS, CONDITION_ALWAYS, 136, 0, 432, FACING_NORTH, 0, 0, 240, 384, 496, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gMogallForestSidePaths_Entrances[] = {
    { MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_NORTH, CONDITION_ALWAYS, 312, 0, 128, FACING_EAST, 0, 288, 0, 560, 224, 0 },
    { MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_WEST, CONDITION_ALWAYS, 248, 0, 368, FACING_WEST, 0, 0, 240, 272, 464, 0 },
    { MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_QUARTZ_NORTH, CONDITION_ALWAYS, 424, 0, 440, FACING_NORTH, 0, 288, 240, 560, 464, 0 },
    { MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_QUARTZ_SOUTH, CONDITION_ALWAYS, 712, 0, 264, FACING_SOUTH, 0, 576, 240, 848, 464, 0 },
    { MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_WEST, CONDITION_ALWAYS, 136, 0, 680, FACING_NORTH, 0, 0, 480, 272, 704, 0 },
    { MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_EAST, CONDITION_ALWAYS, 312, 0, 552, FACING_EAST, 0, 288, 480, 560, 704, 0 },
    { MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_SOUTH, CONDITION_ALWAYS, 312, 0, 648, FACING_EAST, 0, 288, 480, 560, 704, 0 },
    { MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_EAST, CONDITION_ALWAYS, 648, 0, 680, FACING_NORTH, 0, 576, 480, 848, 704, 0 },
    { MOGALL_FOREST_SIDE_PATHS_NORTH_CLEARING_EAST, CONDITION_ALWAYS, 536, 0, 128, FACING_WEST, 0, 288, 0, 560, 224, 0 },
    { MOGALL_FOREST_SIDE_PATHS_WEST_CLEARING_WEST, CONDITION_ALWAYS, 24, 0, 368, FACING_EAST, 0, 0, 240, 272, 464, 0 },
    { SCENE_TABLE_END },
};

u32 gMogallForest_Exits[] = {
    SCENE_EXITS(MogallForestSign),
    SCENE_EXIT(MOGALL_FOREST_SIGN_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_MOGALL_FOREST_SIGN),
    SCENE_EXIT(MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_SIDE_PATHS_WEST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_WEST),
    SCENE_EXIT(MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_SIDE_PATHS_NORTH, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_NORTH),
    SCENE_EXIT(MOGALL_FOREST_SIGN_ENTRANCE_TO_SOUTHWEST_CLEARING, MogallForestSign, MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_FROM_ENTRANCE),
    SCENE_EXIT(MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_TO_ENTRANCE, MogallForestSign, MOGALL_FOREST_SIGN_ENTRANCE_FROM_SOUTHWEST_CLEARING),
    SCENE_EXIT(MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_TO_RIVER, MogallForestSign, MOGALL_FOREST_SIGN_RIVER_FROM_SOUTHWEST_CLEARING),
    SCENE_EXIT(MOGALL_FOREST_SIGN_RIVER_TO_SOUTHWEST_CLEARING, MogallForestSign, MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_FROM_RIVER),
    SCENE_EXIT(MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_SIDE_PATHS_EAST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_EAST),
    SCENE_EXIT(MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_QUARTZ, MogallForestQuartz, MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIGN),
    SCENE_EXITS(MogallForestQuartz),
    SCENE_EXIT(MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_SIGN, MogallForestSign, MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_QUARTZ),
    SCENE_EXIT(MOGALL_FOREST_QUARTZ_NORTHWEST_CLEARING_TO_STUMPS, MogallForestQuartz, MOGALL_FOREST_QUARTZ_STUMPS_FROM_NORTHWEST_CLEARING),
    SCENE_EXIT(MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_SIDE_PATHS_NORTH, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_QUARTZ_NORTH),
    SCENE_EXIT(MOGALL_FOREST_QUARTZ_STUMPS_TO_NORTHWEST_CLEARING, MogallForestQuartz, MOGALL_FOREST_QUARTZ_NORTHWEST_CLEARING_FROM_STUMPS),
    SCENE_EXIT(MOGALL_FOREST_QUARTZ_STUMPS_TO_POND, MogallForestQuartz, MOGALL_FOREST_QUARTZ_POND_FROM_STUMPS),
    SCENE_EXIT(MOGALL_FOREST_QUARTZ_STUMPS_TO_NORTHEAST_CLEARING, MogallForestQuartz, MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_FROM_STUMPS),
    SCENE_EXIT(MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_TO_STUMPS, MogallForestQuartz, MOGALL_FOREST_QUARTZ_STUMPS_FROM_NORTHEAST_CLEARING),
    SCENE_EXIT(MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_TO_RIVER, MogallForestQuartz, MOGALL_FOREST_QUARTZ_RIVER_FROM_NORTHEAST_CLEARING),
    SCENE_EXIT(MOGALL_FOREST_QUARTZ_POND_TO_STUMPS, MogallForestQuartz, MOGALL_FOREST_QUARTZ_STUMPS_FROM_POND),
    SCENE_EXIT(MOGALL_FOREST_QUARTZ_RIVER_TO_NORTHEAST_CLEARING, MogallForestQuartz, MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_FROM_RIVER),
    SCENE_EXIT(MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_KILLER_APE, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_QUARTZ),
    SCENE_EXIT(MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_SIDE_PATHS_SOUTH, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_QUARTZ_SOUTH),
    SCENE_EXITS(MogallForestKillerApe),
    SCENE_EXIT(MOGALL_FOREST_KILLER_APE_NORTHWEST_CLEARING_TO_STUMPS, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_STUMPS_FROM_NORTHWEST_CLEARING),
    SCENE_EXIT(MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_QUARTZ, MogallForestQuartz, MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_KILLER_APE),
    SCENE_EXIT(MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_SIDE_PATHS_WEST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_WEST),
    SCENE_EXIT(MOGALL_FOREST_KILLER_APE_STUMPS_TO_CHEST, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_CHEST_FROM_STUMPS),
    SCENE_EXIT(MOGALL_FOREST_KILLER_APE_STUMPS_TO_NORTHWEST_CLEARING, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_NORTHWEST_CLEARING_FROM_STUMPS),
    SCENE_EXIT(MOGALL_FOREST_KILLER_APE_STUMPS_TO_RIVER, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_RIVER_FROM_STUMPS),
    SCENE_EXIT(MOGALL_FOREST_KILLER_APE_CHEST_TO_STUMPS, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_STUMPS_FROM_CHEST),
    SCENE_EXIT(MOGALL_FOREST_KILLER_APE_RIVER_TO_STUMPS, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_STUMPS_FROM_RIVER),
    SCENE_EXIT(MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_SIDE_PATHS_EAST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_EAST),
    SCENE_EXIT(MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_SIDE_PATHS_SOUTH, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_SOUTH),
    SCENE_EXIT(MOGALL_FOREST_KILLER_APE_RIVER_TO_BRIDGES, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_BRIDGES_FROM_RIVER),
    SCENE_EXIT(MOGALL_FOREST_KILLER_APE_BRIDGES_TO_RIVER, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_RIVER_FROM_BRIDGES),
    SCENE_EXIT(MOGALL_FOREST_KILLER_APE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_MOGALL_FOREST_KILLER_APE),
    SCENE_EXITS(MogallForestSidePaths),
    SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_SIGN_NORTH, MogallForestSign, MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_NORTH),
    SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_SIGN_WEST, MogallForestSign, MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_WEST),
    SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_QUARTZ_NORTH, MogallForestQuartz, MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIDE_PATHS_NORTH),
    SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_QUARTZ_SOUTH, MogallForestQuartz, MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIDE_PATHS_SOUTH),
    SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_KILLER_APE_WEST, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_WEST),
    SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_KILLER_APE_EAST, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_EAST),
    SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_KILLER_APE_SOUTH, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_SOUTH),
    SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_SIGN_EAST, MogallForestSign, MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_EAST),
    SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_NORTH_CLEARING_WEST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_NORTH),
    SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_WEST_CLEARING_EAST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_WEST),
    SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_CENTER_CLEARING, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_QUARTZ_NORTH),
    SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_EAST_CLEARING, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_QUARTZ_SOUTH),
    SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_SOUTHWEST_CLEARING, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_WEST),
    SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_SOUTH_CLEARING_NORTHWEST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_EAST),
    SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_SOUTH_CLEARING_SOUTHWEST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_SOUTH),
    SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_SOUTHEAST_CLEARING, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_EAST),
    SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_NORTH_CLEARING_EAST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_NORTH_CLEARING_EAST),
    SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_WEST_CLEARING_WEST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_WEST_CLEARING_WEST),
    SCENE_EXITS_END,
};

struct ActorPlacement gMogallForestSign_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        488, 0, 464, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        528, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_APE, FLAG_MOGALL_FOREST_FIRST_AREA_NORTH_APE_OUT, (s32)gMogallForest_ActorScript,
        136, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_APE, FLAG_MOGALL_FOREST_FIRST_AREA_ATTACKING_APE_OUT, (s32)gMogallForest_ActorScript,
        376, 0, 432, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_APE, FLAG_MOGALL_FOREST_FIRST_AREA_EAST_APE_OUT, (s32)gMogallForest_ActorScript,
        504, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_MOGALL_FOREST_FIRST_AREA_NORTH_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        136, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_MOGALL_FOREST_FIRST_AREA_ATTACKING_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        376, 0, 432, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_MOGALL_FOREST_FIRST_AREA_EAST_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        504, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_MOGALL_FOREST_NUT_CAUGHT, 7,
        632, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gMogallForestQuartz_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        592, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        592, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        648, 0, 512, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, (s32)gMogallForest_RockScript,
        584, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_APE, FLAG_MOGALL_FOREST_SECOND_AREA_WEST_APE_OUT, (s32)gMogallForest_ActorScript,
        360, 0, 80, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_APE, FLAG_MOGALL_FOREST_SECOND_AREA_EAST_APE_OUT, (s32)gMogallForest_ActorScript,
        488, 0, 80, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_APE, FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_OUT, (s32)gMogallForest_ActorScript,
        504, 0, 480, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_MOGALL_FOREST_SECOND_AREA_WEST_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        360, 0, 80, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_MOGALL_FOREST_SECOND_AREA_EAST_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        488, 0, 80, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        504, 0, 480, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_QUARTZ), MOVEMENT_TYPE_STAND,
        488, 0, 568, FACING_EAST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gMogallForestKillerApe_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        680, 0, 96, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        616, 0, 400, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        552, 0, 480, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        600, 0, 544, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        544, 0, 536, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, (s32)gMogallForest_RockScript,
        696, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, (s32)gMogallForest_RockScript,
        632, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_APE, FLAG_MOGALL_FOREST_THIRD_AREA_ATTACKING_APE_OUT, (s32)gMogallForest_ActorScript,
        488, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_APE, FLAG_MOGALL_FOREST_THIRD_AREA_MIDDLE_APE_OUT, (s32)gMogallForest_ActorScript,
        424, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_APE, FLAG_MOGALL_FOREST_THIRD_AREA_WEST_APE_OUT, (s32)gMogallForest_ActorScript,
        360, 0, 80, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_APE, MOGALL_FOREST_EAST_ACTOR_CONDITION, (s32)gMogallForest_ActorScript,
        744, 0, 400, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_MOGALL_FOREST_THIRD_AREA_ATTACKING_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        488, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_MOGALL_FOREST_THIRD_AREA_MIDDLE_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        424, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_MOGALL_FOREST_THIRD_AREA_WEST_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        360, 0, 80, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLANK, MOGALL_FOREST_EAST_SHARED_SPRITE_CONDITION, (s32)gMogallForest_HidingPlaceScript,
        744, 0, 400, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    { SCENE_TABLE_END },
};

struct ActorPlacement gMogallForestSidePaths_ActorPlacements[] = {
    { SCENE_TABLE_END },
};

struct Trigger gMogallForestSign_Triggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, MOGALL_FOREST_SIGN_TO_WORLD_MAP },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_SIDE_PATHS_WEST },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_SIDE_PATHS_NORTH },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, MOGALL_FOREST_SIGN_ENTRANCE_TO_SOUTHWEST_CLEARING },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_TO_ENTRANCE },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_TO_RIVER },
    { TRIGGER_EXIT, 7, CONDITION_ALWAYS, MOGALL_FOREST_SIGN_RIVER_TO_SOUTHWEST_CLEARING },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_SIDE_PATHS_EAST },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_QUARTZ },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_MOGALL_FOREST_LOG, CONDITION_ALWAYS, EVENT(MogallForest_FirstArea_PushLog) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_MOGALL_FOREST_JUMP, CONDITION_ALWAYS, EVENT(MogallForest_FirstArea_JumpNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_MOGALL_FOREST_JUMP_SOUTH, CONDITION_ALWAYS, EVENT(MogallForest_FirstArea_JumpSouth) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_MOGALL_FOREST_BROKEN_SIGN, CONDITION_ALWAYS, EVENT(MogallForest_ReadBrokenSign) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_FIRST_AREA_NORTH_APE_STUMP, FLAG_MOGALL_FOREST_FIRST_AREA_NORTH_APE_OUT, EVENT(MogallForest_FirstArea_NorthApeShowsWay) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_FIRST_AREA_ATTACKING_APE_STUMP, FLAG_MOGALL_FOREST_FIRST_AREA_ATTACKING_APE_OUT, EVENT(MogallForest_FirstArea_ApeAttacks) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_FIRST_AREA_EAST_APE_STUMP, FLAG_MOGALL_FOREST_FIRST_AREA_EAST_APE_OUT, EVENT(MogallForest_FirstArea_EastApeShowsWay) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_NUT, FLAG_MOGALL_FOREST_NUT_CAUGHT, EVENT(MogallForest_CatchNut) },
    { TRIGGER_TABLE_END },
};

struct Trigger gMogallForestQuartz_Triggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_SIGN },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_NORTHWEST_CLEARING_TO_STUMPS },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_SIDE_PATHS_NORTH },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_STUMPS_TO_NORTHWEST_CLEARING },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_STUMPS_TO_POND },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_STUMPS_TO_NORTHEAST_CLEARING },
    { TRIGGER_EXIT, 7, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_TO_STUMPS },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_TO_RIVER },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_POND_TO_STUMPS },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_RIVER_TO_NORTHEAST_CLEARING },
    { TRIGGER_EXIT, 11, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_KILLER_APE },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_SIDE_PATHS_SOUTH },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_MOGALL_FOREST_LOG, CONDITION_ALWAYS, EVENT(MogallForest_SecondArea_PushLog) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_AREA_ROCK, CONDITION_ALWAYS, EVENT(MogallForest_SecondArea_UpdateRock) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_SECOND_AREA_WEST_APE_STUMP, FLAG_MOGALL_FOREST_SECOND_AREA_WEST_APE_OUT, EVENT(MogallForest_SecondArea_WestApeShowsWay) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_SECOND_AREA_EAST_APE_STUMP, FLAG_MOGALL_FOREST_SECOND_AREA_EAST_APE_OUT, EVENT(MogallForest_SecondArea_EastApeShowsWay) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_SECOND_AREA_SOUTH_APE_STUMP, FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_MOVED, EVENT(MogallForest_SecondArea_SouthApeHidesAgain) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_SECOND_AREA_SOUTH_APE_STUMP, FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_OUT, EVENT(MogallForest_SecondArea_SouthApeShowsWay) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_MOGALL_FOREST_APPLE_CHEST, FIND_ITEM(ITEM_APPLE) },
    { TRIGGER_TALK, ACTOR_SECOND_AREA_QUARTZ, CONDITION_ALWAYS, EVENT(MogallForest_StartDjinnBattle) },
    { TRIGGER_TABLE_END },
};
// clang-format on

struct Trigger gMogallForestKillerApe_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_NORTHWEST_CLEARING_TO_STUMPS},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_QUARTZ},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_SIDE_PATHS_WEST},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_STUMPS_TO_CHEST},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_STUMPS_TO_NORTHWEST_CLEARING},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_STUMPS_TO_RIVER},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_CHEST_TO_STUMPS},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_RIVER_TO_STUMPS},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_SIDE_PATHS_EAST},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_SIDE_PATHS_SOUTH},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_RIVER_TO_BRIDGES},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_BRIDGES_TO_RIVER},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, MOGALL_FOREST_KILLER_APE_TO_WORLD_MAP},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_MOGALL_FOREST_LOG, CONDITION_ALWAYS,
     EVENT(MogallForest_ThirdArea_PushLog)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_MOGALL_FOREST_JUMP,
     CONDITION_ALWAYS, EVENT(MogallForest_ThirdArea_JumpWest)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MOGALL_FOREST_BESIDE_LONE_STUMP,
     FLAG_MOGALL_FOREST_LED_TO_KILLER_APE,
     EVENT(MogallForest_ApeLeadsToKillerApe)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MOGALL_FOREST_BELOW_LONE_STUMP,
     FLAG_MOGALL_FOREST_KILLER_APE_BEATEN,
     EVENT(MogallForest_KillerApeAttacks)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_AREA_NORTH_ROCK,
     CONDITION_ALWAYS, EVENT(MogallForest_ThirdArea_UpdateNorthRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_AREA_SOUTH_ROCK,
     FLAG_MOGALL_FOREST_SOUTH_ROCK_SETTLED,
     EVENT(MogallForest_ThirdArea_UpdateSouthRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_THIRD_AREA_ATTACKING_APE_STUMP,
     FLAG_MOGALL_FOREST_THIRD_AREA_ATTACKING_APE_OUT,
     EVENT(MogallForest_ThirdArea_ApeAttacks)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_THIRD_AREA_MIDDLE_APE_STUMP,
     FLAG_MOGALL_FOREST_THIRD_AREA_MIDDLE_APE_OUT,
     EVENT(MogallForest_ThirdArea_MiddleApeShowsWay)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_THIRD_AREA_WEST_APE_STUMP,
     FLAG_MOGALL_FOREST_THIRD_AREA_WEST_APE_OUT,
     EVENT(MogallForest_ThirdArea_WestApeShowsWay)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_THIRD_AREA_EAST_APE_STUMP,
     FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_MOVED,
     EVENT(MogallForest_ThirdArea_EastApeHidesAgain)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_THIRD_AREA_EAST_APE_STUMP,
     FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT,
     EVENT(MogallForest_ThirdArea_EastApeShowsWay)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_MOGALL_FOREST_ELVEN_SHIRT_CHEST,
     FIND_ITEM(ITEM_ELVEN_SHIRT)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMogallForestSidePaths_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_SIGN_NORTH},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_SIGN_WEST},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_QUARTZ_NORTH},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_QUARTZ_SOUTH},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_KILLER_APE_WEST},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_KILLER_APE_EAST},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_KILLER_APE_SOUTH},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_SIGN_EAST},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_WEST_CLEARING_WEST},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_NORTH_CLEARING_EAST},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_CENTER_CLEARING},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_EAST_CLEARING},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_SOUTHWEST_CLEARING},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_SOUTH_CLEARING_NORTHWEST},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_SOUTH_CLEARING_SOUTHWEST},
    {TRIGGER_EXIT, 18, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_SOUTHEAST_CLEARING},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 100, FLAG_MOGALL_FOREST_PSYNERGY_STONE,
     FIND_PSYNERGY_STONE},
    {TRIGGER_TABLE_END},
};
