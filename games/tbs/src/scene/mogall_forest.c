/// @file
/// Mogall Forest (SceneId_MogallForestSign to SceneId_MogallForestSidePaths),
/// the woods between Fuchin Temple and Xian, which the party finds its way
/// through by using Force, learned from Nyunpa, on the stumps the apes hide in.
///
/// The way runs from the first area (SceneId_MogallForestSign), entered from
/// the World Map, through the second (SceneId_MogallForestQuartz) to the third
/// (SceneId_MogallForestKillerApe), which leaves for the World Map by
/// MOGALL_FOREST_KILLER_APE_TO_WORLD_MAP. Apes hide in hollow purple stumps,
/// each stump holding a blank actor on its spot, and Force on a stump
/// brings its ape out. Most turn to the leader, point the way on and leave; two
/// first bolt to a second hiding place; and one in the first area and one in
/// the third leap at the leader instead, starting a battle with an Ape
/// (ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, slot 0). An ape stays gone
/// once out, and logs pushed into the river stay there, for as long as the
/// party is in the forest (area flags from FLAG_AREA_TEMP_00).
///
/// - First area (SceneId_MogallForestSign). Arriving the first time, an ape
///   peeks out at the leader and hides (FLAG_MOGALL_FOREST_FIRST_APE_SEEN). A
///   broken sign reads `North: Fuchin Temple South: Mogall Prairie...`, a log
///   pushed to the river rolls in and can be crossed, and a Nut floats over the
///   river for Catch.
/// - Second area (SceneId_MogallForestQuartz). Quartz, a Venus Djinni, waits
///   to be fought; one log changes the ground beside it and another rolls into
///   the river; a rock moved with Move settles into place; a chest holds an
///   Apple.
/// - Third area (SceneId_MogallForestKillerApe). Two logs roll into the river,
///   two rocks settle with Move, and a chest holds an Elven Shirt. Touching
///   cell behavior 22 the first time, the east ape leads the party south to
///   a hiding place (FLAG_MOGALL_FOREST_LED_TO_KILLER_APE); there (cell
///   behavior 23) the Killer Ape bursts out to fight
///   (ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, slot 1), and coming back at
///   MOGALL_FOREST_KILLER_APE_AFTER_KILLER_APE_ATTACKS marks it beaten
///   (FLAG_MOGALL_FOREST_KILLER_APE_BEATEN). Entering at
///   MOGALL_FOREST_KILLER_APE_BRIDGES_FROM_RIVER or
///   MOGALL_FOREST_KILLER_APE_FROM_WORLD_MAP once Force is learned and the
///   Killer Ape beaten sets FLAG_NYUNPA_ENLIGHTENED: back at Fuchin Temple the
///   monks say that Nyunpa has reached enlightenment and passed away.
/// - SceneId_MogallForestSidePaths, also named Mogall Forest: side paths that
///   the three areas' exits lead into and back out of, some of them leading
///   back into the scene itself; a Psynergy Stone lies there.
///
/// One overlay serves all four scenes: MogallForest_GetEntrances,
/// _GetActorPlacements and _GetTriggers choose each scene's tables, and
/// MogallForest_Initialize puts back the apes already out, the rolled logs and
/// the settled rocks, scene by scene and entrance by entrance. The
/// third area's east ape is also the guide at cell behavior 22 and the
/// Killer Ape, and the editions differ in how it is kept out of sight once it
/// has come out (the #if blocks below and in MogallForest_Initialize).
///
/// The apes and rocks are told apart by colour mode (enum SpriteColorMode):
/// an ape in hiding is SPRITE_COLOR_HIDDEN, a guide that comes out is drawn
/// with its red and green swapped (SPRITE_COLOR_GRB) and an ape that attacks
/// keeps SPRITE_APE's own colours, while the Killer Ape is the same sprite a
/// fifth larger with its colours rotated (SPRITE_COLOR_BRG). The rocks Move
/// settles are SPRITE_TURNING_ROCK drawn in greys (SPRITE_COLOR_GREY).

#include "script_operands.h"
#include "main_routine_work.h"
#include "system_call.h"
#include "heap_slots.h"
#include "module/import.h"
#include "module/scene/mogall_forest.h"
#include "module/scene/shared/actor_orbit.h"
#include "module/scene/shared/field_effect_spawn.h"
#include "module/field/field_sequence.h"
#include "module/field/event_commands.h"

#define FIELD_STAGED_ACTOR_IMPORTS
#include "actor.h"
#include "edition.h"
#include "types.h"
#include "types.h"
#include "event.h"
#include "scene_tables.h"
#include "djinn_ids.h"
#include "field_effect.h"
#include "pushed_actor.h"
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
#include "edition.h"
#include "call.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "actor_script.h"
#include "scene.h"
#include "fixed_math.h"
#include "vector3.h"
#include "flags.h"
#include "item_ids.h"
#include "sounds.h"
#include "heap.h"

/// Actors of the first area (SceneId_MogallForestSign), numbered in the
/// order of gMogallForestSign_ActorPlacements.
enum MogallForestSignActorId {
  /// A log lying north to south at (488, 464), pushed as the river log is but
  /// with nothing happening where it stops.
  /// ★ Named from what the player sees: a fallen tree trunk lying in the
  /// grass (its render)
  ACTOR_FIRST_AREA_LOG = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: a fallen tree trunk lying in the
  /// grass (its render)
  ACTOR_FIRST_AREA_RIVER_LOG,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  ACTOR_FIRST_AREA_NORTH_APE,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  ACTOR_FIRST_AREA_ATTACKING_APE,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  ACTOR_FIRST_AREA_EAST_APE,
  /// ★ Named from what the player sees: a hollow purple stump an ape hides in
  /// (its render)
  ACTOR_FIRST_AREA_NORTH_APE_STUMP,
  /// ★ Named from what the player sees: a hollow purple stump an ape hides in
  /// (its render)
  ACTOR_FIRST_AREA_ATTACKING_APE_STUMP,
  /// ★ Named from what the player sees: a hollow purple stump an ape hides in
  /// (its render)
  ACTOR_FIRST_AREA_EAST_APE_STUMP,
  /// A Nut floating over the river, caught with Catch.
  /// ★ Named from the game: "Nut" (MsgItemName)
  ACTOR_NUT
};

/// Actors of the second area (SceneId_MogallForestQuartz), numbered in
/// the order of gMogallForestQuartz_ActorPlacements.
enum MogallForestQuartzActorId {
  /// ★ Named from what the player sees: a fallen tree trunk lying in the
  /// grass (its render)
  ACTOR_SECOND_AREA_LOG = ACTOR_FIRST_PLACED,
  /// A log lying west to east at (592, 456), south of ACTOR_SECOND_AREA_LOG,
  /// pushed with nothing happening where it stops.
  /// ★ Named from what the player sees: a fallen tree trunk lying in the
  /// grass (its render)
  ACTOR_SECOND_AREA_SOUTH_LOG,
  /// ★ Named from what the player sees: a fallen tree trunk lying in the
  /// grass (its render)
  ACTOR_SECOND_AREA_RIVER_LOG,
  /// A rock moved with Move, drawn as the rocks of Altmiller Cave's color
  /// puzzle (MsgAltmillerRotatedRock), drawn plain as those are while Reveal is
  /// off.
  /// ★ Named from what the player sees: a small grey rock in the grass
  /// (its render)
  ACTOR_SECOND_AREA_ROCK,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  ACTOR_SECOND_AREA_NORTH_APE,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  ACTOR_SECOND_AREA_EAST_APE,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  ACTOR_SECOND_AREA_SOUTH_APE,
  /// ★ Named from what the player sees: a hollow purple stump an ape hides in
  /// (its render)
  ACTOR_SECOND_AREA_NORTH_APE_STUMP,
  /// ★ Named from what the player sees: a hollow purple stump an ape hides in
  /// (its render)
  ACTOR_SECOND_AREA_EAST_APE_STUMP,
  /// ★ Named from what the player sees: a hollow purple stump an ape hides in
  /// (its render)
  ACTOR_SECOND_AREA_SOUTH_APE_STUMP,
  /// Quartz, a Venus Djinni, placed until the party has it.
  /// ★ Named from the game: "Quartz" (MsgDjinnName)
  ACTOR_SECOND_AREA_QUARTZ
};

