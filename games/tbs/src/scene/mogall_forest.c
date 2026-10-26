/// @file
/// Mogall Forest (SceneId_MogallForestSign to SceneId_MogallForestSidePaths),
/// the woods between Fuchin Temple and Xian, which the party finds its way
/// through by using Force, learned from Nyunpa, on the stumps the apes hide in.
///
/// The way runs from the first area (SceneId_MogallForestSign), entered from
/// the world map, through the second (SceneId_MogallForestQuartz) to the third
/// (SceneId_MogallForestKillerApe), which leaves for the world map by
/// MOGALL_FOREST_KILLER_APE_TO_WORLD_MAP. Apes hide in hollow purple stumps,
/// each stump holding a blank object event on its spot, and Force on a stump
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
///   metatile behavior 22 the first time, the east ape leads the party south to
///   a hiding place (FLAG_MOGALL_FOREST_LED_TO_KILLER_APE); there (metatile
///   behavior 23) the Killer Ape bursts out to fight
///   (ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, slot 1), and coming back at
///   MOGALL_FOREST_KILLER_APE_AFTER_KILLER_APE_ATTACKS marks it beaten
///   (FLAG_MOGALL_FOREST_KILLER_APE_BEATEN). Entering at
///   MOGALL_FOREST_KILLER_APE_BRIDGES_FROM_RIVER or
///   MOGALL_FOREST_KILLER_APE_FROM_WORLD_MAP once Force is learned and the
///   Killer Ape beaten sets FLAG_NYUNPA_ENLIGHTENED: back at Fuchin Temple the
///   monks say that Nyunpa has reached enlightenment and passed away.
/// - SceneId_MogallForestSidePaths, also named Mogall Forest: side paths that
///   the three areas' warps lead into and back out of, some of them leading
///   back into the map itself; a Psynergy Stone lies there.
///
/// One overlay serves all four maps: MogallForest_GetWarpDests,
/// _GetObjectEventTemplates and _GetEvents choose each map's tables, and
/// MogallForest_Initialize puts back the apes already out, the rolled logs and
/// the settled rocks, map by map and warp destination by warp destination. The
/// third area's east ape is also the guide at metatile behavior 22 and the
/// Killer Ape, and the editions differ in how it is kept out of sight once it
/// has come out (the #if blocks below and in MogallForest_Initialize).
///
/// The apes and rocks are told apart by colour mode (enum SpriteColorMode):
/// an ape in hiding is SPRITE_COLOR_HIDDEN, a guide that comes out is drawn
/// with its red and green swapped (SPRITE_COLOR_GRB) and an ape that attacks
/// keeps SPRITE_APE's own colours, while the Killer Ape is the same sprite a
/// fifth larger with its colours rotated (SPRITE_COLOR_BRG). The rocks Move
/// settles are SPRITE_TURNING_ROCK drawn in greys (SPRITE_COLOR_GREY).

#include "action_script.h"
#include "save_block.h"
#include "engine_call.h"
#include "heap_slots.h"
#include "unit/import.h"
#include "unit/scene/mogall_forest.h"
#include "unit/scene/common/object_orbit.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"

#define FIELD_STAGED_OBJ_EVENT_IMPORTS
#include "object_event.h"
#include "map_grid_copy.h"
#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
#include "djinn_ids.h"
#include "field_effect.h"
#include "pushed_object.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "call.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "scene_header.h"
#include "fixed_math.h"
#include "vector3.h"
#include "flags.h"
#include "item_ids.h"
#include "songs.h"
#include "heap.h"

/// Object events of the first area (SceneId_MogallForestSign), numbered in the
/// order of gMogallForestSign_ObjectEventTemplates.
enum MogallForestSignLocalId {
  /// A log lying north to south at (488, 464), pushed as the river log is but
  /// with nothing happening where it stops.
  /// ★ Named from what the player sees: a fallen tree trunk lying in the
  /// grass (its render)
  LOCALID_FIRST_AREA_LOG = LOCALID_FIRST_PLACED,
  /// ★ Named from what the player sees: a fallen tree trunk lying in the
  /// grass (its render)
  LOCALID_FIRST_AREA_RIVER_LOG,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  LOCALID_FIRST_AREA_NORTH_APE,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  LOCALID_FIRST_AREA_ATTACKING_APE,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  LOCALID_FIRST_AREA_EAST_APE,
  /// ★ Named from what the player sees: a hollow purple stump an ape hides in
  /// (its render)
  LOCALID_FIRST_AREA_NORTH_APE_STUMP,
  /// ★ Named from what the player sees: a hollow purple stump an ape hides in
  /// (its render)
  LOCALID_FIRST_AREA_ATTACKING_APE_STUMP,
  /// ★ Named from what the player sees: a hollow purple stump an ape hides in
  /// (its render)
  LOCALID_FIRST_AREA_EAST_APE_STUMP,
  /// A Nut floating over the river, caught with Catch.
  /// ★ Named from the game: "Nut" (MsgItemName)
  LOCALID_NUT
};

/// Object events of the second area (SceneId_MogallForestQuartz), numbered in
/// the order of gMogallForestQuartz_ObjectEventTemplates.
enum MogallForestQuartzLocalId {
  /// ★ Named from what the player sees: a fallen tree trunk lying in the
  /// grass (its render)
  LOCALID_SECOND_AREA_LOG = LOCALID_FIRST_PLACED,
  /// A log lying west to east at (592, 456), south of LOCALID_SECOND_AREA_LOG,
  /// pushed with nothing happening where it stops.
  /// ★ Named from what the player sees: a fallen tree trunk lying in the
  /// grass (its render)
  LOCALID_SECOND_AREA_SOUTH_LOG,
  /// ★ Named from what the player sees: a fallen tree trunk lying in the
  /// grass (its render)
  LOCALID_SECOND_AREA_RIVER_LOG,
  /// A rock moved with Move, drawn as the rocks of Altmiller Cave's color
  /// puzzle (MsgAltmillerRotatedRock), drawn plain as those are while Reveal is
  /// off.
  /// ★ Named from what the player sees: a small grey rock in the grass
  /// (its render)
  LOCALID_SECOND_AREA_ROCK,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  LOCALID_SECOND_AREA_NORTH_APE,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  LOCALID_SECOND_AREA_EAST_APE,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  LOCALID_SECOND_AREA_SOUTH_APE,
  /// ★ Named from what the player sees: a hollow purple stump an ape hides in
  /// (its render)
  LOCALID_SECOND_AREA_NORTH_APE_STUMP,
  /// ★ Named from what the player sees: a hollow purple stump an ape hides in
  /// (its render)
  LOCALID_SECOND_AREA_EAST_APE_STUMP,
  /// ★ Named from what the player sees: a hollow purple stump an ape hides in
  /// (its render)
  LOCALID_SECOND_AREA_SOUTH_APE_STUMP,
  /// Quartz, a Venus Djinni, placed until the party has it.
  /// ★ Named from the game: "Quartz" (MsgDjinnName)
  LOCALID_SECOND_AREA_QUARTZ
};

/// Object events of the third area (SceneId_MogallForestKillerApe), numbered in
/// the order of gMogallForestKillerApe_ObjectEventTemplates.
enum MogallForestKillerApeLocalId {
  /// ★ Named from what the player sees: a fallen tree trunk lying in the
  /// grass (its render)
  LOCALID_THIRD_AREA_LOG = LOCALID_FIRST_PLACED,
  /// ★ Named from what the player sees: a fallen tree trunk lying in the
  /// grass (its render)
  LOCALID_THIRD_AREA_NORTH_RIVER_LOG,
  /// A log lying north to south at (552, 480), west of the two river logs,
  /// pushed with nothing happening where it stops.
  /// ★ Named from what the player sees: a fallen tree trunk lying in the
  /// grass (its render)
  LOCALID_THIRD_AREA_WEST_LOG,
  /// ★ Named from what the player sees: a fallen tree trunk lying in the
  /// grass (its render)
  LOCALID_THIRD_AREA_SOUTH_RIVER_LOG,
  /// A log lying west to east at (544, 536), south of
  /// LOCALID_THIRD_AREA_WEST_LOG, pushed with nothing happening where it stops.
  /// ★ Named from what the player sees: a fallen tree trunk lying in the
  /// grass (its render)
  LOCALID_THIRD_AREA_SOUTHWEST_LOG,
  /// Two rocks moved with Move, as in the second area.
  /// ★ Named from what the player sees: a small grey rock in the grass
  /// (its render)
  LOCALID_THIRD_AREA_NORTH_ROCK,
  /// ★ Named from what the player sees: a small grey rock in the grass
  /// (its render)
  LOCALID_THIRD_AREA_SOUTH_ROCK,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  LOCALID_THIRD_AREA_ATTACKING_APE,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  LOCALID_THIRD_AREA_MIDDLE_APE,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  LOCALID_THIRD_AREA_WEST_APE,
  /// Also the Killer Ape and its guide.
  /// ★ Named from the game: "Killer Ape" (MsgEnemyName)
  LOCALID_THIRD_AREA_EAST_APE,
  /// ★ Named from what the player sees: a hollow purple stump an ape hides in
  /// (its render)
  LOCALID_THIRD_AREA_ATTACKING_APE_STUMP,
  /// ★ Named from what the player sees: a hollow purple stump an ape hides in
  /// (its render)
  LOCALID_THIRD_AREA_MIDDLE_APE_STUMP,
  /// ★ Named from what the player sees: a hollow purple stump an ape hides in
  /// (its render)
  LOCALID_THIRD_AREA_WEST_APE_STUMP,
  /// ★ Named from what the player sees: a hollow purple stump an ape hides in
  /// (its render)
  LOCALID_THIRD_AREA_EAST_APE_STUMP
};

/// The animations of a river log's sprite (SPRITE_LOG_NORTH_SOUTH and
/// SPRITE_LOG_WEST_EAST), named by when these maps play them.
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

/// Map flag, kept while the party stays in the area.
enum {
  /// ★ Named from what the player sees: the rock beside the log, moved with
  /// Move (its render)
  FLAG_MOGALL_FOREST_SOUTH_ROCK_SETTLED = FLAG_SCENE_TEMP_14
};

// The third area's east ape: in the first patch it is placed only until it
// has come out, and from the first patch on the hiding place that shares its
// sprite too.
#if TBS_PATCH_LEVEL == 1
#define MOGALL_FOREST_EAST_OBJ_EVENT_CONDITION                                 \
  FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT
#else
#define MOGALL_FOREST_EAST_OBJ_EVENT_CONDITION CONDITION_ALWAYS
#endif

#if !TBS_PATCH_1
#define MOGALL_FOREST_EAST_SHARED_SPRITE_CONDITION CONDITION_ALWAYS
#else
#define MOGALL_FOREST_EAST_SHARED_SPRITE_CONDITION                             \
  FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT
#endif

#include "common/object_staged_object_event.h"

#include "common/effect_create.h"
#include "common/effect_spawn.h"

/// Mogall Forest, the second area: talking to Quartz, a Venus Djinni, starts
/// the Djinni battle.
static void MogallForest_StartDjinnBattle(void) {
  Field_RequestDjinnBattle(LOCALID_SECOND_AREA_QUARTZ, DJINNI_QUARTZ);
}

/// Jumps the leader to an unobstructed fixed-point position and restores
/// motion.
static s32 MogallForest_TryJumpLeaderTo(struct FieldPosition *position) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  u8 *motionFlags = &leader->motionFlags;
  s32 savedMotionFlags = *motionFlags;

  s32 collision = FieldObject_CheckMovementCollision(leader, position);

  if (collision == 0) {
    s32 jumpMotionFlags;

    FieldScript_Start();
    Object_SelectAnimation(leader, ANIM_LEADER_SPRING);
    WaitFrames(6);
    Sound_PlaySong(SE_JUMP);
    Object_SelectAnimation(leader, ANIM_LEADER_LEAP);
    leader->speed = PIXELS(3);
    leader->acceleration = PIXELS(2);
    leader->velocityY = PIXELS(4);
    jumpMotionFlags = 0x7e;
    jumpMotionFlags &= *motionFlags;
    *motionFlags = jumpMotionFlags;
    ObjectEvent_SetSpriteFlags(leader, 0);
    ObjectMotion_SetPositionAndCommit(LOCALID_PARTY_LEADER,
                                      (s16)((u32)position->x >> 16),
                                      (s16)((u32)position->z >> 16));
    Object_SelectAnimation(leader, ANIM_LEADER_SPRING);
    ObjectEvent_SetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
    *motionFlags = savedMotionFlags;
    FieldScript_End();
    return 1;
  }
  return 0;
}

/// Script command: draws the object event in the colour mode its timer holds.
static s32 MogallForest_CopyTimerToChild(struct Object *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  ObjectEvent_SetSpriteColor(objectEvent, objectEvent->timer);
  return 0;
}

/// Clears the object event's sprite flags when its script requests it.
static s32
MogallForest_ClearObjectEventSpriteFlags(struct Object *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  return 0;
}

/// Hops an object event to a pixel destination with the requested vertical
/// speed.
static void MogallForest_HopObjectEventTo(s32 localId, s32 targetX, s32 targetZ,
                                          s32 verticalSpeed) {
  struct ObjectEvent *objectEvent;

  objectEvent = ObjectEvent_GetById(localId);
  ObjectMotion_SetActionVariant(localId, 1);
  ObjectEvent_SetSpeed(localId, PIXELS(3), PIXELS(3) / 2);
  Sound_PlaySong(SE_JUMP);
  objectEvent->velocityY = verticalSpeed;
  objectEvent->gravity = PIXELS(1) / 2;
  objectEvent->bounce = 0;
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  ObjectEvent_MoveToAndWait(localId, targetX, targetZ);
  ObjectEvent_SetPosition(localId, PIXELS(targetX), PIXELS(targetZ));
  ObjectEvent_SetSpriteFlags(objectEvent, SPRITE_FLAG_SHADOW);
  objectEvent->gravity = PIXELS(1);
}

