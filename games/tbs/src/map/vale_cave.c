/// @file
/// Vale Cave's overlay, which holds three rooms, each a map of its own: the
/// way in, which the game still calls Vale, and two rooms of Vale Cave proper.
/// Kite, a Jupiter Djinni, hides in the second. No story flag is read here
/// and no message is shown: the cave is a side trip from the part of Vale
/// around Kraden's cottage, a run of Psynergy puzzles whose progress lives in
/// map and map section flags.
///
/// - Vale (MapId_ValeCaveMouth), the way in, a map section of its
///   own (29). VALE_CAVE_MOUTH_TO_VALE_KRADEN_WEST and
///   VALE_CAVE_MOUTH_TO_VALE_KRADEN_EAST lead back to Kraden's part of Vale
///   (MapId_ValeKraden), whose VALE_KRADEN_TO_VALE_CAVE_MOUTH_WEST and
///   VALE_KRADEN_TO_VALE_CAVE_MOUTH_EAST lead here;
///   VALE_CAVE_MOUTH_TO_VALE_CAVE_KITE goes on into the cave. Each warp plays
///   the warp sound first (EventScript_LeaveRoom). On arrival
///   ValeCave_Initialize fades the room in from the backdrop and clears
///   FLAG_SHOW_LOCATION_NAME: entering a new map section sets that flag, but
///   this is still Vale.
/// - Vale Cave (MapId_ValeCaveKite), one map in three parts, each
///   with its own warp destinations and camera box; walking south into a
///   ledge's cells hops the leader down it (EventScript_HopDownLedge). In the
///   first part (VALE_CAVE_KITE_FROM_VALE_CAVE_MOUTH to
///   VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_WEST) Reveal shows a hidden cell while
///   it lasts (EventScript_RevealHiddenTile). In Kite's part
///   (VALE_CAVE_KITE_EAST_FROM_WEST to
///   VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_EAST), coming near
///   makes Kite hop away (EventScript_KiteRunsOff) and then run behind a ledge
///   (EventScript_KiteRunsBehindLedge, setting
///   FLAG_VALE_CAVE_KITE_BEHIND_LEDGE); from then on twelve metatile behaviors,
///   45 to 56 on the 23 cells round the ledge, each send it to its point of
///   gValeCave_WalkTargets on the ledge's far side as the leader walks round
///   (EventScript_KiteKeepsAway), so it cannot be reached until Halt stops it
///   (https://alchemy.pascalpixel.com/places/vale-cave-kite/kite-ledge.gif:
///   the leader walking the ring, twice the size, the cells of those metatile
///   behaviors outlined and numbered from 0 by behavior, the walk targets drawn
///   with the same numbers, and Kite boxed). A stopped Kite can be talked to
///   and joins the party (EventScript_TalkToKite), setting FLAG_KITE, which
///   removes it from the object event templates. The room's chests hold the
///   Halt Gem, which gives Halt, and a Nut. In the holes' part
///   (VALE_CAVE_KITE_SOUTH_FROM_WEST to
///   VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_SOUTH) two stacks of crates sink into
///   holes in the floor once Move or a push leaves them over one
///   (FLAG_MAP_TEMP_00 and _01), opening the way, and a log carries the leader
///   along the passage when walked into from the west or the east
///   (FLAG_VALE_CAVE_RIDING_LOG_EAST). Its warps lead back to the first room,
///   on to the third, and between its own parts.
/// - Vale Cave (MapId_ValeCaveLogs), the inner room, with three
///   logs to push: one off a ledge (EventScript_PushLogOffLedge), one along the
///   ledge until it drops at its edge (EventScript_PushLogOverEdge) and one
///   north (EventScript_PushLogNorth), each clearing the cells it held. Its
///   warps lead back into the second room and between its own parts.
///
/// Vale Cave's two rooms are one map section (30), so their map section flags
/// (0x301, 0x302) last while the party goes between them. On every arrival
/// ValeCave_ApplyEntryState rebuilds the cells of each puzzle already solved
/// and, once Kite has run behind its ledge, puts it back there.
///
/// Open questions:
/// - XXX (no evidence): why ValeCave_ApplyEntryState creates still objects of
///   sprite 223 (spr_223.png, a wooden crate) at (120, 232) and SPRITE_CHEST
///   at (642, 552) in the second room that show nothing: the sprites alone
///   hold neither (render probe vale-cave), the crates seen there are the
///   layout's, no chest is drawn at the second, and neither object is solid or
///   a target (OBJ_EVENT_NOT_TARGETED); Vault creates the same kind of object
///   on its well (vault_revisit.c), and nothing in the ROM says what they were
///   meant to show.

#include "game_state.h"
#include "djinn_ids.h"
#include "version.h"
#include "engine_call.h"
#include "heap_slots.h"
#include "unit/import.h"
#include "unit/map/vale_cave.h"
#include "unit/map/common/effect_spawn.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/runtime/runtime.h"

#include "object_event.h"
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
#include "map_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "flags.h"

#include "fixed_math.h"
#include "songs.h"
#include "audio_engine_symbols.h"

/// The way in's object event templates (gValeCave_MouthObjectEventTemplates),
/// numbered from LOCALID_FIRST_PLACED: three stacks of crates (drawn as the
/// crates of the Bilibin Barricade in bilibin.c), then two logs (as in Kolima
/// Forest), none of which moves.
enum ValeCaveMouthLocalId {
  /// ★ Named from what the player sees: three wooden crates stacked by the
  /// cave's mouth (its render)
  LOCALID_VALE_CAVE_MOUTH_FIRST_CRATES = LOCALID_FIRST_PLACED,
  /// ★ Named from what the player sees: two logs lying west to east beside the
  /// crates (its render)
  LOCALID_VALE_CAVE_MOUTH_FIRST_LOG = LOCALID_FIRST_PLACED + 3,
  /// ★ Named from what the player sees: two logs lying west to east beside the
  /// crates (its render)
  LOCALID_VALE_CAVE_MOUTH_SECOND_LOG
};

