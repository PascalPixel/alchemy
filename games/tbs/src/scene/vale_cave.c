/// @file
/// Vale Cave's overlay, which holds three rooms, each a scene of its own: the
/// way in, which the game still calls Vale, and two rooms of Vale Cave proper.
/// Kite, a Jupiter Djinni, hides in the second. No story flag is read here
/// and no message is shown: the cave is a side trip from the part of Vale
/// around Kraden's cottage, a run of Psynergy puzzles whose progress lives in
/// scene and place flags.
///
/// - Vale (SceneId_ValeCaveMouth), the way in, a place group of its
///   own (29). VALE_CAVE_MOUTH_TO_VALE_KRADEN_WEST and
///   VALE_CAVE_MOUTH_TO_VALE_KRADEN_EAST lead back to Kraden's part of Vale
///   (SceneId_ValeKraden), whose VALE_KRADEN_TO_VALE_CAVE_MOUTH_WEST and
///   VALE_KRADEN_TO_VALE_CAVE_MOUTH_EAST lead here;
///   VALE_CAVE_MOUTH_TO_VALE_CAVE_KITE goes on into the cave. Each exit plays
///   the exit sound first (Event_LeaveRoom). On arrival ValeCave_Initialize
///   fades the room in from the backdrop and clears FLAG_SHOW_LOCATION_NAME:
///   entering a new place group sets that flag, but this is still Vale.
/// - Vale Cave (SceneId_ValeCaveKite), one map in three parts, each
///   with its own entrances and camera box; walking south into a ledge's cells
///   hops the leader down it (Event_HopDownLedge). In the first part
///   (VALE_CAVE_KITE_FROM_VALE_CAVE_MOUTH to
///   VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_WEST) Reveal shows a hidden tile while
///   it lasts (Event_RevealHiddenTile). In Kite's part
///   (VALE_CAVE_KITE_EAST_FROM_WEST to
///   VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_EAST), coming near
///   makes Kite hop away (Event_KiteRunsOff) and then run behind a ledge
///   (Event_KiteRunsBehindLedge, setting FLAG_VALE_CAVE_KITE_BEHIND_LEDGE);
///   from then on twelve trigger cells send it to the matching point on the
///   ledge's far side as the leader walks round (Event_KiteKeepsAway), so it
///   cannot be reached until Halt stops it. A stopped Kite can be talked to and
///   joins the party (Event_TalkToKite), setting FLAG_KITE, which removes it
///   from the placements. The room's chests hold the Halt Gem, which gives
///   Halt, and a Nut. In the holes' part (VALE_CAVE_KITE_SOUTH_FROM_WEST to
///   VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_SOUTH) two stacks of
///   crates sink into holes in the floor once Move or a push leaves them over
///   one (FLAG_SCENE_TEMP_00 and _01), opening the way, and a log carries
///   the leader along the passage when walked into from the west or the east
///   (FLAG_VALE_CAVE_RIDING_LOG_EAST). Its exits lead back to the first room,
///   on to the third, and between its own parts.
/// - Vale Cave (SceneId_ValeCaveLogs), the inner room, with three
///   logs to push: one off a ledge (Event_PushLogOffLedge), one along the
///   ledge until it drops at its edge (Event_PushLogOverEdge) and one north
///   (Event_PushLogNorth), each clearing the cells it held. Its exits
///   lead back into the second room and between its own parts.
///
/// Vale Cave's two rooms are one place group (30), so their place flags
/// (0x301, 0x302) last while the party goes between them. On every arrival
/// ValeCave_ApplyEntryState rebuilds the cells of each puzzle already solved
/// and, once Kite has run behind its ledge, puts it back there.

#include "game_state.h"
#include "djinn_ids.h"
#include "version.h"
#include "engine_call.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/scene/vale_cave.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/runtime/runtime.h"

#include "field_obj.h"
#include "mapcopy.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "create_configured_overlay_object.h"
#include "staged_actor.h"
#include "select_overlay_data_by_runtime_selector.h"
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

/// The way in's placements (gValeCave_Placements1), numbered from
/// ACTOR_FIRST_PLACED: three stacks of crates (drawn as the crates of the
/// Bilibin Barricade in bilibin.c), then two logs (as in Kolima Forest), none
/// of which moves.
enum ValeCaveMouthActor {
  ACTOR_VALE_CAVE_MOUTH_FIRST_CRATES = ACTOR_FIRST_PLACED,
  ACTOR_VALE_CAVE_MOUTH_FIRST_LOG = ACTOR_FIRST_PLACED + 3,
  ACTOR_VALE_CAVE_MOUTH_SECOND_LOG
};

/// The second room's placements (gValeCave_Placements2), numbered from
/// ACTOR_FIRST_PLACED.
enum ValeCaveKiteActor {
  /// Kite, a Jupiter Djinni, placed until it joins (FLAG_KITE).
  ACTOR_KITE = ACTOR_FIRST_PLACED,
  /// The two stacks of crates in the holes' part, which sink
  /// into their holes: the western under Move, the eastern under Move or a
  /// push.
  ACTOR_WEST_CRATES,
  ACTOR_EAST_CRATES,
  /// The log the leader rides along the holes' part's passage, from one end
  /// to the other, drawn as the east-west log of Colosso's Lumber Water
  /// Crossing Area (colosseum_azart_match.c).
  ACTOR_RIDING_LOG,
  /// Two stacks of crates and two logs that stand in the first part.
  ACTOR_FIRST_STANDING_CRATES,
  ACTOR_SECOND_STANDING_CRATES,
  ACTOR_FIRST_STANDING_LOG,
  ACTOR_SECOND_STANDING_LOG
};

