#include "types.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_0200406c(void);
void Func_02004074(void);

void Func_02000eb8(void) {
  Func_0200406c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgContigo26);
  Event_AskYesNoAndShowResponse(10, 0);
  Func_02004074();
}

void Func_02000edc(void) {
  s32 msg;

  Func_0200406c();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgContigo25;
  FieldMessage_Show(msg, 1);
  FieldMessage_Show(msg + 1, 1);
  FieldMessage_Show(msg + 2, 1);
  FieldMessage_Show(msg + 3, 1);
  FieldMessage_Show(msg + 4, 1);
  FieldMessage_Show(msg + 5, 1);
  FieldMessage_Show(msg + 6, 1);
  FieldMessage_Show(msg + 7, 1);
  FieldMessage_Show(msg + 8, 1);
  Func_02004074();
}

void Func_02000f40(void) {
  s32 msg;

  Func_0200406c();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgContigo06;
  FieldMessage_Show(msg, 1);
  FieldMessage_Show(msg + 1, 1);
  FieldMessage_Show(msg + 2, 1);
  FieldMessage_Show(msg + 3, 1);
  Func_02004074();
}

void Func_02000f7c(void) {
  s32 msg;

  Func_0200406c();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgContigo07;
  FieldMessage_Show(msg, 1);
  FieldMessage_Show(msg + 1, 1);
  FieldMessage_Show(msg + 2, 1);
  FieldMessage_Show(msg + 3, 1);
  FieldMessage_Show(msg + 4, 1);
  FieldMessage_Show(msg + 5, 1);
  Func_02004074();
}