/// The second room's object event templates
/// (gValeCave_KiteObjectEventTemplates), numbered from LOCALID_FIRST_PLACED.
enum ValeCaveKiteLocalId {
  /// Kite, a Jupiter Djinni, placed until it joins (FLAG_KITE).
  /// ★ Named from the game: "Kite" (MsgDjinnName)
  LOCALID_KITE = LOCALID_FIRST_PLACED,
  /// The two stacks of crates in the holes' part, which sink
  /// into their holes: the western under Move, the eastern under Move or a
  /// push.
  /// ★ Named from what the player sees: a stack of wooden crates beside the
  /// water (its render)
  LOCALID_WEST_CRATES,
  /// ★ Named from what the player sees: a stack of wooden crates beside the
  /// water (its render)
  LOCALID_EAST_CRATES,
  /// The log the leader rides along the holes' part's passage, from one end
  /// to the other, drawn as the east-west log of Colosso's Lumber Water
  /// Crossing Area (colosseum_azart_match.c).
  /// ★ Named from what the player sees: a yellow log floating in the water (its
  /// render)
  LOCALID_RIDING_LOG,
  /// Two stacks of crates and two logs that stand in the first part.
  /// ★ Named from what the player sees: two stacks of wooden crates by the
  /// room's east door (its render)
  LOCALID_FIRST_STANDING_CRATES,
  /// ★ Named from what the player sees: two stacks of wooden crates by the
  /// room's east door (its render)
  LOCALID_SECOND_STANDING_CRATES,
  /// ★ Named from what the player sees: two logs lying west to east beside the
  /// crates (its render)
  LOCALID_FIRST_STANDING_LOG,
  /// ★ Named from what the player sees: two logs lying west to east beside the
  /// crates (its render)
  LOCALID_SECOND_STANDING_LOG
};

/// The third room's object event templates
/// (gValeCave_LogsObjectEventTemplates), numbered from LOCALID_FIRST_PLACED;
/// its first row places Kite as the second room's does. Its three logs are
/// pushed, and lie as in Kolima Forest.
enum ValeCaveLogsLocalId {
  /// Lying north to south, pushed off the ledge.
  /// ★ Named from what the player sees: a log lying north to south west of the
  /// water (its render)
  LOCALID_WEST_LOG = LOCALID_FIRST_PLACED + 1,
  /// ★ Named from what the player sees: a log lying north to south east of the
  /// water (its render)
  LOCALID_EAST_LOG, ///< lying north to south, pushed along the ledge until it
                    ///< falls
  /// ★ Named from what the player sees: a log lying west to east south of the
  /// water (its render)
  LOCALID_SOUTH_LOG ///< lying west to east, pushed north
};

/// The first of the twelve metatile behaviors that send Kite round its ledge
/// (EventScript_KiteKeepsAway), one for each of gValeCave_WalkTargets.
#define VALE_CAVE_FIRST_KITE_TRIGGER_CELL 45

/// The flag of Kite, set once it is caught.
/// ★ Named from the game: "Kite" (MsgDjinnName)
#define FLAG_KITE (FLAG_DJINNI(DJINNI_KITE))

// The cave's map flags:
// - FLAG_MAP_TEMP_00 and _01: in the second room, the western and eastern
//   crates sunk into their holes, or, in Kite's part of it, its first and
//   second escape.
// - FLAG_MAP_TEMP_02 to _05: in the third room, the log pushed off the
//   ledge, the log pushed over its edge, and the log slid north, ending on the
//   northern or the southern row (row 20).

// The cave's map section flags. The second and third rooms are one map section,
// so these last while the party goes between them; the first room is another.
/// The riding log of the second room is at the eastern end of its passage.
/// ★ Named from what the player sees: the log floating in the holes' part
/// of the second room, which carries the leader along it (its render)
#define FLAG_VALE_CAVE_RIDING_LOG_EAST 0x301
/// Kite has run behind the ledge and keeps to its far side.
/// ★ Named from the game: "Kite" (MsgDjinnName)
#define FLAG_VALE_CAVE_KITE_BEHIND_LEDGE 0x302

static void ValeCave_CenterObjectEventOnCell(s32 localId, s32 column, s32 row);
static void ValeCave_ApplyEntryState(void);

#include "common/effect_create.h"
#include "common/effect_spawn.h"
#include "common/object_staged_object_event.h"

/// In Vale Cave's second room, talking to Kite once Halt has stopped it: Kite,
/// a Jupiter Djinni, joins the party.
///
/// Map_TalkToObjectEvent selects this MAP_EVENT_TALK_HALTED event while Kite is
/// the paused object event. Djinn_JoinParty gives the Jupiter Djinni Kite and
/// sets FLAG_KITE, so neither room places Kite again.
static void EventScript_TalkToKite(void) {
  Djinn_JoinParty(LOCALID_KITE, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_KITE));
}

/// Waits for an object event to land, bounded by a one-second timeout.
static void
ValeCave_WaitForObjectEventLanding(struct ObjectEvent *objectEvent) {
  s32 framesRemaining = 60;

  while (framesRemaining != 0) {
    Engine_TaskWait(1);
    framesRemaining--;
    if (objectEvent->y.fixed <= objectEvent->groundHeight)
      break;
  }
  objectEvent->y.fixed = objectEvent->groundHeight;
}