/// Scatters a complete ring of rising puffs around an object event.
static void MogallForest_SpawnPuffRing(s32 id) {
  struct Vector3 velocity;
  struct EffectOptions optionsStorage;
  struct EffectOptions *options;
  struct Vector3 *emissionVelocity;
  struct ObjectEvent *objectEvent;
  u32 directionIndex;
  s32 x;
  s32 z;

  objectEvent = ObjectEvent_GetById(id);
  Sound_PlaySong(SE_THUMP);
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
    FieldEffect_Spawn(objectEvent->x.fixed, PIXELS(16), objectEvent->z.fixed, x,
                      emissionVelocity->y + PIXELS(1) / 10, z,
                      EFFECT_USE_PRIORITY, options);
  }
}

/// The camera goes to the ape's hiding place, which bursts in a ring of
/// puffs, and the ape shows itself in colour mode color: SPRITE_COLOR_GRB for
/// a guide, SPRITE_COLOR_NORMAL for one that attacks.
static void MogallForest_ApeComesOut(s32 localId, s32 color) {
  s32 cameraSpeed = PIXELS(2);
  s32 cameraAcceleration = PIXELS(1) / 4;

  FieldCamera_SetSpeedParameters(cameraSpeed, cameraAcceleration);
  ObjectEvent_PlaceCurrentWithinCameraBounds(localId, 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(30);
  MogallForest_SpawnPuffRing(localId);
  ObjectEvent_ConfigureSpriteColor(localId, color);
}

/// The ape turns to the leader, hops and gestures down the way to go.
static inline void MogallForest_ApePointsTheWay(s32 guideId) {
  FieldCamera_FollowObjectEvent(guideId, 1);
  ObjectEvent_LinkPair(guideId, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVariantCallback(guideId, 2);
  FieldEffect_ConfigureLinkedObject(guideId, ATTACHED_EFFECT_IN_FRONT |
                                                 ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
}

const struct MapWarpDest *MogallForest_GetWarpDests(void) {
  s16 selector = gSaveBlock.sceneId;

  if (selector == (s32)&SceneId_MogallForestSign) {
    return gMogallForestSign_WarpDests;
  }
  if (selector == (s32)&SceneId_MogallForestQuartz) {
    return gMogallForestQuartz_WarpDests;
  }
  if (selector == (s32)&SceneId_MogallForestKillerApe) {
    return gMogallForestKillerApe_WarpDests;
  }
  return gMogallForestSidePaths_WarpDests;
}

const struct MapWarpRegion *MogallForest_GetRegions(void) { return NULL; }

/// Returns the warps, one table for all four maps.
const u32 *MogallForest_GetWarps(void) { return gMogallForest_Warps; }

const struct ObjectEventTemplate *MogallForest_GetObjectEventTemplates(void) {
  s16 selector = gSaveBlock.sceneId;

  if (selector == (s32)&SceneId_MogallForestSign) {
    return gMogallForestSign_ObjectEventTemplates;
  }
  if (selector == (s32)&SceneId_MogallForestQuartz) {
    return gMogallForestQuartz_ObjectEventTemplates;
  }
  if (selector == (s32)&SceneId_MogallForestKillerApe) {
    return gMogallForestKillerApe_ObjectEventTemplates;
  }
  return gMogallForestSidePaths_ObjectEventTemplates;
}

/// Mogall Forest, the first area: walking into a log (metatile behavior 20)
/// pushes it; the river log pushed to row 26 rolls into the water and can be
/// crossed. Sets FLAG_MOGALL_FOREST_FIRST_AREA_LOG_ROLLED.
static void MogallForest_FirstArea_PushLog(void) {
  struct PushedObjectProbe probe;

  FieldScript_Start();
  if (PushedObject_FindClearPosition(&probe) != 0) {
    ObjectEvent_MoveAndRedraw(probe);
    if (probe.objectEventSlot == LOCALID_FIRST_AREA_RIVER_LOG &&
        (probe.position.z >> 20) == 26) {
      GameFlag_Set(FLAG_MOGALL_FOREST_FIRST_AREA_LOG_ROLLED);
      Import_ObjectEventSetAnimation(LOCALID_FIRST_AREA_RIVER_LOG,
                                     MOGALL_FOREST_LOG_ROLLING);
      ObjectEvent_SetSpeed(LOCALID_FIRST_AREA_RIVER_LOG, PIXELS(1) / 4,
                           PIXELS(1) / 2);
      ObjectMotion_OffsetPositionAndResetMotion(LOCALID_FIRST_AREA_RIVER_LOG, 0,
                                                -16);
      FieldScript_WaitFrames(45);
      Import_ObjectEventSetAnimation(LOCALID_FIRST_AREA_RIVER_LOG,
                                     MOGALL_FOREST_LOG_FALLING_IN);
      Sound_PlaySong(SE_LOG_FALL);
      ObjectMotion_SetActionVariant(LOCALID_FIRST_AREA_RIVER_LOG, 1);
      ObjectEvent_GetById(LOCALID_FIRST_AREA_RIVER_LOG)->priorityFlags =
          OBJ_EVENT_PRIORITY_UNDERFOOT;
      MapGrid_CopyCellAttributes(38, 27, 4, 2, 31, 25);
    }
  }
  FieldScript_End();
}

/// Mogall Forest, the first area: walking north into metatile behavior 25 jumps
/// the leader north across the log.
static void MogallForest_FirstArea_JumpNorth(void) {
  struct FieldPosition position;
#if TBS_PATCH_1
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

  position.x = (leader->x.fixed & 0xfff00000) + PIXELS(8);
  position.y = leader->y.fixed;
  position.z = (leader->z.fixed & 0xfff00000) - PIXELS(24);
#else
  position.x = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->x.fixed;
  position.y = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->y.fixed;
  position.z = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.fixed - PIXELS(32);
#endif
  MogallForest_TryJumpLeaderTo(&position);
}

/// Mogall Forest, the first area: walking south into metatile behavior 26 jumps
/// the leader onto the log, which bobs under the weight.
static void MogallForest_FirstArea_JumpSouth(void) {
  struct FieldPosition position;
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  u8 *motionFlags = &objectEvent->motionFlags;
  s32 savedMotionFlags = *motionFlags;

#if TBS_PATCH_1
  position.x = (objectEvent->x.fixed & 0xfff00000) + PIXELS(8);
  position.y = objectEvent->y.fixed;
  position.z = (objectEvent->z.fixed & 0xfff00000) + PIXELS(40);
#else
  position.x = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->x.fixed;
  position.y = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->y.fixed;
  position.z = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.fixed + PIXELS(32);
#endif
  if (MogallForest_TryJumpLeaderTo(&position) != 0) {
    FieldScript_Start();
    *motionFlags = 0;
    Import_ObjectEventSetAnimation(LOCALID_FIRST_AREA_RIVER_LOG,
                                   MOGALL_FOREST_LOG_STEPPED_ON);
    objectEvent->y.fixed += -PIXELS(1);
    objectEvent->groundHeight += -PIXELS(1);
    WaitFrames(2);
    objectEvent->y.fixed += -PIXELS(1);
    objectEvent->groundHeight += -PIXELS(1);
    WaitFrames(10);
    objectEvent->y.fixed += PIXELS(1);
    objectEvent->groundHeight += PIXELS(1);
    WaitFrames(4);
    objectEvent->y.fixed += PIXELS(1);
    objectEvent->groundHeight += PIXELS(1);
    *motionFlags = savedMotionFlags;
    FieldScript_End();
  }
}

/// Mogall Forest, the first area: searching the sign (metatile behavior 27).
/// "The broken sign reads: North: Fuchin Temple / South: Mogall Prairie..."
static void MogallForest_ReadBrokenSign(void) {
  FieldScript_Start();
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  FieldMessage_Show(MSGNO(MsgMogallForestBrokenSign), 1);
  FieldScript_End();
}

/// Mogall Forest, the first area: Force on the north hiding place. The ape
/// comes out, points the way and hops off south.
static void MogallForest_FirstArea_NorthApeShowsWay(void) {
  struct ObjectEvent *creature;

  creature = ObjectEvent_GetById(LOCALID_FIRST_AREA_NORTH_APE);

  FieldScript_Start();

  MogallForest_ApeComesOut(LOCALID_FIRST_AREA_NORTH_APE, SPRITE_COLOR_GRB);
  MogallForest_HopObjectEventTo(LOCALID_FIRST_AREA_NORTH_APE, 88, 120,
                                PIXELS(6));

  FieldEffect_Spawn(creature->x.fixed, creature->y.fixed,
                    creature->z.fixed + PIXELS(24), 0, 0, 0,
                    EFFECT_SCRIPT_MEDIUM, 0);

  MogallForest_ApePointsTheWay(LOCALID_FIRST_AREA_NORTH_APE);

  MogallForest_HopObjectEventTo(LOCALID_FIRST_AREA_NORTH_APE, 88, 152,
                                PIXELS(3));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER,
                              LOCALID_FIRST_AREA_NORTH_APE, 0);
  FieldScript_WaitFrames(10);

  MogallForest_HopObjectEventTo(LOCALID_FIRST_AREA_NORTH_APE, 120, 192,
                                PIXELS(3));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER,
                              LOCALID_FIRST_AREA_NORTH_APE, 0);
  FieldScript_WaitFrames(10);

  MogallForest_HopObjectEventTo(LOCALID_FIRST_AREA_NORTH_APE, 120, 240,
                                PIXELS(3));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER,
                              LOCALID_FIRST_AREA_NORTH_APE, 0);
  FieldScript_WaitFrames(10);

  GameFlag_Set(FLAG_MOGALL_FOREST_FIRST_AREA_NORTH_APE_OUT);
  Import_ObjectEventSetPosition(LOCALID_FIRST_AREA_NORTH_APE_STUMP, 0, 0);
  Import_ObjectEventSetPosition(LOCALID_FIRST_AREA_NORTH_APE, 0, 0);

  FieldScript_End();
}

/// Mogall Forest, the first area: Force on the attacking ape's hiding place. It
/// comes out angry, leaps at the leader and the battle with an Ape begins
/// (ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, slot 0).
static void MogallForest_FirstArea_ApeAttacks(void) {
  struct ObjectEvent *creature;

  creature = ObjectEvent_GetById(LOCALID_FIRST_AREA_ATTACKING_APE);

  FieldScript_Start();

  MogallForest_ApeComesOut(LOCALID_FIRST_AREA_ATTACKING_APE,
                           SPRITE_COLOR_NORMAL);
  MogallForest_HopObjectEventTo(LOCALID_FIRST_AREA_ATTACKING_APE, 408, 456,
                                PIXELS(6));

  FieldEffect_Spawn(creature->x.fixed, creature->y.fixed,
                    creature->z.fixed + PIXELS(24), 0, 0, 0,
                    EFFECT_SCRIPT_MEDIUM, 0);

  FieldCamera_FollowObjectEvent(LOCALID_FIRST_AREA_ATTACKING_APE, 1);
  ObjectEvent_LinkPair(LOCALID_FIRST_AREA_ATTACKING_APE, LOCALID_PARTY_LEADER,
                       0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetVariantCallback(LOCALID_FIRST_AREA_ATTACKING_APE, 2);
  FieldEffect_SpawnLinked(LOCALID_FIRST_AREA_ATTACKING_APE,
                          EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Sound_PlaySong(SE_BEAST_CRY);
  FieldScript_WaitFrames(60);

  MogallForest_HopObjectEventTo(
      LOCALID_FIRST_AREA_ATTACKING_APE,
      ObjectEvent_GetById(LOCALID_PARTY_LEADER)->x.part.pixel,
      ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.part.pixel, PIXELS(4));

  FieldScript_WaitFrames(10);
  GameFlag_Set(FLAG_MOGALL_FOREST_FIRST_AREA_ATTACKING_APE_OUT);
  Import_ObjectEventSetPosition(LOCALID_FIRST_AREA_ATTACKING_APE_STUMP, 0, 0);

  Battle_SetType(BATTLE_TYPE_SCRIPTED);

  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 0);

  FieldScript_End();
}

/// Mogall Forest, the first area: Force on the east hiding place. The ape
/// comes out, points the way and hops off east.
static void MogallForest_FirstArea_EastApeShowsWay(void) {
  struct ObjectEvent *creature;

  creature = ObjectEvent_GetById(LOCALID_FIRST_AREA_EAST_APE);

  FieldScript_Start();

  MogallForest_ApeComesOut(LOCALID_FIRST_AREA_EAST_APE, SPRITE_COLOR_GRB);
  MogallForest_HopObjectEventTo(LOCALID_FIRST_AREA_EAST_APE, 536, 344,
                                PIXELS(7));

  FieldEffect_Spawn(creature->x.fixed, creature->y.fixed,
                    creature->z.fixed + PIXELS(16), 0, 0, 0,
                    EFFECT_SCRIPT_MEDIUM, 0);

  MogallForest_ApePointsTheWay(LOCALID_FIRST_AREA_EAST_APE);

  MogallForest_HopObjectEventTo(LOCALID_FIRST_AREA_EAST_APE, 584, 344,
                                PIXELS(3));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_FIRST_AREA_EAST_APE,
                              0);
  FieldScript_WaitFrames(6);

  MogallForest_HopObjectEventTo(LOCALID_FIRST_AREA_EAST_APE, 632, 344,
                                PIXELS(3));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_FIRST_AREA_EAST_APE,
                              0);
  FieldScript_WaitFrames(6);

  MogallForest_HopObjectEventTo(LOCALID_FIRST_AREA_EAST_APE, 680, 344,
                                PIXELS(3));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_FIRST_AREA_EAST_APE,
                              0);
  FieldScript_WaitFrames(6);

  GameFlag_Set(FLAG_MOGALL_FOREST_FIRST_AREA_EAST_APE_OUT);
  Import_ObjectEventSetPosition(LOCALID_FIRST_AREA_EAST_APE_STUMP, 0, 0);

  FieldScript_End();
}