/// The third room's placements (gValeCave_Placements3), numbered from
/// ACTOR_FIRST_PLACED; its first row places Kite as the second room's does.
/// Its three logs are pushed, and lie as in Kolima Forest.
enum ValeCaveLogsActor {
  /// Lying north to south, pushed off the ledge.
  ACTOR_WEST_LOG = ACTOR_FIRST_PLACED + 1,
  ACTOR_EAST_LOG, ///< lying north to south, pushed along the ledge until it
                  ///< falls
  ACTOR_SOUTH_LOG ///< lying west to east, pushed north
};

#define OBJECT_SINK_FRAMES 68
#define OBJECT_SINK_STEP 0x8000
#define ACTOR_LANDING_TIMEOUT 60
#define FIRST_DJINNI_ROUTE_TRIGGER 45

/// The flag of Kite, set once it is caught.
#define FLAG_KITE (FLAG_DJINNI_0 + DJINNI_KITE)

// The cave's scene flags:
// - FLAG_SCENE_TEMP_00 and _01: in the second room, the western and eastern
//   crates sunk into their holes, or, in Kite's part of it, its first and
//   second escape.
// - FLAG_SCENE_TEMP_02 to _05: in the third room, the log pushed off the
//   ledge, the log pushed over its edge, and the log slid north, ending on the
//   northern or the southern row (row 20).

// The cave's place flags. The second and third rooms are one place group, so
// these last while the party goes between them; the first room is another.
/// The riding log of the second room is at the eastern end of its passage.
#define FLAG_VALE_CAVE_RIDING_LOG_EAST 0x301
/// Kite has run behind the ledge and keeps to its far side.
#define FLAG_VALE_CAVE_KITE_BEHIND_LEDGE 0x302

static void ValeCave_CenterActorOnCell(s32 actorId, s32 column, s32 row);
static void ValeCave_ApplyEntryState(void);

#include "common/effect_create.h"
#include "common/effect_spawn.h"
#include "common/object_staged_actor.h"

/// In Vale Cave's second room, talking to Kite once Halt has stopped it: Kite,
/// a Jupiter Djinni, joins the party.
///
/// Scene_TalkToActor selects this EVENT_TALK_HALTED event while Kite is the
/// paused actor. Djinn_JoinParty gives the Jupiter Djinni Kite and sets
/// FLAG_KITE, so
/// neither room places Kite again.
static void Event_TalkToKite(void) {
  Djinn_JoinParty(ACTOR_KITE, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_KITE));
}

/// Waits for an actor to land, bounded by a one-second timeout.
static void ValeCave_WaitForActorLanding(struct FieldActor *actor) {
  s32 framesRemaining = ACTOR_LANDING_TIMEOUT;

  while (framesRemaining != 0) {
    Engine_TaskWait(1);
    framesRemaining--;
    if (actor->y.fixed <= actor->terrainHeight)
      break;
  }
  actor->y.fixed = actor->terrainHeight;
}

/// Turns Kite to face the party leader each frame.
///
/// Kite's update hook once it has run behind the ledge, set by
/// Event_KiteRunsBehindLedge and again by ValeCave_ApplyEntryState.
// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 38 to 36 bytes in tbs-en.
#ifdef UBFIX
static void ValeCave_FaceActorTowardLeader(union FieldObject *fieldObject) {
#else
static s32 ValeCave_FaceActorTowardLeader(union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
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
  ((void (*)(union FieldObject *))ValeCave_FaceActorTowardLeader)
#endif

/// Selects entrance positions and camera bounds for the current cave room.
const struct SceneEntrance *ValeCave_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_ValeCaveMouth) {
    return gValeCave_Entrances1;
  }
  if (scene == (s32)&SceneId_ValeCaveKite) {
    return gValeCave_Entrances2;
  }
  if (scene == (s32)&SceneId_ValeCaveLogs) {
    return gValeCave_Entrances3;
  }
  return gValeCave_EntrancesOther;
}

const struct SceneRegion *ValeCave_GetRegions(void) { return NULL; }

/// Returns the shared packed exits for all cave rooms.
const u32 *ValeCave_GetExits(void) { return gValeCave_Exits; }

/// Selects the current room's placements: Kite, and the crates and logs.
const struct ScenePlacement *ValeCave_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_ValeCaveMouth) {
    return gValeCave_Placements1;
  }
  if (scene == (s32)&SceneId_ValeCaveKite) {
    return gValeCave_Placements2;
  }
  if (scene == (s32)&SceneId_ValeCaveLogs) {
    return gValeCave_Placements3;
  }
  return gValeCave_Placements1;
}

/// In Vale Cave's second room, Reveal ends and the hidden tile is hidden again.
///
/// Reached by Reveal's PSYNERGY_PHASE_EFFECT_ENDS anywhere in the second room.
static void Event_HideRevealedTile(void) {
  Map_CopyCells(16, 15, 1, 1, 15, 15);
}

/// In Vale Cave's second room, Reveal shows a hidden tile.
///
/// Reached by Reveal's PSYNERGY_PHASE_EFFECT_ACTS anywhere in the second room;
/// copies cell (16, 17) over cell (15, 15) until Event_HideRevealedTile puts it
/// back.
static void Event_RevealHiddenTile(void) {
  Map_CopyCells(16, 17, 1, 1, 15, 15);
}

/// Sinks a stack of crates into its hole, spawning a particle each frame around
/// the opening.
static inline void
ValeCave_SinkObjectWithParticles(s32 actorId, s32 baseX, s32 baseZ,
                                 struct EffectOptions *particleOptions) {
  u32 frame;

  for (frame = 0; frame < OBJECT_SINK_FRAMES; frame++) {
    s32 particleX = (((u32)(Engine_RandomNext() * 17) >> 16) << 16) + baseX;
    s32 particleZ = (((u32)(Engine_RandomNext() * 14) >> 16) << 16) + baseZ;

    Effect_Spawn(particleX, 0, particleZ, 0, 0, 0,
                 EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, particleOptions);
    Object_GetById(actorId)->y.fixed -= OBJECT_SINK_STEP;
    Engine_EventWait(1);
  }
}