/// Turns Kite to face the party leader each frame.
///
/// Kite's update hook once it has run behind the ledge, set by
/// EventScript_KiteRunsBehindLedge and again by ValeCave_ApplyEntryState.
// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 38 to 36 bytes in tbs-en.
#ifdef UBFIX
static void
ValeCave_FaceObjectEventTowardLeader(union FieldObject *fieldObject) {
#else
static s32
ValeCave_FaceObjectEventTowardLeader(union FieldObject *fieldObject) {
#endif
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  s32 distanceZ = leader->z.fixed - objectEvent->z.fixed;
  s32 distanceX = leader->x.fixed - objectEvent->x.fixed;

  objectEvent->facing = ArcTan2(distanceZ, distanceX);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

#ifdef UBFIX
#define VALE_CAVE_FACE_LEADER_CALLBACK ValeCave_FaceObjectEventTowardLeader
#else
#define VALE_CAVE_FACE_LEADER_CALLBACK                                         \
  ((void (*)(union FieldObject *))ValeCave_FaceObjectEventTowardLeader)
#endif

/// Selects warp destination positions and camera bounds for the current cave
/// room.
const struct MapWarpDest *ValeCave_GetWarpDests(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_ValeCaveMouth) {
    return gValeCave_MouthWarpDests;
  }
  if (mapId == (s32)&MapId_ValeCaveKite) {
    return gValeCave_KiteWarpDests;
  }
  if (mapId == (s32)&MapId_ValeCaveLogs) {
    return gValeCave_LogsWarpDests;
  }
  return gValeCave_WarpDestsOther;
}

const struct MapWarpRegion *ValeCave_GetRegions(void) { return NULL; }

/// Returns the shared packed warps for all cave rooms.
const u32 *ValeCave_GetWarps(void) { return gValeCave_Warps; }

/// Selects the current room's object event templates: Kite, and the crates and
/// logs.
const struct ObjectEventTemplate *ValeCave_GetObjectEventTemplates(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_ValeCaveMouth) {
    return gValeCave_MouthObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_ValeCaveKite) {
    return gValeCave_KiteObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_ValeCaveLogs) {
    return gValeCave_LogsObjectEventTemplates;
  }
  return gValeCave_MouthObjectEventTemplates;
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
ValeCave_SinkObjectWithParticles(s32 localId, s32 baseX, s32 baseZ,
                                 struct EffectOptions *particleOptions) {
  u32 frame;

  for (frame = 0; frame < 68; frame++) {
    s32 particleX = (((u32)(Engine_RandomNext() * 17) >> 16) << 16) + baseX;
    s32 particleZ = (((u32)(Engine_RandomNext() * 14) >> 16) << 16) + baseZ;

    FieldEffect_Spawn(particleX, 0, particleZ, 0, 0, 0,
                      EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
                      particleOptions);
    ObjectEvent_GetById(localId)->y.fixed -= PIXELS(1) / 2;
    Engine_ScriptWait(1);
  }
}

/// In Vale Cave's second room, Move on the western crates: once they are over
/// their hole, Isaac steps aside and they sink into the floor in a cloud of
/// dust.
///
/// Reached by Move on LOCALID_WEST_CRATES in the second room, until
/// FLAG_MAP_TEMP_00 is set. Over their hole (column 23) it sets
/// FLAG_MAP_TEMP_00 and makes the hole's cells walkable.
static void EventScript_MoveWestCratesIntoHole(void) {
  struct EffectOptions options;
  struct EffectOptions *particleOptions;
  struct ObjectEvent *leftEffect;
  struct ObjectEvent *rightEffect;

  Engine_ScriptStart();
  if (ObjectEvent_GetById(LOCALID_WEST_CRATES)->x.fixed >> 20 == 23) {
    // Move the leader aside before the crates start sinking.
    ObjectEvent_MoveToAndWait(LOCALID_PARTY_LEADER, 360, 664);
    Engine_ObjectEventFaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 10);
    ObjectEvent_GetById(LOCALID_WEST_CRATES)->x.fixed += PIXELS(2);
    leftEffect = ObjectEvent_PrepareObject(
        ObjectEvent_GetById(LOCALID_WEST_CRATES)->x.fixed, 0,
        ObjectEvent_GetById(LOCALID_WEST_CRATES)->z.fixed + PIXELS(52), 241);
    rightEffect = ObjectEvent_PrepareObject(
        ObjectEvent_GetById(LOCALID_WEST_CRATES)->x.fixed + PIXELS(16), 0,
        ObjectEvent_GetById(LOCALID_WEST_CRATES)->z.fixed + PIXELS(52), 241);
    ObjectEvent_GetById(LOCALID_WEST_CRATES)->motionFlags = 0;
    // Surround the sinking crates with one particle per frame.
    particleOptions = &options;
    particleOptions->startScaleX = 3 * Q16_ONE / 5;
    particleOptions->startScaleY = 3 * Q16_ONE / 5;
    particleOptions->palette = 7;
    Engine_AudioPlayCue(SE_216);
    ValeCave_SinkObjectWithParticles(LOCALID_WEST_CRATES, PIXELS(368),
                                     PIXELS(624), particleOptions);
    MapGrid_CopyCellAttributes(23, 41, 1, 1, 23, 39);
    ObjectEvent_GetById(LOCALID_WEST_CRATES)->priorityFlags |=
        OBJ_EVENT_PRIORITY_UNDERFOOT;
    Engine_GameFlagSet(FLAG_MAP_TEMP_00);
    ObjectEvent_GetById(LOCALID_WEST_CRATES)->y.fixed = -PIXELS(8);
    Engine_ObjectEventSetAnimation(LOCALID_WEST_CRATES, 2);
    Engine_ObjectDispatchRelease(leftEffect);
    Engine_ObjectDispatchRelease(rightEffect);
    Engine_ScriptWait(30);
  }
  Engine_ScriptEnd();
}

/// In Vale Cave's second room, Move on the eastern crates: once they are over
/// their hole, they sink into the floor.
///
/// Reached by Move on LOCALID_EAST_CRATES in the second room, until
/// FLAG_MAP_TEMP_01 is set, and from EventScript_PushEastCratesIntoHole. Over
/// their hole (column 27) it sets FLAG_MAP_TEMP_01 and makes the hole's cells
/// walkable.
static void EventScript_MoveEastCratesIntoHole(void) {
  struct EffectOptions options;
  struct EffectOptions *particleOptions;
  struct ObjectEvent *leftEffect;
  struct ObjectEvent *rightEffect;

  Engine_ScriptStart();
  if (ObjectEvent_GetById(LOCALID_EAST_CRATES)->x.fixed >> 20 == 27) {
    leftEffect = ObjectEvent_PrepareObject(
        ObjectEvent_GetById(LOCALID_EAST_CRATES)->x.fixed - PIXELS(8), 0,
        ObjectEvent_GetById(LOCALID_EAST_CRATES)->z.fixed + PIXELS(52), 241);
    rightEffect = ObjectEvent_PrepareObject(
        ObjectEvent_GetById(LOCALID_EAST_CRATES)->x.fixed + PIXELS(8), 0,
        ObjectEvent_GetById(LOCALID_EAST_CRATES)->z.fixed + PIXELS(52), 241);
    // XXX: likely a slip copied from EventScript_MoveWestCratesIntoHole,
    // because it clears the western crates' motion flags rather than these
    // crates'.
    ObjectEvent_GetById(LOCALID_WEST_CRATES)->motionFlags = 0;
    // Surround the sinking crates with one particle per frame.
    particleOptions = &options;
    particleOptions->startScaleX = 3 * Q16_ONE / 5;
    particleOptions->startScaleY = 3 * Q16_ONE / 5;
    particleOptions->palette = 7;
    Engine_AudioPlayCue(SE_216);
    ValeCave_SinkObjectWithParticles(LOCALID_EAST_CRATES, PIXELS(432),
                                     PIXELS(656), particleOptions);
    MapGrid_CopyCellAttributes(31, 39, 2, 1, 27, 41);
    ObjectEvent_GetById(LOCALID_EAST_CRATES)->priorityFlags |=
        OBJ_EVENT_PRIORITY_UNDERFOOT;
    Engine_GameFlagSet(FLAG_MAP_TEMP_01);
    ObjectEvent_GetById(LOCALID_EAST_CRATES)->y.fixed = -PIXELS(8);
    Engine_ObjectEventSetAnimation(LOCALID_EAST_CRATES, 2);
    Engine_ObjectDispatchRelease(leftEffect);
    Engine_ObjectDispatchRelease(rightEffect);
    Engine_ScriptWait(30);
  }
  Engine_ScriptEnd();
}

