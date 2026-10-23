/// @file
/// Goma Cave Entrance (MapId_GomaCaveEntrance), the mouth of the cave in
/// the Goma Range between Vault and Bilibin, where Ivan rejoins the party.
///
/// A stump caught in ivy (LOCALID_STUMP, held by LOCALID_STUMP_IVY) blocks the
/// way on. Until FLAG_GOMA_CAVE_IVAN_JOINED is set, Move on it runs
/// EventScript_IvanRejoins: Garet says their Psynergy can't free it, and only
/// once the thieves in Vault are jailed (FLAG_VAULT_THIEVES_JAILED) does Ivan
/// walk up, blow the ivy off with Whirlwind and rejoin with the Shaman's Rod
/// equipped. The ivy is never the player's target (OBJ_EVENT_NOT_TARGETED), so
/// that cutscene is the only way past it. After it, Move runs
/// EventScript_MoveStump, which sends the stump leaping out of the way once it
/// stands on column 11: it lands in the pool under the waterfall and sinks
/// until only its top shows, a stepping stone beside LOCALID_SUNKEN_STUMP
/// (FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED; its render,
/// places/goma-cave-entrance_goma-cave-entrance-stump-moved/map-0.png).
///
/// The two stone pillars and a second ivy are puzzles on the way. The west
/// pillar is pushed by walking into it or moved with Move, and stands on one of
/// three columns, which two flags record
/// (FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP and _TWO_STEPS). The east
/// pillar, moved with Move to its last column, settles into the floor so it can
/// be walked over (FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE). Whirlwind
/// blows the second ivy away for good (FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY);
/// its placement row is then skipped, though it keeps its actor number.
///
/// Map_Initialize blocks Retreat and puts everything back as these flags say:
/// the stump where it landed, the pillars on their columns and the map cells
/// that go with them. GOMA_CAVE_ENTRANCE_TO_GOMA_CAVE_STREAM leads into Goma
/// Cave (MapId_GomaCaveStream) and GOMA_CAVE_ENTRANCE_TO_WORLD_MAP out to the
/// world map.
///
/// Open questions: none.
#include "unit/map/goma_cave_entrance.h"
#include "version.h"
#include "unit/map/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "battle.h"
#include "field.h"
#include "menu.h"
#include "system.h"
#include "object_event.h"
#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
#include "create_configured_overlay_object.h"
#include "debug.h"
#include "game.h"
#include "graphics.h"
#include "minigame.h"
#include "sound.h"
#include "edition.h"
#include "call.h"
#include "battle_system.h"
#include "battle_unit.h"
#include "item.h"
#include "inventory.h"
#include "text/msg_ids.h"
#include "objcmd.h"
#include "global_cells.h"
#include "ability_ids.h"
#include "macros.h"
#include "flags.h"
#include "songs.h"
#include "script.h"
#include "item_ids.h"
#include "vector3.h"
#include "fx_work.h"
#include "fixed_math.h"

