/// @file
/// Vale Cave's overlay, which holds three rooms, each a map of its own: the way
/// in, which the game still calls Vale, and two rooms of Vale Cave proper.
/// Kite, a Jupiter Djinni, hides in the second. No story flag is read here and
/// no message is shown: the cave is a side trip from the part of Vale around
/// Kraden's cottage, a run of Psynergy puzzles whose progress lives in map and
/// area flags.
///
/// - Vale (SceneId_ValeCaveMouth), the way in, an area of its own (29).
///   VALE_CAVE_MOUTH_TO_VALE_KRADENS_COTTAGE_WEST and
///   VALE_CAVE_MOUTH_TO_VALE_KRADENS_COTTAGE_EAST lead back to Kraden's part of
///   Vale (SceneId_ValeKradensCottage), whose
///   VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_WEST and
///   VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_EAST lead here;
///   VALE_CAVE_MOUTH_TO_VALE_CAVE_KITE goes on into the cave. Each exit plays
///   the exit sound first (EventScript_LeaveRoom). On arrival
///   ValeCave_Initialize fades the room in from the backdrop and clears
///   FLAG_SHOW_LOCATION_NAME: entering a new area sets that flag, but
///   this is still Vale.
/// - Vale Cave (SceneId_ValeCaveKite), one map in three parts, each with its
/// own
///   entrances and camera box; walking south into a ledge's cells hops
///   the leader down it (EventScript_HopDownLedge). In the first part
///   (VALE_CAVE_KITE_FROM_VALE_CAVE_MOUTH to
///   VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_WEST) Reveal shows a hidden cell while
///   it lasts (EventScript_RevealHiddenTile). In Kite's part
///   (VALE_CAVE_KITE_EAST_FROM_WEST to
///   VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_EAST), coming near makes Kite hop away
///   (EventScript_KiteRunsOff) and then run behind a ledge
///   (EventScript_KiteRunsBehindLedge, setting
///   FLAG_VALE_CAVE_KITE_BEHIND_LEDGE); from then on twelve metatile behaviors,
///   45 to 56 on the 23 cells round the ledge, each send it to its point of
///   gValeCave_WalkTargets on the ledge's far side as the leader walks round
///   (EventScript_KiteKeepsAway), so it cannot be reached until Halt stops it.
///   A stopped Kite can be talked to
///   and joins the party (EventScript_TalkToKite), setting FLAG_KITE, which
///   removes it from the actor templates. The room's chests hold the
///   Halt Gem, which gives Halt, and a Nut. In the holes' part
///   (VALE_CAVE_KITE_SOUTH_FROM_WEST to
///   VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_SOUTH) two stacks of crates sink into
///   holes in the floor once Move or a push leaves them over one
///   (FLAG_VALE_CAVE_WEST_CRATES_SUNK and FLAG_VALE_CAVE_EAST_CRATES_SUNK),
///   opening the way, and a log carries the leader along the passage when
///   walked into from the west or the east (FLAG_VALE_CAVE_RIDING_LOG_EAST).
///   Its exits lead back to the first room, on to the third, and between its
///   own parts.
/// - Vale Cave (SceneId_ValeCaveLogs), the inner room, with three logs to push:
///   one off a ledge (EventScript_PushLogOffLedge), one along the ledge until
///   it drops at its edge (EventScript_PushLogOverEdge) and one north
///   (EventScript_PushLogNorth), each clearing the cells it held. Its exits
///   lead back into the second room and between its own parts.
///
/// Vale Cave's two rooms are one area (30), so their area flags
/// (FLAG_VALE_CAVE_RIDING_LOG_EAST, FLAG_VALE_CAVE_KITE_BEHIND_LEDGE) last
/// while the party goes between them. On every arrival ValeCave_ApplyEntryState
/// rebuilds the cells of each puzzle already solved and, once Kite has run
/// behind its ledge, puts it back there.

#include "save_block.h"
#include "djinn_ids.h"
#include "version.h"
#include "engine_call.h"
#include "heap_slots.h"
#include "unit/import.h"
#include "unit/scene/vale_cave.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/runtime/runtime.h"

#include "actor.h"
#include "map_grid_copy.h"
#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
#include "create_configured_overlay_object.h"
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
#include "field_effect.h"
#include "scene_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "flags.h"

#include "fixed_math.h"
#include "songs.h"
#include "audio_engine_symbols.h"

/// The way in's actor templates (gValeCave_MouthActorTemplates),
/// numbered from ACTOR_FIRST_PLACED: three stacks of crates (drawn as the
/// crates of the Bilibin Barricade in bilibin.c), then two logs (as in Kolima
/// Forest), none of which moves.
enum ValeCaveMouthActorId {
  /// ★ Named from what the player sees: three wooden crates stacked by the
  /// cave's mouth (its render)
  ACTOR_VALE_CAVE_MOUTH_FIRST_CRATES = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: two logs lying west to east beside the
  /// crates (its render)
  ACTOR_VALE_CAVE_MOUTH_FIRST_LOG = ACTOR_FIRST_PLACED + 3,
  /// ★ Named from what the player sees: two logs lying west to east beside the
  /// crates (its render)
  ACTOR_VALE_CAVE_MOUTH_SECOND_LOG
};

/// The second room's actor templates
/// (gValeCave_KiteActorTemplates), numbered from ACTOR_FIRST_PLACED.
enum ValeCaveKiteActorId {
  /// Kite, a Jupiter Djinni, placed until it joins (FLAG_KITE).
  /// ★ Named from the game: "Kite" (MsgDjinnName)
  ACTOR_KITE = ACTOR_FIRST_PLACED,
  /// The two stacks of crates in the holes' part, which sink
  /// into their holes: the western under Move, the eastern under Move or a
  /// push.
  /// ★ Named from what the player sees: a stack of wooden crates beside the
  /// water (its render)
  ACTOR_WEST_CRATES,
  /// ★ Named from what the player sees: a stack of wooden crates beside the
  /// water (its render)
  ACTOR_EAST_CRATES,
  /// The log the leader rides along the holes' part's passage, from one end
  /// to the other, drawn as the east-west log of Colosso's Lumber Water
  /// Crossing Area (colosseum_azart_match.c).
  /// ★ Named from what the player sees: a yellow log floating in the water (its
  /// render)
  ACTOR_RIDING_LOG,
  /// Two stacks of crates and two logs that stand in the first part.
  /// ★ Named from what the player sees: two stacks of wooden crates by the
  /// room's east door (its render)
  ACTOR_FIRST_STANDING_CRATES,
  /// ★ Named from what the player sees: two stacks of wooden crates by the
  /// room's east door (its render)
  ACTOR_SECOND_STANDING_CRATES,
  /// ★ Named from what the player sees: two logs lying west to east beside the
  /// crates (its render)
  ACTOR_FIRST_STANDING_LOG,
  /// ★ Named from what the player sees: two logs lying west to east beside the
  /// crates (its render)
  ACTOR_SECOND_STANDING_LOG
};