/// In Vale Cave's second room, pushing the eastern crates: they move a cell,
/// and sink if they are then over their hole.
///
/// Reached by walking into metatile behavior 24 in the second room, until
/// FLAG_MAP_TEMP_01 is set.
static void EventScript_PushEastCratesIntoHole(void) {
  Engine_ScriptStart();
  PushedObject_AdvancePair();
  Engine_ScriptEnd();
  EventScript_MoveEastCratesIntoHole();
}

/// In Vale Cave's second room, walking south off a ledge: Isaac hops down if
/// the cell below is free.
///
/// Reached by walking south into metatile behavior 21 in the second room.
/// Returns 1 when the leader hopped, 0 when the landing cell is blocked.
static s32 EventScript_HopDownLedge(s32 unusedArgument) {

  union FieldCoordinate landingPosition[3];
  struct ObjectEvent *leader;
  u8 *motionFlags;
  s32 savedMotionFlags;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
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
    Engine_ScriptStart();
    ObjectEvent_SetMode(leader, 6);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(SE_JUMP);
    ObjectEvent_SetMode(leader, 7);
    leader->speed = PIXELS(3);
    leader->acceleration = PIXELS(2);
    leader->velocityY = PIXELS(4);
    *motionFlags &= 126;
    Engine_ObjectEventSetSpriteFlags(leader, 0);
    Engine_ObjectEventMoveToAndWait(LOCALID_PARTY_LEADER,
                                    landingPosition[0].part.pixel,
                                    landingPosition[2].part.pixel);
    ObjectEvent_SetMode(leader, 6);
    Engine_ObjectEventSetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
    *motionFlags = savedMotionFlags;
    Engine_ScriptEnd();
    return 1;
  }
  return 0;
}

/// In Kite's part of Vale Cave's second room, coming near Kite: the camera
/// finds it; it notices the party and hops away up the ledge.
///
/// Reached by stepping on metatile behavior 40 in the second room until Kite is
/// caught (FLAG_KITE); runs once, setting FLAG_MAP_TEMP_00.
static void EventScript_KiteRunsOff(void) {

  struct ObjectEvent *djinni;

  if (GameFlag_IsSet(FLAG_MAP_TEMP_00) == 0) {
    GameFlag_Set(FLAG_MAP_TEMP_00);
    Engine_ScriptStart();
    FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
    Engine_CameraFollowObjectEvent(LOCALID_KITE, 1);
    Engine_CameraWaitForMove();
    Engine_ScriptWait(60);
    ObjectEvent_FaceDirection(LOCALID_KITE, FACING_NORTH, 20);
    ObjectEvent_SetAttachedEffect(LOCALID_KITE, ATTACHED_EFFECT_IN_FRONT |
                                                    ATTACHED_EFFECT_ONCE);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_KITE, 2);
    Engine_ScriptWait(20);
    ObjectEvent_SetSpeed(LOCALID_KITE, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_WalkToAndWait(LOCALID_KITE, 792, 248);
    Engine_AudioPlayCue(SE_JUMP);
    djinni = ObjectEvent_Get(LOCALID_KITE);
    djinni->velocityY = PIXELS(8);
    ObjectEvent_WalkToAndWait(LOCALID_KITE, 792, 280);
    Engine_ScriptWait(20);
    ObjectEvent_FaceDirection(LOCALID_KITE, FACING_NORTH, 20);
    Engine_ScriptWait(30);
    Engine_ScriptEnd();
  }
}

