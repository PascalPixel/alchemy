#include "types.h"

void Func_08196404(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);

void Func_081963ec(s32 a0, s32 value) {
  Func_08196404(a0, 7, 7, value & 0xf, (u32)value >> 4);
}
