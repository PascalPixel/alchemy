/// @file
/// Three of the side rooms of Crossbone Isle Cave, the cave beneath Crossbone
/// Isle, each opening off one of its corridor's nine rooms (crossbone_isle.c)
/// as the pillar puzzles do (crossbone_isle_pillar_rooms.c), with four chests
/// each. All three take the area name Crossbone Isle Cave. In the order the
/// party meets them:
///
/// - CrossboneIsleKeys (scene 117), off the third corridor room, a
///   room of locked doors. Four Blue Keys lie about it until Catch takes them,
///   and a Red Key shows only while Reveal lasts. Walking into one of the four
///   blue doors in a row with a Blue Key opens it and uses the key up (four
///   flags from FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN), and the Red Key
///   opens the red door beyond them (FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN). The
///   keys taken and the doors opened are restored on entry.
/// - CrossboneIslePlatformRooms2 (scene 118), off the fourth, where five
///   pursuers wait. As the leader crosses each one's stretch of the room it
///   steps out after him unless he is hidden by Cloak, and goes back home
///   once he is past (FLAG_SCENE_TEMP_20 while one is after him); Halt stops
///   one where it stands, and Reveal clears the translucent layer over the
///   room while it lasts.
/// - CrossboneIsleBane (scene 120), off the sixth, where Bane, a
///   Venus Djinni, hides. Five pillars to push or move with Move, a ladder and
///   a puddle that Frost freezes into a pillar of ice make a way across the
///   room. The first three times the party comes near, Bane flees, the third
///   time to the room's last corner (FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE to
///   _LAST_ESCAPE); there it runs to whichever spot the leader steps toward
///   unless Halt holds it. Once the battle with Bane is won, Bane joins the
///   party.
#include "game_state.h"
#include "version.h"
#include "global_cells.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/crossbone_isle_platform_rooms.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/runtime/runtime.h"
#include "unit/scene/common/colosso_item_icon.h"
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
#include "field_event.h"
#include "field_service.h"
#include "field_scene.h"
#include "staged_actor.h"
#include "select_overlay_data_by_runtime_selector.h"
#include "io_reg.h"
#include "resource.h"
#include "field_obj.h"
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
#include "event.h"

// Place flags: Scene_ResetFlagsOnEnter clears 0x300 to 0x3ff when the party
// enters another place group, and other places use the numbers for their own.
#define FLAG_CROSSBONE_ISLE_THIRD_PILLAR_LOWERED 0x300

#if TBS_PATCH_1
#define CROSSBONE_PURSUER_STEP_Z 16
#define CROSSBONE_LEADER_STEP_Z 24
#else
#define CROSSBONE_PURSUER_STEP_Z 24
#define CROSSBONE_LEADER_STEP_Z 16
#endif

static void Event_PushFirstPillar(void);
static void CrossboneIslePlatformRooms_FollowSecondPillar(void);
static void CrossboneIslePlatformRooms_PositionSecondPillarMapObject(void);
static void CrossboneIslePlatformRooms_InitializeScene1(void);
static void CrossboneIslePlatformRooms_InitializeScene2(void);
static void CrossboneIslePlatformRooms_InitializeScene3(void);

/// CrossboneIsleKeys's placed actors, numbered in the order of
/// gCrossboneIslePlatformRoomsPlacements1: the keys, drawn as their item icons
/// (Colosso_ShowItemIcon) until Catch takes them.
enum CrossboneIsleKeysActor {
  ACTOR_FIRST_BLUE_KEY = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_BLUE_KEY,
  ACTOR_THIRD_BLUE_KEY,
  ACTOR_FOURTH_BLUE_KEY,
  ACTOR_RED_KEY ///< shown only while Reveal lasts
};

/// CrossboneIslePlatformRooms2's placed actors, numbered in the order of
/// gCrossboneIslePlatformRoomsPlacements2: the five pursuers, sprite 0x1f4,
/// each waiting at its home for the leader to cross its stretch of the room.
enum CrossboneIslePlatformRooms2Actor {
  ACTOR_FIRST_PURSUER = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_PURSUER,
  ACTOR_THIRD_PURSUER,
  ACTOR_FOURTH_PURSUER,
  ACTOR_FIFTH_PURSUER
};

/// The events CrossboneIslePlatformRooms2's per-frame checks raise when the
/// leader crosses the first, second or fifth pursuer's stretch of the room.
/// The third and fourth chases start from touch events instead.
enum CrossboneIslePlatformRooms2Trigger {
  TRIGGER_FIRST_CHASE = 91,
  TRIGGER_SECOND_CHASE = 92,
  TRIGGER_FIFTH_CHASE = 96
};

/// CrossboneIsleBane's placed actors, numbered in the order of
/// gCrossboneIslePlatformRoomsPlacements3.
enum CrossboneIsleBaneActor {
  /// The five pillars, sprite 0xd3 as Goma Cave's, pushed by walking into them
  /// or with Move.
  ACTOR_FIRST_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_PILLAR,
  ACTOR_THIRD_PILLAR,
  ACTOR_FOURTH_PILLAR,
  ACTOR_FIFTH_PILLAR,
  ACTOR_PUDDLE, ///< frozen by Frost into a pillar of ice
  /// The chest on top of the second pillar, sprite 0x14 as McCoy's Palace's
  /// chests, holding the Demon Axe: it follows the pillar until the pillar
  /// reaches the ladder, where the chest's search marker 107 takes its place,
  /// and it is drawn open once the chest's flag 0xed9 is set.
  ACTOR_SECOND_PILLAR_CHEST,
  ACTOR_BANE ///< Bane, a Venus Djinni, until it is caught
};

/// Bane's number among the Venus Djinn.
#define DJINNI_BANE 6

enum { PASSAGE_INDEX_MASK = 0xff, PASSAGE_PLAY_OPENING = 0x100 };

// CrossboneIsleBane's temporary flags: FLAG_SCENE_TEMP_00 the puddle
// is frozen, 01 to 05 end Move on each pillar, 06 the chest on the second
// pillar has reached the ladder, 07 selects the ladder cell's other attributes.
// CrossboneIslePlatformRooms2 keeps FLAG_SCENE_TEMP_20 while a pursuer is
// after the leader.

/// Publishes the next raised-event trigger.
static __inline__ void SceneState_StoreStep(s16 *field, s32 step) {
  *field = step;
}

/// Lifts the first pillar and clears the terrain beneath its column.
static __inline__ void LiftPillar(s32 column, u8 motionFlags) {
  {
    s32 destinationColumn = column;
    Map_CopyCellAttributes(61, 36, 1, 1, destinationColumn, 42);
  }
  Object_GetById(ACTOR_FIRST_PILLAR)->motionFlags = motionFlags;
  Object_GetById(ACTOR_FIRST_PILLAR)->y.fixed = PIXELS(32);
}

#include "common/object_staged_actor.h"

/// Raised in CrossboneIsleBane once the battle with Bane is won:
/// Bane, a Venus Djinni, joins the party.
static void Event_BaneJoins(void) {
  Djinn_JoinParty(ACTOR_BANE, ELEMENT_VENUS, DJINNI_BANE);
}

UNUSED static void CrossboneIslePlatformRooms_UnusedHook(void) {}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 selector = gGameState.scene;

  if (selector == (s32)&SceneId_CrossboneIsleKeys) {
    return gCrossboneIslePlatformRoomsEntrances1;
  }
  if (selector == (s32)&SceneId_CrossboneIslePlatformRooms2) {
    return gCrossboneIslePlatformRoomsEntrances2;
  }
  if (selector == (s32)&SceneId_CrossboneIsleBane) {
    return gCrossboneIslePlatformRoomsEntrances3;
  }
  return gCrossboneIslePlatformRoomsEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gCrossboneIslePlatformRoomsExits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 selector = gGameState.scene;

  if (selector == (s32)&SceneId_CrossboneIsleKeys) {
    return gCrossboneIslePlatformRoomsPlacements1;
  }
  if (selector == (s32)&SceneId_CrossboneIslePlatformRooms2) {
    return gCrossboneIslePlatformRoomsPlacements2;
  }
  if (selector == (s32)&SceneId_CrossboneIsleBane) {
    return gCrossboneIslePlatformRoomsPlacements3;
  }
  return gCrossboneIslePlatformRoomsPlacementsOther;
}

/// Halt on a pursuer in CrossboneIslePlatformRooms2: nothing beyond the
/// Psynergy's own effect, which keeps it from stepping out.
static void Event_HaltPursuer(void) {}

