#include "types.h"

s32 Func_080d1ee8(s32 key);

/// Calls Func_080d1ee8 with key and discards its value; returns 0.
s32 Func_080d1f0c(s32 key) {
  Func_080d1ee8(key);
  return 0;
}

/// Forwards key to Func_080d1ee8 and returns its value.
s32 Func_080d1f18(s32 key) { return Func_080d1ee8(key); }
