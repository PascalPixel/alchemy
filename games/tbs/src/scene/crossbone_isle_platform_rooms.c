// Crossbone Isle's platform rooms: the room of keys, the room of pursuers
// that Cloak and Halt get past, and the room of blocks where Bane hides.
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

#if TBS_PATCH_1
#define CROSSBONE_PURSUER_STEP_Z 16
#define CROSSBONE_LEADER_STEP_Z 24
#else
#define CROSSBONE_PURSUER_STEP_Z 24
#define CROSSBONE_LEADER_STEP_Z 16
#endif

// Forward declarations.
static void Event_PushFirstBlock(void);
static void CrossboneIslePlatformRooms_FollowSecondBlock(void);
static void CrossboneIslePlatformRooms_PositionSecondBlockMapObject(void);
static void CrossboneIslePlatformRooms_InitializeScene1(void);
static void CrossboneIslePlatformRooms_InitializeScene2(void);
static void CrossboneIslePlatformRooms_InitializeScene3(void);

enum { ITEM_RED_KEY = 243, ITEM_BLUE_KEY = 244 };

// The Djinni Bane, in the block room: Venus Djinni 6 (name message 1125).
enum { BANE = 15 };
#define DJINNI_BANE 6

enum { PASSAGE_INDEX_MASK = 0xff, PASSAGE_PLAY_OPENING = 0x100 };

// The block room's temporary flags: FLAG_SCENE_TEMP_00 the puddle is frozen,
// 01 to 05 end Move on each block, 06 the second block's platform has reached
// the ladder, 07 selects the ladder cell's other attributes. The pursuer room
// keeps FLAG_SCENE_TEMP_20 while a pursuer is after the leader.

// Publish the next raised-event trigger.
static __inline__ void SceneState_StoreStep(s16 *field, s32 step) {
  *field = step;
}

// Lift the first block and clear the terrain beneath its column.
static __inline__ void LiftBlock(s32 column, u8 motionFlags) {
  {
    s32 destinationColumn = column;
    Map_CopyCellAttributes(61, 36, 1, 1, destinationColumn, 42);
  }
  Object_GetById(8)->motionFlags = motionFlags;
  Object_GetById(8)->y.fixed = PIXELS(32);
}

// Shared source is included here in ROM order.
#include "common/object_staged_actor.h"

// Raised once the battle with Bane is won: Bane joins the party.
static void Event_BaneJoins(void) {
  Djinn_JoinParty(BANE, ELEMENT_VENUS, DJINNI_BANE);
}

// Unused scene callback with no behavior.
UNUSED static void CrossboneIslePlatformRooms_UnusedHook(void) {}

// Select entrances for the current platform room.
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 selector = gGameState.scene;

  if (selector == (s32)&SceneId_CrossboneIslePlatformRooms1) {
    return gCrossboneIslePlatformRoomsEntrances1;
  }
  if (selector == (s32)&SceneId_CrossboneIslePlatformRooms2) {
    return gCrossboneIslePlatformRoomsEntrances2;
  }
  if (selector == (s32)&SceneId_CrossboneIslePlatformRooms3) {
    return gCrossboneIslePlatformRoomsEntrances3;
  }
  return gCrossboneIslePlatformRoomsEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gCrossboneIslePlatformRoomsExits; }

// Select actor placements for the current platform room.
const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 selector = gGameState.scene;

  if (selector == (s32)&SceneId_CrossboneIslePlatformRooms1) {
    return gCrossboneIslePlatformRoomsPlacements1;
  }
  if (selector == (s32)&SceneId_CrossboneIslePlatformRooms2) {
    return gCrossboneIslePlatformRoomsPlacements2;
  }
  if (selector == (s32)&SceneId_CrossboneIslePlatformRooms3) {
    return gCrossboneIslePlatformRoomsPlacements3;
  }
  return gCrossboneIslePlatformRoomsPlacementsOther;
}

// Halt on a pursuer: nothing beyond the Psynergy's own effect.
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
  struct FieldActor *leader = Object_GetById(0);

  actor->facing = ArcTan2(leader->z.fixed - actor->z.fixed,
                          leader->x.fixed - actor->x.fixed);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

// The first time the party comes near: Bane notices them and hops away.
static void Event_BaneFirstEscape(void) {
  struct FieldActor *djinni;

  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE) == 0) {
    FLAG_SET(FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE);
    EV_BEGIN();
    Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
    Engine_CameraMoveToActor(BANE, 1);
    Engine_CameraWaitForMove();
    OBJ_DIR(BANE, FACING_EAST, 20);
    Actor_SetAttachedEffect(BANE, 0x102);
    Engine_ActorRunRepeatedMotion(15, 2);
    WAIT(20);
    OBJ_SPEED(BANE, PIXELS(1), PIXELS(1) / 2);
    SE(SE_152);
    djinni = Object_GetById(BANE);
    djinni->velocityY = PIXELS(8);
    Actor_WalkToAndWait(BANE, 584, 680);
    OBJ_DIR(BANE, FACING_SOUTH, 20);
    EV_END();
  }
}

// Coming near again: Bane hops, dashes across the platforms and crashes into
// the far wall, shaking the room, and is left squashing and stretching.
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
      Engine_CameraMoveToActor(BANE, 1);
      Engine_CameraWaitForMove();
      OBJ_DIR(BANE, FACING_SOUTH, 20);
      Actor_SetAttachedEffect(BANE, 0x102);
      Engine_ActorRunRepeatedMotion(15, 2);
      WAIT(20);
      OBJ_SPEED(BANE, PIXELS(1), PIXELS(1) / 2);
      SE(SE_152);
      jumpingDjinni = Object_GetById(BANE);
      jumpingDjinni->velocityY = PIXELS(10);
      Actor_WalkToAndWait(BANE, 584, 664);
      OBJ_DIR(BANE, FACING_SOUTH, 20);
      Actor_SetAttachedEffect(BANE, 0x102);
      WAIT(30);

      // Dash across the platforms, then shake the room on landing.
      OBJ_SPEED(BANE, PIXELS(8), PIXELS(1) / 4);
      Actor_WalkToAndWait(BANE, 664, 664);
      Actor_WalkToAndWait(BANE, 744, 664);
      Actor_WalkToAndWait(BANE, 824, 664);
      WAIT(10);
      SE(SE_208);
      Map_SetShake(PIXELS(4), PIXELS(2), Q16_ONE);
      WAIT(20);
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
      WAIT(30);

      // Leave the Djinni tilted while its scale animation continues.
      OBJ_POS_SET(BANE, 888, 664);
      spinningDjinni = Object_GetById(BANE);
      spinningDjinni->sprite->rotation = 0xf800;
      spinningDjinni->facing = 0;
      ObjectDispatch_SetAnimationSpeed(spinningDjinni, 0);
      Engine_ObjectSetScript(spinningDjinni,
                             gCrossboneIslePlatformRooms_BaneSquashScript);
      EV_END();
    }
  }
}