/// In Vale Cave's second room, Move on the western crates: once they are over
/// their hole, Isaac steps aside and they sink into the floor in a cloud of
/// dust.
///
/// Reached by Move on ACTOR_WEST_CRATES in the second room, until
/// FLAG_SCENE_TEMP_00 is set. Over their hole (column 23) it sets
/// FLAG_SCENE_TEMP_00 and makes the hole's cells walkable.
static void Event_MoveWestCratesIntoHole(void) {
  struct EffectOptions options;
  struct EffectOptions *particleOptions;
  struct FieldActor *leftEffect;
  struct FieldActor *rightEffect;

  Engine_EventBegin();
  if (Object_GetById(ACTOR_WEST_CRATES)->x.fixed >> 20 == 23) {
    // Move the leader aside before the crates start sinking.
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 360, 664);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 10);
    Object_GetById(ACTOR_WEST_CRATES)->x.fixed += PIXELS(2);
    leftEffect = OverlayObject_PrepareObject(
        Object_GetById(ACTOR_WEST_CRATES)->x.fixed, 0,
        Object_GetById(ACTOR_WEST_CRATES)->z.fixed + PIXELS(52), 241);
    rightEffect = OverlayObject_PrepareObject(
        Object_GetById(ACTOR_WEST_CRATES)->x.fixed + PIXELS(16), 0,
        Object_GetById(ACTOR_WEST_CRATES)->z.fixed + PIXELS(52), 241);
    Object_GetById(ACTOR_WEST_CRATES)->motionFlags = 0;
    // Surround the sinking crates with one particle per frame.
    particleOptions = &options;
    particleOptions->startScaleX = 0x9999;
    particleOptions->startScaleY = 0x9999;
    particleOptions->palette = 7;
    Engine_AudioPlayCue(SE_216);
    ValeCave_SinkObjectWithParticles(ACTOR_WEST_CRATES, PIXELS(368),
                                     PIXELS(624), particleOptions);
    Map_CopyCellAttributes(23, 41, 1, 1, 23, 39);
    Object_GetById(ACTOR_WEST_CRATES)->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT;
    Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
    Object_GetById(ACTOR_WEST_CRATES)->y.fixed = -PIXELS(8);
    Engine_ActorSetAnimation(ACTOR_WEST_CRATES, 2);
    Engine_ObjectDispatchRelease(leftEffect);
    Engine_ObjectDispatchRelease(rightEffect);
    Engine_EventWait(30);
  }
  Engine_EventEnd();
}

/// In Vale Cave's second room, Move on the eastern crates: once they are over
/// their hole, they sink into the floor.
///
/// Reached by Move on ACTOR_EAST_CRATES in the second room, until
/// FLAG_SCENE_TEMP_01 is set, and from Event_PushEastCratesIntoHole. Over their
/// hole (column 27) it sets FLAG_SCENE_TEMP_01 and makes the hole's cells
/// walkable.
static void Event_MoveEastCratesIntoHole(void) {
  struct EffectOptions options;
  struct EffectOptions *particleOptions;
  struct FieldActor *leftEffect;
  struct FieldActor *rightEffect;

  Engine_EventBegin();
  if (Object_GetById(ACTOR_EAST_CRATES)->x.fixed >> 20 == 27) {
    leftEffect = OverlayObject_PrepareObject(
        Object_GetById(ACTOR_EAST_CRATES)->x.fixed - PIXELS(8), 0,
        Object_GetById(ACTOR_EAST_CRATES)->z.fixed + PIXELS(52), 241);
    rightEffect = OverlayObject_PrepareObject(
        Object_GetById(ACTOR_EAST_CRATES)->x.fixed + PIXELS(8), 0,
        Object_GetById(ACTOR_EAST_CRATES)->z.fixed + PIXELS(52), 241);
    // XXX: likely a slip copied from Event_MoveWestCratesIntoHole, because it
    // clears the western crates' motion flags rather than these crates'.
    Object_GetById(ACTOR_WEST_CRATES)->motionFlags = 0;
    // Surround the sinking crates with one particle per frame.
    particleOptions = &options;
    particleOptions->startScaleX = 0x9999;
    particleOptions->startScaleY = 0x9999;
    particleOptions->palette = 7;
    Engine_AudioPlayCue(SE_216);
    ValeCave_SinkObjectWithParticles(ACTOR_EAST_CRATES, PIXELS(432),
                                     PIXELS(656), particleOptions);
    Map_CopyCellAttributes(31, 39, 2, 1, 27, 41);
    Object_GetById(ACTOR_EAST_CRATES)->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT;
    Engine_GameFlagSet(FLAG_SCENE_TEMP_01);
    Object_GetById(ACTOR_EAST_CRATES)->y.fixed = -PIXELS(8);
    Engine_ActorSetAnimation(ACTOR_EAST_CRATES, 2);
    Engine_ObjectDispatchRelease(leftEffect);
    Engine_ObjectDispatchRelease(rightEffect);
    Engine_EventWait(30);
  }
  Engine_EventEnd();
}

/// In Vale Cave's second room, pushing the eastern crates: they move a cell,
/// and sink if they are then over their hole.
///
/// Reached by walking into cell code 24 in the second room, until
/// FLAG_SCENE_TEMP_01 is set.
static void Event_PushEastCratesIntoHole(void) {
  Engine_EventBegin();
  StagedActor_AdvancePair();
  Engine_EventEnd();
  Event_MoveEastCratesIntoHole();
}

