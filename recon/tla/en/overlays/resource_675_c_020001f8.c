// Draft, differ 92 of 120: the palette cycle's update, the partner of
// Func_02000038: when its countdown (gData_02001a1c) runs out it reloads
// it from gData_02001a20 (which shortens to 2), writes this step's colours
// (row gData_02001a34 of the slot table gData_02001a18, gData_02001a30
// colours a row) and the two alternating lists' colour for that step into
// palette RAM, and advances the step. The same listing as overlay 662's
// Func_0200099c. The slot set to 0x05000000 and then stepped keeps the
// constant in the loop; the volatile store keeps the colour pointer's three
// steps apart; the volatile colour pointer keeps its loads after the slot
// (from 125). Left, allocation: the native has the colour pointer in r1, the
// slot in r4, the count in r5 and the constant in r0 (here r0, r5, r4, r1), and
// a copy of the count's address in ip for the loop test; after the loop the
// native reads blue at [r1, #2], which the volatile pointer prevents.
#include "types.h"

extern u16 gData_02001a14, gData_02001a1c, gData_02001a20, gData_02001a30,
    gData_02001a34;
extern u16 *gData_02001a18, *gData_02001a24, *gData_02001a28, *Data_02001a2c;

void Func_020001f8(void) {
  volatile u16 *colour;
  u16 *slot;
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
    s32 r, g, b;

    slot = (u16 *)0x05000000;
    slot += gData_02001a18[gData_02001a34 * gData_02001a30 + i];
    r = *colour++;
    g = *colour++;
    b = *colour++;
    *(volatile u16 *)slot = b << 10 | g << 5 | r;
  }
  slot = (u16 *)0x05000000 + gData_02001a24[gData_02001a30];
  colour =
      (gData_02001a14 ? gData_02001a28 : Data_02001a2c) + gData_02001a30 * 3;
  {
    s32 r, g, b;

    r = *colour++;
    g = *colour++;
    b = *colour;
    *slot = b << 10 | g << 5 | r;
  }
  gData_02001a14 ^= 1;
  if (++gData_02001a34 == gData_02001a30)
    gData_02001a34 = 0;
}