// The third time: Bane recovers, hops down a winding path to the room's last
// corner and keeps watching the leader.
static void Event_BaneLastEscape(void) {
  struct FieldActor *djinni;

  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE) != 0 &&
      FLAG_CHECK(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE) == 0) {
    FLAG_SET(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE);
    EV_BEGIN();

    // Restore the sprite before the last jump and winding escape path.
    djinni = Object_GetById(BANE);
    djinni->sprite->rotation = 0;
    ObjectDispatch_SetAnimationSpeed(djinni, 16);
    SE(SE_152);
    djinni = Object_GetById(BANE);
    djinni->velocityY = PIXELS(8);
    OBJ_DIR(BANE, FACING_WEST, 30);
    Actor_SetAttachedEffect(BANE, 0x102);
    Engine_ActorRunRepeatedMotion(15, 2);
    WAIT(20);
    OBJ_SPEED(BANE, PIXELS(1), PIXELS(1) / 2);
    SE(SE_152);
    djinni = Object_GetById(BANE);
    djinni->velocityY = PIXELS(4);
    Actor_WalkToAndWait(BANE, 880, 680);
    WAIT(10);
    Actor_SetAttachedEffect(BANE, 0x101);
    OBJ_SPEED(BANE, PIXELS(2), PIXELS(1));
    Actor_WalkToAndWait(BANE, 880, 696);
    Actor_WalkToAndWait(BANE, 882, 704);
    Actor_WalkToAndWait(BANE, 880, 712);
    Actor_WalkToAndWait(BANE, 878, 720);
    Actor_WalkToAndWait(BANE, 880, 728);
    Actor_WalkToAndWait(BANE, 882, 736);
    Actor_WalkToAndWait(BANE, 880, 744);
    Actor_WalkToAndWait(BANE, 878, 752);
    Actor_WalkToAndWait(BANE, 880, 760);
    OBJ_POS_SET(BANE, 856, 824);
    WAIT(10);
    OBJ_DIR(BANE, FACING_NORTH, 20);
    Actor_SetAttachedEffect(BANE, 0x100);
    djinni = Object_GetById(BANE);
#ifdef UBFIX
    djinni->update = CrossboneIslePlatformRooms_FaceBaneTowardLeader;
#else
    djinni->update = (void (*)(
        union FieldObject *))CrossboneIslePlatformRooms_FaceBaneTowardLeader;
#endif
    EV_END();
  }
}

// Stepping on the triggers around Bane's corner (30 to 41): Bane runs to the
// matching spot, as fast as the leader, unless Halt holds it.
static void Event_BaneKeepsAway(void) {
  struct FieldActor *djinni;
  struct FieldActor *leader;
  s32 targetByteOffset;
  const u8 *targets;
  s32 zByteOffset;
  struct EventWork *work;

  work = gEventWork;
  if (Engine_GameFlagIsSet(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE) != 0) {
    if (gGameState.pausedObject != BANE) {
      targetByteOffset = work->touchedTrigger;
      djinni = Object_GetById(BANE);
      leader = Object_GetById(0);
      djinni->speed = leader->speed;
      djinni = Object_GetById(BANE);
      leader = Object_GetById(0);
      djinni->acceleration = leader->speed;
      targetByteOffset -= 30;
      targets = (const u8 *)gCrossboneIslePlatformRooms_BaneWalkTargets;
      // The table stores consecutive signed X/Z pixel pairs.
      targetByteOffset *= 2 * sizeof(s32);
      zByteOffset = targetByteOffset + sizeof(s32);
      Engine_ActorWalkTo(15, *(const s32 *)(targets + targetByteOffset),
                         *(const s32 *)(targets + zByteOffset));
    }
  }
}

// Halt on Bane: it freezes in place.
static void Event_HaltBane(void) {
  EV_BEGIN();
  OBJ_MODE(15, 0);
  EV_END();
}

// Select events for the current platform room.
const struct SceneEvent *Scene_GetEvents(void) {
  s16 selector = gGameState.scene;

  if (selector == (s32)&SceneId_CrossboneIslePlatformRooms1) {
    return gCrossboneIslePlatformRoomsEvents1;
  }
  if (selector == (s32)&SceneId_CrossboneIslePlatformRooms2) {
    return gCrossboneIslePlatformRoomsEvents2;
  }
  if (selector == (s32)&SceneId_CrossboneIslePlatformRooms3) {
    return gCrossboneIslePlatformRoomsEvents3;
  }
  return gCrossboneIslePlatformRoomsEventsOther;
}

// Walking south into the third or fourth blue door: look at what is ahead.
static void Event_CheckLeaderAhead(void) { Engine_LeaderCheckAhead(); }

// Open the selected blue-key passage, optionally playing its opening.
static void CrossboneIslePlatformRooms_OpenPassage(s32 passageInfo) {
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

// Open the red-key passage, optionally playing its opening.
static void CrossboneIslePlatformRooms_OpenPlatformPassage(s32 passageInfo) {
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

// Facing the red door with the Red Key: the door rumbles open, and the key is
// used up.
static void Event_UseRedKey(void) {
  struct FieldActor *leader;

  leader = Object_GetById(0);
  if (leader->facing == FACING_NORTH) {
    if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN) == 0) {
      if (PartyInventory_FindOwner(ITEM_RED_KEY) != -1) {
        FLAG_SET(FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN);
        CrossboneIslePlatformRooms_OpenPlatformPassage(PASSAGE_PLAY_OPENING);
        PartyInventory_Remove(ITEM_RED_KEY);
      }
    }
  }
}

