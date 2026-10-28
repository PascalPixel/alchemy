#include "scene.h"
#include "text/msg_ids.h"

void Func_02000600(void) {
  Func_02004040();
  Func_020041b8(0);
  FieldMessage_Show((s32)MsgNorthAlhafraSouth, 1);
  Func_02004048();
}

void Func_02000620(s32 a, s32 b) { Func_0200388c(gData_02004768, a, b); }

void Func_02000634(s32 a, s32 b) { Func_0200388c(gData_0200478e, a, b); }

void Func_02000648(s32 a, s32 b) {
  Func_0200388c(gData_020047d2, a, b);
  if (a == 1) {
    if (gData_02004808 == 0)
      gData_02004808 = 210;
  }
}

void Func_02000670(s32 a, s32 b) { Func_0200388c(gData_02004800, a, b); }

void Func_02000684(s32 a, s32 b) {
  Func_0200388c(gData_020047ec, a, b);
  if (a == 1) {
    if (gData_02004808 == 0)
      gData_02004808 = 210;
  }
}