/// In Kite's part of Vale Cave's second room, coming near again: Kite runs
/// behind the ledge and turns to watch the party.
///
/// Reached by stepping on metatile behavior 41 in the second room until Kite is
/// caught, once EventScript_KiteRunsOff has run. Sets FLAG_MAP_TEMP_01 and
/// FLAG_VALE_CAVE_KITE_BEHIND_LEDGE, and gives Kite the update hook
/// ValeCave_FaceObjectEventTowardLeader.
static void EventScript_KiteRunsBehindLedge(void) {

  struct ObjectEvent *djinni;

  if (GameFlag_IsSet(FLAG_MAP_TEMP_00) != 0) {
    if (GameFlag_IsSet(FLAG_MAP_TEMP_01) == 0) {
      GameFlag_Set(FLAG_MAP_TEMP_01);
      GameFlag_Set(FLAG_VALE_CAVE_KITE_BEHIND_LEDGE);
      Engine_ScriptStart();
      ObjectEvent_SetAttachedEffect(LOCALID_KITE, ATTACHED_EFFECT_IN_FRONT |
                                                      ATTACHED_EFFECT_ONCE);
      Engine_ObjectEventRunRepeatedMotion(LOCALID_KITE, 2);
      Engine_ScriptWait(20);
      ObjectEvent_SetSpeed(LOCALID_KITE, PIXELS(2), PIXELS(1));
      ObjectEvent_WalkToAndWait(LOCALID_KITE, 760, 280);
      ObjectEvent_WalkToAndWait(LOCALID_KITE, 760, 312);
      ObjectEvent_WalkToAndWait(LOCALID_KITE, 792, 312);
      Engine_ScriptWait(10);
      ObjectEvent_FaceDirection(LOCALID_KITE, FACING_NORTH, 20);
      djinni = ObjectEvent_Get(LOCALID_KITE);
      djinni->update = VALE_CAVE_FACE_LEADER_CALLBACK;
      Engine_ScriptEnd();
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
  struct ObjectEvent *djinni;
  struct ObjectEvent *leader;
  s32 routeIndex;
  const u8 *walkTargetBytes;
  s32 targetZOffset;
  struct EventWork *work;

  work = gFieldWork;
  if (Engine_GameFlagIsSet(FLAG_VALE_CAVE_KITE_BEHIND_LEDGE) != 0) {
    if (gGameState.pausedObject != LOCALID_KITE) {
      // Preserve the speed reads before resolving this route point.
      routeIndex = work->coordEvent;
      djinni = ObjectEvent_GetById(LOCALID_KITE);
      leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      djinni->speed = leader->speed;
      djinni = ObjectEvent_GetById(LOCALID_KITE);
      leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      djinni->acceleration = leader->acceleration;
      routeIndex -= VALE_CAVE_FIRST_KITE_TRIGGER_CELL;
      // X/Z pairs occupy eight bytes. Plain pair indexing changes six
      // instruction words in the English build, so keep separate offsets.
      walkTargetBytes = (const u8 *)gValeCave_WalkTargets;
      routeIndex <<= 3;
      targetZOffset = routeIndex + sizeof(s32);
      Engine_ObjectEventWalkTo(LOCALID_KITE,
                               *(const s32 *)(walkTargetBytes + routeIndex),
                               *(const s32 *)(walkTargetBytes + targetZOffset));
    }
  }
}

/// In Vale Cave's second room, Halt on Kite: it stops where it stands, and the
/// party can walk up to it and talk to it (EventScript_TalkToKite).
///
/// Reached by Halt on Kite in the second room.
static void EventScript_HaltKite(void) {
  Engine_ScriptStart();
  Engine_ObjectEventSetAnimation(LOCALID_KITE, 0);
  Engine_ScriptEnd();
}

/// In Vale Cave's third room, pushing the eastern log along the ledge: at its
/// edge the log falls off, opening the way.
///
/// Reached by walking into metatile behavior 20 in the third room. When the log
/// falls (column 17) it sets FLAG_MAP_TEMP_03 and clears the cells it held.
static void EventScript_PushLogOverEdge(void) {
  struct ObjectEvent *movedLog;
  s32 stationaryMotion;
  s32 clearedRows;
  struct PushedObjectProbe probe;

  Engine_ScriptStart();
  if (PushedObject_FindClearPosition(&probe) != 0) {
    ObjectEvent_MoveAndRedraw(probe);
    // At the ledge's edge (column 17) the log slides west and drops.
    if ((probe.position.x >> 20) == 17) {
      Engine_ObjectEventSetAnimation(probe.objectEventSlot, 3);
      stationaryMotion = 0;
      ObjectEvent_GetById(probe.objectEventSlot)->motionFlags =
          stationaryMotion;
      movedLog = ObjectEvent_GetById(probe.objectEventSlot);
      movedLog->bounce = stationaryMotion;
      Engine_ObjectEventSetDestinationOffset(probe.objectEventSlot, -12, 0);
      Engine_ObjectEventWaitForMove(probe.objectEventSlot);
      Engine_ObjectEventSetAnimation(probe.objectEventSlot, 3);
      Engine_ObjectEventSetSpritePriority(LOCALID_EAST_LOG, 3);
      ObjectEvent_GetById(probe.objectEventSlot)->motionFlags =
          OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
      Engine_ObjectEventSetDestinationOffset(probe.objectEventSlot, -6, 0);
      ValeCave_WaitForObjectEventLanding(
          ObjectEvent_GetById(probe.objectEventSlot));
      Engine_ObjectEventSetAnimation(probe.objectEventSlot, 8);
      {
        struct ObjectEvent *movedLog =
            ObjectEvent_GetById(probe.objectEventSlot);

        movedLog->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
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
      Engine_GameFlagSet(FLAG_MAP_TEMP_03);
      Engine_AudioPlayCue(SE_240);
    }
  }
  Engine_ScriptEnd();
}

/// In Vale Cave's third room, pushing the western log east: it falls off
/// the ledge, opening the way.
///
/// Reached by walking east into metatile behavior 21 in the third room. Sets
/// FLAG_MAP_TEMP_02 and clears the cells the log held.
static void EventScript_PushLogOffLedge(void) {

  struct ObjectEvent *pushedLog;
  s32 stationaryMotion;

  Engine_ScriptStart();
  Engine_ScriptWait(10);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 10);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_PUSH);
  Engine_ScriptWait(15);
  Engine_ObjectEventSetDestinationOffset(LOCALID_PARTY_LEADER, 8, 0);
  Engine_ScriptWait(4);
  Script_PlaySong(SE_STOP_SLOT3);
  Script_PlaySong(SE_HEAVY_MOVE);
  ObjectEvent_SetSpeed(LOCALID_WEST_LOG, PIXELS(1) / 2, PIXELS(1) / 10);
  Engine_ObjectEventSetAnimation(LOCALID_WEST_LOG, 2);
  stationaryMotion = 0;
  ObjectEvent_GetById(LOCALID_WEST_LOG)->motionFlags = stationaryMotion;
  pushedLog = ObjectEvent_Get(LOCALID_WEST_LOG);
  pushedLog->bounce = stationaryMotion;
  Engine_ObjectEventSetDestinationOffset(LOCALID_WEST_LOG, 12, 0);
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, 1);
  Engine_ObjectEventWaitForMove(LOCALID_WEST_LOG);
  Script_PlaySong(SE_STOP_SLOT3);
  Script_PlaySong(SE_HEAVY_STOP);
  Engine_ObjectEventSetAnimation(LOCALID_WEST_LOG, 3);
  ObjectEvent_GetById(LOCALID_WEST_LOG)->motionFlags =
      OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
  Engine_ObjectEventSetDestinationOffset(LOCALID_WEST_LOG, 6, 0);
  ValeCave_WaitForObjectEventLanding(ObjectEvent_Get(LOCALID_WEST_LOG));
  Engine_ObjectEventSetAnimation(LOCALID_WEST_LOG, 8);
  Engine_ObjectEventSetSpritePriority(LOCALID_WEST_LOG, 3);
  ObjectEvent_GetById(LOCALID_WEST_LOG)->priorityFlags =
      OBJ_EVENT_PRIORITY_UNDERFOOT;
  PushedObject_FillGridAttributeRectangle(0, 12, 16, 1, 4, 0);
  PushedObject_FillGridAttributeRectangle(0, 13, 16, 1, 4, 0);
  GameFlag_Set(FLAG_MAP_TEMP_02);
  Script_PlaySong(SE_240);
  Engine_ScriptEnd();
}

