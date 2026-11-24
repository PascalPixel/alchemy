/// @file
/// The Madra Drawbridge, east of Madra, where two guards keep the way into
/// Osenia shut until Madra's elder gives the party permission.
///
/// The first time the party steps onto the bridge (MadraDrawbridge_BlockBridge)
/// the guards close ranks across it and send the party back for the elder's
/// permission. Once FLAG_OSENIA_PASSAGE_GRANTED is set they greet the party
/// instead (MadraDrawbridge_LetPermittedPartyThrough) and step aside.

#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "system/ram_buffer.h"
#include "system/main_routine_state.h"
#include "field/event_inline.h"
#include "field/actor.h"
#include "field/field_sequence_event.h"
#include "text/msg_ids.h"
#include "field/sprite.h"

/// The guards have let the permitted party over the bridge.
#define FLAG_DRAWBRIDGE_GUARDS_LET_PARTY_THROUGH 0x916

void FieldCamera_WaitForArrival(void);

/// Set once the guards have closed ranks on this visit.
#define FLAG_GUARDS_BLOCKED_BRIDGE FLAG_SCENE_TEMP_00
/// Set once the guards have told the party to fetch the elder's permission.
#define FLAG_GUARDS_ASKED_FOR_PERMISSION FLAG_AREA_TEMP_01

/// The cells at the foot of the bridge, whose step trigger brings the guards
/// across it.
enum { CELL_BEHAVIOR_MADRA_DRAWBRIDGE_FOOT = 10 };

/// The two guards, numbering the rows of gActorPlacements.
enum MadraDrawbridgeActorId {
  /// The guard on the east side of the bridge.
  ACTOR_EAST_GUARD = ACTOR_FIRST_PLACED,
  /// The guard on the west side of the bridge.
  ACTOR_WEST_GUARD
};

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// The east guard asks whether the party is bound for Osenia, and without the
/// elder's permission sends it back to town for a permit.
void MadraDrawbridge_AskGoingToOsenia(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgMadraGoingToOsenia);
  // "You goin' to Osenia?" Yes: "You'll have to head back to town and get..."
  // No: "Then get lost, pal. These parts can be dangerous..."
  Event_OpenMessage(ACTOR_EAST_GUARD, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0) ==
      0) {
    Actor_SetAnimationAndWait(ACTOR_EAST_GUARD, ANIM_NOD);
    // "You'll have to head back to town and get a permit! Just go to the..."
    Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 2);
  } else {
    (gFieldState->message)++;
    // "Then get lost, pal. These parts can be dangerous..."
    Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 2);
  }
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// The guards close ranks across the bridge as the party steps onto it, once a
/// visit, and the first time they ask for the elder's permission.
void MadraDrawbridge_BlockBridge(void) {
  if (Event_GetFlag(FLAG_GUARDS_BLOCKED_BRIDGE) == 0) {
    Event_Start();
    Event_EndLastingPsynergy(0);
    if (Event_GetFlag(FLAG_GUARDS_ASKED_FOR_PERMISSION) == 0) {
      FieldCamera_SetSpeed(1.5);
      FieldCamera_MoveTo(164, CAMERA_KEEP, 260, 1);
      Actor_WalkTo(gMainRoutineState.leaderActorId, 158, 316);
    }
    Actor_GetById(ACTOR_EAST_GUARD)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_GetById(ACTOR_WEST_GUARD)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkTo(ACTOR_WEST_GUARD, 148, 252);
    Actor_WalkToAndWait(ACTOR_EAST_GUARD, 170, 252);
    Actor_WaitForMoveById(ACTOR_WEST_GUARD);
    Actor_GetById(ACTOR_EAST_GUARD)->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_GetById(ACTOR_WEST_GUARD)->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTH, 0);
    Actor_WaitForMoveById(gMainRoutineState.leaderActorId);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
    Event_SetFlag(FLAG_GUARDS_BLOCKED_BRIDGE);
    FieldCamera_WaitForArrival();
    // The guards now stand across the bridge: no one walks past them.
    MapGrid_CopyCellAttributes(11, 9, 4, 1, 8, 14);
    if (Event_GetFlag(FLAG_GUARDS_ASKED_FOR_PERMISSION) == 0) {
      Event_SetMessageId((s32)MsgMadraNeedEldersPermission);
      Event_Wait(10);
      Actor_RunRepeatedMotion(ACTOR_EAST_GUARD, 1);
      // "If you wanna cross here, you'll need permission from the town elder!"
      Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 5);
      Actor_SetAnimationAndWait(ACTOR_WEST_GUARD, ANIM_NOD);
      // "Without permission, ain't no one's gonna cross this bridge, no..."
      Event_ShowMessage(ACTOR_WEST_GUARD, 0);
      Event_SetFlag(FLAG_GUARDS_ASKED_FOR_PERMISSION);
      Event_End();
    }
  }
}

