/// @file
/// The Suhalla Gate, in the prologue: Jenna and Kraden cross it on their way
/// to Idejima, while Alex holds back the soldiers behind them. Its three rooms
/// share this overlay:
///
/// - The road past the gate (SceneId_SuhallaGateRoad), coming from the Venus
///   Lighthouse in the east, where a Ruffian spots the two and fights them
///   (SuhallaGate_RuffianSpotsParty), and a door in the rock at its west end
///   leads on to the cliffs.
/// - The cliffs (SceneId_SuhallaGateAmbush), whose two vines lead down to the
///   crevice where three Ruffians wait in ambush
///   (SuhallaGate_RuffiansAmbushParty).
/// - The cave beyond the crevice (SceneId_SuhallaGatePsynergyStone), where a
///   Punch Ant jumps out at the two on the way to the Psynergy Stone
///   (SuhallaGate_PunchAntJumpsOut), and whose far end leads to Idejima.
///
/// Every battle sends the party back to ENTRANCE_AFTER_SUHALLA_GATE_BATTLE in
/// its room, and a lost one to the Venus Lighthouse's entrance
/// (VENUS_LIGHTHOUSE_ENTRANCE_AFTER_DEFEAT). Back from each, its flag set, the
/// beaten Ruffians lie where they fell.
#include "djinn_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "scene_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "field/actor.h"
#include "field/actor_script.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_sequence_event.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "menu/menu.h"
#include "scene/scene.h"
#include "sound/sound.h"
#include "system/main_routine_state.h"
#include "system/math_table.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "text/msg_ids.h"

/// Set as the party comes back from beating the Ruffian on the road; he then
/// lies beaten by it.
#define FLAG_SUHALLA_GATE_RUFFIAN_BEATEN 0x894
/// Set as the party comes back from beating the three Ruffians at the crevice;
/// they then lie beaten before it.
#define FLAG_SUHALLA_GATE_AMBUSH_BEATEN 0x895
/// Set once Kraden has talked over the Punch Ant's battle; the cave's Punch Ant
/// then stays away.
#define FLAG_SUHALLA_GATE_PUNCH_ANT_BEATEN 0x896
/// Set once the chest on the cliffs, at cell (10, 22), has given its Mint.
#define FLAG_SUHALLA_GATE_MINT 0xfb4

/// The entrance each room's battle sends the party back to, 99 in each of the
/// three.
#define ENTRANCE_AFTER_SUHALLA_GATE_BATTLE 99

/// The road's one placed actor.
enum SuhallaGateRoadActorId {
  /// The Ruffian who spots Jenna and Kraden on the road.
  ACTOR_ROAD_RUFFIAN = ACTOR_FIRST_PLACED
};

/// The cliffs' placed actors.
enum SuhallaGateCliffsActorId {
  /// Five sprouts (SPRITE_SPROUT, the sprout Growth grows), with no collision,
  /// each drawn half or fully grown as the party enters.
  ACTOR_FIRST_SPROUT = ACTOR_FIRST_PLACED,
  /// The three Ruffians of the ambush, west to east as they line up.
  ACTOR_WEST_RUFFIAN = ACTOR_FIRST_PLACED + 5,
  ACTOR_MIDDLE_RUFFIAN,
  ACTOR_EAST_RUFFIAN,
  /// Kraden, who jumps out from behind Jenna to point the way.
  ACTOR_CLIFFS_KRADEN
};

/// The cave's placed actors.
enum SuhallaGateCaveActorId {
  ACTOR_PUNCH_ANT = ACTOR_FIRST_PLACED,
  ACTOR_CAVE_KRADEN
};

/// SPRITE_SPROUT's animations as the sprouts here stand, half grown or fully
/// grown.
enum { SPROUT_ANIMATION_HALF_GROWN = 3, SPROUT_ANIMATION_GROWN = 4 };

/// The cells that run the rooms' events.
enum SuhallaGateCell {
  /// The door in the rock at the road's west end, cell (8, 18), holding the
  /// number of the exit it leads by.
  ROAD_DOOR_CELL = 3,
  /// The road's cells (29, 21), (30, 21) and (30, 22), where the Ruffian
  /// spots the party.
  RUFFIAN_SPOTS_PARTY_CELL = 20,
  /// Unused: no cell of the road's map holds it.
  ROAD_UNUSED_CELL = 12,
  /// On the cliffs, cells (31, 54), (32, 54) and (31, 55), away from the
  /// crevice, where Kraden calls Jenna back.
  KRADEN_POINTS_THE_WAY_CELL = 21,
  /// The cliffs' chest, cell (10, 22).
  CLIFFS_CHEST_CELL = 100,
  /// On the way to the Psynergy Stone, cells (31, 26) to (33, 29), where the
  /// Punch Ant jumps out.
  PUNCH_ANT_CELL = 22,
  /// Unused: no cell of the cave's map holds it.
  CAVE_UNUSED_CELL = 23
};