/// In Vale Cave's third room, pushing the southern log north: it slides a cell,
/// opening the way.
///
/// Reached by walking into metatile behavior 22 in the third room. Sets
/// FLAG_MAP_TEMP_05 when it stops on row 20, else FLAG_MAP_TEMP_04, and
/// then opens the cells above it.
static void EventScript_PushLogNorth(void) {
  struct PushedObjectProbe probe;

  Engine_ScriptStart();
  if (PushedObject_FindClearPosition(&probe) != 0) {
    ObjectEvent_MoveAndRedraw(probe);
    // Slide into the new cell before replacing the metatile behavior footprint.
    Engine_ObjectEventSetAnimation(LOCALID_SOUTH_LOG, 3);
    ObjectEvent_SetSpeed(LOCALID_SOUTH_LOG, PIXELS(1) / 4, PIXELS(1) / 2);
    Engine_ObjectEventSetDestinationOffset(LOCALID_SOUTH_LOG, 0, -16);
    Engine_ScriptWait(45);
    Engine_AudioPlayCue(SE_240);
    Engine_ObjectEventSetAnimation(LOCALID_SOUTH_LOG, 8);
    {
      struct ObjectEvent *slidingLog = ObjectEvent_GetById(LOCALID_SOUTH_LOG);
      s32 clearedRows = 2;
      s32 emptyAttributes = 0;

      slidingLog->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
      PushedObject_FillGridAttributeRectangle(
          0, 13, (probe.position.z >> 20) - 1, 4, clearedRows, emptyAttributes);
    }
    if ((probe.position.z >> 20) == 20) {
      Engine_GameFlagSet(FLAG_MAP_TEMP_05);
    } else {
      Engine_GameFlagSet(FLAG_MAP_TEMP_04);
      {

        MapGrid_CopyCellAttributes(14, 17, 2, 1, 14, 16);
        MapGrid_CopyCellAttributes(14, 13, 1, 1, 14, 15);
      }
    }
  }
  Engine_ScriptEnd();
}

/// Selects the current room's coord, Psynergy and bg events.
const struct MapEvent *ValeCave_GetEvents(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_ValeCaveMouth) {
    return gValeCave_MouthEvents;
  }
  if (mapId == (s32)&MapId_ValeCaveKite) {
    return gValeCave_KiteEvents;
  }
  if (mapId == (s32)&MapId_ValeCaveLogs) {
    return gValeCave_LogsEvents;
  }
  return gValeCave_EventsOther;
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
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gGameState.mapId == (s32)&MapId_ValeCaveMouth) {
    *request = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Engine_TaskWait(1);
    Engine_ObjectEventSetSpritePriority(LOCALID_VALE_CAVE_MOUTH_FIRST_LOG, 3);
    Engine_ObjectEventSetSpritePriority(LOCALID_VALE_CAVE_MOUTH_SECOND_LOG, 3);
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  }
  ValeCave_ApplyEntryState();
  return 0;
}

/// In Vale, the cave's way in (MapId_ValeCaveMouth), each warp plays the warp
/// sound before leaving.
///
/// Reached by stepping on metatile behaviors 1 to 3 in the first room; takes
/// the warp of the same number (VALE_CAVE_MOUTH_TO_VALE_KRADEN_WEST,
/// VALE_CAVE_MOUTH_TO_VALE_CAVE_KITE or VALE_CAVE_MOUTH_TO_VALE_KRADEN_EAST).
static void EventScript_LeaveRoom(void) {
  s32 warpNumber = gFieldWork->coordEvent;

  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_ScriptRequestWarp(warpNumber);
}

/// Rebuilds the passage cells around the riding log's saved end.
static void ValeCave_UpdateRidingLogPassage(void) {
  MapGrid_CopyCellAttributes(0, 34, 13, 3, 23, 34);

  if (Engine_GameFlagIsSet(FLAG_VALE_CAVE_RIDING_LOG_EAST) != 0) {
    ValeCave_CenterObjectEventOnCell(LOCALID_RIDING_LOG, 35, 35);
    MapGrid_CopyCellAttributes(24, 34, 1, 3, 23, 34);
  } else {
    ValeCave_CenterObjectEventOnCell(LOCALID_RIDING_LOG, 23, 35);
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

  Engine_AudioPlayCue(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_RIDING_LOG, 112, 0);
  ObjectEvent_PlacePairAtOffset(LOCALID_RIDING_LOG, 80, 0);
  Engine_GameFlagSet(FLAG_VALE_CAVE_RIDING_LOG_EAST);
  Engine_TaskWait(2);
  ValeCave_UpdateRidingLogPassage();
  Engine_AudioPlayCue(SONG_FADE_OUT_ACTION_SE);
}

/// In Vale Cave's second room, walking west onto the riding log: it carries
/// the leader back to the western end of the passage.
///
/// Reached by walking west into metatile behavior 21 in the second room. Clears
/// FLAG_VALE_CAVE_RIDING_LOG_EAST.
static void EventScript_RideLogWest(void) {

  Engine_AudioPlayCue(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_RIDING_LOG, -112, 0);
  ObjectEvent_PlacePairAtOffset(LOCALID_RIDING_LOG, -80, 0);
  Engine_GameFlagClear(FLAG_VALE_CAVE_RIDING_LOG_EAST);
  Engine_TaskWait(2);
  ValeCave_UpdateRidingLogPassage();
  Engine_AudioPlayCue(SONG_FADE_OUT_ACTION_SE);
}

/// Centers an object event on a map cell and gives it underfoot priority.
static void ValeCave_CenterObjectEventOnCell(s32 localId, s32 column, s32 row) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(localId);

  if (objectEvent != 0) {
    Engine_ObjectEventSetSpritePriority(localId, 3);
    objectEvent->layer = 2;
    objectEvent->priorityFlags |= OBJ_EVENT_PRIORITY_UNDERFOOT;
    objectEvent->x.fixed = (column << 20) + PIXELS(8);
    objectEvent->z.fixed = (row << 20) + PIXELS(8);
  }
}

