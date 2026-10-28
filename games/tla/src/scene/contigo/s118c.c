#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "text/msg_ids.h"

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Actor_SetAngleToward(s32 actor, s32 target, s32 mode);
void Func_0200406c(void);
void Func_02004074(void);
void Func_0200412c(s32 message);
void Func_02004144(s32 speaker, s32 flags);
void Func_02004204(s32 skip);

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

void Func_0200118c(void) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *object = Actor_GetById(15);
  s16 angle = object->angle;

  Func_0200406c();
  Func_02004204(0);
  BYTE_AT(object, 99) = 1;
  if (*(s16 *)(event + 358) == 0) {
    Actor_SetAngleToward(15, gSequenceParentWork.currentOwner, 0);
    if (GameFlag_Test(0x9b2) != 0)
      Func_0200412c((s32)MsgContigo143);
    else
      Func_0200412c((s32)MsgContigo42);
  } else {
    if (GameFlag_Test(0x9b2) != 0)
      Func_0200412c((s32)MsgContigo149);
    else
      Func_0200412c((s32)MsgContigo50);
  }
  Func_02004144(15, 0);
  if (*(s16 *)(event + 358) == 0)
    object->angle = angle;
  BYTE_AT(object, 99) = 0;
  Func_02004074();
}
