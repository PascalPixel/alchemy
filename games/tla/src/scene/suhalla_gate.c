/// @file
/// The Suhalla Gate, in the prologue: Jenna and Kraden cross it, fighting a
/// Ruffian who spots them (Room1), three more who wait in ambush (Room2), and a
/// Punch Ant in the Gateway Cave (Room3) on the way to Idejima.

#include "types.h"
#include "flags.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "scene_tables.h"
#include "field/event_inline.h"
#include "module/field/event_commands.h"
#include "module/scene/scene.h"
#include "scene_ids.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "event_work.h"

extern u32 gSceneExits[];

void Actor_SetActionCallback(struct Actor *object, s32 a);
void SuhallaGate_AfterThievesBattle(void);
void Actor_FallOverById(s32 id, s32 a);
void Actor_FallOverWithAnimationById(s32 id, s32 a, s32 b);
void Event_End(void);
void FieldCamera_WaitForArrival(void);
/// The Punch Ant hops at the leader and its battle begins.
void SuhallaGate_FightPunchAnt(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
extern u8 MsgMyThatWas[];
extern u8 MsgNotThatWay[];
#include "text/msg_ids.h"

extern struct SceneEntrance gSuhallaGate_Entrances[];
extern struct SceneEntrance gSuhallaGate_Room2Entrances[];
extern struct SceneEntrance gSuhallaGate_Room3Entrances[];
extern struct ActorPlacement gSuhallaGate_Placements[];
extern struct ActorPlacement gSuhallaGate_Room2Placements[];
extern struct ActorPlacement gSuhallaGate_Room1Placements[];
extern struct Trigger gSuhallaGate_Triggers[];
extern struct Trigger gSuhallaGate_Room2Triggers[];
extern struct Trigger gSuhallaGate_Room3Triggers[];

/// Talking to a Ruffian again starts his battle.
void SuhallaGate_FightRuffian(void) { Field_RequestDjinnBattle(13, 26); }

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_SuhallaGate2)
    return gSuhallaGate_Room2Entrances;
  if (scene == (s32)&SceneId_SuhallaGate3)
    return gSuhallaGate_Room3Entrances;
  return gSuhallaGate_Entrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_SuhallaGate2)
    return gSuhallaGate_Room2Placements;
  if (scene == (s32)&SceneId_SuhallaGate1)
    return gSuhallaGate_Room1Placements;
  return gSuhallaGate_Placements;
}

const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_SuhallaGate2)
    return gSuhallaGate_Room2Triggers;
  if (scene == (s32)&SceneId_SuhallaGate3)
    return gSuhallaGate_Room3Triggers;
  return gSuhallaGate_Triggers;
}

/// After the first battle (flag 0x894) its Ruffian lies beaten by the path.
void SuhallaGate_SetUpRoom1(void) {
  if (gMainRoutineWork.entrance == 99)
    Event_SetFlag(0x894);
  if (Event_GetFlag(0x894)) {
    Actor_SetPosition(8, 456, 312);
    Actor_FallOverWithAnimationById(8, 0x4000, 0);
    Actor_SetActionCallback(Actor_GetById(8), 8);
  }
}

/// After the ambush (flag 0x895) its three Ruffians lie beaten.
void SuhallaGate_SetUpRoom2(void) {
  MapGrid_CopyCellAttributes(108, 38, 1, 1, 102, 56);
  if (gMainRoutineWork.entrance == 99)
    Event_SetFlag(0x895);
  if (Event_GetFlag(0x895)) {
    Actor_SetPosition(13, 590, 920);
    Actor_SetPosition(14, 624, 939);
    Actor_SetPosition(15, 632, 920);
    Actor_GetById(13)->facing = 0x4000;
    Actor_FallOverById(13, 0x6000);
    Actor_GetById(14)->facing = 0;
    Actor_FallOverById(14, 0);
    Actor_GetById(15)->facing = 0xc000;
    Actor_FallOverById(15, 0);
    Actor_GetById(13)->collisionFlags = 1;
    Actor_GetById(14)->collisionFlags = 1;
    Actor_GetById(15)->collisionFlags = 1;
  }
  Actor_SetAnimation(8, 4);
  Actor_SetAnimation(9, 4);
  Actor_SetAnimation(10, 3);
  Actor_SetAnimation(11, 4);
  Actor_SetAnimation(12, 3);
}