// Turn the Djinni toward the party leader each frame.
// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 38 to 36 bytes in tbs-en.
#ifdef UBFIX
static void CrossboneIslePlatformRooms_FaceBaneTowardLeader(
    union FieldObject *fieldObject) {
#else
static s32 CrossboneIslePlatformRooms_FaceBaneTowardLeader(
    union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = &fieldObject->actor;
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  actor->facing = ArcTan2(leader->z.fixed - actor->z.fixed,
                          leader->x.fixed - actor->x.fixed);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// In CrossboneIsleBane, the first time the party comes near
/// (trigger 20): Bane notices them and hops away.
static void Event_BaneFirstEscape(void) {
  struct FieldActor *djinni;

  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE) == 0) {
    FLAG_SET(FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE);
    EV_BEGIN();
    Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
    Engine_CameraMoveToActor(ACTOR_BANE, 1);
    Engine_CameraWaitForMove();
    OBJ_DIR(ACTOR_BANE, FACING_EAST, 20);
    Actor_SetAttachedEffect(ACTOR_BANE, 0x102);
    Engine_ActorRunRepeatedMotion(ACTOR_BANE, 2);
    WAIT(20);
    OBJ_SPEED(ACTOR_BANE, PIXELS(1), PIXELS(1) / 2);
    SE(SE_JUMP);
    djinni = Object_GetById(ACTOR_BANE);
    djinni->velocityY = PIXELS(8);
    Actor_WalkToAndWait(ACTOR_BANE, 584, 680);
    OBJ_DIR(ACTOR_BANE, FACING_SOUTH, 20);
    EV_END();
  }
}

/// In CrossboneIsleBane, coming near again (trigger 21): Bane hops,
/// dashes across the platforms and crashes into the far wall, shaking the room,
/// and is left squashing and stretching.
static void Event_BaneSecondEscape(void) {
  struct FieldActor *spinningDjinni;
  s32 escapeComplete;
  struct FieldActor *jumpingDjinni;

  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE) != 0) {
    escapeComplete = FLAG_CHECK(FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE);
    if (escapeComplete == 0) {
      FLAG_SET(FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE);
      EV_BEGIN();

      // Focus on the Djinni before its next jump away.
      Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
      Engine_CameraMoveToActor(ACTOR_BANE, 1);
      Engine_CameraWaitForMove();
      OBJ_DIR(ACTOR_BANE, FACING_SOUTH, 20);
      Actor_SetAttachedEffect(ACTOR_BANE, 0x102);
      Engine_ActorRunRepeatedMotion(ACTOR_BANE, 2);
      WAIT(20);
      OBJ_SPEED(ACTOR_BANE, PIXELS(1), PIXELS(1) / 2);
      SE(SE_JUMP);
      jumpingDjinni = Object_GetById(ACTOR_BANE);
      jumpingDjinni->velocityY = PIXELS(10);
      Actor_WalkToAndWait(ACTOR_BANE, 584, 664);
      OBJ_DIR(ACTOR_BANE, FACING_SOUTH, 20);
      Actor_SetAttachedEffect(ACTOR_BANE, 0x102);
      WAIT(30);

      // Dash across the platforms, then shake the room on landing.
      OBJ_SPEED(ACTOR_BANE, PIXELS(8), PIXELS(1) / 4);
      Actor_WalkToAndWait(ACTOR_BANE, 664, 664);
      Actor_WalkToAndWait(ACTOR_BANE, 744, 664);
      Actor_WalkToAndWait(ACTOR_BANE, 824, 664);
      WAIT(10);
      SE(SE_208);
      Map_SetShake(PIXELS(4), PIXELS(2), Q16_ONE);
      WAIT(20);
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
      WAIT(30);

      // Leave the Djinni tilted while its scale animation continues.
      OBJ_POS_SET(ACTOR_BANE, 888, 664);
      spinningDjinni = Object_GetById(ACTOR_BANE);
      spinningDjinni->sprite->rotation = 0xf800;
      spinningDjinni->facing = 0;
      ObjectDispatch_SetAnimationSpeed((struct DispatchObject *)spinningDjinni,
                                       0);
      Engine_ObjectSetScript(spinningDjinni,
                             gCrossboneIslePlatformRooms_BaneSquashScript);
      EV_END();
    }
  }
}

/// In CrossboneIsleBane, the third time (trigger 22): Bane recovers,
/// hops down a winding path to the room's last corner and keeps watching the
/// leader.
static void Event_BaneLastEscape(void) {
  struct FieldActor *djinni;

  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE) != 0 &&
      FLAG_CHECK(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE) == 0) {
    FLAG_SET(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE);
    EV_BEGIN();

    // Restore the sprite before the last jump and winding escape path.
    djinni = Object_GetById(ACTOR_BANE);
    djinni->sprite->rotation = 0;
    ObjectDispatch_SetAnimationSpeed((struct DispatchObject *)djinni, 16);
    SE(SE_JUMP);
    djinni = Object_GetById(ACTOR_BANE);
    djinni->velocityY = PIXELS(8);
    OBJ_DIR(ACTOR_BANE, FACING_WEST, 30);
    Actor_SetAttachedEffect(ACTOR_BANE, 0x102);
    Engine_ActorRunRepeatedMotion(ACTOR_BANE, 2);
    WAIT(20);
    OBJ_SPEED(ACTOR_BANE, PIXELS(1), PIXELS(1) / 2);
    SE(SE_JUMP);
    djinni = Object_GetById(ACTOR_BANE);
    djinni->velocityY = PIXELS(4);
    Actor_WalkToAndWait(ACTOR_BANE, 880, 680);
    WAIT(10);
    Actor_SetAttachedEffect(ACTOR_BANE, 0x101);
    OBJ_SPEED(ACTOR_BANE, PIXELS(2), PIXELS(1));
    Actor_WalkToAndWait(ACTOR_BANE, 880, 696);
    Actor_WalkToAndWait(ACTOR_BANE, 882, 704);
    Actor_WalkToAndWait(ACTOR_BANE, 880, 712);
    Actor_WalkToAndWait(ACTOR_BANE, 878, 720);
    Actor_WalkToAndWait(ACTOR_BANE, 880, 728);
    Actor_WalkToAndWait(ACTOR_BANE, 882, 736);
    Actor_WalkToAndWait(ACTOR_BANE, 880, 744);
    Actor_WalkToAndWait(ACTOR_BANE, 878, 752);
    Actor_WalkToAndWait(ACTOR_BANE, 880, 760);
    OBJ_POS_SET(ACTOR_BANE, 856, 824);
    WAIT(10);
    OBJ_DIR(ACTOR_BANE, FACING_NORTH, 20);
    Actor_SetAttachedEffect(ACTOR_BANE, 0x100);
    djinni = Object_GetById(ACTOR_BANE);
#ifdef UBFIX
    djinni->update = CrossboneIslePlatformRooms_FaceBaneTowardLeader;
#else
    djinni->update = (void (*)(
        union FieldObject *))CrossboneIslePlatformRooms_FaceBaneTowardLeader;
#endif
    EV_END();
  }
}

/// In CrossboneIsleBane, stepping on the triggers around Bane's
/// corner (30 to 41): Bane runs to the matching spot, as fast as the leader,
/// unless Halt holds it.
static void Event_BaneKeepsAway(void) {
  struct FieldActor *djinni;
  struct FieldActor *leader;
  s32 targetByteOffset;
  const u8 *targets;
  s32 zByteOffset;
  struct EventWork *work;

  work = gEventWork;
  if (Engine_GameFlagIsSet(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE) != 0) {
    if (gGameState.pausedObject != ACTOR_BANE) {
      targetByteOffset = work->touchedTrigger;
      djinni = Object_GetById(ACTOR_BANE);
      leader = Object_GetById(ACTOR_PARTY_LEADER);
      djinni->speed = leader->speed;
      djinni = Object_GetById(ACTOR_BANE);
      leader = Object_GetById(ACTOR_PARTY_LEADER);
      djinni->acceleration = leader->speed;
      targetByteOffset -= 30;
      targets = (const u8 *)gCrossboneIslePlatformRooms_BaneWalkTargets;
      // The table stores consecutive signed X/Z pixel pairs.
      targetByteOffset *= 2 * sizeof(s32);
      zByteOffset = targetByteOffset + sizeof(s32);
      Engine_ActorWalkTo(ACTOR_BANE, *(const s32 *)(targets + targetByteOffset),
                         *(const s32 *)(targets + zByteOffset));
    }
  }
}

/// Halt on Bane in CrossboneIsleBane: it freezes in place.
static void Event_HaltBane(void) {
  EV_BEGIN();
  OBJ_MODE(ACTOR_BANE, 0);
  EV_END();
}

const struct SceneEvent *Scene_GetEvents(void) {
  s16 selector = gGameState.scene;

  if (selector == (s32)&SceneId_CrossboneIsleKeys) {
    return gCrossboneIslePlatformRoomsEvents1;
  }
  if (selector == (s32)&SceneId_CrossboneIslePlatformRooms2) {
    return gCrossboneIslePlatformRoomsEvents2;
  }
  if (selector == (s32)&SceneId_CrossboneIsleBane) {
    return gCrossboneIslePlatformRoomsEvents3;
  }
  return gCrossboneIslePlatformRoomsEventsOther;
}

