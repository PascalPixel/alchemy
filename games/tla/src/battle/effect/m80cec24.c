#include "types.h"
#include "macros.h"

void Func_080ca9cc(s32 value, s32 *out);

void Func_080cec24(s32 value, s32 *out) {
  SET_VECTOR(out, 0, 0, 0);
  Func_080ca9cc(value, out);
}
