#include "types.h"
#include "macros.h"
#include "main_routine_work.h"

s32 GameFlag_Test(s32 flag);
extern s16 Data_080f2204[];

/// The current scene's rows of the scene item table (Data_080f2204): those
/// after the first header for this scene whose flag is -1 or not yet set, or
/// NULL.
s16 *Func_080ceba8(void) {
  s32 scene = gMainRoutineWork.scene;
  s16 *p = Data_080f2204;
  s16 *found = NULL;
  s32 header;
  s32 flag;
  s16 value;

  for (;;) {
    header = *p++;
    flag = *p++;
    if (header == 0)
      break;
    if ((header & 0x3fff) == scene && (flag == -1 || !GameFlag_Test(flag))) {
      found = p;
      break;
    }
    // Skip to the next header (bit 14) or the end.
    while ((value = p[0]) != 0 && !(value & 0x4000))
      p += 2;
  }
  return found;
}

void Func_080ca9cc(s32 value, s32 *out);

/// Clears out[0..2], then fills it from Func_080ca9cc's table entry for
/// value; returns that lookup's result (0 found, -1 not).
void Func_080cec24(s32 value, s32 *out) {
  SET_VECTOR(out, 0, 0, 0);
  Func_080ca9cc(value, out);
}