/// Unused: walking south into CrossboneIsleKeys's third or fourth blue door
/// looks at what is ahead.
///
/// Both rows that name it follow a plain touch row for the same door
/// (Event_OpenThirdBlueDoor, Event_OpenFourthBlueDoor), which Scene_FindEvent
/// always finds first; those events look ahead themselves after a dozen steps
/// pressed south against the door, so the player sees the same thing.
static void Event_UnusedCheckLeaderAhead(void) { Engine_LeaderCheckAhead(); }

/// Opens the selected blue door, playing its opening when asked.
static void CrossboneIslePlatformRooms_OpenBlueDoor(s32 passageInfo) {
  s32 columnOffset = (passageInfo & PASSAGE_INDEX_MASK) * 4;
  s32 tileColumn = columnOffset + 77;
  s32 collisionColumn = columnOffset + 13;

  if (passageInfo & PASSAGE_PLAY_OPENING) {
    Engine_AudioPlayCue(SE_GATE_MOVE);
    Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
    Map_CopyCells(79, 29, 1, 3, tileColumn, 40);
    Engine_TaskWait(40);
  }
  Map_CopyCells(80, 29, 1, 3, tileColumn, 40);
  Map_CopyCellAttributes(collisionColumn, 40, 1, 1, collisionColumn, 41);
  Map_CopyCellAttributes(collisionColumn, 40, 1, 1, collisionColumn, 42);
}

/// Opens the red door, playing its opening when asked.
static void CrossboneIslePlatformRooms_OpenRedDoor(s32 passageInfo) {
  if ((passageInfo & PASSAGE_PLAY_OPENING) != 0) {
    SE(SE_GATE_MOVE);
    Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
    Map_CopyCells(84, 29, 1, 3, 70, 49);
    Engine_TaskWait(60);
  }
  Map_CopyCells(85, 29, 1, 3, 70, 49);
  Map_CopyCellAttributes(6, 49, 1, 1, 6, 50);
  Map_CopyCellAttributes(6, 49, 1, 1, 6, 51);
}

/// Facing CrossboneIsleKeys's red door with the Red Key (trigger
/// 50): the door rumbles open, and the key is used up.
static void Event_UseRedKey(void) {
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader->facing == FACING_NORTH) {
    if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN) == 0) {
      if (PartyInventory_FindOwner(ITEM_RED_KEY) != -1) {
        FLAG_SET(FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN);
        CrossboneIslePlatformRooms_OpenRedDoor(PASSAGE_PLAY_OPENING);
        PartyInventory_Remove(ITEM_RED_KEY);
      }
    }
  }
}

/// Facing a blue door with a Blue Key: the door rumbles open, and the key is
/// used up.
static void CrossboneIslePlatformRooms_UseBlueKey(s32 doorIndex) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
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
  CrossboneIslePlatformRooms_OpenBlueDoor(PASSAGE_PLAY_OPENING | doorIndex);
  PartyInventory_Remove(ITEM_BLUE_KEY);
}

/// Touching CrossboneIsleKeys's first blue door (trigger 51).
static void Event_OpenFirstBlueDoor(void) {
  CrossboneIslePlatformRooms_UseBlueKey(0);
}

/// Touching CrossboneIsleKeys's second blue door (trigger 52).
static void Event_OpenSecondBlueDoor(void) {
  CrossboneIslePlatformRooms_UseBlueKey(1);
}

/// Touching CrossboneIsleKeys's third blue door (trigger 53);
/// pressing on it from the north looks ahead every dozen blocked steps.
static void Event_OpenThirdBlueDoor(void) {
  struct FieldActor *leader;
  s16 *blockedSteps;
  s32 resetSteps;

  CrossboneIslePlatformRooms_UseBlueKey(2);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader->facing == FACING_SOUTH) {
    blockedSteps = &gEventWork->blockedSteps;
    if (*blockedSteps > 12) {
      Engine_LeaderCheckAhead();
      resetSteps = 0;
      *blockedSteps = resetSteps;
    }
  }
}

/// Touching CrossboneIsleKeys's fourth blue door (trigger 54);
/// pressing on it from the north looks ahead every dozen blocked steps.
static void Event_OpenFourthBlueDoor(void) {
  struct FieldActor *leader;
  s16 *blockedSteps;
  s32 resetSteps;

  CrossboneIslePlatformRooms_UseBlueKey(3);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
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
static void Event_ShowRedKey(void) {
  s32 keyActorId = ACTOR_RED_KEY;
  s32 x = PIXELS(408);
  s32 z = PIXELS(776);

  Map_CopyCells(25, 45, 1, 2, 25, 48);
  if (Engine_GameFlagIsSet(0xeeb) == 0)
    Engine_ActorSetPosition(keyActorId, x, z);
  WAIT(1);
}

/// Reveal wears off in CrossboneIsleKeys: the Red Key's tile is
/// hidden again.
static void Event_HideRedKey(void) {
  Map_CopyCells(24, 48, 1, 2, 25, 48);
  OBJ_POS_SET(ACTOR_RED_KEY, 8, 8);
}

/// Collects the selected key, hides its actor and remembers it was taken.
static inline void CrossboneIslePlatformRooms_CollectKey(s32 actorId,
                                                         s32 itemId,
                                                         s32 collectedFlag) {
  Engine_ItemShowFound(itemId, 3);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_PartyGiveItem(itemId, 0);
  Engine_ActorSetPosition(actorId, 0, 0);
  Engine_GameFlagSet(collectedFlag);
}

/// Catch on CrossboneIsleKeys's first Blue Key: the party gets it.
static void Event_CatchFirstBlueKey(void) {
  CrossboneIslePlatformRooms_CollectKey(ACTOR_FIRST_BLUE_KEY, ITEM_BLUE_KEY,
                                        0xee7);
}

/// Catch on CrossboneIsleKeys's second Blue Key: the party gets it.
static void Event_CatchSecondBlueKey(void) {
  CrossboneIslePlatformRooms_CollectKey(ACTOR_SECOND_BLUE_KEY, ITEM_BLUE_KEY,
                                        0xee8);
}

/// Catch on CrossboneIsleKeys's third Blue Key: the party gets it.
static void Event_CatchThirdBlueKey(void) {
  CrossboneIslePlatformRooms_CollectKey(ACTOR_THIRD_BLUE_KEY, ITEM_BLUE_KEY,
                                        0xee9);
}

/// Catch on CrossboneIsleKeys's fourth Blue Key: the party gets it.
static void Event_CatchFourthBlueKey(void) {
  CrossboneIslePlatformRooms_CollectKey(ACTOR_FOURTH_BLUE_KEY, ITEM_BLUE_KEY,
                                        0xeea);
}

/// Catch on CrossboneIsleKeys's revealed Red Key: the party gets it.
static void Event_CatchRedKey(void) {
  CrossboneIslePlatformRooms_CollectKey(ACTOR_RED_KEY, ITEM_RED_KEY, 0xeeb);
}

/// Starts the first chase when an uncloaked leader enters its area.
static void CrossboneIslePlatformRooms_CheckFirstPursuerTrigger(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 column = leader->x.fixed / PIXELS(16);
  s32 row = leader->z.fixed / PIXELS(16);
  struct EventWork *event = gEventWork;

  if (!FLAG_CHECK(FLAG_SCENE_TEMP_20) && gGameState.cloaked == 0 &&
      gGameState.pausedObject != ACTOR_FIRST_PURSUER &&
      (u32)(column - 21) <= 2 && row >= 10 && row <= 11) {
    Engine_GameFlagSet(FLAG_SCENE_TEMP_20);
    event->raisedTrigger = TRIGGER_FIRST_CHASE;
  }
}

/// Raised in CrossboneIslePlatformRooms2 (91): the first pursuer follows the
/// leader, then goes back home.
static void Event_DodgeFirstPursuer(void) {
  struct FieldActor *leader;

  EV_BEGIN();
  OBJ_SPEED(ACTOR_PARTY_LEADER, PIXELS(19) / 10, PIXELS(19) / 20);
  OBJ_SPEED(ACTOR_FIRST_PURSUER, PIXELS(19) / 10, PIXELS(19) / 20);
  SE(SE_188);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_FIRST_PURSUER, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(ACTOR_FIRST_PURSUER);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 24);
  WAIT(4);
  SE(SE_188);
  Actor_SetDestinationOffset(ACTOR_FIRST_PURSUER, 0, CROSSBONE_PURSUER_STEP_Z);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Actor_SetDestination(ACTOR_FIRST_PURSUER, 360, 152);
  Engine_ActorWaitForMove(ACTOR_FIRST_PURSUER);
  EV_END();
  GameFlag_Clear(FLAG_SCENE_TEMP_20);
}

