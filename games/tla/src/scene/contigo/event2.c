#include "contigo.h"
#include "edition.h"

void FieldMessage_Show(s32 message, s32 position);

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
  FieldMessage_Show(msg, 1);
  FieldMessage_Show(msg + 1, 1);
  FieldMessage_Show(msg + 2, 1);
  FieldMessage_Show(msg + 3, 1);
  FieldMessage_Show(msg + 4, 1);
  FieldMessage_Show(msg + 5, 1);
  Func_02004074();
}

void Func_0200102c(void) {
  s32 msg;

  Func_0200406c();
  Func_02004204(0);
  msg = (s32)MsgContigo21;
  FieldMessage_Show(msg, 1);
  FieldMessage_Show(msg + 1, 1);
  FieldMessage_Show(msg + 2, 1);
  FieldMessage_Show(msg + 3, 1);
#if (LANGUAGE != LANGUAGE_SPANISH) /* The Spanish edition ends after four messages.  \
                              */
  FieldMessage_Show(msg + 4, 1);
#endif
  Func_02004074();
}