/// Actors of the third area (SceneId_MogallForestKillerApe), numbered in
/// the order of gMogallForestKillerApe_ActorPlacements.
enum MogallForestKillerApeActorId {
  /// ★ Named from what the player sees: a fallen tree trunk lying in the
  /// grass (its render)
  ACTOR_THIRD_AREA_LOG = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: a fallen tree trunk lying in the
  /// grass (its render)
  ACTOR_THIRD_AREA_NORTH_RIVER_LOG,
  /// A log lying north to south at (552, 480), west of the two river logs,
  /// pushed with nothing happening where it stops.
  /// ★ Named from what the player sees: a fallen tree trunk lying in the
  /// grass (its render)
  ACTOR_THIRD_AREA_WEST_LOG,
  /// ★ Named from what the player sees: a fallen tree trunk lying in the
  /// grass (its render)
  ACTOR_THIRD_AREA_SOUTH_RIVER_LOG,
  /// A log lying west to east at (544, 536), south of
  /// ACTOR_THIRD_AREA_WEST_LOG, pushed with nothing happening where it stops.
  /// ★ Named from what the player sees: a fallen tree trunk lying in the
  /// grass (its render)
  ACTOR_THIRD_AREA_SOUTHWEST_LOG,
  /// Two rocks moved with Move, as in the second area.
  /// ★ Named from what the player sees: a small grey rock in the grass
  /// (its render)
  ACTOR_THIRD_AREA_NORTH_ROCK,
  /// ★ Named from what the player sees: a small grey rock in the grass
  /// (its render)
  ACTOR_THIRD_AREA_SOUTH_ROCK,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  ACTOR_THIRD_AREA_ATTACKING_APE,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  ACTOR_THIRD_AREA_MIDDLE_APE,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  ACTOR_THIRD_AREA_WEST_APE,
  /// Also the Killer Ape and its guide.
  /// ★ Named from the game: "Killer Ape" (MsgEnemyName)
  ACTOR_THIRD_AREA_EAST_APE,
  /// ★ Named from what the player sees: a hollow purple stump an ape hides in
  /// (its render)
  ACTOR_THIRD_AREA_ATTACKING_APE_STUMP,
  /// ★ Named from what the player sees: a hollow purple stump an ape hides in
  /// (its render)
  ACTOR_THIRD_AREA_MIDDLE_APE_STUMP,
  /// ★ Named from what the player sees: a hollow purple stump an ape hides in
  /// (its render)
  ACTOR_THIRD_AREA_WEST_APE_STUMP,
  /// ★ Named from what the player sees: a hollow purple stump an ape hides in
  /// (its render)
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

/// Kept until the party leaves the forest: each ape once it has come out
/// (or, for two, moved on to a second hiding place) and each rolled log.
enum {
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  FLAG_MOGALL_FOREST_FIRST_AREA_NORTH_APE_OUT = FLAG_AREA_TEMP_00,
  /// The ape that leaps at the leader instead of pointing the way.
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  FLAG_MOGALL_FOREST_FIRST_AREA_ATTACKING_APE_OUT,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  FLAG_MOGALL_FOREST_FIRST_AREA_EAST_APE_OUT,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  FLAG_MOGALL_FOREST_SECOND_AREA_NORTH_APE_OUT,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  FLAG_MOGALL_FOREST_SECOND_AREA_EAST_APE_OUT,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_MOVED,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_OUT,
  /// The ape that leaps at the leader instead of pointing the way.
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  FLAG_MOGALL_FOREST_THIRD_AREA_ATTACKING_APE_OUT,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  FLAG_MOGALL_FOREST_THIRD_AREA_MIDDLE_APE_OUT,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  FLAG_MOGALL_FOREST_THIRD_AREA_WEST_APE_OUT,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_MOVED,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT,
  /// ★ Named from what the player sees: a log pushed over the grass rolls into
  /// the river and lies across it (its render)
  FLAG_MOGALL_FOREST_FIRST_AREA_LOG_ROLLED = FLAG_AREA_TEMP_10,
  /// ★ Named from what the player sees: the second area's log rolled into the
  /// river (its render)
  FLAG_MOGALL_FOREST_SECOND_AREA_LOG_ROLLED,
  /// ★ Named from what the player sees: the third area's north log rolled into
  /// the river (its render)
  FLAG_MOGALL_FOREST_THIRD_AREA_NORTH_LOG_ROLLED,
  /// ★ Named from what the player sees: the third area's south log rolled
  /// into the river (its render)
  FLAG_MOGALL_FOREST_THIRD_AREA_SOUTH_LOG_ROLLED,
  /// ★ Named from the game: "a Psynergy Stone" (MsgGotPsynergyStone)
  FLAG_MOGALL_FOREST_PSYNERGY_STONE = FLAG_AREA_TEMP_50
};

/// Scene flag, kept while the party stays in the area.
enum {
  /// ★ Named from what the player sees: the rock beside the log, moved with
  /// Move (its render)
  FLAG_MOGALL_FOREST_SOUTH_ROCK_SETTLED = FLAG_SCENE_TEMP_14
};

// The third area's east ape: in the first patch it is placed only until it
// has come out, and from the first patch on the hiding place that shares its
// sprite too.
#if TBS_PATCH_LEVEL == 1
#define MOGALL_FOREST_EAST_ACTOR_CONDITION                                     \
  FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT
#else
#define MOGALL_FOREST_EAST_ACTOR_CONDITION CONDITION_ALWAYS
#endif

#if !TBS_PATCH_1
#define MOGALL_FOREST_EAST_SHARED_SPRITE_CONDITION CONDITION_ALWAYS
#else
#define MOGALL_FOREST_EAST_SHARED_SPRITE_CONDITION                             \
  FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT
#endif

#include "shared/staged_actor.h"

#include "shared/field_effect_create.h"
#include "shared/field_effect_spawn.h"

/// Mogall Forest, the second area: talking to Quartz, a Venus Djinni, starts
/// the Djinni battle.
static void MogallForest_StartDjinnBattle(void) {
  Field_RequestDjinnBattle(ACTOR_SECOND_AREA_QUARTZ, DJINNI_QUARTZ);
}

/// Jumps the leader to an unobstructed fixed-point position and restores
/// motion.
static s32 MogallForest_TryJumpLeaderTo(struct FieldPosition *position) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  u8 *motionFlags = &leader->motionFlags;
  s32 savedMotionFlags = *motionFlags;

  s32 collision = FieldActor_CheckMovementCollision(leader, position);

  if (collision == 0) {
    s32 jumpMotionFlags;

    Event_Start();
    Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(leader, ANIM_LEADER_LEAP);
    leader->speedLimit = PIXELS(3);
    leader->acceleration = PIXELS(2);
    leader->velocityY = PIXELS(4);
    jumpMotionFlags = 0x7e;
    jumpMotionFlags &= *motionFlags;
    *motionFlags = jumpMotionFlags;
    Actor_SetSpriteFlags(leader, 0);
    Actor_SetPositionAndCommit(ACTOR_PARTY_LEADER,
                               (s16)((u32)position->x >> 16),
                               (s16)((u32)position->z >> 16));
    Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
    *motionFlags = savedMotionFlags;
    Event_End();
    return 1;
  }
  return 0;
}

/// An actor script call: draws the actor in the colour mode its timer holds.
static s32 MogallForest_CopyTimerToChild(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  Actor_SetSpriteColor(actor, actor->timer);
  return 0;
}