/// Starts the second chase when the leader enters column ten.
static void CrossboneIslePlatformRooms_CheckSecondPursuerTrigger(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 column = leader->x.fixed / PIXELS(16);
  s32 row = leader->z.fixed / PIXELS(16);
  struct EventWork *work = gEventWork;

  if (FLAG_CHECK(FLAG_SCENE_TEMP_20) == 0 && gGameState.cloaked == 0 &&
      gGameState.pausedObject != ACTOR_SECOND_PURSUER && column == 10 &&
      (u32)(row - 16) <= 2) {
    FLAG_SET(FLAG_SCENE_TEMP_20);
    SceneState_StoreStep(&work->raisedTrigger, TRIGGER_SECOND_CHASE);
  }
}

/// Raised in CrossboneIslePlatformRooms2 (92): the second pursuer follows the
/// leader, then goes back home.
static void Event_DodgeSecondPursuer(void) {
  struct FieldActor *leader;

  EV_BEGIN();
  OBJ_SPEED(ACTOR_PARTY_LEADER, PIXELS(17) / 10, PIXELS(17) / 20);
  OBJ_SPEED(ACTOR_SECOND_PURSUER, PIXELS(17) / 10, PIXELS(17) / 20);
  SE(SE_188);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_SECOND_PURSUER, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(ACTOR_SECOND_PURSUER);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, CROSSBONE_LEADER_STEP_Z);
  SE(SE_188);
  WAIT(4);
  Actor_SetDestinationOffset(ACTOR_SECOND_PURSUER, 0, 16);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Actor_SetDestination(ACTOR_SECOND_PURSUER, 168, 264);
  Engine_ActorWaitForMove(ACTOR_SECOND_PURSUER);
  EV_END();
  GameFlag_Clear(FLAG_SCENE_TEMP_20);
}

/// Moves a pursuer to the leader column and remembers its last column.
static void CrossboneIslePlatformRooms_TrackPursuerColumn(s32 pursuerIndex) {

  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 column = leader->x.fixed / PIXELS(16);
  s32 row = leader->z.fixed / PIXELS(16);
  s32 pursuerId = pursuerIndex + ACTOR_THIRD_PURSUER;

  if (gGameState.pausedObject == pursuerId)
    return;
  if (column == gCrossboneIslePlatformRooms_SlotColumns[pursuerIndex])
    return;

  Engine_ActorSetSpeed(pursuerId, PIXELS(9) / 2, PIXELS(9) / 4);
  Engine_AudioPlayCue(SE_188);
  Engine_ActorSetDestination(pursuerId, (column << 4) + 8, 360);

  gCrossboneIslePlatformRooms_SlotColumns[pursuerIndex] = column;

  if (row <= 22) {
    Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, 8);
  }
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
}

/// Touching trigger 53 in CrossboneIslePlatformRooms2: the third pursuer keeps
/// to the leader's column.
static void Event_TrackThirdPursuerColumn(void) {
  CrossboneIslePlatformRooms_TrackPursuerColumn(0);
}

/// Touching trigger 54 in CrossboneIslePlatformRooms2: the fourth pursuer
/// keeps to the leader's column.
static void Event_TrackFourthPursuerColumn(void) {
  CrossboneIslePlatformRooms_TrackPursuerColumn(1);
}

UNUSED static void CrossboneIslePlatformRooms_TrackFifthPursuerColumn(void) {
  CrossboneIslePlatformRooms_TrackPursuerColumn(2);
}

/// Touching trigger 61 in CrossboneIslePlatformRooms2: the third pursuer
/// follows the leader, then goes back home.
static void Event_DodgeThirdPursuer(void) {
  struct FieldActor *leader;

  if (gGameState.pausedObject != ACTOR_THIRD_PURSUER) {
    EV_BEGIN();
    OBJ_SPEED(ACTOR_PARTY_LEADER, PIXELS(17) / 10, PIXELS(17) / 20);
    OBJ_SPEED(ACTOR_THIRD_PURSUER, PIXELS(17) / 10, PIXELS(17) / 20);
    SE(SE_188);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetDestination(ACTOR_THIRD_PURSUER, leader->x.part.pixel,
                           leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(ACTOR_THIRD_PURSUER);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, CROSSBONE_LEADER_STEP_Z);
    WAIT(4);
    SE(SE_188);
    Actor_SetDestinationOffset(ACTOR_THIRD_PURSUER, 0, 16);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetDestination(ACTOR_THIRD_PURSUER, 264, 360);
    Engine_ActorWaitForMove(ACTOR_THIRD_PURSUER);
    WAIT(10);
    EV_END();
  }
}

/// Touching trigger 62 in CrossboneIslePlatformRooms2: the fourth pursuer lines
/// up and goes after the leader.
static void Event_DodgeFourthPursuer(void) {
  struct FieldActor *leader;
  s32 leaderColumn;

  if (gGameState.pausedObject != ACTOR_FOURTH_PURSUER) {
    EV_BEGIN();
    OBJ_SPEED(ACTOR_PARTY_LEADER, PIXELS(17) / 10, PIXELS(17) / 20);
    Engine_ActorSetSpeed(ACTOR_FOURTH_PURSUER, PIXELS(17) / 10,
                         PIXELS(17) / 20);
    Engine_AudioPlayCue(SE_188);
    leaderColumn = Object_GetById(ACTOR_PARTY_LEADER)->x.fixed / PIXELS(16);
    if (leaderColumn >
        Object_GetById(ACTOR_FOURTH_PURSUER)->x.fixed / PIXELS(16)) {
      Engine_ActorSetDestinationOffset(ACTOR_FOURTH_PURSUER, 8, 0);
    }
    leaderColumn = Object_GetById(ACTOR_PARTY_LEADER)->x.fixed / PIXELS(16);
    if (leaderColumn <
        Object_GetById(ACTOR_FOURTH_PURSUER)->x.fixed / PIXELS(16)) {
      FieldObjectMotion_OffsetPositionAndResetMotion(ACTOR_FOURTH_PURSUER, -8,
                                                     0);
    }
    Engine_ActorWaitForMove(ACTOR_FOURTH_PURSUER);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Engine_ActorSetDestination(ACTOR_FOURTH_PURSUER, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(ACTOR_FOURTH_PURSUER);
    Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0,
                                     CROSSBONE_LEADER_STEP_Z);
    WAIT(4);
    Engine_AudioPlayCue(SE_188);
    Engine_ActorSetDestinationOffset(ACTOR_FOURTH_PURSUER, 0, 16);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_ActorSetDestination(ACTOR_FOURTH_PURSUER, 344, 360);
    Engine_ActorWaitForMove(ACTOR_FOURTH_PURSUER);
    WAIT(10);
    EV_END();
  }
}

/// Starts the fifth chase when the leader enters column nineteen.
static void CrossboneIslePlatformRooms_CheckFifthPursuerTrigger(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 column = leader->x.fixed / PIXELS(16);
  s32 row = leader->z.fixed / PIXELS(16);
  struct EventWork *work = gEventWork;
  struct GameState *state = &gGameState;

  if (state->pausedObject != ACTOR_FIFTH_PURSUER &&
      FLAG_CHECK(FLAG_SCENE_TEMP_20) == 0 && state->cloaked == 0 &&
      column == 19 && (u32)(row - 15) <= 1) {
    FLAG_SET(FLAG_SCENE_TEMP_20);
    SceneState_StoreStep(&work->raisedTrigger, TRIGGER_FIFTH_CHASE);
  }
}

/// Raised in CrossboneIslePlatformRooms2 (96): the fifth pursuer follows the
/// leader, then goes back home.
static void Event_DodgeFifthPursuer(void) {
  struct FieldActor *leader;

  EV_BEGIN();
  OBJ_SPEED(ACTOR_PARTY_LEADER, PIXELS(17) / 10, PIXELS(17) / 20);
  OBJ_SPEED(ACTOR_FIFTH_PURSUER, PIXELS(17) / 10, PIXELS(17) / 20);
  SE(SE_188);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_FIFTH_PURSUER, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(ACTOR_FIFTH_PURSUER);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, CROSSBONE_LEADER_STEP_Z);
  SE(SE_188);
  Actor_SetDestinationOffset(ACTOR_FIFTH_PURSUER, 0, 16);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Actor_SetDestination(ACTOR_FIFTH_PURSUER, 312, 232);
  Engine_ActorWaitForMove(ACTOR_FIFTH_PURSUER);
  EV_END();
  GameFlag_Clear(FLAG_SCENE_TEMP_20);
}

/// Reveal in CrossboneIslePlatformRooms2: the translucent layer over the room
/// vanishes while it lasts.
static void Event_BeginRevealBlend(void) {
  u32 coefficient = 208;

  coefficient <<= 4;
  REG_BLDALPHA = coefficient;
}