void SuhallaGate_AfterPunchAntBattle(void);
void Actor_FallOverById(s32 id, u32 angle);
void Actor_FallOverWithAnimationById(s32 id, u32 angle, s32 animation);
void SuhallaGate_FightPunchAnt(void);

extern struct SceneEntrance gSuhallaGate_RoadEntrances[];
extern struct SceneEntrance gSuhallaGate_CliffsEntrances[];
extern struct SceneEntrance gSuhallaGate_CaveEntrances[];
extern struct ActorPlacement gSuhallaGate_CavePlacements[];
extern struct ActorPlacement gSuhallaGate_CliffsPlacements[];
extern struct ActorPlacement gSuhallaGate_RoadPlacements[];
extern struct Trigger gSuhallaGate_RoadTriggers[];
extern struct Trigger gSuhallaGate_CliffsTriggers[];
extern struct Trigger gSuhallaGate_CaveTriggers[];

/// Unused: talking to the west Ruffian at the crevice would start a battle with
/// the Djinni Dew, but his own line, "Maybe I should have eaten a bigger
/// breakfast.", always answers first.
void SuhallaGate_FightDew(void) {
  Field_RequestDjinnBattle(ACTOR_WEST_RUFFIAN, DJINNI_DEW);
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_SUHALLA_GATE_AMBUSH)
    return gSuhallaGate_CliffsEntrances;
  if (scene == SCENE_SUHALLA_GATE_PSYNERGY_STONE)
    return gSuhallaGate_CaveEntrances;
  return gSuhallaGate_RoadEntrances;
}

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_SUHALLA_GATE_AMBUSH)
    return gSuhallaGate_CliffsPlacements;
  if (scene == SCENE_SUHALLA_GATE_ROAD)
    return gSuhallaGate_RoadPlacements;
  return gSuhallaGate_CavePlacements;
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_SUHALLA_GATE_AMBUSH)
    return gSuhallaGate_CliffsTriggers;
  if (scene == SCENE_SUHALLA_GATE_PSYNERGY_STONE)
    return gSuhallaGate_CaveTriggers;
  return gSuhallaGate_RoadTriggers;
}

/// Back from his battle, the road's Ruffian lies beaten by it.
void SuhallaGate_SetUpRoad(void) {
  if (gMainRoutineState.entrance == ENTRANCE_AFTER_SUHALLA_GATE_BATTLE)
    Event_SetFlag(FLAG_SUHALLA_GATE_RUFFIAN_BEATEN);
  if (Event_GetFlag(FLAG_SUHALLA_GATE_RUFFIAN_BEATEN)) {
    Actor_SetPosition(ACTOR_ROAD_RUFFIAN, 456, 312);
    Actor_FallOverWithAnimationById(ACTOR_ROAD_RUFFIAN, FACING_SOUTH,
                                    ANIM_STILL);
    Actor_SetMovementType(Actor_GetById(ACTOR_ROAD_RUFFIAN),
                          MOVEMENT_TYPE_SQUASH);
  }
}