/// Restores, on entry, Kite behind its ledge and the crates and logs of the
/// puzzles already solved.
///
/// Called by ValeCave_Initialize. In Kite's part of the second room
/// (VALE_CAVE_KITE_EAST_FROM_WEST to VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_EAST),
/// while Kite is free and FLAG_VALE_CAVE_KITE_BEHIND_LEDGE is set, it sets
/// FLAG_MAP_TEMP_00 (and _01 from VALE_CAVE_KITE_EAST_FROM_WEST) and puts
/// Kite back behind the ledge.
static void ValeCave_ApplyEntryState(void) {
  s32 southLogOnSouthernRow;

  // Each room reuses the temporary flags for its own puzzle state.
  if (gGameState.mapId == (s32)&MapId_ValeCaveKite) {
    switch (gGameState.warpDest) {
    case VALE_CAVE_KITE_FROM_VALE_CAVE_MOUTH:
    case VALE_CAVE_KITE_WEST_FROM_EAST:
    case VALE_CAVE_KITE_WEST_FROM_SOUTH:
    case VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_WEST:
      Engine_ObjectEventSetSpritePriority(LOCALID_SECOND_STANDING_LOG, 3);
      Engine_ObjectEventSetSpritePriority(LOCALID_SECOND_STANDING_CRATES, 3);
      // A still object of sprite 223 (spr_223.png, a wooden crate) that shows
      // nothing (an open question).
      ObjectEvent_CreateConfiguredObject(PIXELS(120), 0, PIXELS(232), 223);
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
      Engine_GameFlagSet(FLAG_MAP_TEMP_00);
      if (gGameState.warpDest == VALE_CAVE_KITE_EAST_FROM_WEST) {
        Engine_GameFlagSet(FLAG_MAP_TEMP_01);
      }
      Engine_TaskWait(1);
      if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
        break;
      }
      // Kite waits behind the ledge, watching the leader.
      ObjectEvent_SetPosition(LOCALID_KITE, PIXELS(792), PIXELS(280));
      ObjectEvent_GetById(LOCALID_KITE)->update =
          VALE_CAVE_FACE_LEADER_CALLBACK;
      break;
    case VALE_CAVE_KITE_SOUTH_FROM_WEST:
    case VALE_CAVE_KITE_SOUTH_FROM_EAST:
    case VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_SOUTH:
      // The holes' part: a still chest object that shows nothing (an open
      // question), the riding log's passage, and the holes already filled.
      ObjectEvent_CreateConfiguredObject(PIXELS(642), 0, PIXELS(552),
                                         SPRITE_CHEST);
      MapGrid_CopyCellAttributes(23, 34, 13, 3, 0, 34);
      ValeCave_UpdateRidingLogPassage();
      if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_00) != 0) {
        MapGrid_CopyCellAttributes(23, 41, 1, 1, 23, 39);
      }
      if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_01) != 0) {
        MapGrid_CopyCellAttributes(31, 39, 2, 1, 27, 41);
      }
      break;
    }
  } else if (gGameState.mapId == (s32)&MapId_ValeCaveLogs) {
    switch (gGameState.warpDest) {
    case VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_WEST:
    case VALE_CAVE_LOGS_NORTH_FROM_SOUTH:
    case VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_EAST:
      // Each log already pushed leaves open floor; else its footprint
      // blocks its cells again.
      if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_02) != 0) {
        PushedObject_FillGridAttributeRectangle(0, 12, 16, 1, 4, 0);
        PushedObject_FillGridAttributeRectangle(0, 13, 16, 1, 4, 0);
      } else {
        Map_RedrawObjectEventFootprint(LOCALID_WEST_LOG);
      }
      if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_03) != 0) {
        PushedObject_FillGridAttributeRectangle(2, 16, 16, 1, 4, 0);
        PushedObject_FillGridAttributeRectangle(0, 16, 16, 1, 4, 0);
      } else {
        Map_RedrawObjectEventFootprint(LOCALID_EAST_LOG);
      }
      southLogOnSouthernRow = Engine_GameFlagIsSet(FLAG_MAP_TEMP_05);
      if (southLogOnSouthernRow != 0) {
        PushedObject_FillGridAttributeRectangle(0, 13, 19, 4, 2, 0);
      } else if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_04) != 0) {
        PushedObject_FillGridAttributeRectangle(0, 13, 15, 4, 2,
                                                southLogOnSouthernRow);
        MapGrid_CopyCellAttributes(14, 17, 2, 1, 14, 16);
        MapGrid_CopyCellAttributes(14, 13, 1, 1, 14, 15);
      } else {
        Map_RedrawObjectEventFootprint(LOCALID_SOUTH_LOG);
        Engine_ObjectEventSetSpritePriority(LOCALID_SOUTH_LOG, 3);
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
  Engine_ObjectEventSetAnimation(LOCALID_RIDING_LOG, 1);
  Engine_ObjectEventSetAnimation(LOCALID_RIDING_LOG, 2);
}

// The lifetimes FieldEffect_Spawn gives its effects through gEffectScripts: an
// effect waits 23, 45 or 127 frames, then is put away and freed.
// FieldEffect_Spawn divides a change of scale by the loop count in each one's
// fourth word, so that the effect reaches its scale as its time runs out.

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
/// one for each metatile behavior from VALE_CAVE_FIRST_KITE_TRIGGER_CELL on.
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
    gValeCave_ShortEffectScript,
    gValeCave_MediumEffectScript,
    gValeCave_LongEffectScript,
};

struct MapWarpDest gValeCave_WarpDestsOther[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gValeCave_MouthWarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, 16, 0, 336, 368, 0 },
    { VALE_CAVE_MOUTH_FROM_VALE_KRADEN_WEST, CONDITION_ALWAYS, 104, 0, 344, FACING_NORTH, 0, 16, 0, 336, 368, 0 },
    { VALE_CAVE_MOUTH_FROM_VALE_CAVE_KITE, CONDITION_ALWAYS, 136, 0, 184, FACING_SOUTH, 0, 16, 0, 336, 368, 0 },
    { VALE_CAVE_MOUTH_FROM_VALE_KRADEN_EAST, CONDITION_ALWAYS, 136, 0, 376, FACING_NORTH, 0, 16, 0, 336, 368, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gValeCave_KiteWarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
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

struct MapWarpDest gValeCave_LogsWarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 200, 0, 432, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_WEST, CONDITION_ALWAYS, 200, 0, 432, FACING_NORTH, 0, 64, 24, 416, 464, 0 },
    { VALE_CAVE_LOGS_NORTH_FROM_SOUTH, CONDITION_ALWAYS, 344, 0, 120, FACING_SOUTH, 0, 64, 24, 416, 464, 0 },
    { VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_EAST, CONDITION_ALWAYS, 120, 0, 264, FACING_NORTH, 0, 64, 24, 416, 464, 0 },
    { VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_SOUTH, CONDITION_ALWAYS, 536, 0, 776, FACING_NORTH, 0, 400, 440, 896, 800, 0 },
    { VALE_CAVE_LOGS_SOUTH_FROM_NORTH, CONDITION_ALWAYS, 840, 0, 776, FACING_NORTH, 0, 400, 440, 896, 800, 0 },
    { MAP_TABLE_END },
};