/// Reveal wears off in CrossboneIslePlatformRooms2: the translucent layer
/// returns.
static void Event_EndRevealBlend(void) {
  u16 value = 0x0607;

  REG_BLDALPHA = value;
}

/// At CrossboneIsleBane's ladder by the second pillar (trigger 60):
/// climb up or down, look ahead to the east, or push the first pillar from the
/// floor to the west.
static void Event_UseLadder(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  u16 facing = leader->facing;

  if (facing == FACING_NORTH) {
    if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_06) != 0) {
      {
        s32 destinationColumn = 45;
        s32 destinationRow = 43;
        Map_CopyCellAttributes(46, 43, 1, 1, destinationColumn, destinationRow);
      }
    }
    Engine_GameFlagClear(FLAG_SCENE_TEMP_07);
    Field_ClimbUp();
  } else if (facing == FACING_SOUTH) {
    Field_ClimbDown();
  } else if (facing == 0) {
    if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_06) != 0) {
      {
        s32 destinationColumn = 45;
        s32 destinationRow = 43;
        Map_CopyCellAttributes(58, 36, 1, 1, destinationColumn, destinationRow);
      }
    }
    Engine_GameFlagSet(FLAG_SCENE_TEMP_07);
    Engine_LeaderCheckAhead();
  } else if (facing == FACING_WEST) {
    if (leader->y.fixed == 0) {
      Event_PushFirstPillar();
    } else {
      Engine_LeaderCheckAhead();
    }
  }
}

UNUSED static void CrossboneIslePlatformRooms_UnusedCallback(void) {}

/// Frost on CrossboneIsleBane's puddle: it freezes into a pillar of
/// ice the party can stand on.
static void Event_FreezePuddle(void) {
  struct FieldActor *puddle = Object_GetById(ACTOR_PUDDLE);

  Map_CopyCellAttributes(40, 54, 1, 1, 40, 55);
  if (puddle != 0) {
    u8 *motionFlags = &Object_GetById(ACTOR_PUDDLE)->motionFlags;
    u8 *priorityFlags = &puddle->priorityFlags;

    *motionFlags = 0;
    *priorityFlags = 2;
  }
  Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
}

/// Walking into CrossboneIsleBane's first pillar from the ladder side
/// (trigger 70): facing east, look ahead; otherwise push the pillar.
static void Event_LookAheadOrPushFirstPillar(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  if (leader->facing == 0) {
    Engine_LeaderCheckAhead();
  } else {
    Event_PushFirstPillar();
  }
}

/// Restores the first pillar row before applying its current footprint.
static void CrossboneIslePlatformRooms_RestoreFirstPillarTerrain(void) {
  Map_CopyCellAttributes(57, 42, 1, 1, 40, 42);
  Map_CopyCellAttributes(57, 42, 1, 1, 41, 42);
  Map_CopyCellAttributes(58, 42, 1, 1, 42, 42);
  Map_CopyCellAttributes(62, 37, 3, 1, 37, 42);

  Object_GetById(ACTOR_FIRST_PILLAR)->motionFlags = 1;
}

/// Restores support under the first pillar at the eastern cell.
static void CrossboneIslePlatformRooms_RestoreEastUnderfootCell(void) {

  Map_CopyCellAttributes(58, 41, 1, 1, 42, 42);
  Object_GetById(ACTOR_FIRST_PILLAR)->priorityFlags = 2;
}

/// Restores support under the first pillar at the middle cell.
static void CrossboneIslePlatformRooms_RestoreMiddleUnderfootCell(void) {

  Map_CopyCellAttributes(44, 42, 1, 1, 41, 42);
  Object_GetById(ACTOR_FIRST_PILLAR)->priorityFlags = 2;
}

/// Restores support under the first pillar at the western cell.
static void CrossboneIslePlatformRooms_RestoreWestUnderfootCell(void) {

  Map_CopyCellAttributes(39, 42, 1, 1, 40, 42);
  Object_GetById(ACTOR_FIRST_PILLAR)->priorityFlags = 2;
}

/// Move on CrossboneIsleBane's first pillar: its support cells are
/// rebuilt from its height and column.
static void Event_UpdateFirstPillarTerrain(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_FIRST_PILLAR);
  s32 x = actor->x.fixed;
  s32 height = actor->y.fixed;
  s32 column = x / PIXELS(16);
  u8 lowered;

  if (height == 0) {
    Object_GetById(ACTOR_FIRST_PILLAR)->priorityFlags = 2;
  }
  CrossboneIslePlatformRooms_RestoreFirstPillarTerrain();
  lowered = 0;
  Object_GetById(ACTOR_FIRST_PILLAR)->motionFlags = 3;
  if (column == 40) {
    CrossboneIslePlatformRooms_RestoreWestUnderfootCell();
  } else if (column == 42) {
    CrossboneIslePlatformRooms_RestoreEastUnderfootCell();
  } else if (column == 41) {
    CrossboneIslePlatformRooms_RestoreMiddleUnderfootCell();
  } else if (column == 39) {
    LiftPillar(column, lowered);
  } else if (column == 38) {
    LiftPillar(column, lowered);
  } else if (column == 37) {
    LiftPillar(column, lowered);
  }
}

/// Pushing CrossboneIsleBane's first pillar (trigger 51); standing at
/// the ladder in column 38, the leader climbs instead.
static void Event_PushFirstPillar(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldActor *pillar = Object_GetById(ACTOR_FIRST_PILLAR);
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

  CrossboneIslePlatformRooms_RestoreFirstPillarTerrain();
  StagedActor_AdvancePair();
  Event_UpdateFirstPillarTerrain();
}

/// Leaves the chest where the second pillar has reached the ladder: the chest's
/// actor is hidden, its search marker takes its place and the ladder cell is
/// restored.
static void CrossboneIslePlatformRooms_ReleaseSecondPillarChest(void) {
  Scheduler_RemoveCallback(CrossboneIslePlatformRooms_FollowSecondPillar);
  OBJ_POS_SET(ACTOR_SECOND_PILLAR_CHEST, 0, 0);
  if (FLAG_CHECK(FLAG_SCENE_TEMP_07) != 0) {
    Map_CopyCellAttributes(58, 36, 1, 1, 45, 43);
  } else {
    Map_CopyCellAttributes(46, 43, 1, 1, 45, 43);
  }
  CrossboneIslePlatformRooms_PositionSecondPillarMapObject();
  FLAG_SET(FLAG_SCENE_TEMP_06);
}

/// Move on CrossboneIsleBane's second pillar: the chest on top of it stays
/// at the ladder cell once the pillar reaches it.
static void Event_UpdateSecondPillarTerrain(void) {
  struct FieldActor *pillar = Object_GetById(ACTOR_SECOND_PILLAR);
  s32 column = pillar->x.fixed / PIXELS(16);
  s32 row = pillar->z.fixed / PIXELS(16);

  if (column == 45 && row == 43) {
    CrossboneIslePlatformRooms_ReleaseSecondPillarChest();
  }
}

/// Pushing CrossboneIsleBane's second pillar (trigger 52); at the ladder, the
/// chest on top of it stays there.
static void Event_PushSecondPillar(void) {
  StagedActor_AdvancePair();
  Event_UpdateSecondPillarTerrain();
}

/// Restores the terrain at both ends of the third pillar row.
static void CrossboneIslePlatformRooms_RestoreThirdPillarTerrain(void) {

  Map_CopyCellAttributes(38, 56, 1, 1, 38, 55);
  Map_CopyCellAttributes(42, 56, 1, 1, 42, 55);
}

/// Restores the third pillar support at the western end.
static void CrossboneIslePlatformRooms_RestoreThirdPillarWestCell(void) {

  Map_CopyCellAttributes(40, 54, 1, 1, 38, 55);
  Object_GetById(ACTOR_THIRD_PILLAR)->motionFlags = 0;
}

/// Restores the third pillar support at the eastern end.
static void CrossboneIslePlatformRooms_RestoreThirdPillarEastCell(void) {

  Map_CopyCellAttributes(40, 54, 1, 1, 42, 55);
  Object_GetById(ACTOR_THIRD_PILLAR)->motionFlags = 0;
}

/// Move on CrossboneIsleBane's third pillar: its support is rebuilt
/// from its height and destination.
static void Event_UpdateThirdPillarTerrain(void) {
  struct FieldActor *pillar = Object_GetById(ACTOR_THIRD_PILLAR);
  s32 heightCells = pillar->y.fixed / PIXELS(16);
  s32 column = pillar->x.fixed / PIXELS(16);
  s32 row = pillar->z.fixed / PIXELS(16);

  if (!FLAG_CHECK(FLAG_CROSSBONE_ISLE_THIRD_PILLAR_LOWERED) &&
      heightCells <= 2) {
    CrossboneIslePlatformRooms_RestoreThirdPillarWestCell();
    Engine_GameFlagSet(FLAG_CROSSBONE_ISLE_THIRD_PILLAR_LOWERED);
  }
  if (row == 55) {
    if (column == 42) {
      CrossboneIslePlatformRooms_RestoreThirdPillarEastCell();
    }
    if (column == 38) {
      CrossboneIslePlatformRooms_RestoreThirdPillarWestCell();
    }
  } else {
    CrossboneIslePlatformRooms_RestoreThirdPillarTerrain();
  }
}

