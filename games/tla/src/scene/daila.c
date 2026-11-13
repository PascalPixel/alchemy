/// @file
/// The scene code of overlay 0x651, in ROM order.

#include "shared/actors.h"
#include "event_work.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "scene_tables.h"
#include "event_inline.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "actor.h"
#include "module/scene/daila.h"
#include "module/scene/scene.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gDailaEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

static inline void ReturnHome(s32 mode, s32 actor, s32 flag) {
  /* FAKEMATCH(wrapper): the call forwarded through the inline, as in the scene
   * layer of The Broken Seal; as a direct call, which shifts 0x300 before
   * the actor is loaded, 2 of 6 instructions differ in tla-en. */
  Import_ActorReturnHome(mode, actor, flag);
}

s32 *Scene_GetExits(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Daila2)
    return gData_02001068;
  else
    return gData_02001018;
}

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Daila2)
    return gDaila2ActorPlacements;
  else
    return gDaila1ActorPlacements;
}

/// Stepping onto one of the village's doors: the leader walks through it and
/// the scene leaves by the exit of its number.
void Daila_EnterDoor(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  Import_EventStart();
  Event_ResolvePendingActions(0);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Sound_Play(123);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -6);
  Event_Wait(10);
  Import_EventSetExitRequest(work->exit);
  Import_EventFinishScreenTransition();
  Import_EventWaitTransitionFrames();
  Import_EventEnd();
}

void Func_02000118(s32 mode) { ReturnHome(mode, 66, 0x300); }

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Daila2)
    return gDaila2Triggers;
  else
    return gDaila1Triggers;
}

void Daila_TalkBeenBefore(void) {
  s32 msg;

  if (GameFlag_Test(0x843) == 0) {
    Import_EventStart();
    Event_ResolvePendingActions(0);
    msg = (s32)MsgDailaBeenBefore;
    Event_SetMessageId(msg);
    FieldMessage_ShowAtActor(8, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessageAndWait(8, 0);
    GameFlag_SetBit(0x843);
    Import_EventEnd();
  } else {
    Event_SetMessageId((s32)MsgDailaWaveNoInjuries);
    Event_ShowMessageAndWait(8, 0);
  }
}

void Daila_TalkIndraMoved(void) {
  s32 msg;

  Import_EventStart();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgDailaIndraMoved;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(9, 0);
  Import_EventEnd();
}

void Daila_TalkBoat(void) {
  s32 msg;

  Import_EventStart();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgDailaWantBoat;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(9, 0);
  Import_EventEnd();
}

void Daila_AskAfterTaviAndRiki(s32 keeper) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Import_ShopRun(3, keeper);
  } else {
    Import_EventStart();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgDailaTaviRikiWorry);
    Event_ShowMessageAndWait(keeper, 0);
    Import_EventEnd();
  }
}

void Daila_TalkHip(void) {
  s32 msg;

  Import_EventStart();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgDailaHurtHip;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(12, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(12, 0);
  Import_EventEnd();
}

void Daila_TaviAndRikiAreHome(s32 keeper) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Import_ShopRun(3, keeper);
  } else {
    Import_EventStart();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgDailaTaviRikiHome);
    Event_ShowMessageAndWait(keeper, 0);
    Import_EventEnd();
  }
}

