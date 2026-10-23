/// @file
/// Three of the side rooms of Crossbone Isle Cave, the cave beneath Crossbone
/// Isle, each opening off one of the nine floors the cave descends through
/// (crossbone_isle.c) as the other five do (crossbone_isle_stepping_stones.c),
/// with four chests each. All three take the area name Crossbone Isle Cave. In
/// the order the party meets them:
///
/// - MapId_CrossboneIsleKeys, off the third floor, a room of locked doors.
///   Four Blue Keys lie about it until Catch takes them, and a Red Key shows
///   only while Reveal lasts. Walking into one of the four blue doors in a row
///   with a Blue Key opens it and uses the key up (four flags from
///   FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN), and the Red Key opens the red
///   door beyond them (FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN). The keys taken and
///   the doors opened are restored on entry.
/// - MapId_CrossboneIsleStatues, off the fourth, where five statues of a
///   robed woman wait, lit in circles by braziers in the dark. As the leader
///   crosses each one's stretch of the room it steps out after him unless he is
///   hidden by Cloak, and goes back home once he is past
///   (PURSUER_AFTER_LEADER_FLAG while one is after him); Halt stops one where
///   it stands, and Reveal clears the translucent layer over the room while it
///   lasts. Each statue's home and the stretch of floor that sets it after the
///   leader are drawn over the room's render in
///   https://alchemy.pascalpixel.com/places/crossbone-isle-statues/stretches.png:
///   the first, second and fifth start from per-frame checks of the leader's
///   cell, the third and fourth from coord events in the three cells around
///   their homes and the three south of them.
/// - MapId_CrossboneIsleBane, off the sixth, where Bane, a Venus Djinni,
///   hides. Five pillars to push or move with Move, a ladder and a puddle that
///   Frost freezes into a pillar of ice make a way across the room. The first
///   three times the party comes near, Bane flees, the third time to the room's
///   last corner (FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE to _LAST_ESCAPE); there
///   it runs to whichever spot the leader steps toward unless Halt holds it.
///   Talked to while Halt holds it, Bane joins the party.
///
/// Open questions: none.

#include "game_state.h"
#include "djinn_ids.h"
#include "version.h"
#include "global_cells.h"
#include "map_ids.h"
#include "unit/import.h"
#include "unit/map/crossbone_isle_keys.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/runtime/runtime.h"
#include "unit/map/common/colosso_item_icon.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "edition.h"
#include "script_layer.h"
#include "field_service.h"
#include "map_tables.h"
#include "staged_object_event.h"
#include "select_overlay_data_by_runtime_selector.h"
#include "io_reg.h"
#include "resource.h"
#include "object_event.h"
#include "animspr.h"
#include "mem.h"
#include "dma.h"
#include "callback_scheduler.h"
#include "call.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "iowrite.h"
#include "field_effect.h"
#include "fixed_math.h"
#include "flags.h"
#include "songs.h"
#include "script.h"

// Map-section temp flags: Map_ResetFlagsOnEnter clears 0x300 to 0x3ff when the
// party enters another map section, and other map sections use the numbers for
// their own.
/// CrossboneIsleBane's third stone pillar has come down to two cells high or
/// less.
/// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
#define FLAG_CROSSBONE_ISLE_THIRD_PILLAR_LOWERED 0x300

#if TBS_PATCH_1
#define CROSSBONE_PURSUER_STEP_Z 16
#define CROSSBONE_LEADER_STEP_Z 24
#else
#define CROSSBONE_PURSUER_STEP_Z 24
#define CROSSBONE_LEADER_STEP_Z 16
#endif

static void EventScript_PushFirstPillar(void);
static void CrossboneIsleKeys_FollowSecondPillar(void);
static void CrossboneIsleKeys_PositionSecondPillarMarker(void);
static void CrossboneIsleKeys_InitializeKeys(void);
static void CrossboneIsleKeys_InitializeStatues(void);
static void CrossboneIsleKeys_InitializeBane(void);

/// CrossboneIsleKeys's placed object events, numbered in the order of
/// gCrossboneIsleKeysRoomObjectEventTemplates: the keys, drawn as their item
/// icons (Colosso_ShowItemIcon) until Catch takes them.
enum CrossboneIsleKeysLocalId {
  /// ★ Named from the game: "Blue Key" (MsgItemName), a key icon lying among
  /// the room's stone posts
  LOCALID_FIRST_BLUE_KEY = LOCALID_FIRST_PLACED,
  /// ★ Named from the game: "Blue Key" (MsgItemName), the second key icon
  LOCALID_SECOND_BLUE_KEY,
  /// ★ Named from the game: "Blue Key" (MsgItemName), the third key icon
  LOCALID_THIRD_BLUE_KEY,
  /// ★ Named from the game: "Blue Key" (MsgItemName), the fourth key icon
  LOCALID_FOURTH_BLUE_KEY,
  /// Shown only while Reveal lasts.
  /// ★ Named from the game: "Red Key" (MsgItemName), which opens the red door
  LOCALID_RED_KEY
};

/// CrossboneIsleStatues's placed object events, numbered in the order of
/// gCrossboneIsleStatuesObjectEventTemplates: the five statues that pursue the
/// leader, all of one sprite, each waiting at its home for the leader to cross
/// its stretch of the room.
enum CrossboneIsleStatuesLocalId {
  /// ★ Named from what the player sees: a pink statue of a robed woman, lit in
  /// a circle by a brazier (its render)
  LOCALID_FIRST_STATUE = LOCALID_FIRST_PLACED,
  /// ★ Named from what the player sees: the second statue of a robed woman (its
  /// render)
  LOCALID_SECOND_STATUE,
  /// ★ Named from what the player sees: the third statue of a robed woman (its
  /// render)
  LOCALID_THIRD_STATUE,
  /// ★ Named from what the player sees: the fourth statue of a robed woman (its
  /// render)
  LOCALID_FOURTH_STATUE,
  /// ★ Named from what the player sees: the fifth statue of a robed woman (its
  /// render)
  LOCALID_FIFTH_STATUE
};

/// The events CrossboneIsleStatues's per-frame checks raise when the
/// leader crosses the first, second or fifth pursuer's stretch of the room.
/// The third and fourth chases start from coord events instead.
enum CrossboneIsleStatuesTrigger {
  TRIGGER_FIRST_CHASE = 91,
  TRIGGER_SECOND_CHASE = 92,
  TRIGGER_FIFTH_CHASE = 96
};

/// CrossboneIsleBane's placed object events, numbered in the order of
/// gCrossboneIsleBaneObjectEventTemplates.
enum CrossboneIsleBaneLocalId {
  /// The five pillars, in Goma Cave's pillar sprite, pushed by walking into
  /// them or with Move.
  /// ★ Named from what the player sees: a brown stone pillar in the north-west
  /// of the room (its render)
  LOCALID_FIRST_PILLAR = LOCALID_FIRST_PLACED,
  /// ★ Named from what the player sees: a brown stone pillar in the middle of
  /// the room, a chest on its top (its render)
  LOCALID_SECOND_PILLAR,
  /// ★ Named from what the player sees: a brown stone pillar in the west of the
  /// room (its render)
  LOCALID_THIRD_PILLAR,
  /// ★ Named from what the player sees: one of the two brown stone pillars
  /// side by side in the south-east (its render)
  LOCALID_FOURTH_PILLAR,
  /// ★ Named from what the player sees: the other of the two brown stone
  /// pillars in the south-east (its render)
  LOCALID_FIFTH_PILLAR,
  /// Frozen by Frost into a pillar of ice.
  /// ★ Named from what the player sees: a small blue puddle on the floor of the
  /// south-west (its render)
  LOCALID_PUDDLE,
  /// The chest on top of the second pillar, in the sprite of McCoy's Palace's
  /// chests, holding the Demon Axe: it follows the pillar until the pillar
  /// reaches the ladder, where the chest's bg event 107 takes its place,
  /// and it is drawn open once FLAG_CROSSBONE_ISLE_DEMON_AXE is set.
  /// ★ Named from what the player sees: a red chest standing on the second
  /// pillar (its render)
  LOCALID_SECOND_PILLAR_CHEST,
  /// Bane, a Venus Djinni, until it is caught.
  /// ★ Named from the game: "Bane" (MsgDjinnName)
  LOCALID_BANE
};

enum { PASSAGE_INDEX_MASK = 0xff, PASSAGE_PLAY_OPENING = 0x100 };

/// CrossboneIsleBane: the puddle is frozen.
#define BANE_PUDDLE_FROZEN_FLAG FLAG_MAP_TEMP_00
/// CrossboneIsleBane: the chest on the second pillar has reached the ladder.
#define BANE_CHEST_AT_LADDER_FLAG FLAG_MAP_TEMP_06
/// CrossboneIsleBane: the leader last looked east from the ladder, which gives
/// the ladder cell its other metatile behaviors.
#define BANE_LOOKED_EAST_FLAG FLAG_MAP_TEMP_07
// The Move events of CrossboneIsleBane's pillars also test FLAG_MAP_TEMP_01
// and 03 to 05, which nothing sets.
/// CrossboneIsleStatues: a statue is after the leader.
#define PURSUER_AFTER_LEADER_FLAG FLAG_MAP_TEMP_20

/// Publishes the next raised-event trigger.
static __inline__ void MapLocalState_StoreStep(s16 *field, s32 step) {
  *field = step;
}

/// Lifts the first pillar and clears the terrain beneath its column.
static __inline__ void LiftPillar(s32 column, u8 motionFlags) {
  {
    s32 destinationColumn = column;
    MapGrid_CopyCellAttributes(61, 36, 1, 1, destinationColumn, 42);
  }
  ObjectEvent_GetById(LOCALID_FIRST_PILLAR)->motionFlags = motionFlags;
  ObjectEvent_GetById(LOCALID_FIRST_PILLAR)->y.fixed = PIXELS(32);
}

#include "common/object_staged_object_event.h"