/// Pushing CrossboneIsleBane's third pillar (trigger 50).
static void Event_PushThirdPillar(void) {
  CrossboneIslePlatformRooms_RestoreThirdPillarTerrain();
  StagedActor_AdvancePair();
  Event_UpdateThirdPillarTerrain();
}

/// Walking into CrossboneIsleBane's third pillar (trigger 54): facing
/// south, the rising sequence plays; otherwise the pillar is pushed.
static void Event_TouchThirdPillar(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  if (leader->facing == FACING_SOUTH) {
    FieldBattleFx_RunRisingObjectSequence(0, 6, 0);
  } else {
    Event_PushThirdPillar();
  }
}

/// Clears the support row before repainting the fourth and fifth pillars.
static void
CrossboneIslePlatformRooms_RestoreFourthAndFifthPillarTerrain(void) {

  Map_CopyCellAttributes(48, 55, 1, 1, 49, 55);
  Map_CopyCellAttributes(48, 55, 1, 1, 50, 55);
  Map_CopyCellAttributes(48, 55, 1, 1, 51, 55);
  Map_CopyCellAttributes(48, 55, 1, 1, 52, 55);
}

/// Places support cells beneath the fourth and fifth pillars.
static void CrossboneIslePlatformRooms_RepaintFourthAndFifthPillarCells(void) {
  struct FieldActor *pillar;
  s32 column;

  CrossboneIslePlatformRooms_RestoreFourthAndFifthPillarTerrain();

  pillar = Object_GetById(ACTOR_FOURTH_PILLAR);
  column = pillar->x.fixed / PIXELS(16);
  Map_CopyCellAttributes(53, 55, 1, 1, column, 55);
  pillar = Object_GetById(ACTOR_FIFTH_PILLAR);
  column = pillar->x.fixed / PIXELS(16);
  Map_CopyCellAttributes(53, 55, 1, 1, column, 55);
}

/// Move on CrossboneIsleBane's fourth pillar: the support row it
/// shares with the fifth is refreshed.
static void Event_UpdateFourthPillarTerrain(void) {
  CrossboneIslePlatformRooms_RepaintFourthAndFifthPillarCells();
}

/// Pushing CrossboneIsleBane's fourth pillar (trigger 53).
static void Event_PushFourthPillar(void) {
  CrossboneIslePlatformRooms_RestoreFourthAndFifthPillarTerrain();
  StagedActor_AdvancePair();
  Event_UpdateFourthPillarTerrain();
}

/// Move on CrossboneIsleBane's fifth pillar: the support row it shares
/// with the fourth is refreshed.
static void Event_UpdateFifthPillarTerrain(void) {
  CrossboneIslePlatformRooms_RepaintFourthAndFifthPillarCells();
}

/// Unused callback that moves the fifth pillar and refreshes its terrain.
UNUSED static void CrossboneIslePlatformRooms_MoveFifthPillar(void) {
  CrossboneIslePlatformRooms_RestoreFourthAndFifthPillarTerrain();
  StagedActor_AdvancePair();
  Event_UpdateFifthPillarTerrain();
}

/// Keeps the chest on top of the second pillar as the pillar moves.
static void CrossboneIslePlatformRooms_FollowSecondPillar(void) {
  struct FieldActor *target = Object_GetById(ACTOR_SECOND_PILLAR_CHEST);
  struct FieldActor *source = Object_GetById(ACTOR_SECOND_PILLAR);

  target->y.fixed = PIXELS(32);
  target->x.fixed = source->x.fixed;
  target->z.fixed = source->z.fixed + PIXELS(1);
}

/// Puts the chest's search marker 107 where the second pillar stands.
static void CrossboneIslePlatformRooms_PositionSecondPillarMapObject(void) {
  struct FieldActor *frame = Object_GetById(ACTOR_SECOND_PILLAR);

  Engine_MapObjectSetPosition(107, frame->x.fixed, frame->z.fixed + PIXELS(1));
}

/// Chooses pillar drawing priorities from the leader height and row.
static void CrossboneIslePlatformRooms_UpdatePillarPriorities(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  u8 priority;

  if (leader->y.fixed > PIXELS(16)) {
    priority = ACTOR_PRIORITY_UNDERFOOT;
    Object_GetById(ACTOR_FIRST_PILLAR)->priorityFlags = priority;
    if (Object_GetById(ACTOR_THIRD_PILLAR)->y.fixed == 0)
      Object_GetById(ACTOR_THIRD_PILLAR)->priorityFlags = priority;
    Object_GetById(ACTOR_FOURTH_PILLAR)->priorityFlags = priority;
  } else {
    if (Object_GetById(ACTOR_THIRD_PILLAR)->y.fixed == 0 &&
        Object_GetById(ACTOR_PARTY_LEADER)->z.fixed / PIXELS(16) > 56) {
      Engine_ActorSetSpritePriority(ACTOR_THIRD_PILLAR, 3);
    } else {
      Engine_ActorSetSpritePriority(ACTOR_THIRD_PILLAR, 1);
      Object_GetById(ACTOR_THIRD_PILLAR)->priorityFlags =
          ACTOR_PRIORITY_AUTOMATIC;
    }
    priority = 0;
    Object_GetById(ACTOR_FOURTH_PILLAR)->priorityFlags = priority;
  }
  Object_GetById(ACTOR_FIFTH_PILLAR)->priorityFlags = priority;
}

/// Sets up whichever of the three rooms the party enters: the keys and doors
/// of CrossboneIsleKeys, the pursuers of CrossboneIslePlatformRooms2,
/// or the pillars and Bane of CrossboneIsleBane.
s32 Scene_Initialize(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (gGameState.scene == (s32)&SceneId_CrossboneIsleKeys) {
    CrossboneIslePlatformRooms_InitializeScene1();
  }
  if (gGameState.scene == (s32)&SceneId_CrossboneIslePlatformRooms2) {
    CrossboneIslePlatformRooms_InitializeScene2();
  }
  if (gGameState.scene == (s32)&SceneId_CrossboneIsleBane) {
    CrossboneIslePlatformRooms_InitializeScene3();
  }
  return 0;
}

/// Unused callback that cycles key icon displays.
UNUSED static void CrossboneIslePlatformRooms_UpdateItemIcons(void) {
  if (++gCrossboneIslePlatformRooms_IconTimer > 16) {
    gCrossboneIslePlatformRooms_IconTimer = 0;
  }
  switch (gCrossboneIslePlatformRooms_IconTimer) {
  case 12:
    if (Engine_GameFlagIsSet(0xee7) == 0) {
      OBJ_POS_SET(ACTOR_FIRST_BLUE_KEY, 232, 872);
    }
    if (Engine_GameFlagIsSet(0xee8) == 0) {
      OBJ_POS_SET(ACTOR_SECOND_BLUE_KEY, 296, 824);
    }
    if (Engine_GameFlagIsSet(0xee9) == 0) {
      OBJ_POS_SET(ACTOR_THIRD_BLUE_KEY, 328, 760);
    }
    if (Engine_GameFlagIsSet(0xeea) == 0) {
      OBJ_POS_SET(ACTOR_FOURTH_BLUE_KEY, 360, 872);
    }
    break;
  case 10:
    Colosso_ShowItemIcon(ACTOR_FIRST_BLUE_KEY, 0);
    break;
  case 8:
    Colosso_ShowItemIcon(ACTOR_SECOND_BLUE_KEY, 0);
    break;
  case 6:
    Colosso_ShowItemIcon(ACTOR_THIRD_BLUE_KEY, 0);
    break;
  case 4:
    Colosso_ShowItemIcon(ACTOR_FOURTH_BLUE_KEY, 0);
    break;
  case 2:
    Colosso_ShowItemIcon(ACTOR_RED_KEY, 1);
    break;
  }
}

#include "common/colosso_item_icon.h"

