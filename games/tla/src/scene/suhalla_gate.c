/// @file
/// The Suhalla Gate, in the prologue: Jenna and Kraden cross it on their way
/// to Idejima, while Alex holds back the soldiers behind them. Its three rooms
/// share this overlay:
///
/// - The road past the gate (SceneId_SuhallaGate1), where a Ruffian spots the
///   two and fights them (SuhallaGate_RuffianSpotsParty).
/// - The cliffs (SceneId_SuhallaGate2), whose two vines lead down to the
///   crevice where three Ruffians wait in ambush
///   (SuhallaGate_RuffiansAmbushParty).
/// - The Gateway Cave (SceneId_SuhallaGate3), where a Punch Ant hops out at the
///   two before the Psynergy Stone (SuhallaGate_PunchAntHopsFromWest and
///   SuhallaGate_PunchAntHopsFromEast), and whose far end leads to Idejima.
///
/// Every battle sends the party back to its room's ENTRANCE_AFTER_BATTLE, and
/// a lost one to the Venus Lighthouse's entrance. Back from each, its flag set,
/// the beaten Ruffians lie where they fell.

#include "flags.h"
#include "types.h"
#include "actor.h"
#include "djinn_ids.h"
#include "item_ids.h"
#include "lib.h"
#include "scene_ids.h"
#include "scene_tables.h"
#include "battle/battle.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_sequence_event.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "module/field/event_commands.h"
#include "module/scene/scene.h"
#include "sound/sound.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "text/msg_ids.h"

/// Set as the party comes back from beating the Ruffian on the road; he then
/// lies beaten by it.
#define FLAG_SUHALLA_GATE_RUFFIAN_BEATEN 0x894
/// Set as the party comes back from beating the three Ruffians at the crevice;
/// they then lie beaten before it.
#define FLAG_SUHALLA_GATE_AMBUSH_BEATEN 0x895
/// Set once Kraden has talked over the Punch Ant's battle; the Gateway Cave's
/// Punch Ant then stays away.
#define FLAG_GATEWAY_CAVE_PUNCH_ANT_BEATEN 0x896
/// Set once the chest in the cliffs room (cell behavior 100) has given its
/// Mint.
#define FLAG_SUHALLA_GATE_MINT 0xfb4

/// The entrance each room's battles send the party back to.
#define ENTRANCE_AFTER_BATTLE 99
/// The Venus Lighthouse entrance a lost battle sends the party back to.
#define ENTRANCE_VENUS_LIGHTHOUSE_DEFEAT 98

/// The road's one placed actor.
enum SuhallaGateRoadActorId {
  /// The Ruffian who spots Jenna and Kraden on the road.
  ACTOR_ROAD_RUFFIAN = ACTOR_FIRST_PLACED
};

/// The cliffs' placed actors.
enum SuhallaGateCliffsActorId {
  /// The first of five actors drawn with sprite 299 and moved by movement
  /// type 7, each given its own animation as the party enters.
  // XXX: what the five actors of sprite 299 are; a render of
  // SceneId_SuhallaGate2
  // would settle it.
  ACTOR_CLIFFS_SPRITE_299_FIRST = ACTOR_FIRST_PLACED,
  /// The three Ruffians of the ambush, west to east as they line up.
  ACTOR_WEST_RUFFIAN = ACTOR_FIRST_PLACED + 5,
  ACTOR_MIDDLE_RUFFIAN,
  ACTOR_EAST_RUFFIAN,
  /// Kraden, who jumps out from behind Jenna to point the way.
  ACTOR_CLIFFS_KRADEN
};

/// The Gateway Cave's placed actors.
enum GatewayCaveActorId {
  ACTOR_PUNCH_ANT = ACTOR_FIRST_PLACED,
  ACTOR_CAVE_KRADEN
};

extern u32 gSceneExits[];