/// In Vale Cave's second room, walking south off a ledge: Isaac hops down if
/// the cell below is free.
///
/// Reached by walking south into cell code 21 in the second room. Returns 1
/// when the leader hopped, 0 when the landing cell is blocked.
static s32 Event_HopDownLedge(s32 unusedArgument) {

  union FieldCoordinate landingPosition[3];
  struct FieldActor *leader;
  u8 *motionFlags;
  s32 savedMotionFlags;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  motionFlags = &leader->motionFlags;
  savedMotionFlags = *motionFlags;
#if TBS_PATCH_1
  landingPosition[0].fixed = (leader->x.fixed & -0x100000) + 0x80000;
  landingPosition[1].fixed = leader->y.fixed;
  landingPosition[2].fixed = (leader->z.fixed & -0x100000) + 0x280000;
#else
  landingPosition[0].fixed = leader->x.fixed;
  landingPosition[1].fixed = leader->y.fixed;
  landingPosition[2].fixed = leader->z.fixed + 0x200000;
#endif
  // Keep the normal motion flags while staging the hop.
  if (FieldObject_CheckMovementCollision(
          leader, (struct FieldPosition *)landingPosition) == 0) {
    Engine_EventBegin();
    Object_SetMode(leader, 6);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(SE_JUMP);
    Object_SetMode(leader, 7);
    leader->speed = 0x30000;
    leader->acceleration = 0x20000;
    leader->velocityY = 0x40000;
    *motionFlags &= 126;
    Engine_ActorSetSpriteFlags(leader, 0);
    Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, landingPosition[0].part.pixel,
                              landingPosition[2].part.pixel);
    Object_SetMode(leader, 6);
    Engine_ActorSetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
    *motionFlags = savedMotionFlags;
    Engine_EventEnd();
    return 1;
  }
  return 0;
}

