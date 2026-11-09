#include "types.h"
#include "event_inline.h"

void Func_02001f5c(void);
void Func_02001f64(void);
extern const u8 MsgTheDoorIsHeld[];

void Func_02000168(void) {
  Func_02001f5c();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered(MsgTheDoorIsHeld, 1);
  Func_02001f64();
}
