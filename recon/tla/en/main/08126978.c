// Near miss, differ 6 of 20 (tla-en, 2026-10-31): only the schedule of the two
// register addresses differs: the native starts BLDALPHA's address (movs r7)
// after BLDCNT's is complete and loads the 0x10xx base between its two halves.
// In sched2 nothing in the first block depends on the BLDCNT store (priority
// 1), while BLDALPHA's address chain and the 0x1000 pool load rank 3, so they
// go first; the native order needs the store to rank 3. Tried without gain:
// BLEND_ALPHA (orrs, not adds), a do-while, i = 1 or the store in the for
// init, i < 17, a (u16) cast (all 6); a pointer to BLDALPHA set first, u32 i
// (8); s16 i (24). The permuter's only gains (to 2) test an uninitialized i
// with the store in both arms.
#include "types.h"
#include "io_reg.h"
#include "system.h"

void Func_08126978(void) {
  s32 i;

  REG_BLDCNT = 0x2044;
  for (i = 1; i <= 16; i += 2) {
    REG_BLDALPHA = i + 0x1000;
    wait(1);
  }
}