/// In Kite's part of Vale Cave's second room, coming near Kite: the camera
/// finds it; it notices the party and hops away up the ledge.
///
/// Reached by stepping on cell code 40 in the second room until Kite is
/// caught (FLAG_KITE); runs once, setting FLAG_SCENE_TEMP_00.
static void Event_KiteRunsOff(void) {

  struct FieldActor *djinni;

  if (GameFlag_IsSet(FLAG_SCENE_TEMP_00) == 0) {
    GameFlag_Set(FLAG_SCENE_TEMP_00);
    Engine_EventBegin();
    Camera_SetSpeed(Q16_ONE, 0x2000);
    Engine_CameraFollowActor(ACTOR_KITE, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(60);
    Actor_FaceDirection(ACTOR_KITE, FACING_NORTH, 20);
    Actor_SetAttachedEffect(ACTOR_KITE,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_ActorRunRepeatedMotion(ACTOR_KITE, 2);
    Engine_EventWait(20);
    Actor_SetSpeed(ACTOR_KITE, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_KITE, 792, 248);
    Engine_AudioPlayCue(SE_JUMP);
    djinni = Actor_Get(ACTOR_KITE);
    djinni->velocityY = 0x80000;
    Actor_WalkToAndWait(ACTOR_KITE, 792, 280);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_KITE, FACING_NORTH, 20);
    Engine_EventWait(30);
    Engine_EventEnd();
  }
}

/// In Kite's part of Vale Cave's second room, coming near again: Kite runs
/// behind the ledge and turns to watch the party.
///
/// Reached by stepping on cell code 41 in the second room until Kite is
/// caught, once Event_KiteRunsOff has run. Sets FLAG_SCENE_TEMP_01 and
/// FLAG_VALE_CAVE_KITE_BEHIND_LEDGE, and gives Kite the update hook
/// ValeCave_FaceActorTowardLeader.
static void Event_KiteRunsBehindLedge(void) {

  struct FieldActor *djinni;

  if (GameFlag_IsSet(FLAG_SCENE_TEMP_00) != 0) {
    if (GameFlag_IsSet(FLAG_SCENE_TEMP_01) == 0) {
      GameFlag_Set(FLAG_SCENE_TEMP_01);
      GameFlag_Set(FLAG_VALE_CAVE_KITE_BEHIND_LEDGE);
      Engine_EventBegin();
      Actor_SetAttachedEffect(ACTOR_KITE,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Engine_ActorRunRepeatedMotion(ACTOR_KITE, 2);
      Engine_EventWait(20);
      Actor_SetSpeed(ACTOR_KITE, PIXELS(2), PIXELS(1));
      Actor_WalkToAndWait(ACTOR_KITE, 760, 280);
      Actor_WalkToAndWait(ACTOR_KITE, 760, 312);
      Actor_WalkToAndWait(ACTOR_KITE, 792, 312);
      Engine_EventWait(10);
      Actor_FaceDirection(ACTOR_KITE, FACING_NORTH, 20);
      djinni = Actor_Get(ACTOR_KITE);
      djinni->update = VALE_CAVE_FACE_LEADER_CALLBACK;
      Engine_EventEnd();
    }
  }
}

/// In Kite's part of Vale Cave's second room, walking around the ledge: Kite
/// keeps to the far side of it, unless Halt has stopped it.
///
/// Reached by stepping on cell codes 45 to 56 in the second room until Kite is
/// caught, once FLAG_VALE_CAVE_KITE_BEHIND_LEDGE is set; each code sends Kite
/// to its row of gValeCave_WalkTargets at the leader's speed.
static void Event_KiteKeepsAway(void) {
  struct FieldActor *djinni;
  struct FieldActor *leader;
  s32 routeIndex;
  const u8 *walkTargetBytes;
  s32 targetZOffset;
  struct EventWork *event;

  event = gEventWork;
  if (Engine_GameFlagIsSet(FLAG_VALE_CAVE_KITE_BEHIND_LEDGE) != 0) {
    if (gGameState.pausedObject != ACTOR_KITE) {
      // Preserve the speed reads before resolving this route point.
      routeIndex = event->touchedTrigger;
      djinni = Object_GetById(ACTOR_KITE);
      leader = Object_GetById(ACTOR_PARTY_LEADER);
      djinni->speed = leader->speed;
      djinni = Object_GetById(ACTOR_KITE);
      leader = Object_GetById(ACTOR_PARTY_LEADER);
      djinni->acceleration = leader->acceleration;
      routeIndex -= FIRST_DJINNI_ROUTE_TRIGGER;
      // X/Z pairs occupy eight bytes. Plain pair indexing changes six
      // instruction words in the English build, so keep separate offsets.
      walkTargetBytes = (const u8 *)gValeCave_WalkTargets;
      routeIndex <<= 3;
      targetZOffset = routeIndex + sizeof(s32);
      Engine_ActorWalkTo(ACTOR_KITE,
                         *(const s32 *)(walkTargetBytes + routeIndex),
                         *(const s32 *)(walkTargetBytes + targetZOffset));
    }
  }
}

/// In Vale Cave's second room, Halt on Kite: it stops where it stands, and the
/// party can walk up to it and talk to it (Event_TalkToKite).
///
/// Reached by Halt on Kite in the second room.
static void Event_HaltKite(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_KITE, 0);
  Engine_EventEnd();
}

/// In Vale Cave's third room, pushing the eastern log along the ledge: at its
/// edge the log falls off, opening the way.
///
/// Reached by walking into cell code 20 in the third room. When the log
/// falls (column 17) it sets FLAG_SCENE_TEMP_03 and clears the cells it held.
static void Event_PushLogOverEdge(void) {
  struct FieldActor *movedLog;
  s32 stationaryMotion;
  s32 clearedRows;
  struct StagedActorProbe probe;

  Engine_EventBegin();
  if (StagedActor_FindClearPosition(&probe) != 0) {
    SceneActor_MoveAndRedraw(probe);
    // At the ledge's edge (column 17) the log slides west and drops.
    if ((probe.position.x >> 20) == 17) {
      Engine_ActorSetAnimation(probe.actorSlot, 3);
      stationaryMotion = 0;
      Object_GetById(probe.actorSlot)->motionFlags = stationaryMotion;
      movedLog = Object_GetById(probe.actorSlot);
      movedLog->bounce = stationaryMotion;
      Engine_ActorSetDestinationOffset(probe.actorSlot, -12, 0);
      Engine_ActorWaitForMove(probe.actorSlot);
      Engine_ActorSetAnimation(probe.actorSlot, 3);
      Engine_ActorSetSpritePriority(ACTOR_EAST_LOG, 3);
      Object_GetById(probe.actorSlot)->motionFlags =
          ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      Engine_ActorSetDestinationOffset(probe.actorSlot, -6, 0);
      ValeCave_WaitForActorLanding(Object_GetById(probe.actorSlot));
      Engine_ActorSetAnimation(probe.actorSlot, 8);
      {
        struct FieldActor *movedLog = Object_GetById(probe.actorSlot);

        movedLog->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      }
      // The cells it held on the ledge, and its landing, become open floor.
      clearedRows = 4;
      StagedActor_FillGridAttributeRectangle(0, (probe.position.x >> 20),
                                             ((probe.position.z >> 20) - 2), 1,
                                             clearedRows, stationaryMotion);
      StagedActor_FillGridAttributeRectangle(2, (probe.position.x >> 20),
                                             ((probe.position.z >> 20) - 2), 1,
                                             clearedRows, stationaryMotion);
      StagedActor_FillGridAttributeRectangle(2, 16, 18, 1, 2, stationaryMotion);
      StagedActor_FillGridAttributeRectangle(0, 16, 16, 1, clearedRows,
                                             stationaryMotion);
      Engine_GameFlagSet(FLAG_SCENE_TEMP_03);
      Engine_AudioPlayCue(SE_240);
    }
  }
  Engine_EventEnd();
}

/// In Vale Cave's third room, pushing the western log east: it falls off
/// the ledge, opening the way.
///
/// Reached by walking east into cell code 21 in the third room. Sets
/// FLAG_SCENE_TEMP_02 and clears the cells the log held.
static void Event_PushLogOffLedge(void) {

  struct FieldActor *pushedLog;
  s32 stationaryMotion;

  Engine_EventBegin();
  Engine_EventWait(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x1999);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 8);
  Engine_EventWait(15);
  Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 8, 0);
  Engine_EventWait(4);
  Event_PlayCue(SE_288);
  Event_PlayCue(SE_239);
  Actor_SetSpeed(ACTOR_WEST_LOG, 0x8000, 0x1999);
  Engine_ActorSetAnimation(ACTOR_WEST_LOG, 2);
  stationaryMotion = 0;
  Object_GetById(ACTOR_WEST_LOG)->motionFlags = stationaryMotion;
  pushedLog = Actor_Get(ACTOR_WEST_LOG);
  pushedLog->bounce = stationaryMotion;
  Engine_ActorSetDestinationOffset(ACTOR_WEST_LOG, 12, 0);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  Engine_ActorWaitForMove(ACTOR_WEST_LOG);
  Event_PlayCue(SE_288);
  Event_PlayCue(SE_213);
  Engine_ActorSetAnimation(ACTOR_WEST_LOG, 3);
  Object_GetById(ACTOR_WEST_LOG)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Engine_ActorSetDestinationOffset(ACTOR_WEST_LOG, 6, 0);
  ValeCave_WaitForActorLanding(Actor_Get(ACTOR_WEST_LOG));
  Engine_ActorSetAnimation(ACTOR_WEST_LOG, 8);
  Engine_ActorSetSpritePriority(ACTOR_WEST_LOG, 3);
  Object_GetById(ACTOR_WEST_LOG)->priorityFlags = 2;
  StagedActor_FillGridAttributeRectangle(0, 12, 16, 1, 4, 0);
  StagedActor_FillGridAttributeRectangle(0, 13, 16, 1, 4, 0);
  GameFlag_Set(FLAG_SCENE_TEMP_02);
  Event_PlayCue(SE_240);
  Engine_EventEnd();
}