/// The room's actors. Garet's and Ivan's rows go to their party slots, and the
/// rows after them number from LOCALID_FIRST_PLACED.
enum GomaCaveEntranceLocalId {
  /// The stump caught in ivy that blocks the way on.
  /// ★ Named from the game: "It's no use. Our Psynergy isn't strong enough to
  /// free that stump." (MsgGomaNoUsePsynergy)
  LOCALID_STUMP = LOCALID_FIRST_PLACED,
  /// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
  LOCALID_WEST_PILLAR,
  /// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
  LOCALID_EAST_PILLAR,
  /// A second stump, always sunk until only its top shows, that the party
  /// walks over.
  /// ★ Named from what the player sees: a stump sunk to its cut top, the last
  /// frame of STUMP_ANIMATION_SINK in spr_212.png (its render)
  LOCALID_SUNKEN_STUMP,
  /// Unused: two Move hands and a third stump, placed at the map's origin
  /// outside the camera's bounds. Nothing in the overlay addresses them, and
  /// Move spawns its own hand (FieldEffect_StartItemBreak), so the player never
  /// sees them.
  /// ★ Named from what the player sees: a large open hand, spr_215.png (its
  /// render)
  LOCALID_UNUSED_MOVE_HAND_1,
  /// ★ Named from what the player sees: a large open hand, spr_215.png (its
  /// render)
  LOCALID_UNUSED_MOVE_HAND_2,
  /// ★ Named from what the player sees: a tree stump, spr_212.png (its render)
  LOCALID_UNUSED_STUMP,
  /// The ivy holding the stump, until Ivan blows it away.
  /// ★ Named from the game: "It looks like it's trapped in the ivy."
  /// (MsgGomaNoUsePsynergy)
  LOCALID_STUMP_IVY,
  /// ★ Named from the game: "It looks like it's trapped in the ivy."
  /// (MsgGomaNoUsePsynergy)
  LOCALID_IVY
};

// Private scene actions used by the reunion sequence.
static void GomaCaveEntrance_WhirlwindOnStumpIvy(void);
static void GomaCaveEntrance_LeapStump(s32 stumpId);
static void GomaCaveEntrance_GiveShamansRod(void);

// Shared effects stay at their native positions in the overlay.
#include "common/effect_create.h"
#include "common/effect_spawn.h"

/// Shows the next message over the speaker, then pauses.
static void GomaCaveEntrance_SayThenWait(s32 objectEventId, s32 frames) {
  MESSAGE_PUT(objectEventId, 0);
  WAIT(frames);
}

const struct MapWarpDest *Map_GetWarpDests(void) {
  return gGomaCaveEntranceWarpDests;
}

const struct MapWarpRegion *Map_GetWarpRegions(void) { return NULL; }

const u32 *Map_GetWarps(void) { return gGomaCaveEntranceWarps; }

const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  return gGomaCaveEntranceObjectEventTemplates;
}

/// Move on the west pillar at Goma Cave Entrance, and after each push: the
/// pillar stands on one of three columns, and the map cells and flags follow
/// it.
static void EventScript_MoveWestPillar(void) {
  struct ObjectEvent *pillar;
  s32 column;

  pillar = ObjectEvent_GetById(LOCALID_WEST_PILLAR);
  column = pillar->x.fixed / PIXELS(16);
  GameFlag_Clear(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP);
  GameFlag_Clear(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_TWO_STEPS);
  if (column == 15) {
    MapGrid_CopyCellAttributes(47, 18, 1, 2, 16, 18);
  } else if (column == 16) {
    MapGrid_CopyCellAttributes(48, 18, 1, 2, column, 18);
    FLAG_SET(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP);
  } else {
    MapGrid_CopyCellAttributes(47, 18, 1, 2, 16, 18);
    FLAG_SET(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_TWO_STEPS);
  }
}

/// Move on the east pillar at Goma Cave Entrance: once it reaches column 23 it
/// settles into the floor and can be walked over.
static void EventScript_MoveEastPillar(void) {
  struct ObjectEvent *pillar;
  s32 column;

  pillar = ObjectEvent_GetById(LOCALID_EAST_PILLAR);
  column = pillar->x.fixed / PIXELS(16);
  if (column == 23) {
    WAIT(10);
    ObjectEvent_GetById(LOCALID_EAST_PILLAR)->priorityFlags =
        OBJ_EVENT_PRIORITY_UNDERFOOT;
    ObjectEvent_GetById(LOCALID_EAST_PILLAR)->motionFlags = 0;
    Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_EAST_PILLAR),
                                     0);
    MapGrid_CopyCellAttributes(54, 17, 1, 1, column, 17);
    Engine_GameFlagSet(FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE);
  }
}