/// Talking to Bane in CrossboneIsleBane while Halt holds it
/// (MAP_EVENT_TALK_HALTED): Bane, a Venus Djinni, joins the party.
static void EventScript_BaneJoins(void) {
  Djinn_JoinParty(LOCALID_BANE, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_BANE));
}

/// Unused: an empty function nothing calls.
UNUSED static void CrossboneIsleKeys_UnusedHook(void) {}

const struct MapWarpDest *Map_GetWarpDests(void) {
  s16 selector = gGameState.mapId;

  if (selector == (s32)&MapId_CrossboneIsleKeys) {
    return gCrossboneIsleKeysRoomWarpDests;
  }
  if (selector == (s32)&MapId_CrossboneIsleStatues) {
    return gCrossboneIsleStatuesWarpDests;
  }
  if (selector == (s32)&MapId_CrossboneIsleBane) {
    return gCrossboneIsleBaneWarpDests;
  }
  return gCrossboneIsleKeysWarpDestsOther;
}

const struct MapWarpRegion *Map_GetWarpRegions(void) { return NULL; }

const u32 *Map_GetWarps(void) { return gCrossboneIsleKeysWarps; }

const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  s16 selector = gGameState.mapId;

  if (selector == (s32)&MapId_CrossboneIsleKeys) {
    return gCrossboneIsleKeysRoomObjectEventTemplates;
  }
  if (selector == (s32)&MapId_CrossboneIsleStatues) {
    return gCrossboneIsleStatuesObjectEventTemplates;
  }
  if (selector == (s32)&MapId_CrossboneIsleBane) {
    return gCrossboneIsleBaneObjectEventTemplates;
  }
  return gCrossboneIsleKeysObjectEventTemplatesOther;
}

/// Halt on a pursuer in CrossboneIsleStatues: nothing beyond the
/// Psynergy's own effect, which keeps it from stepping out.
static void EventScript_HaltPursuer(void) {}

// Turn the Djinni toward the party leader each frame.
// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 38 to 36 bytes in tbs-en.
#ifdef UBFIX
static void
CrossboneIsleKeys_FaceBaneTowardLeader(union FieldObject *fieldObject) {
#else
static s32
CrossboneIsleKeys_FaceBaneTowardLeader(union FieldObject *fieldObject) {
#endif
  struct ObjectEvent *objectEvent = &fieldObject->objectEvent;
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

  objectEvent->facing = ArcTan2(leader->z.fixed - objectEvent->z.fixed,
                                leader->x.fixed - objectEvent->x.fixed);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// In CrossboneIsleBane, the first time the party comes near
/// (coord event 20): Bane notices them and hops away.
static void EventScript_BaneFirstEscape(void) {
  struct ObjectEvent *djinni;

  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE) == 0) {
    FLAG_SET(FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE);
    EV_START();
    Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
    Engine_CameraMoveToObjectEvent(LOCALID_BANE, 1);
    Engine_CameraWaitForMove();
    OBJECT_DIR(LOCALID_BANE, FACING_EAST, 20);
    ObjectEvent_SetAttachedEffect(LOCALID_BANE, 0x102);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_BANE, 2);
    WAIT(20);
    OBJECT_SPEED(LOCALID_BANE, PIXELS(1), PIXELS(1) / 2);
    SE(SE_JUMP);
    djinni = ObjectEvent_GetById(LOCALID_BANE);
    djinni->velocityY = PIXELS(8);
    ObjectEvent_WalkToAndWait(LOCALID_BANE, 584, 680);
    OBJECT_DIR(LOCALID_BANE, FACING_SOUTH, 20);
    EV_END();
  }
}

/// In CrossboneIsleBane, coming near again (coord event 21): Bane hops,
/// dashes across the platforms and crashes into the far wall, shaking the room,
/// and is left squashing and stretching.
static void EventScript_BaneSecondEscape(void) {
  struct ObjectEvent *spinningDjinni;
  s32 escapeComplete;
  struct ObjectEvent *jumpingDjinni;

  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE) != 0) {
    escapeComplete = FLAG_CHECK(FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE);
    if (escapeComplete == 0) {
      FLAG_SET(FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE);
      EV_START();

      // Focus on the Djinni before its next jump away.
      Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
      Engine_CameraMoveToObjectEvent(LOCALID_BANE, 1);
      Engine_CameraWaitForMove();
      OBJECT_DIR(LOCALID_BANE, FACING_SOUTH, 20);
      ObjectEvent_SetAttachedEffect(LOCALID_BANE, 0x102);
      Engine_ObjectEventRunRepeatedMotion(LOCALID_BANE, 2);
      WAIT(20);
      OBJECT_SPEED(LOCALID_BANE, PIXELS(1), PIXELS(1) / 2);
      SE(SE_JUMP);
      jumpingDjinni = ObjectEvent_GetById(LOCALID_BANE);
      jumpingDjinni->velocityY = PIXELS(10);
      ObjectEvent_WalkToAndWait(LOCALID_BANE, 584, 664);
      OBJECT_DIR(LOCALID_BANE, FACING_SOUTH, 20);
      ObjectEvent_SetAttachedEffect(LOCALID_BANE, 0x102);
      WAIT(30);

      // Dash across the platforms, then shake the room on landing.
      OBJECT_SPEED(LOCALID_BANE, PIXELS(8), PIXELS(1) / 4);
      ObjectEvent_WalkToAndWait(LOCALID_BANE, 664, 664);
      ObjectEvent_WalkToAndWait(LOCALID_BANE, 744, 664);
      ObjectEvent_WalkToAndWait(LOCALID_BANE, 824, 664);
      WAIT(10);
      SE(SE_208);
      MapGrid_SetShake(PIXELS(4), PIXELS(2), Q16_ONE);
      WAIT(20);
      MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
      WAIT(30);

      // Leave the Djinni tilted while its scale animation continues.
      OBJECT_POSITION_SET(LOCALID_BANE, 888, 664);
      spinningDjinni = ObjectEvent_GetById(LOCALID_BANE);
      spinningDjinni->sprite->rotation = 0xf800;
      spinningDjinni->facing = 0;
      ObjectDispatch_SetAnimationSpeed((struct DispatchObject *)spinningDjinni,
                                       0);
      Engine_ObjectSetScript(spinningDjinni,
                             gCrossboneIsleKeys_BaneSquashScript);
      EV_END();
    }
  }
}

/// In CrossboneIsleBane, the third time (coord event 22): Bane recovers,
/// hops down a winding path to the room's last corner and keeps watching the
/// leader.
static void EventScript_BaneLastEscape(void) {
  struct ObjectEvent *djinni;

  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE) != 0 &&
      FLAG_CHECK(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE) == 0) {
    FLAG_SET(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE);
    EV_START();

    // Restore the sprite before the last jump and winding escape path.
    djinni = ObjectEvent_GetById(LOCALID_BANE);
    djinni->sprite->rotation = 0;
    ObjectDispatch_SetAnimationSpeed((struct DispatchObject *)djinni, 16);
    SE(SE_JUMP);
    djinni = ObjectEvent_GetById(LOCALID_BANE);
    djinni->velocityY = PIXELS(8);
    OBJECT_DIR(LOCALID_BANE, FACING_WEST, 30);
    ObjectEvent_SetAttachedEffect(LOCALID_BANE, 0x102);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_BANE, 2);
    WAIT(20);
    OBJECT_SPEED(LOCALID_BANE, PIXELS(1), PIXELS(1) / 2);
    SE(SE_JUMP);
    djinni = ObjectEvent_GetById(LOCALID_BANE);
    djinni->velocityY = PIXELS(4);
    ObjectEvent_WalkToAndWait(LOCALID_BANE, 880, 680);
    WAIT(10);
    ObjectEvent_SetAttachedEffect(LOCALID_BANE, 0x101);
    OBJECT_SPEED(LOCALID_BANE, PIXELS(2), PIXELS(1));
    ObjectEvent_WalkToAndWait(LOCALID_BANE, 880, 696);
    ObjectEvent_WalkToAndWait(LOCALID_BANE, 882, 704);
    ObjectEvent_WalkToAndWait(LOCALID_BANE, 880, 712);
    ObjectEvent_WalkToAndWait(LOCALID_BANE, 878, 720);
    ObjectEvent_WalkToAndWait(LOCALID_BANE, 880, 728);
    ObjectEvent_WalkToAndWait(LOCALID_BANE, 882, 736);
    ObjectEvent_WalkToAndWait(LOCALID_BANE, 880, 744);
    ObjectEvent_WalkToAndWait(LOCALID_BANE, 878, 752);
    ObjectEvent_WalkToAndWait(LOCALID_BANE, 880, 760);
    OBJECT_POSITION_SET(LOCALID_BANE, 856, 824);
    WAIT(10);
    OBJECT_DIR(LOCALID_BANE, FACING_NORTH, 20);
    ObjectEvent_SetAttachedEffect(LOCALID_BANE, 0x100);
    djinni = ObjectEvent_GetById(LOCALID_BANE);
#ifdef UBFIX
    djinni->update = CrossboneIsleKeys_FaceBaneTowardLeader;
#else
    djinni->update =
        (void (*)(union FieldObject *))CrossboneIsleKeys_FaceBaneTowardLeader;
#endif
    EV_END();
  }
}

/// In CrossboneIsleBane, stepping on the coord events around Bane's
/// corner (30 to 41): Bane runs to the matching spot, as fast as the leader,
/// unless Halt holds it.
static void EventScript_BaneKeepsAway(void) {
  struct ObjectEvent *djinni;
  struct ObjectEvent *leader;
  s32 targetByteOffset;
  const u8 *targets;
  s32 zByteOffset;
  struct EventWork *work;

  work = gEventWork;
  if (Engine_GameFlagIsSet(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE) != 0) {
    if (gGameState.pausedObject != LOCALID_BANE) {
      targetByteOffset = work->touchedTrigger;
      djinni = ObjectEvent_GetById(LOCALID_BANE);
      leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      djinni->speed = leader->speed;
      djinni = ObjectEvent_GetById(LOCALID_BANE);
      leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      djinni->acceleration = leader->speed;
      targetByteOffset -= 30;
      targets = (const u8 *)gCrossboneIsleKeys_BaneWalkTargets;
      // The table stores consecutive signed X/Z pixel pairs.
      targetByteOffset *= 2 * sizeof(s32);
      zByteOffset = targetByteOffset + sizeof(s32);
      Engine_ObjectEventWalkTo(LOCALID_BANE,
                               *(const s32 *)(targets + targetByteOffset),
                               *(const s32 *)(targets + zByteOffset));
    }
  }
}