/// Clears the actor's sprite flags when its script requests it.
static s32 MogallForest_ClearActorSpriteFlags(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

/// Hops an actor to a pixel destination with the requested vertical
/// speed.
static void MogallForest_HopActorTo(s32 actorId, s32 targetX, s32 targetZ,
                                    s32 verticalSpeed) {
  struct Actor *actor;

  actor = Actor_GetById(actorId);
  Actor_SetSpritePriority(actorId, 1);
  Actor_SetSpeed(actorId, PIXELS(3), PIXELS(3) / 2);
  Sound_Play(SE_JUMP);
  actor->velocityY = verticalSpeed;
  actor->gravity = PIXELS(1) / 2;
  actor->bounce = 0;
  Actor_SetSpriteFlags(actor, 0);
  Actor_MoveToAndWait(actorId, targetX, targetZ);
  Actor_SetPosition(actorId, PIXELS(targetX), PIXELS(targetZ));
  Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  actor->gravity = PIXELS(1);
}

/// Scatters a complete ring of rising puffs around an actor.
static void MogallForest_SpawnPuffRing(s32 id) {
  struct Vector3 velocity;
  struct EffectOptions optionsStorage;
  struct EffectOptions *options;
  struct Vector3 *emissionVelocity;
  struct Actor *actor;
  u32 directionIndex;
  s32 x;
  s32 z;

  actor = Actor_GetById(id);
  Sound_Play(SE_THUMP);
  options = &optionsStorage;
  options->priority = 1;
  for (directionIndex = 0; directionIndex <= 16; directionIndex++) {
    emissionVelocity = &velocity;
    emissionVelocity->x = Trig_Cos(directionIndex << 12);
    emissionVelocity->y = 0;
    z = Trig_Sin(directionIndex << 12);
    x = emissionVelocity->x;
    emissionVelocity->z = z;
    x += x / 3;
    emissionVelocity->x = x;
    FieldEffect_Spawn(actor->x.fixed, PIXELS(16), actor->z.fixed, x,
                      emissionVelocity->y + PIXELS(1) / 10, z,
                      EFFECT_USE_PRIORITY, options);
  }
}

/// The camera goes to the ape's hiding place, which bursts in a ring of
/// puffs, and the ape shows itself in colour mode color: SPRITE_COLOR_GRB for
/// a guide, SPRITE_COLOR_NORMAL for one that attacks.
static void MogallForest_ApeComesOut(s32 actorId, s32 color) {
  s32 cameraSpeed = PIXELS(2);
  s32 cameraAcceleration = PIXELS(1) / 4;

  FieldCamera_SetSpeedParameters(cameraSpeed, cameraAcceleration);
  Actor_PlaceCurrentWithinCameraBounds(actorId, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(30);
  MogallForest_SpawnPuffRing(actorId);
  Actor_ConfigureSpriteColor(actorId, color);
}

/// The ape turns to the leader, hops and gestures down the way to go.
static inline void MogallForest_ApePointsTheWay(s32 guideId) {
  FieldCamera_FollowActor(guideId, 1);
  Actor_TurnToFaceEachOther(guideId, ACTOR_PARTY_LEADER, 0);
  Event_Wait(20);
  Actor_StartRepeatedMotion(guideId, 2);
  FieldEffect_SetAttached(guideId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
}

const struct SceneEntrance *MogallForest_GetEntrances(void) {
  s16 selector = gMainRoutineWork.sceneId;

  if (selector == (s32)&SceneId_MogallForestSign) {
    return gMogallForestSign_Entrances;
  }
  if (selector == (s32)&SceneId_MogallForestQuartz) {
    return gMogallForestQuartz_Entrances;
  }
  if (selector == (s32)&SceneId_MogallForestKillerApe) {
    return gMogallForestKillerApe_Entrances;
  }
  return gMogallForestSidePaths_Entrances;
}

const struct SceneExitRegion *MogallForest_GetRegions(void) { return NULL; }

/// Returns the exits, one table for all four scenes.
const u32 *MogallForest_GetExits(void) { return gMogallForest_Exits; }

const struct ActorPlacement *MogallForest_GetActorPlacements(void) {
  s16 selector = gMainRoutineWork.sceneId;

  if (selector == (s32)&SceneId_MogallForestSign) {
    return gMogallForestSign_ActorPlacements;
  }
  if (selector == (s32)&SceneId_MogallForestQuartz) {
    return gMogallForestQuartz_ActorPlacements;
  }
  if (selector == (s32)&SceneId_MogallForestKillerApe) {
    return gMogallForestKillerApe_ActorPlacements;
  }
  return gMogallForestSidePaths_ActorPlacements;
}

/// Mogall Forest, the first area: walking into a log (cell behavior 20)
/// pushes it; the river log pushed to row 26 rolls into the water and can be
/// crossed. Sets FLAG_MOGALL_FOREST_FIRST_AREA_LOG_ROLLED.
static void MogallForest_FirstArea_PushLog(void) {
  struct PushedActorProbe probe;

  Event_Start();
  if (PushedActor_FindClearPosition(&probe) != 0) {
    Actor_MoveAndRedraw(probe);
    if (probe.actorSlot == ACTOR_FIRST_AREA_RIVER_LOG &&
        (probe.position.z >> 20) == 26) {
      GameFlag_Set(FLAG_MOGALL_FOREST_FIRST_AREA_LOG_ROLLED);
      Import_ActorSetAnimation(ACTOR_FIRST_AREA_RIVER_LOG,
                               MOGALL_FOREST_LOG_ROLLING);
      Actor_SetSpeed(ACTOR_FIRST_AREA_RIVER_LOG, PIXELS(1) / 4, PIXELS(1) / 2);
      Actor_OffsetPositionAndResetMotion(ACTOR_FIRST_AREA_RIVER_LOG, 0, -16);
      Event_Wait(45);
      Import_ActorSetAnimation(ACTOR_FIRST_AREA_RIVER_LOG,
                               MOGALL_FOREST_LOG_FALLING_IN);
      Sound_Play(SE_LOG_FALL);
      Actor_SetSpritePriority(ACTOR_FIRST_AREA_RIVER_LOG, 1);
      Actor_GetById(ACTOR_FIRST_AREA_RIVER_LOG)->priorityFlags =
          ACTOR_PRIORITY_UNDERFOOT;
      MapGrid_CopyCellAttributes(38, 27, 4, 2, 31, 25);
    }
  }
  Event_End();
}

/// Mogall Forest, the first area: walking north into cell behavior 25 jumps
/// the leader north across the log.
static void MogallForest_FirstArea_JumpNorth(void) {
  struct FieldPosition position;
#if TBS_PATCH_1
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);

  position.x = (leader->x.fixed & 0xfff00000) + PIXELS(8);
  position.y = leader->y.fixed;
  position.z = (leader->z.fixed & 0xfff00000) - PIXELS(24);
#else
  position.x = Actor_GetById(ACTOR_PARTY_LEADER)->x.fixed;
  position.y = Actor_GetById(ACTOR_PARTY_LEADER)->y.fixed;
  position.z = Actor_GetById(ACTOR_PARTY_LEADER)->z.fixed - PIXELS(32);
#endif
  MogallForest_TryJumpLeaderTo(&position);
}

/// Mogall Forest, the first area: walking south into cell behavior 26 jumps
/// the leader onto the log, which bobs under the weight.
static void MogallForest_FirstArea_JumpSouth(void) {
  struct FieldPosition position;
  struct Actor *actor = Actor_GetById(ACTOR_PARTY_LEADER);
  u8 *motionFlags = &actor->motionFlags;
  s32 savedMotionFlags = *motionFlags;

#if TBS_PATCH_1
  position.x = (actor->x.fixed & 0xfff00000) + PIXELS(8);
  position.y = actor->y.fixed;
  position.z = (actor->z.fixed & 0xfff00000) + PIXELS(40);
#else
  position.x = Actor_GetById(ACTOR_PARTY_LEADER)->x.fixed;
  position.y = Actor_GetById(ACTOR_PARTY_LEADER)->y.fixed;
  position.z = Actor_GetById(ACTOR_PARTY_LEADER)->z.fixed + PIXELS(32);
#endif
  if (MogallForest_TryJumpLeaderTo(&position) != 0) {
    Event_Start();
    *motionFlags = 0;
    Import_ActorSetAnimation(ACTOR_FIRST_AREA_RIVER_LOG,
                             MOGALL_FOREST_LOG_STEPPED_ON);
    actor->y.fixed += -PIXELS(1);
    actor->groundHeight += -PIXELS(1);
    wait(2);
    actor->y.fixed += -PIXELS(1);
    actor->groundHeight += -PIXELS(1);
    wait(10);
    actor->y.fixed += PIXELS(1);
    actor->groundHeight += PIXELS(1);
    wait(4);
    actor->y.fixed += PIXELS(1);
    actor->groundHeight += PIXELS(1);
    *motionFlags = savedMotionFlags;
    Event_End();
  }
}

/// Mogall Forest, the first area: searching the sign (cell behavior 27).
/// "The broken sign reads: North: Fuchin Temple / South: Mogall Prairie..."
static void MogallForest_ReadBrokenSign(void) {
  Event_Start();
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  FieldMessage_Show(MSGNO(MsgMogallForestBrokenSign), 1);
  Event_End();
}

/// Mogall Forest, the first area: Force on the north hiding place. The ape
/// comes out, points the way and hops off south.
static void MogallForest_FirstArea_NorthApeShowsWay(void) {
  struct Actor *creature;

  creature = Actor_GetById(ACTOR_FIRST_AREA_NORTH_APE);

  Event_Start();

  MogallForest_ApeComesOut(ACTOR_FIRST_AREA_NORTH_APE, SPRITE_COLOR_GRB);
  MogallForest_HopActorTo(ACTOR_FIRST_AREA_NORTH_APE, 88, 120, PIXELS(6));

  FieldEffect_Spawn(creature->x.fixed, creature->y.fixed,
                    creature->z.fixed + PIXELS(24), 0, 0, 0,
                    EFFECT_SCRIPT_MEDIUM, 0);

  MogallForest_ApePointsTheWay(ACTOR_FIRST_AREA_NORTH_APE);

  MogallForest_HopActorTo(ACTOR_FIRST_AREA_NORTH_APE, 88, 152, PIXELS(3));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_NORTH_APE, 0);
  Event_Wait(10);

  MogallForest_HopActorTo(ACTOR_FIRST_AREA_NORTH_APE, 120, 192, PIXELS(3));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_NORTH_APE, 0);
  Event_Wait(10);

  MogallForest_HopActorTo(ACTOR_FIRST_AREA_NORTH_APE, 120, 240, PIXELS(3));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_NORTH_APE, 0);
  Event_Wait(10);

  GameFlag_Set(FLAG_MOGALL_FOREST_FIRST_AREA_NORTH_APE_OUT);
  Import_ActorSetPosition(ACTOR_FIRST_AREA_NORTH_APE_STUMP, 0, 0);
  Import_ActorSetPosition(ACTOR_FIRST_AREA_NORTH_APE, 0, 0);

  Event_End();
}

/// Mogall Forest, the first area: Force on the attacking ape's hiding place. It
/// comes out angry, leaps at the leader and the battle with an Ape begins
/// (ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, slot 0).
static void MogallForest_FirstArea_ApeAttacks(void) {
  struct Actor *creature;

  creature = Actor_GetById(ACTOR_FIRST_AREA_ATTACKING_APE);

  Event_Start();

  MogallForest_ApeComesOut(ACTOR_FIRST_AREA_ATTACKING_APE, SPRITE_COLOR_NORMAL);
  MogallForest_HopActorTo(ACTOR_FIRST_AREA_ATTACKING_APE, 408, 456, PIXELS(6));

  FieldEffect_Spawn(creature->x.fixed, creature->y.fixed,
                    creature->z.fixed + PIXELS(24), 0, 0, 0,
                    EFFECT_SCRIPT_MEDIUM, 0);

  FieldCamera_FollowActor(ACTOR_FIRST_AREA_ATTACKING_APE, 1);
  Actor_TurnToFaceEachOther(ACTOR_FIRST_AREA_ATTACKING_APE, ACTOR_PARTY_LEADER,
                            0);
  Event_Wait(30);
  Actor_StartRepeatedMotion(ACTOR_FIRST_AREA_ATTACKING_APE, 2);
  FieldEffect_SpawnAttached(ACTOR_FIRST_AREA_ATTACKING_APE,
                            EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Sound_Play(SE_BEAST_CRY);
  Event_Wait(60);

  MogallForest_HopActorTo(ACTOR_FIRST_AREA_ATTACKING_APE,
                          Actor_GetById(ACTOR_PARTY_LEADER)->x.part.pixel,
                          Actor_GetById(ACTOR_PARTY_LEADER)->z.part.pixel,
                          PIXELS(4));

  Event_Wait(10);
  GameFlag_Set(FLAG_MOGALL_FOREST_FIRST_AREA_ATTACKING_APE_OUT);
  Import_ActorSetPosition(ACTOR_FIRST_AREA_ATTACKING_APE_STUMP, 0, 0);

  Battle_SetType(BATTLE_TYPE_SCRIPTED);

  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 0);

  Event_End();
}

/// Mogall Forest, the first area: Force on the east hiding place. The ape
/// comes out, points the way and hops off east.
static void MogallForest_FirstArea_EastApeShowsWay(void) {
  struct Actor *creature;

  creature = Actor_GetById(ACTOR_FIRST_AREA_EAST_APE);

  Event_Start();

  MogallForest_ApeComesOut(ACTOR_FIRST_AREA_EAST_APE, SPRITE_COLOR_GRB);
  MogallForest_HopActorTo(ACTOR_FIRST_AREA_EAST_APE, 536, 344, PIXELS(7));

  FieldEffect_Spawn(creature->x.fixed, creature->y.fixed,
                    creature->z.fixed + PIXELS(16), 0, 0, 0,
                    EFFECT_SCRIPT_MEDIUM, 0);

  MogallForest_ApePointsTheWay(ACTOR_FIRST_AREA_EAST_APE);

  MogallForest_HopActorTo(ACTOR_FIRST_AREA_EAST_APE, 584, 344, PIXELS(3));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_EAST_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_FIRST_AREA_EAST_APE, 632, 344, PIXELS(3));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_EAST_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_FIRST_AREA_EAST_APE, 680, 344, PIXELS(3));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_EAST_APE, 0);
  Event_Wait(6);

  GameFlag_Set(FLAG_MOGALL_FOREST_FIRST_AREA_EAST_APE_OUT);
  Import_ActorSetPosition(ACTOR_FIRST_AREA_EAST_APE_STUMP, 0, 0);

  Event_End();
}

/// Mogall Forest, the first area: Catch on the Nut floating over the river
/// brings it to the party. Sets FLAG_MOGALL_FOREST_NUT_CAUGHT.
static void MogallForest_CatchNut(void) {
  Event_Start();
  Import_ActorSetPosition(ACTOR_NUT, 0, 0);
  GameFlag_Set(FLAG_MOGALL_FOREST_NUT_CAUGHT);
  ItemDisplay_ShowItem(ITEM_NUT, 3);
  PartyInventory_GiveItem(ITEM_NUT, 0);
  Event_End();
}

