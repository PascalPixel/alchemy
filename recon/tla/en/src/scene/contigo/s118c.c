#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "event_work.h"

struct ContigoObject {
  u8 unk00[6];
  u16 angle;
  u8 unk08[0x5b];
  u8 unk63;
};

struct ContigoObject *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Actor_SetAngleToward(s32 actor, s32 target, s32 mode);
void Func_0200406c(void);
void Func_02004074(void);
void Func_0200412c(s32 message);
void Func_02004144(s32 speaker, s32 flags);
void Func_02004204(s32 skip);

void Func_0200118c(void) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct ContigoObject *object = Actor_GetById(15);
  s16 angle = object->angle;

  Func_0200406c();
  Func_02004204(0);
  object->unk63 = 1;
  if (event->unk166 == 0) {
    Actor_SetAngleToward(15, gMainRoutineWork.currentOwner, 0);
    if (GameFlag_Test(SPLIT_CONSTANT(SPLIT_CONSTANT(0x900, 178))) != 0)
      Func_0200412c((s32)MsgContigo143);
    else
      Func_0200412c((s32)MsgContigo42);
  } else {
    if (GameFlag_Test(SPLIT_CONSTANT(SPLIT_CONSTANT(0x900, 178))) != 0)
      Func_0200412c((s32)MsgContigo149);
    else
      Func_0200412c((s32)MsgContigo50);
  }
  Func_02004144(15, 0);
  if (event->unk166 == 0)
    object->angle = angle;
  object->unk63 = 0;
  Func_02004074();
}