/// Halt on Bane in CrossboneIsleBane: it freezes in place.
static void EventScript_HaltBane(void) {
  EV_START();
  OBJECT_MODE(LOCALID_BANE, 0);
  EV_END();
}

const struct MapEvent *Map_GetEvents(void) {
  s16 selector = gGameState.mapId;

  if (selector == (s32)&MapId_CrossboneIsleKeys) {
    return gCrossboneIsleKeysRoomEvents;
  }
  if (selector == (s32)&MapId_CrossboneIsleStatues) {
    return gCrossboneIsleStatuesEvents;
  }
  if (selector == (s32)&MapId_CrossboneIsleBane) {
    return gCrossboneIsleBaneEvents;
  }
  return gCrossboneIsleKeysEventsOther;
}

/// Unused: walking south into CrossboneIsleKeys's third or fourth blue door
/// looks at what is ahead.
///
/// Both rows that name it follow a plain coord event row for the same door
/// (EventScript_OpenThirdBlueDoor, EventScript_OpenFourthBlueDoor), which
/// Map_FindEvent always finds first; those scripts look ahead themselves after
/// a dozen steps pressed south against the door, so the player sees the same
/// thing.
static void EventScript_UnusedCheckLeaderAhead(void) {
  Engine_LeaderCheckAhead();
}

/// Opens the selected blue door, playing its opening when asked.
static void CrossboneIsleKeys_OpenBlueDoor(s32 passageInfo) {
  s32 columnOffset = (passageInfo & PASSAGE_INDEX_MASK) * 4;
  s32 tileColumn = columnOffset + 77;
  s32 collisionColumn = columnOffset + 13;

  if (passageInfo & PASSAGE_PLAY_OPENING) {
    Engine_AudioPlayCue(SE_GATE_MOVE);
    MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    MapGrid_CopyCells(79, 29, 1, 3, tileColumn, 40);
    Engine_TaskWait(40);
  }
  MapGrid_CopyCells(80, 29, 1, 3, tileColumn, 40);
  MapGrid_CopyCellAttributes(collisionColumn, 40, 1, 1, collisionColumn, 41);
  MapGrid_CopyCellAttributes(collisionColumn, 40, 1, 1, collisionColumn, 42);
}

/// Opens the red door, playing its opening when asked.
static void CrossboneIsleKeys_OpenRedDoor(s32 passageInfo) {
  if ((passageInfo & PASSAGE_PLAY_OPENING) != 0) {
    SE(SE_GATE_MOVE);
    MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    MapGrid_CopyCells(84, 29, 1, 3, 70, 49);
    Engine_TaskWait(60);
  }
  MapGrid_CopyCells(85, 29, 1, 3, 70, 49);
  MapGrid_CopyCellAttributes(6, 49, 1, 1, 6, 50);
  MapGrid_CopyCellAttributes(6, 49, 1, 1, 6, 51);
}

/// Facing CrossboneIsleKeys's red door with the Red Key (coord event
/// 50): the door rumbles open, and the key is used up.
static void EventScript_UseRedKey(void) {
  struct ObjectEvent *leader;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader->facing == FACING_NORTH) {
    if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN) == 0) {
      if (PartyInventory_FindOwner(ITEM_RED_KEY) != -1) {
        FLAG_SET(FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN);
        CrossboneIsleKeys_OpenRedDoor(PASSAGE_PLAY_OPENING);
        PartyInventory_Remove(ITEM_RED_KEY);
      }
    }
  }
}

/// Facing a blue door with a Blue Key: the door rumbles open, and the key is
/// used up.
static void CrossboneIsleKeys_UseBlueKey(s32 doorIndex) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  s32 flag;

  if (leader->facing != FACING_NORTH) {
    return;
  }
  flag = doorIndex + FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN;
  if (Engine_GameFlagIsSet(flag) != 0) {
    return;
  }
  if (PartyInventory_FindOwner(ITEM_BLUE_KEY) == -1) {
    return;
  }
  Engine_GameFlagSet(flag);
  CrossboneIsleKeys_OpenBlueDoor(PASSAGE_PLAY_OPENING | doorIndex);
  PartyInventory_Remove(ITEM_BLUE_KEY);
}

/// Touching CrossboneIsleKeys's first blue door (coord event 51).
static void EventScript_OpenFirstBlueDoor(void) {
  CrossboneIsleKeys_UseBlueKey(0);
}

/// Touching CrossboneIsleKeys's second blue door (coord event 52).
static void EventScript_OpenSecondBlueDoor(void) {
  CrossboneIsleKeys_UseBlueKey(1);
}

/// Touching CrossboneIsleKeys's third blue door (coord event 53);
/// pressing on it from the north looks ahead every dozen blocked steps.
static void EventScript_OpenThirdBlueDoor(void) {
  struct ObjectEvent *leader;
  s16 *blockedSteps;
  s32 resetSteps;

  CrossboneIsleKeys_UseBlueKey(2);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader->facing == FACING_SOUTH) {
    blockedSteps = &gEventWork->blockedSteps;
    if (*blockedSteps > 12) {
      Engine_LeaderCheckAhead();
      resetSteps = 0;
      *blockedSteps = resetSteps;
    }
  }
}

/// Touching CrossboneIsleKeys's fourth blue door (coord event 54);
/// pressing on it from the north looks ahead every dozen blocked steps.
static void EventScript_OpenFourthBlueDoor(void) {
  struct ObjectEvent *leader;
  s16 *blockedSteps;
  s32 resetSteps;

  CrossboneIsleKeys_UseBlueKey(3);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader->facing == FACING_SOUTH) {
    blockedSteps = &gEventWork->blockedSteps;
    if (*blockedSteps > 12) {
      Engine_LeaderCheckAhead();
      resetSteps = 0;
      *blockedSteps = resetSteps;
    }
  }
}

/// Reveal in CrossboneIsleKeys: the hidden Red Key appears on its
/// tile.
static void EventScript_ShowRedKey(void) {
  s32 keyObjectEventId = LOCALID_RED_KEY;
  s32 x = PIXELS(408);
  s32 z = PIXELS(776);

  MapGrid_CopyCells(25, 45, 1, 2, 25, 48);
  if (Engine_GameFlagIsSet(FLAG_CROSSBONE_ISLE_RED_KEY) == 0)
    Engine_ObjectEventSetPosition(keyObjectEventId, x, z);
  WAIT(1);
}

/// Reveal wears off in CrossboneIsleKeys: the Red Key's tile is
/// hidden again.
static void EventScript_HideRedKey(void) {
  MapGrid_CopyCells(24, 48, 1, 2, 25, 48);
  OBJECT_POSITION_SET(LOCALID_RED_KEY, 8, 8);
}

