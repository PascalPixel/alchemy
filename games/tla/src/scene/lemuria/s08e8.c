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
  s32 y = Actor_GetById(gMainRoutineWork.currentOwner)->y;


  if (y < 0x5a0000) {
    FieldMessage_Show((s32)MsgAndTheOthers, 1);
    Sound_Play(126);
    Func_02001bb4(1);
  } else {
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 346) = 1;
  }
}

/// Actor 30 offers a test of luck at the Spring of Lemuria; flag 0x908
/// records that the party took it.
void Func_02000934(void) {
  if (Event_GetFlag(0x908)) {
    Actor_SetAngleToward(30, gMainRoutineWork.currentOwner, 0);
    Actor_RefreshSelectorById(30);
    Event_SetMessageId((s32)MsgYouLookLike);
    Event_ShowMessageAndWait(30, 0);
    Actor_FaceDirectionAndWait(30, FACING_NORTH + FACING_STEP);
    return;
  }
  Func_02001bcc();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgDoYouWish);
  Func_080d47b4(30, 0x100, 30);
  Actor_SetAngleToward(30, gMainRoutineWork.currentOwner, 0);
  Actor_RefreshSelectorById(30);
  Event_Wait(5);
  FieldMessage_ShowAtActor(30, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0)) {
    Event_Wait(30);
    Event_ShowMessageThenWait(30, 0, 0);
  } else {
    Event_Wait(20);
    Motion_SetModeAndWaitAnimation(30, 3);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageThenWait(30, 0, 5);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 464, 1040);
    Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_NORTH);
    Event_Wait(5);
    Actor_SetSpeed(30, 0x13333, 0x9999);
    Actor_SetPositionAndReset(30, 440, 1048);
    GameFlag_SetBit(0x908);
  }
  Actor_FaceDirectionAndWait(30, FACING_NORTH + FACING_STEP);
  Func_02001bd4();
}