/// Walking into the west pillar at Goma Cave Entrance from the west (trigger 5)
/// or the east (trigger 6): the leader leans on it and pushes it one step.
static void EventScript_PushWestPillar(void) {
  struct EventWork *work;
  s32 offsetX;

  work = gEventWork;
  EV_START();
  OBJECT_MODE(LOCALID_PARTY_LEADER, 8);
  WAIT(20);
  OBJECT_SPEED(LOCALID_PARTY_LEADER, PIXELS(1) / 5, PIXELS(1) / 10);
  OBJECT_SPEED(LOCALID_WEST_PILLAR, PIXELS(1) / 5, PIXELS(1) / 10);
  SE(SE_PUSH);
  // 16 pixels east from trigger 5, west from trigger 6.
  offsetX = (11 - (work->touchedTrigger << 1)) << 4;
  ObjectEvent_SetDestinationOffset(LOCALID_PARTY_LEADER, offsetX, 0);
  ObjectEvent_SetDestinationOffset(LOCALID_WEST_PILLAR, offsetX, 0);
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  Engine_ObjectEventWaitForMove(LOCALID_WEST_PILLAR);
  WAIT(20);
  OBJECT_MODE(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  EventScript_MoveWestPillar();
  FieldEffect_PlayQueuedSound();
  EV_END();
}

/// Unused: an empty function nothing calls.
UNUSED static void GomaCaveEntrance_NoOpA(void) {}

/// Unused: a second empty function nothing calls.
UNUSED static void GomaCaveEntrance_NoOpB(void) {}

/// Whirlwind on the second ivy at Goma Cave Entrance: it is blown away for
/// good.
static void EventScript_BlowAwayIvy(void) {
  EV_START();
  Engine_GameFlagSet(FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY);
  EV_END();
}

const struct MapEvent *Map_GetEvents(void) { return gGomaCaveEntranceEvents; }

/// Sets up Goma Cave Entrance: Retreat is blocked, and the stump and pillars
/// stand where the party left them, the stump's ivy gone once Ivan has blown
/// it off.
s32 Map_Initialize(void) {
  struct ObjectEvent *movedObject;

  Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
  WAIT(10);
  FieldEffect_SetQueuedSoundAndPlay(SE_170);
  OBJECT_MODE(LOCALID_SUNKEN_STUMP, STUMP_ANIMATION_SINK);
  ObjectEvent_GetById(LOCALID_SUNKEN_STUMP)->priorityFlags =
      OBJ_EVENT_PRIORITY_UNDERFOOT;
  {
    struct ObjectEvent *record = ObjectEvent_GetById(LOCALID_STUMP);

    record->collisionFlags = OBJ_EVENT_NEAR_PSYNERGY | record->collisionFlags;
  }
  // The player never targets the stump's ivy; only Ivan's Whirlwind in
  // EventScript_IvanRejoins removes it.
  {
    struct ObjectEvent *record = ObjectEvent_GetById(LOCALID_STUMP_IVY);

    record->collisionFlags = OBJ_EVENT_NOT_TARGETED | record->collisionFlags;
  }
  if (Engine_GameFlagIsSet(FLAG_GOMA_CAVE_IVAN_JOINED) != 0) {
    MapGrid_CopyCellAttributes(74, 11, 1, 1, 73, 11);
  }
  if (Engine_GameFlagIsSet(FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED) != 0) {
    Engine_ObjectEventSetPosition(LOCALID_STUMP, PIXELS(136), PIXELS(196));
    ObjectEvent_GetById(LOCALID_STUMP)->priorityFlags |=
        OBJ_EVENT_PRIORITY_UNDERFOOT;
    OBJECT_MODE(LOCALID_STUMP, STUMP_ANIMATION_SINK);
    MapGrid_CopyCellAttributes(39, 12, 3, 1, 8, 12);
    MapGrid_CopyCellAttributes(43, 11, 3, 1, 12, 11);
  }
  if (Engine_GameFlagIsSet(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP) != 0) {
    OBJECT_POSITION_SET(LOCALID_WEST_PILLAR, 264, 312);
    MapGrid_CopyCellAttributes(48, 18, 1, 2, 16, 18);
  } else {
    if (Engine_GameFlagIsSet(FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_TWO_STEPS) !=
        0) {
      OBJECT_POSITION_SET(LOCALID_WEST_PILLAR, 280, 312);
      MapGrid_CopyCellAttributes(47, 18, 1, 2, 16, 18);
    }
  }
  if (Engine_GameFlagIsSet(FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE) != 0) {
    Engine_ObjectEventSetPosition(LOCALID_EAST_PILLAR, PIXELS(376),
                                  PIXELS(280));
    ObjectEvent_GetById(LOCALID_EAST_PILLAR)->priorityFlags =
        OBJ_EVENT_PRIORITY_UNDERFOOT;
    ObjectEvent_GetById(LOCALID_EAST_PILLAR)->motionFlags = 0;
    movedObject = ObjectEvent_GetById(LOCALID_EAST_PILLAR);
    Engine_ObjectEventSetSpriteFlags(movedObject, 0);
    MapGrid_CopyCellAttributes(54, 17, 1, 1, 23, 17);
  }
  return 0;
}

/// Move on the stump at Goma Cave Entrance while the ivy holds it: Garet says
/// their Psynergy is not strong enough. Once the thieves in Vault are jailed
/// (FLAG_VAULT_THIEVES_JAILED), Ivan turns up, blows the ivy away with
/// Whirlwind, says he could not get into Lunpa and asks to join the party; he
/// rejoins with the Shaman's Rod equipped (FLAG_GOMA_CAVE_IVAN_JOINED).
static void EventScript_IvanRejoins(void) {
  struct ObjectEvent *leader;

#if TBS_PATCH_1
  FieldEffect_PlayQueuedSound();
#endif
  EV_START();
  WAIT(30);
  MESSAGE_SET(MsgGomaNoUsePsynergy);
  OBJECT_SPEED(LOCALID_PARTY_LEADER, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  OBJECT_SPEED(GARET, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 20);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  ObjectEvent_WalkToAndWait(GARET, 264, 168);
  OBJECT_DIR(GARET, FACING_SOUTHWEST, 20);
  WAIT(20);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_SHAKE_HEAD);
  WAIT(20);
  Script_ShowMessageAndWait(GARET, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  WAIT(20);
  if (FLAG_CHECK(FLAG_VAULT_THIEVES_JAILED) == 0) {
    // Garet falls back into line.
    OBJECT_MODE(GARET, ANIMATION_WALK);
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (leader != 0) {
      ObjectEvent_SetDestination(GARET, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Engine_ObjectEventWaitForMove(GARET);
    OBJECT_POSITION_SET(GARET, 0, 0);
    EV_END();
  } else {
    // Ivan comes up from the south: "You want to move the stump, right?"
    OBJECT_POSITION_SET(IVAN, 360, 248);
    OBJECT_SPEED(IVAN, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
    ObjectEvent_WalkToAndWait(IVAN, 272, 248);
    ObjectEvent_WalkToAndWait(IVAN, 272, 208);
    OBJECT_DIR(IVAN, FACING_NORTHWEST, 20);
    Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
    WAIT(20);
    Script_ShowMessageAndWait(IVAN, 0, 20);
    OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
    OBJECT_DIR(GARET, FACING_SOUTH, 0);
    WAIT(20);
    OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_NOD);
    ObjectEvent_WalkToAndWait(IVAN, 264, 200);
    OBJECT_WALK(LOCALID_PARTY_LEADER, 248, 168);
    ObjectEvent_WalkToAndWait(IVAN, 248, 184);
    Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
    OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
    OBJECT_DIR(GARET, FACING_SOUTHWEST, 0);
    ObjectEvent_WalkToAndWait(IVAN, 232, 184);
    WAIT(20);
    OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    OBJECT_DIR(IVAN, FACING_NORTHEAST, 20);
    Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD);
    WAIT(20);
    Script_ShowMessageAndWait(IVAN, 0, 20);
    OBJECT_MODE(LOCALID_PARTY_LEADER, ANIMATION_NOD);
    Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
    WAIT(20);
    OBJECT_DIR(IVAN, FACING_WEST, 20);
    Script_ShowMessageAndWait(IVAN, 0, 120);
    OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, GARET, 0);
    WAIT(60);
    OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
    OBJECT_DIR(GARET, FACING_SOUTHWEST, 0);
    WAIT(60);
    OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
    Engine_ObjectEventStartRepeatedMotion(IVAN, 1);
    WAIT(30);
    Script_ShowMessageAndWait(IVAN, 0, 30);
    OBJECT_DIR(IVAN, FACING_NORTHEAST, 20);
    Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
    Engine_ObjectEventRunRepeatedMotion(GARET, 2);
    WAIT(20);
    Script_ShowMessageAndWait(IVAN, 0, 20);
    OBJECT_MODE(LOCALID_PARTY_LEADER, ANIMATION_NOD);
    Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
    WAIT(20);
    Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_NOD);
    WAIT(20);
    OBJECT_DIR(IVAN, FACING_WEST, 20);
    // "I have a great idea. Leave it to me!": Whirlwind.
    GomaCaveEntrance_WhirlwindOnStumpIvy();
    OBJECT_MODE(IVAN, ANIMATION_STAND);
    WAIT(20);
#if TBS_PATCH_1
    FieldEffect_PlayQueuedSound();
#endif
    OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    Engine_ObjectEventJump(GARET, 2, 0);
    WAIT(20);
    GomaCaveEntrance_SayThenWait(GARET, 20);
    ObjectEvent_WalkToAndWait(IVAN, 264, 184);
    WAIT(10);
    ObjectEvent_FaceObjectEvent(IVAN, GARET, 0);
    ObjectEvent_FaceObjectEvent(GARET, IVAN, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
    WAIT(20);
    Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_NOD);
    WAIT(20);
    GomaCaveEntrance_SayThenWait(IVAN, 60);
    OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    WAIT(60);
    OBJECT_DIR(GARET, FACING_WEST, 0);
    OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_EAST, 0);
    WAIT(60);
    OBJECT_DIR(GARET, FACING_SOUTH, 0);
    OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
    WAIT(10);
    GomaCaveEntrance_SayThenWait(GARET, 20);
    OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    WAIT(60);
    Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD);
    WAIT(20);
    GomaCaveEntrance_SayThenWait(IVAN, 20);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
    ObjectEvent_FaceObjectEvent(GARET, IVAN, 0);
    OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    WAIT(60);
    Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
    WAIT(20);
    GomaCaveEntrance_SayThenWait(IVAN, 30);
    OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    WAIT(80);
    Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_NOD);
    WAIT(20);
    GomaCaveEntrance_SayThenWait(IVAN, 20);
    Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 1);
    Engine_ObjectEventStartRepeatedMotion(GARET, 1);
    ObjectEvent_SetAttachedEffect(
        LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                             ATTACHED_EFFECT_ONCE);
    WAIT(60);
    Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD);
    WAIT(20);
    GomaCaveEntrance_SayThenWait(IVAN, 20);
    OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_EAST, 0);
    OBJECT_DIR(GARET, FACING_WEST, 0);
    WAIT(80);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
    ObjectEvent_FaceObjectEvent(GARET, IVAN, 0);
    WAIT(30);
    Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_NOD);
    WAIT(20);
    GomaCaveEntrance_SayThenWait(IVAN, 30);
    Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD);
    WAIT(20);
    GomaCaveEntrance_SayThenWait(IVAN, 20);
    Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
    Engine_ObjectEventRunRepeatedMotion(GARET, 2);
    WAIT(20);
    Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_NOD);
    WAIT(20);
    GomaCaveEntrance_SayThenWait(IVAN, 40);
    OBJECT_MODE(LOCALID_PARTY_LEADER, ANIMATION_NOD);
    Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
    WAIT(20);
    // "Please, allow me to join your quest.": Ivan rejoins with his rod.
    FieldScript_PrepareObjectAndApplyValue(IVAN, 1);
    WAIT(60);
    GomaCaveEntrance_GiveShamansRod();
    Engine_ObjectEventRunRepeatedMotion(IVAN, 1);
    WAIT(20);
    ObjectEvent_WalkToAndWait(IVAN, 248, 184);
    WAIT(20);
    GomaCaveEntrance_SayThenWait(IVAN, 20);
    OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_WEST, 0);
    OBJECT_DIR(GARET, FACING_WEST, 0);
    WAIT(120);
    GomaCaveEntrance_SayThenWait(IVAN, 30);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
    ObjectEvent_FaceObjectEvent(GARET, IVAN, 0);
    ObjectEvent_FaceObjectEvent(IVAN, LOCALID_PARTY_LEADER, 0);
    WAIT(20);
    OBJECT_MODE(LOCALID_PARTY_LEADER, ANIMATION_NOD);
    OBJECT_MODE(GARET, ANIMATION_NOD);
    Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_NOD);
    WAIT(50);
    OBJECT_SPEED(GARET, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
    OBJECT_SPEED(IVAN, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
    OBJECT_WALK(GARET, 248, 168);
    ObjectEvent_WalkToAndWait(IVAN, 248, 168);
    OBJECT_POSITION_SET(IVAN, 0, 0);
    Engine_ObjectEventWaitForMove(GARET);
    OBJECT_POSITION_SET(GARET, 0, 0);
    MapGrid_CopyCellAttributes(74, 11, 1, 1, 73, 11);
    FLAG_SET(FLAG_GOMA_CAVE_IVAN_JOINED);
    EV_END();
  }
}