/// Restores collected keys and already opened passages.
static void CrossboneIslePlatformRooms_InitializeScene1(void) {
  Engine_TaskWait(1);
  Colosso_ShowItemIcon(ACTOR_RED_KEY, ITEM_RED_KEY);
  Colosso_ShowItemIcon(ACTOR_FOURTH_BLUE_KEY, ITEM_BLUE_KEY);
  Colosso_ShowItemIcon(ACTOR_THIRD_BLUE_KEY, ITEM_BLUE_KEY);
  Colosso_ShowItemIcon(ACTOR_SECOND_BLUE_KEY, ITEM_BLUE_KEY);
  Colosso_ShowItemIcon(ACTOR_FIRST_BLUE_KEY, ITEM_BLUE_KEY);
  if (FLAG_CHECK(0xee7) == 0) {
    OBJ_POS_SET(ACTOR_FIRST_BLUE_KEY, 232, 872);
  }
  if (FLAG_CHECK(0xee8) == 0) {
    OBJ_POS_SET(ACTOR_SECOND_BLUE_KEY, 296, 824);
  }
  if (FLAG_CHECK(0xee9) == 0) {
    OBJ_POS_SET(ACTOR_THIRD_BLUE_KEY, 328, 760);
  }
  if (FLAG_CHECK(0xeea) == 0) {
    OBJ_POS_SET(ACTOR_FOURTH_BLUE_KEY, 360, 872);
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN) != 0) {
    CrossboneIslePlatformRooms_OpenBlueDoor(0);
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN + 1) != 0) {
    CrossboneIslePlatformRooms_OpenBlueDoor(1);
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN + 2) != 0) {
    CrossboneIslePlatformRooms_OpenBlueDoor(2);
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN + 3) != 0) {
    CrossboneIslePlatformRooms_OpenBlueDoor(3);
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN) != 0) {
    CrossboneIslePlatformRooms_OpenRedDoor(0);
  }
}

/// Configures the pursuers, their chase triggers and blending.
static void CrossboneIslePlatformRooms_InitializeScene2(void) {
  Object_GetById(ACTOR_FIRST_PURSUER)->collisionFlags = 1;
  Object_GetById(ACTOR_SECOND_PURSUER)->collisionFlags = 1;
  Object_GetById(ACTOR_THIRD_PURSUER)->collisionFlags = 1;
  Object_GetById(ACTOR_FOURTH_PURSUER)->collisionFlags = 1;
  Object_GetById(ACTOR_FIRST_PURSUER)->scaleX = 7 * Q16_ONE / 10;
  Object_GetById(ACTOR_SECOND_PURSUER)->scaleX = 7 * Q16_ONE / 10;
  Object_GetById(ACTOR_THIRD_PURSUER)->scaleX = 7 * Q16_ONE / 10;
  Object_GetById(ACTOR_FOURTH_PURSUER)->scaleX = 7 * Q16_ONE / 10;
  Object_GetById(ACTOR_FIFTH_PURSUER)->scaleX = 7 * Q16_ONE / 10;
  FieldScheduler_AddOrUpdateCallback(
      CrossboneIslePlatformRooms_CheckFifthPursuerTrigger, 0xc80);
  FieldScheduler_AddOrUpdateCallback(
      CrossboneIslePlatformRooms_CheckSecondPursuerTrigger, 0xc80);
  FieldScheduler_AddCallback(
      CrossboneIslePlatformRooms_CheckFirstPursuerTrigger, 0xc80);
  Io_Write16(0x3f42, &REG_BLDCNT);
  Io_Write16(0x607, &REG_BLDALPHA);
}

/// Restores movable pillars, terrain and the Djinni escape progress.
static void CrossboneIslePlatformRooms_InitializeScene3(void) {
  struct FieldActor *djinni;

  // Put the chest back on the second pillar and rebuild all pillar support
  // cells.
  Object_GetById(ACTOR_SECOND_PILLAR_CHEST)->motionFlags = 0;
  FieldScheduler_AddOrUpdateCallback(
      CrossboneIslePlatformRooms_FollowSecondPillar, 0xc80);
  FieldScheduler_AddCallback(CrossboneIslePlatformRooms_UpdatePillarPriorities,
                             0xc80);
  MapObject_SetPosition(107, 0, 0);
  if (FLAG_CHECK(0xed9) != 0) {
    OBJ_MODE(ACTOR_SECOND_PILLAR_CHEST, 2);
  }
  Event_UpdateFirstPillarTerrain();
  Event_UpdateSecondPillarTerrain();
  Event_UpdateThirdPillarTerrain();
  Event_UpdateFourthPillarTerrain();
  Event_UpdateFifthPillarTerrain();
#if TBS_PATCH_1
  Engine_ActorSetSpritePriority(ACTOR_FIRST_PILLAR, 3);
#endif
  Object_GetById(ACTOR_FOURTH_PILLAR)->motionFlags = 0;
  Object_GetById(ACTOR_FIFTH_PILLAR)->motionFlags = 0;
  CrossboneIslePlatformRooms_RepaintFourthAndFifthPillarCells();
  if (FLAG_CHECK(FLAG_SCENE_TEMP_00) != 0) {
    Event_FreezePuddle();
    OBJ_MODE(ACTOR_PUDDLE, 5);
  }

  // Restore the surviving Djinni at the last completed escape stage.
  if (FLAG_CHECK(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE) != 0) {
      OBJ_POS_SET(ACTOR_BANE, 856, 824);
      djinni = Object_GetById(ACTOR_BANE);
#ifdef UBFIX
      djinni->update = CrossboneIslePlatformRooms_FaceBaneTowardLeader;
#else
      djinni->update = (void (*)(
          union FieldObject *))CrossboneIslePlatformRooms_FaceBaneTowardLeader;
#endif
    } else if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE) != 0) {
      OBJ_POS_SET(ACTOR_BANE, 888, 664);
      djinni = Object_GetById(ACTOR_BANE);
      djinni->sprite->rotation = 0;
      ObjectDispatch_SetAnimationSpeed((struct DispatchObject *)djinni, 16);
    } else if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE) != 0) {
      OBJ_POS_SET(ACTOR_BANE, 584, 680);
    } else {
      OBJ_POS_SET(ACTOR_BANE, 584, 664);
    }
  }
}

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))

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