void Actor_SetActionCallback(struct Actor *object, s32 a);
void SuhallaGate_AfterPunchAntBattle(void);
void Actor_FallOverById(s32 id, s32 a);
void Actor_FallOverWithAnimationById(s32 id, s32 a, s32 b);
void Event_End(void);
void FieldCamera_WaitForArrival(void);
/// The Punch Ant hops at the leader and its battle begins.
void SuhallaGate_FightPunchAnt(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

extern struct SceneEntrance gSuhallaGate_RoadEntrances[];
extern struct SceneEntrance gSuhallaGate_CliffsEntrances[];
extern struct SceneEntrance gSuhallaGate_CaveEntrances[];
extern struct ActorPlacement gSuhallaGate_CavePlacements[];
extern struct ActorPlacement gSuhallaGate_CliffsPlacements[];
extern struct ActorPlacement gSuhallaGate_RoadPlacements[];
extern struct Trigger gSuhallaGate_RoadTriggers[];
extern struct Trigger gSuhallaGate_CliffsTriggers[];
extern struct Trigger gSuhallaGate_CaveTriggers[];

/// Unused: a battle with the Djinni Dew, from the west Ruffian; the row before
/// this one's in gSuhallaGate_CliffsTriggers answers talking to him first.
void SuhallaGate_FightDew(void) {
  Field_RequestDjinnBattle(ACTOR_WEST_RUFFIAN, DJINNI_DEW);
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_SUHALLA_GATE_2)
    return gSuhallaGate_CliffsEntrances;
  if (scene == SCENE_SUHALLA_GATE_3)
    return gSuhallaGate_CaveEntrances;
  return gSuhallaGate_RoadEntrances;
}

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_SUHALLA_GATE_2)
    return gSuhallaGate_CliffsPlacements;
  if (scene == SCENE_SUHALLA_GATE_1)
    return gSuhallaGate_RoadPlacements;
  return gSuhallaGate_CavePlacements;
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_SUHALLA_GATE_2)
    return gSuhallaGate_CliffsTriggers;
  if (scene == SCENE_SUHALLA_GATE_3)
    return gSuhallaGate_CaveTriggers;
  return gSuhallaGate_RoadTriggers;
}

/// Back from his battle, the road's Ruffian lies beaten by it.
void SuhallaGate_SetUpRoad(void) {
  if (gMainRoutineWork.entrance == ENTRANCE_AFTER_BATTLE)
    Event_SetFlag(FLAG_SUHALLA_GATE_RUFFIAN_BEATEN);
  if (Event_GetFlag(FLAG_SUHALLA_GATE_RUFFIAN_BEATEN)) {
    Actor_SetPosition(ACTOR_ROAD_RUFFIAN, 456, 312);
    Actor_FallOverWithAnimationById(ACTOR_ROAD_RUFFIAN, FACING_SOUTH, 0);
    Actor_SetActionCallback(Actor_GetById(ACTOR_ROAD_RUFFIAN), 8);
  }
}

/// Back from the ambush, its three Ruffians lie beaten before the crevice.
void SuhallaGate_SetUpCliffs(void) {
  MapGrid_CopyCellAttributes(108, 38, 1, 1, 102, 56);
  if (gMainRoutineWork.entrance == ENTRANCE_AFTER_BATTLE)
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
    Actor_GetById(ACTOR_WEST_RUFFIAN)->collisionFlags = 1;
    Actor_GetById(ACTOR_MIDDLE_RUFFIAN)->collisionFlags = 1;
    Actor_GetById(ACTOR_EAST_RUFFIAN)->collisionFlags = 1;
  }
  Actor_SetAnimation(ACTOR_CLIFFS_SPRITE_299_FIRST, ANIM_SHAKE_HEAD);
  Actor_SetAnimation(ACTOR_CLIFFS_SPRITE_299_FIRST + 1, 4);
  Actor_SetAnimation(ACTOR_CLIFFS_SPRITE_299_FIRST + 2, 3);
  Actor_SetAnimation(ACTOR_CLIFFS_SPRITE_299_FIRST + 3, 4);
  Actor_SetAnimation(ACTOR_CLIFFS_SPRITE_299_FIRST + 4, 3);
}

/// Back from the Punch Ant's battle, Kraden talks it over.
void SuhallaGate_SetUpCave(void) {
  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0 &&
      gMainRoutineWork.entrance == ENTRANCE_AFTER_BATTLE)
    SuhallaGate_AfterPunchAntBattle();
}