/// Mogall Forest, the first area: Catch on the Nut floating over the river
/// brings it to the party. Sets FLAG_MOGALL_FOREST_NUT_CAUGHT.
static void MogallForest_CatchNut(void) {
  FieldScript_Start();
  Import_ObjectEventSetPosition(LOCALID_NUT, 0, 0);
  GameFlag_Set(FLAG_MOGALL_FOREST_NUT_CAUGHT);
  ItemDisplay_ShowItem(ITEM_NUT, 3);
  PartyInventory_GiveItem(ITEM_NUT, 0);
  FieldScript_End();
}

/// Mogall Forest, the second area: walking into a log (metatile behavior 20)
/// pushes it; the log at row 23 changes the ground beside it, and the river log
/// pushed to column 35 rolls into the water.
static void MogallForest_SecondArea_PushLog(void) {
  struct PushedObjectProbe probe;
  s32 destinationColumn;
  s32 destinationRow;
  s32 rectangleHeight;
  s32 attributes;

  FieldScript_Start();

  if (PushedObject_FindClearPosition(&probe) != 0) {
    ObjectEvent_MoveAndRedraw(probe);

    if (probe.objectEventSlot == LOCALID_SECOND_AREA_LOG &&
        (probe.position.z >> 20) == 23) {
      destinationColumn = 35;
      destinationRow = 68;
      MapGrid_CopyCellAttributes(35, 67, 4, 1, destinationColumn,
                                 destinationRow);
    } else if (probe.objectEventSlot == LOCALID_SECOND_AREA_RIVER_LOG &&
               (probe.position.x >> 20) == 35) {

      attributes = 0;
      GameFlag_Set(FLAG_MOGALL_FOREST_SECOND_AREA_LOG_ROLLED);
      Import_ObjectEventSetAnimation(LOCALID_SECOND_AREA_RIVER_LOG,
                                     MOGALL_FOREST_LOG_ROLLING);
      ObjectMotion_OffsetPositionAndResetMotion(LOCALID_SECOND_AREA_RIVER_LOG,
                                                -16, 6);
      FieldScript_WaitFrames(30);
      Import_ObjectEventSetAnimation(LOCALID_SECOND_AREA_RIVER_LOG,
                                     MOGALL_FOREST_LOG_FALLING_IN);
      Sound_PlaySong(SE_LOG_FALL);

      ObjectEvent_GetById(LOCALID_SECOND_AREA_RIVER_LOG)->priorityFlags =
          OBJ_EVENT_PRIORITY_UNDERFOOT;

      destinationColumn = 34;
      destinationRow = 30;
      MapGrid_CopyCellAttributes(44, 30, 2, 4, destinationColumn,
                                 destinationRow);
      rectangleHeight = 4;
      PushedObject_FillGridAttributeRectangle(2, 35, 30, 1, rectangleHeight,
                                              attributes);
    }
  }

  FieldScript_End();
}

/// Marks the rock's cell and clears the cells it could have moved from.
static inline void MogallForest_UpdateRockFootprint(s32 column, s32 row) {
  PushedObject_FillGridAttributeRectangle(2, column, row, 1, 1, MB_WALL);
  PushedObject_FillGridAttributeRectangle(2, column + 1, row, 1, 1, MB_NORMAL);
  PushedObject_FillGridAttributeRectangle(2, column - 1, row, 1, 1, MB_NORMAL);
  PushedObject_FillGridAttributeRectangle(2, column, row + 1, 1, 1, MB_NORMAL);
  PushedObject_FillGridAttributeRectangle(2, column, row - 1, 1, 1, MB_NORMAL);
}

/// Mogall Forest, the second area: Move on the rock. Its footprint follows it,
/// and at (36, 24) it sinks two pixels into place.
static void MogallForest_SecondArea_UpdateRock(void) {
  s32 column;
  s32 row;

  FieldScript_Start();

  column = ObjectEvent_GetById(LOCALID_SECOND_AREA_ROCK)->x.fixed >> 20;
  row = ObjectEvent_GetById(LOCALID_SECOND_AREA_ROCK)->z.fixed >> 20;

  MogallForest_UpdateRockFootprint(column, row);

  if (column == 36 && row == 24) {
    struct ObjectEvent *rock = ObjectEvent_GetById(LOCALID_SECOND_AREA_ROCK);

    rock->motionFlags = 0;
    rock->groundHeight = -PIXELS(2);
    rock->y.fixed = -PIXELS(2);
  }

  FieldScript_End();
}

/// Mogall Forest, the second area: Force on the north hiding place. The ape
/// points the way and hops off south.
static void MogallForest_SecondArea_NorthApeShowsWay(void) {
  struct ObjectEvent *creature;

  creature = ObjectEvent_GetById(LOCALID_SECOND_AREA_NORTH_APE);
  FieldScript_Start();
  MogallForest_ApeComesOut(LOCALID_SECOND_AREA_NORTH_APE, SPRITE_COLOR_GRB);
  MogallForest_HopObjectEventTo(LOCALID_SECOND_AREA_NORTH_APE, 392, 104,
                                PIXELS(7));
  FieldScript_WaitFrames(10);
  FieldEffect_Spawn(creature->x.fixed, creature->y.fixed,
                    (creature->z.fixed + PIXELS(4)), 0, 0, 0,
                    EFFECT_SCRIPT_MEDIUM, 0);
  MogallForest_ApePointsTheWay(LOCALID_SECOND_AREA_NORTH_APE);
  MogallForest_HopObjectEventTo(LOCALID_SECOND_AREA_NORTH_APE, 424, 120,
                                PIXELS(3));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER,
                              LOCALID_SECOND_AREA_NORTH_APE, 0);
  FieldScript_WaitFrames(6);
  MogallForest_HopObjectEventTo(LOCALID_SECOND_AREA_NORTH_APE, 424, 168,
                                PIXELS(3));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER,
                              LOCALID_SECOND_AREA_NORTH_APE, 0);
  FieldScript_WaitFrames(6);
  MogallForest_HopObjectEventTo(LOCALID_SECOND_AREA_NORTH_APE, 424, 208,
                                PIXELS(3));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER,
                              LOCALID_SECOND_AREA_NORTH_APE, 0);
  FieldScript_WaitFrames(6);
  MogallForest_HopObjectEventTo(LOCALID_SECOND_AREA_NORTH_APE, 424, 232,
                                PIXELS(3));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER,
                              LOCALID_SECOND_AREA_NORTH_APE, 0);
  FieldScript_WaitFrames(6);
  Import_ObjectEventSetPosition(LOCALID_SECOND_AREA_NORTH_APE, 0, 0);
  GameFlag_Set(FLAG_MOGALL_FOREST_SECOND_AREA_NORTH_APE_OUT);
  Import_ObjectEventSetPosition(LOCALID_SECOND_AREA_NORTH_APE_STUMP, 0, 0);
  FieldScript_End();
}

/// Mogall Forest, the second area: Force on the east hiding place. The ape
/// points the way and hops off east.
static void MogallForest_SecondArea_EastApeShowsWay(void) {
  struct ObjectEvent *creature;

  creature = ObjectEvent_GetById(LOCALID_SECOND_AREA_EAST_APE);

  FieldScript_Start();

  MogallForest_ApeComesOut(LOCALID_SECOND_AREA_EAST_APE, SPRITE_COLOR_GRB);
  MogallForest_HopObjectEventTo(LOCALID_SECOND_AREA_EAST_APE, 456, 104,
                                PIXELS(7));
  FieldScript_WaitFrames(10);

  FieldEffect_Spawn(creature->x.fixed, creature->y.fixed,
                    creature->z.fixed + PIXELS(4), 0, 0, 0,
                    EFFECT_SCRIPT_MEDIUM, 0);

  MogallForest_ApePointsTheWay(LOCALID_SECOND_AREA_EAST_APE);

  MogallForest_HopObjectEventTo(LOCALID_SECOND_AREA_EAST_APE, 472, 136,
                                PIXELS(3));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER,
                              LOCALID_SECOND_AREA_EAST_APE, 0);
  FieldScript_WaitFrames(6);

  MogallForest_HopObjectEventTo(LOCALID_SECOND_AREA_EAST_APE, 504, 136,
                                PIXELS(16) / 5);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER,
                              LOCALID_SECOND_AREA_EAST_APE, 0);
  FieldScript_WaitFrames(6);

  MogallForest_HopObjectEventTo(LOCALID_SECOND_AREA_EAST_APE, 552, 136,
                                PIXELS(7) / 2);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER,
                              LOCALID_SECOND_AREA_EAST_APE, 0);
  FieldScript_WaitFrames(6);

  MogallForest_HopObjectEventTo(LOCALID_SECOND_AREA_EAST_APE, 584, 136,
                                PIXELS(7) / 2);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER,
                              LOCALID_SECOND_AREA_EAST_APE, 0);
  FieldScript_WaitFrames(6);

  Import_ObjectEventSetPosition(LOCALID_SECOND_AREA_EAST_APE, 0, 0);
  GameFlag_Set(FLAG_MOGALL_FOREST_SECOND_AREA_EAST_APE_OUT);
  Import_ObjectEventSetPosition(LOCALID_SECOND_AREA_EAST_APE_STUMP, 0, 0);

  FieldScript_End();
}

/// Mogall Forest, the second area: Force on the south hiding place the first
/// time. The ape bolts to another hiding place nearby.
static void MogallForest_SecondArea_SouthApeHidesAgain(void) {
  struct ObjectEvent *creature;

  FieldScript_Start();
  MogallForest_ApeComesOut(LOCALID_SECOND_AREA_SOUTH_APE, SPRITE_COLOR_GRB);
  MogallForest_HopObjectEventTo(LOCALID_SECOND_AREA_SOUTH_APE, 424, 480,
                                PIXELS(38) / 5);
  FieldScript_WaitFrames(2);
  MogallForest_SpawnPuffRing(LOCALID_SECOND_AREA_SOUTH_APE);
  ObjectEvent_SetSpriteColorById(LOCALID_SECOND_AREA_SOUTH_APE,
                                 SPRITE_COLOR_HIDDEN);
  creature = ObjectEvent_Get(LOCALID_SECOND_AREA_SOUTH_APE);
  ObjectEvent_SetSpriteFlags(creature, 0);
  FieldScript_WaitFrames(30);
  GameFlag_Set(FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_MOVED);
  ObjectEvent_SetPosition(LOCALID_SECOND_AREA_SOUTH_APE_STUMP, PIXELS(424),
                          PIXELS(480));
  FieldScript_End();
}

/// Mogall Forest, the second area: Force on the south ape's new hiding place.
/// It points the way and hops off west.
static void MogallForest_SecondArea_SouthApeShowsWay(void) {
  struct ObjectEvent *creature;

  creature = ObjectEvent_GetById(LOCALID_SECOND_AREA_SOUTH_APE);

  FieldScript_Start();

  MogallForest_ApeComesOut(LOCALID_SECOND_AREA_SOUTH_APE, SPRITE_COLOR_GRB);
  MogallForest_HopObjectEventTo(LOCALID_SECOND_AREA_SOUTH_APE, 392, 504,
                                PIXELS(6));
  FieldScript_WaitFrames(10);

  FieldEffect_Spawn(creature->x.fixed, creature->y.fixed,
                    creature->z.fixed + PIXELS(4), 0, 0, 0,
                    EFFECT_SCRIPT_MEDIUM, 0);

  MogallForest_ApePointsTheWay(LOCALID_SECOND_AREA_SOUTH_APE);

  MogallForest_HopObjectEventTo(LOCALID_SECOND_AREA_SOUTH_APE, 360, 528,
                                PIXELS(3));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER,
                              LOCALID_SECOND_AREA_SOUTH_APE, 0);
  FieldScript_WaitFrames(6);

  MogallForest_HopObjectEventTo(LOCALID_SECOND_AREA_SOUTH_APE, 328, 528,
                                PIXELS(3));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER,
                              LOCALID_SECOND_AREA_SOUTH_APE, 0);
  FieldScript_WaitFrames(6);

  MogallForest_HopObjectEventTo(LOCALID_SECOND_AREA_SOUTH_APE, 288, 528,
                                PIXELS(3));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER,
                              LOCALID_SECOND_AREA_SOUTH_APE, 0);
  FieldScript_WaitFrames(6);

  MogallForest_HopObjectEventTo(LOCALID_SECOND_AREA_SOUTH_APE, 256, 528,
                                PIXELS(3));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER,
                              LOCALID_SECOND_AREA_SOUTH_APE, 0);
  FieldScript_WaitFrames(6);

  FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 1);
  Import_ObjectEventSetPosition(LOCALID_SECOND_AREA_SOUTH_APE, 0, 0);
  FieldScript_WaitFrames(30);
  GameFlag_Set(FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_OUT);
  Import_ObjectEventSetPosition(LOCALID_SECOND_AREA_SOUTH_APE_STUMP, 0, 0);

  FieldScript_End();
}