/// In Vale Cave's third room, pushing the southern log north: it slides a cell,
/// opening the way.
///
/// Reached by walking into cell code 22 in the third room. Sets
/// FLAG_SCENE_TEMP_05 when it stops on row 20, else FLAG_SCENE_TEMP_04, and
/// then opens the cells above it.
static void Event_PushLogNorth(void) {
  struct StagedActorProbe probe;

  Engine_EventBegin();
  if (StagedActor_FindClearPosition(&probe) != 0) {
    SceneActor_MoveAndRedraw(probe);
    // Slide into the new cell before replacing the collision footprint.
    Engine_ActorSetAnimation(ACTOR_SOUTH_LOG, 3);
    Actor_SetSpeed(ACTOR_SOUTH_LOG, 0x4000, 0x8000);
    Engine_ActorSetDestinationOffset(ACTOR_SOUTH_LOG, 0, -16);
    Engine_EventWait(45);
    Engine_AudioPlayCue(SE_240);
    Engine_ActorSetAnimation(ACTOR_SOUTH_LOG, 8);
    {
      struct FieldActor *slidingLog = Object_GetById(ACTOR_SOUTH_LOG);
      s32 clearedRows = 2;
      s32 emptyAttributes = 0;

      slidingLog->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      StagedActor_FillGridAttributeRectangle(
          0, 13, (probe.position.z >> 20) - 1, 4, clearedRows, emptyAttributes);
    }
    if ((probe.position.z >> 20) == 20) {
      Engine_GameFlagSet(FLAG_SCENE_TEMP_05);
    } else {
      Engine_GameFlagSet(FLAG_SCENE_TEMP_04);
      {

        Map_CopyCellAttributes(14, 17, 2, 1, 14, 16);
        Map_CopyCellAttributes(14, 13, 1, 1, 14, 15);
      }
    }
  }
  Engine_EventEnd();
}

/// Selects the current room's touch, Psynergy and search bindings.
const struct SceneEvent *ValeCave_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_ValeCaveMouth) {
    return gValeCave_Events1;
  }
  if (scene == (s32)&SceneId_ValeCaveKite) {
    return gValeCave_Events2;
  }
  if (scene == (s32)&SceneId_ValeCaveLogs) {
    return gValeCave_Events3;
  }
  return gValeCave_EventsOther;
}

/// Arriving in any of the three rooms: chooses the entry transition and
/// restores the room's puzzle state.
///
/// The scene's initialize entry. The first room, Vale, fades in from the
/// backdrop and clears FLAG_SHOW_LOCATION_NAME, so the place name is not shown
/// again; the two rooms of Vale Cave open with the window transition.
s32 ValeCave_Initialize(void) {
  s32 *request = &gEventWork->startTransition;

  *request =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gGameState.scene == (s32)&SceneId_ValeCaveMouth) {
    *request = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Engine_TaskWait(1);
    Engine_ActorSetSpritePriority(ACTOR_VALE_CAVE_MOUTH_FIRST_LOG, 3);
    Engine_ActorSetSpritePriority(ACTOR_VALE_CAVE_MOUTH_SECOND_LOG, 3);
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  }
  ValeCave_ApplyEntryState();
  return 0;
}

/// In Vale, the cave's way in (SceneId_ValeCaveMouth), each exit plays the exit
/// sound before leaving.
///
/// Reached by stepping on cell codes 1 to 3 in the first room; takes the exit
/// of the same number (VALE_CAVE_MOUTH_TO_VALE_KRADEN_WEST,
/// VALE_CAVE_MOUTH_TO_VALE_CAVE_KITE or VALE_CAVE_MOUTH_TO_VALE_KRADEN_EAST).
static void Event_LeaveRoom(void) {
  s32 exitId = gEventWork->touchedTrigger;

  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_EventRequestExit(exitId);
}

/// Rebuilds the passage cells around the riding log's saved end.
static void ValeCave_UpdateRidingLogPassage(void) {
  Map_CopyCellAttributes(0, 34, 13, 3, 23, 34);

  if (Engine_GameFlagIsSet(FLAG_VALE_CAVE_RIDING_LOG_EAST) != 0) {
    ValeCave_CenterActorOnCell(ACTOR_RIDING_LOG, 35, 35);
    Map_CopyCellAttributes(24, 34, 1, 3, 23, 34);
  } else {
    ValeCave_CenterActorOnCell(ACTOR_RIDING_LOG, 23, 35);
    Map_CopyCellAttributes(24, 34, 1, 3, 35, 34);
  }
}

#include "common/object_pairmove.h"

/// In Vale Cave's second room, walking east onto the riding log: it carries
/// the leader to the eastern end of the passage.
///
/// Reached by walking east into cell code 20 in the second room. Sets
/// FLAG_VALE_CAVE_RIDING_LOG_EAST.
static void Event_RideLogEast(void) {

  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_RIDING_LOG, 112, 0);
  SceneActor_PlacePairAtOffset(ACTOR_RIDING_LOG, 80, 0);
  Engine_GameFlagSet(FLAG_VALE_CAVE_RIDING_LOG_EAST);
  Engine_TaskWait(2);
  ValeCave_UpdateRidingLogPassage();
  Engine_AudioPlayCue(SE_289);
}

/// In Vale Cave's second room, walking west onto the riding log: it carries
/// the leader back to the western end of the passage.
///
/// Reached by walking west into cell code 21 in the second room. Clears
/// FLAG_VALE_CAVE_RIDING_LOG_EAST.
static void Event_RideLogWest(void) {

  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_RIDING_LOG, -112, 0);
  SceneActor_PlacePairAtOffset(ACTOR_RIDING_LOG, -80, 0);
  Engine_GameFlagClear(FLAG_VALE_CAVE_RIDING_LOG_EAST);
  Engine_TaskWait(2);
  ValeCave_UpdateRidingLogPassage();
  Engine_AudioPlayCue(SE_289);
}

