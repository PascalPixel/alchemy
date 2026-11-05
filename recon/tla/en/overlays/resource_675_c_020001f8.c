// Draft, differ 44 of 120 (tla-en, 2026-11-05): the one-pass and volatile forms
// are gone, the colour loop is built from a label; left: native keeps the count's
// address in ip and step's in lr, draft reads count through r6 once.
// Draft: matched under -O2, back on its scaffold when the overlays moved to -Os (2026-11-02).
#include "types.h"
#include "call.h"

extern u16 gData_02001a14, gData_02001a1c, gData_02001a20, gData_02001a30,
    gData_02001a34;
extern u16 *gData_02001a18, *gData_02001a24, *gData_02001a28, *Data_02001a2c;

void Func_020001f8(void) {
  u16 *colour;
  u16 *slot;
  s32 due = 0;
  u32 i;
  s32 r, g, b;
  s32 index;
  u16 **list;

  if (gData_02001a1c != 0 && --gData_02001a1c == 0) {
    due = 1;
    gData_02001a1c = gData_02001a20;
    if (gData_02001a20 > 2)
      gData_02001a20--;
  }
  if (!due)
    return;
  colour = Data_02001a2c;
  i = 0;
  if (i < gData_02001a30) {
    u16 *table;
    u16 *step;
    u16 *count;

    table = gData_02001a18;
    step = &gData_02001a34;
    count = &gData_02001a30;
  top:
    slot = (u16 *)0x05000000 + table[*step * *count + i];
    r = *colour++;
    g = *colour++;
    b = *colour++;
    *slot = b << 10 | g << 5 | r;
    i++;
    if (i < *count)
      goto top;
  }
  slot = (u16 *)0x05000000 + gData_02001a24[gData_02001a30];
  index = gData_02001a30 * 3;
  if (gData_02001a14)
    colour = gData_02001a28 + index;
  else
    colour = Data_02001a2c + index;
  r = *colour++;
  g = colour[0];
  b = colour[1];
  *slot = b << 10 | g << 5 | r;
  gData_02001a14 ^= 1;
  if (++gData_02001a34 == gData_02001a30) {
    gData_02001a34 = 0;
  }
}