/// Mogall Forest, the third area: walking into a log (metatile behavior 20)
/// pushes it; each river log rolls into the water at its column (42 and 40) and
/// can be crossed.
static void MogallForest_ThirdArea_PushLog(void) {
  struct PushedObjectProbe probe;
  s32 landed;

  FieldScript_Start();
  landed = 0;
  if (PushedObject_FindClearPosition(&probe) != 0) {
    ObjectEvent_MoveAndRedraw(probe);
    if (probe.objectEventSlot == LOCALID_THIRD_AREA_NORTH_RIVER_LOG ||
        probe.objectEventSlot == LOCALID_THIRD_AREA_SOUTH_RIVER_LOG) {
      if (probe.objectEventSlot == LOCALID_THIRD_AREA_NORTH_RIVER_LOG) {
        MapGrid_CopyCellAttributes(38, 68, 1, 4, probe.position.x >> 20, 68);
        if (probe.position.x >> 20 == 42) {
          MapGrid_CopyCellAttributes(26, 20, 2, 4, probe.position.x >> 20, 23);
          ObjectMotion_SetActionVariant(LOCALID_THIRD_AREA_NORTH_RIVER_LOG, 1);
          landed = 1;
          GameFlag_Set(FLAG_MOGALL_FOREST_THIRD_AREA_NORTH_LOG_ROLLED);
        }
      } else {
        if (probe.position.x >> 20 == 40) {
          MapGrid_CopyCellAttributes(26, 20, 2, 4, probe.position.x >> 20, 32);
          ObjectMotion_SetActionVariant(LOCALID_THIRD_AREA_SOUTH_RIVER_LOG, 1);
          landed = 1;
          GameFlag_Set(FLAG_MOGALL_FOREST_THIRD_AREA_SOUTH_LOG_ROLLED);
        }
      }
      if (landed == 0) {
        FieldScript_End();
        return;
      }
      Import_ObjectEventSetAnimation(probe.objectEventSlot,
                                     MOGALL_FOREST_LOG_ROLLING);
      ObjectMotion_OffsetPositionAndResetMotion(probe.objectEventSlot, 18, 6);
      FieldScript_WaitFrames(30);
      Import_ObjectEventSetAnimation(probe.objectEventSlot,
                                     MOGALL_FOREST_LOG_FALLING_IN);
      Sound_PlaySong(SE_LOG_FALL);
      ObjectEvent_Get(probe.objectEventSlot)->priorityFlags =
          OBJ_EVENT_PRIORITY_UNDERFOOT;
    } else {
      if (probe.objectEventSlot == LOCALID_THIRD_AREA_LOG)
        MapGrid_CopyCellAttributes(42, 49, 1, 4, probe.position.x >> 20, 49);
    }
  }
  FieldScript_End();
}

/// Mogall Forest, the third area: walking west into metatile behavior 25 jumps
/// the leader onto the second river log, which bobs under the weight.
static void MogallForest_ThirdArea_JumpWest(void) {
  struct ObjectEvent *leader;
  u8 *motionFlags;
  u8 savedMotionFlags;
  struct FieldPosition position;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  motionFlags = &leader->motionFlags;
  savedMotionFlags = *motionFlags;

  position.x = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->x.fixed - PIXELS(32);
  position.y = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->y.fixed;
  position.z = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.fixed;

  if (MogallForest_TryJumpLeaderTo(&position) != 0) {

    FieldScript_Start();

    *motionFlags = 0;
    Import_ObjectEventSetAnimation(LOCALID_THIRD_AREA_SOUTH_RIVER_LOG,
                                   MOGALL_FOREST_LOG_STEPPED_ON);

    leader->y.fixed += -PIXELS(1);
    leader->groundHeight += -PIXELS(1);
    WaitFrames(2);

    leader->y.fixed += -PIXELS(1);
    leader->groundHeight += -PIXELS(1);
    WaitFrames(10);

    leader->y.fixed += PIXELS(1);
    leader->groundHeight += PIXELS(1);
    WaitFrames(4);

    leader->y.fixed += PIXELS(1);
    leader->groundHeight += PIXELS(1);

    *motionFlags = savedMotionFlags;
    FieldScript_End();
  }
}

/// Mogall Forest, the third area: Move on the north rock; at (45, 6) it
/// sinks into place.
static void MogallForest_ThirdArea_UpdateNorthRock(void) {
  s32 column;
  s32 row;

  FieldScript_Start();

  column = ObjectEvent_GetById(LOCALID_THIRD_AREA_NORTH_ROCK)->x.fixed >> 20;
  row = ObjectEvent_GetById(LOCALID_THIRD_AREA_NORTH_ROCK)->z.fixed >> 20;

  MogallForest_UpdateRockFootprint(column, row);

  if (column == 45 && row == 6) {
    struct ObjectEvent *rock =
        ObjectEvent_GetById(LOCALID_THIRD_AREA_NORTH_ROCK);

    rock->motionFlags = 0;
    rock->groundHeight = -PIXELS(2);
    rock->y.fixed = -PIXELS(2);
  }

  FieldScript_End();
}

/// Mogall Forest, the third area: Move on the south rock; on row 27 it
/// sinks into place and stays.
static void MogallForest_ThirdArea_UpdateSouthRock(void) {

  s32 column;
  s32 row;
  struct ObjectEvent *rock;

  FieldScript_Start();
  rock = ObjectEvent_GetById(LOCALID_THIRD_AREA_SOUTH_ROCK);
  column = rock->x.fixed;
  rock = ObjectEvent_GetById(LOCALID_THIRD_AREA_SOUTH_ROCK);
  row = rock->z.fixed;
  column >>= 20;
  row >>= 20;
  MogallForest_UpdateRockFootprint(column, row);
  rock = ObjectEvent_GetById(LOCALID_THIRD_AREA_SOUTH_ROCK);
  if ((rock->z.fixed >> 20) == 27) {
    rock = ObjectEvent_GetById(LOCALID_THIRD_AREA_SOUTH_ROCK);
    rock->motionFlags = 0;
    rock->groundHeight = -PIXELS(2);
    rock->y.fixed = -PIXELS(2);
    GameFlag_Set(FLAG_MOGALL_FOREST_SOUTH_ROCK_SETTLED);
    PushedObject_FillGridAttributeRectangle(2, 43, 23, 1, 1, MB_WALL);
  }
  FieldScript_End();
}

/// Mogall Forest, the third area: Force on the attacking ape's hiding place.
/// The ape leaps at the leader and the battle with an Ape begins.
static void MogallForest_ThirdArea_ApeAttacks(void) {

  struct ObjectEvent *creature;
  s32 jumpSpeed;
  struct ObjectEvent *leaderForX;
  s32 leaderX;
  struct ObjectEvent *leaderForZ;

  creature = ObjectEvent_GetById(LOCALID_THIRD_AREA_ATTACKING_APE);
  jumpSpeed = PIXELS(8);
  FieldScript_Start();
  MogallForest_ApeComesOut(LOCALID_THIRD_AREA_ATTACKING_APE,
                           SPRITE_COLOR_NORMAL);
  MogallForest_HopObjectEventTo(LOCALID_THIRD_AREA_ATTACKING_APE, 472, 104,
                                jumpSpeed);
  FieldScript_WaitFrames(10);
  FieldEffect_Spawn(creature->x.fixed, creature->y.fixed,
                    (creature->z.fixed + jumpSpeed), 0, 0, 0,
                    EFFECT_SCRIPT_MEDIUM, 0);
  FieldCamera_FollowObjectEvent(LOCALID_THIRD_AREA_ATTACKING_APE, 1);
  ObjectEvent_LinkPair(LOCALID_THIRD_AREA_ATTACKING_APE, LOCALID_PARTY_LEADER,
                       0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetVariantCallback(LOCALID_THIRD_AREA_ATTACKING_APE, 2);
  FieldEffect_SpawnLinked(LOCALID_THIRD_AREA_ATTACKING_APE,
                          EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Sound_PlaySong(SE_BEAST_CRY);
  FieldScript_WaitFrames(60);
  leaderForX = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  leaderX = leaderForX->x.part.pixel;
  leaderForZ = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  MogallForest_HopObjectEventTo(LOCALID_THIRD_AREA_ATTACKING_APE, leaderX,
                                leaderForZ->z.part.pixel, PIXELS(6));
  FieldScript_WaitFrames(10);
  GameFlag_Set(FLAG_MOGALL_FOREST_THIRD_AREA_ATTACKING_APE_OUT);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 0);
  FieldScript_End();
}

/// Mogall Forest, the third area: Force on the middle hiding place. The ape
/// points the way and hops off south.
static void MogallForest_ThirdArea_MiddleApeShowsWay(void) {
  struct ObjectEvent *creature;

  creature = ObjectEvent_GetById(LOCALID_THIRD_AREA_MIDDLE_APE);

  FieldScript_Start();

  MogallForest_ApeComesOut(LOCALID_THIRD_AREA_MIDDLE_APE, SPRITE_COLOR_GRB);
  MogallForest_HopObjectEventTo(LOCALID_THIRD_AREA_MIDDLE_APE, 456, 152,
                                PIXELS(6));
  FieldScript_WaitFrames(10);

  FieldEffect_Spawn(creature->x.fixed, creature->y.fixed,
                    creature->z.fixed + PIXELS(4), 0, 0, 0,
                    EFFECT_SCRIPT_MEDIUM, 0);

  MogallForest_ApePointsTheWay(LOCALID_THIRD_AREA_MIDDLE_APE);

  MogallForest_HopObjectEventTo(LOCALID_THIRD_AREA_MIDDLE_APE, 448, 192,
                                PIXELS(3));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER,
                              LOCALID_THIRD_AREA_MIDDLE_APE, 0);
  FieldScript_WaitFrames(6);

  MogallForest_HopObjectEventTo(LOCALID_THIRD_AREA_MIDDLE_APE, 424, 208,
                                PIXELS(3));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER,
                              LOCALID_THIRD_AREA_MIDDLE_APE, 0);
  FieldScript_WaitFrames(6);

  MogallForest_HopObjectEventTo(LOCALID_THIRD_AREA_MIDDLE_APE, 424, 224,
                                PIXELS(3));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER,
                              LOCALID_THIRD_AREA_MIDDLE_APE, 0);
  FieldScript_WaitFrames(6);

  FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 1);
  Import_ObjectEventSetPosition(LOCALID_THIRD_AREA_MIDDLE_APE, 0, 0);
  FieldScript_WaitFrames(30);
  GameFlag_Set(FLAG_MOGALL_FOREST_THIRD_AREA_MIDDLE_APE_OUT);
  Import_ObjectEventSetPosition(LOCALID_THIRD_AREA_MIDDLE_APE_STUMP, 0, 0);

  FieldScript_End();
}

/// Mogall Forest, the third area: Force on the west hiding place. The ape
/// points the way and hops off west.
static void MogallForest_ThirdArea_WestApeShowsWay(void) {
  struct ObjectEvent *creature;

  creature = ObjectEvent_GetById(LOCALID_THIRD_AREA_WEST_APE);

  FieldScript_Start();

  MogallForest_ApeComesOut(LOCALID_THIRD_AREA_WEST_APE, SPRITE_COLOR_GRB);
  MogallForest_HopObjectEventTo(LOCALID_THIRD_AREA_WEST_APE, 392, 104,
                                PIXELS(6));
  FieldScript_WaitFrames(10);

  FieldEffect_Spawn(creature->x.fixed, creature->y.fixed,
                    creature->z.fixed + PIXELS(4), 0, 0, 0,
                    EFFECT_SCRIPT_MEDIUM, 0);

  MogallForest_ApePointsTheWay(LOCALID_THIRD_AREA_WEST_APE);

  MogallForest_HopObjectEventTo(LOCALID_THIRD_AREA_WEST_APE, 376, 152,
                                PIXELS(6));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_THIRD_AREA_WEST_APE,
                              0);
  FieldScript_WaitFrames(10);

  MogallForest_HopObjectEventTo(LOCALID_THIRD_AREA_WEST_APE, 328, 160,
                                PIXELS(3));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_THIRD_AREA_WEST_APE,
                              0);
  FieldScript_WaitFrames(6);

  MogallForest_HopObjectEventTo(LOCALID_THIRD_AREA_WEST_APE, 296, 160,
                                PIXELS(3));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_THIRD_AREA_WEST_APE,
                              0);
  FieldScript_WaitFrames(6);

  FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 1);
  Import_ObjectEventSetPosition(LOCALID_THIRD_AREA_WEST_APE, 0, 0);
  FieldScript_WaitFrames(30);
  GameFlag_Set(FLAG_MOGALL_FOREST_THIRD_AREA_WEST_APE_OUT);
  Import_ObjectEventSetPosition(LOCALID_THIRD_AREA_WEST_APE_STUMP, 0, 0);

  FieldScript_End();
}