/// Back from the Punch Ant's battle, the party talks it over.
void SuhallaGate_SetUpRoom3(void) {
  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0 &&
      gMainRoutineWork.entrance == 99)
    SuhallaGate_AfterThievesBattle();
}

s32 Scene_Initialize(void) {
  struct EventWork *work;
  s16 scene;

  work = Ram_HeapSlots->eventWork;
  work->screenEffect = 256;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_SuhallaGate1)
    SuhallaGate_SetUpRoom1();
  else if (scene == (s32)&SceneId_SuhallaGate2)
    SuhallaGate_SetUpRoom2();
  else if (scene == (s32)&SceneId_SuhallaGate3)
    SuhallaGate_SetUpRoom3();
  return 0;
}

/// Unused: opens Room2's cell as its set-up does, and sets flag 0x8de; no
/// trigger runs it.
UNUSED void SuhallaGate_OpenRoom2Cell(void) {
  MapGrid_CopyCellAttributes(108, 38, 1, 1, 102, 56);
  Event_SetFlag(0x8de);
}

void SuhallaGate_EnterHouse(void) {
  s32 value = *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x154);

  Actor_GetById(0)->motionFlags = 0;
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_CopyCellsTo(66, 36, 71, 8, 2, 2);
  wait(4);
  MapGrid_CopyCellsTo(68, 36, 71, 8, 2, 2);
  wait(4);
  Actor_CenterAndWalk(0, 3, -16);
  Event_PlaySound(SE_MAP_EXIT);
  Import_EventSetExitRequest(value);
}

/// The leader takes control again and the party leaves for Idejima.
void SuhallaGate_LeaveForIdejima(void) {
  s32 value = *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x154);

  gMainRoutineWork.currentOwner = 4;
  Event_PlaySound(SE_MAP_EXIT);
  Import_EventSetExitRequest(value);
}

s32 Scene_PrepareMap(void) { return 0; }

/// Places the leader and the camera after a message, runs actor 8's motions
/// and sends the party to Suhalla Gate 1, entrance 99. The message is
/// "Waahhh".
void SuhallaGate_ThievesSpotParty(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgWaahhhItS);
  Actor_WalkTo(gMainRoutineWork.currentOwner, 472, 320);
  FieldCamera_MoveTo(456, CAMERA_KEEP, 280, 1);
  FieldCamera_WaitForArrival();
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
  Event_Wait(10);
  Actor_Jump(8, 6, 20);
  Event_Wait(20);
  Event_ShowMessageThenWait(8, 0, 10);
  {
    s32 x = 256;
    s32 z = 50;

    FieldActor_ShowEmote(8, x, z);
  }
  Event_ShowMessageThenWait(8, 0, 10);
  Actor_SetAnimationAndWait(8, 3);
  Event_Wait(30);
  Event_ShowMessageThenWait(8, 0, 10);
  Actor_WalkToAndWait(8, 456, 312);
  Event_SetAfterBattleDestination((s32)&SceneId_SuhallaGate1, 99);
  Party_SetFields1f2And1f4((s32)&SceneId_VenusLighthouseEntrance, 98);
  gMainRoutineWork.battleRule24b = 2;
  Field_RequestBattleFromEncounterTable(9, 1);
  Event_End();
}