s32 Scene_Initialize(void) {
  struct FieldWork *work;
  s16 scene;

  work = Ram_HeapSlots->fieldWork;
  work->transition = 256;
  scene = gMainRoutineWork.next;
  if (scene == SCENE_SUHALLA_GATE_1)
    SuhallaGate_SetUpRoad();
  else if (scene == SCENE_SUHALLA_GATE_2)
    SuhallaGate_SetUpCliffs();
  else if (scene == SCENE_SUHALLA_GATE_3)
    SuhallaGate_SetUpCave();
  return 0;
}

/// Unused: copies the cliffs' cell as their set-up does, and sets
/// FLAG_SHIP_SAILABLE, which the Lemurian Ship's deck sets once the party can
/// sail it; no trigger runs it.
UNUSED void SuhallaGate_OpenCliffsCell(void) {
  MapGrid_CopyCellAttributes(108, 38, 1, 1, 102, 56);
  Event_SetFlag(FLAG_SHIP_SAILABLE);
}

/// Opens the door the leader walks into and walks actor 0 through it, as The
/// Broken Seal's doors walk their leader, actor 0 there; here Jenna leads.
void SuhallaGate_EnterHouse(void) {
  s32 value = gFieldWork->stepTrigger;

  Actor_GetById(ACTOR_ISAAC)->motionFlags = 0;
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_CopyCellsTo(66, 36, 71, 8, 2, 2);
  wait(4);
  MapGrid_CopyCellsTo(68, 36, 71, 8, 2, 2);
  wait(4);
  Actor_CenterAndWalk(ACTOR_ISAAC, 3, -16);
  Event_PlaySound(SE_MAP_EXIT);
  Import_EventSetExitRequest(value);
}

/// Felix becomes the leader again as Jenna and Kraden leave for Idejima.
void SuhallaGate_LeaveForIdejima(void) {
  s32 value = gFieldWork->stepTrigger;

  gMainRoutineWork.leaderActorId = ACTOR_FELIX;
  Event_PlaySound(SE_MAP_EXIT);
  Import_EventSetExitRequest(value);
}

s32 Scene_PrepareMap(void) { return 0; }

/// The road's Ruffian spots Jenna and Kraden and fights them.
void SuhallaGate_RuffianSpotsParty(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgWaahhhItS);
  Actor_WalkTo(gMainRoutineWork.leaderActorId, 472, 320);
  FieldCamera_MoveTo(456, CAMERA_KEEP, 280, 1);
  FieldCamera_WaitForArrival();
  Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHWEST, 0);
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
  Event_SetAfterBattleDestination(SCENE_SUHALLA_GATE_1, ENTRANCE_AFTER_BATTLE);
  Party_SetFields1f2And1f4(SCENE_VENUS_LIGHTHOUSE_ENTRANCE,
                           ENTRANCE_VENUS_LIGHTHOUSE_DEFEAT);
  gMainRoutineWork.battleType = BATTLE_TYPE_NO_ESCAPE;
  Field_RequestBattleFromEncounterTable(9, 1);
  Event_End();
}

/// Three Ruffians who waited at the crevice step out and fight Jenna and
/// Kraden.
void SuhallaGate_RuffiansAmbushParty(void) {
  struct MainRoutineWork *work;
  s32 *owner;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgHaThatWas);
  work = &gMainRoutineWork;
  owner = &work->leaderActorId;
  Actor_WalkByAndWait(*owner, 0, -4);
  FieldEvent_Wait(30);
  Actor_GetById(*owner)->steeringFlags &= 254;
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
  FieldEvent_SetAfterBattleDestination(SCENE_SUHALLA_GATE_2,
                                       ENTRANCE_AFTER_BATTLE);
  Event_SetDefeatDestination(SCENE_VENUS_LIGHTHOUSE_ENTRANCE,
                             ENTRANCE_VENUS_LIGHTHOUSE_DEFEAT);
  work->battleType = BATTLE_TYPE_NO_ESCAPE;
  Field_RequestBattleFromEncounterTable(9, 2);
  Event_End();
}