/// Mogall Forest, the third area: Force on the east hiding place the first
/// time. The ape bolts to another hiding place to the south.
static void MogallForest_ThirdArea_EastApeHidesAgain(void) {
  struct ObjectEvent *creature;

  FieldScript_Start();
  MogallForest_ApeComesOut(LOCALID_THIRD_AREA_EAST_APE, SPRITE_COLOR_GRB);
  FieldCamera_MoveTo(PIXELS(744), -1, PIXELS(504), 1);
  MogallForest_HopObjectEventTo(LOCALID_THIRD_AREA_EAST_APE, 744, 504,
                                PIXELS(9));
  MogallForest_SpawnPuffRing(LOCALID_THIRD_AREA_EAST_APE);
  ObjectEvent_SetSpriteColorById(LOCALID_THIRD_AREA_EAST_APE,
                                 SPRITE_COLOR_HIDDEN);
  creature = ObjectEvent_Get(LOCALID_THIRD_AREA_EAST_APE);
  ObjectEvent_SetSpriteFlags(creature, 0);
  FieldScript_WaitFrames(30);
  GameFlag_Set(FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_MOVED);
  ObjectEvent_SetPosition(LOCALID_THIRD_AREA_EAST_APE_STUMP, PIXELS(744),
                          PIXELS(504));
  FieldScript_End();
}

/// Mogall Forest, the third area: Force on the east ape's new hiding place.
/// It points the way and hops off south.
static void MogallForest_ThirdArea_EastApeShowsWay(void) {
  struct ObjectEvent *creature;

  creature = ObjectEvent_GetById(LOCALID_THIRD_AREA_EAST_APE);

  FieldScript_Start();

  MogallForest_ApeComesOut(LOCALID_THIRD_AREA_EAST_APE, SPRITE_COLOR_GRB);
  MogallForest_HopObjectEventTo(LOCALID_THIRD_AREA_EAST_APE, 712, 536,
                                PIXELS(6));
  FieldScript_WaitFrames(10);

  FieldEffect_Spawn(creature->x.fixed, creature->y.fixed,
                    creature->z.fixed + PIXELS(4), 0, 0, 0,
                    EFFECT_SCRIPT_MEDIUM, 0);

  MogallForest_ApePointsTheWay(LOCALID_THIRD_AREA_EAST_APE);

  MogallForest_HopObjectEventTo(LOCALID_THIRD_AREA_EAST_APE, 712, 568,
                                PIXELS(6));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_THIRD_AREA_EAST_APE,
                              0);
  FieldScript_WaitFrames(10);

  MogallForest_HopObjectEventTo(LOCALID_THIRD_AREA_EAST_APE, 712, 600,
                                PIXELS(3));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_THIRD_AREA_EAST_APE,
                              0);
  FieldScript_WaitFrames(6);

  MogallForest_HopObjectEventTo(LOCALID_THIRD_AREA_EAST_APE, 736, 640,
                                PIXELS(3));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_THIRD_AREA_EAST_APE,
                              0);
  FieldScript_WaitFrames(6);

  MogallForest_HopObjectEventTo(LOCALID_THIRD_AREA_EAST_APE, 736, 704,
                                PIXELS(3));
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_THIRD_AREA_EAST_APE,
                              0);
  FieldScript_WaitFrames(6);

  FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 1);
  Import_ObjectEventSetPosition(LOCALID_THIRD_AREA_EAST_APE, 0, 0);
  FieldScript_WaitFrames(30);
  GameFlag_Set(FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT);

  FieldScript_End();
}

/// Mogall Forest, the third area: touching metatile behavior 22, once. The east
/// ape appears again, turns to the party and leads it south to a hiding place.
/// Sets FLAG_MOGALL_FOREST_LED_TO_KILLER_APE.
static void MogallForest_ApeLeadsToKillerApe(void) {
  struct ObjectEvent *creature;

  creature = ObjectEvent_GetById(LOCALID_THIRD_AREA_EAST_APE);
  FieldScript_Start();
  ObjectEvent_SetPosition(LOCALID_THIRD_AREA_EAST_APE, PIXELS(136),
                          PIXELS(360));
  MogallForest_ApeComesOut(LOCALID_THIRD_AREA_EAST_APE, SPRITE_COLOR_GRB);
  MogallForest_HopObjectEventTo(LOCALID_THIRD_AREA_EAST_APE, 136, 408,
                                PIXELS(8));
  FieldScript_WaitFrames(10);
  FieldEffect_Spawn(creature->x.fixed, creature->y.fixed,
                    (creature->z.fixed + PIXELS(4)), 0, 0, 0,
                    EFFECT_SCRIPT_MEDIUM, 0);
  ObjectEvent_FaceDirection(LOCALID_THIRD_AREA_EAST_APE, FACING_NORTH, 40);
  ObjectEvent_SetAttachedEffect(LOCALID_THIRD_AREA_EAST_APE,
                                ATTACHED_EFFECT_IN_FRONT |
                                    ATTACHED_EFFECT_ONCE);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_THIRD_AREA_EAST_APE, 2);
  FieldCamera_FollowObjectEvent(LOCALID_THIRD_AREA_EAST_APE, 1);
  MogallForest_HopObjectEventTo(LOCALID_THIRD_AREA_EAST_APE, 136, 440,
                                PIXELS(6));
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_THIRD_AREA_EAST_APE,
                              0);
  FieldScript_WaitFrames(10);
  MogallForest_HopObjectEventTo(LOCALID_THIRD_AREA_EAST_APE, 136, 472,
                                PIXELS(3));
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_THIRD_AREA_EAST_APE,
                              0);
  FieldScript_WaitFrames(6);
  MogallForest_HopObjectEventTo(LOCALID_THIRD_AREA_EAST_APE, 136, 504,
                                PIXELS(3));
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_THIRD_AREA_EAST_APE,
                              0);
  FieldScript_WaitFrames(6);
  FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 1);
  ObjectEvent_SetPosition(LOCALID_THIRD_AREA_EAST_APE, 0, 0);
  FieldScript_WaitFrames(60);
  GameFlag_Set(FLAG_MOGALL_FOREST_LED_TO_KILLER_APE);
  FieldScript_End();
}

/// Mogall Forest, the third area: touching metatile behavior 23 until the
/// Killer Ape is beaten. The hiding place shakes twice and out comes the Killer
/// Ape, the east ape's object event a fifth again as big, to fight
/// (ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, slot 1); the party comes back
/// at MOGALL_FOREST_KILLER_APE_AFTER_KILLER_APE_ATTACKS.
static void MogallForest_KillerApeAttacks(void) {
  struct Vector3 velocity;
  struct Vector3 *emissionVelocity;
  struct ObjectEvent *objectEvent;
  u32 directionIndex;
  s32 x;
  s32 z;

  objectEvent = ObjectEvent_GetById(LOCALID_THIRD_AREA_EAST_APE);
  FieldScript_Start();
  // Shake the hiding place twice before the creature appears.
  ObjectEvent_ConfigureSpriteColor(LOCALID_THIRD_AREA_EAST_APE,
                                   SPRITE_COLOR_HIDDEN);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_THIRD_AREA_EAST_APE),
                             0);
  ObjectEvent_SetPosition(LOCALID_THIRD_AREA_EAST_APE, PIXELS(136),
                          PIXELS(360));
  FieldCamera_SetSpeedParameters(PIXELS(1) / 2, PIXELS(1) / 16);
  FieldCamera_MoveTo(PIXELS(136), -1, PIXELS(392), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(60);
  MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  MogallForest_SpawnPuffRing(LOCALID_THIRD_AREA_EAST_APE);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  MapGrid_WaitForShakeToSettle();
  ObjectMotion_ArmCallback(LOCALID_PARTY_LEADER, FACING_NORTH, 20);
  FieldScript_WaitFrames(40);
  MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  MogallForest_SpawnPuffRing(LOCALID_THIRD_AREA_EAST_APE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  MapGrid_WaitForShakeToSettle();
  FieldScript_WaitFrames(40);
  MogallForest_SpawnPuffRing(LOCALID_THIRD_AREA_EAST_APE);
  objectEvent->scaleX = 6 * Q16_ONE / 5;
  objectEvent->scaleY = 6 * Q16_ONE / 5;
  ObjectEvent_ConfigureSpriteColor(LOCALID_THIRD_AREA_EAST_APE,
                                   SPRITE_COLOR_BRG);
  MogallForest_HopObjectEventTo(LOCALID_THIRD_AREA_EAST_APE, 136, 392,
                                PIXELS(15));
  FieldScript_WaitFrames(15);
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
    FieldEffect_Spawn(objectEvent->x.fixed, 0, objectEvent->z.fixed, x,
                      emissionVelocity->y, z, EFFECT_SCRIPT_MEDIUM, NULL);
  }
  FieldScript_WaitFrames(30);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  MapGrid_WaitForShakeToSettle();
  Sound_PlaySong(SE_BOSS_ROAR);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_THIRD_AREA_EAST_APE, 2);
  FieldScript_WaitFrames(20);
  // Jump toward the leader, then enter the final creature encounter.
  {
    s32 leaderX = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->x.part.pixel;

    MogallForest_HopObjectEventTo(
        LOCALID_THIRD_AREA_EAST_APE, leaderX,
        ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.part.pixel - 16,
        PIXELS(8));
  }
  FieldScript_WaitFrames(10);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  FieldScript_SetNextDestination(
      (s32)&SceneId_MogallForestKillerApe,
      MOGALL_FOREST_KILLER_APE_AFTER_KILLER_APE_ATTACKS);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 1);
  FieldScript_End();
}

const struct MapEvent *MogallForest_GetEvents(void) {
  s16 selector = gSaveBlock.sceneId;

  if (selector == (s32)&SceneId_MogallForestSign) {
    return gMogallForestSign_Events;
  }
  if (selector == (s32)&SceneId_MogallForestQuartz) {
    return gMogallForestQuartz_Events;
  }
  if (selector == (s32)&SceneId_MogallForestKillerApe) {
    return gMogallForestKillerApe_Events;
  }
  return gMogallForestSidePaths_Events;
}

