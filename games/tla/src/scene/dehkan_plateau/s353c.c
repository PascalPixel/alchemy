#include "types.h"
#include "event_inline.h"

extern const u16 gData_02004276[];

void Func_0200353c(void) {
  /* FAKEMATCH(pin): flag in r5, which leaves r6 to the entry; as a plain
   * local, 61 of 79 instructions differ in tla-en. */
  register s32 flag asm("r5") = 800;
  s32 offset = 0;

  while (1) {
    if (GameFlag_Test(flag)) {
      u16 *entry = (u16 *)((u8 *)gData_02004276 + offset);
      s32 y;
      s32 x;
      s32 one;

      flag -= 800;
      x = *entry++;
      y = *entry++;
      one = 1;
      MapGrid_CopyCellsTo(3, 8, x, y, one, one);
      x = entry[0];
      y = entry[1];
      MapGrid_CopyCellsTo(67, 3, x, y, one, 5);
      if (flag == 1)
        MapGrid_CopyCellsTo(67, 4, 69, 3, one, one);
      x -= 64;
      y += 66;
      MapGrid_CopyCellsTo(3, 69, x, y, one, 4);
      return;
    }
    if (flag == 863)
      return;
    offset += 8;
    flag++;
  }
}