/// Back from the ambush, its three Ruffians lie beaten before the crevice.
void SuhallaGate_SetUpCliffs(void) {
  MapGrid_CopyCellAttributes(108, 38, 1, 1, 102, 56);
  if (gMainRoutineState.entrance == ENTRANCE_AFTER_SUHALLA_GATE_BATTLE)
    Event_SetFlag(FLAG_SUHALLA_GATE_AMBUSH_BEATEN);
  if (Event_GetFlag(FLAG_SUHALLA_GATE_AMBUSH_BEATEN)) {
    Actor_SetPosition(ACTOR_WEST_RUFFIAN, 590, 920);
    Actor_SetPosition(ACTOR_MIDDLE_RUFFIAN, 624, 939);
    Actor_SetPosition(ACTOR_EAST_RUFFIAN, 632, 920);
    Actor_GetById(ACTOR_WEST_RUFFIAN)->facing = FACING_SOUTH;
    Actor_FallOverById(ACTOR_WEST_RUFFIAN, FACING_SOUTHWEST);
    Actor_GetById(ACTOR_MIDDLE_RUFFIAN)->facing = FACING_EAST;
    Actor_FallOverById(ACTOR_MIDDLE_RUFFIAN, FACING_EAST);
    Actor_GetById(ACTOR_EAST_RUFFIAN)->facing = FACING_NORTH;
    Actor_FallOverById(ACTOR_EAST_RUFFIAN, FACING_EAST);
    Actor_GetById(ACTOR_WEST_RUFFIAN)->collisionFlags = ACTOR_SOLID;
    Actor_GetById(ACTOR_MIDDLE_RUFFIAN)->collisionFlags = ACTOR_SOLID;
    Actor_GetById(ACTOR_EAST_RUFFIAN)->collisionFlags = ACTOR_SOLID;
  }
  Actor_SetAnimation(ACTOR_FIRST_SPROUT, SPROUT_ANIMATION_GROWN);
  Actor_SetAnimation(ACTOR_FIRST_SPROUT + 1, SPROUT_ANIMATION_GROWN);
  Actor_SetAnimation(ACTOR_FIRST_SPROUT + 2, SPROUT_ANIMATION_HALF_GROWN);
  Actor_SetAnimation(ACTOR_FIRST_SPROUT + 3, SPROUT_ANIMATION_GROWN);
  Actor_SetAnimation(ACTOR_FIRST_SPROUT + 4, SPROUT_ANIMATION_HALF_GROWN);
}

/// Back from the Punch Ant's battle, Kraden talks it over.
void SuhallaGate_SetUpCave(void) {
  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0 &&
      gMainRoutineState.entrance == ENTRANCE_AFTER_SUHALLA_GATE_BATTLE)
    SuhallaGate_AfterPunchAntBattle();
}

/// Suhalla Gate's setup for each of its rooms: the road by the house, the
/// cliffs of the ambush, and the cave of the Psynergy Stone.
s32 Scene_Initialize(void) {
  struct FieldState *state;
  s16 scene;

  state = Ram_HeapSlots->fieldState;
  state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  scene = gMainRoutineState.next;
  if (scene == SCENE_SUHALLA_GATE_ROAD)
    SuhallaGate_SetUpRoad();
  else if (scene == SCENE_SUHALLA_GATE_AMBUSH)
    SuhallaGate_SetUpCliffs();
  else if (scene == SCENE_SUHALLA_GATE_PSYNERGY_STONE)
    SuhallaGate_SetUpCave();
  return 0;
}

/// Unused: copies the cliffs' cell as their set-up does, and sets
/// FLAG_SHIP_CAN_SAIL, which the Lemurian Ship's deck sets once the party can
/// sail it; no trigger runs it.
UNUSED void SuhallaGate_OpenCliffsCell(void) {
  MapGrid_CopyCellAttributes(108, 38, 1, 1, 102, 56);
  Event_SetFlag(FLAG_SHIP_CAN_SAIL);
}

/// Walking into the door in the rock at the road's west end, cell (8, 18), the
/// door opens and the screen fades out with the leader still
/// standing before it; the party comes out on the cliffs. The code walks
/// `ACTOR_ISAAC` in through the doorway, as The Broken Seal's doors walk their
/// leader, but here Jenna leads and Isaac is not on stage, so nobody is seen to
/// go in.
void SuhallaGate_EnterRockDoor(void) {
  s32 exit = gFieldState->stepTrigger;

  Actor_GetById(ACTOR_ISAAC)->motionFlags = 0;
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_CopyCellsTo(66, 36, 71, 8, 2, 2);
  wait(4);
  MapGrid_CopyCellsTo(68, 36, 71, 8, 2, 2);
  wait(4);
  Actor_CenterAndWalk(ACTOR_ISAAC, 3, -16);
  Event_PlaySound(SE_MAP_EXIT);
  Import_EventSetExitRequest(exit);
}

/// Walking out of the cave's far end, cell (41, 39), Jenna and Kraden leave for
/// Idejima, and Felix becomes the leader again.
void SuhallaGate_LeaveForIdejima(void) {
  s32 exit = gFieldState->stepTrigger;

  gMainRoutineState.leaderActorId = ACTOR_FELIX;
  Event_PlaySound(SE_MAP_EXIT);
  Import_EventSetExitRequest(exit);
}

s32 Scene_PrepareMap(void) { return 0; }

