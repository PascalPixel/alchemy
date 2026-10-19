// Near miss, differ 2 of 48 (20 as plain C): the same thirteen stores. Left:
// the window height's pool load, which the native issues between the halves
// of the window address (movs r3, #128; ldr r2, =0x1088; lsls r3, r3, #19)
// and agscc after them. The sched2 dump shows why: the load's priority is one
// below the shift's (42 against 43), as everywhere in ⚓️ where the native
// issues a load earlier, as if its loads cost one cycle more. Pinned values
// in one-pass blocks order the other three pool loads without moving the
// literal pool (a barrier asm counts 4 bytes and moves the first pool two
// rows earlier: differ 16). Holding the height the same way loads it before
// the address's movs (still differ 2).
#include "types.h"
#include "io_reg.h"

void Func_0814355c(void) {
  {
    /* FAKEMATCH(fixed register): BLDCNT's value loaded into r3 inside the
     * block below, before its register's address starts; as plain C, 20 of
     * 48 instructions differ in tla-en. */
    register u16 control asm("r3");

    /* FAKEMATCH(one-pass block): ends the scheduling region after the load,
     * with no bytes for the pool's estimate. */
    do {
      control = 0x3f44;
    } while (0);
    REG_BLDCNT = control;
  }
  REG_BLDALPHA = 0x100e;
  REG_BG2X = 0;
  REG_BG2Y = 0xfffff000;
  REG_BG2PA = 0x80;
  REG_BG2PB = 0;
  REG_BG2PC = 0;
  REG_BG2PD = 0x100;
  {
    /* FAKEMATCH(fixed register): the window width loaded into r1 before the
     * window address starts, as above. */
    register u16 width asm("r1");

    /* FAKEMATCH(one-pass block): as above. */
    do {
      width = 0xf0;
    } while (0);
    REG_WIN0H = width;
    REG_WIN0V = 0x1088;
    REG_WIN1H = width;
    REG_WIN1V = 0x1088;
  }
  {
    /* FAKEMATCH(fixed register): WINOUT's value loaded into r3 before its
     * address starts, as above. */
    register u16 outside asm("r3");

    /* FAKEMATCH(one-pass block): as above. */
    do {
      outside = 0x3f21;
    } while (0);
    REG_WINOUT = outside;
  }
}
