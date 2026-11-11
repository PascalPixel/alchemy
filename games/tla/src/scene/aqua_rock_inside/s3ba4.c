#include "local.h"

s32 Func_02003ba4(s32 index) {
  u8 *base = (u8 *)VENT_WORK;

  return ((struct VentRow *)(base + index * 20))->active;
}
