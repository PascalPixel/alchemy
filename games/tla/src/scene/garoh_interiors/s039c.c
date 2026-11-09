#include "garoh_interiors.h"

/* FAKEMATCH: the arguments are set up as for a call whose value is used (flags
 * before speaker); the opener is declared void in scene_call.h for the scenes
 * that match with that. */
#define Message_Open(speaker, flags)                                           \
  ((s32 (*)(s32, s32))FieldMessage_ShowAtActor)(speaker, flags)

void Func_02000318(s32 actor) {
  s32 message;

  Func_020007f4();
  Event_ResolvePendingActions(0);
  message = (s32)MsgGarohInteriors01;
  Event_SetMessageId(message);
  Message_Open(actor, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Event_SetMessageId(message + 2);
    Message_Open(actor, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
      Event_ShowMessageAndWait(actor, 0);
    } else {
      ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
      Event_ShowMessageAndWait(actor, 0);
    }
  } else {
    Event_ShowMessageAndWait(actor, 0);
  }
  Func_020007fc();
}

void Func_0200039c(void) {
  Func_020007f4();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x91b))
    Event_SetMessageId((s32)MsgGarohInteriors60);
  else
    Event_SetMessageId((s32)MsgGarohInteriors25);
  Event_ShowMessageAndWait(17, 0);
  Func_020007fc();
}

void Func_020003dc(void) {
  Func_020007f4();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x91b))
    Event_SetMessageId((s32)MsgGarohInteriors61);
  else
    Event_SetMessageId((s32)MsgGarohInteriors26);
  Event_ShowMessageAndWait(18, 0);
  Func_020007fc();
}

void Func_0200041c(void) {
  Func_020007f4();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x91b))
    Event_SetMessageId((s32)MsgGarohInteriors64);
  else
    Event_SetMessageId((s32)MsgGarohInteriors29);
  Event_ShowMessageAndWait(17, 0);
  Func_020007fc();
}

void Func_0200045c(void) {
  Func_020007f4();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x91b))
    Event_SetMessageId((s32)MsgGarohInteriors65);
  else
    Event_SetMessageId((s32)MsgGarohInteriors30);
  Event_ShowMessageAndWait(18, 0);
  Func_020007fc();
}

void Func_0200049c(void) {
  s32 message = (s32)MsgGarohInteriors47;

  Event_SetMessageId(message);
  FieldMessage_ShowAtActor(11, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(message + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(message + 2);
  }
  Event_ShowMessageAndWait(11, 0);
  GameFlag_SetBit(0x91a);
}

/// Returns the room's table, chosen by flag 0x91B, the scene and, in the
/// first room, the entrance.
const u8 *Func_020004f0(void) {
  s32 moved = GameFlag_Test(0x91b);
  struct MainRoutineWork *party = &gMainRoutineWork;
  s32 room1 = (s32)&SceneId_Ov660Room1;
  s32 scene;

  if (moved) {
    scene = party->scene;
    if (scene == room1)
      return gData_02000e44;
    if (scene == (s32)&SceneId_Ov660Room2)
      return gData_02000fe8;
  } else {
    scene = party->scene;
    if (scene == room1) {
      if (party->entrance == 99)
        return gData_02000e44;
      return gData_02000b80;
    }
    if (scene == (s32)&SceneId_Ov660Room2)
      return gData_02000d18;
  }
  if (scene == (s32)&SceneId_Ov660Room3)
    return gData_02000de4;
  return gData_02000b5c;
}
