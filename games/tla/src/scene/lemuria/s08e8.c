#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02001bb4(s32 value);

void Func_02001bcc(void);
void Func_02001bd4(void);

void Func_020008e8(void) {
  s32 y = Actor_Get(gMainRoutineWork.currentOwner)->y;


  if (y < 0x5a0000) {
    Message_ShowCentered((s32)MsgAndTheOthers, 1);
    Event_PlaySound(126);
    Func_02001bb4(1);
  } else {
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 346) = 1;
  }
}

/// Actor 30 offers a test of luck at the Spring of Lemuria; flag 0x908
/// records that the party took it.
void Func_02000934(void) {
  if (Event_GetFlag(0x908)) {
    Actor_FaceActor(30, gMainRoutineWork.currentOwner, 0);
    FieldActor_RefreshSelectorById(30);
    FieldEvent_SetMessageId((s32)MsgYouLookLike);
    Event_ShowMessage(30, 0);
    Actor_FaceDirectionAndWait(30, FACING_NORTH + FACING_STEP);
    return;
  }
  Func_02001bcc();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgDoYouWish);
  Actor_ShowEmote(30, 0x100, 30);
  Actor_FaceActor(30, gMainRoutineWork.currentOwner, 0);
  FieldActor_RefreshSelectorById(30);
  FieldEvent_Wait(5);
  Event_OpenMessage(30, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0)) {
    FieldEvent_Wait(30);
    FieldEvent_ShowMessageThenWait(30, 0, 0);
  } else {
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(30, 3);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    FieldEvent_ShowMessageThenWait(30, 0, 5);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 464, 1040);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_NORTH);
    FieldEvent_Wait(5);
    Actor_SetSpeed(30, 0x13333, 0x9999);
    Actor_WalkToAndWait(30, 440, 1048);
    Event_SetFlag(0x908);
  }
  Actor_FaceDirectionAndWait(30, FACING_NORTH + FACING_STEP);
  Func_02001bd4();
}