/// Mogall Forest, the second area: walking into a log (cell behavior 20)
/// pushes it; the log at row 23 changes the ground beside it, and the river log
/// pushed to column 35 rolls into the water.
static void MogallForest_SecondArea_PushLog(void) {
  struct PushedActorProbe probe;
  s32 destinationColumn;
  s32 destinationRow;
  s32 rectangleHeight;
  s32 attributes;

  Event_Start();

  if (PushedActor_FindClearPosition(&probe) != 0) {
    Actor_MoveAndRedraw(probe);

    if (probe.actorSlot == ACTOR_SECOND_AREA_LOG &&
        (probe.position.z >> 20) == 23) {
      destinationColumn = 35;
      destinationRow = 68;
      MapGrid_CopyCellAttributes(35, 67, 4, 1, destinationColumn,
                                 destinationRow);
    } else if (probe.actorSlot == ACTOR_SECOND_AREA_RIVER_LOG &&
               (probe.position.x >> 20) == 35) {

      attributes = 0;
      GameFlag_Set(FLAG_MOGALL_FOREST_SECOND_AREA_LOG_ROLLED);
      Import_ActorSetAnimation(ACTOR_SECOND_AREA_RIVER_LOG,
                               MOGALL_FOREST_LOG_ROLLING);
      Actor_OffsetPositionAndResetMotion(ACTOR_SECOND_AREA_RIVER_LOG, -16, 6);
      Event_Wait(30);
      Import_ActorSetAnimation(ACTOR_SECOND_AREA_RIVER_LOG,
                               MOGALL_FOREST_LOG_FALLING_IN);
      Sound_Play(SE_LOG_FALL);

      Actor_GetById(ACTOR_SECOND_AREA_RIVER_LOG)->priorityFlags =
          ACTOR_PRIORITY_UNDERFOOT;

      destinationColumn = 34;
      destinationRow = 30;
      MapGrid_CopyCellAttributes(44, 30, 2, 4, destinationColumn,
                                 destinationRow);
      rectangleHeight = 4;
      PushedActor_FillGridAttributeRectangle(2, 35, 30, 1, rectangleHeight,
                                             attributes);
    }
  }

  Event_End();
}

/// Marks the rock's cell and clears the cells it could have moved from.
static inline void MogallForest_UpdateRockFootprint(s32 column, s32 row) {
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

/// Mogall Forest, the second area: Move on the rock. Its footprint follows it,
/// and at (36, 24) it sinks two pixels into place.
static void MogallForest_SecondArea_UpdateRock(void) {
  s32 column;
  s32 row;

  Event_Start();

  column = Actor_GetById(ACTOR_SECOND_AREA_ROCK)->x.fixed >> 20;
  row = Actor_GetById(ACTOR_SECOND_AREA_ROCK)->z.fixed >> 20;

  MogallForest_UpdateRockFootprint(column, row);

  if (column == 36 && row == 24) {
    struct Actor *rock = Actor_GetById(ACTOR_SECOND_AREA_ROCK);

    rock->motionFlags = 0;
    rock->groundHeight = -PIXELS(2);
    rock->y.fixed = -PIXELS(2);
  }

  Event_End();
}

/// Mogall Forest, the second area: Force on the north hiding place. The ape
/// points the way and hops off south.
static void MogallForest_SecondArea_NorthApeShowsWay(void) {
  struct Actor *creature;

  creature = Actor_GetById(ACTOR_SECOND_AREA_NORTH_APE);
  Event_Start();
  MogallForest_ApeComesOut(ACTOR_SECOND_AREA_NORTH_APE, SPRITE_COLOR_GRB);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_NORTH_APE, 392, 104, PIXELS(7));
  Event_Wait(10);
  FieldEffect_Spawn(creature->x.fixed, creature->y.fixed,
                    (creature->z.fixed + PIXELS(4)), 0, 0, 0,
                    EFFECT_SCRIPT_MEDIUM, 0);
  MogallForest_ApePointsTheWay(ACTOR_SECOND_AREA_NORTH_APE);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_NORTH_APE, 424, 120, PIXELS(3));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_NORTH_APE, 0);
  Event_Wait(6);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_NORTH_APE, 424, 168, PIXELS(3));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_NORTH_APE, 0);
  Event_Wait(6);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_NORTH_APE, 424, 208, PIXELS(3));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_NORTH_APE, 0);
  Event_Wait(6);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_NORTH_APE, 424, 232, PIXELS(3));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_NORTH_APE, 0);
  Event_Wait(6);
  Import_ActorSetPosition(ACTOR_SECOND_AREA_NORTH_APE, 0, 0);
  GameFlag_Set(FLAG_MOGALL_FOREST_SECOND_AREA_NORTH_APE_OUT);
  Import_ActorSetPosition(ACTOR_SECOND_AREA_NORTH_APE_STUMP, 0, 0);
  Event_End();
}

/// Mogall Forest, the second area: Force on the east hiding place. The ape
/// points the way and hops off east.
static void MogallForest_SecondArea_EastApeShowsWay(void) {
  struct Actor *creature;

  creature = Actor_GetById(ACTOR_SECOND_AREA_EAST_APE);

  Event_Start();

  MogallForest_ApeComesOut(ACTOR_SECOND_AREA_EAST_APE, SPRITE_COLOR_GRB);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_EAST_APE, 456, 104, PIXELS(7));
  Event_Wait(10);

  FieldEffect_Spawn(creature->x.fixed, creature->y.fixed,
                    creature->z.fixed + PIXELS(4), 0, 0, 0,
                    EFFECT_SCRIPT_MEDIUM, 0);

  MogallForest_ApePointsTheWay(ACTOR_SECOND_AREA_EAST_APE);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_EAST_APE, 472, 136, PIXELS(3));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_EAST_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_EAST_APE, 504, 136, PIXELS(16) / 5);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_EAST_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_EAST_APE, 552, 136, PIXELS(7) / 2);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_EAST_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_EAST_APE, 584, 136, PIXELS(7) / 2);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_EAST_APE, 0);
  Event_Wait(6);

  Import_ActorSetPosition(ACTOR_SECOND_AREA_EAST_APE, 0, 0);
  GameFlag_Set(FLAG_MOGALL_FOREST_SECOND_AREA_EAST_APE_OUT);
  Import_ActorSetPosition(ACTOR_SECOND_AREA_EAST_APE_STUMP, 0, 0);

  Event_End();
}

/// Mogall Forest, the second area: Force on the south hiding place the first
/// time. The ape bolts to another hiding place nearby.
static void MogallForest_SecondArea_SouthApeHidesAgain(void) {
  struct Actor *creature;

  Event_Start();
  MogallForest_ApeComesOut(ACTOR_SECOND_AREA_SOUTH_APE, SPRITE_COLOR_GRB);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 424, 480,
                          PIXELS(38) / 5);
  Event_Wait(2);
  MogallForest_SpawnPuffRing(ACTOR_SECOND_AREA_SOUTH_APE);
  Actor_SetSpriteColorById(ACTOR_SECOND_AREA_SOUTH_APE, SPRITE_COLOR_HIDDEN);
  creature = Actor_Get(ACTOR_SECOND_AREA_SOUTH_APE);
  Actor_SetSpriteFlags(creature, 0);
  Event_Wait(30);
  GameFlag_Set(FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_MOVED);
  Actor_SetPosition(ACTOR_SECOND_AREA_SOUTH_APE_STUMP, PIXELS(424),
                    PIXELS(480));
  Event_End();
}

/// Mogall Forest, the second area: Force on the south ape's new hiding place.
/// It points the way and hops off west.
static void MogallForest_SecondArea_SouthApeShowsWay(void) {
  struct Actor *creature;

  creature = Actor_GetById(ACTOR_SECOND_AREA_SOUTH_APE);

  Event_Start();

  MogallForest_ApeComesOut(ACTOR_SECOND_AREA_SOUTH_APE, SPRITE_COLOR_GRB);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 392, 504, PIXELS(6));
  Event_Wait(10);

  FieldEffect_Spawn(creature->x.fixed, creature->y.fixed,
                    creature->z.fixed + PIXELS(4), 0, 0, 0,
                    EFFECT_SCRIPT_MEDIUM, 0);

  MogallForest_ApePointsTheWay(ACTOR_SECOND_AREA_SOUTH_APE);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 360, 528, PIXELS(3));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_SOUTH_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 328, 528, PIXELS(3));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_SOUTH_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 288, 528, PIXELS(3));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_SOUTH_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 256, 528, PIXELS(3));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_SOUTH_APE, 0);
  Event_Wait(6);

  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  Import_ActorSetPosition(ACTOR_SECOND_AREA_SOUTH_APE, 0, 0);
  Event_Wait(30);
  GameFlag_Set(FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_OUT);
  Import_ActorSetPosition(ACTOR_SECOND_AREA_SOUTH_APE_STUMP, 0, 0);

  Event_End();
}

/// Mogall Forest, the third area: walking into a log (cell behavior 20)
/// pushes it; each river log rolls into the water at its column (42 and 40) and
/// can be crossed.
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
        MapGrid_CopyCellAttributes(38, 68, 1, 4, probe.position.x >> 20, 68);
        if (probe.position.x >> 20 == 42) {
          MapGrid_CopyCellAttributes(26, 20, 2, 4, probe.position.x >> 20, 23);
          Actor_SetSpritePriority(ACTOR_THIRD_AREA_NORTH_RIVER_LOG, 1);
          landed = 1;
          GameFlag_Set(FLAG_MOGALL_FOREST_THIRD_AREA_NORTH_LOG_ROLLED);
        }
      } else {
        if (probe.position.x >> 20 == 40) {
          MapGrid_CopyCellAttributes(26, 20, 2, 4, probe.position.x >> 20, 32);
          Actor_SetSpritePriority(ACTOR_THIRD_AREA_SOUTH_RIVER_LOG, 1);
          landed = 1;
          GameFlag_Set(FLAG_MOGALL_FOREST_THIRD_AREA_SOUTH_LOG_ROLLED);
        }
      }
      if (landed == 0) {
        Event_End();
        return;
      }
      Import_ActorSetAnimation(probe.actorSlot, MOGALL_FOREST_LOG_ROLLING);
      Actor_OffsetPositionAndResetMotion(probe.actorSlot, 18, 6);
      Event_Wait(30);
      Import_ActorSetAnimation(probe.actorSlot, MOGALL_FOREST_LOG_FALLING_IN);
      Sound_Play(SE_LOG_FALL);
      Actor_Get(probe.actorSlot)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    } else {
      if (probe.actorSlot == ACTOR_THIRD_AREA_LOG)
        MapGrid_CopyCellAttributes(42, 49, 1, 4, probe.position.x >> 20, 49);
    }
  }
  Event_End();
}