/// With the elder's permission, the guards greet the party and let it through.
void MadraDrawbridge_LetPermittedPartyThrough(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgMadraHeardAboutYou);
  FieldCamera_MoveTo(164, CAMERA_KEEP, 260, 1);
  FieldCamera_WaitForArrival();
  Actor_SetAnimationAndWait(ACTOR_EAST_GUARD, ANIM_NOD);
  // "Yeah, we heard about you already. If you're going to Osenia, you'd..."
  Event_ShowMessage(ACTOR_EAST_GUARD, 0);
  Actor_SetAnimationAndWait(ACTOR_WEST_GUARD, ANIM_NOD);
  // "Alhafra is up along the northeast of Osenia... You'll see it eventually."
  Event_ShowMessage(ACTOR_WEST_GUARD, 0);
  Event_SetFlag(FLAG_DRAWBRIDGE_GUARDS_LET_PARTY_THROUGH);
  Event_End();
}

s32 Scene_Initialize(void) {
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 152, 0, 176, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(MadraDrawbridge),
    SCENE_EXIT_UNLESS(1, WorldMap, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, WorldMap, 11, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 188, 0, 260, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAN_SOLDIER, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 132, 0, 260, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, CELL_BEHAVIOR_MADRA_DRAWBRIDGE_FOOT,
     FLAG_OSENIA_PASSAGE_GRANTED, EVENT(MadraDrawbridge_BlockBridge)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MADRA_DRAWBRIDGE_FOOT,
     FLAG_DRAWBRIDGE_GUARDS_LET_PARTY_THROUGH,
     EVENT(MadraDrawbridge_LetPermittedPartyThrough)},
    {TRIGGER_TALK, ACTOR_EAST_GUARD, FLAG_OSENIA_PASSAGE_GRANTED,
     EVENT(MadraDrawbridge_AskGoingToOsenia)},
    {TRIGGER_TALK, ACTOR_EAST_GUARD, CONDITION_ALWAYS,
     MSGNO(MsgMadraMikasallaVillage)},
    {TRIGGER_TALK, ACTOR_WEST_GUARD, FLAG_OSENIA_PASSAGE_GRANTED,
     MSGNO(MsgMadraCanCrossWithPermission)},
    {TRIGGER_TALK, ACTOR_WEST_GUARD, CONDITION_ALWAYS,
     MSGNO(MsgMadraAvoidYampiDesert)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD,
     FLAG_OSENIA_PASSAGE_GRANTED, MSGNO(MsgMadraForTheirSafety)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD, CONDITION_ALWAYS,
     MSGNO(MsgMadraWouldNeedBoat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_GUARD,
     FLAG_OSENIA_PASSAGE_GRANTED, MSGNO(MsgMadraElderReturn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_GUARD, CONDITION_ALWAYS,
     MSGNO(MsgMadraRatherStayHome)},
    {TRIGGER_TABLE_END},
};