void SuhallaGate_FightPunchAnt(void) {
  struct Actor *owner;

  Actor_Jump(ACTOR_PUNCH_ANT, 4, 0);
  owner = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (owner != NULL)
    Actor_SetDestination(ACTOR_PUNCH_ANT, owner->x.part[0].pixel,
                         owner->z.part[0].pixel);
  Event_Wait(20);
  Event_SetAfterBattleDestination(SCENE_SUHALLA_GATE_3, ENTRANCE_AFTER_BATTLE);
  Party_SetFields1f2And1f4(SCENE_VENUS_LIGHTHOUSE_ENTRANCE,
                           ENTRANCE_VENUS_LIGHTHOUSE_DEFEAT);
  gMainRoutineWork.battleType = BATTLE_TYPE_NO_ESCAPE;
  Field_RequestBattleFromEncounterTable(9, 3);
  Event_End();
}
/// The Punch Ant springs out and hops at the party, from either side of the
/// cave's path.
void SuhallaGate_PunchAntHopsFromWest(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetPosition(ACTOR_PUNCH_ANT, 472, 480);
  Event_Wait(16);
  Actor_FaceActor(gMainRoutineWork.leaderActorId, ACTOR_PUNCH_ANT, 0);
  FieldActor_ShowEmote(gMainRoutineWork.leaderActorId,
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

/// As SuhallaGate_PunchAntHopsFromWest, from the east, the leader stepping
/// aside first.
void SuhallaGate_PunchAntHopsFromEast(void) {
  s32 emote, frames;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetPosition(ACTOR_PUNCH_ANT, 472, 480);
  Event_Wait(16);
  Actor_FaceActor(gMainRoutineWork.leaderActorId, ACTOR_PUNCH_ANT, 0);
  emote = EMOTE_IN_FRONT | EMOTE_EXCLAMATION;
  frames = 0;
  FieldActor_ShowEmote(gMainRoutineWork.leaderActorId, emote, frames);
  Event_Wait(16);
  Actor_WalkTo(gMainRoutineWork.leaderActorId, 488, 440);
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

/// Back from the Punch Ant's battle, Kraden talks it over and goes back into
/// the party.
void SuhallaGate_AfterPunchAntBattle(void) {
  struct Actor *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgMyThatWas);
  Actor_SetPosition(ACTOR_CAVE_KRADEN, 512, 432);
  Actor_SetPosition(gMainRoutineWork.leaderActorId, 528, 432);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  // "My! That was invigorating! He wasn't too strong..."
  Event_ShowMessageThenWait(ACTOR_CAVE_KRADEN, 0, 10);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_CAVE_KRADEN, 2);
  Event_Wait(20);
  // "I never knew fighting could be so fun! Ah, but we'd better get on to the
  // peninsula..."
  Event_ShowMessageThenWait(ACTOR_CAVE_KRADEN, 0, 10);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Event_Wait(30);
  Event_SetFlag(FLAG_GATEWAY_CAVE_PUNCH_ANT_BEATEN);
  Actor_SetAnimation(ACTOR_CAVE_KRADEN, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader)
    Actor_SetDestination(ACTOR_CAVE_KRADEN, leader->x.part[0].pixel,
                         leader->z.part[0].pixel);
  Actor_WaitForMoveById(ACTOR_CAVE_KRADEN);
  Actor_SetPosition(ACTOR_CAVE_KRADEN, 0, 0);
  Event_Wait(10);
  Event_End();
}

/// Kraden jumps out from behind the leader to point the way down to the
/// Gateway Cave, and the leader steps back.
void SuhallaGate_PointToMeetingSpot(void) {
  struct Actor *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgNotThatWay);
  Actor_LaunchFromLeader(ACTOR_CLIFFS_KRADEN, -16, 0, FACING_EAST);
  Actor_WaitForMoveById(ACTOR_CLIFFS_KRADEN);
  // "Not that way, Jenna! The meeting spot is down this way." "We must climb
  // down here and go into that cave."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_CLIFFS_KRADEN, 0, 10);
  Actor_SetAnimation(ACTOR_CLIFFS_KRADEN, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader)
    Actor_SetDestination(ACTOR_CLIFFS_KRADEN, leader->x.part[0].pixel,
                         leader->z.part[0].pixel);
  Actor_WaitForMoveById(ACTOR_CLIFFS_KRADEN);
  Actor_SetPosition(ACTOR_CLIFFS_KRADEN, 0, 0);
  Event_Wait(10);
  Actor_WalkByAndWait(gMainRoutineWork.leaderActorId, 16, 0);
  Event_End();
}

