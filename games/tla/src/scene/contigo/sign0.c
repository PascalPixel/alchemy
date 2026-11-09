#include "types.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_0200406c(void);
void Func_02004074(void);

void Func_020008ec(void) {
  Func_0200406c();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgContigo110, 1);
  Func_02004074();
}

void Func_0200090c(void) {
  Func_0200406c();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgContigo111, 1);
  Func_02004074();
}

void Func_0200092c(void) {
  Func_0200406c();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgContigo112, 1);
  Func_02004074();
}

void Func_0200094c(void) {
  Func_0200406c();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgContigo113, 1);
  Func_02004074();
}

void Func_0200096c(void) {
  Func_0200406c();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgContigo114, 1);
  Func_02004074();
}

void Func_0200098c(void) {
  Func_0200406c();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgShamanEntranceLocked, 1);
  Func_02004074();
}

void Func_020009ac(void) {
  Func_0200406c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgContigo35);
  Event_AskYesNoAndShowResponse(8, 0);
  Func_02004074();
}
