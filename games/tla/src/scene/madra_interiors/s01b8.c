#include "madra_interiors.h"
#include "scene_tables.h"

void Func_020001b8(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->angle;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02001f00(actor);
  } else {
    if (Event_GetFlag(0x8ff))
      FieldEvent_SetMessageId((s32)MsgMadraInteriors149);
    else if (Event_GetFlag(0x8ab))
      FieldEvent_SetMessageId((s32)MsgMadraInteriors81);
    else
      FieldEvent_SetMessageId((s32)MsgMadraInteriors33);
    Event_ShowMessage(actor, 0);
  }
}

void Func_0200023c(void) {
  Func_02001d50();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceActor(14, 4, 0);
  FieldActor_Jump(14, 6, 20);
  Actor_FaceActor(4, 14, 0);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors18);
  Event_ShowMessage(14, 0);
  ActorTurn(14, FACING_SOUTHEAST, 0);
  Func_02001d58();
}

void Func_0200028c(void) {
  s32 msg;

  if (Event_GetFlag(0x98a)) {
    if (Event_GetFlag(0x310))
      msg = (s32)MsgMadraInteriors111;
    else
      msg = (s32)MsgMadraInteriors118;
    FieldEvent_SetMessageId(msg);
    Event_ShowMessage(10, 0);
  } else {
    if (Event_GetFlag(0x210)) {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors116);
      Event_ShowMessage(10, 0);
    } else {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors106);
      Event_ShowMessage(10, 0);
    }
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 346) = 1;
  }
}

void Func_02000308(void) {
  if (Event_GetFlag(0x98a)) {
    if (Event_GetFlag(0x310)) {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors112);
      Event_ShowMessage(11, 0);
    } else {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors119);
      Event_ShowMessage(11, 0);
    }
  } else if (Event_GetFlag(0x210)) {
    FieldEvent_SetMessageId((s32)MsgMadraInteriors117);
    Event_ShowMessage(11, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgMadraInteriors107);
    Event_ShowMessage(11, 0);
  }
}

void Func_02000368(void) {
  if (Event_GetFlag(0x98a)) {
    if (Event_GetFlag(0x310)) {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors113);
      Event_ShowMessage(10, 0);
    } else {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors120);
      Event_ShowMessage(10, 0);
    }
  } else {
    s32 flag = Event_GetFlag(0x210);
    s32 msg = (s32)MsgMadraInteriors108;

    if (flag) {
      FieldEvent_SetMessageId(msg);
      Event_ShowMessage(10, 0);
    } else {
      FieldEvent_SetMessageId(msg);
      Event_ShowMessage(10, 0);
    }
  }
}

void Func_020003c8(void) {
  if (Event_GetFlag(0x98a)) {
    if (Event_GetFlag(0x310)) {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors114);
      Event_ShowMessage(11, 0);
    } else {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors121);
      Event_ShowMessage(11, 0);
    }
  } else {
    s32 flag = Event_GetFlag(0x210);
    s32 msg = (s32)MsgMadraInteriors109;

    if (flag) {
      FieldEvent_SetMessageId(msg);
      Event_ShowMessage(11, 0);
    } else {
      FieldEvent_SetMessageId(msg);
      Event_ShowMessage(11, 0);
    }
  }
}

void Func_02000428(void) {
  if (!FlagCheck(0x211)) {
    Event_SetFlag(0x211);
    FieldEvent_SetMessageId((s32)MsgUhhhnnnUhhhnnn2);
    Actor_WalkByAndWait(14, -12, 0);
    FieldEvent_Wait(20);
    Actor_RunRepeatedMotion(14, 2);
    FieldEvent_Wait(20);
    Event_ShowMessage(14, 0);
    FieldActor_ShowEmote(14, 265, 30);
    FieldEvent_Wait(20);
    Actor_WalkByAndWait(14, 12, 0);
    Actor_FaceActor(14, 4, 0);
    Event_ShowMessage(14, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgMadraInteriors97);
    Actor_FaceActor(14, 4, 0);
    Event_ShowMessage(14, 0);
  }
}

const u8 *Func_020004c4(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov659Room2)
    return gData_02002c9c;
  if (gMainRoutineWork.entrance == 20)
    return gData_02002bb8;
  if (Event_GetFlag(0x8ff))
    return gData_0200308c;
  if (Event_GetFlag(0x8ab))
    return gData_02002da4;
  return gData_020028a0;
}

void Func_02000534(void) {
  if (Event_GetFlag(0x8ab)) {
    ActorPlace(26, PIXELS(888), PIXELS(8));
  } else {
    Actor_SetItemIcon(Actor_Get(26), 242);
    Actor_SetSingleChildFlags(Actor_Get(26), 0);
  }
}