/// Move on the freed stump at Goma Cave Entrance: from column 11 it spins,
/// leaps away and lies where it lands (FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED).
static void EventScript_MoveStump(void) {
  s32 column;
  struct ObjectEvent *stump;

  EV_START();
  column = ObjectEvent_GetById(LOCALID_STUMP)->x.fixed >> 20;
  if (column == 11) {
    GomaCaveEntrance_LeapStump(LOCALID_STUMP);
    stump = ObjectEvent_GetById(LOCALID_STUMP);
    stump->priorityFlags |= OBJ_EVENT_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(39, 12, 3, 1, 8, 12);
    MapGrid_CopyCellAttributes(43, 11, 3, 1, 12, column);
    Engine_GameFlagSet(FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED);
  }
  EV_END();
}

/// Ivan casts Whirlwind on the stump's ivy, and the scene waits for it. The
/// announcement is lowered to a fixed place (POSITIONED_MESSAGE_LOWER), since
/// it would otherwise be placed by where the leader, not Ivan, stands.
static void GomaCaveEntrance_WhirlwindOnStumpIvy(void) {
  struct FieldEffectWork *effectWork;
  u8 flags;

  effectWork = gEffectWork;
  FieldEffect_LoadActionEffectResources(ABILITY_WHIRLWIND, 1);
  FieldEffect_SetupObjectPair(IVAN, LOCALID_STUMP_IVY);
  flags = (u8)effectWork->messageMode | POSITIONED_MESSAGE_LOWER;
  effectWork->messageMode = flags;
  EventObject_Initialize();
  Field_DispatchTypeHandler(PSYNERGY_STEP_ON_TARGET);
  FieldEffect_StopCasterHook();
}