/// Mogall Forest, the third area: walking west into cell behavior 25 jumps
/// the leader onto the second river log, which bobs under the weight.
static void MogallForest_ThirdArea_JumpWest(void) {
  struct Actor *leader;
  u8 *motionFlags;
  u8 savedMotionFlags;
  struct FieldPosition position;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  motionFlags = &leader->motionFlags;
  savedMotionFlags = *motionFlags;

  position.x = Actor_GetById(ACTOR_PARTY_LEADER)->x.fixed - PIXELS(32);
  position.y = Actor_GetById(ACTOR_PARTY_LEADER)->y.fixed;
  position.z = Actor_GetById(ACTOR_PARTY_LEADER)->z.fixed;

  if (MogallForest_TryJumpLeaderTo(&position) != 0) {

    Event_Start();

    *motionFlags = 0;
    Import_ActorSetAnimation(ACTOR_THIRD_AREA_SOUTH_RIVER_LOG,
                             MOGALL_FOREST_LOG_STEPPED_ON);

    leader->y.fixed += -PIXELS(1);
    leader->groundHeight += -PIXELS(1);
    wait(2);

    leader->y.fixed += -PIXELS(1);
    leader->groundHeight += -PIXELS(1);
    wait(10);

    leader->y.fixed += PIXELS(1);
    leader->groundHeight += PIXELS(1);
    wait(4);

    leader->y.fixed += PIXELS(1);
    leader->groundHeight += PIXELS(1);

    *motionFlags = savedMotionFlags;
    Event_End();
  }
}

/// Mogall Forest, the third area: Move on the north rock; at (45, 6) it
/// sinks into place.
static void MogallForest_ThirdArea_UpdateNorthRock(void) {
  s32 column;
  s32 row;

  Event_Start();

  column = Actor_GetById(ACTOR_THIRD_AREA_NORTH_ROCK)->x.fixed >> 20;
  row = Actor_GetById(ACTOR_THIRD_AREA_NORTH_ROCK)->z.fixed >> 20;

  MogallForest_UpdateRockFootprint(column, row);

  if (column == 45 && row == 6) {
    struct Actor *rock = Actor_GetById(ACTOR_THIRD_AREA_NORTH_ROCK);

    rock->motionFlags = 0;
    rock->groundHeight = -PIXELS(2);
    rock->y.fixed = -PIXELS(2);
  }

  Event_End();
}

/// Mogall Forest, the third area: Move on the south rock; on row 27 it
/// sinks into place and stays.
static void MogallForest_ThirdArea_UpdateSouthRock(void) {

  s32 column;
  s32 row;
  struct Actor *rock;

  Event_Start();
  rock = Actor_GetById(ACTOR_THIRD_AREA_SOUTH_ROCK);
  column = rock->x.fixed;
  rock = Actor_GetById(ACTOR_THIRD_AREA_SOUTH_ROCK);
  row = rock->z.fixed;
  column >>= 20;
  row >>= 20;
  MogallForest_UpdateRockFootprint(column, row);
  rock = Actor_GetById(ACTOR_THIRD_AREA_SOUTH_ROCK);
  if ((rock->z.fixed >> 20) == 27) {
    rock = Actor_GetById(ACTOR_THIRD_AREA_SOUTH_ROCK);
    rock->motionFlags = 0;
    rock->groundHeight = -PIXELS(2);
    rock->y.fixed = -PIXELS(2);
    GameFlag_Set(FLAG_MOGALL_FOREST_SOUTH_ROCK_SETTLED);
    PushedActor_FillGridAttributeRectangle(2, 43, 23, 1, 1, CELL_BEHAVIOR_WALL);
  }
  Event_End();
}

/// Mogall Forest, the third area: Force on the attacking ape's hiding place.
/// The ape leaps at the leader and the battle with an Ape begins.
static void MogallForest_ThirdArea_ApeAttacks(void) {

  struct Actor *creature;
  s32 jumpSpeed;
  struct Actor *leaderForX;
  s32 leaderX;
  struct Actor *leaderForZ;

  creature = Actor_GetById(ACTOR_THIRD_AREA_ATTACKING_APE);
  jumpSpeed = PIXELS(8);
  Event_Start();
  MogallForest_ApeComesOut(ACTOR_THIRD_AREA_ATTACKING_APE, SPRITE_COLOR_NORMAL);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_ATTACKING_APE, 472, 104, jumpSpeed);
  Event_Wait(10);
  FieldEffect_Spawn(creature->x.fixed, creature->y.fixed,
                    (creature->z.fixed + jumpSpeed), 0, 0, 0,
                    EFFECT_SCRIPT_MEDIUM, 0);
  FieldCamera_FollowActor(ACTOR_THIRD_AREA_ATTACKING_APE, 1);
  Actor_TurnToFaceEachOther(ACTOR_THIRD_AREA_ATTACKING_APE, ACTOR_PARTY_LEADER,
                            0);
  Event_Wait(30);
  Actor_StartRepeatedMotion(ACTOR_THIRD_AREA_ATTACKING_APE, 2);
  FieldEffect_SpawnAttached(ACTOR_THIRD_AREA_ATTACKING_APE,
                            EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Sound_Play(SE_BEAST_CRY);
  Event_Wait(60);
  leaderForX = Actor_GetById(ACTOR_PARTY_LEADER);
  leaderX = leaderForX->x.part.pixel;
  leaderForZ = Actor_GetById(ACTOR_PARTY_LEADER);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_ATTACKING_APE, leaderX,
                          leaderForZ->z.part.pixel, PIXELS(6));
  Event_Wait(10);
  GameFlag_Set(FLAG_MOGALL_FOREST_THIRD_AREA_ATTACKING_APE_OUT);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 0);
  Event_End();
}

/// Mogall Forest, the third area: Force on the middle hiding place. The ape
/// points the way and hops off south.
static void MogallForest_ThirdArea_MiddleApeShowsWay(void) {
  struct Actor *creature;

  creature = Actor_GetById(ACTOR_THIRD_AREA_MIDDLE_APE);

  Event_Start();

  MogallForest_ApeComesOut(ACTOR_THIRD_AREA_MIDDLE_APE, SPRITE_COLOR_GRB);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_MIDDLE_APE, 456, 152, PIXELS(6));
  Event_Wait(10);

  FieldEffect_Spawn(creature->x.fixed, creature->y.fixed,
                    creature->z.fixed + PIXELS(4), 0, 0, 0,
                    EFFECT_SCRIPT_MEDIUM, 0);

  MogallForest_ApePointsTheWay(ACTOR_THIRD_AREA_MIDDLE_APE);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_MIDDLE_APE, 448, 192, PIXELS(3));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_MIDDLE_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_MIDDLE_APE, 424, 208, PIXELS(3));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_MIDDLE_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_MIDDLE_APE, 424, 224, PIXELS(3));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_MIDDLE_APE, 0);
  Event_Wait(6);

  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  Import_ActorSetPosition(ACTOR_THIRD_AREA_MIDDLE_APE, 0, 0);
  Event_Wait(30);
  GameFlag_Set(FLAG_MOGALL_FOREST_THIRD_AREA_MIDDLE_APE_OUT);
  Import_ActorSetPosition(ACTOR_THIRD_AREA_MIDDLE_APE_STUMP, 0, 0);

  Event_End();
}

/// Mogall Forest, the third area: Force on the west hiding place. The ape
/// points the way and hops off west.
static void MogallForest_ThirdArea_WestApeShowsWay(void) {
  struct Actor *creature;

  creature = Actor_GetById(ACTOR_THIRD_AREA_WEST_APE);

  Event_Start();

  MogallForest_ApeComesOut(ACTOR_THIRD_AREA_WEST_APE, SPRITE_COLOR_GRB);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_WEST_APE, 392, 104, PIXELS(6));
  Event_Wait(10);

  FieldEffect_Spawn(creature->x.fixed, creature->y.fixed,
                    creature->z.fixed + PIXELS(4), 0, 0, 0,
                    EFFECT_SCRIPT_MEDIUM, 0);

  MogallForest_ApePointsTheWay(ACTOR_THIRD_AREA_WEST_APE);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_WEST_APE, 376, 152, PIXELS(6));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_WEST_APE, 0);
  Event_Wait(10);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_WEST_APE, 328, 160, PIXELS(3));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_WEST_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_WEST_APE, 296, 160, PIXELS(3));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_WEST_APE, 0);
  Event_Wait(6);

  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  Import_ActorSetPosition(ACTOR_THIRD_AREA_WEST_APE, 0, 0);
  Event_Wait(30);
  GameFlag_Set(FLAG_MOGALL_FOREST_THIRD_AREA_WEST_APE_OUT);
  Import_ActorSetPosition(ACTOR_THIRD_AREA_WEST_APE_STUMP, 0, 0);

  Event_End();
}

/// Mogall Forest, the third area: Force on the east hiding place the first
/// time. The ape bolts to another hiding place to the south.
static void MogallForest_ThirdArea_EastApeHidesAgain(void) {
  struct Actor *creature;

  Event_Start();
  MogallForest_ApeComesOut(ACTOR_THIRD_AREA_EAST_APE, SPRITE_COLOR_GRB);
  FieldCamera_MoveTo(PIXELS(744), -1, PIXELS(504), 1);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 744, 504, PIXELS(9));
  MogallForest_SpawnPuffRing(ACTOR_THIRD_AREA_EAST_APE);
  Actor_SetSpriteColorById(ACTOR_THIRD_AREA_EAST_APE, SPRITE_COLOR_HIDDEN);
  creature = Actor_Get(ACTOR_THIRD_AREA_EAST_APE);
  Actor_SetSpriteFlags(creature, 0);
  Event_Wait(30);
  GameFlag_Set(FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_MOVED);
  Actor_SetPosition(ACTOR_THIRD_AREA_EAST_APE_STUMP, PIXELS(744), PIXELS(504));
  Event_End();
}

/// Mogall Forest, the third area: Force on the east ape's new hiding place.
/// It points the way and hops off south.
static void MogallForest_ThirdArea_EastApeShowsWay(void) {
  struct Actor *creature;

  creature = Actor_GetById(ACTOR_THIRD_AREA_EAST_APE);

  Event_Start();

  MogallForest_ApeComesOut(ACTOR_THIRD_AREA_EAST_APE, SPRITE_COLOR_GRB);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 712, 536, PIXELS(6));
  Event_Wait(10);

  FieldEffect_Spawn(creature->x.fixed, creature->y.fixed,
                    creature->z.fixed + PIXELS(4), 0, 0, 0,
                    EFFECT_SCRIPT_MEDIUM, 0);

  MogallForest_ApePointsTheWay(ACTOR_THIRD_AREA_EAST_APE);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 712, 568, PIXELS(6));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
  Event_Wait(10);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 712, 600, PIXELS(3));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 736, 640, PIXELS(3));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
  Event_Wait(6);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 736, 704, PIXELS(3));
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
  Event_Wait(6);

  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  Import_ActorSetPosition(ACTOR_THIRD_AREA_EAST_APE, 0, 0);
  Event_Wait(30);
  GameFlag_Set(FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT);

  Event_End();
}