// Facing a blue door with a Blue Key: the door rumbles open, and the key is
// used up.
static void CrossboneIslePlatformRooms_UseBlueKey(s32 doorIndex) {
  struct FieldActor *leader = Object_GetById(0);
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
  CrossboneIslePlatformRooms_OpenPassage(PASSAGE_PLAY_OPENING | doorIndex);
  PartyInventory_Remove(ITEM_BLUE_KEY);
}

// Touching the first blue door.
static void Event_OpenFirstBlueDoor(void) {
  CrossboneIslePlatformRooms_UseBlueKey(0);
}

// Touching the second blue door.
static void Event_OpenSecondBlueDoor(void) {
  CrossboneIslePlatformRooms_UseBlueKey(1);
}

// Touching the third blue door; pressing on it from the north looks ahead
// every dozen blocked steps.
static void Event_OpenThirdBlueDoor(void) {
  struct FieldActor *leader;
  s16 *blockedSteps;
  s32 resetSteps;

  CrossboneIslePlatformRooms_UseBlueKey(2);
  leader = Object_GetById(0);
  if (leader->facing == FACING_SOUTH) {
    blockedSteps = &gEventWork->blockedSteps;
    if (*blockedSteps > 12) {
      Engine_LeaderCheckAhead();
      resetSteps = 0;
      *blockedSteps = resetSteps;
    }
  }
}

// Touching the fourth blue door; pressing on it from the north looks ahead
// every dozen blocked steps.
static void Event_OpenFourthBlueDoor(void) {
  struct FieldActor *leader;
  s16 *blockedSteps;
  s32 resetSteps;

  CrossboneIslePlatformRooms_UseBlueKey(3);
  leader = Object_GetById(0);
  if (leader->facing == FACING_SOUTH) {
    blockedSteps = &gEventWork->blockedSteps;
    if (*blockedSteps > 12) {
      Engine_LeaderCheckAhead();
      resetSteps = 0;
      *blockedSteps = resetSteps;
    }
  }
}

// Reveal in the key room: the hidden Red Key appears on its tile.
static void Event_ShowRedKey(void) {
  s32 keyActorId = 12;
  s32 x = PIXELS(408);
  s32 z = PIXELS(776);

  Map_CopyCells(25, 45, 1, 2, 25, 48);
  if (Engine_GameFlagIsSet(0xeeb) == 0)
    Engine_ActorSetPosition(keyActorId, x, z);
  WAIT(1);
}

// Reveal wears off: the Red Key's tile is hidden again.
static void Event_HideRedKey(void) {
  Map_CopyCells(24, 48, 1, 2, 25, 48);
  OBJ_POS_SET(12, 8, 8);
}

// Collect the selected key, hide its actor and remember it was taken.
static inline void CrossboneIslePlatformRooms_CollectKey(s32 actorId,
                                                         s32 itemId,
                                                         s32 collectedFlag) {
  Engine_ItemShowFound(itemId, 3);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_PartyGiveItem(itemId, 0);
  Engine_ActorSetPosition(actorId, 0, 0);
  Engine_GameFlagSet(collectedFlag);
}

// Catch on the first Blue Key: the party gets it.
static void Event_CatchFirstBlueKey(void) {
  CrossboneIslePlatformRooms_CollectKey(8, ITEM_BLUE_KEY, 0xee7);
}

// Catch on the second Blue Key.
static void Event_CatchSecondBlueKey(void) {
  CrossboneIslePlatformRooms_CollectKey(9, ITEM_BLUE_KEY, 0xee8);
}

// Catch on the third Blue Key.
static void Event_CatchThirdBlueKey(void) {
  CrossboneIslePlatformRooms_CollectKey(10, ITEM_BLUE_KEY, 0xee9);
}

// Catch on the fourth Blue Key.
static void Event_CatchFourthBlueKey(void) {
  CrossboneIslePlatformRooms_CollectKey(11, ITEM_BLUE_KEY, 0xeea);
}

// Catch on the revealed Red Key.
static void Event_CatchRedKey(void) {
  CrossboneIslePlatformRooms_CollectKey(12, ITEM_RED_KEY, 0xeeb);
}

// Start the first chase when an uncloaked leader enters its area.
static void CrossboneIslePlatformRooms_CheckFirstPursuerTrigger(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 column = leader->x.fixed / PIXELS(16);
  s32 row = leader->z.fixed / PIXELS(16);
  struct EventWork *event = gEventWork;

  if (!FLAG_CHECK(FLAG_SCENE_TEMP_20) && gGameState.cloaked == 0 &&
      gGameState.pausedObject != 8 && (u32)(column - 21) <= 2 && row >= 10 &&
      row <= 11) {
    Engine_GameFlagSet(FLAG_SCENE_TEMP_20);
    event->raisedTrigger = 91;
  }
}