/// The leaping stump's update hook turns its sprite a fixed amount per frame.
static void GomaCaveEntrance_SpinStump(union FieldObject *fieldObject) {
  struct ObjectEvent *obj = &fieldObject->objectEvent;
  struct AnimationObject *state = obj->sprite;
  s32 v = state->rotation - 0x800;

  state->rotation = v;
}

// Shared height-wait code stays at its native position in the overlay.
#include "common/object_waitdown.h"

/// The stump spins in place, leaps and lands with a shake and a ring of dust,
/// then hops on into the pool under the waterfall with a splash and sinks
/// (STUMP_ANIMATION_SPLASH).
static void GomaCaveEntrance_LeapStump(s32 stumpId) {
  struct Vector3 dir;
  struct EffectOptions params;
  struct Vector3 *v;
  struct EffectOptions *p;
  struct ObjectEvent *stump;
  u32 i;

  stump = ObjectEvent_GetById(stumpId);
  stump->motionFlags = 0;
  for (i = 0; i <= 17; i++) {
    Engine_TaskWait(1);
    // The corresponding signed halfword view retains the negative literal.
    *(s16 *)&stump->sprite->rotation += -256;
    stump->x.fixed -= Engine_MathCos(stump->sprite->rotation) / 2;
    stump->targetX = OBJ_EVENT_NO_TARGET;
  }
  stump->update = GomaCaveEntrance_SpinStump;
  OBJECT_SPEED(stumpId, PIXELS(3), 3 * PIXELS(1) / 2);
  Engine_ObjectEventSetDestination(stumpId, 160, 192);
  stump->gravity = 4 * PIXELS(1) / 5;
  stump->motionFlags = OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
  stump->terrainId = 0;
  Engine_ObjectEventWaitForMove(stumpId);
  MapObjectEvent_WaitBelowHeight(stump, PIXELS(32));
  Engine_AudioPlayCue(SE_188);
  MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  Engine_AudioPlayCue(SE_141);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  for (i = 0; i <= 16; i++) {
    v = &dir;
    SET_POSITION(v, Engine_MathCos(i << 12), 0, Engine_MathSin(i << 12));
    v->x -= v->x / 4;
    v->z -= v->z / 2;
    FieldEffect_Spawn(stump->x.fixed, stump->y.fixed, stump->z.fixed, v->x,
                      v->y, v->z, 0, NULL);
  }
  stump->velocityY = PIXELS(5);
  Engine_ObjectEventSetDestination(stumpId, 139, 196);
  Engine_ObjectEventWaitForMove(stumpId);
  MapObjectEvent_WaitBelowHeight(stump, PIXELS(32));
  // UB: the native integer store aliases the update pointer. A typed clear
  // swaps this store with the following sprite-pointer load.
#ifdef UBFIX
  stump->update = NULL;
#else
  *(s32 *)&stump->update = 0;
#endif
  stump->sprite->rotation = 0x1000;
  p = &params;
  p->type = SPRITE_SPLASH;
  p->startScaleX = Q16_ONE / 2;
  p->startScaleY = 4 * Q16_ONE / 5;
  p->targetScaleX = 3 * Q16_ONE / 2;
  p->targetScaleY = 6 * Q16_ONE / 5;
  FieldEffect_Spawn(
      stump->x.fixed, stump->y.fixed, stump->z.fixed, 0, 0, 0,
      EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET, p);
  Engine_AudioPlayCue(SE_CAST);
  OBJECT_MODE(stumpId, STUMP_ANIMATION_SPLASH);
  Engine_MapGridWaitWorkValuesBelow256();
}