/// Mogall Forest, the third area: touching cell behavior 22, once. The east
/// ape appears again, turns to the party and leads it south to a hiding place.
/// Sets FLAG_MOGALL_FOREST_LED_TO_KILLER_APE.
static void MogallForest_ApeLeadsToKillerApe(void) {
  struct Actor *creature;

  creature = Actor_GetById(ACTOR_THIRD_AREA_EAST_APE);
  Event_Start();
  Actor_SetPosition(ACTOR_THIRD_AREA_EAST_APE, PIXELS(136), PIXELS(360));
  MogallForest_ApeComesOut(ACTOR_THIRD_AREA_EAST_APE, SPRITE_COLOR_GRB);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 136, 408, PIXELS(8));
  Event_Wait(10);
  FieldEffect_Spawn(creature->x.fixed, creature->y.fixed,
                    (creature->z.fixed + PIXELS(4)), 0, 0, 0,
                    EFFECT_SCRIPT_MEDIUM, 0);
  Actor_FaceDirection(ACTOR_THIRD_AREA_EAST_APE, FACING_NORTH, 40);
  Actor_SetAttachedEffect(ACTOR_THIRD_AREA_EAST_APE,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_RunRepeatedMotion(ACTOR_THIRD_AREA_EAST_APE, 2);
  FieldCamera_FollowActor(ACTOR_THIRD_AREA_EAST_APE, 1);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 136, 440, PIXELS(6));
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
  Event_Wait(10);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 136, 472, PIXELS(3));
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
  Event_Wait(6);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 136, 504, PIXELS(3));
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
  Event_Wait(6);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  Actor_SetPosition(ACTOR_THIRD_AREA_EAST_APE, 0, 0);
  Event_Wait(60);
  GameFlag_Set(FLAG_MOGALL_FOREST_LED_TO_KILLER_APE);
  Event_End();
}

/// Mogall Forest, the third area: touching cell behavior 23 until the
/// Killer Ape is beaten. The hiding place shakes twice and out comes the Killer
/// Ape, the east ape's actor a fifth again as big, to fight
/// (ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, slot 1); the party comes back
/// at MOGALL_FOREST_KILLER_APE_AFTER_KILLER_APE_ATTACKS.
static void MogallForest_KillerApeAttacks(void) {
  struct Vector3 velocity;
  struct Vector3 *emissionVelocity;
  struct Actor *actor;
  u32 directionIndex;
  s32 x;
  s32 z;

  actor = Actor_GetById(ACTOR_THIRD_AREA_EAST_APE);
  Event_Start();
  // Shake the hiding place twice before the creature appears.
  Actor_ConfigureSpriteColor(ACTOR_THIRD_AREA_EAST_APE, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_THIRD_AREA_EAST_APE), 0);
  Actor_SetPosition(ACTOR_THIRD_AREA_EAST_APE, PIXELS(136), PIXELS(360));
  FieldCamera_SetSpeedParameters(PIXELS(1) / 2, PIXELS(1) / 16);
  FieldCamera_MoveTo(PIXELS(136), -1, PIXELS(392), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(60);
  MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  MogallForest_SpawnPuffRing(ACTOR_THIRD_AREA_EAST_APE);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  MapGrid_WaitForShakeToSettle();
  Actor_ArmCallback(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Event_Wait(40);
  MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  MogallForest_SpawnPuffRing(ACTOR_THIRD_AREA_EAST_APE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  MapGrid_WaitForShakeToSettle();
  Event_Wait(40);
  MogallForest_SpawnPuffRing(ACTOR_THIRD_AREA_EAST_APE);
  actor->scaleX = 6 * Q16_ONE / 5;
  actor->scaleY = 6 * Q16_ONE / 5;
  Actor_ConfigureSpriteColor(ACTOR_THIRD_AREA_EAST_APE, SPRITE_COLOR_BRG);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 136, 392, PIXELS(15));
  Event_Wait(15);
  // The larger reveal uses a wider, level ring of puffs.
  MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  for (directionIndex = 0; directionIndex <= 16; directionIndex++) {
    emissionVelocity = &velocity;
    emissionVelocity->x = Trig_Cos(directionIndex << 12);
    emissionVelocity->y = 0;
    z = Trig_Sin(directionIndex << 12);
    x = emissionVelocity->x;
    emissionVelocity->z = z;
    x += x / 2;
    emissionVelocity->x = x;
    FieldEffect_Spawn(actor->x.fixed, 0, actor->z.fixed, x, emissionVelocity->y,
                      z, EFFECT_SCRIPT_MEDIUM, NULL);
  }
  Event_Wait(30);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  MapGrid_WaitForShakeToSettle();
  Sound_Play(SE_BOSS_ROAR);
  Actor_RunRepeatedMotion(ACTOR_THIRD_AREA_EAST_APE, 2);
  Event_Wait(20);
  // Jump toward the leader, then enter the final creature encounter.
  {
    s32 leaderX = Actor_GetById(ACTOR_PARTY_LEADER)->x.part.pixel;

    MogallForest_HopActorTo(
        ACTOR_THIRD_AREA_EAST_APE, leaderX,
        Actor_GetById(ACTOR_PARTY_LEADER)->z.part.pixel - 16, PIXELS(8));
  }
  Event_Wait(10);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination(
      (s32)&SceneId_MogallForestKillerApe,
      MOGALL_FOREST_KILLER_APE_AFTER_KILLER_APE_ATTACKS);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 1);
  Event_End();
}

const struct Trigger *MogallForest_GetTriggers(void) {
  s16 selector = gMainRoutineWork.sceneId;

  if (selector == (s32)&SceneId_MogallForestSign) {
    return gMogallForestSign_Triggers;
  }
  if (selector == (s32)&SceneId_MogallForestQuartz) {
    return gMogallForestQuartz_Triggers;
  }
  if (selector == (s32)&SceneId_MogallForestKillerApe) {
    return gMogallForestKillerApe_Triggers;
  }
  return gMogallForestSidePaths_Triggers;
}