/// Centers an actor on a map cell and gives it underfoot priority.
static void ValeCave_CenterActorOnCell(s32 actorId, s32 column, s32 row) {
  struct FieldActor *actor = Object_GetById(actorId);

  if (actor != 0) {
    Engine_ActorSetSpritePriority(actorId, 3);
    actor->terrainId = 2;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actor->x.fixed = (column << 20) + 0x80000;
    actor->z.fixed = (row << 20) + 0x80000;
  }
}

/// Restores, on entry, Kite behind its ledge and the crates and logs of the
/// puzzles already solved.
///
/// Called by ValeCave_Initialize. In Kite's part of the second room
/// (VALE_CAVE_KITE_EAST_FROM_WEST to VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_EAST),
/// while Kite is free and FLAG_VALE_CAVE_KITE_BEHIND_LEDGE
/// is set, it sets FLAG_SCENE_TEMP_00 (and _01 from
/// VALE_CAVE_KITE_EAST_FROM_WEST) and puts Kite back behind the ledge.
static void ValeCave_ApplyEntryState(void) {
  s32 southLogOnRow20;

  // Each room reuses the temporary flags for its own puzzle state.
  if (gGameState.scene == (s32)&SceneId_ValeCaveKite) {
    switch (gGameState.entrance) {
    case VALE_CAVE_KITE_FROM_VALE_CAVE_MOUTH:
    case VALE_CAVE_KITE_WEST_FROM_EAST:
    case VALE_CAVE_KITE_WEST_FROM_SOUTH:
    case VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_WEST:
      Engine_ActorSetSpritePriority(ACTOR_SECOND_STANDING_LOG, 3);
      Engine_ActorSetSpritePriority(ACTOR_SECOND_STANDING_CRATES, 3);
      OverlayObject_CreateConfiguredObject(PIXELS(120), 0, PIXELS(232), 223);
      break;
    case VALE_CAVE_KITE_EAST_FROM_WEST:
    case VALE_CAVE_KITE_EAST_FROM_SOUTH:
    case VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_EAST:
      if (Engine_GameFlagIsSet(FLAG_KITE) != 0) {
        break;
      }
      if (Engine_GameFlagIsSet(FLAG_VALE_CAVE_KITE_BEHIND_LEDGE) == 0) {
        break;
      }
      Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
      if (gGameState.entrance == VALE_CAVE_KITE_EAST_FROM_WEST) {
        Engine_GameFlagSet(FLAG_SCENE_TEMP_01);
      }
      Engine_TaskWait(1);
      if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
        break;
      }
      // Kite waits behind the ledge, watching the leader.
      Actor_SetPosition(ACTOR_KITE, PIXELS(792), PIXELS(280));
      Object_GetById(ACTOR_KITE)->update = VALE_CAVE_FACE_LEADER_CALLBACK;
      break;
    case VALE_CAVE_KITE_SOUTH_FROM_WEST:
    case VALE_CAVE_KITE_SOUTH_FROM_EAST:
    case VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_SOUTH:
      // The holes' part: the riding log's passage, and the holes already
      // filled.
      OverlayObject_CreateConfiguredObject(PIXELS(642), 0, PIXELS(552), 20);
      Map_CopyCellAttributes(23, 34, 13, 3, 0, 34);
      ValeCave_UpdateRidingLogPassage();
      if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_00) != 0) {
        Map_CopyCellAttributes(23, 41, 1, 1, 23, 39);
      }
      if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_01) != 0) {
        Map_CopyCellAttributes(31, 39, 2, 1, 27, 41);
      }
      break;
    }
  } else if (gGameState.scene == (s32)&SceneId_ValeCaveLogs) {
    switch (gGameState.entrance) {
    case VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_WEST:
    case VALE_CAVE_LOGS_NORTH_FROM_SOUTH:
    case VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_EAST:
      // Each log already pushed leaves open floor; else its footprint
      // blocks its cells again.
      if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_02) != 0) {
        StagedActor_FillGridAttributeRectangle(0, 12, 16, 1, 4, 0);
        StagedActor_FillGridAttributeRectangle(0, 13, 16, 1, 4, 0);
      } else {
        FieldScene_RedrawActorFootprint(ACTOR_WEST_LOG);
      }
      if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_03) != 0) {
        StagedActor_FillGridAttributeRectangle(2, 16, 16, 1, 4, 0);
        StagedActor_FillGridAttributeRectangle(0, 16, 16, 1, 4, 0);
      } else {
        FieldScene_RedrawActorFootprint(ACTOR_EAST_LOG);
      }
      southLogOnRow20 = Engine_GameFlagIsSet(FLAG_SCENE_TEMP_05);
      if (southLogOnRow20 != 0) {
        StagedActor_FillGridAttributeRectangle(0, 13, 19, 4, 2, 0);
      } else if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_04) != 0) {
        StagedActor_FillGridAttributeRectangle(0, 13, 15, 4, 2,
                                               southLogOnRow20);
        Map_CopyCellAttributes(14, 17, 2, 1, 14, 16);
        Map_CopyCellAttributes(14, 13, 1, 1, 14, 15);
      } else {
        FieldScene_RedrawActorFootprint(ACTOR_SOUTH_LOG);
        Engine_ActorSetSpritePriority(ACTOR_SOUTH_LOG, 3);
      }
      break;
    }
  }
}

/// In Vale Cave's second room, touching the riding log restarts its
/// animation.
///
/// Reached by stepping on cell code 20 or 21 in the second room.
static void Event_TouchRidingLog(void) {
  Engine_ActorSetAnimation(ACTOR_RIDING_LOG, 1);
  Engine_ActorSetAnimation(ACTOR_RIDING_LOG, 2);
}

s32 gEffectScript0[] = {
    SC_WAIT(1),      SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),      SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),      SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

