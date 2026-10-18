#include "types.h"
#include "macros.h"

void Func_080ca9cc(s32 value, s32 *out);

// Clears out[0..2], then fills it from Func_080ca9cc's table entry for
// value; returns that lookup's result (0 found, -1 not).
void Func_080cec24(s32 value, s32 *out) {
  SET_VECTOR(out, 0, 0, 0);
  Func_080ca9cc(value, out);
}