/// Walking west along the road, cells (29, 21) to (30, 22), Jenna and Kraden
/// are spotted by a Ruffian: "Waahhh! It's them!" He looks for Alex, decides he
/// can handle "some girl and her grandpa", and fights them.
void SuhallaGate_RuffianSpotsParty(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgWaahhhItS);
  Actor_WalkTo(gMainRoutineState.leaderActorId, 472, 320);
  FieldCamera_MoveTo(456, CAMERA_KEEP, 280, 1);
  FieldCamera_WaitForArrival();
  Actor_WaitForMoveById(gMainRoutineState.leaderActorId);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTHWEST, 0);
  Event_Wait(10);
  Actor_Jump(ACTOR_ROAD_RUFFIAN, 6, 20);
  Event_Wait(20);
  // "Waahhh! It's them!"
  Event_ShowMessageThenWait(ACTOR_ROAD_RUFFIAN, 0, 10);
  {
    s32 emote = EMOTE_IN_FRONT | EMOTE_EXCLAMATION;
    s32 frames = 50;

    FieldActor_ShowEmote(ACTOR_ROAD_RUFFIAN, emote, frames);
  }
  // "Whoa there... Where'd the blue-haired guy go?"
  Event_ShowMessageThenWait(ACTOR_ROAD_RUFFIAN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_ROAD_RUFFIAN, ANIM_NOD);
  Event_Wait(30);
  // "Who cares! I can handle some girl and her grandpa!"
  Event_ShowMessageThenWait(ACTOR_ROAD_RUFFIAN, 0, 10);
  Actor_WalkToAndWait(ACTOR_ROAD_RUFFIAN, 456, 312);
  Event_SetAfterBattleDestination(SCENE_SUHALLA_GATE_ROAD,
                                  ENTRANCE_AFTER_SUHALLA_GATE_BATTLE);
  Event_SetDefeatDestination(SCENE_VENUS_LIGHTHOUSE_ENTRANCE,
                             VENUS_LIGHTHOUSE_ENTRANCE_AFTER_DEFEAT);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_EARLY_STORY, 1);
  Event_End();
}

/// Trying to enter the crevice below the cliffs' vines, cell (38, 57), Jenna
/// steps up to it and backs away as three Ruffians step out of it one by one:
/// "Ha! That was a good idea, waiting here for 'em!" Then they fight her and
/// Kraden.
void SuhallaGate_RuffiansAmbushParty(void) {
  struct MainRoutineState *state;
  s32 *owner;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgHaThatWas);
  state = &gMainRoutineState;
  owner = &state->leaderActorId;
  Actor_WalkByAndWait(*owner, 0, -4);
  FieldEvent_Wait(30);
  Actor_GetById(*owner)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(*owner, 0, 12);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(*owner, 0, 16);
  FieldEvent_Wait(30);
  Actor_SetPosition(ACTOR_WEST_RUFFIAN, 616, 912);
  Actor_WalkToAndWait(ACTOR_WEST_RUFFIAN, 600, 920);
  Actor_SetPosition(ACTOR_EAST_RUFFIAN, 616, 912);
  Actor_FaceDirection(ACTOR_WEST_RUFFIAN, FACING_SOUTH, 0);
  Actor_WalkToAndWait(ACTOR_EAST_RUFFIAN, 632, 920);
  Actor_FaceDirection(ACTOR_EAST_RUFFIAN, FACING_SOUTH, 0);
  Actor_SetPosition(ACTOR_MIDDLE_RUFFIAN, 616, 912);
  Actor_WalkToAndWait(ACTOR_MIDDLE_RUFFIAN, 616, 920);
  Actor_RunRepeatedMotion(ACTOR_WEST_RUFFIAN, 2);
  FieldEvent_Wait(20);
  // "Ha! That was a good idea, waiting here for 'em!"
  FieldEvent_ShowMessageThenWait(ACTOR_WEST_RUFFIAN, 0, 10);
  // "You two aren't going anywhere!"
  FieldEvent_ShowMessageThenWait(ACTOR_MIDDLE_RUFFIAN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_EAST_RUFFIAN, ANIM_NOD);
  FieldEvent_Wait(30);
  // "Except with us, back to Master Iodem!"
  FieldEvent_ShowMessageThenWait(ACTOR_EAST_RUFFIAN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_WEST_RUFFIAN, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIDDLE_RUFFIAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_EAST_RUFFIAN, ANIM_NOD);
  FieldEvent_SetAfterBattleDestination(SCENE_SUHALLA_GATE_AMBUSH,
                                       ENTRANCE_AFTER_SUHALLA_GATE_BATTLE);
  Event_SetDefeatDestination(SCENE_VENUS_LIGHTHOUSE_ENTRANCE,
                             VENUS_LIGHTHOUSE_ENTRANCE_AFTER_DEFEAT);
  state->battleType = BATTLE_TYPE_NO_ESCAPE;
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_EARLY_STORY, 2);
  Event_End();
}