// Pixel step by direction: X in the upper halfword, Z in the lower.
// UB: negative pixel steps cannot be shifted as signed values.
#ifdef UBFIX
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((u32)(dz) & 0xffff))
#else
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))
#endif

s32 gStagedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gStagedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

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

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

struct SceneEntrance gValeCave_EntrancesOther[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gValeCave_Entrances1[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, 16, 0, 336, 368, 0 },
    { VALE_CAVE_MOUTH_FROM_VALE_KRADEN_WEST, CONDITION_ALWAYS, 104, 0, 344, FACING_NORTH, 0, 16, 0, 336, 368, 0 },
    { VALE_CAVE_MOUTH_FROM_VALE_CAVE_KITE, CONDITION_ALWAYS, 136, 0, 184, FACING_SOUTH, 0, 16, 0, 336, 368, 0 },
    { VALE_CAVE_MOUTH_FROM_VALE_KRADEN_EAST, CONDITION_ALWAYS, 136, 0, 376, FACING_NORTH, 0, 16, 0, 336, 368, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gValeCave_Entrances2[] = {
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

struct SceneEntrance gValeCave_Entrances3[] = {
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
    SCENE_EXIT(VALE_CAVE_MOUTH_TO_VALE_KRADEN_WEST, ValeKraden, VALE_KRADEN_FROM_VALE_CAVE_MOUTH_WEST),
    SCENE_EXIT(VALE_CAVE_MOUTH_TO_VALE_CAVE_KITE, ValeCaveKite, VALE_CAVE_KITE_FROM_VALE_CAVE_MOUTH),
    SCENE_EXIT(VALE_CAVE_MOUTH_TO_VALE_KRADEN_EAST, ValeKraden, VALE_KRADEN_FROM_VALE_CAVE_MOUTH_EAST),
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

struct ScenePlacement gValeCave_Placements1[] = {
    { 0xf1, CONDITION_ALWAYS, 7,
        PIXELS(232), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf1, CONDITION_ALWAYS, 7,
        PIXELS(232), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf1, CONDITION_ALWAYS, 7,
        PIXELS(248), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(192), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(208), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeCave_Placements2[] = {
    { 0x5d, FLAG_KITE, ACTOR_STAND,
        PIXELS(792), 0, PIXELS(232), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf1, CONDITION_ALWAYS, 7,
        PIXELS(344), 0, PIXELS(632), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf1, CONDITION_ALWAYS, 7,
        PIXELS(456), 0, PIXELS(664), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(376), 0, PIXELS(576), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf1, CONDITION_ALWAYS, 7,
        PIXELS(422), 0, PIXELS(206), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf1, CONDITION_ALWAYS, 7,
        PIXELS(440), 0, PIXELS(206), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(384), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(400), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeCave_Placements3[] = {
    { 0x5d, FLAG_KITE, ACTOR_STAND,
        PIXELS(792), 0, PIXELS(232), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(200), 0, PIXELS(288), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(312), 0, PIXELS(288), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(240), 0, PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gValeCave_EventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeCave_Events1[] = {
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveRoom) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveRoom) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveRoom) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeCave_Events2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, VALE_CAVE_KITE_TO_VALE_CAVE_MOUTH },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, VALE_CAVE_KITE_WEST_TO_EAST },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, VALE_CAVE_KITE_WEST_TO_SOUTH },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_WEST },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, VALE_CAVE_KITE_EAST_TO_WEST },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, VALE_CAVE_KITE_EAST_TO_SOUTH },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_EAST },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, VALE_CAVE_KITE_SOUTH_TO_WEST },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, VALE_CAVE_KITE_SOUTH_TO_EAST },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_SOUTH },
    { TOUCH_WALKING_INTO(FACING_EAST), 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RideLogEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RideLogWest) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopDownLedge) },
    { EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TouchRidingLog) },
    { EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TouchRidingLog) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 24, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Event_PushEastCratesIntoHole) },
    { EVENT_TOUCH, 40, FLAG_KITE, EVENT_SCRIPT(Event_KiteRunsOff) },
    { EVENT_TOUCH, 41, FLAG_KITE, EVENT_SCRIPT(Event_KiteRunsBehindLedge) },
    { EVENT_TOUCH, 45, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { EVENT_TOUCH, 46, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { EVENT_TOUCH, 47, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { EVENT_TOUCH, 48, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { EVENT_TOUCH, 49, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { EVENT_TOUCH, 50, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { EVENT_TOUCH, 51, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { EVENT_TOUCH, 52, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { EVENT_TOUCH, 53, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { EVENT_TOUCH, 54, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { EVENT_TOUCH, 55, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { EVENT_TOUCH, 56, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { EVENT_TALK_HALTED, ACTOR_KITE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToKite) },
    { SEARCH(SEARCH_CHEST), 100, FLAG_VALE_CAVE_HALT_GEM, FIND_ITEM(ITEM_HALT_GEM) },
    { SEARCH(SEARCH_CHEST), 101, FLAG_VALE_CAVE_NUT, FIND_ITEM(ITEM_NUT) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RevealHiddenTile) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HideRevealedTile) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_CRATES, FLAG_SCENE_TEMP_00, EVENT_SCRIPT(Event_MoveWestCratesIntoHole) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_CRATES, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Event_MoveEastCratesIntoHole) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_KITE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HaltKite) },
    { SCENE_EVENTS_END },
};
// clang-format on

struct SceneEvent gValeCave_Events3[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_WEST},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, VALE_CAVE_LOGS_NORTH_TO_SOUTH},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_EAST},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_SOUTH},
    {EVENT_EXIT, 5, CONDITION_ALWAYS, VALE_CAVE_LOGS_SOUTH_TO_NORTH},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 20, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_PushLogOverEdge)},
    {TOUCH_WALKING_INTO(FACING_EAST), 21, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_PushLogOffLedge)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 22, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_PushLogNorth)},
    {SCENE_EVENTS_END},
};