/// Sets up Mogall Forest's area. In the first area, the first time, an ape
/// peeks out and hides; apes already out stay gone, rolled logs and moved
/// rocks stay put. In the third area, coming back from the Killer Ape
/// (MOGALL_FOREST_KILLER_APE_AFTER_KILLER_APE_ATTACKS) marks it beaten, and
/// entering at MOGALL_FOREST_KILLER_APE_BRIDGES_FROM_RIVER or
/// MOGALL_FOREST_KILLER_APE_FROM_WORLD_MAP once Force is learned and the Killer
/// Ape beaten sets FLAG_NYUNPA_ENLIGHTENED.
s32 MogallForest_Initialize(void) {
  struct Actor *actor;
  s32 sceneId;
  s32 entrance;
  s32 column;
  s32 fixedZ;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  sceneId = gMainRoutineWork.sceneId;
  if (sceneId == (s32)&SceneId_MogallForestSign) {
    // Show the first guide once, or restore the first area's completed
    // crossing.
    entrance = gMainRoutineWork.entrance;
    switch (entrance) {
    case MOGALL_FOREST_SIGN_FROM_WORLD_MAP:
    case MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_WEST:
    case MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_NORTH:
    case MOGALL_FOREST_SIGN_ENTRANCE_FROM_SOUTHWEST_CLEARING:
      if (Event_GetFlag(FLAG_MOGALL_FOREST_FIRST_APE_SEEN) == 0) {
        Event_Start();
        wait(1);
        Actor_ConfigureSpriteColor(ACTOR_FIRST_AREA_NORTH_APE,
                                   SPRITE_COLOR_GRB);
        Actor_SetPosition(ACTOR_FIRST_AREA_NORTH_APE, PIXELS(92), PIXELS(120));
        Actor_FaceDirection(ACTOR_FIRST_AREA_NORTH_APE,
                            FACING_NORTH + FACING_STEP, 0);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2) / 5, PIXELS(1) / 5);
        Actor_ResetAndSetPositionInMode2(ACTOR_PARTY_LEADER, 136, 64);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
        Event_Wait(30);
        Actor_ShowEmote(ACTOR_FIRST_AREA_NORTH_APE,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
        Actor_RunRepeatedMotion(ACTOR_FIRST_AREA_NORTH_APE, 2);
        Event_Wait(30);
        MogallForest_HopActorTo(ACTOR_FIRST_AREA_NORTH_APE, 136, 116,
                                PIXELS(7));
        MogallForest_SpawnPuffRing(ACTOR_FIRST_AREA_NORTH_APE);
        Actor_ConfigureSpriteColor(ACTOR_FIRST_AREA_NORTH_APE,
                                   SPRITE_COLOR_HIDDEN);
        Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_AREA_NORTH_APE), 0);
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
      Actor_ConfigureSpriteColor(ACTOR_FIRST_AREA_NORTH_APE,
                                 SPRITE_COLOR_HIDDEN);
      Actor_SetPosition(ACTOR_FIRST_AREA_NORTH_APE, PIXELS(136), PIXELS(116));
      break;

    case MOGALL_FOREST_SIGN_RIVER_FROM_SOUTHWEST_CLEARING:
    case MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_EAST:
    case MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_QUARTZ:
      actor = Actor_GetById(ACTOR_PARTY_LEADER);
      if (actor != 0) {
        Import_ActorSetPosition(ACTOR_NUT, actor->x.fixed, actor->z.fixed);
      }
      actor = Actor_GetById(ACTOR_NUT);
      actor->update = 0;
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
        actor = Actor_GetById(ACTOR_NUT);
        actor->y.fixed = PIXELS(32);
      }
      wait(1);
      Import_ActorSetPosition(ACTOR_NUT, PIXELS(632), PIXELS(440));
      if (GameFlag_Get(FLAG_MOGALL_FOREST_NUT_CAUGHT) == 0) {
        Scene_FloatNut(ACTOR_NUT);
      }
      Actor_ConfigureSpriteColor(ACTOR_FIRST_AREA_ATTACKING_APE,
                                 SPRITE_COLOR_HIDDEN);
      Actor_ConfigureSpriteColor(ACTOR_FIRST_AREA_EAST_APE,
                                 SPRITE_COLOR_HIDDEN);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_AREA_ATTACKING_APE), 0);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_AREA_EAST_APE), 0);
      Scene_RedrawActorFootprint(ACTOR_FIRST_AREA_LOG);
      if (GameFlag_Get(FLAG_MOGALL_FOREST_FIRST_AREA_LOG_ROLLED) == 0) {
        Scene_RedrawActorFootprint(ACTOR_FIRST_AREA_RIVER_LOG);
        break;
      }
      wait(1);
      Actor_SetPosition(ACTOR_FIRST_AREA_RIVER_LOG, PIXELS(528), PIXELS(408));
      Import_ActorSetAnimation(ACTOR_FIRST_AREA_RIVER_LOG,
                               MOGALL_FOREST_LOG_IN_RIVER);
      MapGrid_CopyCellAttributes(38, 27, 4, 2, 31, 25);
      Actor_GetById(ACTOR_FIRST_AREA_RIVER_LOG)->priorityFlags =
          ACTOR_PRIORITY_UNDERFOOT;
      break;
    }
  } else if (sceneId == (s32)&SceneId_MogallForestQuartz) {
    // Restore revealed guides, moved rocks and the second crossing.
    entrance = gMainRoutineWork.entrance;
    switch (entrance) {
    case MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIDE_PATHS_NORTH:
    case MOGALL_FOREST_QUARTZ_STUMPS_FROM_NORTHWEST_CLEARING:
    case MOGALL_FOREST_QUARTZ_STUMPS_FROM_POND:
    case MOGALL_FOREST_QUARTZ_STUMPS_FROM_NORTHEAST_CLEARING:
      if (GameFlag_Get(FLAG_MOGALL_FOREST_SECOND_AREA_NORTH_APE_OUT) == 0) {
        Actor_ConfigureSpriteColor(ACTOR_SECOND_AREA_NORTH_APE,
                                   SPRITE_COLOR_HIDDEN);
        Actor_SetSpriteFlags(Actor_GetById(ACTOR_SECOND_AREA_NORTH_APE), 0);
      }
      if (GameFlag_Get(FLAG_MOGALL_FOREST_SECOND_AREA_EAST_APE_OUT) != 0) {
        break;
      }
      Actor_ConfigureSpriteColor(ACTOR_SECOND_AREA_EAST_APE,
                                 SPRITE_COLOR_HIDDEN);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_SECOND_AREA_EAST_APE), 0);
      break;

    case MOGALL_FOREST_QUARTZ_RIVER_FROM_NORTHEAST_CLEARING:
    case MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_KILLER_APE:
    case MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIDE_PATHS_SOUTH:
      if (GameFlag_Get(FLAG_MOGALL_FOREST_SECOND_AREA_LOG_ROLLED) == 0) {
        Scene_RedrawActorFootprint(ACTOR_SECOND_AREA_RIVER_LOG);
      } else {
#if TBS_PATCH_1
        s32 attr = 0;
#endif

        wait(1);
        Actor_SetPosition(ACTOR_SECOND_AREA_RIVER_LOG, PIXELS(552),
                          PIXELS(510));
        Import_ActorSetAnimation(ACTOR_SECOND_AREA_RIVER_LOG,
                                 MOGALL_FOREST_LOG_IN_RIVER);
        Actor_GetById(ACTOR_SECOND_AREA_RIVER_LOG)->priorityFlags =
            ACTOR_PRIORITY_UNDERFOOT;
        MapGrid_CopyCellAttributes(44, 30, 2, 4, 34, 30);
#if TBS_PATCH_1
        PushedActor_FillGridAttributeRectangle(0, 35, 29, 1, 4, attr);
#endif
      }
      Scene_RedrawActorFootprint(ACTOR_SECOND_AREA_LOG);
      Scene_RedrawActorFootprint(ACTOR_SECOND_AREA_SOUTH_LOG);
      column = Actor_GetById(ACTOR_SECOND_AREA_ROCK)->x.fixed;
      fixedZ = Actor_GetById(ACTOR_SECOND_AREA_ROCK)->z.fixed;
      column >>= 20;
      PushedActor_FillGridAttributeRectangle(2, column, fixedZ >> 20, 1, 1,
                                             CELL_BEHAVIOR_WALL);
      wait(1);
      Actor_ConfigureSpriteColor(ACTOR_SECOND_AREA_ROCK, SPRITE_COLOR_GREY);
      Actor_GetById(ACTOR_SECOND_AREA_LOG)->collisionFlags |=
          ACTOR_NOT_TARGETED;
      if (GameFlag_Get(FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_OUT) != 0) {
        break;
      }
      Actor_ConfigureSpriteColor(ACTOR_SECOND_AREA_SOUTH_APE,
                                 SPRITE_COLOR_HIDDEN);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_SECOND_AREA_SOUTH_APE), 0);
      if (GameFlag_Get(FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_MOVED) == 0) {
        break;
      }
      Actor_SetPosition(ACTOR_SECOND_AREA_SOUTH_APE, PIXELS(424), PIXELS(480));
      Actor_SetPosition(ACTOR_SECOND_AREA_SOUTH_APE_STUMP, PIXELS(424),
                        PIXELS(480));
      break;
    }
  } else if (sceneId == (s32)&SceneId_MogallForestKillerApe) {
    // Restore both crossing logs and the creatures leading to the final
    // encounter.
    entrance = gMainRoutineWork.entrance;
    switch (entrance) {
    case MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_WEST:
    case MOGALL_FOREST_KILLER_APE_STUMPS_FROM_CHEST:
    case MOGALL_FOREST_KILLER_APE_STUMPS_FROM_NORTHWEST_CLEARING:
    case MOGALL_FOREST_KILLER_APE_STUMPS_FROM_RIVER:
      wait(1);
      if (GameFlag_Get(FLAG_MOGALL_FOREST_THIRD_AREA_ATTACKING_APE_OUT) == 0) {
        Actor_ConfigureSpriteColor(ACTOR_THIRD_AREA_ATTACKING_APE,
                                   SPRITE_COLOR_HIDDEN);
        Actor_SetSpriteFlags(Actor_GetById(ACTOR_THIRD_AREA_ATTACKING_APE), 0);
        Actor_SetSpriteFlags(
            Actor_GetById(ACTOR_THIRD_AREA_ATTACKING_APE_STUMP), 0);
      }
      if (GameFlag_Get(FLAG_MOGALL_FOREST_THIRD_AREA_MIDDLE_APE_OUT) == 0) {
        Actor_ConfigureSpriteColor(ACTOR_THIRD_AREA_MIDDLE_APE,
                                   SPRITE_COLOR_HIDDEN);
        Actor_SetSpriteFlags(Actor_GetById(ACTOR_THIRD_AREA_MIDDLE_APE), 0);
        Actor_SetSpriteFlags(Actor_GetById(ACTOR_THIRD_AREA_MIDDLE_APE_STUMP),
                             0);
      }
      if (GameFlag_Get(FLAG_MOGALL_FOREST_THIRD_AREA_WEST_APE_OUT) != 0) {
        break;
      }
      Actor_ConfigureSpriteColor(ACTOR_THIRD_AREA_WEST_APE,
                                 SPRITE_COLOR_HIDDEN);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_THIRD_AREA_WEST_APE), 0);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_THIRD_AREA_WEST_APE_STUMP), 0);
      break;

    case MOGALL_FOREST_KILLER_APE_CHEST_FROM_STUMPS:
      column = Actor_GetById(ACTOR_THIRD_AREA_NORTH_ROCK)->x.fixed;
      fixedZ = Actor_GetById(ACTOR_THIRD_AREA_NORTH_ROCK)->z.fixed;
      column >>= 20;
      PushedActor_FillGridAttributeRectangle(2, column, fixedZ >> 20, 1, 1,
                                             CELL_BEHAVIOR_WALL);
      Actor_ConfigureSpriteColor(ACTOR_THIRD_AREA_NORTH_ROCK,
                                 SPRITE_COLOR_GREY);
      wait(1);
      Actor_GetById(ACTOR_THIRD_AREA_LOG)->collisionFlags |= ACTOR_NOT_TARGETED;
      Scene_RedrawActorFootprint(ACTOR_THIRD_AREA_LOG);
      break;

    case MOGALL_FOREST_KILLER_APE_RIVER_FROM_STUMPS:
    case MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_EAST:
    case MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_SOUTH:
    case MOGALL_FOREST_KILLER_APE_RIVER_FROM_BRIDGES:
      // XXX (no evidence): what these two crates are for; they are hidden, not
      // solid and never a target, so the player neither sees nor meets them.
      Actor_CreateConfigured(PIXELS(734), 0, PIXELS(370), SPRITE_CRATE);
      Actor_CreateConfigured(PIXELS(754), 0, PIXELS(370), SPRITE_CRATE);
      Scene_RedrawActorFootprint(ACTOR_THIRD_AREA_WEST_LOG);
      Scene_RedrawActorFootprint(ACTOR_THIRD_AREA_SOUTHWEST_LOG);
      if (GameFlag_Get(FLAG_MOGALL_FOREST_THIRD_AREA_NORTH_LOG_ROLLED) == 0) {
        Scene_RedrawActorFootprint(ACTOR_THIRD_AREA_NORTH_RIVER_LOG);
      } else {
        wait(1);
        Import_ActorSetAnimation(ACTOR_THIRD_AREA_NORTH_RIVER_LOG,
                                 MOGALL_FOREST_LOG_IN_RIVER);
        Import_ActorSetPosition(ACTOR_THIRD_AREA_NORTH_RIVER_LOG, PIXELS(698),
                                PIXELS(398));
        Actor_GetById(ACTOR_THIRD_AREA_NORTH_RIVER_LOG)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        MapGrid_CopyCellAttributes(26, 20, 2, 4, 42, 23);
        GameFlag_Set(FLAG_MOGALL_FOREST_SOUTH_ROCK_SETTLED);
        Actor_SetPosition(ACTOR_THIRD_AREA_SOUTH_ROCK, PIXELS(632),
                          PIXELS(440));
        Actor_SetSpriteFlags(Actor_GetById(ACTOR_THIRD_AREA_SOUTH_ROCK), 0);
      }
      if (GameFlag_Get(FLAG_MOGALL_FOREST_THIRD_AREA_SOUTH_LOG_ROLLED) == 0) {
        Scene_RedrawActorFootprint(ACTOR_THIRD_AREA_SOUTH_RIVER_LOG);
      } else {
        wait(1);
        Import_ActorSetAnimation(ACTOR_THIRD_AREA_SOUTH_RIVER_LOG,
                                 MOGALL_FOREST_LOG_IN_RIVER);
        Import_ActorSetPosition(ACTOR_THIRD_AREA_SOUTH_RIVER_LOG, PIXELS(666),
                                PIXELS(550));
        Actor_GetById(ACTOR_THIRD_AREA_SOUTH_RIVER_LOG)->priorityFlags =
            ACTOR_PRIORITY_UNDERFOOT;
        MapGrid_CopyCellAttributes(26, 20, 2, 4, 40, 32);
      }
      column = Actor_GetById(ACTOR_THIRD_AREA_SOUTH_ROCK)->x.fixed;
      fixedZ = Actor_GetById(ACTOR_THIRD_AREA_SOUTH_ROCK)->z.fixed;
      column >>= 20;
      PushedActor_FillGridAttributeRectangle(2, column, fixedZ >> 20, 1, 1,
                                             CELL_BEHAVIOR_WALL);
      Actor_ConfigureSpriteColor(ACTOR_THIRD_AREA_SOUTH_ROCK,
                                 SPRITE_COLOR_GREY);
      wait(1);
      Actor_GetById(ACTOR_THIRD_AREA_NORTH_RIVER_LOG)->collisionFlags |=
          ACTOR_NOT_TARGETED;
      if (GameFlag_Get(FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT) == 0) {
        Actor_ConfigureSpriteColor(ACTOR_THIRD_AREA_EAST_APE,
                                   SPRITE_COLOR_HIDDEN);
        Actor_SetSpriteFlags(Actor_GetById(ACTOR_THIRD_AREA_EAST_APE), 0);
        if (GameFlag_Get(FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_MOVED) != 0) {
          Actor_SetPosition(ACTOR_THIRD_AREA_EAST_APE_STUMP, PIXELS(744),
                            PIXELS(504));
          Actor_SetPosition(ACTOR_THIRD_AREA_EAST_APE, PIXELS(744),
                            PIXELS(504));
        }
#if TBS_PATCH_2
      } else {
        Import_ActorSetPosition(ACTOR_THIRD_AREA_EAST_APE, 0, 0);
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

#include "shared/actor_orbit.h"

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))