/// Sets up Mogall Forest's area. In the first area, the first time, an ape
/// peeks out and hides; apes already out stay gone, rolled logs and moved
/// rocks stay put. In the third area, coming back from the Killer Ape
/// (MOGALL_FOREST_KILLER_APE_AFTER_KILLER_APE_ATTACKS) marks it beaten, and
/// entering at MOGALL_FOREST_KILLER_APE_BRIDGES_FROM_RIVER or
/// MOGALL_FOREST_KILLER_APE_FROM_WORLD_MAP once Force is learned and the Killer
/// Ape beaten sets FLAG_NYUNPA_ENLIGHTENED.
s32 MogallForest_Initialize(void) {
  struct ObjectEvent *objectEvent;
  s32 sceneId;
  s32 warpDest;
  s32 column;
  s32 fixedZ;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  sceneId = gSaveBlock.sceneId;
  if (sceneId == (s32)&SceneId_MogallForestSign) {
    // Show the first guide once, or restore the first area's completed
    // crossing.
    warpDest = gSaveBlock.warpDest;
    switch (warpDest) {
    case MOGALL_FOREST_SIGN_FROM_WORLD_MAP:
    case MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_WEST:
    case MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_NORTH:
    case MOGALL_FOREST_SIGN_ENTRANCE_FROM_SOUTHWEST_CLEARING:
      if (Script_GetFlag(FLAG_MOGALL_FOREST_FIRST_APE_SEEN) == 0) {
        FieldScript_Start();
        WaitFrames(1);
        ObjectEvent_ConfigureSpriteColor(LOCALID_FIRST_AREA_NORTH_APE,
                                         SPRITE_COLOR_GRB);
        ObjectEvent_SetPosition(LOCALID_FIRST_AREA_NORTH_APE, PIXELS(92),
                                PIXELS(120));
        ObjectEvent_FaceDirection(LOCALID_FIRST_AREA_NORTH_APE,
                                  FACING_NORTH + FACING_STEP, 0);
        ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2) / 5,
                             PIXELS(1) / 5);
        ObjectMotion_ResetAndSetPositionInMode2(LOCALID_PARTY_LEADER, 136, 64);
        FieldScript_StartScreenTransition();
        FieldScript_WaitTransitionFrames();
        ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
        FieldScript_WaitFrames(30);
        ObjectEvent_ShowEmote(LOCALID_FIRST_AREA_NORTH_APE,
                              EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
        ObjectMotion_SetVarCbAndRefresh(LOCALID_FIRST_AREA_NORTH_APE, 2);
        FieldScript_WaitFrames(30);
        MogallForest_HopObjectEventTo(LOCALID_FIRST_AREA_NORTH_APE, 136, 116,
                                      PIXELS(7));
        MogallForest_SpawnPuffRing(LOCALID_FIRST_AREA_NORTH_APE);
        ObjectEvent_ConfigureSpriteColor(LOCALID_FIRST_AREA_NORTH_APE,
                                         SPRITE_COLOR_HIDDEN);
        ObjectEvent_SetSpriteFlags(
            ObjectEvent_GetById(LOCALID_FIRST_AREA_NORTH_APE), 0);
        GameFlag_Set(FLAG_MOGALL_FOREST_FIRST_APE_SEEN);
        FieldScript_WaitFrames(60);
        FieldScript_End();
      }
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
        break;
      }
      if (GameFlag_Get(FLAG_MOGALL_FOREST_FIRST_AREA_NORTH_APE_OUT) != 0) {
        break;
      }
      ObjectEvent_ConfigureSpriteColor(LOCALID_FIRST_AREA_NORTH_APE,
                                       SPRITE_COLOR_HIDDEN);
      ObjectEvent_SetPosition(LOCALID_FIRST_AREA_NORTH_APE, PIXELS(136),
                              PIXELS(116));
      break;

    case MOGALL_FOREST_SIGN_RIVER_FROM_SOUTHWEST_CLEARING:
    case MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_EAST:
    case MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_QUARTZ:
      objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      if (objectEvent != 0) {
        Import_ObjectEventSetPosition(LOCALID_NUT, objectEvent->x.fixed,
                                      objectEvent->z.fixed);
      }
      objectEvent = ObjectEvent_GetById(LOCALID_NUT);
      objectEvent->update = 0;
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
        objectEvent = ObjectEvent_GetById(LOCALID_NUT);
        objectEvent->y.fixed = PIXELS(32);
      }
      WaitFrames(1);
      Import_ObjectEventSetPosition(LOCALID_NUT, PIXELS(632), PIXELS(440));
      if (GameFlag_Get(FLAG_MOGALL_FOREST_NUT_CAUGHT) == 0) {
        Scene_FloatNut(LOCALID_NUT);
      }
      ObjectEvent_ConfigureSpriteColor(LOCALID_FIRST_AREA_ATTACKING_APE,
                                       SPRITE_COLOR_HIDDEN);
      ObjectEvent_ConfigureSpriteColor(LOCALID_FIRST_AREA_EAST_APE,
                                       SPRITE_COLOR_HIDDEN);
      ObjectEvent_SetSpriteFlags(
          ObjectEvent_GetById(LOCALID_FIRST_AREA_ATTACKING_APE), 0);
      ObjectEvent_SetSpriteFlags(
          ObjectEvent_GetById(LOCALID_FIRST_AREA_EAST_APE), 0);
      Scene_RedrawObjectEventFootprint(LOCALID_FIRST_AREA_LOG);
      if (GameFlag_Get(FLAG_MOGALL_FOREST_FIRST_AREA_LOG_ROLLED) == 0) {
        Scene_RedrawObjectEventFootprint(LOCALID_FIRST_AREA_RIVER_LOG);
        break;
      }
      WaitFrames(1);
      ObjectEvent_SetPosition(LOCALID_FIRST_AREA_RIVER_LOG, PIXELS(528),
                              PIXELS(408));
      Import_ObjectEventSetAnimation(LOCALID_FIRST_AREA_RIVER_LOG,
                                     MOGALL_FOREST_LOG_IN_RIVER);
      MapGrid_CopyCellAttributes(38, 27, 4, 2, 31, 25);
      ObjectEvent_GetById(LOCALID_FIRST_AREA_RIVER_LOG)->priorityFlags =
          OBJ_EVENT_PRIORITY_UNDERFOOT;
      break;
    }
  } else if (sceneId == (s32)&SceneId_MogallForestQuartz) {
    // Restore revealed guides, moved rocks and the second crossing.
    warpDest = gSaveBlock.warpDest;
    switch (warpDest) {
    case MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIDE_PATHS_NORTH:
    case MOGALL_FOREST_QUARTZ_STUMPS_FROM_NORTHWEST_CLEARING:
    case MOGALL_FOREST_QUARTZ_STUMPS_FROM_POND:
    case MOGALL_FOREST_QUARTZ_STUMPS_FROM_NORTHEAST_CLEARING:
      if (GameFlag_Get(FLAG_MOGALL_FOREST_SECOND_AREA_NORTH_APE_OUT) == 0) {
        ObjectEvent_ConfigureSpriteColor(LOCALID_SECOND_AREA_NORTH_APE,
                                         SPRITE_COLOR_HIDDEN);
        ObjectEvent_SetSpriteFlags(
            ObjectEvent_GetById(LOCALID_SECOND_AREA_NORTH_APE), 0);
      }
      if (GameFlag_Get(FLAG_MOGALL_FOREST_SECOND_AREA_EAST_APE_OUT) != 0) {
        break;
      }
      ObjectEvent_ConfigureSpriteColor(LOCALID_SECOND_AREA_EAST_APE,
                                       SPRITE_COLOR_HIDDEN);
      ObjectEvent_SetSpriteFlags(
          ObjectEvent_GetById(LOCALID_SECOND_AREA_EAST_APE), 0);
      break;

    case MOGALL_FOREST_QUARTZ_RIVER_FROM_NORTHEAST_CLEARING:
    case MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_KILLER_APE:
    case MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIDE_PATHS_SOUTH:
      if (GameFlag_Get(FLAG_MOGALL_FOREST_SECOND_AREA_LOG_ROLLED) == 0) {
        Scene_RedrawObjectEventFootprint(LOCALID_SECOND_AREA_RIVER_LOG);
      } else {
#if TBS_PATCH_1
        s32 attr = 0;
#endif

        WaitFrames(1);
        ObjectEvent_SetPosition(LOCALID_SECOND_AREA_RIVER_LOG, PIXELS(552),
                                PIXELS(510));
        Import_ObjectEventSetAnimation(LOCALID_SECOND_AREA_RIVER_LOG,
                                       MOGALL_FOREST_LOG_IN_RIVER);
        ObjectEvent_GetById(LOCALID_SECOND_AREA_RIVER_LOG)->priorityFlags =
            OBJ_EVENT_PRIORITY_UNDERFOOT;
        MapGrid_CopyCellAttributes(44, 30, 2, 4, 34, 30);
#if TBS_PATCH_1
        PushedObject_FillGridAttributeRectangle(0, 35, 29, 1, 4, attr);
#endif
      }
      Scene_RedrawObjectEventFootprint(LOCALID_SECOND_AREA_LOG);
      Scene_RedrawObjectEventFootprint(LOCALID_SECOND_AREA_SOUTH_LOG);
      column = ObjectEvent_GetById(LOCALID_SECOND_AREA_ROCK)->x.fixed;
      fixedZ = ObjectEvent_GetById(LOCALID_SECOND_AREA_ROCK)->z.fixed;
      column >>= 20;
      PushedObject_FillGridAttributeRectangle(2, column, fixedZ >> 20, 1, 1,
                                              MB_WALL);
      WaitFrames(1);
      ObjectEvent_ConfigureSpriteColor(LOCALID_SECOND_AREA_ROCK,
                                       SPRITE_COLOR_GREY);
      ObjectEvent_GetById(LOCALID_SECOND_AREA_LOG)->collisionFlags |=
          OBJ_EVENT_NOT_TARGETED;
      if (GameFlag_Get(FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_OUT) != 0) {
        break;
      }
      ObjectEvent_ConfigureSpriteColor(LOCALID_SECOND_AREA_SOUTH_APE,
                                       SPRITE_COLOR_HIDDEN);
      ObjectEvent_SetSpriteFlags(
          ObjectEvent_GetById(LOCALID_SECOND_AREA_SOUTH_APE), 0);
      if (GameFlag_Get(FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_MOVED) == 0) {
        break;
      }
      ObjectEvent_SetPosition(LOCALID_SECOND_AREA_SOUTH_APE, PIXELS(424),
                              PIXELS(480));
      ObjectEvent_SetPosition(LOCALID_SECOND_AREA_SOUTH_APE_STUMP, PIXELS(424),
                              PIXELS(480));
      break;
    }
  } else if (sceneId == (s32)&SceneId_MogallForestKillerApe) {
    // Restore both crossing logs and the creatures leading to the final
    // encounter.
    warpDest = gSaveBlock.warpDest;
    switch (warpDest) {
    case MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_WEST:
    case MOGALL_FOREST_KILLER_APE_STUMPS_FROM_CHEST:
    case MOGALL_FOREST_KILLER_APE_STUMPS_FROM_NORTHWEST_CLEARING:
    case MOGALL_FOREST_KILLER_APE_STUMPS_FROM_RIVER:
      WaitFrames(1);
      if (GameFlag_Get(FLAG_MOGALL_FOREST_THIRD_AREA_ATTACKING_APE_OUT) == 0) {
        ObjectEvent_ConfigureSpriteColor(LOCALID_THIRD_AREA_ATTACKING_APE,
                                         SPRITE_COLOR_HIDDEN);
        ObjectEvent_SetSpriteFlags(
            ObjectEvent_GetById(LOCALID_THIRD_AREA_ATTACKING_APE), 0);
        ObjectEvent_SetSpriteFlags(
            ObjectEvent_GetById(LOCALID_THIRD_AREA_ATTACKING_APE_STUMP), 0);
      }
      if (GameFlag_Get(FLAG_MOGALL_FOREST_THIRD_AREA_MIDDLE_APE_OUT) == 0) {
        ObjectEvent_ConfigureSpriteColor(LOCALID_THIRD_AREA_MIDDLE_APE,
                                         SPRITE_COLOR_HIDDEN);
        ObjectEvent_SetSpriteFlags(
            ObjectEvent_GetById(LOCALID_THIRD_AREA_MIDDLE_APE), 0);
        ObjectEvent_SetSpriteFlags(
            ObjectEvent_GetById(LOCALID_THIRD_AREA_MIDDLE_APE_STUMP), 0);
      }
      if (GameFlag_Get(FLAG_MOGALL_FOREST_THIRD_AREA_WEST_APE_OUT) != 0) {
        break;
      }
      ObjectEvent_ConfigureSpriteColor(LOCALID_THIRD_AREA_WEST_APE,
                                       SPRITE_COLOR_HIDDEN);
      ObjectEvent_SetSpriteFlags(
          ObjectEvent_GetById(LOCALID_THIRD_AREA_WEST_APE), 0);
      ObjectEvent_SetSpriteFlags(
          ObjectEvent_GetById(LOCALID_THIRD_AREA_WEST_APE_STUMP), 0);
      break;

    case MOGALL_FOREST_KILLER_APE_CHEST_FROM_STUMPS:
      column = ObjectEvent_GetById(LOCALID_THIRD_AREA_NORTH_ROCK)->x.fixed;
      fixedZ = ObjectEvent_GetById(LOCALID_THIRD_AREA_NORTH_ROCK)->z.fixed;
      column >>= 20;
      PushedObject_FillGridAttributeRectangle(2, column, fixedZ >> 20, 1, 1,
                                              MB_WALL);
      ObjectEvent_ConfigureSpriteColor(LOCALID_THIRD_AREA_NORTH_ROCK,
                                       SPRITE_COLOR_GREY);
      WaitFrames(1);
      ObjectEvent_GetById(LOCALID_THIRD_AREA_LOG)->collisionFlags |=
          OBJ_EVENT_NOT_TARGETED;
      Scene_RedrawObjectEventFootprint(LOCALID_THIRD_AREA_LOG);
      break;

    case MOGALL_FOREST_KILLER_APE_RIVER_FROM_STUMPS:
    case MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_EAST:
    case MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_SOUTH:
    case MOGALL_FOREST_KILLER_APE_RIVER_FROM_BRIDGES:
      // XXX (no evidence): what these two crates are for; they are hidden, not
      // solid and never a target, so the player neither sees nor meets them.
      ObjectEvent_CreateConfiguredObject(PIXELS(734), 0, PIXELS(370),
                                         SPRITE_CRATE);
      ObjectEvent_CreateConfiguredObject(PIXELS(754), 0, PIXELS(370),
                                         SPRITE_CRATE);
      Scene_RedrawObjectEventFootprint(LOCALID_THIRD_AREA_WEST_LOG);
      Scene_RedrawObjectEventFootprint(LOCALID_THIRD_AREA_SOUTHWEST_LOG);
      if (GameFlag_Get(FLAG_MOGALL_FOREST_THIRD_AREA_NORTH_LOG_ROLLED) == 0) {
        Scene_RedrawObjectEventFootprint(LOCALID_THIRD_AREA_NORTH_RIVER_LOG);
      } else {
        WaitFrames(1);
        Import_ObjectEventSetAnimation(LOCALID_THIRD_AREA_NORTH_RIVER_LOG,
                                       MOGALL_FOREST_LOG_IN_RIVER);
        Import_ObjectEventSetPosition(LOCALID_THIRD_AREA_NORTH_RIVER_LOG,
                                      PIXELS(698), PIXELS(398));
        ObjectEvent_GetById(LOCALID_THIRD_AREA_NORTH_RIVER_LOG)
            ->priorityFlags |= OBJ_EVENT_PRIORITY_UNDERFOOT;
        MapGrid_CopyCellAttributes(26, 20, 2, 4, 42, 23);
        GameFlag_Set(FLAG_MOGALL_FOREST_SOUTH_ROCK_SETTLED);
        ObjectEvent_SetPosition(LOCALID_THIRD_AREA_SOUTH_ROCK, PIXELS(632),
                                PIXELS(440));
        ObjectEvent_SetSpriteFlags(
            ObjectEvent_GetById(LOCALID_THIRD_AREA_SOUTH_ROCK), 0);
      }
      if (GameFlag_Get(FLAG_MOGALL_FOREST_THIRD_AREA_SOUTH_LOG_ROLLED) == 0) {
        Scene_RedrawObjectEventFootprint(LOCALID_THIRD_AREA_SOUTH_RIVER_LOG);
      } else {
        WaitFrames(1);
        Import_ObjectEventSetAnimation(LOCALID_THIRD_AREA_SOUTH_RIVER_LOG,
                                       MOGALL_FOREST_LOG_IN_RIVER);
        Import_ObjectEventSetPosition(LOCALID_THIRD_AREA_SOUTH_RIVER_LOG,
                                      PIXELS(666), PIXELS(550));
        ObjectEvent_GetById(LOCALID_THIRD_AREA_SOUTH_RIVER_LOG)->priorityFlags =
            OBJ_EVENT_PRIORITY_UNDERFOOT;
        MapGrid_CopyCellAttributes(26, 20, 2, 4, 40, 32);
      }
      column = ObjectEvent_GetById(LOCALID_THIRD_AREA_SOUTH_ROCK)->x.fixed;
      fixedZ = ObjectEvent_GetById(LOCALID_THIRD_AREA_SOUTH_ROCK)->z.fixed;
      column >>= 20;
      PushedObject_FillGridAttributeRectangle(2, column, fixedZ >> 20, 1, 1,
                                              MB_WALL);
      ObjectEvent_ConfigureSpriteColor(LOCALID_THIRD_AREA_SOUTH_ROCK,
                                       SPRITE_COLOR_GREY);
      WaitFrames(1);
      ObjectEvent_GetById(LOCALID_THIRD_AREA_NORTH_RIVER_LOG)->collisionFlags |=
          OBJ_EVENT_NOT_TARGETED;
      if (GameFlag_Get(FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT) == 0) {
        ObjectEvent_ConfigureSpriteColor(LOCALID_THIRD_AREA_EAST_APE,
                                         SPRITE_COLOR_HIDDEN);
        ObjectEvent_SetSpriteFlags(
            ObjectEvent_GetById(LOCALID_THIRD_AREA_EAST_APE), 0);
        if (GameFlag_Get(FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_MOVED) != 0) {
          ObjectEvent_SetPosition(LOCALID_THIRD_AREA_EAST_APE_STUMP,
                                  PIXELS(744), PIXELS(504));
          ObjectEvent_SetPosition(LOCALID_THIRD_AREA_EAST_APE, PIXELS(744),
                                  PIXELS(504));
        }
#if TBS_PATCH_2
      } else {
        Import_ObjectEventSetPosition(LOCALID_THIRD_AREA_EAST_APE, 0, 0);
#endif
      }
      MogallForest_ThirdArea_UpdateSouthRock();
      break;

    case MOGALL_FOREST_KILLER_APE_BRIDGES_FROM_RIVER:
    case MOGALL_FOREST_KILLER_APE_FROM_WORLD_MAP:
      ObjectEvent_SetActionScript(LOCALID_THIRD_AREA_EAST_APE,
                                  gMogallForest_ObjectEventScript);
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

#include "common/object_orbit.h"

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))