// Let the first pursuer follow the leader, then return it home.
static void Event_DodgeFirstPursuer(void) {
  struct FieldActor *leader;

  EV_BEGIN();
  OBJ_SPEED(ACTOR_PARTY_LEADER, PIXELS(19) / 10, PIXELS(19) / 20);
  OBJ_SPEED(8, PIXELS(19) / 10, PIXELS(19) / 20);
  SE(SE_188);
  leader = Object_GetById(0);
  if (leader != 0) {
    Actor_SetDestination(8, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(8);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 24);
  WAIT(4);
  SE(SE_188);
  Actor_SetDestinationOffset(8, 0, CROSSBONE_PURSUER_STEP_Z);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Actor_SetDestination(8, 360, 152);
  Engine_ActorWaitForMove(8);
  EV_END();
  GameFlag_Clear(FLAG_SCENE_TEMP_20);
}

// Start the second chase when the leader enters column ten.
static void CrossboneIslePlatformRooms_CheckSecondPursuerTrigger(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 column = leader->x.fixed / PIXELS(16);
  s32 row = leader->z.fixed / PIXELS(16);
  struct EventWork *work = gEventWork;

  if (FLAG_CHECK(FLAG_SCENE_TEMP_20) == 0 && gGameState.cloaked == 0 &&
      gGameState.pausedObject != 9 && column == 10 && (u32)(row - 16) <= 2) {
    FLAG_SET(FLAG_SCENE_TEMP_20);
    SceneState_StoreStep(&work->raisedTrigger, 92);
  }
}

// Let the second pursuer follow the leader, then return it home.
static void Event_DodgeSecondPursuer(void) {
  struct FieldActor *leader;

  EV_BEGIN();
  OBJ_SPEED(ACTOR_PARTY_LEADER, PIXELS(17) / 10, PIXELS(17) / 20);
  OBJ_SPEED(9, PIXELS(17) / 10, PIXELS(17) / 20);
  SE(SE_188);
  leader = Object_GetById(0);
  if (leader != 0) {
    Actor_SetDestination(9, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(9);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, CROSSBONE_LEADER_STEP_Z);
  SE(SE_188);
  WAIT(4);
  Actor_SetDestinationOffset(9, 0, 16);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Actor_SetDestination(9, 168, 264);
  Engine_ActorWaitForMove(9);
  EV_END();
  GameFlag_Clear(FLAG_SCENE_TEMP_20);
}

// Move a pursuer to the leader column and remember its last column.
static void CrossboneIslePlatformRooms_TrackPursuerColumn(s32 pursuerIndex) {

  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 column = leader->x.fixed / PIXELS(16);
  s32 row = leader->z.fixed / PIXELS(16);
  s32 pursuerId = pursuerIndex + 10;

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

// Align the third actor with the leader column.
static void Event_TrackThirdPursuerColumn(void) {
  CrossboneIslePlatformRooms_TrackPursuerColumn(0);
}

// Align the fourth actor with the leader column.
static void Event_TrackFourthPursuerColumn(void) {
  CrossboneIslePlatformRooms_TrackPursuerColumn(1);
}

// Unused callback that aligns the fifth actor with the leader column.
UNUSED static void CrossboneIslePlatformRooms_TrackFifthPursuerColumn(void) {
  CrossboneIslePlatformRooms_TrackPursuerColumn(2);
}

// Let the third pursuer follow the leader, then return it home.
static void Event_DodgeThirdPursuer(void) {
  struct FieldActor *leader;

  if (gGameState.pausedObject != 10) {
    EV_BEGIN();
    OBJ_SPEED(ACTOR_PARTY_LEADER, PIXELS(17) / 10, PIXELS(17) / 20);
    OBJ_SPEED(10, PIXELS(17) / 10, PIXELS(17) / 20);
    SE(SE_188);
    leader = Object_GetById(0);
    if (leader != 0) {
      Actor_SetDestination(10, leader->x.part.pixel, leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(10);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, CROSSBONE_LEADER_STEP_Z);
    WAIT(4);
    SE(SE_188);
    Actor_SetDestinationOffset(10, 0, 16);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetDestination(10, 264, 360);
    Engine_ActorWaitForMove(10);
    WAIT(10);
    EV_END();
  }
}

// Align the fourth pursuer before sending it after the leader.
static void Event_DodgeFourthPursuer(void) {
  struct FieldActor *leader;
  s32 leaderColumn;

  if (gGameState.pausedObject != 11) {
    EV_BEGIN();
    OBJ_SPEED(0, PIXELS(17) / 10, PIXELS(17) / 20);
    Engine_ActorSetSpeed(11, PIXELS(17) / 10, PIXELS(17) / 20);
    Engine_AudioPlayCue(SE_188);
    leaderColumn = Object_GetById(0)->x.fixed / PIXELS(16);
    if (leaderColumn > Object_GetById(11)->x.fixed / PIXELS(16)) {
      Engine_ActorSetDestinationOffset(11, 8, 0);
    }
    leaderColumn = Object_GetById(0)->x.fixed / PIXELS(16);
    if (leaderColumn < Object_GetById(11)->x.fixed / PIXELS(16)) {
      FieldObjectMotion_OffsetPositionAndResetMotion(11, -8, 0);
    }
    Engine_ActorWaitForMove(11);
    leader = Object_GetById(0);
    if (leader != 0) {
      Engine_ActorSetDestination(11, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(11);
    Engine_ActorSetDestinationOffset(0, 0, CROSSBONE_LEADER_STEP_Z);
    WAIT(4);
    Engine_AudioPlayCue(SE_188);
    Engine_ActorSetDestinationOffset(11, 0, 16);
    Engine_ActorWaitForMove(0);
    Engine_ActorSetDestination(11, 344, 360);
    Engine_ActorWaitForMove(11);
    WAIT(10);
    EV_END();
  }
}

// Start the fifth chase when the leader enters column nineteen.
static void CrossboneIslePlatformRooms_CheckFifthPursuerTrigger(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 column = leader->x.fixed / PIXELS(16);
  s32 row = leader->z.fixed / PIXELS(16);
  struct EventWork *work = gEventWork;
  struct GameState *state = &gGameState;

  if (state->pausedObject != 12 && FLAG_CHECK(FLAG_SCENE_TEMP_20) == 0 &&
      state->cloaked == 0 && column == 19 && (u32)(row - 15) <= 1) {
    FLAG_SET(FLAG_SCENE_TEMP_20);
    SceneState_StoreStep(&work->raisedTrigger, 96);
  }
}

// Let the fifth pursuer follow the leader, then return it home.
static void Event_DodgeFifthPursuer(void) {
  struct FieldActor *leader;

  EV_BEGIN();
  OBJ_SPEED(ACTOR_PARTY_LEADER, PIXELS(17) / 10, PIXELS(17) / 20);
  OBJ_SPEED(12, PIXELS(17) / 10, PIXELS(17) / 20);
  SE(SE_188);
  leader = Object_GetById(0);
  if (leader != 0) {
    Actor_SetDestination(12, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(12);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, CROSSBONE_LEADER_STEP_Z);
  SE(SE_188);
  Actor_SetDestinationOffset(12, 0, 16);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Actor_SetDestination(12, 312, 232);
  Engine_ActorWaitForMove(12);
  EV_END();
  GameFlag_Clear(FLAG_SCENE_TEMP_20);
}

// Reveal in the pursuer room: the translucent layer over the room vanishes
// while it lasts.
static void Event_BeginRevealBlend(void) {
  u32 coefficient = 208;

  coefficient <<= 4;
  REG_BLDALPHA = coefficient;
}

// Reveal wears off: the translucent layer returns.
static void Event_EndRevealBlend(void) {
  u16 value = 0x0607;

  REG_BLDALPHA = value;
}

// At the ladder by the second block: climb up or down, look ahead to the
// east, or push the first block from the floor to the west.
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
      Event_PushFirstBlock();
    } else {
      Engine_LeaderCheckAhead();
    }
  }
}

// Unused scene callback with no behavior.
UNUSED static void CrossboneIslePlatformRooms_UnusedCallback(void) {}

// Frost on the puddle: it freezes into a pillar of ice the party can stand
// on.
static void Event_FreezePuddle(void) {
  struct FieldActor *puddle = Object_GetById(13);

  Map_CopyCellAttributes(40, 54, 1, 1, 40, 55);
  if (puddle != 0) {
    u8 *motionFlags = &Object_GetById(13)->motionFlags;
    u8 *priorityFlags = &puddle->priorityFlags;

    *motionFlags = 0;
    *priorityFlags = 2;
  }
  Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
}

// Walking into the first block from the ladder side (trigger 70): facing east,
// look ahead; otherwise push the block.
static void Event_LookAheadOrPushFirstBlock(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  if (leader->facing == 0) {
    Engine_LeaderCheckAhead();
  } else {
    Event_PushFirstBlock();
  }
}

// Restore the first block row before applying its current footprint.
static void CrossboneIslePlatformRooms_RestoreFirstBlockTerrain(void) {
  Map_CopyCellAttributes(57, 42, 1, 1, 40, 42);
  Map_CopyCellAttributes(57, 42, 1, 1, 41, 42);
  Map_CopyCellAttributes(58, 42, 1, 1, 42, 42);
  Map_CopyCellAttributes(62, 37, 3, 1, 37, 42);

  Object_GetById(8)->motionFlags = 1;
}

// Restore support under the first block at the eastern cell.
static void CrossboneIslePlatformRooms_RestoreEastUnderfootCell(void) {

  Map_CopyCellAttributes(58, 41, 1, 1, 42, 42);
  Object_GetById(8)->priorityFlags = 2;
}

// Restore support under the first block at the middle cell.
static void CrossboneIslePlatformRooms_RestoreMiddleUnderfootCell(void) {

  Map_CopyCellAttributes(44, 42, 1, 1, 41, 42);
  Object_GetById(8)->priorityFlags = 2;
}

// Restore support under the first block at the western cell.
static void CrossboneIslePlatformRooms_RestoreWestUnderfootCell(void) {

  Map_CopyCellAttributes(39, 42, 1, 1, 40, 42);
  Object_GetById(8)->priorityFlags = 2;
}

// Rebuild the first block support cells from its height and column.
static void Event_UpdateFirstBlockTerrain(void) {
  struct FieldActor *actor = Object_GetById(8);
  s32 x = actor->x.fixed;
  s32 height = actor->y.fixed;
  s32 column = x / PIXELS(16);
  u8 lowered;

  if (height == 0) {
    Object_GetById(8)->priorityFlags = 2;
  }
  CrossboneIslePlatformRooms_RestoreFirstBlockTerrain();
  lowered = 0;
  Object_GetById(8)->motionFlags = 3;
  if (column == 40) {
    CrossboneIslePlatformRooms_RestoreWestUnderfootCell();
  } else if (column == 42) {
    CrossboneIslePlatformRooms_RestoreEastUnderfootCell();
  } else if (column == 41) {
    CrossboneIslePlatformRooms_RestoreMiddleUnderfootCell();
  } else if (column == 39) {
    LiftBlock(column, lowered);
  } else if (column == 38) {
    LiftBlock(column, lowered);
  } else if (column == 37) {
    LiftBlock(column, lowered);
  }
}

// Pushing the first block (trigger 51); standing at the ladder in column 38,
// the leader climbs instead.
static void Event_PushFirstBlock(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldActor *block = Object_GetById(8);
  s32 leaderColumn = leader->x.fixed / PIXELS(16);
  s32 blockColumn = block->x.fixed / PIXELS(16);

  if (leaderColumn == 38 && blockColumn != 38) {
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

  CrossboneIslePlatformRooms_RestoreFirstBlockTerrain();
  StagedActor_AdvancePair();
  Event_UpdateFirstBlockTerrain();
}

// Detach the second block follower and restore the ladder cell.
static void CrossboneIslePlatformRooms_ReleaseSecondBlockPlatform(void) {
  Scheduler_RemoveCallback(CrossboneIslePlatformRooms_FollowSecondBlock);
  OBJ_POS_SET(14, 0, 0);
  if (FLAG_CHECK(FLAG_SCENE_TEMP_07) != 0) {
    Map_CopyCellAttributes(58, 36, 1, 1, 45, 43);
  } else {
    Map_CopyCellAttributes(46, 43, 1, 1, 45, 43);
  }
  CrossboneIslePlatformRooms_PositionSecondBlockMapObject();
  FLAG_SET(FLAG_SCENE_TEMP_06);
}

// Release the second block platform when it reaches the ladder cell.
static void Event_UpdateSecondBlockTerrain(void) {
  struct FieldActor *block = Object_GetById(9);
  s32 column = block->x.fixed / PIXELS(16);
  s32 row = block->z.fixed / PIXELS(16);

  if (column == 45 && row == 43) {
    CrossboneIslePlatformRooms_ReleaseSecondBlockPlatform();
  }
}

// Pushing the second block (trigger 52); at the ladder, the platform riding
// on it stays there.
static void Event_PushSecondBlock(void) {
  StagedActor_AdvancePair();
  Event_UpdateSecondBlockTerrain();
}

// Restore the terrain at both ends of the third block row.
static void CrossboneIslePlatformRooms_RestoreThirdBlockTerrain(void) {

  Map_CopyCellAttributes(38, 56, 1, 1, 38, 55);
  Map_CopyCellAttributes(42, 56, 1, 1, 42, 55);
}

// Restore the third block support at the western end.
static void CrossboneIslePlatformRooms_RestoreThirdBlockWestCell(void) {

  Map_CopyCellAttributes(40, 54, 1, 1, 38, 55);
  Object_GetById(10)->motionFlags = 0;
}

// Restore the third block support at the eastern end.
static void CrossboneIslePlatformRooms_RestoreThirdBlockEastCell(void) {

  Map_CopyCellAttributes(40, 54, 1, 1, 42, 55);
  Object_GetById(10)->motionFlags = 0;
}

// Rebuild support for the third block from its height and destination.
static void Event_UpdateThirdBlockTerrain(void) {
  struct FieldActor *block = Object_GetById(10);
  s32 heightCells = block->y.fixed / PIXELS(16);
  s32 column = block->x.fixed / PIXELS(16);
  s32 row = block->z.fixed / PIXELS(16);

  if (!FLAG_CHECK(FLAG_CROSSBONE_ISLE_THIRD_BLOCK_LOWERED) &&
      heightCells <= 2) {
    CrossboneIslePlatformRooms_RestoreThirdBlockWestCell();
    Engine_GameFlagSet(FLAG_CROSSBONE_ISLE_THIRD_BLOCK_LOWERED);
  }
  if (row == 55) {
    if (column == 42) {
      CrossboneIslePlatformRooms_RestoreThirdBlockEastCell();
    }
    if (column == 38) {
      CrossboneIslePlatformRooms_RestoreThirdBlockWestCell();
    }
  } else {
    CrossboneIslePlatformRooms_RestoreThirdBlockTerrain();
  }
}

// Pushing the third block (trigger 50).
static void Event_PushThirdBlock(void) {
  CrossboneIslePlatformRooms_RestoreThirdBlockTerrain();
  StagedActor_AdvancePair();
  Event_UpdateThirdBlockTerrain();
}

// Walking into the third block (trigger 54): facing south, the rising
// sequence plays; otherwise the block is pushed.
static void Event_TouchThirdBlock(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  if (leader->facing == FACING_SOUTH) {
    FieldBattleFx_RunRisingObjectSequence(0, 6, 0);
  } else {
    Event_PushThirdBlock();
  }
}

// Clear the support row before repainting the fourth and fifth blocks.
static void CrossboneIslePlatformRooms_RestoreFourthAndFifthBlockTerrain(void) {

  Map_CopyCellAttributes(48, 55, 1, 1, 49, 55);
  Map_CopyCellAttributes(48, 55, 1, 1, 50, 55);
  Map_CopyCellAttributes(48, 55, 1, 1, 51, 55);
  Map_CopyCellAttributes(48, 55, 1, 1, 52, 55);
}

// Place support cells beneath the fourth and fifth blocks.
static void CrossboneIslePlatformRooms_RepaintFourthAndFifthBlockCells(void) {
  struct FieldActor *block;
  s32 column;

  CrossboneIslePlatformRooms_RestoreFourthAndFifthBlockTerrain();

  block = Object_GetById(11);
  column = block->x.fixed / PIXELS(16);
  Map_CopyCellAttributes(53, 55, 1, 1, column, 55);
  block = Object_GetById(12);
  column = block->x.fixed / PIXELS(16);
  Map_CopyCellAttributes(53, 55, 1, 1, column, 55);
}

// Refresh the shared support row after the fourth block moves.
static void Event_UpdateFourthBlockTerrain(void) {
  CrossboneIslePlatformRooms_RepaintFourthAndFifthBlockCells();
}

// Pushing the fourth block (trigger 53).
static void Event_PushFourthBlock(void) {
  CrossboneIslePlatformRooms_RestoreFourthAndFifthBlockTerrain();
  StagedActor_AdvancePair();
  Event_UpdateFourthBlockTerrain();
}

// Refresh the shared support row after the fifth block moves.
static void Event_UpdateFifthBlockTerrain(void) {
  CrossboneIslePlatformRooms_RepaintFourthAndFifthBlockCells();
}

// Unused callback that moves the fifth block and refreshes its terrain.
UNUSED static void CrossboneIslePlatformRooms_MoveFifthBlock(void) {
  CrossboneIslePlatformRooms_RestoreFourthAndFifthBlockTerrain();
  StagedActor_AdvancePair();
  Event_UpdateFifthBlockTerrain();
}

// Keep actor fourteen just above and behind the second block.
static void CrossboneIslePlatformRooms_FollowSecondBlock(void) {
  struct FieldActor *target = Object_GetById(14);
  struct FieldActor *source = Object_GetById(9);

  target->y.fixed = PIXELS(32);
  target->x.fixed = source->x.fixed;
  target->z.fixed = source->z.fixed + PIXELS(1);
}

// Align map object 107 with the second block.
static void CrossboneIslePlatformRooms_PositionSecondBlockMapObject(void) {
  struct FieldActor *frame = Object_GetById(9);

  Engine_MapObjectSetPosition(107, frame->x.fixed, frame->z.fixed + PIXELS(1));
}

// Choose block drawing priorities from the leader height and row.
static void CrossboneIslePlatformRooms_UpdateBlockPriorities(void) {
  struct FieldActor *leader = Object_GetById(0);
  u8 priority;

  if (leader->y.fixed > PIXELS(16)) {
    priority = ACTOR_PRIORITY_UNDERFOOT;
    Object_GetById(8)->priorityFlags = priority;
    if (Object_GetById(10)->y.fixed == 0)
      Object_GetById(10)->priorityFlags = priority;
    Object_GetById(11)->priorityFlags = priority;
  } else {
    if (Object_GetById(10)->y.fixed == 0 &&
        Object_GetById(0)->z.fixed / PIXELS(16) > 56) {
      Engine_ActorSetSpritePriority(10, 3);
    } else {
      Engine_ActorSetSpritePriority(10, 1);
      Object_GetById(10)->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    }
    priority = 0;
    Object_GetById(11)->priorityFlags = priority;
  }
  Object_GetById(12)->priorityFlags = priority;
}

// Initialize the key, pursuer or movable-block room.
s32 Scene_Initialize(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (gGameState.scene == (s32)&SceneId_CrossboneIslePlatformRooms1) {
    CrossboneIslePlatformRooms_InitializeScene1();
  }
  if (gGameState.scene == (s32)&SceneId_CrossboneIslePlatformRooms2) {
    CrossboneIslePlatformRooms_InitializeScene2();
  }
  if (gGameState.scene == (s32)&SceneId_CrossboneIslePlatformRooms3) {
    CrossboneIslePlatformRooms_InitializeScene3();
  }
  return 0;
}

// Unused callback that cycles key icon displays.
UNUSED static void CrossboneIslePlatformRooms_UpdateItemIcons(void) {
  if (++gCrossboneIslePlatformRooms_IconTimer > 16) {
    gCrossboneIslePlatformRooms_IconTimer = 0;
  }
  switch (gCrossboneIslePlatformRooms_IconTimer) {
  case 12:
    if (Engine_GameFlagIsSet(0xee7) == 0) {
      OBJ_POS_SET(8, 232, 872);
    }
    if (Engine_GameFlagIsSet(0xee8) == 0) {
      OBJ_POS_SET(9, 296, 824);
    }
    if (Engine_GameFlagIsSet(0xee9) == 0) {
      OBJ_POS_SET(10, 328, 760);
    }
    if (Engine_GameFlagIsSet(0xeea) == 0) {
      OBJ_POS_SET(11, 360, 872);
    }
    break;
  case 10:
    Colosso_ShowItemIcon(8, 0);
    break;
  case 8:
    Colosso_ShowItemIcon(9, 0);
    break;
  case 6:
    Colosso_ShowItemIcon(10, 0);
    break;
  case 4:
    Colosso_ShowItemIcon(11, 0);
    break;
  case 2:
    Colosso_ShowItemIcon(12, 1);
    break;
  }
}

// Shared source is included here in ROM order.
#include "common/colosso_item_icon.h"

// Restore collected keys and already opened passages.
static void CrossboneIslePlatformRooms_InitializeScene1(void) {
  Engine_TaskWait(1);
  Colosso_ShowItemIcon(12, ITEM_RED_KEY);
  Colosso_ShowItemIcon(11, ITEM_BLUE_KEY);
  Colosso_ShowItemIcon(10, ITEM_BLUE_KEY);
  Colosso_ShowItemIcon(9, ITEM_BLUE_KEY);
  Colosso_ShowItemIcon(8, ITEM_BLUE_KEY);
  if (FLAG_CHECK(0xee7) == 0) {
    OBJ_POS_SET(8, 232, 872);
  }
  if (FLAG_CHECK(0xee8) == 0) {
    OBJ_POS_SET(9, 296, 824);
  }
  if (FLAG_CHECK(0xee9) == 0) {
    OBJ_POS_SET(10, 328, 760);
  }
  if (FLAG_CHECK(0xeea) == 0) {
    OBJ_POS_SET(11, 360, 872);
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN) != 0) {
    CrossboneIslePlatformRooms_OpenPassage(0);
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN + 1) != 0) {
    CrossboneIslePlatformRooms_OpenPassage(1);
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN + 2) != 0) {
    CrossboneIslePlatformRooms_OpenPassage(2);
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN + 3) != 0) {
    CrossboneIslePlatformRooms_OpenPassage(3);
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN) != 0) {
    CrossboneIslePlatformRooms_OpenPlatformPassage(0);
  }
}