/// "Ha! That was a good idea, waiting here for 'em!"
void SuhallaGate_ThievesAmbushParty(void) {
  struct MainRoutineWork *work;
  s32 *owner;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgHaThatWas);
  work = &gMainRoutineWork;
  owner = &work->currentOwner;
  Actor_WalkByAndWait(*owner, 0, -4);
  FieldEvent_Wait(30);
  Actor_GetById(*owner)->actionFlags &= 254;
  Actor_WalkByAndWait(*owner, 0, 12);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(*owner, 0, 16);
  FieldEvent_Wait(30);
  Actor_SetPosition(13, 616, 912);
  Actor_WalkToAndWait(13, 0x258, 0x398);
  Actor_SetPosition(15, 616, 912);
  Actor_FaceDirection(13, FACING_SOUTH, 0);
  Actor_WalkToAndWait(15, 0x278, 0x398);
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  Actor_SetPosition(14, 616, 912);
  Actor_WalkToAndWait(14, 0x268, 0x398);
  Actor_RunRepeatedMotion(13, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(13, 0, 10);
  FieldEvent_ShowMessageThenWait(14, 0, 10);
  Actor_SetAnimationAndWait(15, 3);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
  Actor_SetAnimationAndWait(13, 3);
  Actor_SetAnimation(14, 3);
  Actor_SetAnimationAndWait(15, 3);
  FieldEvent_SetAfterBattleDestination((s32)&SceneId_SuhallaGate2, 99);
  Event_SetDefeatDestination((s32)&SceneId_VenusLighthouseEntrance, 98);
  *((u8 *)work + 0x24b) = 2;
  Field_RequestBattleFromEncounterTable(9, 2);
  Event_End();
}

void SuhallaGate_FightPunchAnt(void) {
  struct Actor *owner;

  Actor_Jump(8, 4, 0);
  owner = Actor_GetById(gMainRoutineWork.currentOwner);
  if (owner != NULL)
    Actor_SetDestination(8, *(s16 *)((u8 *)owner + 10),
                         *(s16 *)((u8 *)owner + 18));
  Event_Wait(20);
  Event_SetAfterBattleDestination((s32)&SceneId_SuhallaGate3, 99);
  Party_SetFields1f2And1f4((s32)&SceneId_VenusLighthouseEntrance, 98);
  gMainRoutineWork.battleRule24b = 2;
  Field_RequestBattleFromEncounterTable(9, 3);
  Event_End();
}

