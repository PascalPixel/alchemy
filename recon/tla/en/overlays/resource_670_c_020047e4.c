// near miss: differ 24 (listing 71 instructions, draft 71); as Func_02000d74's
// wait loop trial: the loop as an if around a label and a backward goto keeps
// the address and constant loads in the loop (differ 2): the native then still
// starts 0x240000 before it loads y ahead of the loop; comparing as 0x240000 <
// y, a one-pass block or an empty asm before the test change nothing. Not
// adopted: a goto that only forces a layout (S2) trial: Func_02004294 (overlay
// 670) waits in the same loop, there not rotated (b to the test); pins on the
// slot address do not keep it in the loop (differ 24 and 27)
#include "../../../../games/tla/src/scene/kibombo/kibombo.h"

void Func_020047e4(void) {
  Func_02004524();
  Func_02005c9c(161);
  Func_02003640(0, 1);
  Battle_WaitMode0(20);
  Func_02005b4c((s32)MsgYouAreStillNot);
  Func_02005b64(17, 0);
  if (gData_02007950 != 0) {
    gData_02007950->motionFlags = 0;
    while (gData_02007950->y > 0x240000) {
      Kibombo_ObjArm(17, 0xd000, 0);
      gData_02007950->y += 0xffffc000;
      WaitFrames(1);
    }
  }
  Motion_SetModeAndWaitAnimation(17, 4);
  Func_02005b64(17, 0);
  Kibombo_ObjTurn(17, 0x3000);
  Func_02005b64(0x4011, 0);
  GameFlag_SetBit(0x8fc);
  GameFlag_SetBit(265);
  Func_02005bc4(12);
}