s32 gPushedObject_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gPushedObject_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedObject_FootprintBounds[] = {
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
    SC_FREE,
};

s32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

s32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
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
s32 gMogallForest_ObjectEventScript[] = {
    SC_SET(SW_SCALE_X, 4 * Q16_ONE / 5),
    SC_SET(SW_SCALE_Y, 4 * Q16_ONE / 5),
    SC_SET(SW_TIMER, SPRITE_COLOR_HIDDEN),
    SC_CALL(MogallForest_CopyTimerToChild),
    SC_CALL(MogallForest_ClearObjectEventSpriteFlags),
    SC_END,
};

/// The script of each invisible hiding place: still, and its sprite hidden.
s32 gMogallForest_HidingPlaceScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_Y, 0),
    SC_SET(SW_TARGET_Y, PIXELS(-32768)),
    SC_CALL(MogallForest_ClearObjectEventSpriteFlags),
    SC_WAIT(2),
    SC_END,
};

struct MapWarpDest gMogallForestSign_WarpDests[] = {
    { MOGALL_FOREST_SIGN_FROM_WORLD_MAP, CONDITION_ALWAYS, 136, 0, 24, FACING_SOUTH, 0, 0, 0, 272, 224, 0 },
    { MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_WEST, CONDITION_ALWAYS, 24, 0, 128, FACING_EAST, 0, 0, 0, 272, 224, 0 },
    { MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_NORTH, CONDITION_ALWAYS, 248, 0, 128, FACING_WEST, 0, 0, 0, 272, 224, 0 },
    { MOGALL_FOREST_SIGN_ENTRANCE_FROM_SOUTHWEST_CLEARING, CONDITION_ALWAYS, 136, 0, 200, FACING_NORTH, 0, 0, 0, 272, 224, 0 },
    { MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_FROM_ENTRANCE, CONDITION_ALWAYS, 72, 0, 264, FACING_SOUTH, 0, 0, 240, 272, 464, 0 },
    { MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_FROM_RIVER, CONDITION_ALWAYS, 200, 0, 440, FACING_NORTH, 0, 0, 240, 272, 464, 0 },
    { MOGALL_FOREST_SIGN_RIVER_FROM_SOUTHWEST_CLEARING, CONDITION_ALWAYS, 416, 0, 264, FACING_SOUTH, 0, 288, 240, 672, 624, 0 },
    { MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_EAST, CONDITION_ALWAYS, 560, 0, 264, FACING_SOUTH, 0, 288, 240, 672, 624, 0 },
    { MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_QUARTZ, CONDITION_ALWAYS, 648, 0, 344, FACING_WEST, 0, 288, 240, 672, 624, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gMogallForestQuartz_WarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 120, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
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
    { MAP_TABLE_END },
};

struct MapWarpDest gMogallForestKillerApe_WarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 172, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
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
    { MAP_TABLE_END },
};

struct MapWarpDest gMogallForestSidePaths_WarpDests[] = {
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
    { MAP_TABLE_END },
};

u32 gMogallForest_Warps[] = {
    MAP_WARPS(MogallForestSign),
    MAP_WARP(MOGALL_FOREST_SIGN_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_MOGALL_FOREST_SIGN),
    MAP_WARP(MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_SIDE_PATHS_WEST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_WEST),
    MAP_WARP(MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_SIDE_PATHS_NORTH, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_NORTH),
    MAP_WARP(MOGALL_FOREST_SIGN_ENTRANCE_TO_SOUTHWEST_CLEARING, MogallForestSign, MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_FROM_ENTRANCE),
    MAP_WARP(MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_TO_ENTRANCE, MogallForestSign, MOGALL_FOREST_SIGN_ENTRANCE_FROM_SOUTHWEST_CLEARING),
    MAP_WARP(MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_TO_RIVER, MogallForestSign, MOGALL_FOREST_SIGN_RIVER_FROM_SOUTHWEST_CLEARING),
    MAP_WARP(MOGALL_FOREST_SIGN_RIVER_TO_SOUTHWEST_CLEARING, MogallForestSign, MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_FROM_RIVER),
    MAP_WARP(MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_SIDE_PATHS_EAST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_EAST),
    MAP_WARP(MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_QUARTZ, MogallForestQuartz, MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIGN),
    MAP_WARPS(MogallForestQuartz),
    MAP_WARP(MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_SIGN, MogallForestSign, MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_QUARTZ),
    MAP_WARP(MOGALL_FOREST_QUARTZ_NORTHWEST_CLEARING_TO_STUMPS, MogallForestQuartz, MOGALL_FOREST_QUARTZ_STUMPS_FROM_NORTHWEST_CLEARING),
    MAP_WARP(MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_SIDE_PATHS_NORTH, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_QUARTZ_NORTH),
    MAP_WARP(MOGALL_FOREST_QUARTZ_STUMPS_TO_NORTHWEST_CLEARING, MogallForestQuartz, MOGALL_FOREST_QUARTZ_NORTHWEST_CLEARING_FROM_STUMPS),
    MAP_WARP(MOGALL_FOREST_QUARTZ_STUMPS_TO_POND, MogallForestQuartz, MOGALL_FOREST_QUARTZ_POND_FROM_STUMPS),
    MAP_WARP(MOGALL_FOREST_QUARTZ_STUMPS_TO_NORTHEAST_CLEARING, MogallForestQuartz, MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_FROM_STUMPS),
    MAP_WARP(MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_TO_STUMPS, MogallForestQuartz, MOGALL_FOREST_QUARTZ_STUMPS_FROM_NORTHEAST_CLEARING),
    MAP_WARP(MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_TO_RIVER, MogallForestQuartz, MOGALL_FOREST_QUARTZ_RIVER_FROM_NORTHEAST_CLEARING),
    MAP_WARP(MOGALL_FOREST_QUARTZ_POND_TO_STUMPS, MogallForestQuartz, MOGALL_FOREST_QUARTZ_STUMPS_FROM_POND),
    MAP_WARP(MOGALL_FOREST_QUARTZ_RIVER_TO_NORTHEAST_CLEARING, MogallForestQuartz, MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_FROM_RIVER),
    MAP_WARP(MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_KILLER_APE, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_QUARTZ),
    MAP_WARP(MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_SIDE_PATHS_SOUTH, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_QUARTZ_SOUTH),
    MAP_WARPS(MogallForestKillerApe),
    MAP_WARP(MOGALL_FOREST_KILLER_APE_NORTHWEST_CLEARING_TO_STUMPS, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_STUMPS_FROM_NORTHWEST_CLEARING),
    MAP_WARP(MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_QUARTZ, MogallForestQuartz, MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_KILLER_APE),
    MAP_WARP(MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_SIDE_PATHS_WEST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_WEST),
    MAP_WARP(MOGALL_FOREST_KILLER_APE_STUMPS_TO_CHEST, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_CHEST_FROM_STUMPS),
    MAP_WARP(MOGALL_FOREST_KILLER_APE_STUMPS_TO_NORTHWEST_CLEARING, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_NORTHWEST_CLEARING_FROM_STUMPS),
    MAP_WARP(MOGALL_FOREST_KILLER_APE_STUMPS_TO_RIVER, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_RIVER_FROM_STUMPS),
    MAP_WARP(MOGALL_FOREST_KILLER_APE_CHEST_TO_STUMPS, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_STUMPS_FROM_CHEST),
    MAP_WARP(MOGALL_FOREST_KILLER_APE_RIVER_TO_STUMPS, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_STUMPS_FROM_RIVER),
    MAP_WARP(MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_SIDE_PATHS_EAST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_EAST),
    MAP_WARP(MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_SIDE_PATHS_SOUTH, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_SOUTH),
    MAP_WARP(MOGALL_FOREST_KILLER_APE_RIVER_TO_BRIDGES, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_BRIDGES_FROM_RIVER),
    MAP_WARP(MOGALL_FOREST_KILLER_APE_BRIDGES_TO_RIVER, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_RIVER_FROM_BRIDGES),
    MAP_WARP(MOGALL_FOREST_KILLER_APE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_MOGALL_FOREST_KILLER_APE),
    MAP_WARPS(MogallForestSidePaths),
    MAP_WARP(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_SIGN_NORTH, MogallForestSign, MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_NORTH),
    MAP_WARP(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_SIGN_WEST, MogallForestSign, MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_WEST),
    MAP_WARP(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_QUARTZ_NORTH, MogallForestQuartz, MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIDE_PATHS_NORTH),
    MAP_WARP(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_QUARTZ_SOUTH, MogallForestQuartz, MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIDE_PATHS_SOUTH),
    MAP_WARP(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_KILLER_APE_WEST, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_WEST),
    MAP_WARP(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_KILLER_APE_EAST, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_EAST),
    MAP_WARP(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_KILLER_APE_SOUTH, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_SOUTH),
    MAP_WARP(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_SIGN_EAST, MogallForestSign, MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_EAST),
    MAP_WARP(MOGALL_FOREST_SIDE_PATHS_TO_NORTH_CLEARING_WEST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_NORTH),
    MAP_WARP(MOGALL_FOREST_SIDE_PATHS_TO_WEST_CLEARING_EAST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_WEST),
    MAP_WARP(MOGALL_FOREST_SIDE_PATHS_TO_CENTER_CLEARING, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_QUARTZ_NORTH),
    MAP_WARP(MOGALL_FOREST_SIDE_PATHS_TO_EAST_CLEARING, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_QUARTZ_SOUTH),
    MAP_WARP(MOGALL_FOREST_SIDE_PATHS_TO_SOUTHWEST_CLEARING, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_WEST),
    MAP_WARP(MOGALL_FOREST_SIDE_PATHS_TO_SOUTH_CLEARING_NORTHWEST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_EAST),
    MAP_WARP(MOGALL_FOREST_SIDE_PATHS_TO_SOUTH_CLEARING_SOUTHWEST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_SOUTH),
    MAP_WARP(MOGALL_FOREST_SIDE_PATHS_TO_SOUTHEAST_CLEARING, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_EAST),
    MAP_WARP(MOGALL_FOREST_SIDE_PATHS_TO_NORTH_CLEARING_EAST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_NORTH_CLEARING_EAST),
    MAP_WARP(MOGALL_FOREST_SIDE_PATHS_TO_WEST_CLEARING_WEST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_WEST_CLEARING_WEST),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gMogallForestSign_ObjectEventTemplates[] = {
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(488), 0, PIXELS(464), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        PIXELS(528), 0, PIXELS(504), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_APE, FLAG_MOGALL_FOREST_FIRST_AREA_NORTH_APE_OUT, (s32)gMogallForest_ObjectEventScript,
        PIXELS(136), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_APE, FLAG_MOGALL_FOREST_FIRST_AREA_ATTACKING_APE_OUT, (s32)gMogallForest_ObjectEventScript,
        PIXELS(376), 0, PIXELS(432), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_APE, FLAG_MOGALL_FOREST_FIRST_AREA_EAST_APE_OUT, (s32)gMogallForest_ObjectEventScript,
        PIXELS(504), 0, PIXELS(312), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, FLAG_MOGALL_FOREST_FIRST_AREA_NORTH_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        PIXELS(136), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, FLAG_MOGALL_FOREST_FIRST_AREA_ATTACKING_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        PIXELS(376), 0, PIXELS(432), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLANK, FLAG_MOGALL_FOREST_FIRST_AREA_EAST_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        PIXELS(504), 0, PIXELS(312), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLANK, FLAG_MOGALL_FOREST_NUT_CAUGHT, 7,
        PIXELS(632), 0, PIXELS(440), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gMogallForestQuartz_ObjectEventTemplates[] = {
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        PIXELS(592), 0, PIXELS(360), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        PIXELS(592), 0, PIXELS(456), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(648), 0, PIXELS(512), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_TURNING_ROCK, CONDITION_ALWAYS, (s32)gMogallForest_RockScript,
        PIXELS(584), 0, PIXELS(376), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_APE, FLAG_MOGALL_FOREST_SECOND_AREA_NORTH_APE_OUT, (s32)gMogallForest_ObjectEventScript,
        PIXELS(360), 0, PIXELS(80), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_APE, FLAG_MOGALL_FOREST_SECOND_AREA_EAST_APE_OUT, (s32)gMogallForest_ObjectEventScript,
        PIXELS(488), 0, PIXELS(80), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_APE, FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_OUT, (s32)gMogallForest_ObjectEventScript,
        PIXELS(504), 0, PIXELS(480), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, FLAG_MOGALL_FOREST_SECOND_AREA_NORTH_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        PIXELS(360), 0, PIXELS(80), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, FLAG_MOGALL_FOREST_SECOND_AREA_EAST_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        PIXELS(488), 0, PIXELS(80), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLANK, FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        PIXELS(504), 0, PIXELS(480), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { VENUS_DJINNI, FLAG_DJINNI(DJINNI_QUARTZ), MOVEMENT_TYPE_STAND,
        PIXELS(488), 0, PIXELS(568), FACING_EAST, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gMogallForestKillerApe_ObjectEventTemplates[] = {
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
        PIXELS(632), 0, PIXELS(408), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_APE, FLAG_MOGALL_FOREST_THIRD_AREA_ATTACKING_APE_OUT, (s32)gMogallForest_ObjectEventScript,
        PIXELS(488), 0, PIXELS(88), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_APE, FLAG_MOGALL_FOREST_THIRD_AREA_MIDDLE_APE_OUT, (s32)gMogallForest_ObjectEventScript,
        PIXELS(424), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_APE, FLAG_MOGALL_FOREST_THIRD_AREA_WEST_APE_OUT, (s32)gMogallForest_ObjectEventScript,
        PIXELS(360), 0, PIXELS(80), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_APE, MOGALL_FOREST_EAST_OBJ_EVENT_CONDITION, (s32)gMogallForest_ObjectEventScript,
        PIXELS(744), 0, PIXELS(400), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, FLAG_MOGALL_FOREST_THIRD_AREA_ATTACKING_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        PIXELS(488), 0, PIXELS(88), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, FLAG_MOGALL_FOREST_THIRD_AREA_MIDDLE_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        PIXELS(424), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLANK, FLAG_MOGALL_FOREST_THIRD_AREA_WEST_APE_OUT, (s32)gMogallForest_HidingPlaceScript,
        PIXELS(360), 0, PIXELS(80), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLANK, MOGALL_FOREST_EAST_SHARED_SPRITE_CONDITION, (s32)gMogallForest_HidingPlaceScript,
        PIXELS(744), 0, PIXELS(400), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gMogallForestSidePaths_ObjectEventTemplates[] = {
    { MAP_TABLE_END },
};

struct MapEvent gMogallForestSign_Events[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, MOGALL_FOREST_SIGN_TO_WORLD_MAP },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_SIDE_PATHS_WEST },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_SIDE_PATHS_NORTH },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, MOGALL_FOREST_SIGN_ENTRANCE_TO_SOUTHWEST_CLEARING },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_TO_ENTRANCE },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_TO_RIVER },
    { MAP_EVENT_WARP, 7, CONDITION_ALWAYS, MOGALL_FOREST_SIGN_RIVER_TO_SOUTHWEST_CLEARING },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_SIDE_PATHS_EAST },
    { MAP_EVENT_WARP, 9, CONDITION_ALWAYS, MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_QUARTZ },
    { COORD_EVENT_WALKING, 20, CONDITION_ALWAYS, EVENT_SCRIPT(MogallForest_FirstArea_PushLog) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 25, CONDITION_ALWAYS, EVENT_SCRIPT(MogallForest_FirstArea_JumpNorth) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 26, CONDITION_ALWAYS, EVENT_SCRIPT(MogallForest_FirstArea_JumpSouth) },
    { BG_EVENT(BG_EVENT_SILENT), 27, CONDITION_ALWAYS, EVENT_SCRIPT(MogallForest_ReadBrokenSign) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FORCE), LOCALID_FIRST_AREA_NORTH_APE_STUMP, FLAG_MOGALL_FOREST_FIRST_AREA_NORTH_APE_OUT, EVENT_SCRIPT(MogallForest_FirstArea_NorthApeShowsWay) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FORCE), LOCALID_FIRST_AREA_ATTACKING_APE_STUMP, FLAG_MOGALL_FOREST_FIRST_AREA_ATTACKING_APE_OUT, EVENT_SCRIPT(MogallForest_FirstArea_ApeAttacks) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FORCE), LOCALID_FIRST_AREA_EAST_APE_STUMP, FLAG_MOGALL_FOREST_FIRST_AREA_EAST_APE_OUT, EVENT_SCRIPT(MogallForest_FirstArea_EastApeShowsWay) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_CATCH), LOCALID_NUT, FLAG_MOGALL_FOREST_NUT_CAUGHT, EVENT_SCRIPT(MogallForest_CatchNut) },
    { MAP_EVENTS_END },
};

