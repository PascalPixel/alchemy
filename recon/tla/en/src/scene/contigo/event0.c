#include "../../../../../../games/tla/src/scene/contigo/contigo.h"
#include "iwram_call.h"

void Func_020005f8(void) { Func_02004214(16, 0, 14); }

void Func_02000608(void) {
  Func_02004214(17, 2, 15);
  GameFlag_SetBit(SPLIT_CONSTANT(0x900, 230));
}

s32 Func_02000620(s32 *a, s32 *b) {
  s32 dx = (*a++ - *b++) >> 16;
  s32 dy = (*a++ - *b++) >> 16;
  s32 dz = (*a - *b) >> 16;
  s32 dxsq = dx * dx;
  s32 dysq = dy * dy;
  s32 dzsq = dz * dz;

  return Iwram_Sqrt(dxsq + dysq + dzsq);
}