/// Collects the selected key, hides its object event and remembers it was
/// taken.
static inline void CrossboneIsleKeys_CollectKey(s32 objectEventId, s32 itemId,
                                                s32 collectedFlag) {
  Engine_ItemShowFound(itemId, 3);
  OBJECT_MODE(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Engine_PartyGiveItem(itemId, 0);
  Engine_ObjectEventSetPosition(objectEventId, 0, 0);
  Engine_GameFlagSet(collectedFlag);
}

/// Catch on CrossboneIsleKeys's first Blue Key: the party gets it.
static void EventScript_CatchFirstBlueKey(void) {
  CrossboneIsleKeys_CollectKey(LOCALID_FIRST_BLUE_KEY, ITEM_BLUE_KEY,
                               FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY);
}

/// Catch on CrossboneIsleKeys's second Blue Key: the party gets it.
static void EventScript_CatchSecondBlueKey(void) {
  CrossboneIsleKeys_CollectKey(LOCALID_SECOND_BLUE_KEY, ITEM_BLUE_KEY,
                               FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 1);
}

/// Catch on CrossboneIsleKeys's third Blue Key: the party gets it.
static void EventScript_CatchThirdBlueKey(void) {
  CrossboneIsleKeys_CollectKey(LOCALID_THIRD_BLUE_KEY, ITEM_BLUE_KEY,
                               FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 2);
}

/// Catch on CrossboneIsleKeys's fourth Blue Key: the party gets it.
static void EventScript_CatchFourthBlueKey(void) {
  CrossboneIsleKeys_CollectKey(LOCALID_FOURTH_BLUE_KEY, ITEM_BLUE_KEY,
                               FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 3);
}

/// Catch on CrossboneIsleKeys's revealed Red Key: the party gets it.
static void EventScript_CatchRedKey(void) {
  CrossboneIsleKeys_CollectKey(LOCALID_RED_KEY, ITEM_RED_KEY,
                               FLAG_CROSSBONE_ISLE_RED_KEY);
}

/// Starts the first chase when an uncloaked leader enters its area.
static void CrossboneIsleKeys_CheckFirstPursuerTrigger(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  s32 column = leader->x.fixed / PIXELS(16);
  s32 row = leader->z.fixed / PIXELS(16);
  struct EventWork *work = gEventWork;

  if (!FLAG_CHECK(PURSUER_AFTER_LEADER_FLAG) && gGameState.cloaked == 0 &&
      gGameState.pausedObject != LOCALID_FIRST_STATUE &&
      (u32)(column - 21) <= 2 && row >= 10 && row <= 11) {
    Engine_GameFlagSet(PURSUER_AFTER_LEADER_FLAG);
    work->raisedTrigger = TRIGGER_FIRST_CHASE;
  }
}

/// Raised in CrossboneIsleStatues (91): the first pursuer follows the
/// leader, then goes back home.
static void EventScript_DodgeFirstPursuer(void) {
  struct ObjectEvent *leader;

  EV_START();
  OBJECT_SPEED(LOCALID_PARTY_LEADER, PIXELS(19) / 10, PIXELS(19) / 20);
  OBJECT_SPEED(LOCALID_FIRST_STATUE, PIXELS(19) / 10, PIXELS(19) / 20);
  SE(SE_188);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetDestination(LOCALID_FIRST_STATUE, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Engine_ObjectEventWaitForMove(LOCALID_FIRST_STATUE);
  ObjectEvent_SetDestinationOffset(LOCALID_PARTY_LEADER, 0, 24);
  WAIT(4);
  SE(SE_188);
  ObjectEvent_SetDestinationOffset(LOCALID_FIRST_STATUE, 0,
                                   CROSSBONE_PURSUER_STEP_Z);
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  ObjectEvent_SetDestination(LOCALID_FIRST_STATUE, 360, 152);
  Engine_ObjectEventWaitForMove(LOCALID_FIRST_STATUE);
  EV_END();
  GameFlag_Clear(PURSUER_AFTER_LEADER_FLAG);
}

/// Starts the second chase when the leader enters column ten.
static void CrossboneIsleKeys_CheckSecondPursuerTrigger(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  s32 column = leader->x.fixed / PIXELS(16);
  s32 row = leader->z.fixed / PIXELS(16);
  struct EventWork *work = gEventWork;

  if (FLAG_CHECK(PURSUER_AFTER_LEADER_FLAG) == 0 && gGameState.cloaked == 0 &&
      gGameState.pausedObject != LOCALID_SECOND_STATUE && column == 10 &&
      (u32)(row - 16) <= 2) {
    FLAG_SET(PURSUER_AFTER_LEADER_FLAG);
    MapLocalState_StoreStep(&work->raisedTrigger, TRIGGER_SECOND_CHASE);
  }
}

/// Raised in CrossboneIsleStatues (92): the second pursuer follows the
/// leader, then goes back home.
static void EventScript_DodgeSecondPursuer(void) {
  struct ObjectEvent *leader;

  EV_START();
  OBJECT_SPEED(LOCALID_PARTY_LEADER, PIXELS(17) / 10, PIXELS(17) / 20);
  OBJECT_SPEED(LOCALID_SECOND_STATUE, PIXELS(17) / 10, PIXELS(17) / 20);
  SE(SE_188);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetDestination(LOCALID_SECOND_STATUE, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Engine_ObjectEventWaitForMove(LOCALID_SECOND_STATUE);
  ObjectEvent_SetDestinationOffset(LOCALID_PARTY_LEADER, 0,
                                   CROSSBONE_LEADER_STEP_Z);
  SE(SE_188);
  WAIT(4);
  ObjectEvent_SetDestinationOffset(LOCALID_SECOND_STATUE, 0, 16);
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  ObjectEvent_SetDestination(LOCALID_SECOND_STATUE, 168, 264);
  Engine_ObjectEventWaitForMove(LOCALID_SECOND_STATUE);
  EV_END();
  GameFlag_Clear(PURSUER_AFTER_LEADER_FLAG);
}

/// Moves a pursuer to the leader column and remembers its last column.
static void CrossboneIsleKeys_TrackPursuerColumn(s32 pursuerIndex) {

  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  s32 column = leader->x.fixed / PIXELS(16);
  s32 row = leader->z.fixed / PIXELS(16);
  s32 pursuerId = pursuerIndex + LOCALID_THIRD_STATUE;

  if (gGameState.pausedObject == pursuerId)
    return;
  if (column == gCrossboneIsleKeys_SlotColumns[pursuerIndex])
    return;

  Engine_ObjectEventSetSpeed(pursuerId, PIXELS(9) / 2, PIXELS(9) / 4);
  Engine_AudioPlayCue(SE_188);
  Engine_ObjectEventSetDestination(pursuerId, (column << 4) + 8, 360);

  gCrossboneIsleKeys_SlotColumns[pursuerIndex] = column;

  if (row <= 22) {
    Engine_ObjectEventSetDestinationOffset(LOCALID_PARTY_LEADER, 0, 8);
  }
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
}

/// Touching coord event 53 in CrossboneIsleStatues: the third pursuer keeps
/// to the leader's column.
static void EventScript_TrackThirdPursuerColumn(void) {
  CrossboneIsleKeys_TrackPursuerColumn(0);
}

/// Touching coord event 54 in CrossboneIsleStatues: the fourth pursuer
/// keeps to the leader's column.
static void EventScript_TrackFourthPursuerColumn(void) {
  CrossboneIsleKeys_TrackPursuerColumn(1);
}

/// Unused: tracks the fifth pursuer's column; nothing calls it.
UNUSED static void CrossboneIsleKeys_TrackFifthPursuerColumn(void) {
  CrossboneIsleKeys_TrackPursuerColumn(2);
}

/// Touching coord event 61 in CrossboneIsleStatues: the third pursuer
/// follows the leader, then goes back home.
static void EventScript_DodgeThirdPursuer(void) {
  struct ObjectEvent *leader;

  if (gGameState.pausedObject != LOCALID_THIRD_STATUE) {
    EV_START();
    OBJECT_SPEED(LOCALID_PARTY_LEADER, PIXELS(17) / 10, PIXELS(17) / 20);
    OBJECT_SPEED(LOCALID_THIRD_STATUE, PIXELS(17) / 10, PIXELS(17) / 20);
    SE(SE_188);
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (leader != 0) {
      ObjectEvent_SetDestination(LOCALID_THIRD_STATUE, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Engine_ObjectEventWaitForMove(LOCALID_THIRD_STATUE);
    ObjectEvent_SetDestinationOffset(LOCALID_PARTY_LEADER, 0,
                                     CROSSBONE_LEADER_STEP_Z);
    WAIT(4);
    SE(SE_188);
    ObjectEvent_SetDestinationOffset(LOCALID_THIRD_STATUE, 0, 16);
    Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
    ObjectEvent_SetDestination(LOCALID_THIRD_STATUE, 264, 360);
    Engine_ObjectEventWaitForMove(LOCALID_THIRD_STATUE);
    WAIT(10);
    EV_END();
  }
}

/// Touching coord event 62 in CrossboneIsleStatues: the fourth pursuer lines
/// up and goes after the leader.
static void EventScript_DodgeFourthPursuer(void) {
  struct ObjectEvent *leader;
  s32 leaderColumn;

  if (gGameState.pausedObject != LOCALID_FOURTH_STATUE) {
    EV_START();
    OBJECT_SPEED(LOCALID_PARTY_LEADER, PIXELS(17) / 10, PIXELS(17) / 20);
    Engine_ObjectEventSetSpeed(LOCALID_FOURTH_STATUE, PIXELS(17) / 10,
                               PIXELS(17) / 20);
    Engine_AudioPlayCue(SE_188);
    leaderColumn =
        ObjectEvent_GetById(LOCALID_PARTY_LEADER)->x.fixed / PIXELS(16);
    if (leaderColumn >
        ObjectEvent_GetById(LOCALID_FOURTH_STATUE)->x.fixed / PIXELS(16)) {
      Engine_ObjectEventSetDestinationOffset(LOCALID_FOURTH_STATUE, 8, 0);
    }
    leaderColumn =
        ObjectEvent_GetById(LOCALID_PARTY_LEADER)->x.fixed / PIXELS(16);
    if (leaderColumn <
        ObjectEvent_GetById(LOCALID_FOURTH_STATUE)->x.fixed / PIXELS(16)) {
      FieldObjectMotion_OffsetPositionAndResetMotion(LOCALID_FOURTH_STATUE, -8,
                                                     0);
    }
    Engine_ObjectEventWaitForMove(LOCALID_FOURTH_STATUE);
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (leader != 0) {
      Engine_ObjectEventSetDestination(
          LOCALID_FOURTH_STATUE, leader->x.part.pixel, leader->z.part.pixel);
    }
    Engine_ObjectEventWaitForMove(LOCALID_FOURTH_STATUE);
    Engine_ObjectEventSetDestinationOffset(LOCALID_PARTY_LEADER, 0,
                                           CROSSBONE_LEADER_STEP_Z);
    WAIT(4);
    Engine_AudioPlayCue(SE_188);
    Engine_ObjectEventSetDestinationOffset(LOCALID_FOURTH_STATUE, 0, 16);
    Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
    Engine_ObjectEventSetDestination(LOCALID_FOURTH_STATUE, 344, 360);
    Engine_ObjectEventWaitForMove(LOCALID_FOURTH_STATUE);
    WAIT(10);
    EV_END();
  }
}

/// Starts the fifth chase when the leader enters column nineteen.
static void CrossboneIsleKeys_CheckFifthPursuerTrigger(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  s32 column = leader->x.fixed / PIXELS(16);
  s32 row = leader->z.fixed / PIXELS(16);
  struct EventWork *work = gEventWork;
  struct GameState *state = &gGameState;

  if (state->pausedObject != LOCALID_FIFTH_STATUE &&
      FLAG_CHECK(PURSUER_AFTER_LEADER_FLAG) == 0 && state->cloaked == 0 &&
      column == 19 && (u32)(row - 15) <= 1) {
    FLAG_SET(PURSUER_AFTER_LEADER_FLAG);
    MapLocalState_StoreStep(&work->raisedTrigger, TRIGGER_FIFTH_CHASE);
  }
}

/// Raised in CrossboneIsleStatues (96): the fifth pursuer follows the
/// leader, then goes back home.
static void EventScript_DodgeFifthPursuer(void) {
  struct ObjectEvent *leader;

  EV_START();
  OBJECT_SPEED(LOCALID_PARTY_LEADER, PIXELS(17) / 10, PIXELS(17) / 20);
  OBJECT_SPEED(LOCALID_FIFTH_STATUE, PIXELS(17) / 10, PIXELS(17) / 20);
  SE(SE_188);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetDestination(LOCALID_FIFTH_STATUE, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Engine_ObjectEventWaitForMove(LOCALID_FIFTH_STATUE);
  ObjectEvent_SetDestinationOffset(LOCALID_PARTY_LEADER, 0,
                                   CROSSBONE_LEADER_STEP_Z);
  SE(SE_188);
  ObjectEvent_SetDestinationOffset(LOCALID_FIFTH_STATUE, 0, 16);
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  ObjectEvent_SetDestination(LOCALID_FIFTH_STATUE, 312, 232);
  Engine_ObjectEventWaitForMove(LOCALID_FIFTH_STATUE);
  EV_END();
  GameFlag_Clear(PURSUER_AFTER_LEADER_FLAG);
}

/// Reveal in CrossboneIsleStatues: the translucent layer over the room
/// vanishes while it lasts.
static void EventScript_StartRevealBlend(void) {
  u32 coefficient = 208;

  coefficient <<= 4;
  REG_BLDALPHA = coefficient;
}

/// Reveal wears off in CrossboneIsleStatues: the translucent layer
/// returns.
static void EventScript_EndRevealBlend(void) {
  u16 value = 0x0607;

  REG_BLDALPHA = value;
}

/// At CrossboneIsleBane's ladder by the second pillar (coord event 60):
/// climb up or down, look ahead to the east, or push the first pillar from the
/// floor to the west.
static void EventScript_UseLadder(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  u16 facing = leader->facing;

  if (facing == FACING_NORTH) {
    if (Engine_GameFlagIsSet(BANE_CHEST_AT_LADDER_FLAG) != 0) {
      {
        s32 destinationColumn = 45;
        s32 destinationRow = 43;
        MapGrid_CopyCellAttributes(46, 43, 1, 1, destinationColumn,
                                   destinationRow);
      }
    }
    Engine_GameFlagClear(BANE_LOOKED_EAST_FLAG);
    Field_ClimbUp();
  } else if (facing == FACING_SOUTH) {
    Field_ClimbDown();
  } else if (facing == FACING_EAST) {
    if (Engine_GameFlagIsSet(BANE_CHEST_AT_LADDER_FLAG) != 0) {
      {
        s32 destinationColumn = 45;
        s32 destinationRow = 43;
        MapGrid_CopyCellAttributes(58, 36, 1, 1, destinationColumn,
                                   destinationRow);
      }
    }
    Engine_GameFlagSet(BANE_LOOKED_EAST_FLAG);
    Engine_LeaderCheckAhead();
  } else if (facing == FACING_WEST) {
    if (leader->y.fixed == 0) {
      EventScript_PushFirstPillar();
    } else {
      Engine_LeaderCheckAhead();
    }
  }
}

/// Unused: an empty function nothing calls.
UNUSED static void CrossboneIsleKeys_UnusedCallback(void) {}

/// Frost on CrossboneIsleBane's puddle: it freezes into a pillar of
/// ice the party can stand on.
static void EventScript_FreezePuddle(void) {
  struct ObjectEvent *puddle = ObjectEvent_GetById(LOCALID_PUDDLE);

  MapGrid_CopyCellAttributes(40, 54, 1, 1, 40, 55);
  if (puddle != 0) {
    u8 *motionFlags = &ObjectEvent_GetById(LOCALID_PUDDLE)->motionFlags;
    u8 *priorityFlags = &puddle->priorityFlags;

    *motionFlags = 0;
    *priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
  }
  Engine_GameFlagSet(BANE_PUDDLE_FROZEN_FLAG);
}

/// Walking into CrossboneIsleBane's first pillar from the ladder side
/// (coord event 70): facing east, look ahead; otherwise push the pillar.
static void EventScript_LookAheadOrPushFirstPillar(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

  if (leader->facing == 0) {
    Engine_LeaderCheckAhead();
  } else {
    EventScript_PushFirstPillar();
  }
}

/// Restores the first pillar row before applying its current footprint.
static void CrossboneIsleKeys_RestoreFirstPillarTerrain(void) {
  MapGrid_CopyCellAttributes(57, 42, 1, 1, 40, 42);
  MapGrid_CopyCellAttributes(57, 42, 1, 1, 41, 42);
  MapGrid_CopyCellAttributes(58, 42, 1, 1, 42, 42);
  MapGrid_CopyCellAttributes(62, 37, 3, 1, 37, 42);

  ObjectEvent_GetById(LOCALID_FIRST_PILLAR)->motionFlags =
      OBJ_EVENT_FOLLOWS_TERRAIN;
}

/// Restores support under the first pillar at the eastern cell.
static void CrossboneIsleKeys_RestoreEastUnderfootCell(void) {

  MapGrid_CopyCellAttributes(58, 41, 1, 1, 42, 42);
  ObjectEvent_GetById(LOCALID_FIRST_PILLAR)->priorityFlags =
      OBJ_EVENT_PRIORITY_UNDERFOOT;
}

/// Restores support under the first pillar at the middle cell.
static void CrossboneIsleKeys_RestoreMiddleUnderfootCell(void) {

  MapGrid_CopyCellAttributes(44, 42, 1, 1, 41, 42);
  ObjectEvent_GetById(LOCALID_FIRST_PILLAR)->priorityFlags =
      OBJ_EVENT_PRIORITY_UNDERFOOT;
}

/// Restores support under the first pillar at the western cell.
static void CrossboneIsleKeys_RestoreWestUnderfootCell(void) {

  MapGrid_CopyCellAttributes(39, 42, 1, 1, 40, 42);
  ObjectEvent_GetById(LOCALID_FIRST_PILLAR)->priorityFlags =
      OBJ_EVENT_PRIORITY_UNDERFOOT;
}

/// Move on CrossboneIsleBane's first pillar: its support cells are
/// rebuilt from its height and column.
static void EventScript_UpdateFirstPillarTerrain(void) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_FIRST_PILLAR);
  s32 x = objectEvent->x.fixed;
  s32 height = objectEvent->y.fixed;
  s32 column = x / PIXELS(16);
  u8 lowered;

  if (height == 0) {
    ObjectEvent_GetById(LOCALID_FIRST_PILLAR)->priorityFlags =
        OBJ_EVENT_PRIORITY_UNDERFOOT;
  }
  CrossboneIsleKeys_RestoreFirstPillarTerrain();
  lowered = 0;
  ObjectEvent_GetById(LOCALID_FIRST_PILLAR)->motionFlags =
      OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
  if (column == 40) {
    CrossboneIsleKeys_RestoreWestUnderfootCell();
  } else if (column == 42) {
    CrossboneIsleKeys_RestoreEastUnderfootCell();
  } else if (column == 41) {
    CrossboneIsleKeys_RestoreMiddleUnderfootCell();
  } else if (column == 39) {
    LiftPillar(column, lowered);
  } else if (column == 38) {
    LiftPillar(column, lowered);
  } else if (column == 37) {
    LiftPillar(column, lowered);
  }
}

/// Pushing CrossboneIsleBane's first pillar (coord event 51); standing at
/// the ladder in column 38, the leader climbs instead.
static void EventScript_PushFirstPillar(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  struct ObjectEvent *pillar = ObjectEvent_GetById(LOCALID_FIRST_PILLAR);
  s32 leaderColumn = leader->x.fixed / PIXELS(16);
  s32 pillarColumn = pillar->x.fixed / PIXELS(16);

  if (leaderColumn == 38 && pillarColumn != 38) {
    u16 facing = leader->facing;

    if (facing == FACING_NORTH) {
      Field_ClimbUp();
      return;
    }
    if (facing == FACING_SOUTH) {
      Field_ClimbDown();
      return;
    }
  }

  CrossboneIsleKeys_RestoreFirstPillarTerrain();
  StagedObjectEvent_AdvancePair();
  EventScript_UpdateFirstPillarTerrain();
}

/// Leaves the chest where the second pillar has reached the ladder: the chest's
/// object event is hidden, its bg event takes its place and the ladder cell is
/// restored.
static void CrossboneIsleKeys_ReleaseSecondPillarChest(void) {
  Scheduler_RemoveCallback(CrossboneIsleKeys_FollowSecondPillar);
  OBJECT_POSITION_SET(LOCALID_SECOND_PILLAR_CHEST, 0, 0);
  if (FLAG_CHECK(BANE_LOOKED_EAST_FLAG) != 0) {
    MapGrid_CopyCellAttributes(58, 36, 1, 1, 45, 43);
  } else {
    MapGrid_CopyCellAttributes(46, 43, 1, 1, 45, 43);
  }
  CrossboneIsleKeys_PositionSecondPillarMarker();
  FLAG_SET(BANE_CHEST_AT_LADDER_FLAG);
}

/// Move on CrossboneIsleBane's second pillar: the chest on top of it stays
/// at the ladder cell once the pillar reaches it.
static void EventScript_UpdateSecondPillarTerrain(void) {
  struct ObjectEvent *pillar = ObjectEvent_GetById(LOCALID_SECOND_PILLAR);
  s32 column = pillar->x.fixed / PIXELS(16);
  s32 row = pillar->z.fixed / PIXELS(16);

  if (column == 45 && row == 43) {
    CrossboneIsleKeys_ReleaseSecondPillarChest();
  }
}

/// Pushing CrossboneIsleBane's second pillar (coord event 52); at the ladder,
/// the chest on top of it stays there.
static void EventScript_PushSecondPillar(void) {
  StagedObjectEvent_AdvancePair();
  EventScript_UpdateSecondPillarTerrain();
}

/// Restores the terrain at both ends of the third pillar row.
static void CrossboneIsleKeys_RestoreThirdPillarTerrain(void) {

  MapGrid_CopyCellAttributes(38, 56, 1, 1, 38, 55);
  MapGrid_CopyCellAttributes(42, 56, 1, 1, 42, 55);
}

/// Restores the third pillar support at the western end.
static void CrossboneIsleKeys_RestoreThirdPillarWestCell(void) {

  MapGrid_CopyCellAttributes(40, 54, 1, 1, 38, 55);
  ObjectEvent_GetById(LOCALID_THIRD_PILLAR)->motionFlags = 0;
}

/// Restores the third pillar support at the eastern end.
static void CrossboneIsleKeys_RestoreThirdPillarEastCell(void) {

  MapGrid_CopyCellAttributes(40, 54, 1, 1, 42, 55);
  ObjectEvent_GetById(LOCALID_THIRD_PILLAR)->motionFlags = 0;
}

/// Move on CrossboneIsleBane's third pillar: its support is rebuilt
/// from its height and destination.
static void EventScript_UpdateThirdPillarTerrain(void) {
  struct ObjectEvent *pillar = ObjectEvent_GetById(LOCALID_THIRD_PILLAR);
  s32 heightCells = pillar->y.fixed / PIXELS(16);
  s32 column = pillar->x.fixed / PIXELS(16);
  s32 row = pillar->z.fixed / PIXELS(16);

  if (!FLAG_CHECK(FLAG_CROSSBONE_ISLE_THIRD_PILLAR_LOWERED) &&
      heightCells <= 2) {
    CrossboneIsleKeys_RestoreThirdPillarWestCell();
    Engine_GameFlagSet(FLAG_CROSSBONE_ISLE_THIRD_PILLAR_LOWERED);
  }
  if (row == 55) {
    if (column == 42) {
      CrossboneIsleKeys_RestoreThirdPillarEastCell();
    }
    if (column == 38) {
      CrossboneIsleKeys_RestoreThirdPillarWestCell();
    }
  } else {
    CrossboneIsleKeys_RestoreThirdPillarTerrain();
  }
}

/// Pushing CrossboneIsleBane's third pillar (coord event 50).
static void EventScript_PushThirdPillar(void) {
  CrossboneIsleKeys_RestoreThirdPillarTerrain();
  StagedObjectEvent_AdvancePair();
  EventScript_UpdateThirdPillarTerrain();
}

/// Walking into CrossboneIsleBane's third pillar (coord event 54): facing
/// south, the rising sequence plays; otherwise the pillar is pushed.
static void EventScript_TouchThirdPillar(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

  if (leader->facing == FACING_SOUTH) {
    FieldObjectEvent_JumpDownLedge(0, 6, 0);
  } else {
    EventScript_PushThirdPillar();
  }
}

/// Clears the support row before repainting the fourth and fifth pillars.
static void CrossboneIsleKeys_RestoreFourthAndFifthPillarTerrain(void) {

  MapGrid_CopyCellAttributes(48, 55, 1, 1, 49, 55);
  MapGrid_CopyCellAttributes(48, 55, 1, 1, 50, 55);
  MapGrid_CopyCellAttributes(48, 55, 1, 1, 51, 55);
  MapGrid_CopyCellAttributes(48, 55, 1, 1, 52, 55);
}

/// Places support cells beneath the fourth and fifth pillars.
static void CrossboneIsleKeys_RepaintFourthAndFifthPillarCells(void) {
  struct ObjectEvent *pillar;
  s32 column;

  CrossboneIsleKeys_RestoreFourthAndFifthPillarTerrain();

  pillar = ObjectEvent_GetById(LOCALID_FOURTH_PILLAR);
  column = pillar->x.fixed / PIXELS(16);
  MapGrid_CopyCellAttributes(53, 55, 1, 1, column, 55);
  pillar = ObjectEvent_GetById(LOCALID_FIFTH_PILLAR);
  column = pillar->x.fixed / PIXELS(16);
  MapGrid_CopyCellAttributes(53, 55, 1, 1, column, 55);
}

/// Move on CrossboneIsleBane's fourth pillar: the support row it
/// shares with the fifth is refreshed.
static void EventScript_UpdateFourthPillarTerrain(void) {
  CrossboneIsleKeys_RepaintFourthAndFifthPillarCells();
}

/// Pushing CrossboneIsleBane's fourth pillar (coord event 53).
static void EventScript_PushFourthPillar(void) {
  CrossboneIsleKeys_RestoreFourthAndFifthPillarTerrain();
  StagedObjectEvent_AdvancePair();
  EventScript_UpdateFourthPillarTerrain();
}

/// Move on CrossboneIsleBane's fifth pillar: the support row it shares
/// with the fourth is refreshed.
static void EventScript_UpdateFifthPillarTerrain(void) {
  CrossboneIsleKeys_RepaintFourthAndFifthPillarCells();
}

/// Unused: a callback that moves the fifth pillar and refreshes its terrain;
/// nothing calls it.
UNUSED static void CrossboneIsleKeys_MoveFifthPillar(void) {
  CrossboneIsleKeys_RestoreFourthAndFifthPillarTerrain();
  StagedObjectEvent_AdvancePair();
  EventScript_UpdateFifthPillarTerrain();
}

/// Keeps the chest on top of the second pillar as the pillar moves.
static void CrossboneIsleKeys_FollowSecondPillar(void) {
  struct ObjectEvent *target = ObjectEvent_GetById(LOCALID_SECOND_PILLAR_CHEST);
  struct ObjectEvent *source = ObjectEvent_GetById(LOCALID_SECOND_PILLAR);

  target->y.fixed = PIXELS(32);
  target->x.fixed = source->x.fixed;
  target->z.fixed = source->z.fixed + PIXELS(1);
}

/// Puts the chest's bg event 107 where the second pillar stands.
static void CrossboneIsleKeys_PositionSecondPillarMarker(void) {
  struct ObjectEvent *frame = ObjectEvent_GetById(LOCALID_SECOND_PILLAR);

  Engine_MarkerSetPosition(107, frame->x.fixed, frame->z.fixed + PIXELS(1));
}

/// Chooses pillar drawing priorities from the leader height and row.
static void CrossboneIsleKeys_UpdatePillarPriorities(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  u8 priority;

  if (leader->y.fixed > PIXELS(16)) {
    priority = OBJ_EVENT_PRIORITY_UNDERFOOT;
    ObjectEvent_GetById(LOCALID_FIRST_PILLAR)->priorityFlags = priority;
    if (ObjectEvent_GetById(LOCALID_THIRD_PILLAR)->y.fixed == 0)
      ObjectEvent_GetById(LOCALID_THIRD_PILLAR)->priorityFlags = priority;
    ObjectEvent_GetById(LOCALID_FOURTH_PILLAR)->priorityFlags = priority;
  } else {
    if (ObjectEvent_GetById(LOCALID_THIRD_PILLAR)->y.fixed == 0 &&
        ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.fixed / PIXELS(16) > 56) {
      Engine_ObjectEventSetSpritePriority(LOCALID_THIRD_PILLAR, 3);
    } else {
      Engine_ObjectEventSetSpritePriority(LOCALID_THIRD_PILLAR, 1);
      ObjectEvent_GetById(LOCALID_THIRD_PILLAR)->priorityFlags =
          OBJ_EVENT_PRIORITY_AUTOMATIC;
    }
    priority = 0;
    ObjectEvent_GetById(LOCALID_FOURTH_PILLAR)->priorityFlags = priority;
  }
  ObjectEvent_GetById(LOCALID_FIFTH_PILLAR)->priorityFlags = priority;
}

/// Sets up whichever of the three rooms the party enters: the keys and doors
/// of CrossboneIsleKeys, the pursuers of CrossboneIsleStatues,
/// or the pillars and Bane of CrossboneIsleBane.
s32 Map_Initialize(void) {
  gEventWork->startTransition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gGameState.mapId == (s32)&MapId_CrossboneIsleKeys) {
    CrossboneIsleKeys_InitializeKeys();
  }
  if (gGameState.mapId == (s32)&MapId_CrossboneIsleStatues) {
    CrossboneIsleKeys_InitializeStatues();
  }
  if (gGameState.mapId == (s32)&MapId_CrossboneIsleBane) {
    CrossboneIsleKeys_InitializeBane();
  }
  return 0;
}

/// Unused: the Blue Keys still lying about would have flashed their icons. In
/// CrossboneIsleKeys they would have shown their item icons one after another,
/// every 16 frames.
///
/// Nothing adds it as a callback, so the keys never flash.
UNUSED static void CrossboneIsleKeys_UnusedUpdateKeyIcons(void) {
  if (++gCrossboneIsleKeys_IconTimer > 16) {
    gCrossboneIsleKeys_IconTimer = 0;
  }
  switch (gCrossboneIsleKeys_IconTimer) {
  case 12:
    if (Engine_GameFlagIsSet(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY) == 0) {
      OBJECT_POSITION_SET(LOCALID_FIRST_BLUE_KEY, 232, 872);
    }
    if (Engine_GameFlagIsSet(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 1) == 0) {
      OBJECT_POSITION_SET(LOCALID_SECOND_BLUE_KEY, 296, 824);
    }
    if (Engine_GameFlagIsSet(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 2) == 0) {
      OBJECT_POSITION_SET(LOCALID_THIRD_BLUE_KEY, 328, 760);
    }
    if (Engine_GameFlagIsSet(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 3) == 0) {
      OBJECT_POSITION_SET(LOCALID_FOURTH_BLUE_KEY, 360, 872);
    }
    break;
  case 10:
    Colosso_ShowItemIcon(LOCALID_FIRST_BLUE_KEY, 0);
    break;
  case 8:
    Colosso_ShowItemIcon(LOCALID_SECOND_BLUE_KEY, 0);
    break;
  case 6:
    Colosso_ShowItemIcon(LOCALID_THIRD_BLUE_KEY, 0);
    break;
  case 4:
    Colosso_ShowItemIcon(LOCALID_FOURTH_BLUE_KEY, 0);
    break;
  case 2:
    Colosso_ShowItemIcon(LOCALID_RED_KEY, 1);
    break;
  }
}

#include "common/colosso_item_icon.h"

/// Restores collected keys and already opened passages.
static void CrossboneIsleKeys_InitializeKeys(void) {
  Engine_TaskWait(1);
  Colosso_ShowItemIcon(LOCALID_RED_KEY, ITEM_RED_KEY);
  Colosso_ShowItemIcon(LOCALID_FOURTH_BLUE_KEY, ITEM_BLUE_KEY);
  Colosso_ShowItemIcon(LOCALID_THIRD_BLUE_KEY, ITEM_BLUE_KEY);
  Colosso_ShowItemIcon(LOCALID_SECOND_BLUE_KEY, ITEM_BLUE_KEY);
  Colosso_ShowItemIcon(LOCALID_FIRST_BLUE_KEY, ITEM_BLUE_KEY);
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY) == 0) {
    OBJECT_POSITION_SET(LOCALID_FIRST_BLUE_KEY, 232, 872);
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 1) == 0) {
    OBJECT_POSITION_SET(LOCALID_SECOND_BLUE_KEY, 296, 824);
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 2) == 0) {
    OBJECT_POSITION_SET(LOCALID_THIRD_BLUE_KEY, 328, 760);
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 3) == 0) {
    OBJECT_POSITION_SET(LOCALID_FOURTH_BLUE_KEY, 360, 872);
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN) != 0) {
    CrossboneIsleKeys_OpenBlueDoor(0);
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN + 1) != 0) {
    CrossboneIsleKeys_OpenBlueDoor(1);
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN + 2) != 0) {
    CrossboneIsleKeys_OpenBlueDoor(2);
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN + 3) != 0) {
    CrossboneIsleKeys_OpenBlueDoor(3);
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN) != 0) {
    CrossboneIsleKeys_OpenRedDoor(0);
  }
}