// Configure the pursuers, their chase triggers and blending.
static void CrossboneIslePlatformRooms_InitializeScene2(void) {
  Object_GetById(8)->collisionFlags = 1;
  Object_GetById(9)->collisionFlags = 1;
  Object_GetById(10)->collisionFlags = 1;
  Object_GetById(11)->collisionFlags = 1;
  Object_GetById(8)->scaleX = 7 * Q16_ONE / 10;
  Object_GetById(9)->scaleX = 7 * Q16_ONE / 10;
  Object_GetById(10)->scaleX = 7 * Q16_ONE / 10;
  Object_GetById(11)->scaleX = 7 * Q16_ONE / 10;
  Object_GetById(12)->scaleX = 7 * Q16_ONE / 10;
  FieldScheduler_AddOrUpdateCallback(
      CrossboneIslePlatformRooms_CheckFifthPursuerTrigger, 0xc80);
  FieldScheduler_AddOrUpdateCallback(
      CrossboneIslePlatformRooms_CheckSecondPursuerTrigger, 0xc80);
  FieldScheduler_AddCallback(
      CrossboneIslePlatformRooms_CheckFirstPursuerTrigger, 0xc80);
  Io_Write16(0x3f42, &REG_BLDCNT);
  Io_Write16(0x607, &REG_BLDALPHA);
}