/// The Punch Ant leaps at Jenna and its battle begins.
void SuhallaGate_FightPunchAnt(void) {
  struct Actor *owner;

  Actor_Jump(ACTOR_PUNCH_ANT, 4, 0);
  owner = Actor_GetById(gMainRoutineState.leaderActorId);
  if (owner != NULL)
    Actor_SetDestination(ACTOR_PUNCH_ANT, owner->x.part[0].integer,
                         owner->z.part[0].integer);
  Event_Wait(20);
  Event_SetAfterBattleDestination(SCENE_SUHALLA_GATE_PSYNERGY_STONE,
                                  ENTRANCE_AFTER_SUHALLA_GATE_BATTLE);
  Event_SetDefeatDestination(SCENE_VENUS_LIGHTHOUSE_ENTRANCE,
                             VENUS_LIGHTHOUSE_ENTRANCE_AFTER_DEFEAT);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_EARLY_STORY, 3);
  Event_End();
}

/// On the way to the Psynergy Stone, cells (31, 26) to (33, 29), a Punch Ant
/// jumps out from the south-west; Jenna turns to it with a start, and it hops
/// up to her and attacks.
void SuhallaGate_PunchAntJumpsOut(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_SetPosition(ACTOR_PUNCH_ANT, 472, 480);
  Event_Wait(16);
  Actor_FaceActor(gMainRoutineState.leaderActorId, ACTOR_PUNCH_ANT, 0);
  FieldActor_ShowEmote(gMainRoutineState.leaderActorId,
                       EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldCamera_FollowActor(ACTOR_PUNCH_ANT, 1);
  FieldCamera_WaitForArrival();
  Actor_Jump(ACTOR_PUNCH_ANT, 4, 0);
  Event_Wait(16);
  Actor_Jump(ACTOR_PUNCH_ANT, 4, 0);
  Actor_MoveToAndWait(ACTOR_PUNCH_ANT, 480, 472);
  Event_Wait(30);
  Actor_Jump(ACTOR_PUNCH_ANT, 2, 0);
  Actor_MoveToAndWait(ACTOR_PUNCH_ANT, 496, 464);
  Actor_Jump(ACTOR_PUNCH_ANT, 4, 0);
  Actor_MoveToAndWait(ACTOR_PUNCH_ANT, 488, 456);
  Actor_Jump(ACTOR_PUNCH_ANT, 4, 0);
  Actor_MoveToAndWait(ACTOR_PUNCH_ANT, 496, 448);
  SuhallaGate_FightPunchAnt();
}

/// Unused: the Punch Ant would jump out as in SuhallaGate_PunchAntJumpsOut,
/// but with Jenna first stepping aside and the ant hopping further east after
/// her; no spot in the cave sets it off.
void SuhallaGate_PunchAntJumpsOutAsLeaderStepsAside(void) {
  s32 emote, frames;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_SetPosition(ACTOR_PUNCH_ANT, 472, 480);
  Event_Wait(16);
  Actor_FaceActor(gMainRoutineState.leaderActorId, ACTOR_PUNCH_ANT, 0);
  emote = EMOTE_IN_FRONT | EMOTE_EXCLAMATION;
  frames = 0;
  FieldActor_ShowEmote(gMainRoutineState.leaderActorId, emote, frames);
  Event_Wait(16);
  Actor_WalkTo(gMainRoutineState.leaderActorId, 488, 440);
  Event_Wait(16);
  FieldCamera_FollowActor(ACTOR_PUNCH_ANT, 1);
  FieldCamera_WaitForArrival();
  Actor_Jump(ACTOR_PUNCH_ANT, 4, 0);
  Event_Wait(16);
  Actor_Jump(ACTOR_PUNCH_ANT, 4, 0);
  Actor_MoveToAndWait(ACTOR_PUNCH_ANT, 488, 480);
  Event_Wait(30);
  Actor_Jump(ACTOR_PUNCH_ANT, 2, 0);
  Actor_MoveToAndWait(ACTOR_PUNCH_ANT, 504, 472);
  Actor_Jump(ACTOR_PUNCH_ANT, 4, 0);
  Actor_MoveToAndWait(ACTOR_PUNCH_ANT, 520, 464);
  Actor_Jump(ACTOR_PUNCH_ANT, 4, 0);
  Actor_MoveToAndWait(ACTOR_PUNCH_ANT, 528, 456);
  SuhallaGate_FightPunchAnt();
}

/// Back from the Punch Ant's battle, Kraden finds the fight invigorating: "My!
/// That was invigorating!" Then he falls in behind Jenna again.
void SuhallaGate_AfterPunchAntBattle(void) {
  struct Actor *leader;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)&MsgMyThatWas);
  Actor_SetPosition(ACTOR_CAVE_KRADEN, 512, 432);
  Actor_SetPosition(gMainRoutineState.leaderActorId, 528, 432);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  // "My! That was invigorating! He wasn't too strong..."
  Event_ShowMessageThenWait(ACTOR_CAVE_KRADEN, 0, 10);
  Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_CAVE_KRADEN, 2);
  Event_Wait(20);
  // "I never knew fighting could be so fun! Ah, but we'd better get on to the
  // peninsula..."
  Event_ShowMessageThenWait(ACTOR_CAVE_KRADEN, 0, 10);
  Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
  Event_Wait(30);
  Event_SetFlag(FLAG_SUHALLA_GATE_PUNCH_ANT_BEATEN);
  Actor_SetAnimation(ACTOR_CAVE_KRADEN, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  if (leader)
    Actor_SetDestination(ACTOR_CAVE_KRADEN, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_CAVE_KRADEN);
  Actor_SetPosition(ACTOR_CAVE_KRADEN, 0, 0);
  Event_Wait(10);
  Event_End();
}