struct MapEvent gMogallForestQuartz_Events[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_SIGN },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_NORTHWEST_CLEARING_TO_STUMPS },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_SIDE_PATHS_NORTH },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_STUMPS_TO_NORTHWEST_CLEARING },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_STUMPS_TO_POND },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_STUMPS_TO_NORTHEAST_CLEARING },
    { MAP_EVENT_WARP, 7, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_TO_STUMPS },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_TO_RIVER },
    { MAP_EVENT_WARP, 9, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_POND_TO_STUMPS },
    { MAP_EVENT_WARP, 10, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_RIVER_TO_NORTHEAST_CLEARING },
    { MAP_EVENT_WARP, 11, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_KILLER_APE },
    { MAP_EVENT_WARP, 12, CONDITION_ALWAYS, MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_SIDE_PATHS_SOUTH },
    { COORD_EVENT_WALKING, 20, CONDITION_ALWAYS, EVENT_SCRIPT(MogallForest_SecondArea_PushLog) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_SECOND_AREA_ROCK, CONDITION_ALWAYS, EVENT_SCRIPT(MogallForest_SecondArea_UpdateRock) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FORCE), LOCALID_SECOND_AREA_NORTH_APE_STUMP, FLAG_MOGALL_FOREST_SECOND_AREA_NORTH_APE_OUT, EVENT_SCRIPT(MogallForest_SecondArea_NorthApeShowsWay) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FORCE), LOCALID_SECOND_AREA_EAST_APE_STUMP, FLAG_MOGALL_FOREST_SECOND_AREA_EAST_APE_OUT, EVENT_SCRIPT(MogallForest_SecondArea_EastApeShowsWay) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FORCE), LOCALID_SECOND_AREA_SOUTH_APE_STUMP, FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_MOVED, EVENT_SCRIPT(MogallForest_SecondArea_SouthApeHidesAgain) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FORCE), LOCALID_SECOND_AREA_SOUTH_APE_STUMP, FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_OUT, EVENT_SCRIPT(MogallForest_SecondArea_SouthApeShowsWay) },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_MOGALL_FOREST_APPLE_CHEST, FIND_ITEM(ITEM_APPLE) },
    { MAP_EVENT_TALK, LOCALID_SECOND_AREA_QUARTZ, CONDITION_ALWAYS, EVENT_SCRIPT(MogallForest_StartDjinnBattle) },
    { MAP_EVENTS_END },
};
// clang-format on

struct MapEvent gMogallForestKillerApe_Events[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_NORTHWEST_CLEARING_TO_STUMPS},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_QUARTZ},
    {MAP_EVENT_WARP, 3, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_SIDE_PATHS_WEST},
    {MAP_EVENT_WARP, 4, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_STUMPS_TO_CHEST},
    {MAP_EVENT_WARP, 5, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_STUMPS_TO_NORTHWEST_CLEARING},
    {MAP_EVENT_WARP, 6, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_STUMPS_TO_RIVER},
    {MAP_EVENT_WARP, 7, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_CHEST_TO_STUMPS},
    {MAP_EVENT_WARP, 8, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_RIVER_TO_STUMPS},
    {MAP_EVENT_WARP, 9, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_SIDE_PATHS_EAST},
    {MAP_EVENT_WARP, 10, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_SIDE_PATHS_SOUTH},
    {MAP_EVENT_WARP, 11, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_RIVER_TO_BRIDGES},
    {MAP_EVENT_WARP, 12, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_BRIDGES_TO_RIVER},
    {MAP_EVENT_WARP, 13, CONDITION_ALWAYS,
     MOGALL_FOREST_KILLER_APE_TO_WORLD_MAP},
    {COORD_EVENT_WALKING, 20, CONDITION_ALWAYS,
     EVENT_SCRIPT(MogallForest_ThirdArea_PushLog)},
    {COORD_EVENT_WALKING_INTO(FACING_WEST), 25, CONDITION_ALWAYS,
     EVENT_SCRIPT(MogallForest_ThirdArea_JumpWest)},
    {MAP_EVENT_COORD, 22, FLAG_MOGALL_FOREST_LED_TO_KILLER_APE,
     EVENT_SCRIPT(MogallForest_ApeLeadsToKillerApe)},
    {MAP_EVENT_COORD, 23, FLAG_MOGALL_FOREST_KILLER_APE_BEATEN,
     EVENT_SCRIPT(MogallForest_KillerApeAttacks)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_THIRD_AREA_NORTH_ROCK,
     CONDITION_ALWAYS, EVENT_SCRIPT(MogallForest_ThirdArea_UpdateNorthRock)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_THIRD_AREA_SOUTH_ROCK,
     FLAG_MOGALL_FOREST_SOUTH_ROCK_SETTLED,
     EVENT_SCRIPT(MogallForest_ThirdArea_UpdateSouthRock)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_FORCE),
     LOCALID_THIRD_AREA_ATTACKING_APE_STUMP,
     FLAG_MOGALL_FOREST_THIRD_AREA_ATTACKING_APE_OUT,
     EVENT_SCRIPT(MogallForest_ThirdArea_ApeAttacks)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_FORCE), LOCALID_THIRD_AREA_MIDDLE_APE_STUMP,
     FLAG_MOGALL_FOREST_THIRD_AREA_MIDDLE_APE_OUT,
     EVENT_SCRIPT(MogallForest_ThirdArea_MiddleApeShowsWay)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_FORCE), LOCALID_THIRD_AREA_WEST_APE_STUMP,
     FLAG_MOGALL_FOREST_THIRD_AREA_WEST_APE_OUT,
     EVENT_SCRIPT(MogallForest_ThirdArea_WestApeShowsWay)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_FORCE), LOCALID_THIRD_AREA_EAST_APE_STUMP,
     FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_MOVED,
     EVENT_SCRIPT(MogallForest_ThirdArea_EastApeHidesAgain)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_FORCE), LOCALID_THIRD_AREA_EAST_APE_STUMP,
     FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT,
     EVENT_SCRIPT(MogallForest_ThirdArea_EastApeShowsWay)},
    {BG_EVENT(BG_EVENT_CHEST), 100, FLAG_MOGALL_FOREST_ELVEN_SHIRT_CHEST,
     FIND_ITEM(ITEM_ELVEN_SHIRT)},
    {MAP_EVENTS_END},
};

struct MapEvent gMogallForestSidePaths_Events[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_SIGN_NORTH},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_SIGN_WEST},
    {MAP_EVENT_WARP, 3, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_QUARTZ_NORTH},
    {MAP_EVENT_WARP, 4, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_QUARTZ_SOUTH},
    {MAP_EVENT_WARP, 5, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_KILLER_APE_WEST},
    {MAP_EVENT_WARP, 6, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_KILLER_APE_EAST},
    {MAP_EVENT_WARP, 7, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_KILLER_APE_SOUTH},
    {MAP_EVENT_WARP, 8, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_SIGN_EAST},
    {MAP_EVENT_WARP, 11, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_WEST_CLEARING_WEST},
    {MAP_EVENT_WARP, 12, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_NORTH_CLEARING_EAST},
    {MAP_EVENT_WARP, 13, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_CENTER_CLEARING},
    {MAP_EVENT_WARP, 14, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_EAST_CLEARING},
    {MAP_EVENT_WARP, 15, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_SOUTHWEST_CLEARING},
    {MAP_EVENT_WARP, 16, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_SOUTH_CLEARING_NORTHWEST},
    {MAP_EVENT_WARP, 17, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_SOUTH_CLEARING_SOUTHWEST},
    {MAP_EVENT_WARP, 18, CONDITION_ALWAYS,
     MOGALL_FOREST_SIDE_PATHS_TO_SOUTHEAST_CLEARING},
    {BG_EVENT(BG_EVENT_SILENT), 100, FLAG_MOGALL_FOREST_PSYNERGY_STONE,
     FIND_PSYNERGY_STONE},
    {MAP_EVENTS_END},
};
