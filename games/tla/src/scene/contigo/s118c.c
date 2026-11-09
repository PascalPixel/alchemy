#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "event_work.h"
#include "event_inline.h"

struct ContigoObject {
  u8 unk00[6];
  u16 angle;
  u8 unk08[0x5b];
  u8 unk63;
};

void Func_0200406c(void);
void Func_02004074(void);

void Func_0200118c(void) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct ContigoObject *object = ((struct ContigoObject *)Actor_GetById(15));
  s16 angle = object->angle;

  Func_0200406c();
  Event_ResolvePendingActions(0);
  object->unk63 = 1;
  if (event->unk166 == 0) {
    Actor_SetAngleToward(15, gMainRoutineWork.currentOwner, 0);
    if (GameFlag_Test(0x9b2) != 0)
      Event_SetMessageId((s32)MsgContigo143);
    else
      Event_SetMessageId((s32)MsgContigo42);
  } else {
    if (GameFlag_Test(0x9b2) != 0)
      Event_SetMessageId((s32)MsgContigo149);
    else
      Event_SetMessageId((s32)MsgContigo50);
  }
  Event_ShowMessageAndWait(15, 0);
  if (event->unk166 == 0)
    object->angle = angle;
  object->unk63 = 0;
  Func_02004074();
}
