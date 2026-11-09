#include "types.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_020042dc(void);
void Func_020042e4(void);

void Func_02000ba4(void) {
  Func_020042dc();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(14, 5);
  FieldEvent_SetMessageId((s32)MsgMarsStar03);
  Event_ShowMessage(14, 0);
  Actor_SetAnimation(14, 9);
  Func_020042e4();
}
