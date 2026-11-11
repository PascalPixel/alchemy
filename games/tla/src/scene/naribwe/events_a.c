#include "local.h"
#include "event_inline.h"

void Func_02000088(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  u8 *flag;

  flag = work + 0xd38;
  *flag = 1;
  if (gMainRoutineWork.entrance != 10)
    *flag = 0;
}

const u8 *Func_020000b8(void) {
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
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x2000) &
            0xc000) == 0x8000) {
    Func_020005e4(16, actor);
    return;
  }
  Func_0200054c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgNaribwe21);
  Event_ShowMessageAndWait(actor, 0);
  Func_02000554();
}

void Func_02000250(s32 actor) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x2000) &
            0xc000) == 0x8000) {
    Func_020005e4(16, actor);
    return;
  }
  Func_0200054c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgNaribwe43);
  Event_ShowMessageAndWait(actor, 0);
  Func_02000554();
}

void Func_020002b0(s32 actor) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x2000) &
            0xc000) == 0xc000) {
    Func_020005ec(actor);
    return;
  }
  Func_0200054c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgNaribwe23);
  Event_ShowMessageAndWait(26, 0);
  Func_02000554();
}

void Func_0200030c(s32 actor) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x2000) &
            0xc000) == 0xc000) {
    Func_020005ec(actor);
    return;
  }
  Func_0200054c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgNaribwe45);
  Event_ShowMessageAndWait(26, 0);
  Func_02000554();
}

void Func_02000368(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 index;
  u32 id;

  Func_0200054c();
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
  /* FAKEMATCH(dead): the empty once-only blocks on each side of the call keep
   * the entrance's subtraction after it, where sched2 would rank it above the
   * call; as no first block 2 instructions differ, as no second 4. */
  do {
  } while (0);
  Sound_Play(158);
  do {
  } while (0);
  index -= 4;
  MapGrid_AnimateCells(gData_0200064c[index].script, gData_0200064c[index].x,
                       gData_0200064c[index].z);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Import_EventSetExitRequest(*(s16 *)work);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02000554();
}