/// Brings the actor to the player character's position.
static inline void Actor_JoinLeader(s32 actor) {
  struct Actor *leader;

  Actor_SetModeById(actor, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestinationById(actor, ((s16 *)&leader->x)[1],
                             ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(actor);
  Actor_SetHorizontalPositionWithTerrain(actor, 0, 0);
}

void Func_020003a0(void) {
  s32 msg;

  if (GameFlag_Test(0x84b))
    return;
  if (GameFlag_Test(0x910))
    return;
  if (GameFlag_Test(7))
    return;
  Import_EventStart();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgDailaFelixCall;
  Event_SetMessageId(msg);
  Actor_SetHorizontalPositionWithTerrain(27, 472 << 16, 264 << 16);
  Event_Wait(30);
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 504, 296);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
  Actor_MoveToAndWait(27, 472, 280);
  Actor_FaceDirection(27, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_ShowEmoteById(27, 256, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(27, 0);
  FieldActor_LaunchFromLeader(5, 8, 16, 0xa000);
  FieldActor_LaunchFromLeader(6, 16, 0, 0xa000);
  Actor_LaunchFromLeader(28, 32, -8, 0x8000);
  Event_Wait(60);
  Event_ShowMessageAndWait(27, 0);
  Event_Wait(30);
  Actor_ShowEmote(5, 259, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(5, 0);
  Event_Wait(30);
  Actor_ArmCallback(27, FACING_EAST, 0);
  Event_Wait(30);
  Actor_FaceDirection(27, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_ArmCallback(27, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_ArmCallback(27, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(27, 0);
  Event_Wait(30);
  Actor_FaceActorById(6, 28, 0);
  Event_Wait(30);
  Actor_SetModeById(6, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(6, 0);
  Event_Wait(30);
  Actor_TurnToFaceEachOther(gMainRoutineWork.currentOwner, 28, 0);
  Event_Wait(30);
  Actor_FaceActorById(28, 27, 0);
  Actor_FaceActorById(gMainRoutineWork.currentOwner, 27, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(28, 0);
  Event_Wait(30);
  Actor_FaceActorById(6, 27, 0);
  Actor_SetModeById(27, 4);
  Event_Wait(60);
  Event_ShowMessageAndWait(27, 0);
  Event_Wait(30);
  Actor_ArmCallback(27, FACING_NORTH, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(27, 0);
  Actor_FaceActorById(27, gMainRoutineWork.currentOwner, 0);
  Event_Wait(60);
  Actor_StartRepeatedMotion(28, 1);
  Event_Wait(60);
  Event_ShowMessageAndWait(28, 0);
  Event_Wait(30);
  Actor_SetModeById(27, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(27, 0);
  Event_Wait(30);
  Actor_FaceActorById(5, gMainRoutineWork.currentOwner, 0);
  FieldMessage_ShowAtActor(5, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(30);
    Event_SetMessageId(msg + 11);
    Event_ShowMessageAndWait(27, 0);
    Event_Wait(60);
    Actor_ShowEmoteById(5, 259, 0);
    Event_Wait(60);
    Actor_FaceActorById(5, 27, 0);
    Event_ShowMessageAndWait(5, 0);
    Event_SetMessageId(msg + 15);
  } else {
    Event_Wait(30);
    Event_SetMessageId(msg + 13);
    Event_ShowMessageAndWait(27, 0);
    Event_Wait(60);
    Motion_SetVarCbAndRefresh(5, 1);
    Actor_FaceActorById(5, 27, 0);
    Event_ShowMessageAndWait(5, 0);
  }
  Actor_SetModeById(27, 4);
  Event_Wait(60);
  Event_ShowMessageAndWait(27, 0);
  Motion_SetVarCbAndRefresh(6, 1);
  Event_Wait(60);
  Event_ShowMessageAndWait(6, 0);
  Event_Wait(60);
  Actor_FaceActorById(27, 6, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(27, 0);
  Event_Wait(60);
  Actor_ShowEmoteById(28, 258, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(28, 0);
  Event_Wait(60);
  Actor_SetModeById(27, 3);
  Event_Wait(60);
  Event_ShowMessageAndWait(27, 0);
  Actor_WalkToAndWait(27, 504, 328);
  Actor_FaceActorById(gMainRoutineWork.currentOwner, 27, 0);
  Actor_FaceActorById(5, 27, 0);
  Actor_FaceActorById(6, 27, 0);
  Actor_FaceActorById(28, 27, 0);
  Actor_WalkToAndWait(27, 456, 328);
  Actor_FaceActorById(gMainRoutineWork.currentOwner, 27, 0);
  Actor_FaceActorById(5, 27, 0);
  Actor_FaceActorById(6, 27, 0);
  Actor_FaceActorById(28, 27, 0);
  Actor_WalkToAndWait(27, 424, 472);
  Actor_FaceActorById(gMainRoutineWork.currentOwner, 27, 0);
  Actor_FaceActorById(5, 27, 0);
  Actor_FaceActorById(6, 27, 0);
  Actor_FaceActorById(28, 27, 0);
  Event_Wait(60);
  Actor_SetHorizontalPositionWithTerrain(27, 0, 0);
  Event_Wait(120);
  Actor_Jump(5, 4, 20);
  Actor_Jump(5, 4, 20);
  Event_Wait(30);
  Actor_ShowEmoteById(5, 259, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(5, 0);
  Event_Wait(60);
  Actor_SetModeById(6, 3);
  Event_Wait(60);
  Event_ShowMessageAndWait(6, 0);
  Event_Wait(60);
  Actor_FaceActorById(28, gMainRoutineWork.currentOwner, 0);
  Event_Wait(60);
  Actor_ArmCallback(28, FACING_SOUTH, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(28, 0);
  Event_Wait(60);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Event_Wait(120);
  Actor_JoinLeader(5);
  Actor_JoinLeader(6);
  Actor_JoinLeader(28);
  Import_EventEnd();
  GameFlag_SetBit(0x84b);
}

#define DEFINE_DAILA_CLEAR_OBSTACLE(name, n1, n2, n3, n4, n5)                  \
  void name(void) {                                                            \
    Import_EventStart();                                                       \
    Event_ResolvePendingActions(0);                                            \
    MapGrid_CopyCellAttributes(n1, n2, 1, 1, n3, n2);                          \
    Actor_SetHorizontalPositionWithTerrain(n4, 0, 0);                          \
    GameFlag_SetBit(n5);                                                       \
    Import_EventEnd();                                                         \
  }

DEFINE_DAILA_CLEAR_OBSTACLE(Func_02000924, 10, 19, 11, 22, 0x210)

DEFINE_DAILA_CLEAR_OBSTACLE(Func_02000960, 25, 20, 26, 23, 0x211)

DEFINE_DAILA_CLEAR_OBSTACLE(Func_020009a0, 28, 23, 29, 24, 0x212)

DEFINE_DAILA_CLEAR_OBSTACLE(Func_020009e0, 17, 11, 18, 25, 0x213)

DEFINE_DAILA_CLEAR_OBSTACLE(Func_02000a20, 30, 17, 31, 26, 0x214)

DEFINE_DAILA_CLEAR_OBSTACLE(Func_02000a5c, 16, 23, 17, 31, 0x215)

void Actor_SetWideSprite(s32 actor, s32 flag);

DEFINE_SCENE_RAISE_HELD_ACTOR(Scene_RaiseHeldActor, Func_02000d58,
                              gData_02000d80)

s32 Scene_Initialize(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  if (gMainRoutineWork.scene == (s32)&SceneId_Daila1)
    Func_02000d68(12, 0);
  else if (gMainRoutineWork.scene == (s32)&SceneId_Daila2)
    Func_02000d50(gData_02000d80);
  Actor_SetSpritePriority(21, 1);
  if (gMainRoutineWork.scene == (s32)&SceneId_Daila2 &&
      GameFlag_Test(0x300) == 0)
    Actor_SetWideSprite(66, 0);
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Daila2) {
    if (GameFlag_Test(0x210))
      MapGrid_CopyCellAttributes(10, 19, 1, 1, 11, 19);
    if (GameFlag_Test(0x211))
      MapGrid_CopyCellAttributes(25, 20, 1, 1, 26, 20);
    if (GameFlag_Test(0x212))
      MapGrid_CopyCellAttributes(28, 23, 1, 1, 29, 23);
    if (GameFlag_Test(0x213))
      MapGrid_CopyCellAttributes(17, 11, 1, 1, 18, 11);
    if (GameFlag_Test(0x214))
      MapGrid_CopyCellAttributes(30, 17, 1, 1, 31, 17);
    if (GameFlag_Test(0x215))
      MapGrid_CopyCellAttributes(16, 23, 1, 1, 17, 23);
  }
  return 0;
}

void Func_02000c30(void) { Field_TryJumpForward(); }

// clang-format off
s16 gData_02000d80[] = {
    22, 534, 23, 535, 24, 536, 25, 537,
    26, 538, 29, 539, -1, 0,
};

s32 gData_02000d9c[] = {
    22, 30, 131, 39, 0, (s32)0x80010000,
    23, 9, -0x4000, 23, 10, 0x2000,
    0, 2, 23, 9, 0x2000, 23,
    10, -4096, 0, 2, 23, 9,
    0x1000, 23, 10, -2048, 0, 2,
    23, 9, 0x1000, 23, 10, -2048,
    0, 40, 23, 9, -0x4000, 23,
    10, 0x2000, 0, 2, 23, 9,
    0x2000, 23, 10, -4096, 0, 2,
    23, 9, 0x1000, 23, 10, -2048,
    0, 2, 23, 9, 0x1000, 23,
    10, -2048, 0, 5, 23, 9,
    -0x4000, 23, 10, 0x2000, 0, 2,
    23, 9, 0x2000, 23, 10, -4096,
    0, 2, 23, 9, 0x1000, 23,
    10, -2048, 0, 2, 23, 9,
    0x1000, 23, 10, -2048, 0, 30,
    13, 0xffff, (s32)0xc0010000, 17,
};

u32 gData_02000f44[] = {
    22, 30, 129, 22, 16, 0x3333,
    22, 21, 0x1999, 22, 15, 0x6666,
    39, 0, 0x80010000, 22, 6, 0x40000,
    0, 30, 22, 6, 0x40000, 0,
    60, 13, 0xffff, 0xc0010000, 17,
};

struct SceneEntrance gDailaEntrances[] = {
    {0, -1, 120, 0, 338, 0x4000, 0, -1, -1, -1, -1, 0},
    {10, -1, 368, 0, 224, 0x4000, 0, -1, -1, -1, -1, 0},
    {20, -1, 80, 0, 352, 0x4000, 0, -1, -1, -1, -1, 0},
    {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

s32 gData_02001018[] = {
    10, 0x1010100b, -1, 0x1020200b, -1, 0x1030300b,
    -1, 0x1040400b, -1, 0x1050500b, -1, 0x1060600b,
    -1, 0x1070100c, -1, 0x10802002, -1, 0x10903002,
    -1, 511,
};

s32 gData_02001068[] = {
    13, 0x1010100b, -1, 0x1020200b, -1, 0x1030300b,
    -1, 0x1040400b, -1, 0x1050500b, -1, 0x1060600b,
    -1, 0x1070100c, -1, 0x10802002, -1, 0x10903002,
    -1, 511, 0xffff, 0, 0, 0,
    0, 0,
};
// clang-format on

struct ActorPlacement gDaila1ActorPlacements[] = {
    {115, CONDITION_ALWAYS, 2, PIXELS(40), 0, PIXELS(280), FACING_EAST, 1, 0},
    {116, CONDITION_ALWAYS, 3, PIXELS(264), 0, PIXELS(232), FACING_SOUTH, 1, 0},
    {114, CONDITION_ALWAYS, 3, PIXELS(88), 0, PIXELS(376), FACING_EAST, 1, 0},
    {143, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(312), FACING_WEST, 1, 0},
    {112, CONDITION_ALWAYS, (s32)gData_02000d9c, PIXELS(360), 0, PIXELS(344),
     FACING_SOUTH, 2, 0},
    {140, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(424), FACING_NORTH, 1, 0},
    {146, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(456), FACING_SOUTH, 1, 0},
    {147, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(456), FACING_SOUTH, 1, 0},
    {224, CONDITION_ALWAYS, 3, PIXELS(376), 0, PIXELS(472), FACING_SOUTH, 1, 0},
    {224, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(312), FACING_SOUTHEAST, 1,
     0},
    {187, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(184),
     FACING_SOUTH + FACING_STEP, 1, 0},
    {146, CONDITION_ALWAYS, (s32)gData_02000f44, PIXELS(584), 0, PIXELS(384),
     FACING_SOUTH, 2, 0},
    {147, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(328), FACING_SOUTH, 1, 0},
    {110, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(424), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gDaila2ActorPlacements[] = {
    {115, CONDITION_ALWAYS, 2, PIXELS(40), 0, PIXELS(280), FACING_EAST, 1, 0},
    {116, CONDITION_ALWAYS, 3, PIXELS(264), 0, PIXELS(232), FACING_SOUTH, 1, 0},
    {114, CONDITION_ALWAYS, 3, PIXELS(88), 0, PIXELS(376), FACING_EAST, 1, 0},
    {143, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(312), FACING_WEST, 1, 0},
    {139, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(344), FACING_SOUTH, 0, 0},
    {140, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(424), FACING_NORTH, 1, 0},
    {146, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(456), FACING_EAST, 1, 0},
    {147, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(456), FACING_WEST, 1, 0},
    {224, CONDITION_ALWAYS, 3, PIXELS(376), 0, PIXELS(472), FACING_SOUTH, 1, 0},
    {224, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(312), FACING_SOUTHEAST, 1,
     0},
    {187, CONDITION_ALWAYS, 3, PIXELS(120), 0, PIXELS(216), FACING_WEST, 1, 0},
    {146, CONDITION_ALWAYS, (s32)gData_02000f44, PIXELS(584), 0, PIXELS(384),
     FACING_SOUTH, 2, 0},
    {147, CONDITION_ALWAYS, 1, PIXELS(584), 0, PIXELS(344), FACING_WEST, 1, 0},
    {110, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(424), FACING_SOUTH, 1, 0},
    {290, 0x210, 1, PIXELS(184), 0, PIXELS(312), FACING_SOUTH, 2, 0},
    {290, 0x211, 1, PIXELS(424), 0, PIXELS(328), FACING_SOUTH, 2, 0},
    {290, 0x212, 1, PIXELS(472), 0, PIXELS(376), FACING_SOUTH, 2, 0},
    {290, 0x213, 1, PIXELS(296), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {290, 0x214, 1, PIXELS(504), 0, PIXELS(280), FACING_SOUTH, 2, 0},
    {57, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {290, 0x215, 1, PIXELS(280), 0, PIXELS(376), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gDaila1Triggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 6, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(Daila_TalkBeenBefore)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(Daila_TalkIndraMoved)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgFieldISawABrightLightIn)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS,
     MSGNO(MsgFieldThankGoodnessForThatMountainRange)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS,
     MSGNO(MsgFieldOhhhThatWaveKnockedMeOver)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS,
     MSGNO(MsgFieldThatAncientTowerToTheEast)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgFieldRikiAndTaviSureAreOut)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgFieldIWonderIfThoseTwoAre)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS,
     MSGNO(MsgFieldOurVillageSurvivedTheWaveBut)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(Daila_TalkBoat)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgFieldThatTidalWaveLeftALot)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS,
     MSGNO(MsgFieldYaaayPuddlesInTheVillageYaaay)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgFieldBleahIFellInAPuddle)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, EVENT(Daila_AskAfterTaviAndRiki)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgFieldThatWaveTurnedThisPlaceInto)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgFieldThatMustHaveBeenOneSerious)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgFieldIThoughtISawSomeLights)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgFieldTheSeaGodMustHaveSaved)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgFieldMyHipsKillingMeAndAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgFieldICantBelieveThoseChildrenPlay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgFieldTheShrineOfTheSeaGod)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgFieldICantBelieveThoseLosersWere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgFieldThatWaveWasBrutalItSmashed)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgFieldWithAllTheBoatsGoneWe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgFieldWithAllThisSaltWaterOn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgFieldYayWeNeverGetPuddlesLike)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgFieldBleccchThisWatersSaaalty)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgFieldImSureThoseTwoWentTo)},
    {0x1f3, 200, CONDITION_ALWAYS, 0x00400000 | MSGNO(MsgDailaWellSeawater)},
    {TRIGGERS_END},
};

struct Trigger gDaila2Triggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 6, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(Func_020003a0)},
    {0x4602, 30, CONDITION_ALWAYS, EVENT(Func_02000c30)},
    {0x602, 31, CONDITION_ALWAYS, EVENT(Func_02000c30)},
    {0x8602, 32, CONDITION_ALWAYS, EVENT(Func_02000c30)},
    {0xc602, 33, CONDITION_ALWAYS, EVENT(Func_02000c30)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgFieldAfterThatWaveHitIWas)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgFieldSureAreALotOfTravelers)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS,
     MSGNO(MsgFieldThosePuddlesOfSeaWaterThe)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgFieldAhTheSeaIsCalmAgain)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(Daila_TalkHip)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS,
     MSGNO(MsgFieldIveAlwaysHeardThatTheShrine)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS,
     MSGNO(MsgFieldTheDreadPirateBriggsWasCaptured)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS,
     MSGNO(MsgFieldThoseTwoLosersAreBackHome)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS,
     MSGNO(MsgFieldIThoughtISawSomethingWhen)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgFieldSoIHearTheyCaughtThe)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS,
     MSGNO(MsgFieldItWasCreepyRightBeforeThe)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS,
     MSGNO(MsgFieldThePuddlesAreGoneAndNow)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgFieldIGotHurtWhenIWas)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, EVENT(Daila_TaviAndRikiAreHome)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgFieldThankOurLuckyStarsOurLittle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgFieldIsThatGuyStillMeetingWith)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgFieldThosePuddlesStankIThoughtId)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgFieldImGladEverythingIsBackTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgFieldNobodyBelievesAWaveCouldBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgFieldIdSureLikeToSeeWhat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgFieldIWonderWhatHappenedToBriggs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgFieldTaviAndRikiMustHaveBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgFieldItsProbablyNothingButJunkAnd)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgFieldTheSeasArePeacefulAndFree)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgFieldBeforeTheWaveHitTheTide)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgFieldIHopeSomeOfTheLittle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgFieldICaughtMyFootOnSomething)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgDailaTaviRikiPunishment)},
    {0x50008a05, 60, 0x210, EVENT(Func_02000924)},
    {0x50008a05, 61, 0x211, EVENT(Func_020009e0)},
    {0x50008a05, 62, 0x212, EVENT(Func_02000960)},
    {0x50008a05, 63, 0x213, EVENT(Func_020009a0)},
    {0x50008a05, 64, 0x214, EVENT(Func_02000a20)},
    {0x50008a05, 65, 0x215, EVENT(Func_02000a5c)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 22, 0x216, EVENT(Scene_RaiseHeldActor)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 23, 0x217, EVENT(Scene_RaiseHeldActor)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 24, 0x218, EVENT(Scene_RaiseHeldActor)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 25, 0x219, EVENT(Scene_RaiseHeldActor)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 26, 0x21a, EVENT(Scene_RaiseHeldActor)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 29, 0x21b, EVENT(Scene_RaiseHeldActor)},
    {0x50008805, 100, 0x300, EVENT(Func_02000118)},
    {0x1f3, 200, CONDITION_ALWAYS, 0x00400000 | MSGNO(MsgDailaWellSeawater)},
    {TRIGGERS_END},
};
