#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "text/msg_ids.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 mode);
void Func_0200406c(void);
void Func_02004074(void);
void Func_0200412c(s32 message);
void Func_02004144(s32 speaker, s32 flags);
void Func_02004204(s32 skip);

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

void Func_02001094(void) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *object = Object_GetById(14);
  s16 angle = object->angle;

  Func_0200406c();
  Func_02004204(0);
  BYTE_AT(object, 99) = 1;
  if (*(s16 *)(event + 358) == 0) {
    ObjectMotion_SetAngleToward(14, gPartyState.currentOwner, 0);
    if (GameFlag_Test(0x9b2) != 0)
      Func_0200412c((s32)MsgContigo203);
    else if (GameFlag_Test(0xa21) != 0)
      Func_0200412c((s32)MsgContigo199);
    else
      Func_0200412c((s32)MsgContigo41);
  } else {
    if (GameFlag_Test(0x9b2) != 0)
      Func_0200412c((s32)MsgContigo205);
    else if (GameFlag_Test(0xa21) != 0)
      Func_0200412c((s32)MsgContigo201);
    else
      Func_0200412c((s32)MsgContigo49);
  }
  Func_02004144(14, 0);
  if (*(s16 *)(event + 358) == 0)
    object->angle = angle;
  BYTE_AT(object, 99) = 0;
  Func_02004074();
}
