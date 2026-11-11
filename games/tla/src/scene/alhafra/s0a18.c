#include "types.h"
#include "event_inline.h"


s32 Func_02000a18(void) {
  if (GameFlag_Test(0x8ab)) {
    u16 *src = (u16 *)0x5000116;

    u32 c;

    /* FAKEMATCH(dead): the four copies in a once-only block, whose end holds
     * the order around it; as plain statements, 4 of 57 instructions differ in
     * tla-en. */
    do {
      c = *src;
      *(u16 *)0x50000f6 = c;
      src++;
      c = *src;
      *(u16 *)0x50000f8 = c;
      src++;
      c = *src;
      *(u16 *)0x50000fa = c;
      {
        u32 last = src[1];

        *(u16 *)0x50000fc = last;
      }
    } while (0);
    MapGrid_CopyCellAttributes(37, 18, 2, 2, 37, 33);
    MapGrid_CopyCellAttributes(37, 18, 2, 3, 39, 31);
    MapGrid_CopyCellAttributes(50, 80, 7, 5, 35, 95);
  }
  return 0;
}