// clang-format off
/// Unused: a script that squashes and stretches its actor over and over;
/// nothing runs it.
s32 gSuhallaGate_SquashLoop[] = {
    (s32)0x80010000, 23, 9, -0x4000, 23, 10,
    0x2000, 0, 2, 23, 9, 0x2000,
    23, 10, -4096, 0, 2, 23,
    9, 0x1000, 23, 10, -2048, 0,
    2, 23, 9, 0x1000, 23, 10,
    -2048, 0, 4, (s32)0x80010000, 23, 9,
    -0x4000, 23, 10, 0x2000, 0, 2,
    23, 9, 0x2000, 23, 10, -4096,
    0, 2, 23, 9, 0x1000, 23,
    10, -2048, 0, 2, 23, 9,
    0x1000, 23, 10, -2048, 0, 8,
    13, 0xffff, (s32)0xc0010000, 0, 5, (s32)0x80010000,
    23, 9, -0x4000, 23, 10, 0x2000,
    0, 2, 23, 9, 0x2000, 23,
    10, -4096, 0, 2, 23, 9,
    0x1000, 23, 10, -2048, 0, 2,
    23, 9, 0x1000, 23, 10, -2048,
    0, 4, (s32)0x80010000, 23, 9, -0x4000,
    23, 10, 0x2000, 0, 2, 23,
    9, 0x2000, 23, 10, -4096, 0,
    2, 23, 9, 0x1000, 23, 10,
    -2048, 0, 2, 23, 9, 0x1000,
    23, 10, -2048, 0, 8, 13,
    0xffff, (s32)0xc0010000,
};

