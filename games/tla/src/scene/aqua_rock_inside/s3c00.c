#include "local.h"

void Func_02003c00(s32 index, s32 value) {
  u8 *base = (u8 *)VENT_WORK;

  ((struct VentRow *)(base + index * 20))->value = value;
}
