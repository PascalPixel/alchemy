/// @file
/// The scene code of overlay 0x651, in ROM order.

#include "shared/routines.h"
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

s32 *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Daila2)
    return gData_02001238;
  else
    return gData_020010d0;
}

void Func_0200009c(void) {
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

s32 *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Daila2)
    return gData_02001664;
  else
    return gData_02001490;
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

void Func_02000288(s32 keeper) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Import_ShopOpen(3, keeper);
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

void Func_02000340(s32 keeper) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Import_ShopOpen(3, keeper);
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
    Actor_ResetAndSetPosition(actor, ((s16 *)&leader->x)[1],
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

DEFINE_FUNC_02000924(Func_02000924, 10, 19, 11, 22, 0x210)

DEFINE_FUNC_02000924(Func_02000960, 25, 20, 26, 23, 0x211)

DEFINE_FUNC_02000924(Func_020009a0, 28, 23, 29, 24, 0x212)

DEFINE_FUNC_02000924(Func_020009e0, 17, 11, 18, 25, 0x213)

DEFINE_FUNC_02000924(Func_02000a20, 30, 17, 31, 26, 0x214)

DEFINE_FUNC_02000924(Func_02000a5c, 16, 23, 17, 31, 0x215)

void Actor_SetWideSprite(s32 actor, s32 flag);

DEFINE_FUNC_02000704(Func_02000a9c, Func_02000d58, gData_02000d80)

s32 Scene_Initialize(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
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

void Func_02000c30(void) { Func_02000d18(); }

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

s32 gData_020010d0[] = {
    (s32)0xffff0073,
    2,
    0x280000,
    0,
    0x1180000,
    0x10000,
    (s32)0xffff0074,
    3,
    0x1080000,
    0,
    0xe80000,
    0x14000,
    (s32)0xffff0072,
    3,
    0x580000,
    0,
    0x1780000,
    0x10000,
    (s32)0xffff008f,
    1,
    0x1080000,
    0,
    0x1380000,
    0x18000,
    (s32)0xffff0070,
    (s32)gData_02000d9c,
    0x1680000,
    0,
    0x1580000,
    0x24000,
    (s32)0xffff008c,
    1,
    0x2380000,
    0,
    0x1a80000,
    0x1c000,
    (s32)0xffff0092,
    1,
    0xf80000,
    0,
    0x1c80000,
    0x14000,
    (s32)0xffff0093,
    1,
    0x1080000,
    0,
    0x1c80000,
    0x14000,
    (s32)0xffff00e0,
    3,
    0x1780000,
    0,
    0x1d80000,
    0x14000,
    (s32)0xffff00e0,
    1,
    0x1880000,
    0,
    0x1380000,
    0x12000,
    (s32)0xffff00bb,
    1,
    0x880000,
    0,
    0xb80000,
    0x15000,
    (s32)0xffff0092,
    (s32)gData_02000f44,
    0x2480000,
    0,
    0x1800000,
    0x24000,
    (s32)0xffff0093,
    1,
    0x2380000,
    0,
    0x1480000,
    0x14000,
    (s32)0xffff006e,
    1,
    0x1380000,
    0,
    0x1a80000,
    0x14000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

s32 gData_02001238[] = {
    (s32)0xffff0073,
    2,
    0x280000,
    0,
    0x1180000,
    0x10000,
    (s32)0xffff0074,
    3,
    0x1080000,
    0,
    0xe80000,
    0x14000,
    (s32)0xffff0072,
    3,
    0x580000,
    0,
    0x1780000,
    0x10000,
    (s32)0xffff008f,
    1,
    0x1080000,
    0,
    0x1380000,
    0x18000,
    (s32)0xffff008b,
    1,
    0x1680000,
    0,
    0x1580000,
    0x4000,
    (s32)0xffff008c,
    1,
    0x2380000,
    0,
    0x1a80000,
    0x1c000,
    (s32)0xffff0092,
    1,
    0xf80000,
    0,
    0x1c80000,
    0x10000,
    (s32)0xffff0093,
    1,
    0x1080000,
    0,
    0x1c80000,
    0x18000,
    (s32)0xffff00e0,
    3,
    0x1780000,
    0,
    0x1d80000,
    0x14000,
    (s32)0xffff00e0,
    1,
    0x1880000,
    0,
    0x1380000,
    0x12000,
    (s32)0xffff00bb,
    3,
    0x780000,
    0,
    0xd80000,
    0x18000,
    (s32)0xffff0092,
    (s32)gData_02000f44,
    0x2480000,
    0,
    0x1800000,
    0x24000,
    (s32)0xffff0093,
    1,
    0x2480000,
    0,
    0x1580000,
    0x18000,
    (s32)0xffff006e,
    1,
    0x1380000,
    0,
    0x1a80000,
    0x14000,
    0x2100122,
    1,
    0xb80000,
    0,
    0x1380000,
    0x24000,
    0x2110122,
    1,
    0x1a80000,
    0,
    0x1480000,
    0x24000,
    0x2120122,
    1,
    0x1d80000,
    0,
    0x1780000,
    0x24000,
    0x2130122,
    1,
    0x1280000,
    0,
    0xb80000,
    0x24000,
    0x2140122,
    1,
    0x1f80000,
    0,
    0x1180000,
    0x24000,
    (s32)0xffff0039,
    1,
    0,
    0,
    0,
    0x24000,
    (s32)0xffff0038,
    1,
    0,
    0,
    0,
    0x24000,
    (s32)0xffff0005,
    1,
    0,
    0,
    0,
    0x24000,
    (s32)0xffff0006,
    1,
    0,
    0,
    0,
    0x24000,
    0x2150122,
    1,
    0x1180000,
    0,
    0x1780000,
    0x24000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

s32 gData_02001490[] = {
    2,
    (s32)0xffff0001,
    (s32)Func_0200009c,
    2,
    (s32)0xffff0002,
    (s32)Func_0200009c,
    2,
    (s32)0xffff0003,
    (s32)Func_0200009c,
    2,
    (s32)0xffff0004,
    (s32)Func_0200009c,
    2,
    (s32)0xffff0005,
    (s32)Func_0200009c,
    2,
    (s32)0xffff0006,
    (s32)Func_0200009c,
    1,
    (s32)0xffff0007,
    7,
    1,
    (s32)0xffff0008,
    8,
    1,
    (s32)0xffff0009,
    9,
    0,
    (s32)0xffff0008,
    (s32)Daila_TalkBeenBefore,
    0,
    (s32)0xffff0009,
    (s32)Daila_TalkIndraMoved,
    0,
    (s32)0xffff000a,
    MSGNO(MsgFieldISawABrightLightIn),
    0,
    (s32)0xffff000b,
    MSGNO(MsgFieldThankGoodnessForThatMountainRange),
    0,
    (s32)0xffff000c,
    MSGNO(MsgFieldOhhhThatWaveKnockedMeOver),
    0,
    (s32)0xffff000d,
    MSGNO(MsgFieldThatAncientTowerToTheEast),
    0,
    (s32)0xffff000e,
    MSGNO(MsgFieldRikiAndTaviSureAreOut),
    0,
    (s32)0xffff000f,
    MSGNO(MsgFieldIWonderIfThoseTwoAre),
    0,
    (s32)0xffff0010,
    MSGNO(MsgFieldOurVillageSurvivedTheWaveBut),
    0,
    (s32)0xffff0011,
    (s32)Daila_TalkBoat,
    0,
    (s32)0xffff0012,
    MSGNO(MsgFieldThatTidalWaveLeftALot),
    0,
    (s32)0xffff0013,
    MSGNO(MsgFieldYaaayPuddlesInTheVillageYaaay),
    0,
    (s32)0xffff0014,
    MSGNO(MsgFieldBleahIFellInAPuddle),
    0,
    (s32)0xffff0015,
    (s32)Func_02000288,
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgFieldThatWaveTurnedThisPlaceInto),
    0x8d15,
    (s32)0xffff0009,
    MSGNO(MsgFieldThatMustHaveBeenOneSerious),
    0x8d15,
    (s32)0xffff000a,
    MSGNO(MsgFieldIThoughtISawSomeLights),
    0x8d15,
    (s32)0xffff000b,
    MSGNO(MsgFieldTheSeaGodMustHaveSaved),
    0x8d15,
    (s32)0xffff000c,
    MSGNO(MsgFieldMyHipsKillingMeAndAll),
    0x8d15,
    (s32)0xffff000d,
    MSGNO(MsgFieldICantBelieveThoseChildrenPlay),
    0x8d15,
    (s32)0xffff000e,
    MSGNO(MsgFieldTheShrineOfTheSeaGod),
    0x8d15,
    (s32)0xffff000f,
    MSGNO(MsgFieldICantBelieveThoseLosersWere),
    0x8d15,
    (s32)0xffff0010,
    MSGNO(MsgFieldThatWaveWasBrutalItSmashed),
    0x8d15,
    (s32)0xffff0011,
    MSGNO(MsgFieldWithAllTheBoatsGoneWe),
    0x8d15,
    (s32)0xffff0012,
    MSGNO(MsgFieldWithAllThisSaltWaterOn),
    0x8d15,
    (s32)0xffff0013,
    MSGNO(MsgFieldYayWeNeverGetPuddlesLike),
    0x8d15,
    (s32)0xffff0014,
    MSGNO(MsgFieldBleccchThisWatersSaaalty),
    0x8d15,
    (s32)0xffff0015,
    MSGNO(MsgFieldImSureThoseTwoWentTo),
    499,
    (s32)0xffff00c8,
    0x00400000 | MSGNO(MsgDailaWellSeawater),
    -1,
    0,
    0,
};

s32 gData_02001664[] = {
    2,
    (s32)0xffff0001,
    (s32)Func_0200009c,
    2,
    (s32)0xffff0002,
    (s32)Func_0200009c,
    2,
    (s32)0xffff0003,
    (s32)Func_0200009c,
    2,
    (s32)0xffff0004,
    (s32)Func_0200009c,
    2,
    (s32)0xffff0005,
    (s32)Func_0200009c,
    2,
    (s32)0xffff0006,
    (s32)Func_0200009c,
    1,
    (s32)0xffff0007,
    7,
    1,
    (s32)0xffff0008,
    8,
    1,
    (s32)0xffff0009,
    9,
    2,
    (s32)0xffff0014,
    (s32)Func_020003a0,
    0x4602,
    (s32)0xffff001e,
    (s32)Func_02000c30,
    0x602,
    (s32)0xffff001f,
    (s32)Func_02000c30,
    0x8602,
    (s32)0xffff0020,
    (s32)Func_02000c30,
    0xc602,
    (s32)0xffff0021,
    (s32)Func_02000c30,
    0,
    (s32)0xffff0008,
    MSGNO(MsgFieldAfterThatWaveHitIWas),
    0,
    (s32)0xffff0009,
    MSGNO(MsgFieldSureAreALotOfTravelers),
    0,
    (s32)0xffff000a,
    MSGNO(MsgFieldThosePuddlesOfSeaWaterThe),
    0,
    (s32)0xffff000b,
    MSGNO(MsgFieldAhTheSeaIsCalmAgain),
    0,
    (s32)0xffff000c,
    (s32)Daila_TalkHip,
    0,
    (s32)0xffff000d,
    MSGNO(MsgFieldIveAlwaysHeardThatTheShrine),
    0,
    (s32)0xffff000e,
    MSGNO(MsgFieldTheDreadPirateBriggsWasCaptured),
    0,
    (s32)0xffff000f,
    MSGNO(MsgFieldThoseTwoLosersAreBackHome),
    0,
    (s32)0xffff0010,
    MSGNO(MsgFieldIThoughtISawSomethingWhen),
    0,
    (s32)0xffff0011,
    MSGNO(MsgFieldSoIHearTheyCaughtThe),
    0,
    (s32)0xffff0012,
    MSGNO(MsgFieldItWasCreepyRightBeforeThe),
    0,
    (s32)0xffff0013,
    MSGNO(MsgFieldThePuddlesAreGoneAndNow),
    0,
    (s32)0xffff0014,
    MSGNO(MsgFieldIGotHurtWhenIWas),
    0,
    (s32)0xffff0015,
    (s32)Func_02000340,
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgFieldThankOurLuckyStarsOurLittle),
    0x8d15,
    (s32)0xffff0009,
    MSGNO(MsgFieldIsThatGuyStillMeetingWith),
    0x8d15,
    (s32)0xffff000a,
    MSGNO(MsgFieldThosePuddlesStankIThoughtId),
    0x8d15,
    (s32)0xffff000b,
    MSGNO(MsgFieldImGladEverythingIsBackTo),
    0x8d15,
    (s32)0xffff000c,
    MSGNO(MsgFieldNobodyBelievesAWaveCouldBe),
    0x8d15,
    (s32)0xffff000d,
    MSGNO(MsgFieldIdSureLikeToSeeWhat),
    0x8d15,
    (s32)0xffff000e,
    MSGNO(MsgFieldIWonderWhatHappenedToBriggs),
    0x8d15,
    (s32)0xffff000f,
    MSGNO(MsgFieldTaviAndRikiMustHaveBeen),
    0x8d15,
    (s32)0xffff0010,
    MSGNO(MsgFieldItsProbablyNothingButJunkAnd),
    0x8d15,
    (s32)0xffff0011,
    MSGNO(MsgFieldTheSeasArePeacefulAndFree),
    0x8d15,
    (s32)0xffff0012,
    MSGNO(MsgFieldBeforeTheWaveHitTheTide),
    0x8d15,
    (s32)0xffff0013,
    MSGNO(MsgFieldIHopeSomeOfTheLittle),
    0x8d15,
    (s32)0xffff0014,
    MSGNO(MsgFieldICaughtMyFootOnSomething),
    0x8d15,
    (s32)0xffff0015,
    MSGNO(MsgDailaTaviRikiPunishment),
    0x50008a05,
    0x210003c,
    (s32)Func_02000924,
    0x50008a05,
    0x211003d,
    (s32)Func_020009e0,
    0x50008a05,
    0x212003e,
    (s32)Func_02000960,
    0x50008a05,
    0x213003f,
    (s32)Func_020009a0,
    0x50008a05,
    0x2140040,
    (s32)Func_02000a20,
    0x50008a05,
    0x2150041,
    (s32)Func_02000a5c,
    0x1815,
    0x2160016,
    (s32)Func_02000a9c,
    0x1815,
    0x2170017,
    (s32)Func_02000a9c,
    0x1815,
    0x2180018,
    (s32)Func_02000a9c,
    0x1815,
    0x2190019,
    (s32)Func_02000a9c,
    0x1815,
    0x21a001a,
    (s32)Func_02000a9c,
    0x1815,
    0x21b001d,
    (s32)Func_02000a9c,
    0x50008805,
    0x3000064,
    (s32)Func_02000118,
    499,
    (s32)0xffff00c8,
    0x00400000 | MSGNO(MsgDailaWellSeawater),
    -1,
    0,
    0,
};