// Shared inventory equipment code stays at its native overlay position.
#include "common/item_equip.h"

/// Gives Ivan the Shaman's Rod and equips it. With his items full, he drops
/// another to make room; later patches skip items marked only indispensable.
static void GomaCaveEntrance_GiveShamansRod(void) {
  struct BattleUnit *member;
  struct ItemDefinition *item;
  s32 tries;
  s32 i;
  s32 id;

  id = ITEM_SHAMANS_ROD;
  member = Unit_Get(IVAN);
  tries = 0;
retry:
  if (++tries > 1000) {
#if TBS_PATCH_3
    for (i = 0; i <= INVENTORY_SLOTS - 1; i++) {
      item = Item_Get(member->inventory[i]);
      if (item->flags != ITEM_INDISPENSABLE) {
        Inventory_Discard(IVAN, i);
        break;
      }
    }
    if (i == INVENTORY_SLOTS)
#endif
      member->inventory[INVENTORY_SLOTS - 1] = 0;
  }
  if (Inventory_AddItem(IVAN, id) == -1) {
    for (i = 0; i <= INVENTORY_SLOTS - 1; i++) {
      item = Item_Get(member->inventory[i]);
      if (item->type == ITEM_TYPE_WEAPON) {
        Inventory_Discard(IVAN, i);
        goto retry;
      }
    }
    for (i = 0; i <= INVENTORY_SLOTS - 1; i++) {
      item = Item_Get(member->inventory[i]);
      if (item->type == ITEM_TYPE_NONE &&
          (item->flags & ITEM_INDISPENSABLE) == 0 &&
          item->useType == ITEM_USE_CONSUMED) {
        Inventory_Discard(IVAN, i);
        goto retry;
      }
    }
    goto retry;
  }
  for (i = 0; i <= INVENTORY_SLOTS - 1; i++) {
    if (member->inventory[i] == id)
      Inventory_Equip(IVAN, i);
  }
}