// Restore movable blocks, terrain and the Djinni escape progress.
static void CrossboneIslePlatformRooms_InitializeScene3(void) {
  struct FieldActor *djinni;

  // Reattach the second block follower and rebuild all block support cells.
  Object_GetById(14)->motionFlags = 0;
  FieldScheduler_AddOrUpdateCallback(
      CrossboneIslePlatformRooms_FollowSecondBlock, 0xc80);
  FieldScheduler_AddCallback(CrossboneIslePlatformRooms_UpdateBlockPriorities,
                             0xc80);
  MapObject_SetPosition(107, 0, 0);
  if (FLAG_CHECK(0xed9) != 0) {
    OBJ_MODE(14, 2);
  }
  Event_UpdateFirstBlockTerrain();
  Event_UpdateSecondBlockTerrain();
  Event_UpdateThirdBlockTerrain();
  Event_UpdateFourthBlockTerrain();
  Event_UpdateFifthBlockTerrain();
#if TBS_PATCH_1
  Engine_ActorSetSpritePriority(8, 3);
#endif
  Object_GetById(11)->motionFlags = 0;
  Object_GetById(12)->motionFlags = 0;
  CrossboneIslePlatformRooms_RepaintFourthAndFifthBlockCells();
  if (FLAG_CHECK(FLAG_SCENE_TEMP_00) != 0) {
    Event_FreezePuddle();
    OBJ_MODE(13, 5);
  }

  // Restore the surviving Djinni at the last completed escape stage.
  if (FLAG_CHECK(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE) != 0) {
      OBJ_POS_SET(BANE, 856, 824);
      djinni = Object_GetById(BANE);
#ifdef UBFIX
      djinni->update = CrossboneIslePlatformRooms_FaceBaneTowardLeader;
#else
      djinni->update = (void (*)(
          union FieldObject *))CrossboneIslePlatformRooms_FaceBaneTowardLeader;
#endif
    } else if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE) != 0) {
      OBJ_POS_SET(BANE, 888, 664);
      djinni = Object_GetById(BANE);
      djinni->sprite->rotation = 0;
      ObjectDispatch_SetAnimationSpeed(djinni, 16);
    } else if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE) != 0) {
      OBJ_POS_SET(BANE, 584, 680);
    } else {
      OBJ_POS_SET(BANE, 584, 664);
    }
  }
}