s32 gCrossboneIslePlatformRooms_BaneSquashScript[] = {
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

s32 gCrossboneIslePlatformRooms_BaneWalkTargets[] = {
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

struct SceneEntrance gCrossboneIslePlatformRoomsEntrancesOther[] = {
    { 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIslePlatformRoomsEntrances1[] = {
    { CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_8_EAST, CONDITION_ALWAYS, 200, 0, 968, FACING_NORTH, 0, 24, 560, 504, 1008, 0 },
    { CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_8_WEST, CONDITION_ALWAYS, 88, 0, 968, FACING_NORTH, 0, 24, 560, 504, 1008, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIslePlatformRoomsEntrances2[] = {
    { CROSSBONE_ISLE_PLATFORM_ROOMS_2_FROM_CROSSBONE_ISLE_9_EAST, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 40, 0, 464, 440, 0 },
    { CROSSBONE_ISLE_PLATFORM_ROOMS_2_FROM_CROSSBONE_ISLE_9_WEST, CONDITION_ALWAYS, 88, 0, 392, FACING_NORTH, 0, 40, 0, 464, 440, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIslePlatformRoomsEntrances3[] = {
    { CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_11_EAST, CONDITION_ALWAYS, 776, 0, 952, FACING_NORTH, 0, 512, 544, 928, 992, 0 },
    { CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_11_WEST, CONDITION_ALWAYS, 664, 0, 952, FACING_NORTH, 0, 512, 544, 928, 992, 0 },
    { SCENE_TABLE_END },
};

u32 gCrossboneIslePlatformRoomsExits[] = {
    SCENE_EXITS(CrossboneIsleKeys),
    SCENE_EXIT(CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_8_EAST, CrossboneIsle8, CROSSBONE_ISLE_8_FROM_CROSSBONE_ISLE_KEYS_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_8_WEST, CrossboneIsle8, CROSSBONE_ISLE_8_FROM_CROSSBONE_ISLE_KEYS_WEST),
    SCENE_EXITS(CrossboneIslePlatformRooms2),
    SCENE_EXIT(CROSSBONE_ISLE_PLATFORM_ROOMS_2_TO_CROSSBONE_ISLE_9_EAST, CrossboneIsle9, CROSSBONE_ISLE_9_FROM_CROSSBONE_ISLE_PLATFORM_ROOMS_2_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_PLATFORM_ROOMS_2_TO_CROSSBONE_ISLE_9_WEST, CrossboneIsle9, CROSSBONE_ISLE_9_FROM_CROSSBONE_ISLE_PLATFORM_ROOMS_2_WEST),
    SCENE_EXITS(CrossboneIsleBane),
    SCENE_EXIT(CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_11_EAST, CrossboneIsle11, CROSSBONE_ISLE_11_FROM_CROSSBONE_ISLE_BANE_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_11_WEST, CrossboneIsle11, CROSSBONE_ISLE_11_FROM_CROSSBONE_ISLE_BANE_WEST),
    SCENE_EXITS_END,
};

struct ScenePlacement gCrossboneIslePlatformRoomsPlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gCrossboneIslePlatformRoomsPlacements1[] = {
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gCrossboneIslePlatformRoomsPlacements2[] = {
    { 0x1f4, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(152), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x1f4, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x1f4, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x1f4, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x1f4, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(312), 0, PIXELS(232), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gCrossboneIslePlatformRoomsPlacements3[] = {
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(616), 0, PIXELS(680), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(728), 0, PIXELS(744), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(600), 0, PIXELS(856), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(824), 0, PIXELS(888), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(840), 0, PIXELS(888), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(648), 0, PIXELS(888), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x14, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(728), 0, PIXELS(760), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    // Bane, until it is caught
    { 0x5a, FLAG_DJINNI_0 + DJINNI_BANE, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gCrossboneIslePlatformRoomsEventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gCrossboneIslePlatformRoomsEvents1[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_8_EAST },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_8_WEST },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ShowRedKey) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HideRedKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_FIRST_BLUE_KEY, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CatchFirstBlueKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_SECOND_BLUE_KEY, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CatchSecondBlueKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_THIRD_BLUE_KEY, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CatchThirdBlueKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_FOURTH_BLUE_KEY, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CatchFourthBlueKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_RED_KEY, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CatchRedKey) },
    { EVENT_TOUCH, 50, CONDITION_ALWAYS, EVENT_SCRIPT(Event_UseRedKey) },
    { EVENT_TOUCH, 51, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenFirstBlueDoor) },
    { EVENT_TOUCH, 52, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenSecondBlueDoor) },
    { EVENT_TOUCH, 53, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenThirdBlueDoor) },
    { EVENT_TOUCH, 54, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenFourthBlueDoor) },
    // Unused: the plain touch rows above answer codes 53 and 54 first.
    { TOUCH_WALKING_INTO(FACING_SOUTH), 53, CONDITION_ALWAYS, EVENT_SCRIPT(Event_UnusedCheckLeaderAhead) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 54, CONDITION_ALWAYS, EVENT_SCRIPT(Event_UnusedCheckLeaderAhead) },
    { SEARCH(SEARCH_CHEST), 100, 0xeca, FIND_COINS(333) },
    { SEARCH(SEARCH_CHEST), 101, 0xecb, FIND_ITEM(ITEM_FAIRY_RING) },
    { SEARCH(SEARCH_CHEST), 102, 0xecc, FIND_ITEM(ITEM_COOKIE) },
    { SEARCH(SEARCH_CHEST), 103, 0xecd, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gCrossboneIslePlatformRoomsEvents2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_PLATFORM_ROOMS_2_TO_CROSSBONE_ISLE_9_EAST },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_PLATFORM_ROOMS_2_TO_CROSSBONE_ISLE_9_WEST },
    { EVENT_TOUCH, 53, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TrackThirdPursuerColumn) },
    { EVENT_TOUCH, 54, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TrackFourthPursuerColumn) },
    { EVENT_TOUCH, 61, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DodgeThirdPursuer) },
    { EVENT_TOUCH, 62, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DodgeFourthPursuer) },
    { EVENT_RAISED, TRIGGER_FIFTH_CHASE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DodgeFifthPursuer) },
    { EVENT_RAISED, TRIGGER_SECOND_CHASE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DodgeSecondPursuer) },
    { EVENT_RAISED, TRIGGER_FIRST_CHASE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DodgeFirstPursuer) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_BeginRevealBlend) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EndRevealBlend) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_FIRST_PURSUER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HaltPursuer) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_SECOND_PURSUER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HaltPursuer) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_THIRD_PURSUER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HaltPursuer) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_FOURTH_PURSUER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HaltPursuer) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_FIFTH_PURSUER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HaltPursuer) },
    { SEARCH(SEARCH_CHEST), 100, 0xece, FIND_COINS(444) },
    { SEARCH(SEARCH_CHEST), 101, 0xecf, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SEARCH(SEARCH_CHEST), 102, 0xed0, FIND_ITEM(ITEM_STORM_GEAR) },
    { SEARCH(SEARCH_CHEST), 103, 0xed1, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    { SCENE_EVENTS_END },
};
// clang-format on

struct SceneEvent gCrossboneIslePlatformRoomsEvents3[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS,
     CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_11_EAST},
    {EVENT_EXIT, 2, CONDITION_ALWAYS,
     CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_11_WEST},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 50, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_PushThirdPillar)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 51, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_PushFirstPillar)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 52, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_PushSecondPillar)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 53, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_PushFourthPillar)},
#if TBS_PATCH_1
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 61, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_PushFirstPillar)},
#endif
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 54, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_TouchThirdPillar)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 60, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_UseLadder)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 70, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_LookAheadOrPushFirstPillar)},
    {EVENT_TOUCH, 20, FLAG_DJINNI_0 + DJINNI_BANE,
     EVENT_SCRIPT(Event_BaneFirstEscape)},
    {EVENT_TOUCH, 21, FLAG_DJINNI_0 + DJINNI_BANE,
     EVENT_SCRIPT(Event_BaneSecondEscape)},
    {EVENT_TOUCH, 22, FLAG_DJINNI_0 + DJINNI_BANE,
     EVENT_SCRIPT(Event_BaneLastEscape)},
    {EVENT_TOUCH, 30, FLAG_DJINNI_0 + DJINNI_BANE,
     EVENT_SCRIPT(Event_BaneKeepsAway)},
    {EVENT_TOUCH, 31, FLAG_DJINNI_0 + DJINNI_BANE,
     EVENT_SCRIPT(Event_BaneKeepsAway)},
    {EVENT_TOUCH, 32, FLAG_DJINNI_0 + DJINNI_BANE,
     EVENT_SCRIPT(Event_BaneKeepsAway)},
    {EVENT_TOUCH, 33, FLAG_DJINNI_0 + DJINNI_BANE,
     EVENT_SCRIPT(Event_BaneKeepsAway)},
    {EVENT_TOUCH, 34, FLAG_DJINNI_0 + DJINNI_BANE,
     EVENT_SCRIPT(Event_BaneKeepsAway)},
    {EVENT_TOUCH, 35, FLAG_DJINNI_0 + DJINNI_BANE,
     EVENT_SCRIPT(Event_BaneKeepsAway)},
    {EVENT_TOUCH, 36, FLAG_DJINNI_0 + DJINNI_BANE,
     EVENT_SCRIPT(Event_BaneKeepsAway)},
    {EVENT_TOUCH, 37, FLAG_DJINNI_0 + DJINNI_BANE,
     EVENT_SCRIPT(Event_BaneKeepsAway)},
    {EVENT_TOUCH, 38, FLAG_DJINNI_0 + DJINNI_BANE,
     EVENT_SCRIPT(Event_BaneKeepsAway)},
    {EVENT_TOUCH, 39, FLAG_DJINNI_0 + DJINNI_BANE,
     EVENT_SCRIPT(Event_BaneKeepsAway)},
    {EVENT_TOUCH, 40, FLAG_DJINNI_0 + DJINNI_BANE,
     EVENT_SCRIPT(Event_BaneKeepsAway)},
    {EVENT_TOUCH, 41, FLAG_DJINNI_0 + DJINNI_BANE,
     EVENT_SCRIPT(Event_BaneKeepsAway)},
    {0x7, ACTOR_BANE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_BaneJoins)},
    {PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_BANE, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_HaltBane)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_SCENE_TEMP_00,
     EVENT_SCRIPT(Event_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR, FLAG_SCENE_TEMP_01,
     EVENT_SCRIPT(Event_UpdateFirstPillarTerrain)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_PILLAR, FLAG_SCENE_TEMP_06,
     EVENT_SCRIPT(Event_UpdateSecondPillarTerrain)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_PILLAR, FLAG_SCENE_TEMP_03,
     EVENT_SCRIPT(Event_UpdateThirdPillarTerrain)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FOURTH_PILLAR, FLAG_SCENE_TEMP_04,
     EVENT_SCRIPT(Event_UpdateFourthPillarTerrain)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIFTH_PILLAR, FLAG_SCENE_TEMP_05,
     EVENT_SCRIPT(Event_UpdateFifthPillarTerrain)},
    {SEARCH(SEARCH_CHEST), 104, 0xed6, FIND_COINS(666)},
    {SEARCH(SEARCH_CHEST), 105, 0xed7, FIND_ITEM(ITEM_WATER_OF_LIFE)},
    {SEARCH(SEARCH_CHEST), 106, 0xed8, FIND_ITEM(ITEM_ELIXIR)},
    {SEARCH(SEARCH_CHEST), 107, 0xed9, FIND_ITEM(ITEM_DEMON_AXE)},
    {SCENE_EVENTS_END},
};

u32 gCrossboneIslePlatformRooms_SlotColumns[] = {
    16,
    21,
    0,
};

s32 gCrossboneIslePlatformRooms_IconTimer = 0;