/// The third room's actor templates
/// (gValeCave_LogsActorTemplates), numbered from ACTOR_FIRST_PLACED;
/// its first row places Kite as the second room's does. Its three logs are
/// pushed, and lie as in Kolima Forest.
enum ValeCaveLogsActorId {
  /// Lying north to south, pushed off the ledge.
  /// ★ Named from what the player sees: a log lying north to south west of the
  /// water (its render)
  ACTOR_WEST_LOG = ACTOR_FIRST_PLACED + 1,
  /// ★ Named from what the player sees: a log lying north to south east of the
  /// water (its render)
  ACTOR_EAST_LOG, ///< lying north to south, pushed along the ledge until it
                  ///< falls
  /// ★ Named from what the player sees: a log lying west to east south of the
  /// water (its render)
  ACTOR_SOUTH_LOG ///< lying west to east, pushed north
};

/// The first of the twelve metatile behaviors that send Kite round its ledge
/// (EventScript_KiteKeepsAway), one for each of gValeCave_WalkTargets.
enum ValeCaveMetatileBehavior { MB_VALE_CAVE_FIRST_KITE_ROUTE = 45 };

/// The flag of Kite, set once it is caught.
/// ★ Named from the game: "Kite" (MsgDjinnName)
#define FLAG_KITE (FLAG_DJINNI(DJINNI_KITE))

// The cave's map flags: each room reuses them for its own puzzle, so the second
// room's two have one name in the holes' part and another in Kite's.
/// The holes' part: the western crates have sunk into their hole.
/// ★ Named from what the player sees: stacks of wooden crates at the edge of
/// the cave's water (its render)
#define FLAG_VALE_CAVE_WEST_CRATES_SUNK FLAG_SCENE_TEMP_00
/// The holes' part: the eastern crates have sunk into their hole.
/// ★ Named from what the player sees: stacks of wooden crates at the edge of
/// the cave's water (its render)
#define FLAG_VALE_CAVE_EAST_CRATES_SUNK FLAG_SCENE_TEMP_01
/// Kite's part: Kite has run off once (EventScript_KiteRunsOff).
/// ★ Named from the game: "Kite" (MsgDjinnName)
#define FLAG_VALE_CAVE_KITE_RAN_OFF FLAG_SCENE_TEMP_00
/// Kite's part: Kite has run behind the ledge
/// (EventScript_KiteRunsBehindLedge).
/// ★ Named from the game: "Kite" (MsgDjinnName)
#define FLAG_VALE_CAVE_KITE_RAN_BEHIND_LEDGE FLAG_SCENE_TEMP_01

// The cave's area flags. The second and third rooms are one area,
// so these last while the party goes between them; the first room is another.
/// The riding log of the second room is at the eastern end of its passage.
/// ★ Named from what the player sees: the log floating in the holes' part
/// of the second room, which carries the leader along it (its render)
#define FLAG_VALE_CAVE_RIDING_LOG_EAST FLAG_AREA_TEMP_01
/// Kite has run behind the ledge and keeps to its far side.
/// ★ Named from the game: "Kite" (MsgDjinnName)
#define FLAG_VALE_CAVE_KITE_BEHIND_LEDGE FLAG_AREA_TEMP_02

static void ValeCave_CenterActorOnCell(s32 actorId, s32 column, s32 row);
static void ValeCave_ApplyEntryState(void);

#include "common/effect_create.h"
#include "common/effect_spawn.h"
#include "common/object_staged_actor.h"

/// In the third room, the western log has been pushed off the ledge.
/// ★ Named from what the player sees: two logs standing north to south on the
/// ledges either side of the water, and a third lying west to east south of it
/// (its render)
#define FLAG_VALE_CAVE_WEST_LOG_PUSHED_OFF_LEDGE FLAG_SCENE_TEMP_02
/// In the third room, the eastern log has been pushed along the ledge and over
/// its edge.
/// ★ Named from what the player sees: two logs standing north to south on the
/// ledges either side of the water, and a third lying west to east south of it
/// (its render)
#define FLAG_VALE_CAVE_EAST_LOG_PUSHED_OVER_EDGE FLAG_SCENE_TEMP_03
/// In the third room, the southern log has been pushed north and stopped on the
/// northern row.
/// ★ Named from what the player sees: two logs standing north to south on the
/// ledges either side of the water, and a third lying west to east south of it
/// (its render)
#define FLAG_VALE_CAVE_SOUTH_LOG_ON_NORTH_ROW FLAG_SCENE_TEMP_04
/// In the third room, the southern log has been pushed north and stopped on the
/// southern row (row 20).
/// ★ Named from what the player sees: two logs standing north to south on the
/// ledges either side of the water, and a third lying west to east south of it
/// (its render)
#define FLAG_VALE_CAVE_SOUTH_LOG_ON_SOUTH_ROW FLAG_SCENE_TEMP_05

/// In Vale Cave's second room, talking to Kite once Halt has stopped it: Kite,
/// a Jupiter Djinni, joins the party.
///
/// Scene_TalkToActor selects this TRIGGER_TALK_HALTED event while Kite
/// is the paused actor. Djinn_JoinParty gives the Jupiter Djinni Kite
/// and sets FLAG_KITE, so neither room places Kite again.
static void EventScript_TalkToKite(void) {
  Djinn_JoinParty(ACTOR_KITE, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_KITE));
}

/// Waits for an actor to land, bounded by a one-second timeout.
static void ValeCave_WaitForActorLanding(struct Actor *actor) {
  s32 framesRemaining = 60;

  while (framesRemaining != 0) {
    WaitFrames(1);
    framesRemaining--;
    if (actor->y.fixed <= actor->groundHeight)
      break;
  }
  actor->y.fixed = actor->groundHeight;
}