// Pixel step by direction: X in the upper halfword, Z in the lower.
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
    { 1, CONDITION_ALWAYS, 200, 0, 968, FACING_NORTH, 0, 24, 560, 504, 1008, 0 },
    { 2, CONDITION_ALWAYS, 88, 0, 968, FACING_NORTH, 0, 24, 560, 504, 1008, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIslePlatformRoomsEntrances2[] = {
    { 1, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 40, 0, 464, 440, 0 },
    { 2, CONDITION_ALWAYS, 88, 0, 392, FACING_NORTH, 0, 40, 0, 464, 440, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIslePlatformRoomsEntrances3[] = {
    { 1, CONDITION_ALWAYS, 776, 0, 952, FACING_NORTH, 0, 512, 544, 928, 992, 0 },
    { 2, CONDITION_ALWAYS, 664, 0, 952, FACING_NORTH, 0, 512, 544, 928, 992, 0 },
    { SCENE_TABLE_END },
};

u32 gCrossboneIslePlatformRoomsExits[] = {
    SCENE_EXITS(117),
    SCENE_EXIT(1, 128, 3),
    SCENE_EXIT(2, 128, 4),
    SCENE_EXIT(0, 118, 0),
    SCENE_EXIT(1, 129, 3),
    SCENE_EXIT(2, 129, 4),
    SCENE_EXIT(0, 120, 0),
    SCENE_EXIT(1, 131, 3),
    SCENE_EXIT(2, 131, 4),
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
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ShowRedKey) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HideRedKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CatchFirstBlueKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CatchSecondBlueKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CatchThirdBlueKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CatchFourthBlueKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CatchRedKey) },
    { EVENT_TOUCH, 50, CONDITION_ALWAYS, EVENT_SCRIPT(Event_UseRedKey) },
    { EVENT_TOUCH, 51, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenFirstBlueDoor) },
    { EVENT_TOUCH, 52, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenSecondBlueDoor) },
    { EVENT_TOUCH, 53, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenThirdBlueDoor) },
    { EVENT_TOUCH, 54, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenFourthBlueDoor) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 53, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CheckLeaderAhead) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 54, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CheckLeaderAhead) },
    { SEARCH(SEARCH_CHEST), 100, 0xeca, FIND_COINS(333) },
    { SEARCH(SEARCH_CHEST), 101, 0xecb, FIND_ITEM(0x10b) },
    { SEARCH(SEARCH_CHEST), 102, 0xecc, FIND_ITEM(0xc0) },
    { SEARCH(SEARCH_CHEST), 103, 0xecd, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gCrossboneIslePlatformRoomsEvents2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TOUCH, 53, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TrackThirdPursuerColumn) },
    { EVENT_TOUCH, 54, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TrackFourthPursuerColumn) },
    { EVENT_TOUCH, 61, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DodgeThirdPursuer) },
    { EVENT_TOUCH, 62, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DodgeFourthPursuer) },
    { EVENT_RAISED, 96, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DodgeFifthPursuer) },
    { EVENT_RAISED, 92, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DodgeSecondPursuer) },
    { EVENT_RAISED, 91, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DodgeFirstPursuer) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_BeginRevealBlend) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EndRevealBlend) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HaltPursuer) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HaltPursuer) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HaltPursuer) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HaltPursuer) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HaltPursuer) },
    { SEARCH(SEARCH_CHEST), 100, 0xece, FIND_COINS(444) },
    { SEARCH(SEARCH_CHEST), 101, 0xecf, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SEARCH(SEARCH_CHEST), 102, 0xed0, FIND_ITEM(0x60) },
    { SEARCH(SEARCH_CHEST), 103, 0xed1, FIND_ITEM(0xba) },
    { SCENE_EVENTS_END },
};
// clang-format on

