/// @file
/// The scene code of overlay 0x66d, in ROM order.

#include "naribwe/local.h"
#include "field.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const u8 *Scene_GetPlacements(void) {
  if (gMainRoutineWork.entrance == 10)
    return gData_020008c4;
  if (GameFlag_Test(0x8ff))
    return gData_02000954;
  return gData_020006cc;
}

#include "event_inline.h"

void Func_02000088(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  u8 *flag;

  flag = work + 0xd38;
  *flag = 1;
  if (gMainRoutineWork.entrance != 10)
    *flag = 0;
}

const u8 *Scene_GetEvents(void) {
  if (GameFlag_Test(0x8ff))
    return gData_02000cfc;
  return gData_02000b4c;
}

void Func_020000d8(void) {
  s32 msg;

  if (Event_GetFlag(0x85d) == 0) {
    msg = (s32)&MsgNaribwe02;
    Event_SetMessageId(msg);
    FieldMessage_ShowAtActor(14, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetMessageId(msg + 1);
      GameFlag_SetBit(0x85d);
    } else {
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessageAndWait(14, 0);
  } else {
    Event_SetMessageId((s32)&MsgNaribwe03);
    Event_ShowMessageAndWait(14, 0);
  }
}

void Func_02000140(void) {
  s32 msg;

  if (GameFlag_Test(0x85e) == 0) {
    msg = (s32)&MsgNaribwe07;
    Event_SetMessageId(msg);
    FieldMessage_ShowAtActor(18, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetMessageId(msg + 1);
      GameFlag_SetBit(0x85d);
    } else {
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessageAndWait(18, 0);
  } else {
    Event_SetMessageId((s32)&MsgNaribwe08);
    Event_ShowMessageAndWait(14, 0);
  }
}

void Func_020001a8(void) {
  s32 msg = (s32)&MsgNaribwe09;

  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(19, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(19, 0);
}

void Func_020001f0(s32 actor) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0x8000) {
    Func_020005e4(16, actor);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgNaribwe21);
  Event_ShowMessageAndWait(actor, 0);
  Event_End();
}

void Func_02000250(s32 actor) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0x8000) {
    Func_020005e4(16, actor);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgNaribwe43);
  Event_ShowMessageAndWait(actor, 0);
  Event_End();
}

void Func_020002b0(s32 actor) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0xc000) {
    Func_020005ec(actor);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgNaribwe23);
  Event_ShowMessageAndWait(26, 0);
  Event_End();
}

void Func_0200030c(s32 actor) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x2000) &
            0xc000) == 0xc000) {
    Func_020005ec(actor);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgNaribwe45);
  Event_ShowMessageAndWait(26, 0);
  Event_End();
}

void Func_02000368(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 index;
  u32 id;

  Event_Start();
  Event_ResolvePendingActions(0);
  for (id = 8; id <= 63; id++) {
    u8 *actor = (u8 *)Actor_GetById(id);

    if (actor != NULL) {
      u8 *flags = &actor[85];
      s32 zero = 0;

      *flags = zero;
    }
  }
  work += 0x154;
  index = *(s16 *)work;
  Sound_Play(158);
  index -= 4;
  MapGrid_AnimateCells(gData_0200064c[index].script, gData_0200064c[index].x,
                       gData_0200064c[index].z);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Import_EventSetExitRequest(*(s16 *)work);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

void Func_02000400(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  GameFlag_SetBit(0x201);
  Event_End();
}

void Func_0200041c(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  work[0xd31] = 0;
  work[0xd32] = 24;
  work[0xd33] = 25;
}

void Func_02000448(void) {
  Actor_SetHorizontalPositionWithTerrain(27, 0, 0);
  TreasureSpot_SetPosition(65, 0x680000, 0x680000);
}

void Func_02000464(void) {
  ObjectEvent_PlaceOnGround(65, 0, 0);
  Actor_SetPosition(27, PIXELS(104), PIXELS(104));
}

s32 Scene_Initialize(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  Actor_SetSingleChildFlags(Actor_GetById(9), 0);
  Actor_SetSingleChildFlags(Actor_GetById(10), 0);
  Actor_SetSingleChildFlags(Actor_GetById(23), 0);
  Actor_GetById(23)->scaleY = 0x19999;
  Actor_GetById(27)->scaleX = 0x8000;
  Actor_GetById(27)->scaleY = 0x8000;
  Func_02000464();
  if (GameFlag_Test(0x201))
    Actor_SetHorizontalPositionWithTerrain(23, 0, 0);
  Func_020005cc();
  Func_020005d4(0, 0x202, 24, 25);
  Actor_SetSingleChildFlags(Actor_GetById(24), 0);
  Func_0200059c(25);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