/// Configures the pursuers, their chase triggers and blending.
static void CrossboneIsleKeys_InitializeStatues(void) {
  ObjectEvent_GetById(LOCALID_FIRST_STATUE)->collisionFlags = OBJ_EVENT_SOLID;
  ObjectEvent_GetById(LOCALID_SECOND_STATUE)->collisionFlags = OBJ_EVENT_SOLID;
  ObjectEvent_GetById(LOCALID_THIRD_STATUE)->collisionFlags = OBJ_EVENT_SOLID;
  ObjectEvent_GetById(LOCALID_FOURTH_STATUE)->collisionFlags = OBJ_EVENT_SOLID;
  ObjectEvent_GetById(LOCALID_FIRST_STATUE)->scaleX = 7 * Q16_ONE / 10;
  ObjectEvent_GetById(LOCALID_SECOND_STATUE)->scaleX = 7 * Q16_ONE / 10;
  ObjectEvent_GetById(LOCALID_THIRD_STATUE)->scaleX = 7 * Q16_ONE / 10;
  ObjectEvent_GetById(LOCALID_FOURTH_STATUE)->scaleX = 7 * Q16_ONE / 10;
  ObjectEvent_GetById(LOCALID_FIFTH_STATUE)->scaleX = 7 * Q16_ONE / 10;
  FieldScheduler_AddOrUpdateCallback(CrossboneIsleKeys_CheckFifthPursuerTrigger,
                                     TASK_PRIORITY_MAP);
  FieldScheduler_AddOrUpdateCallback(
      CrossboneIsleKeys_CheckSecondPursuerTrigger, TASK_PRIORITY_MAP);
  FieldScheduler_AddCallback(CrossboneIsleKeys_CheckFirstPursuerTrigger,
                             TASK_PRIORITY_MAP);
  Io_Write16(0x3f42, &REG_BLDCNT);
  Io_Write16(0x607, &REG_BLDALPHA);
}