/// Turns Kite to face the party leader each frame.
///
/// Kite's update hook once it has run behind the ledge, set by
/// EventScript_KiteRunsBehindLedge and again by ValeCave_ApplyEntryState.
// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 38 to 36 bytes in tbs-en.
#ifdef UBFIX
static void ValeCave_FaceActorTowardLeader(union ActorOrEffect *fieldObject) {
#else
static s32 ValeCave_FaceActorTowardLeader(union ActorOrEffect *fieldObject) {
#endif
  struct Actor *actor = (struct Actor *)fieldObject;
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  s32 distanceZ = leader->z.fixed - actor->z.fixed;
  s32 distanceX = leader->x.fixed - actor->x.fixed;

  actor->facing = ArcTan2(distanceZ, distanceX);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

#ifdef UBFIX
#define VALE_CAVE_FACE_LEADER_CALLBACK ValeCave_FaceActorTowardLeader
#else
#define VALE_CAVE_FACE_LEADER_CALLBACK                                         \
  ((void (*)(union ActorOrEffect *))ValeCave_FaceActorTowardLeader)
#endif

/// Selects entrance positions and camera bounds for the current cave
/// room.
const struct SceneEntrance *ValeCave_GetEntrances(void) {
  s16 sceneId = gSaveBlock.sceneId;

  if (sceneId == (s32)&SceneId_ValeCaveMouth) {
    return gValeCave_MouthEntrances;
  }
  if (sceneId == (s32)&SceneId_ValeCaveKite) {
    return gValeCave_KiteEntrances;
  }
  if (sceneId == (s32)&SceneId_ValeCaveLogs) {
    return gValeCave_LogsEntrances;
  }
  return gValeCave_EntrancesOther;
}

const struct SceneExitRegion *ValeCave_GetRegions(void) { return NULL; }

/// Returns the shared packed exits for all cave rooms.
const u32 *ValeCave_GetExits(void) { return gValeCave_Exits; }

/// Selects the current room's actor templates: Kite, and the crates and
/// logs.
const struct ActorTemplate *ValeCave_GetActorTemplates(void) {
  s16 sceneId = gSaveBlock.sceneId;

  if (sceneId == (s32)&SceneId_ValeCaveMouth) {
    return gValeCave_MouthActorTemplates;
  }
  if (sceneId == (s32)&SceneId_ValeCaveKite) {
    return gValeCave_KiteActorTemplates;
  }
  if (sceneId == (s32)&SceneId_ValeCaveLogs) {
    return gValeCave_LogsActorTemplates;
  }
  return gValeCave_MouthActorTemplates;
}

/// In Vale Cave's second room, Reveal ends and the hidden cell is hidden again.
///
/// Reached by Reveal's PSYNERGY_PHASE_EFFECT_ENDS anywhere in the second room.
static void EventScript_HideRevealedTile(void) {
  MapGrid_CopyCells(16, 15, 1, 1, 15, 15);
}

/// In Vale Cave's second room, Reveal shows a hidden cell.
///
/// Reached by Reveal's PSYNERGY_PHASE_EFFECT_ACTS anywhere in the second room;
/// copies cell (16, 17) over cell (15, 15) until EventScript_HideRevealedTile
/// puts it back.
static void EventScript_RevealHiddenTile(void) {
  MapGrid_CopyCells(16, 17, 1, 1, 15, 15);
}

/// Sinks a stack of crates into its hole, spawning a particle each frame around
/// the opening.
static inline void
ValeCave_SinkObjectWithParticles(s32 actorId, s32 baseX, s32 baseZ,
                                 struct EffectOptions *particleOptions) {
  u32 frame;

  for (frame = 0; frame < 68; frame++) {
    s32 particleX = (((u32)(Import_RandomNext() * 17) >> 16) << 16) + baseX;
    s32 particleZ = (((u32)(Import_RandomNext() * 14) >> 16) << 16) + baseZ;

    FieldEffect_Spawn(particleX, 0, particleZ, 0, 0, 0,
                      EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
                      particleOptions);
    Actor_GetById(actorId)->y.fixed -= PIXELS(1) / 2;
    FieldScript_WaitFrames(1);
  }
}

/// In Vale Cave's second room, Move on the western crates: once they are over
/// their hole, Isaac steps aside and they sink into the floor in a cloud of
/// dust.
///
/// Reached by Move on ACTOR_WEST_CRATES in the second room, until
/// FLAG_VALE_CAVE_WEST_CRATES_SUNK is set. Over their hole (column 23) it sets
/// FLAG_VALE_CAVE_WEST_CRATES_SUNK and makes the hole's cells walkable.
static void EventScript_MoveWestCratesIntoHole(void) {
  struct EffectOptions options;
  struct EffectOptions *particleOptions;
  struct Actor *leftEffect;
  struct Actor *rightEffect;

  FieldScript_Start();
  if (Actor_GetById(ACTOR_WEST_CRATES)->x.fixed >> 20 == 23) {
    // Move the leader aside before the crates start sinking.
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 360, 664);
    ObjectMotion_ArmCallback(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 10);
    Actor_GetById(ACTOR_WEST_CRATES)->x.fixed += PIXELS(2);
    leftEffect = Actor_PrepareObject(
        Actor_GetById(ACTOR_WEST_CRATES)->x.fixed, 0,
        Actor_GetById(ACTOR_WEST_CRATES)->z.fixed + PIXELS(52), SPRITE_CRATES);
    rightEffect = Actor_PrepareObject(
        Actor_GetById(ACTOR_WEST_CRATES)->x.fixed + PIXELS(16), 0,
        Actor_GetById(ACTOR_WEST_CRATES)->z.fixed + PIXELS(52), SPRITE_CRATES);
    Actor_GetById(ACTOR_WEST_CRATES)->motionFlags = 0;
    // Surround the sinking crates with one particle per frame.
    particleOptions = &options;
    particleOptions->startScaleX = 3 * Q16_ONE / 5;
    particleOptions->startScaleY = 3 * Q16_ONE / 5;
    particleOptions->palette = 7;
    Sound_PlaySong(SE_BURST);
    ValeCave_SinkObjectWithParticles(ACTOR_WEST_CRATES, PIXELS(368),
                                     PIXELS(624), particleOptions);
    MapGrid_CopyCellAttributes(23, 41, 1, 1, 23, 39);
    Actor_GetById(ACTOR_WEST_CRATES)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    GameFlag_Set(FLAG_VALE_CAVE_WEST_CRATES_SUNK);
    Actor_GetById(ACTOR_WEST_CRATES)->y.fixed = -PIXELS(8);
    Import_ActorSetAnimation(ACTOR_WEST_CRATES, 2);
    Actor_Release(leftEffect);
    Actor_Release(rightEffect);
    FieldScript_WaitFrames(30);
  }
  FieldScript_End();
}

/// In Vale Cave's second room, Move on the eastern crates: once they are over
/// their hole, they sink into the floor.
///
/// Reached by Move on ACTOR_EAST_CRATES in the second room, until
/// FLAG_VALE_CAVE_EAST_CRATES_SUNK is set, and from
/// EventScript_PushEastCratesIntoHole. Over their hole (column 27) it sets
/// FLAG_VALE_CAVE_EAST_CRATES_SUNK and makes the hole's cells walkable.
static void EventScript_MoveEastCratesIntoHole(void) {
  struct EffectOptions options;
  struct EffectOptions *particleOptions;
  struct Actor *leftEffect;
  struct Actor *rightEffect;

  FieldScript_Start();
  if (Actor_GetById(ACTOR_EAST_CRATES)->x.fixed >> 20 == 27) {
    leftEffect = Actor_PrepareObject(
        Actor_GetById(ACTOR_EAST_CRATES)->x.fixed - PIXELS(8), 0,
        Actor_GetById(ACTOR_EAST_CRATES)->z.fixed + PIXELS(52), SPRITE_CRATES);
    rightEffect = Actor_PrepareObject(
        Actor_GetById(ACTOR_EAST_CRATES)->x.fixed + PIXELS(8), 0,
        Actor_GetById(ACTOR_EAST_CRATES)->z.fixed + PIXELS(52), SPRITE_CRATES);
    // This clears the western crates' motion flags, not these crates', where
    // EventScript_MoveWestCratesIntoHole clears its own. These sink all the
    // same, since the sinking writes their height directly and nothing pulls
    // them back up, as the recording of this event shows.
    Actor_GetById(ACTOR_WEST_CRATES)->motionFlags = 0;
    // Surround the sinking crates with one particle per frame.
    particleOptions = &options;
    particleOptions->startScaleX = 3 * Q16_ONE / 5;
    particleOptions->startScaleY = 3 * Q16_ONE / 5;
    particleOptions->palette = 7;
    Sound_PlaySong(SE_BURST);
    ValeCave_SinkObjectWithParticles(ACTOR_EAST_CRATES, PIXELS(432),
                                     PIXELS(656), particleOptions);
    MapGrid_CopyCellAttributes(31, 39, 2, 1, 27, 41);
    Actor_GetById(ACTOR_EAST_CRATES)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    GameFlag_Set(FLAG_VALE_CAVE_EAST_CRATES_SUNK);
    Actor_GetById(ACTOR_EAST_CRATES)->y.fixed = -PIXELS(8);
    Import_ActorSetAnimation(ACTOR_EAST_CRATES, 2);
    Actor_Release(leftEffect);
    Actor_Release(rightEffect);
    FieldScript_WaitFrames(30);
  }
  FieldScript_End();
}

/// In Vale Cave's second room, pushing the eastern crates: they move a cell,
/// and sink if they are then over their hole.
///
/// Reached by walking into metatile behavior 24 in the second room, until
/// FLAG_VALE_CAVE_EAST_CRATES_SUNK is set.
static void EventScript_PushEastCratesIntoHole(void) {
  FieldScript_Start();
  PushedObject_AdvancePair();
  FieldScript_End();
  EventScript_MoveEastCratesIntoHole();
}

/// In Vale Cave's second room, walking south off a ledge: Isaac hops down if
/// the cell below is free.
///
/// Reached by walking south into metatile behavior 21 in the second room.
/// Returns 1 when the leader hopped, 0 when the landing cell is blocked.
static s32 EventScript_HopDownLedge(s32 unusedArgument) {

  union FieldCoordinate landingPosition[3];
  struct Actor *leader;
  u8 *motionFlags;
  s32 savedMotionFlags;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  motionFlags = &leader->motionFlags;
  savedMotionFlags = *motionFlags;
#if TBS_PATCH_1
  landingPosition[0].fixed = (leader->x.fixed & -0x100000) + PIXELS(8);
  landingPosition[1].fixed = leader->y.fixed;
  landingPosition[2].fixed = (leader->z.fixed & -0x100000) + PIXELS(40);
#else
  landingPosition[0].fixed = leader->x.fixed;
  landingPosition[1].fixed = leader->y.fixed;
  landingPosition[2].fixed = leader->z.fixed + PIXELS(32);
#endif
  // Keep the normal motion flags while staging the hop.
  if (FieldObject_CheckMovementCollision(
          leader, (struct FieldPosition *)landingPosition) == 0) {
    FieldScript_Start();
    Object_SelectAnimation(leader, 6);
    WaitFrames(6);
    Sound_PlaySong(SE_JUMP);
    Object_SelectAnimation(leader, 7);
    leader->speed = PIXELS(3);
    leader->acceleration = PIXELS(2);
    leader->velocityY = PIXELS(4);
    *motionFlags &= 126;
    Actor_SetSpriteFlags(leader, 0);
    ObjectMotion_SetPositionAndCommit(ACTOR_PARTY_LEADER,
                                      landingPosition[0].part.pixel,
                                      landingPosition[2].part.pixel);
    Object_SelectAnimation(leader, 6);
    Actor_SetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
    *motionFlags = savedMotionFlags;
    FieldScript_End();
    return 1;
  }
  return 0;
}

/// In Kite's part of Vale Cave's second room, coming near Kite: the camera
/// finds it; it notices the party and hops away up the ledge.
///
/// Reached by stepping on metatile behavior 40 in the second room until Kite is
/// caught (FLAG_KITE); runs once, setting FLAG_VALE_CAVE_KITE_RAN_OFF.
static void EventScript_KiteRunsOff(void) {

  struct Actor *djinni;

  if (Script_GetFlag(FLAG_VALE_CAVE_KITE_RAN_OFF) == 0) {
    GameFlag_Set(FLAG_VALE_CAVE_KITE_RAN_OFF);
    FieldScript_Start();
    FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
    FieldCamera_FollowActor(ACTOR_KITE, 1);
    FieldCamera_WaitForArrival();
    FieldScript_WaitFrames(60);
    Actor_FaceDirection(ACTOR_KITE, FACING_NORTH, 20);
    Actor_SetAttachedEffect(ACTOR_KITE,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    ObjectMotion_SetVarCbAndRefresh(ACTOR_KITE, 2);
    FieldScript_WaitFrames(20);
    Actor_SetSpeed(ACTOR_KITE, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_KITE, 792, 248);
    Sound_PlaySong(SE_JUMP);
    djinni = Actor_Get(ACTOR_KITE);
    djinni->velocityY = PIXELS(8);
    Actor_WalkToAndWait(ACTOR_KITE, 792, 280);
    FieldScript_WaitFrames(20);
    Actor_FaceDirection(ACTOR_KITE, FACING_NORTH, 20);
    FieldScript_WaitFrames(30);
    FieldScript_End();
  }
}

/// In Kite's part of Vale Cave's second room, coming near again: Kite runs
/// behind the ledge and turns to watch the party.
///
/// Reached by stepping on metatile behavior 41 in the second room until Kite is
/// caught, once EventScript_KiteRunsOff has run. Sets
/// FLAG_VALE_CAVE_KITE_RAN_BEHIND_LEDGE and FLAG_VALE_CAVE_KITE_BEHIND_LEDGE,
/// and gives Kite the update hook ValeCave_FaceActorTowardLeader.
static void EventScript_KiteRunsBehindLedge(void) {

  struct Actor *djinni;

  if (GameFlag_Get(FLAG_VALE_CAVE_KITE_RAN_OFF) != 0) {
    if (Script_GetFlag(FLAG_VALE_CAVE_KITE_RAN_BEHIND_LEDGE) == 0) {
      GameFlag_Set(FLAG_VALE_CAVE_KITE_RAN_BEHIND_LEDGE);
      GameFlag_Set(FLAG_VALE_CAVE_KITE_BEHIND_LEDGE);
      FieldScript_Start();
      Actor_SetAttachedEffect(ACTOR_KITE,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      ObjectMotion_SetVarCbAndRefresh(ACTOR_KITE, 2);
      FieldScript_WaitFrames(20);
      Actor_SetSpeed(ACTOR_KITE, PIXELS(2), PIXELS(1));
      Actor_WalkToAndWait(ACTOR_KITE, 760, 280);
      Actor_WalkToAndWait(ACTOR_KITE, 760, 312);
      Actor_WalkToAndWait(ACTOR_KITE, 792, 312);
      FieldScript_WaitFrames(10);
      Actor_FaceDirection(ACTOR_KITE, FACING_NORTH, 20);
      djinni = Actor_Get(ACTOR_KITE);
      djinni->update = VALE_CAVE_FACE_LEADER_CALLBACK;
      FieldScript_End();
    }
  }
}

/// In Kite's part of Vale Cave's second room, walking around the ledge: Kite
/// keeps to the far side of it, unless Halt has stopped it.
///
/// Reached by stepping on metatile behaviors 45 to 56 in the second room until
/// Kite is caught, once FLAG_VALE_CAVE_KITE_BEHIND_LEDGE is set; each behavior
/// sends Kite to its row of gValeCave_WalkTargets at the leader's speed.
static void EventScript_KiteKeepsAway(void) {
  struct Actor *djinni;
  struct Actor *leader;
  s32 routeIndex;
  const u8 *walkTargetBytes;
  s32 targetZOffset;
  struct FieldWork *work;

  work = gFieldWork;
  if (GameFlag_Get(FLAG_VALE_CAVE_KITE_BEHIND_LEDGE) != 0) {
    if (gSaveBlock.pausedObject != ACTOR_KITE) {
      // Preserve the speed reads before resolving this route point.
      routeIndex = work->stepTrigger;
      djinni = Actor_GetById(ACTOR_KITE);
      leader = Actor_GetById(ACTOR_PARTY_LEADER);
      djinni->speed = leader->speed;
      djinni = Actor_GetById(ACTOR_KITE);
      leader = Actor_GetById(ACTOR_PARTY_LEADER);
      djinni->acceleration = leader->acceleration;
      routeIndex -= MB_VALE_CAVE_FIRST_KITE_ROUTE;
      // X/Z pairs occupy eight bytes. Plain pair indexing changes six
      // instruction words in the English build, so keep separate offsets.
      walkTargetBytes = (const u8 *)gValeCave_WalkTargets;
      routeIndex <<= 3;
      targetZOffset = routeIndex + sizeof(s32);
      ObjectMotion_ResetAndSetPositionInMode2(
          ACTOR_KITE, *(const s32 *)(walkTargetBytes + routeIndex),
          *(const s32 *)(walkTargetBytes + targetZOffset));
    }
  }
}

/// In Vale Cave's second room, Halt on Kite: it stops where it stands, and the
/// party can walk up to it and talk to it (EventScript_TalkToKite).
///
/// Reached by Halt on Kite in the second room.
static void EventScript_HaltKite(void) {
  FieldScript_Start();
  Import_ActorSetAnimation(ACTOR_KITE, 0);
  FieldScript_End();
}

/// In Vale Cave's third room, pushing the eastern log along the ledge: at its
/// edge the log falls off, opening the way.
///
/// Reached by walking into metatile behavior 20 in the third room. When the log
/// falls (column 17) it sets FLAG_VALE_CAVE_EAST_LOG_PUSHED_OVER_EDGE and
/// clears the cells it held.
static void EventScript_PushLogOverEdge(void) {
  struct Actor *movedLog;
  s32 stationaryMotion;
  s32 clearedRows;
  struct PushedObjectProbe probe;

  FieldScript_Start();
  if (PushedObject_FindClearPosition(&probe) != 0) {
    Actor_MoveAndRedraw(probe);
    // At the ledge's edge (column 17) the log slides west and drops.
    if ((probe.position.x >> 20) == 17) {
      Import_ActorSetAnimation(probe.actorSlot, 3);
      stationaryMotion = 0;
      Actor_GetById(probe.actorSlot)->motionFlags = stationaryMotion;
      movedLog = Actor_GetById(probe.actorSlot);
      movedLog->bounce = stationaryMotion;
      ObjectMotion_OffsetPositionAndResetMotion(probe.actorSlot, -12, 0);
      ObjectMotion_CommitCurrentPositionAndActivate(probe.actorSlot);
      Import_ActorSetAnimation(probe.actorSlot, 3);
      ObjectMotion_SetActionVariant(ACTOR_EAST_LOG, 3);
      Actor_GetById(probe.actorSlot)->motionFlags =
          ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      ObjectMotion_OffsetPositionAndResetMotion(probe.actorSlot, -6, 0);
      ValeCave_WaitForActorLanding(Actor_GetById(probe.actorSlot));
      Import_ActorSetAnimation(probe.actorSlot, 8);
      {
        struct Actor *movedLog = Actor_GetById(probe.actorSlot);

        movedLog->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      }
      // The cells it held on the ledge, and its landing, become open floor.
      clearedRows = 4;
      PushedObject_FillGridAttributeRectangle(0, (probe.position.x >> 20),
                                              ((probe.position.z >> 20) - 2), 1,
                                              clearedRows, stationaryMotion);
      PushedObject_FillGridAttributeRectangle(2, (probe.position.x >> 20),
                                              ((probe.position.z >> 20) - 2), 1,
                                              clearedRows, stationaryMotion);
      PushedObject_FillGridAttributeRectangle(2, 16, 18, 1, 2,
                                              stationaryMotion);
      PushedObject_FillGridAttributeRectangle(0, 16, 16, 1, clearedRows,
                                              stationaryMotion);
      GameFlag_Set(FLAG_VALE_CAVE_EAST_LOG_PUSHED_OVER_EDGE);
      Sound_PlaySong(SE_LOG_FALL);
    }
  }
  FieldScript_End();
}

/// In Vale Cave's third room, pushing the western log east: it falls off the
/// ledge, opening the way.
///
/// Reached by walking east into metatile behavior 21 in the third room. Sets
/// FLAG_VALE_CAVE_WEST_LOG_PUSHED_OFF_LEDGE and clears the cells the log held.
static void EventScript_PushLogOffLedge(void) {

  struct Actor *pushedLog;
  s32 stationaryMotion;

  FieldScript_Start();
  FieldScript_WaitFrames(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
  FieldScript_WaitFrames(15);
  ObjectMotion_OffsetPositionAndResetMotion(ACTOR_PARTY_LEADER, 8, 0);
  FieldScript_WaitFrames(4);
  Script_PlaySong(SE_STOP_ACTION_SE);
  Script_PlaySong(SE_HEAVY_MOVE);
  Actor_SetSpeed(ACTOR_WEST_LOG, PIXELS(1) / 2, PIXELS(1) / 10);
  Import_ActorSetAnimation(ACTOR_WEST_LOG, 2);
  stationaryMotion = 0;
  Actor_GetById(ACTOR_WEST_LOG)->motionFlags = stationaryMotion;
  pushedLog = Actor_Get(ACTOR_WEST_LOG);
  pushedLog->bounce = stationaryMotion;
  ObjectMotion_OffsetPositionAndResetMotion(ACTOR_WEST_LOG, 12, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_PARTY_LEADER);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_WEST_LOG);
  Script_PlaySong(SE_STOP_ACTION_SE);
  Script_PlaySong(SE_HEAVY_STOP);
  Import_ActorSetAnimation(ACTOR_WEST_LOG, 3);
  Actor_GetById(ACTOR_WEST_LOG)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  ObjectMotion_OffsetPositionAndResetMotion(ACTOR_WEST_LOG, 6, 0);
  ValeCave_WaitForActorLanding(Actor_Get(ACTOR_WEST_LOG));
  Import_ActorSetAnimation(ACTOR_WEST_LOG, 8);
  ObjectMotion_SetActionVariant(ACTOR_WEST_LOG, 3);
  Actor_GetById(ACTOR_WEST_LOG)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  PushedObject_FillGridAttributeRectangle(0, 12, 16, 1, 4, MB_NORMAL);
  PushedObject_FillGridAttributeRectangle(0, 13, 16, 1, 4, MB_NORMAL);
  GameFlag_Set(FLAG_VALE_CAVE_WEST_LOG_PUSHED_OFF_LEDGE);
  Script_PlaySong(SE_LOG_FALL);
  FieldScript_End();
}

/// In Vale Cave's third room, pushing the southern log north: it slides a cell,
/// opening the way.
///
/// Reached by walking into metatile behavior 22 in the third room. Sets
/// FLAG_VALE_CAVE_SOUTH_LOG_ON_SOUTH_ROW when it stops on row 20, else
/// FLAG_VALE_CAVE_SOUTH_LOG_ON_NORTH_ROW, and then opens the cells above it.
static void EventScript_PushLogNorth(void) {
  struct PushedObjectProbe probe;

  FieldScript_Start();
  if (PushedObject_FindClearPosition(&probe) != 0) {
    Actor_MoveAndRedraw(probe);
    // Slide into the new cell before replacing the metatile behavior footprint.
    Import_ActorSetAnimation(ACTOR_SOUTH_LOG, 3);
    Actor_SetSpeed(ACTOR_SOUTH_LOG, PIXELS(1) / 4, PIXELS(1) / 2);
    ObjectMotion_OffsetPositionAndResetMotion(ACTOR_SOUTH_LOG, 0, -16);
    FieldScript_WaitFrames(45);
    Sound_PlaySong(SE_LOG_FALL);
    Import_ActorSetAnimation(ACTOR_SOUTH_LOG, 8);
    {
      struct Actor *slidingLog = Actor_GetById(ACTOR_SOUTH_LOG);
      s32 clearedRows = 2;
      s32 emptyAttributes = 0;

      slidingLog->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      PushedObject_FillGridAttributeRectangle(
          0, 13, (probe.position.z >> 20) - 1, 4, clearedRows, emptyAttributes);
    }
    if ((probe.position.z >> 20) == 20) {
      GameFlag_Set(FLAG_VALE_CAVE_SOUTH_LOG_ON_SOUTH_ROW);
    } else {
      GameFlag_Set(FLAG_VALE_CAVE_SOUTH_LOG_ON_NORTH_ROW);
      {

        MapGrid_CopyCellAttributes(14, 17, 2, 1, 14, 16);
        MapGrid_CopyCellAttributes(14, 13, 1, 1, 14, 15);
      }
    }
  }
  FieldScript_End();
}

/// Selects the current room's coord, Psynergy and examine triggers.
const struct Trigger *ValeCave_GetTriggers(void) {
  s16 sceneId = gSaveBlock.sceneId;

  if (sceneId == (s32)&SceneId_ValeCaveMouth) {
    return gValeCave_MouthTriggers;
  }
  if (sceneId == (s32)&SceneId_ValeCaveKite) {
    return gValeCave_KiteTriggers;
  }
  if (sceneId == (s32)&SceneId_ValeCaveLogs) {
    return gValeCave_LogsTriggers;
  }
  return gValeCave_TriggersOther;
}

/// Arriving in any of the three rooms: chooses the entry transition and
/// restores the room's puzzle state.
///
/// The map's initialize entry. The first room, Vale, fades in from the
/// backdrop and clears FLAG_SHOW_LOCATION_NAME, so the area name is not shown
/// again; the two rooms of Vale Cave open with the window transition.
s32 ValeCave_Initialize(void) {
  s32 *request = &gFieldWork->transition;

  *request =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gSaveBlock.sceneId == (s32)&SceneId_ValeCaveMouth) {
    *request = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    WaitFrames(1);
    ObjectMotion_SetActionVariant(ACTOR_VALE_CAVE_MOUTH_FIRST_LOG, 3);
    ObjectMotion_SetActionVariant(ACTOR_VALE_CAVE_MOUTH_SECOND_LOG, 3);
    GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  }
  ValeCave_ApplyEntryState();
  return 0;
}

/// In Vale, the cave's way in (SceneId_ValeCaveMouth), each exit plays the exit
/// sound before leaving.
///
/// Reached by stepping on metatile behaviors 1 to 3 in the first room; takes
/// the exit of the same number (VALE_CAVE_MOUTH_TO_VALE_KRADENS_COTTAGE_WEST,
/// VALE_CAVE_MOUTH_TO_VALE_CAVE_KITE or
/// VALE_CAVE_MOUTH_TO_VALE_KRADENS_COTTAGE_EAST).
static void EventScript_LeaveRoom(void) {
  s32 exitNumber = gFieldWork->stepTrigger;

  Sound_PlaySong(SE_MAP_EXIT);
  Import_ScriptRequestExit(exitNumber);
}

/// Rebuilds the passage cells around the riding log's saved end.
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

#include "common/object_pairmove.h"

/// In Vale Cave's second room, walking east onto the riding log: it carries
/// the leader to the eastern end of the passage.
///
/// Reached by walking east into metatile behavior 20 in the second room. Sets
/// FLAG_VALE_CAVE_RIDING_LOG_EAST.
static void EventScript_RideLogEast(void) {

  Sound_PlaySong(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_RIDING_LOG, 112, 0);
  Actor_PlacePairAtOffset(ACTOR_RIDING_LOG, 80, 0);
  GameFlag_Set(FLAG_VALE_CAVE_RIDING_LOG_EAST);
  WaitFrames(2);
  ValeCave_UpdateRidingLogPassage();
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
}

/// In Vale Cave's second room, walking west onto the riding log: it carries
/// the leader back to the western end of the passage.
///
/// Reached by walking west into metatile behavior 21 in the second room. Clears
/// FLAG_VALE_CAVE_RIDING_LOG_EAST.
static void EventScript_RideLogWest(void) {

  Sound_PlaySong(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_RIDING_LOG, -112, 0);
  Actor_PlacePairAtOffset(ACTOR_RIDING_LOG, -80, 0);
  GameFlag_Clear(FLAG_VALE_CAVE_RIDING_LOG_EAST);
  WaitFrames(2);
  ValeCave_UpdateRidingLogPassage();
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
}

/// Centers an actor on a map cell and gives it underfoot priority.
static void ValeCave_CenterActorOnCell(s32 actorId, s32 column, s32 row) {
  struct Actor *actor = Actor_GetById(actorId);

  if (actor != 0) {
    ObjectMotion_SetActionVariant(actorId, 3);
    actor->layer = 2;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actor->x.fixed = (column << 20) + PIXELS(8);
    actor->z.fixed = (row << 20) + PIXELS(8);
  }
}

/// Restores, on entry, Kite behind its ledge and the crates and logs of the
/// puzzles already solved.
///
/// Called by ValeCave_Initialize. In Kite's part of the second room
/// (VALE_CAVE_KITE_EAST_FROM_WEST to VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_EAST),
/// while Kite is free and FLAG_VALE_CAVE_KITE_BEHIND_LEDGE is set, it sets
/// FLAG_VALE_CAVE_KITE_RAN_OFF (and FLAG_VALE_CAVE_KITE_RAN_BEHIND_LEDGE from
/// VALE_CAVE_KITE_EAST_FROM_WEST) and puts Kite back behind the ledge.
static void ValeCave_ApplyEntryState(void) {
  s32 southLogOnSouthernRow;

  // Each room reuses the temporary flags for its own puzzle state.
  if (gSaveBlock.sceneId == (s32)&SceneId_ValeCaveKite) {
    switch (gSaveBlock.entrance) {
    case VALE_CAVE_KITE_FROM_VALE_CAVE_MOUTH:
    case VALE_CAVE_KITE_WEST_FROM_EAST:
    case VALE_CAVE_KITE_WEST_FROM_SOUTH:
    case VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_WEST:
      ObjectMotion_SetActionVariant(ACTOR_SECOND_STANDING_LOG, 3);
      ObjectMotion_SetActionVariant(ACTOR_SECOND_STANDING_CRATES, 3);
      // A still object of sprite 223 (spr_223.png, a wooden crate) that shows
      // nothing. XXX (no evidence): what it and the chest below were meant to
      // show; the sprites alone hold neither, and neither object is solid or a
      // target.
      Actor_CreateConfiguredObject(PIXELS(120), 0, PIXELS(232), SPRITE_CRATE);
      break;
    case VALE_CAVE_KITE_EAST_FROM_WEST:
    case VALE_CAVE_KITE_EAST_FROM_SOUTH:
    case VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_EAST:
      if (GameFlag_Get(FLAG_KITE) != 0) {
        break;
      }
      if (GameFlag_Get(FLAG_VALE_CAVE_KITE_BEHIND_LEDGE) == 0) {
        break;
      }
      GameFlag_Set(FLAG_VALE_CAVE_KITE_RAN_OFF);
      if (gSaveBlock.entrance == VALE_CAVE_KITE_EAST_FROM_WEST) {
        GameFlag_Set(FLAG_VALE_CAVE_KITE_RAN_BEHIND_LEDGE);
      }
      WaitFrames(1);
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
        break;
      }
      // Kite waits behind the ledge, watching the leader.
      Actor_SetPosition(ACTOR_KITE, PIXELS(792), PIXELS(280));
      Actor_GetById(ACTOR_KITE)->update = VALE_CAVE_FACE_LEADER_CALLBACK;
      break;
    case VALE_CAVE_KITE_SOUTH_FROM_WEST:
    case VALE_CAVE_KITE_SOUTH_FROM_EAST:
    case VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_SOUTH:
      // The holes' part: a still chest object that shows nothing, the riding
      // log's passage, and the holes already filled.
      Actor_CreateConfiguredObject(PIXELS(642), 0, PIXELS(552), SPRITE_CHEST);
      MapGrid_CopyCellAttributes(23, 34, 13, 3, 0, 34);
      ValeCave_UpdateRidingLogPassage();
      if (GameFlag_Get(FLAG_VALE_CAVE_WEST_CRATES_SUNK) != 0) {
        MapGrid_CopyCellAttributes(23, 41, 1, 1, 23, 39);
      }
      if (GameFlag_Get(FLAG_VALE_CAVE_EAST_CRATES_SUNK) != 0) {
        MapGrid_CopyCellAttributes(31, 39, 2, 1, 27, 41);
      }
      break;
    }
  } else if (gSaveBlock.sceneId == (s32)&SceneId_ValeCaveLogs) {
    switch (gSaveBlock.entrance) {
    case VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_WEST:
    case VALE_CAVE_LOGS_NORTH_FROM_SOUTH:
    case VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_EAST:
      // Each log already pushed leaves open floor; else its footprint
      // blocks its cells again.
      if (GameFlag_Get(FLAG_VALE_CAVE_WEST_LOG_PUSHED_OFF_LEDGE) != 0) {
        PushedObject_FillGridAttributeRectangle(0, 12, 16, 1, 4, MB_NORMAL);
        PushedObject_FillGridAttributeRectangle(0, 13, 16, 1, 4, MB_NORMAL);
      } else {
        Scene_RedrawActorFootprint(ACTOR_WEST_LOG);
      }
      if (GameFlag_Get(FLAG_VALE_CAVE_EAST_LOG_PUSHED_OVER_EDGE) != 0) {
        PushedObject_FillGridAttributeRectangle(2, 16, 16, 1, 4, MB_NORMAL);
        PushedObject_FillGridAttributeRectangle(0, 16, 16, 1, 4, MB_NORMAL);
      } else {
        Scene_RedrawActorFootprint(ACTOR_EAST_LOG);
      }
      southLogOnSouthernRow =
          GameFlag_Get(FLAG_VALE_CAVE_SOUTH_LOG_ON_SOUTH_ROW);
      if (southLogOnSouthernRow != 0) {
        PushedObject_FillGridAttributeRectangle(0, 13, 19, 4, 2, MB_NORMAL);
      } else if (GameFlag_Get(FLAG_VALE_CAVE_SOUTH_LOG_ON_NORTH_ROW) != 0) {
        PushedObject_FillGridAttributeRectangle(0, 13, 15, 4, 2,
                                                southLogOnSouthernRow);
        MapGrid_CopyCellAttributes(14, 17, 2, 1, 14, 16);
        MapGrid_CopyCellAttributes(14, 13, 1, 1, 14, 15);
      } else {
        Scene_RedrawActorFootprint(ACTOR_SOUTH_LOG);
        ObjectMotion_SetActionVariant(ACTOR_SOUTH_LOG, 3);
      }
      break;
    }
  }
}

/// In Vale Cave's second room, touching the riding log restarts its
/// animation.
///
/// Reached by stepping on metatile behavior 20 or 21 in the second room.
static void EventScript_TouchRidingLog(void) {
  Import_ActorSetAnimation(ACTOR_RIDING_LOG, 1);
  Import_ActorSetAnimation(ACTOR_RIDING_LOG, 2);
}

// The lifetimes FieldEffect_Spawn gives its effects through
// gFieldEffect_DurationScripts: an effect waits 23, 45 or 127 frames, then is
// put away and freed. FieldEffect_Spawn divides a change of scale by the loop
// count in each one's fourth word, so that the effect reaches its scale as its
// time runs out.

s32 gValeCave_ShortEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gValeCave_MediumEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gValeCave_LongEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

// Pixel step by direction: X in the upper halfword, Z in the lower.
// UB: negative pixel steps cannot be shifted as signed values.
#ifdef UBFIX
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((u32)(dz) & 0xffff))
#else
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))
#endif

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