struct SceneEntrance gSuhallaGate_RoadEntrances[] = {
    {0, CONDITION_ALWAYS, 152, 0, 504, FACING_SOUTH, 0, 32, 32, 272, 544, 0},
    {1, CONDITION_ALWAYS, 152, 0, 520, FACING_NORTH, 0, 32, 32, 272, 544, 0},
    {2, CONDITION_ALWAYS, 248, 0, 280, FACING_WEST, 0, 32, 32, 272, 544, 0},
    {3, CONDITION_ALWAYS, 136, 0, 304, FACING_SOUTH, 0, 32, 32, 272, 544, 0},
    {4, CONDITION_ALWAYS, 360, 0, 296, FACING_EAST, 0, 336, 32, 736, 496, 0},
    {5, CONDITION_ALWAYS, 664, 0, 424, FACING_WEST, 0, 336, 32, 736, 496, 0},
    {ENTRANCE_AFTER_BATTLE, CONDITION_ALWAYS, 472, 0, 312, FACING_WEST, 0, 336, 32, 736, 496, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSuhallaGate_CliffsEntrances[] = {
    {1, CONDITION_ALWAYS, 40, 0, 304, FACING_EAST, 0, 16, 32, 416, 480, 0},
    {2, CONDITION_ALWAYS, 392, 0, 368, FACING_WEST, 0, 16, 32, 416, 480, 0},
    {3, CONDITION_ALWAYS, 40, 0, 912, FACING_EAST, 0, 16, 608, 720, 1008, 0},
    {4, CONDITION_ALWAYS, 680, 0, 904, FACING_WEST, 0, 16, 608, 720, 1008, 0},
    {5, CONDITION_ALWAYS, 616, 0, 920, FACING_SOUTH, 0, 16, 608, 720, 1008, 0},
    {ENTRANCE_AFTER_BATTLE, CONDITION_ALWAYS, 616, 0, 920, FACING_SOUTH, 0, 16, 608, 720, 1008, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSuhallaGate_CaveEntrances[] = {
    {1, CONDITION_ALWAYS, 72, 0, 264, FACING_NORTH, 0, 24, 24, 280, 256, 0},
    {2, CONDITION_ALWAYS, 184, 0, 168, FACING_SOUTH, 0, 24, 24, 280, 256, 0},
    {3, CONDITION_ALWAYS, 280, 0, 344, FACING_SOUTH, 0, 208, 288, 704, 640, 0},
    {4, CONDITION_ALWAYS, 664, 0, 616, FACING_NORTH, 0, 208, 288, 704, 640, 0},
    {ENTRANCE_AFTER_BATTLE, CONDITION_ALWAYS, 528, 0, 432, FACING_SOUTH, 0, 208, 288, 704, 640, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(SuhallaGate1),
    SCENE_EXIT(2, SuhallaGate2, 1),
    SCENE_EXIT(4, SuhallaGate2, 4),
    SCENE_EXIT(5, VenusLighthouseEntrance, 2),
    SCENE_EXITS(SuhallaGate2),
    SCENE_EXIT(1, SuhallaGate1, 2),
    SCENE_EXIT(2, SuhallaGate2, 3),
    SCENE_EXIT(3, SuhallaGate2, 2),
    SCENE_EXIT(4, SuhallaGate1, 4),
    SCENE_EXIT(5, SuhallaGate3, 1),
    SCENE_EXITS(SuhallaGate3),
    SCENE_EXIT(1, SuhallaGate2, 5),
    SCENE_EXIT(2, SuhallaGate3, 3),
    SCENE_EXIT(3, SuhallaGate3, 2),
    SCENE_EXIT(4, Idejima, 11),
    SCENE_EXITS_END,
};

struct ActorPlacement gSuhallaGate_CavePlacements[] = {
    ACTOR_PLACEMENT(247, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSuhallaGate_CliffsPlacements[] = {
    ACTOR_PLACEMENT(299, CONDITION_ALWAYS, 7, 264, 0, 400, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(299, CONDITION_ALWAYS, 7, 280, 0, 368, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(299, CONDITION_ALWAYS, 7, 472, 0, 912, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(299, CONDITION_ALWAYS, 7, 536, 0, 912, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(299, CONDITION_ALWAYS, 7, 552, 0, 880, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(93, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 24, 0, 24, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(93, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 24, 0, 24, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(93, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 24, 0, 24, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSuhallaGate_RoadPlacements[] = {
    ACTOR_PLACEMENT(93, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 264, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gSuhallaGate_RoadTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
     EVENT(SuhallaGate_EnterHouse)},
    {TRIGGER_STEP, 20, FLAG_SUHALLA_GATE_RUFFIAN_BEATEN,
     EVENT(SuhallaGate_RuffianSpotsParty)},
    {TRIGGER_STEP, 12, CONDITION_ALWAYS, EVENT(SuhallaGate_PointToMeetingSpot)},
    {TRIGGER_TALK, ACTOR_ROAD_RUFFIAN, CONDITION_ALWAYS,
     MSGNO(MsgOoooogMaybeI)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSuhallaGate_CliffsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_STEP, 5, FLAG_SUHALLA_GATE_AMBUSH_BEATEN,
     EVENT(SuhallaGate_RuffiansAmbushParty)},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(SuhallaGate_PointToMeetingSpot)},
    {TRIGGER_TALK, ACTOR_WEST_RUFFIAN, CONDITION_ALWAYS,
     MSGNO(MsgMaybeIShould)},
    {TRIGGER_TALK, ACTOR_MIDDLE_RUFFIAN, CONDITION_ALWAYS,
     MSGNO(MsgAllRightSo)},
    {TRIGGER_TALK, ACTOR_EAST_RUFFIAN, CONDITION_ALWAYS, MSGNO(MsgOhManThere)},
    {TRIGGER_TALK, ACTOR_WEST_RUFFIAN, CONDITION_ALWAYS,
     EVENT(SuhallaGate_FightDew)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_SUHALLA_GATE_MINT,
     FIND_ITEM(ITEM_MINT)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSuhallaGate_CaveTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(SuhallaGate_LeaveForIdejima)},
    {TRIGGER_STEP, 22, FLAG_GATEWAY_CAVE_PUNCH_ANT_BEATEN,
     EVENT(SuhallaGate_PunchAntHopsFromWest)},
    {TRIGGER_STEP, 23, FLAG_GATEWAY_CAVE_PUNCH_ANT_BEATEN,
     EVENT(SuhallaGate_PunchAntHopsFromEast)},
    {TRIGGER_TABLE_END},
};
