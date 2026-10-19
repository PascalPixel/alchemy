#include "types.h"
#include "io_reg.h"

/// BG2 back to one texel per pixel across.
void Graphics_ResetBg2Pa(void) {
  /* FAKEMATCH: the native loads the halfword from the pool before it starts the
   * register address; the plain store starts the address first (one reordered
   * pair, differ 2). Only a halfword register loads from the pool and a u16
   * local is widened, so the value is pinned to the r3 it already takes, as in
   * f18284c.c. */
  register u16 value __asm__("r3") = Q_8_8(1);

  /* FAKEMATCH: the barrier of the pin above: it holds r2 until the value is
   * loaded. */
  __asm__("" : "+l"(value) : : "r2");
  REG_BG2PA = value;
}

/// BG2 at half a texel per pixel across: twice as wide.
void Graphics_SetBg2AffineScaleHalf(void) {
  /* FAKEMATCH: as in Graphics_ResetBg2Pa, the pooled halfword is loaded before
   * the register address is started (differ 2 without the pin and its barrier).
   */
  register u16 value __asm__("r3") = Q_8_8(0.5);

  /* FAKEMATCH: the barrier of the pin above. */
  __asm__("" : "+l"(value) : : "r2");
  REG_BG2PA = value;
}