/// The twelve points round the far side of Kite's ledge, x then z in pixels,
/// one for each metatile behavior from MB_VALE_CAVE_FIRST_KITE_ROUTE on.
s32 gValeCave_WalkTargets[] = {
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
    gValeCave_ShortEffectScript,
    gValeCave_MediumEffectScript,
    gValeCave_LongEffectScript,
};

struct SceneEntrance gValeCave_EntrancesOther[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MAP_TABLE_END },
};

struct SceneEntrance gValeCave_MouthEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, 16, 0, 336, 368, 0 },
    { VALE_CAVE_MOUTH_FROM_VALE_KRADENS_COTTAGE_WEST, CONDITION_ALWAYS, 104, 0, 344, FACING_NORTH, 0, 16, 0, 336, 368, 0 },
    { VALE_CAVE_MOUTH_FROM_VALE_CAVE_KITE, CONDITION_ALWAYS, 136, 0, 184, FACING_SOUTH, 0, 16, 0, 336, 368, 0 },
    { VALE_CAVE_MOUTH_FROM_VALE_KRADENS_COTTAGE_EAST, CONDITION_ALWAYS, 136, 0, 376, FACING_NORTH, 0, 16, 0, 336, 368, 0 },
    { MAP_TABLE_END },
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
    { MAP_TABLE_END },
};