struct SceneEvent gCrossboneIslePlatformRoomsEvents3[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 50, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_PushThirdBlock)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 51, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_PushFirstBlock)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 52, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_PushSecondBlock)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 53, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_PushFourthBlock)},
#if TBS_PATCH_1
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 61, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_PushFirstBlock)},
#endif
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 54, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_TouchThirdBlock)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 60, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_UseLadder)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 70, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_LookAheadOrPushFirstBlock)},
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
    {0x7, BANE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_BaneJoins)},
    {PSYNERGY_ON_ACTOR(ABILITY_HALT), BANE, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_HaltBane)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 13, FLAG_SCENE_TEMP_00,
     EVENT_SCRIPT(Event_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, FLAG_SCENE_TEMP_01,
     EVENT_SCRIPT(Event_UpdateFirstBlockTerrain)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, FLAG_SCENE_TEMP_06,
     EVENT_SCRIPT(Event_UpdateSecondBlockTerrain)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, FLAG_SCENE_TEMP_03,
     EVENT_SCRIPT(Event_UpdateThirdBlockTerrain)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, FLAG_SCENE_TEMP_04,
     EVENT_SCRIPT(Event_UpdateFourthBlockTerrain)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, FLAG_SCENE_TEMP_05,
     EVENT_SCRIPT(Event_UpdateFifthBlockTerrain)},
    {SEARCH(SEARCH_CHEST), 104, 0xed6, FIND_COINS(666)},
    {SEARCH(SEARCH_CHEST), 105, 0xed7, FIND_ITEM(ITEM_WATER_OF_LIFE)},
    {SEARCH(SEARCH_CHEST), 106, 0xed8, FIND_ITEM(ITEM_ELIXIR)},
    {SEARCH(SEARCH_CHEST), 107, 0xed9, FIND_ITEM(0x26)},
    {SCENE_EVENTS_END},
};

u32 gCrossboneIslePlatformRooms_SlotColumns[] = {
    16,
    21,
    0,
};

s32 gCrossboneIslePlatformRooms_IconTimer = 0;