/// Walking onto cells (31, 54), (32, 54) or (31, 55) on the cliffs, away from
/// the crevice, Kraden jumps out of the party beside Jenna: "Not that way,
/// Jenna! The meeting spot is down this way." She steps back east.
void SuhallaGate_PointToMeetingSpot(void) {
  struct Actor *leader;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)&MsgNotThatWay);
  Actor_LaunchFromLeader(ACTOR_CLIFFS_KRADEN, -16, 0, FACING_EAST);
  Actor_WaitForMoveById(ACTOR_CLIFFS_KRADEN);
  // "Not that way, Jenna! The meeting spot is down this way." "We must climb
  // down here and go into that cave."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_CLIFFS_KRADEN, 0, 10);
  Actor_SetAnimation(ACTOR_CLIFFS_KRADEN, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  if (leader)
    Actor_SetDestination(ACTOR_CLIFFS_KRADEN, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_CLIFFS_KRADEN);
  Actor_SetPosition(ACTOR_CLIFFS_KRADEN, 0, 0);
  Event_Wait(10);
  Actor_WalkByAndWait(gMainRoutineState.leaderActorId, 16, 0);
  Event_End();
}

// clang-format off
/// Unused: a script that squashes and stretches its actor over and over;
/// nothing runs it.
s32 gSuhallaGate_SquashScript[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(4),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(8),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_WAIT(5),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(4),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(8),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

struct SceneEntrance gSuhallaGate_RoadEntrances[] = {
    {0, CONDITION_ALWAYS, 152, 0, 504, FACING_SOUTH, 0, 32, 32, 272, 544, 0},
    {1, CONDITION_ALWAYS, 152, 0, 520, FACING_NORTH, 0, 32, 32, 272, 544, 0},
    {SUHALLA_GATE_ROAD_FROM_SUHALLA_GATE_AMBUSH_A, CONDITION_ALWAYS, 248, 0, 280, FACING_WEST, 0, 32, 32, 272, 544, 0},
    {3, CONDITION_ALWAYS, 136, 0, 304, FACING_SOUTH, 0, 32, 32, 272, 544, 0},
    {SUHALLA_GATE_ROAD_FROM_SUHALLA_GATE_AMBUSH_B, CONDITION_ALWAYS, 360, 0, 296, FACING_EAST, 0, 336, 32, 736, 496, 0},
    {SUHALLA_GATE_ROAD_FROM_VENUS_LIGHTHOUSE_ENTRANCE_EAST, CONDITION_ALWAYS, 664, 0, 424, FACING_WEST, 0, 336, 32, 736, 496, 0},
    {ENTRANCE_AFTER_SUHALLA_GATE_BATTLE, CONDITION_ALWAYS, 472, 0, 312, FACING_WEST, 0, 336, 32, 736, 496, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSuhallaGate_CliffsEntrances[] = {
    {SUHALLA_GATE_AMBUSH_FROM_SUHALLA_GATE_ROAD_A, CONDITION_ALWAYS, 40, 0, 304, FACING_EAST, 0, 16, 32, 416, 480, 0},
    {2, CONDITION_ALWAYS, 392, 0, 368, FACING_WEST, 0, 16, 32, 416, 480, 0},
    {3, CONDITION_ALWAYS, 40, 0, 912, FACING_EAST, 0, 16, 608, 720, 1008, 0},
    {SUHALLA_GATE_AMBUSH_FROM_SUHALLA_GATE_ROAD_B, CONDITION_ALWAYS, 680, 0, 904, FACING_WEST, 0, 16, 608, 720, 1008, 0},
    {SUHALLA_GATE_AMBUSH_FROM_SUHALLA_GATE_PSYNERGY_STONE, CONDITION_ALWAYS, 616, 0, 920, FACING_SOUTH, 0, 16, 608, 720, 1008, 0},
    {ENTRANCE_AFTER_SUHALLA_GATE_BATTLE, CONDITION_ALWAYS, 616, 0, 920, FACING_SOUTH, 0, 16, 608, 720, 1008, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSuhallaGate_CaveEntrances[] = {
    {SUHALLA_GATE_PSYNERGY_STONE_FROM_SUHALLA_GATE_AMBUSH, CONDITION_ALWAYS, 72, 0, 264, FACING_NORTH, 0, 24, 24, 280, 256, 0},
    {2, CONDITION_ALWAYS, 184, 0, 168, FACING_SOUTH, 0, 24, 24, 280, 256, 0},
    {3, CONDITION_ALWAYS, 280, 0, 344, FACING_SOUTH, 0, 208, 288, 704, 640, 0},
    {4, CONDITION_ALWAYS, 664, 0, 616, FACING_NORTH, 0, 208, 288, 704, 640, 0},
    {ENTRANCE_AFTER_SUHALLA_GATE_BATTLE, CONDITION_ALWAYS, 528, 0, 432, FACING_SOUTH, 0, 208, 288, 704, 640, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(SuhallaGateRoad),
    SCENE_EXIT(SUHALLA_GATE_ROAD_TO_SUHALLA_GATE_AMBUSH_A, SuhallaGateAmbush, SUHALLA_GATE_AMBUSH_FROM_SUHALLA_GATE_ROAD_A),
    SCENE_EXIT(SUHALLA_GATE_ROAD_TO_SUHALLA_GATE_AMBUSH_B, SuhallaGateAmbush, SUHALLA_GATE_AMBUSH_FROM_SUHALLA_GATE_ROAD_B),
    SCENE_EXIT(SUHALLA_GATE_ROAD_TO_VENUS_LIGHTHOUSE_ENTRANCE_EAST, VenusLighthouseEntrance, VENUS_LIGHTHOUSE_ENTRANCE_FROM_SUHALLA_GATE_ROAD_EAST),
    SCENE_EXITS(SuhallaGateAmbush),
    SCENE_EXIT(SUHALLA_GATE_AMBUSH_TO_SUHALLA_GATE_ROAD_A, SuhallaGateRoad, SUHALLA_GATE_ROAD_FROM_SUHALLA_GATE_AMBUSH_A),
    SCENE_EXIT(2, SuhallaGateAmbush, 3),
    SCENE_EXIT(3, SuhallaGateAmbush, 2),
    SCENE_EXIT(SUHALLA_GATE_AMBUSH_TO_SUHALLA_GATE_ROAD_B, SuhallaGateRoad, SUHALLA_GATE_ROAD_FROM_SUHALLA_GATE_AMBUSH_B),
    SCENE_EXIT(SUHALLA_GATE_AMBUSH_TO_SUHALLA_GATE_PSYNERGY_STONE, SuhallaGatePsynergyStone, SUHALLA_GATE_PSYNERGY_STONE_FROM_SUHALLA_GATE_AMBUSH),
    SCENE_EXITS(SuhallaGatePsynergyStone),
    SCENE_EXIT(SUHALLA_GATE_PSYNERGY_STONE_TO_SUHALLA_GATE_AMBUSH, SuhallaGateAmbush, SUHALLA_GATE_AMBUSH_FROM_SUHALLA_GATE_PSYNERGY_STONE),
    SCENE_EXIT(2, SuhallaGatePsynergyStone, 3),
    SCENE_EXIT(3, SuhallaGatePsynergyStone, 2),
    SCENE_EXIT(SUHALLA_GATE_PSYNERGY_STONE_TO_IDEJIMA, Idejima, IDEJIMA_FROM_WORLD_MAP_NORTH_A),
    SCENE_EXITS_END,
};

struct ActorPlacement gSuhallaGate_CavePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUNCH_ANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSuhallaGate_CliffsPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 264, 0, 400, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 280, 0, 368, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 472, 0, 912, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 536, 0, 912, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 552, 0, 880, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_RUFFIAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 24, 0, 24, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_RUFFIAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 24, 0, 24, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_RUFFIAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 24, 0, 24, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSuhallaGate_RoadPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_RUFFIAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 264, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gSuhallaGate_RoadTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     SUHALLA_GATE_ROAD_TO_SUHALLA_GATE_AMBUSH_A},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS,
     SUHALLA_GATE_ROAD_TO_SUHALLA_GATE_AMBUSH_B},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS,
     SUHALLA_GATE_ROAD_TO_VENUS_LIGHTHOUSE_ENTRANCE_EAST},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), ROAD_DOOR_CELL, CONDITION_ALWAYS,
     EVENT(SuhallaGate_EnterRockDoor)},
    {TRIGGER_STEP, RUFFIAN_SPOTS_PARTY_CELL, FLAG_SUHALLA_GATE_RUFFIAN_BEATEN,
     EVENT(SuhallaGate_RuffianSpotsParty)},
    // Unused: no cell of the road's map runs it.
    {TRIGGER_STEP, ROAD_UNUSED_CELL, CONDITION_ALWAYS,
     EVENT(SuhallaGate_PointToMeetingSpot)},
    {TRIGGER_TALK, ACTOR_ROAD_RUFFIAN, CONDITION_ALWAYS,
     MSGNO(MsgOoooogMaybeI)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSuhallaGate_CliffsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     SUHALLA_GATE_AMBUSH_TO_SUHALLA_GATE_ROAD_A},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS,
     SUHALLA_GATE_AMBUSH_TO_SUHALLA_GATE_ROAD_B},
    // The way into the cave, until the ambush is beaten.
    {TRIGGER_STEP, 5, FLAG_SUHALLA_GATE_AMBUSH_BEATEN,
     EVENT(SuhallaGate_RuffiansAmbushParty)},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS,
     SUHALLA_GATE_AMBUSH_TO_SUHALLA_GATE_PSYNERGY_STONE},
    {TRIGGER_STEP, KRADEN_POINTS_THE_WAY_CELL, CONDITION_ALWAYS,
     EVENT(SuhallaGate_PointToMeetingSpot)},
    {TRIGGER_TALK, ACTOR_WEST_RUFFIAN, CONDITION_ALWAYS,
     MSGNO(MsgMaybeIShould)},
    {TRIGGER_TALK, ACTOR_MIDDLE_RUFFIAN, CONDITION_ALWAYS,
     MSGNO(MsgAllRightSo)},
    {TRIGGER_TALK, ACTOR_EAST_RUFFIAN, CONDITION_ALWAYS, MSGNO(MsgOhManThere)},
    {TRIGGER_TALK, ACTOR_WEST_RUFFIAN, CONDITION_ALWAYS,
     EVENT(SuhallaGate_FightDew)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), CLIFFS_CHEST_CELL, FLAG_SUHALLA_GATE_MINT,
     FIND_ITEM(ITEM_MINT)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSuhallaGate_CaveTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     SUHALLA_GATE_PSYNERGY_STONE_TO_SUHALLA_GATE_AMBUSH},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_STEP, SUHALLA_GATE_PSYNERGY_STONE_TO_IDEJIMA, CONDITION_ALWAYS,
     EVENT(SuhallaGate_LeaveForIdejima)},
    {TRIGGER_STEP, PUNCH_ANT_CELL, FLAG_SUHALLA_GATE_PUNCH_ANT_BEATEN,
     EVENT(SuhallaGate_PunchAntJumpsOut)},
    {TRIGGER_STEP, CAVE_UNUSED_CELL, FLAG_SUHALLA_GATE_PUNCH_ANT_BEATEN,
     EVENT(SuhallaGate_PunchAntJumpsOutAsLeaderStepsAside)},
    {TRIGGER_TABLE_END},
};
