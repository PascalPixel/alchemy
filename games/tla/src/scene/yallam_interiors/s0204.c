#include "yallam_interiors.h"

u8 *Func_02000204(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov677Room1)
    return gData_02000ea8;
  if (scene == (s32)&SceneId_Ov677Room2)
    return gData_02000fe0;
  return gData_02000e84;
}

void Func_02000244(void) {
  s32 msg;

  Func_020009b0();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgYallamInteriors10;
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
  Func_020009b8();
}

void Func_0200029c(s32 actor) {
  u16 facing = ((struct YallamInteriorsObject *)Actor_GetById(gMainRoutineWork.currentOwner))->facing;


  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000a68(20, actor);
  } else {
    Func_020009b0();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgYallamInteriors21);
    Event_ShowMessageAndWait(actor, 0);
    Func_020009b8();
  }
}

void Func_020002fc(s32 actor) {
  u16 facing = ((struct YallamInteriorsObject *)Actor_GetById(gMainRoutineWork.currentOwner))->facing;


  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000a68(21, actor);
  } else {
    Func_020009b0();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgYallamInteriors23);
    Event_ShowMessageAndWait(actor, 0);
    Func_020009b8();
  }
}

void Func_0200035c(s32 actor) {
  u16 facing = ((struct YallamInteriorsObject *)Actor_GetById(gMainRoutineWork.currentOwner))->facing;


  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000a68(22, actor);
  } else {
    Func_020009b0();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgYallamInteriors25);
    Event_ShowMessageAndWait(actor, 0);
    Func_020009b8();
  }
}

void Func_020003bc(s32 actor) {
  u16 facing = ((struct YallamInteriorsObject *)Actor_GetById(gMainRoutineWork.currentOwner))->facing;


  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000a78(7, actor);
    GameFlag_ClearBit(0x301);
  } else {
    Func_020009b0();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgYallamInteriors27);
    Event_ShowMessageAndWait(actor, 0);
    Func_020009b8();
  }
}

void Func_02000424(s32 actor) {
  u16 facing = ((struct YallamInteriorsObject *)Actor_GetById(gMainRoutineWork.currentOwner))->facing;


  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000a70(actor);
  } else {
    Func_020009b0();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgMikasallaInteriors27);
    Event_ShowMessageAndWait(actor, 0);
    Func_020009b8();
  }
}

void Func_02000480(void) {
  s32 msg;

  Func_020009b0();
  Event_ResolvePendingActions(0);
  Actor_StartRepeatedMotion(14, 2);
  msg = (s32)MsgYallamInteriors31;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(14, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(14, 0);
  Func_020009b8();
}
