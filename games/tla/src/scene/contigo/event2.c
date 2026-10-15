#include "contigo.h"

void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);

void Func_02000fc8(void) {
  Func_0200406c();
  Func_02004204(0);
  Func_02001938(0);
  Func_02004074();
}

void Func_02000fe0(void) {
  s32 msg;

  Func_0200406c();
  Func_02004204(0);
  msg = (s32)MsgContigo20;
  UiText_ShowPositionedMessageAndWait(msg, 1);
  UiText_ShowPositionedMessageAndWait(msg + 1, 1);
  UiText_ShowPositionedMessageAndWait(msg + 2, 1);
  UiText_ShowPositionedMessageAndWait(msg + 3, 1);
  UiText_ShowPositionedMessageAndWait(msg + 4, 1);
  UiText_ShowPositionedMessageAndWait(msg + 5, 1);
  Func_02004074();
}

void Func_0200102c(void) {
  s32 msg;

  Func_0200406c();
  Func_02004204(0);
  msg = (s32)MsgContigo21;
  UiText_ShowPositionedMessageAndWait(msg, 1);
  UiText_ShowPositionedMessageAndWait(msg + 1, 1);
  UiText_ShowPositionedMessageAndWait(msg + 2, 1);
  UiText_ShowPositionedMessageAndWait(msg + 3, 1);
#if !defined(TLA_EDITION_ES) /* スペイン語版は四つのメッセージで終わる */
  UiText_ShowPositionedMessageAndWait(msg + 4, 1);
#endif
  Func_02004074();
}
