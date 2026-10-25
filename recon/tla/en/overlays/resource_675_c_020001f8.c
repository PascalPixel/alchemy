// Draft, differ 125 of 120: the palette cycle's tick, the partner of
// Func_02000038: when its countdown (gData_02001a1c) runs out it reloads
// it from gData_02001a20 (which shortens to 2), writes this step's colours
// (row gData_02001a34 of the slot table gData_02001a18, gData_02001a30
// colours a row) and the two alternating lists' colour for that step into
// palette RAM, and advances the step. The same listing as overlay 662's
// Func_0200099c. Left: the native tracks the step through a flag set in the
// countdown, keeps the colour count's address in r12 for the loop test,
// stores 1 and 0 from the literal pool, and orders the colour loads
// otherwise; the alignment is poor.
#include "types.h"

extern u16 gData_02001a14, gData_02001a1c, gData_02001a20, gData_02001a30,
    gData_02001a34;
extern u16 *gData_02001a18, *gData_02001a24, *gData_02001a28, *Data_02001a2c;

void Func_020001f8(void) {
  u16 *colour, *slot;
  s32 due = 0;
  u32 i;

  if (gData_02001a1c != 0 && --gData_02001a1c == 0) {
    due = 1;
    gData_02001a1c = gData_02001a20;
    if (gData_02001a20 > 2)
      gData_02001a20--;
  }
  if (!due)
    return;
  colour = Data_02001a2c;
  for (i = 0; i < gData_02001a30; i++) {
    slot = (u16 *)0x05000000 +
           gData_02001a18[gData_02001a34 * gData_02001a30 + i];
    *slot = colour[2] << 10 | colour[1] << 5 | colour[0];
    colour += 3;
  }
  slot = (u16 *)0x05000000 + gData_02001a24[gData_02001a30];
  colour = (gData_02001a14 ? gData_02001a28 : Data_02001a2c) +
           gData_02001a30 * 3;
  *slot = colour[2] << 10 | colour[1] << 5 | colour[0];
  gData_02001a14 ^= 1;
  if (++gData_02001a34 == gData_02001a30)
    gData_02001a34 = 0;
}
