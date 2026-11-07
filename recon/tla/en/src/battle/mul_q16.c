#include "types.h"
#include "io_reg.h"
#include "iwram_call.h"

void Func_081269ac(void) {
  /* FAKEMATCH: the blend value pinned to r3, so it loads from the pool before
   * the register address is started; as a plain store, 2 instructions differ
   * in tla-en. */
  register u16 blend __asm__("r3") = 0xbf;

  /* FAKEMATCH: the barrier of the pin above: it holds r2 until the value is
   * loaded. */
  __asm__("" : "+l"(blend) : : "r2");
  REG_BLDCNT = blend;
}

s32 Func_081269bc(s32 a, s32 b) { return Iwram_MulQ16(a, b); }