/// Restores movable pillars, terrain and the Djinni escape progress.
static void CrossboneIsleKeys_InitializeBane(void) {
  struct ObjectEvent *djinni;

  // Put the chest back on the second pillar and rebuild all pillar support
  // cells.
  ObjectEvent_GetById(LOCALID_SECOND_PILLAR_CHEST)->motionFlags = 0;
  FieldScheduler_AddOrUpdateCallback(CrossboneIsleKeys_FollowSecondPillar,
                                     TASK_PRIORITY_MAP);
  FieldScheduler_AddCallback(CrossboneIsleKeys_UpdatePillarPriorities,
                             TASK_PRIORITY_MAP);
  Marker_SetPosition(107, 0, 0);
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_DEMON_AXE) != 0) {
    OBJECT_MODE(LOCALID_SECOND_PILLAR_CHEST, 2);
  }
  EventScript_UpdateFirstPillarTerrain();
  EventScript_UpdateSecondPillarTerrain();
  EventScript_UpdateThirdPillarTerrain();
  EventScript_UpdateFourthPillarTerrain();
  EventScript_UpdateFifthPillarTerrain();
#if TBS_PATCH_1
  Engine_ObjectEventSetSpritePriority(LOCALID_FIRST_PILLAR, 3);
#endif
  ObjectEvent_GetById(LOCALID_FOURTH_PILLAR)->motionFlags = 0;
  ObjectEvent_GetById(LOCALID_FIFTH_PILLAR)->motionFlags = 0;
  CrossboneIsleKeys_RepaintFourthAndFifthPillarCells();
  if (FLAG_CHECK(BANE_PUDDLE_FROZEN_FLAG) != 0) {
    EventScript_FreezePuddle();
    OBJECT_MODE(LOCALID_PUDDLE, 5);
  }

  // Restore the surviving Djinni at the last completed escape stage.
  if (FLAG_CHECK(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE) != 0) {
      OBJECT_POSITION_SET(LOCALID_BANE, 856, 824);
      djinni = ObjectEvent_GetById(LOCALID_BANE);
#ifdef UBFIX
      djinni->update = CrossboneIsleKeys_FaceBaneTowardLeader;
#else
      djinni->update =
          (void (*)(union FieldObject *))CrossboneIsleKeys_FaceBaneTowardLeader;
#endif
    } else if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE) != 0) {
      OBJECT_POSITION_SET(LOCALID_BANE, 888, 664);
      djinni = ObjectEvent_GetById(LOCALID_BANE);
      djinni->sprite->rotation = 0;
      ObjectDispatch_SetAnimationSpeed((struct DispatchObject *)djinni, 16);
    } else if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE) != 0) {
      OBJECT_POSITION_SET(LOCALID_BANE, 584, 680);
    } else {
      OBJECT_POSITION_SET(LOCALID_BANE, 584, 664);
    }
  }
}

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))