s32 gPushedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gPushedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedActor_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

s32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

s32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

s32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// The script of each rock moved with Move: it is drawn in greys.
s32 gMogallForest_RockScript[] = {
    SC_ANIMATION(2),
    SC_SET(SW_TIMER, SPRITE_COLOR_GREY),
    SC_CALL(MogallForest_CopyTimerToChild),
    SC_END,
};

/// The script of each ape: four fifths of its size, hidden while it hides, and
/// its shadow off.
s32 gMogallForest_ActorScript[] = {
    SC_SET(SW_SCALE_X, 4 * Q16_ONE / 5),
    SC_SET(SW_SCALE_Y, 4 * Q16_ONE / 5),
    SC_SET(SW_TIMER, SPRITE_COLOR_HIDDEN),
    SC_CALL(MogallForest_CopyTimerToChild),
    SC_CALL(MogallForest_ClearActorSpriteFlags),
    SC_END,
};

/// The script of each invisible hiding place: still, and its sprite hidden.
s32 gMogallForest_HidingPlaceScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_Y, 0),
    SC_SET(SW_TARGET_Y, PIXELS(-32768)),
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
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(488), 0, PIXELS(464), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        PIXELS(528), 0, PIXELS(504), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_APE, FLAG_MOGALL_FOREST_FIRST_AREA_NORTH_APE_OUT, (s32)gMogallForest_ActorScript,
        PIXELS(136), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_APE, FLAG_MOGALL_FOREST_FIRST_AREA_ATTACKING_APE_OUT, (s32)gMogallForest_ActorScript,
        PIXELS(376), 0, PIXELS(432), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_APE, FLAG_MOGALL_FOREST_FIRST_AREA_EAST_APE_OUT, (s32)gMogallForest_ActorScript,
        PIXELS(504), 0, PIXELS(312), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, FLAG_MOGALL_FOREST_FIRST_AREA_NORTH_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        PIXELS(136), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, FLAG_MOGALL_FOREST_FIRST_AREA_ATTACKING_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        PIXELS(376), 0, PIXELS(432), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLANK, FLAG_MOGALL_FOREST_FIRST_AREA_EAST_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        PIXELS(504), 0, PIXELS(312), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLANK, FLAG_MOGALL_FOREST_NUT_CAUGHT, 7,
        PIXELS(632), 0, PIXELS(440), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gMogallForestQuartz_ActorPlacements[] = {
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        PIXELS(592), 0, PIXELS(360), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        PIXELS(592), 0, PIXELS(456), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(648), 0, PIXELS(512), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_TURNING_ROCK, CONDITION_ALWAYS, (s32)gMogallForest_RockScript,
        PIXELS(584), 0, PIXELS(376), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_APE, FLAG_MOGALL_FOREST_SECOND_AREA_NORTH_APE_OUT, (s32)gMogallForest_ActorScript,
        PIXELS(360), 0, PIXELS(80), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_APE, FLAG_MOGALL_FOREST_SECOND_AREA_EAST_APE_OUT, (s32)gMogallForest_ActorScript,
        PIXELS(488), 0, PIXELS(80), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_APE, FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_OUT, (s32)gMogallForest_ActorScript,
        PIXELS(504), 0, PIXELS(480), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, FLAG_MOGALL_FOREST_SECOND_AREA_NORTH_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        PIXELS(360), 0, PIXELS(80), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, FLAG_MOGALL_FOREST_SECOND_AREA_EAST_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        PIXELS(488), 0, PIXELS(80), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLANK, FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        PIXELS(504), 0, PIXELS(480), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { VENUS_DJINNI, FLAG_DJINNI(DJINNI_QUARTZ), MOVEMENT_TYPE_STAND,
        PIXELS(488), 0, PIXELS(568), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gMogallForestKillerApe_ActorPlacements[] = {
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(680), 0, PIXELS(96), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(616), 0, PIXELS(400), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(552), 0, PIXELS(480), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(600), 0, PIXELS(544), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        PIXELS(544), 0, PIXELS(536), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_TURNING_ROCK, CONDITION_ALWAYS, (s32)gMogallForest_RockScript,
        PIXELS(696), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_TURNING_ROCK, CONDITION_ALWAYS, (s32)gMogallForest_RockScript,
        PIXELS(632), 0, PIXELS(408), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_APE, FLAG_MOGALL_FOREST_THIRD_AREA_ATTACKING_APE_OUT, (s32)gMogallForest_ActorScript,
        PIXELS(488), 0, PIXELS(88), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_APE, FLAG_MOGALL_FOREST_THIRD_AREA_MIDDLE_APE_OUT, (s32)gMogallForest_ActorScript,
        PIXELS(424), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_APE, FLAG_MOGALL_FOREST_THIRD_AREA_WEST_APE_OUT, (s32)gMogallForest_ActorScript,
        PIXELS(360), 0, PIXELS(80), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_APE, MOGALL_FOREST_EAST_ACTOR_CONDITION, (s32)gMogallForest_ActorScript,
        PIXELS(744), 0, PIXELS(400), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, FLAG_MOGALL_FOREST_THIRD_AREA_ATTACKING_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        PIXELS(488), 0, PIXELS(88), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, FLAG_MOGALL_FOREST_THIRD_AREA_MIDDLE_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        PIXELS(424), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLANK, FLAG_MOGALL_FOREST_THIRD_AREA_WEST_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        PIXELS(360), 0, PIXELS(80), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLANK, MOGALL_FOREST_EAST_SHARED_SPRITE_CONDITION, (s32)gMogallForest_HidingPlaceScript,
        PIXELS(744), 0, PIXELS(400), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
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
    { STEP_TRIGGER_WALKING, 20, CONDITION_ALWAYS, EVENT(MogallForest_FirstArea_PushLog) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 25, CONDITION_ALWAYS, EVENT(MogallForest_FirstArea_JumpNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 26, CONDITION_ALWAYS, EVENT(MogallForest_FirstArea_JumpSouth) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 27, CONDITION_ALWAYS, EVENT(MogallForest_ReadBrokenSign) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_FIRST_AREA_NORTH_APE_STUMP, FLAG_MOGALL_FOREST_FIRST_AREA_NORTH_APE_OUT, EVENT(MogallForest_FirstArea_NorthApeShowsWay) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_FIRST_AREA_ATTACKING_APE_STUMP, FLAG_MOGALL_FOREST_FIRST_AREA_ATTACKING_APE_OUT, EVENT(MogallForest_FirstArea_ApeAttacks) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_FIRST_AREA_EAST_APE_STUMP, FLAG_MOGALL_FOREST_FIRST_AREA_EAST_APE_OUT, EVENT(MogallForest_FirstArea_EastApeShowsWay) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_NUT, FLAG_MOGALL_FOREST_NUT_CAUGHT, EVENT(MogallForest_CatchNut) },
    { TRIGGERS_END },
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
    { STEP_TRIGGER_WALKING, 20, CONDITION_ALWAYS, EVENT(MogallForest_SecondArea_PushLog) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_AREA_ROCK, CONDITION_ALWAYS, EVENT(MogallForest_SecondArea_UpdateRock) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_SECOND_AREA_NORTH_APE_STUMP, FLAG_MOGALL_FOREST_SECOND_AREA_NORTH_APE_OUT, EVENT(MogallForest_SecondArea_NorthApeShowsWay) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_SECOND_AREA_EAST_APE_STUMP, FLAG_MOGALL_FOREST_SECOND_AREA_EAST_APE_OUT, EVENT(MogallForest_SecondArea_EastApeShowsWay) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_SECOND_AREA_SOUTH_APE_STUMP, FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_MOVED, EVENT(MogallForest_SecondArea_SouthApeHidesAgain) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_SECOND_AREA_SOUTH_APE_STUMP, FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_OUT, EVENT(MogallForest_SecondArea_SouthApeShowsWay) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_MOGALL_FOREST_APPLE_CHEST, FIND_ITEM(ITEM_APPLE) },
    { TRIGGER_TALK, ACTOR_SECOND_AREA_QUARTZ, CONDITION_ALWAYS, EVENT(MogallForest_StartDjinnBattle) },
    { TRIGGERS_END },
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
    {STEP_TRIGGER_WALKING, 20, CONDITION_ALWAYS,
     EVENT(MogallForest_ThirdArea_PushLog)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 25, CONDITION_ALWAYS,
     EVENT(MogallForest_ThirdArea_JumpWest)},
    {TRIGGER_STEP, 22, FLAG_MOGALL_FOREST_LED_TO_KILLER_APE,
     EVENT(MogallForest_ApeLeadsToKillerApe)},
    {TRIGGER_STEP, 23, FLAG_MOGALL_FOREST_KILLER_APE_BEATEN,
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
    {TRIGGERS_END},
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
    {TRIGGERS_END},
};