u32 gValeCave_Warps[] = {
    MAP_WARPS(ValeCaveMouth),
    MAP_WARP(VALE_CAVE_MOUTH_TO_VALE_KRADEN_WEST, ValeKraden, VALE_KRADEN_FROM_VALE_CAVE_MOUTH_WEST),
    MAP_WARP(VALE_CAVE_MOUTH_TO_VALE_CAVE_KITE, ValeCaveKite, VALE_CAVE_KITE_FROM_VALE_CAVE_MOUTH),
    MAP_WARP(VALE_CAVE_MOUTH_TO_VALE_KRADEN_EAST, ValeKraden, VALE_KRADEN_FROM_VALE_CAVE_MOUTH_EAST),
    MAP_WARPS(ValeCaveKite),
    MAP_WARP(VALE_CAVE_KITE_TO_VALE_CAVE_MOUTH, ValeCaveMouth, VALE_CAVE_MOUTH_FROM_VALE_CAVE_KITE),
    MAP_WARP(VALE_CAVE_KITE_WEST_TO_EAST, ValeCaveKite, VALE_CAVE_KITE_EAST_FROM_WEST),
    MAP_WARP(VALE_CAVE_KITE_WEST_TO_SOUTH, ValeCaveKite, VALE_CAVE_KITE_SOUTH_FROM_WEST),
    MAP_WARP(VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_WEST, ValeCaveLogs, VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_WEST),
    MAP_WARP(VALE_CAVE_KITE_EAST_TO_WEST, ValeCaveKite, VALE_CAVE_KITE_WEST_FROM_EAST),
    MAP_WARP(VALE_CAVE_KITE_EAST_TO_SOUTH, ValeCaveKite, VALE_CAVE_KITE_SOUTH_FROM_EAST),
    MAP_WARP(VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_EAST, ValeCaveLogs, VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_EAST),
    MAP_WARP(VALE_CAVE_KITE_SOUTH_TO_WEST, ValeCaveKite, VALE_CAVE_KITE_WEST_FROM_SOUTH),
    MAP_WARP(VALE_CAVE_KITE_SOUTH_TO_EAST, ValeCaveKite, VALE_CAVE_KITE_EAST_FROM_SOUTH),
    MAP_WARP(VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_SOUTH, ValeCaveLogs, VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_SOUTH),
    MAP_WARPS(ValeCaveLogs),
    MAP_WARP(VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_WEST, ValeCaveKite, VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_WEST),
    MAP_WARP(VALE_CAVE_LOGS_NORTH_TO_SOUTH, ValeCaveLogs, VALE_CAVE_LOGS_SOUTH_FROM_NORTH),
    MAP_WARP(VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_EAST, ValeCaveKite, VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_EAST),
    MAP_WARP(VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_SOUTH, ValeCaveKite, VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_SOUTH),
    MAP_WARP(VALE_CAVE_LOGS_SOUTH_TO_NORTH, ValeCaveLogs, VALE_CAVE_LOGS_NORTH_FROM_SOUTH),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gValeCave_MouthObjectEventTemplates[] = {
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

struct ObjectEventTemplate gValeCave_KiteObjectEventTemplates[] = {
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
        PIXELS(440), 0, PIXELS(206), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        PIXELS(384), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        PIXELS(400), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gValeCave_LogsObjectEventTemplates[] = {
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

struct MapEvent gValeCave_EventsOther[] = {
    { MAP_EVENTS_END },
};

struct MapEvent gValeCave_MouthEvents[] = {
    { MAP_EVENT_COORD, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveRoom) },
    { MAP_EVENT_COORD, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveRoom) },
    { MAP_EVENT_COORD, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveRoom) },
    { MAP_EVENTS_END },
};

struct MapEvent gValeCave_KiteEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, VALE_CAVE_KITE_TO_VALE_CAVE_MOUTH },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, VALE_CAVE_KITE_WEST_TO_EAST },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, VALE_CAVE_KITE_WEST_TO_SOUTH },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_WEST },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, VALE_CAVE_KITE_EAST_TO_WEST },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, VALE_CAVE_KITE_EAST_TO_SOUTH },
    { MAP_EVENT_WARP, 7, CONDITION_ALWAYS, VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_EAST },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, VALE_CAVE_KITE_SOUTH_TO_WEST },
    { MAP_EVENT_WARP, 9, CONDITION_ALWAYS, VALE_CAVE_KITE_SOUTH_TO_EAST },
    { MAP_EVENT_WARP, 10, CONDITION_ALWAYS, VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_SOUTH },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RideLogEast) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RideLogWest) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 21, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopDownLedge) },
    { MAP_EVENT_COORD, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TouchRidingLog) },
    { MAP_EVENT_COORD, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TouchRidingLog) },
    { MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 24, FLAG_MAP_TEMP_01, EVENT_SCRIPT(EventScript_PushEastCratesIntoHole) },
    { MAP_EVENT_COORD, 40, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteRunsOff) },
    { MAP_EVENT_COORD, 41, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteRunsBehindLedge) },
    { MAP_EVENT_COORD, 45, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { MAP_EVENT_COORD, 46, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { MAP_EVENT_COORD, 47, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { MAP_EVENT_COORD, 48, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { MAP_EVENT_COORD, 49, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { MAP_EVENT_COORD, 50, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { MAP_EVENT_COORD, 51, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { MAP_EVENT_COORD, 52, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { MAP_EVENT_COORD, 53, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { MAP_EVENT_COORD, 54, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { MAP_EVENT_COORD, 55, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { MAP_EVENT_COORD, 56, FLAG_KITE, EVENT_SCRIPT(EventScript_KiteKeepsAway) },
    { MAP_EVENT_TALK_HALTED, LOCALID_KITE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToKite) },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_VALE_CAVE_HALT_GEM, FIND_ITEM(ITEM_HALT_GEM) },
    { BG_EVENT(BG_EVENT_CHEST), 101, FLAG_VALE_CAVE_NUT, FIND_ITEM(ITEM_NUT) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RevealHiddenTile) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HideRevealedTile) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_WEST_CRATES, FLAG_MAP_TEMP_00, EVENT_SCRIPT(EventScript_MoveWestCratesIntoHole) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_EAST_CRATES, FLAG_MAP_TEMP_01, EVENT_SCRIPT(EventScript_MoveEastCratesIntoHole) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_HALT), LOCALID_KITE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HaltKite) },
    { MAP_EVENTS_END },
};
// clang-format on

struct MapEvent gValeCave_LogsEvents[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS,
     VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_WEST},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS, VALE_CAVE_LOGS_NORTH_TO_SOUTH},
    {MAP_EVENT_WARP, 3, CONDITION_ALWAYS,
     VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_EAST},
    {MAP_EVENT_WARP, 4, CONDITION_ALWAYS,
     VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_SOUTH},
    {MAP_EVENT_WARP, 5, CONDITION_ALWAYS, VALE_CAVE_LOGS_SOUTH_TO_NORTH},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 20, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_PushLogOverEdge)},
    {COORD_EVENT_WALKING_INTO(FACING_EAST), 21, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_PushLogOffLedge)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 22, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_PushLogNorth)},
    {MAP_EVENTS_END},
};
