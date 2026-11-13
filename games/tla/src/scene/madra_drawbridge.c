/// @file
/// The Madra Drawbridge, east of Madra, where two guards keep the way into
/// Osenia shut until Madra's elder gives the party permission.
///
/// The first time the party steps onto the bridge (MadraDrawbridge_BlockBridge)
/// the guards close ranks across it and send the party back for the elder's
/// permission. Once flag 0x913 is set they greet the party instead
/// (MadraDrawbridge_GreetPermittedParty) and step aside.

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"
#include "actor.h"
#include "event_work.h"
#include "module/scene/madra_drawbridge.h"

/// Set once the guards have closed ranks on this visit.
#define FLAG_GUARDS_BLOCKED_BRIDGE 0x200
/// Set once the guards have told the party to fetch the elder's permission.
#define FLAG_GUARDS_ASKED_FOR_PERMISSION 0x301

/// The two guards, numbering the rows of gActorPlacements.
enum MadraDrawbridgeActorId {
  /// The guard on the east side of the bridge.
  /// ★ Named from the game: "If you wanna cross here, you'll need permission
  /// from the town elder!" (MsgMadraNeedEldersPermission)
  ACTOR_EAST_GUARD = ACTOR_FIRST_PLACED,
  /// The guard on the west side of the bridge.
  /// ★ Named from the game: "But try to cross illegally, and we'll throw you
  /// in the hoosegow with Mr. Bluehair!" (MsgMadraCanCrossWithPermission)
  ACTOR_WEST_GUARD
};

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

/// The east guard asks whether the party is bound for Osenia, and without the
/// elder's permission sends it back to town for a permit.
/// "You goin' to Osenia?"
void MadraDrawbridge_AskGoingToOsenia(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgMadraGoingToOsenia);
  FieldMessage_ShowAtActor(ACTOR_EAST_GUARD, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Motion_SetModeAndWaitAnimation(ACTOR_EAST_GUARD, 3);
    Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 2);
  } else {
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 2);
  }
  Event_End();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

/// The guards close ranks across the bridge as the party steps onto it, once a
/// visit, and the first time they ask for the elder's permission.
void MadraDrawbridge_BlockBridge(void) {
  if (Event_GetFlag(FLAG_GUARDS_BLOCKED_BRIDGE) == 0) {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (Event_GetFlag(FLAG_GUARDS_ASKED_FOR_PERMISSION) == 0) {
      Camera_SetSpeedParameters(0x18000, 0x3000);
      Camera_MoveWithinMapBounds(PIXELS(164), -1, PIXELS(260), 1);
      Actor_WalkTo(gMainRoutineWork.currentOwner, 158, 316);
    }
    Actor_GetById(ACTOR_EAST_GUARD)->actionFlags &= ~1;
    Actor_GetById(ACTOR_WEST_GUARD)->actionFlags &= ~1;
    Actor_ResetAndSetPositionInMode2(ACTOR_WEST_GUARD, 148, 252);
    Actor_SetPositionAndReset(ACTOR_EAST_GUARD, 170, 252);
    Actor_WaitForMoveById(ACTOR_WEST_GUARD);
    Actor_GetById(ACTOR_EAST_GUARD)->actionFlags |= 1;
    Actor_GetById(ACTOR_WEST_GUARD)->actionFlags |= 1;
    Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH, 0);
    Actor_ArmCallback(ACTOR_WEST_GUARD, FACING_SOUTH, 0);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    GameFlag_SetBit(FLAG_GUARDS_BLOCKED_BRIDGE);
    Func_02000338();
    MapGrid_CopyCellAttributes(11, 9, 4, 1, 8, 14);
    if (Event_GetFlag(FLAG_GUARDS_ASKED_FOR_PERMISSION) == 0) {
      Event_SetMessageId((s32)MsgMadraNeedEldersPermission);
      Event_Wait(10);
      Motion_SetVarCbAndRefresh(ACTOR_EAST_GUARD, 1);
      Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 5);
      Motion_SetModeAndWaitAnimation(ACTOR_WEST_GUARD, 3);
      Event_ShowMessageAndWait(ACTOR_WEST_GUARD, 0);
      GameFlag_SetBit(FLAG_GUARDS_ASKED_FOR_PERMISSION);
      Event_End();
    }
  }
}

/// With the elder's permission, the guards let the party through.
/// "Yeah, we heard about you already."
void MadraDrawbridge_GreetPermittedParty(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgMadraHeardAboutYou);
  Camera_MoveWithinMapBounds(PIXELS(164), -1, PIXELS(260), 1);
  Func_02000338();
  Motion_SetModeAndWaitAnimation(ACTOR_EAST_GUARD, 3);
  Event_ShowMessageAndWait(ACTOR_EAST_GUARD, 0);
  Motion_SetModeAndWaitAnimation(ACTOR_WEST_GUARD, 3);
  Event_ShowMessageAndWait(ACTOR_WEST_GUARD, 0);
  GameFlag_SetBit(0x916);
  Event_End();
}

s32 Scene_Initialize(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 152, 0, 176, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(53),
    SCENE_EXIT_IF(1, 2, 10, -1),
    SCENE_EXIT_IF(2, 2, 11, -1),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {73, CONDITION_ALWAYS, 1, PIXELS(188), 0, PIXELS(260), FACING_SOUTH, 1, 0},
    {88, CONDITION_ALWAYS, 1, PIXELS(132), 0, PIXELS(260), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 10, 0x913, EVENT(MadraDrawbridge_BlockBridge)},
    {TRIGGER_STEP, 10, 0x916, EVENT(MadraDrawbridge_GreetPermittedParty)},
    {TRIGGER_TALK, ACTOR_EAST_GUARD, 0x913, EVENT(MadraDrawbridge_AskGoingToOsenia)},
    {TRIGGER_TALK, ACTOR_EAST_GUARD, CONDITION_ALWAYS, MSGNO(MsgMadraMikasallaVillage)},
    {TRIGGER_TALK, ACTOR_WEST_GUARD, 0x913, MSGNO(MsgMadraCanCrossWithPermission)},
    {TRIGGER_TALK, ACTOR_WEST_GUARD, CONDITION_ALWAYS, MSGNO(MsgMadraAvoidYampiDesert)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD, 0x913, MSGNO(MsgMadraForTheirSafety)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD, CONDITION_ALWAYS, MSGNO(MsgMadraWouldNeedBoat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_GUARD, 0x913, MSGNO(MsgMadraElderReturn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_GUARD, CONDITION_ALWAYS, MSGNO(MsgMadraRatherStayHome)},
    {TRIGGERS_END},
};