struct SceneEntrance gValeCave_LogsEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 200, 0, 432, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_WEST, CONDITION_ALWAYS, 200, 0, 432, FACING_NORTH, 0, 64, 24, 416, 464, 0 },
    { VALE_CAVE_LOGS_NORTH_FROM_SOUTH, CONDITION_ALWAYS, 344, 0, 120, FACING_SOUTH, 0, 64, 24, 416, 464, 0 },
    { VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_EAST, CONDITION_ALWAYS, 120, 0, 264, FACING_NORTH, 0, 64, 24, 416, 464, 0 },
    { VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_SOUTH, CONDITION_ALWAYS, 536, 0, 776, FACING_NORTH, 0, 400, 440, 896, 800, 0 },
    { VALE_CAVE_LOGS_SOUTH_FROM_NORTH, CONDITION_ALWAYS, 840, 0, 776, FACING_NORTH, 0, 400, 440, 896, 800, 0 },
    { MAP_TABLE_END },
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

struct ActorTemplate gValeCave_MouthActorTemplates[] = {
    { SPRITE_CRATES, CONDITION_ALWAYS, 7,
        PIXELS(232), 0, PIXELS(168), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CRATES, CONDITION_ALWAYS, 7,
        PIXELS(232), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CRATES, CONDITION_ALWAYS, 7,
        PIXELS(248), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        PIXELS(192), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        PIXELS(208), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ActorTemplate gValeCave_KiteActorTemplates[] = {
    { JUPITER_DJINNI, FLAG_KITE, MOVEMENT_TYPE_STAND,
        PIXELS(792), 0, PIXELS(232), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_CRATES, CONDITION_ALWAYS, 7,
        PIXELS(344), 0, PIXELS(632), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CRATES, CONDITION_ALWAYS, 7,
        PIXELS(456), 0, PIXELS(664), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(376), 0, PIXELS(576), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CRATES, CONDITION_ALWAYS, 7,
        PIXELS(422), 0, PIXELS(206), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CRATES, CONDITION_ALWAYS, 7,
        PIXELS(440), 0, PIXELS(206), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        PIXELS(384), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        PIXELS(400), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { MAP_TABLE_END },
};

struct ActorTemplate gValeCave_LogsActorTemplates[] = {
    { JUPITER_DJINNI, FLAG_KITE, MOVEMENT_TYPE_STAND,
        PIXELS(792), 0, PIXELS(232), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(200), 0, PIXELS(288), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(312), 0, PIXELS(288), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        PIXELS(240), 0, PIXELS(360), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct Trigger gValeCave_TriggersOther[] = {
    { TRIGGERS_END },
};

struct Trigger gValeCave_MouthTriggers[] = {
    { TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveRoom) },
    { TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveRoom) },
    { TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveRoom) },
    { TRIGGERS_END },
};

struct Trigger gValeCave_KiteTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, VALE_CAVE_KITE_TO_VALE_CAVE_MOUTH },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, VALE_CAVE_KITE_WEST_TO_EAST },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, VALE_CAVE_KITE_WEST_TO_SOUTH },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_WEST },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, VALE_CAVE_KITE_EAST_TO_WEST },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, VALE_CAVE_KITE_EAST_TO_SOUTH },
    { TRIGGER_EXIT, 7, CONDITION_ALWAYS, VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_EAST },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, VALE_CAVE_KITE_SOUTH_TO_WEST },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, VALE_CAVE_KITE_SOUTH_TO_EAST },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_SOUTH },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RideLogEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RideLogWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 21, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopDownLedge) },
    { TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TouchRidingLog) },
    { TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TouchRidingLog) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 24, FLAG_VALE_CAVE_EAST_CRATES_SUNK, EVENT_SCRIPT(EventScript_PushEastCratesIntoHole) },
    { TRIGGER_STEP, 40, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteRunsOff) },
    { TRIGGER_STEP, 41, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteRunsBehindLedge) },
    { TRIGGER_STEP, 45, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { TRIGGER_STEP, 46, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { TRIGGER_STEP, 47, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { TRIGGER_STEP, 48, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { TRIGGER_STEP, 49, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { TRIGGER_STEP, 50, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { TRIGGER_STEP, 51, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { TRIGGER_STEP, 52, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { TRIGGER_STEP, 53, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { TRIGGER_STEP, 54, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { TRIGGER_STEP, 55, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { TRIGGER_STEP, 56, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { TRIGGER_TALK_HALTED, ACTOR_KITE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToKite) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_VALE_CAVE_HALT_GEM, FIND_ITEM(ITEM_HALT_GEM) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_VALE_CAVE_NUT, FIND_ITEM(ITEM_NUT) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RevealHiddenTile) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HideRevealedTile) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_CRATES, FLAG_VALE_CAVE_WEST_CRATES_SUNK, EVENT_SCRIPT(EventScript_MoveWestCratesIntoHole) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_CRATES, FLAG_VALE_CAVE_EAST_CRATES_SUNK, EVENT_SCRIPT(EventScript_MoveEastCratesIntoHole) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_KITE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HaltKite) },
    { TRIGGERS_END },
};
// clang-format on

struct Trigger gValeCave_LogsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_WEST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, VALE_CAVE_LOGS_NORTH_TO_SOUTH},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_EAST},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_SOUTH},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, VALE_CAVE_LOGS_SOUTH_TO_NORTH},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 20, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_PushLogOverEdge)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 21, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_PushLogOffLedge)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 22, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_PushLogNorth)},
    {TRIGGERS_END},
};
