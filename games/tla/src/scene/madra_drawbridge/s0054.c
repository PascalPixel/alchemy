#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_020002b8(void);
void Func_020002c0(void);
void Func_02000338(void);

void Func_02000054(void) {
  Func_020002b8();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgMadraGoingToOsenia);
  FieldMessage_ShowAtActor(8, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Motion_SetModeAndWaitAnimation(8, 3);
    Event_ShowMessageThenWait(8, 0, 2);
  } else {
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageThenWait(8, 0, 2);
  }
  Func_020002c0();
}
