// Draft, differ 121 of 120: Contigo's palette animation tick: when its countdown
// runs out (the next one a frame shorter each time, down to 2), it writes this
// step's colours (RGB triples from Data_02006018) to their palette entries,
// then one more entry from the triples of whichever table the toggle picks,
// flips the toggle and moves to the next step.
#include "types.h"

extern u16 gData_02006000;
extern u16 *gData_02006004;
extern u16 gData_02006008;
extern u16 gData_0200600c;
extern u16 *gData_02006010;
extern u16 *gData_02006014;
extern u16 *Data_02006018;
extern u16 gData_0200601c;
extern u16 gData_02006020;

#define PLTT ((u16 *)0x05000000)

void Func_020001f8(void) {
  s32 step = 0;
  u16 *color;
  u16 *entry;
  u32 i;

  if (gData_02006008 != 0 && --gData_02006008 == 0) {
    step = 1;
    gData_02006008 = gData_0200600c;
    if (gData_0200600c > 2)
      gData_0200600c--;
  }
  if (step == 0)
    return;
  color = Data_02006018;
  for (i = 0; i < gData_0200601c; i++) {
    entry = PLTT + gData_02006004[gData_02006020 * gData_0200601c + i];
    *entry = color[0] | (color[1] << 5) | (color[2] << 10);
    color += 3;
  }
  entry = PLTT + gData_02006010[gData_0200601c];
  color = (gData_02006000 != 0 ? gData_02006014 : Data_02006018) +
          gData_0200601c * 3;
  *entry = color[0] | (color[1] << 5) | (color[2] << 10);
  gData_02006000 ^= 1;
  if (++gData_02006020 == gData_0200601c)
    gData_02006020 = 0;
}