s32 gStagedObjectEvent_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gStagedObjectEvent_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedObjectEvent_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

s32 gCrossboneIsleKeys_BaneSquashScript[] = {
    SC_ADD(SW_SCALE_X, -Q16_ONE / 4),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 8),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, -Q16_ONE / 4),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 8),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(64),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gCrossboneIsleKeys_BaneWalkTargets[] = {
    888, 856,
    872, 856,
    856, 856,
    840, 856,
    824, 856,
    824, 840,
    824, 824,
    840, 824,
    856, 824,
    872, 824,
    888, 824,
    888, 840,
};

struct MapWarpDest gCrossboneIsleKeysWarpDestsOther[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gCrossboneIsleKeysRoomWarpDests[] = {
    { CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_THIRD_FLOOR_EAST, CONDITION_ALWAYS, 200, 0, 968, FACING_NORTH, 0, 24, 560, 504, 1008, 0 },
    { CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_THIRD_FLOOR_WEST, CONDITION_ALWAYS, 88, 0, 968, FACING_NORTH, 0, 24, 560, 504, 1008, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gCrossboneIsleStatuesWarpDests[] = {
    { CROSSBONE_ISLE_STATUES_FROM_CROSSBONE_ISLE_FOURTH_FLOOR_EAST, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 40, 0, 464, 440, 0 },
    { CROSSBONE_ISLE_STATUES_FROM_CROSSBONE_ISLE_FOURTH_FLOOR_WEST, CONDITION_ALWAYS, 88, 0, 392, FACING_NORTH, 0, 40, 0, 464, 440, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gCrossboneIsleBaneWarpDests[] = {
    { CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_SIXTH_FLOOR_EAST, CONDITION_ALWAYS, 776, 0, 952, FACING_NORTH, 0, 512, 544, 928, 992, 0 },
    { CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_SIXTH_FLOOR_WEST, CONDITION_ALWAYS, 664, 0, 952, FACING_NORTH, 0, 512, 544, 928, 992, 0 },
    { MAP_TABLE_END },
};

u32 gCrossboneIsleKeysWarps[] = {
    MAP_WARPS(CrossboneIsleKeys),
    MAP_WARP(CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_THIRD_FLOOR_EAST, CrossboneIsleThirdFloor, CROSSBONE_ISLE_THIRD_FLOOR_FROM_CROSSBONE_ISLE_KEYS_EAST),
    MAP_WARP(CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_THIRD_FLOOR_WEST, CrossboneIsleThirdFloor, CROSSBONE_ISLE_THIRD_FLOOR_FROM_CROSSBONE_ISLE_KEYS_WEST),
    MAP_WARPS(CrossboneIsleStatues),
    MAP_WARP(CROSSBONE_ISLE_STATUES_TO_CROSSBONE_ISLE_FOURTH_FLOOR_EAST, CrossboneIsleFourthFloor, CROSSBONE_ISLE_FOURTH_FLOOR_FROM_CROSSBONE_ISLE_STATUES_EAST),
    MAP_WARP(CROSSBONE_ISLE_STATUES_TO_CROSSBONE_ISLE_FOURTH_FLOOR_WEST, CrossboneIsleFourthFloor, CROSSBONE_ISLE_FOURTH_FLOOR_FROM_CROSSBONE_ISLE_STATUES_WEST),
    MAP_WARPS(CrossboneIsleBane),
    MAP_WARP(CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_SIXTH_FLOOR_EAST, CrossboneIsleSixthFloor, CROSSBONE_ISLE_SIXTH_FLOOR_FROM_CROSSBONE_ISLE_BANE_EAST),
    MAP_WARP(CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_SIXTH_FLOOR_WEST, CrossboneIsleSixthFloor, CROSSBONE_ISLE_SIXTH_FLOOR_FROM_CROSSBONE_ISLE_BANE_WEST),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gCrossboneIsleKeysObjectEventTemplatesOther[] = {
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gCrossboneIsleKeysRoomObjectEventTemplates[] = {
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gCrossboneIsleStatuesObjectEventTemplates[] = {
    { SPRITE_LADY_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(360), 0, PIXELS(152), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LADY_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LADY_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LADY_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LADY_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(312), 0, PIXELS(232), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gCrossboneIsleBaneObjectEventTemplates[] = {
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(616), 0, PIXELS(680), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(728), 0, PIXELS(744), FACING_SOUTH, TALK_KEEP_FACING, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(600), 0, PIXELS(856), FACING_SOUTH, TALK_KEEP_FACING, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(824), 0, PIXELS(888), FACING_SOUTH, TALK_KEEP_FACING, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(840), 0, PIXELS(888), FACING_SOUTH, TALK_KEEP_FACING, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(648), 0, PIXELS(888), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(728), 0, PIXELS(760), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    // Bane, until it is caught
    { VENUS_DJINNI, FLAG_DJINNI(DJINNI_BANE), MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gCrossboneIsleKeysEventsOther[] = {
    { MAP_EVENTS_END },
};

struct MapEvent gCrossboneIsleKeysRoomEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_THIRD_FLOOR_EAST },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_THIRD_FLOOR_WEST },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ShowRedKey) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HideRedKey) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_CATCH), LOCALID_FIRST_BLUE_KEY, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_CatchFirstBlueKey) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_CATCH), LOCALID_SECOND_BLUE_KEY, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_CatchSecondBlueKey) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_CATCH), LOCALID_THIRD_BLUE_KEY, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_CatchThirdBlueKey) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_CATCH), LOCALID_FOURTH_BLUE_KEY, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_CatchFourthBlueKey) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_CATCH), LOCALID_RED_KEY, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_CatchRedKey) },
    { MAP_EVENT_COORD, 50, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_UseRedKey) },
    { MAP_EVENT_COORD, 51, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenFirstBlueDoor) },
    { MAP_EVENT_COORD, 52, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenSecondBlueDoor) },
    { MAP_EVENT_COORD, 53, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenThirdBlueDoor) },
    { MAP_EVENT_COORD, 54, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenFourthBlueDoor) },
    // Unused: the plain coord event rows above answer codes 53 and 54 first.
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 53, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_UnusedCheckLeaderAhead) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 54, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_UnusedCheckLeaderAhead) },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_CROSSBONE_ISLE_KEYS_333_COINS, FIND_COINS(333) },
    { BG_EVENT(BG_EVENT_CHEST), 101, FLAG_CROSSBONE_ISLE_KEYS_FAIRY_RING, FIND_ITEM(ITEM_FAIRY_RING) },
    { BG_EVENT(BG_EVENT_CHEST), 102, FLAG_CROSSBONE_ISLE_KEYS_COOKIE, FIND_ITEM(ITEM_COOKIE) },
    { BG_EVENT(BG_EVENT_CHEST), 103, FLAG_CROSSBONE_ISLE_KEYS_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { MAP_EVENTS_END },
};

struct MapEvent gCrossboneIsleStatuesEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_STATUES_TO_CROSSBONE_ISLE_FOURTH_FLOOR_EAST },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_STATUES_TO_CROSSBONE_ISLE_FOURTH_FLOOR_WEST },
    { MAP_EVENT_COORD, 53, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TrackThirdPursuerColumn) },
    { MAP_EVENT_COORD, 54, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TrackFourthPursuerColumn) },
    { MAP_EVENT_COORD, 61, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_DodgeThirdPursuer) },
    { MAP_EVENT_COORD, 62, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_DodgeFourthPursuer) },
    { MAP_EVENT_RAISED, TRIGGER_FIFTH_CHASE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_DodgeFifthPursuer) },
    { MAP_EVENT_RAISED, TRIGGER_SECOND_CHASE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_DodgeSecondPursuer) },
    { MAP_EVENT_RAISED, TRIGGER_FIRST_CHASE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_DodgeFirstPursuer) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_StartRevealBlend) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EndRevealBlend) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_HALT), LOCALID_FIRST_STATUE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HaltPursuer) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_HALT), LOCALID_SECOND_STATUE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HaltPursuer) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_HALT), LOCALID_THIRD_STATUE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HaltPursuer) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_HALT), LOCALID_FOURTH_STATUE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HaltPursuer) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_HALT), LOCALID_FIFTH_STATUE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HaltPursuer) },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_CROSSBONE_ISLE_STATUES_444_COINS, FIND_COINS(444) },
    { BG_EVENT(BG_EVENT_CHEST), 101, FLAG_CROSSBONE_ISLE_STATUES_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { BG_EVENT(BG_EVENT_CHEST), 102, FLAG_CROSSBONE_ISLE_STATUES_STORM_GEAR, FIND_ITEM(ITEM_STORM_GEAR) },
    { BG_EVENT(BG_EVENT_CHEST), 103, FLAG_CROSSBONE_ISLE_STATUES_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    { MAP_EVENTS_END },
};
// clang-format on

struct MapEvent gCrossboneIsleBaneEvents[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS,
     CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_SIXTH_FLOOR_EAST},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS,
     CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_SIXTH_FLOOR_WEST},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 50, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_PushThirdPillar)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 51, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_PushFirstPillar)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 52, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_PushSecondPillar)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 53, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_PushFourthPillar)},
#if TBS_PATCH_1
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 61, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_PushFirstPillar)},
#endif
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 54, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_TouchThirdPillar)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 60, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_UseLadder)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 70, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_LookAheadOrPushFirstPillar)},
    {MAP_EVENT_COORD, 20, FLAG_DJINNI(DJINNI_BANE),
     EVENT_SCRIPT(EventScript_BaneFirstEscape)},
    {MAP_EVENT_COORD, 21, FLAG_DJINNI(DJINNI_BANE),
     EVENT_SCRIPT(EventScript_BaneSecondEscape)},
    {MAP_EVENT_COORD, 22, FLAG_DJINNI(DJINNI_BANE),
     EVENT_SCRIPT(EventScript_BaneLastEscape)},
    {MAP_EVENT_COORD, 30, FLAG_DJINNI(DJINNI_BANE),
     EVENT_SCRIPT(EventScript_BaneKeepsAway)},
    {MAP_EVENT_COORD, 31, FLAG_DJINNI(DJINNI_BANE),
     EVENT_SCRIPT(EventScript_BaneKeepsAway)},
    {MAP_EVENT_COORD, 32, FLAG_DJINNI(DJINNI_BANE),
     EVENT_SCRIPT(EventScript_BaneKeepsAway)},
    {MAP_EVENT_COORD, 33, FLAG_DJINNI(DJINNI_BANE),
     EVENT_SCRIPT(EventScript_BaneKeepsAway)},
    {MAP_EVENT_COORD, 34, FLAG_DJINNI(DJINNI_BANE),
     EVENT_SCRIPT(EventScript_BaneKeepsAway)},
    {MAP_EVENT_COORD, 35, FLAG_DJINNI(DJINNI_BANE),
     EVENT_SCRIPT(EventScript_BaneKeepsAway)},
    {MAP_EVENT_COORD, 36, FLAG_DJINNI(DJINNI_BANE),
     EVENT_SCRIPT(EventScript_BaneKeepsAway)},
    {MAP_EVENT_COORD, 37, FLAG_DJINNI(DJINNI_BANE),
     EVENT_SCRIPT(EventScript_BaneKeepsAway)},
    {MAP_EVENT_COORD, 38, FLAG_DJINNI(DJINNI_BANE),
     EVENT_SCRIPT(EventScript_BaneKeepsAway)},
    {MAP_EVENT_COORD, 39, FLAG_DJINNI(DJINNI_BANE),
     EVENT_SCRIPT(EventScript_BaneKeepsAway)},
    {MAP_EVENT_COORD, 40, FLAG_DJINNI(DJINNI_BANE),
     EVENT_SCRIPT(EventScript_BaneKeepsAway)},
    {MAP_EVENT_COORD, 41, FLAG_DJINNI(DJINNI_BANE),
     EVENT_SCRIPT(EventScript_BaneKeepsAway)},
    {MAP_EVENT_TALK_HALTED, LOCALID_BANE, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_BaneJoins)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_HALT), LOCALID_BANE, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_HaltBane)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_PUDDLE,
     BANE_PUDDLE_FROZEN_FLAG, EVENT_SCRIPT(EventScript_FreezePuddle)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_FIRST_PILLAR,
     FLAG_MAP_TEMP_01, EVENT_SCRIPT(EventScript_UpdateFirstPillarTerrain)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_SECOND_PILLAR,
     BANE_CHEST_AT_LADDER_FLAG,
     EVENT_SCRIPT(EventScript_UpdateSecondPillarTerrain)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_THIRD_PILLAR,
     FLAG_MAP_TEMP_03, EVENT_SCRIPT(EventScript_UpdateThirdPillarTerrain)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_FOURTH_PILLAR,
     FLAG_MAP_TEMP_04, EVENT_SCRIPT(EventScript_UpdateFourthPillarTerrain)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_FIFTH_PILLAR,
     FLAG_MAP_TEMP_05, EVENT_SCRIPT(EventScript_UpdateFifthPillarTerrain)},
    {BG_EVENT(BG_EVENT_CHEST), 104, FLAG_CROSSBONE_ISLE_BANE_666_COINS,
     FIND_COINS(666)},
    {BG_EVENT(BG_EVENT_CHEST), 105, FLAG_CROSSBONE_ISLE_BANE_WATER_OF_LIFE,
     FIND_ITEM(ITEM_WATER_OF_LIFE)},
    {BG_EVENT(BG_EVENT_CHEST), 106, FLAG_CROSSBONE_ISLE_BANE_ELIXIR,
     FIND_ITEM(ITEM_ELIXIR)},
    {BG_EVENT(BG_EVENT_CHEST), 107, FLAG_CROSSBONE_ISLE_DEMON_AXE,
     FIND_ITEM(ITEM_DEMON_AXE)},
    {MAP_EVENTS_END},
};

u32 gCrossboneIsleKeys_SlotColumns[] = {
    16,
    21,
    0,
};

s32 gCrossboneIsleKeys_IconTimer = 0;