/// The Punch Ant springs out and hops at the party, from either side of the
/// cave's path.
void SuhallaGate_PunchAntHopsFromWest(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetPosition(8, 472, 480);
  Event_Wait(16);
  Actor_FaceActor(gMainRoutineWork.currentOwner, 8, 0);
  FieldActor_ShowEmote(gMainRoutineWork.currentOwner,
                       EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldCamera_FollowActor(8, 1);
  FieldCamera_WaitForArrival();
  Actor_Jump(8, 4, 0);
  Event_Wait(16);
  Actor_Jump(8, 4, 0);
  Actor_MoveToAndWait(8, 480, 472);
  Event_Wait(30);
  Actor_Jump(8, 2, 0);
  Actor_MoveToAndWait(8, 496, 464);
  Actor_Jump(8, 4, 0);
  Actor_MoveToAndWait(8, 488, 456);
  Actor_Jump(8, 4, 0);
  Actor_MoveToAndWait(8, 496, 448);
  SuhallaGate_FightPunchAnt();
}

void SuhallaGate_PunchAntHopsFromEast(void) {
  s32 x, z;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetPosition(8, 472, 480);
  Event_Wait(16);
  Actor_FaceActor(gMainRoutineWork.currentOwner, 8, 0);
  x = 0x100;
  z = 0;
  FieldActor_ShowEmote(gMainRoutineWork.currentOwner, x, z);
  Event_Wait(16);
  Actor_WalkTo(gMainRoutineWork.currentOwner, 488, 440);
  Event_Wait(16);
  FieldCamera_FollowActor(8, 1);
  FieldCamera_WaitForArrival();
  Actor_Jump(8, 4, 0);
  Event_Wait(16);
  Actor_Jump(8, 4, 0);
  Actor_MoveToAndWait(8, 488, 480);
  Event_Wait(30);
  Actor_Jump(8, 2, 0);
  Actor_MoveToAndWait(8, 504, 472);
  Actor_Jump(8, 4, 0);
  Actor_MoveToAndWait(8, 520, 464);
  Actor_Jump(8, 4, 0);
  Actor_MoveToAndWait(8, 528, 456);
  SuhallaGate_FightPunchAnt();
}

/// "My! That was invigorating! He wasn't too strong..."
void SuhallaGate_AfterThievesBattle(void) {
  struct Actor *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgMyThatWas);
  Actor_SetPosition(9, 512, 432);
  Actor_SetPosition(gMainRoutineWork.currentOwner, 528, 432);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  Event_Wait(30);
  Actor_RunRepeatedMotion(9, 2);
  Event_Wait(20);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  Event_Wait(30);
  Event_SetFlag(0x896);
  Actor_SetAnimation(9, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader)
    Actor_SetDestination(9, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(9);
  Actor_SetPosition(9, 0, 0);
  Event_Wait(10);
  Event_End();
}

/// "Not that way, Jenna! The meeting spot is down this way."
void SuhallaGate_PointToMeetingSpot(void) {
  struct Actor *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgNotThatWay);
  Actor_LaunchFromLeader(16, -16, 0, 0);
  Actor_WaitForMoveById(16);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 16, 0, 10);
  Actor_SetAnimation(16, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader)
    Actor_SetDestination(16, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(16);
  Actor_SetPosition(16, 0, 0);
  Event_Wait(10);
  Actor_WalkByAndWait(gMainRoutineWork.currentOwner, 16, 0);
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

struct SceneEntrance gSuhallaGate_Entrances[] = {
    {0, CONDITION_ALWAYS, 152, 0, 504, FACING_SOUTH, 0, 32, 32, 272, 544, 0},
    {1, CONDITION_ALWAYS, 152, 0, 520, FACING_NORTH, 0, 32, 32, 272, 544, 0},
    {2, CONDITION_ALWAYS, 248, 0, 280, FACING_WEST, 0, 32, 32, 272, 544, 0},
    {3, CONDITION_ALWAYS, 136, 0, 304, FACING_SOUTH, 0, 32, 32, 272, 544, 0},
    {4, CONDITION_ALWAYS, 360, 0, 296, FACING_EAST, 0, 336, 32, 736, 496, 0},
    {5, CONDITION_ALWAYS, 664, 0, 424, FACING_WEST, 0, 336, 32, 736, 496, 0},
    {99, CONDITION_ALWAYS, 472, 0, 312, FACING_WEST, 0, 336, 32, 736, 496, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSuhallaGate_Room2Entrances[] = {
    {1, CONDITION_ALWAYS, 40, 0, 304, FACING_EAST, 0, 16, 32, 416, 480, 0},
    {2, CONDITION_ALWAYS, 392, 0, 368, FACING_WEST, 0, 16, 32, 416, 480, 0},
    {3, CONDITION_ALWAYS, 40, 0, 912, FACING_EAST, 0, 16, 608, 720, 1008, 0},
    {4, CONDITION_ALWAYS, 680, 0, 904, FACING_WEST, 0, 16, 608, 720, 1008, 0},
    {5, CONDITION_ALWAYS, 616, 0, 920, FACING_SOUTH, 0, 16, 608, 720, 1008, 0},
    {99, CONDITION_ALWAYS, 616, 0, 920, FACING_SOUTH, 0, 16, 608, 720, 1008, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSuhallaGate_Room3Entrances[] = {
    {1, CONDITION_ALWAYS, 72, 0, 264, FACING_NORTH, 0, 24, 24, 280, 256, 0},
    {2, CONDITION_ALWAYS, 184, 0, 168, FACING_SOUTH, 0, 24, 24, 280, 256, 0},
    {3, CONDITION_ALWAYS, 280, 0, 344, FACING_SOUTH, 0, 208, 288, 704, 640, 0},
    {4, CONDITION_ALWAYS, 664, 0, 616, FACING_NORTH, 0, 208, 288, 704, 640, 0},
    {99, CONDITION_ALWAYS, 528, 0, 432, FACING_SOUTH, 0, 208, 288, 704, 640, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(6),
    SCENE_EXIT(2, 7, 1),
    SCENE_EXIT(4, 7, 4),
    SCENE_EXIT(5, 4, 2),
    SCENE_EXITS(7),
    SCENE_EXIT(1, 6, 2),
    SCENE_EXIT(2, 7, 3),
    SCENE_EXIT(3, 7, 2),
    SCENE_EXIT(4, 6, 4),
    SCENE_EXIT(5, 8, 1),
    SCENE_EXITS(8),
    SCENE_EXIT(1, 7, 5),
    SCENE_EXIT(2, 8, 3),
    SCENE_EXIT(3, 8, 2),
    SCENE_EXIT(4, 9, 11),
    SCENE_EXITS_END,
};

struct ActorPlacement gSuhallaGate_Placements[] = {
    {247, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 0, 0},
    {56, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTHEAST, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gSuhallaGate_Room2Placements[] = {
    {299, CONDITION_ALWAYS, 7, PIXELS(264), 0, PIXELS(400), 0x5000, 2, 0},
    {299, CONDITION_ALWAYS, 7, PIXELS(280), 0, PIXELS(368), 0x5000, 2, 0},
    {299, CONDITION_ALWAYS, 7, PIXELS(472), 0, PIXELS(912), 0x5000, 2, 0},
    {299, CONDITION_ALWAYS, 7, PIXELS(536), 0, PIXELS(912), 0x5000, 2, 0},
    {299, CONDITION_ALWAYS, 7, PIXELS(552), 0, PIXELS(880), 0x5000, 2, 0},
    {93, CONDITION_ALWAYS, 1, PIXELS(24), 0, PIXELS(24), FACING_SOUTHEAST, 2, 0,
     },
    {93, CONDITION_ALWAYS, 1, PIXELS(24), 0, PIXELS(24), FACING_SOUTHEAST, 2, 0,
     },
    {93, CONDITION_ALWAYS, 1, PIXELS(24), 0, PIXELS(24), FACING_SOUTHEAST, 2, 0,
     },
    {56, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTHEAST, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gSuhallaGate_Room1Placements[] = {
    {93, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(264), FACING_SOUTHEAST, 2,
     0},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gSuhallaGate_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(SuhallaGate_EnterHouse)},
    {TRIGGER_STEP, 20, 0x894, EVENT(SuhallaGate_ThievesSpotParty)},
    {TRIGGER_STEP, 12, CONDITION_ALWAYS, EVENT(SuhallaGate_PointToMeetingSpot)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgOoooogMaybeI)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSuhallaGate_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_STEP, 5, 0x895, EVENT(SuhallaGate_ThievesAmbushParty)},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(SuhallaGate_PointToMeetingSpot)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgMaybeIShould)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgAllRightSo)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgOhManThere)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(SuhallaGate_FightRuffian)},
    {19, 100, 0xfb4, 0x1000c3},
    {TRIGGER_TABLE_END},
};

struct Trigger gSuhallaGate_Room3Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {49, 2, CONDITION_ALWAYS, 2},
    {33, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(SuhallaGate_LeaveForIdejima)},
    {TRIGGER_STEP, 22, 0x896, EVENT(SuhallaGate_PunchAntHopsFromWest)},
    {TRIGGER_STEP, 23, 0x896, EVENT(SuhallaGate_PunchAntHopsFromEast)},
    {TRIGGER_TABLE_END},
};