s32 gEffectScript0[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

s32 gGomaCaveEntranceTallIvyScript[] = {
    SC_SET(SW_SCALE_Y, 0x19999),
    SC_END,
};

struct MapWarpDest gGomaCaveEntranceWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 192, 0, 197, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {GOMA_CAVE_ENTRANCE_FROM_WORLD_MAP, CONDITION_ALWAYS, 96, 0, 440,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {GOMA_CAVE_ENTRANCE_FROM_GOMA_CAVE_STREAM, CONDITION_ALWAYS, 200, 0, 184,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

u32 gGomaCaveEntranceWarps[] = {
    MAP_WARPS(GomaCaveEntrance),
    MAP_WARP(GOMA_CAVE_ENTRANCE_TO_GOMA_CAVE_STREAM, GomaCaveStream,
             GOMA_CAVE_STREAM_FROM_GOMA_CAVE_ENTRANCE),
    MAP_WARP(GOMA_CAVE_ENTRANCE_TO_WORLD_MAP, WorldMap,
             WORLD_MAP_FROM_GOMA_CAVE_ENTRANCE),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gGomaCaveEntranceObjectEventTemplates[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(192), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(136), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_MOVE_HAND, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_WEST, TALK_KEEP_FACING, 0 },
    { SPRITE_MOVE_HAND, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_WEST, TALK_KEEP_FACING, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_IVY, FLAG_GOMA_CAVE_IVAN_JOINED, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(191), 0, PIXELS(194), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_IVY, FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY, (s32)gGomaCaveEntranceTallIvyScript,
        PIXELS(200), 0, PIXELS(168), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gGomaCaveEntranceEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, GOMA_CAVE_ENTRANCE_TO_GOMA_CAVE_STREAM },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, GOMA_CAVE_ENTRANCE_TO_WORLD_MAP },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushWestPillar) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 6, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushWestPillar) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_STUMP, FLAG_GOMA_CAVE_IVAN_JOINED, EVENT_SCRIPT(EventScript_IvanRejoins) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_STUMP, FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED, EVENT_SCRIPT(EventScript_MoveStump) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_WEST_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_MoveWestPillar) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_EAST_PILLAR, FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE, EVENT_SCRIPT(EventScript_MoveEastPillar) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_WHIRLWIND), LOCALID_IVY, FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY, EVENT_SCRIPT(EventScript_BlowAwayIvy) },
    { MAP_EVENTS_END },
};
// clang-format on
