#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

s32 Func_02004464();
s32 Func_0200446c();
extern u8 MsgProx33[];

void Func_02000620(void) {
  Func_02004464();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgProx33);
  Event_AskYesNoAndShowResponse(0xf, 0);
  Func_0200446c();
}

s32 Func_02000644(void) {
  u16 turn;
  s32 facing;

  turn = Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x5fff;
  facing = 1;
  if (turn >= 0x3fff)
    facing = 0;
  return facing;
}
