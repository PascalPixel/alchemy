#include "types.h"

void Func_02002618(s32 id, s32 dx, s32 dz);
void Func_02004b68(s32 a, s32 b, s32 c, s32 d);

void Func_02002640(s32 a, s32 b, s32 dx, s32 dz) {
  Func_02004b68(a, b, 48, 224);
  Func_02002618(5, dx, dz);
  Func_02002618(8, dx, dz);
  Func_02002618(9, dx, dz);
  Func_02002618(10, dx, dz);
  Func_02004b68(a + 16, b, 64, 224);
}
