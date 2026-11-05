// Near miss, differ 2 of 20 (tla-en, 2026-11-05), with untagged empty asms, one-pass
// blocks and locals found by search; each needs its S2 tag on adoption, measured then.
// Near miss, differ 4 of 20 (tla-en, 2026-10-31): sched2 order of the
// REG_BLDCNT store; plain C gives the store priority 1 (a leaf), and the native
// order needs priority 3 with one dependent and BLDALPHA's address set ahead of
// it in RTL. The Broken Seal's BattleBackground_FadeAlphaOut.
// The native loads BLDCNT's value from the pool between the first two rows of
// its address, starts BLDALPHA's address (movs r7) before the store, then loads
// the 0x1010 base between that address's rows. Plain C (the twin's form) is
// differ 6: the store comes after movs r7, ldr r6 and lsls r7. A full empty
// barrier after the store keeps the loop setup after it, so ldr r3 leads and
// movs r7 comes one row late (differ 4). Pinning the value to r3 adds nothing
// (differ 8 alone, 4 with the barrier). Setting a BLDALPHA pointer first, with
// or without step first, is differ 8; step as u16, a base local, (step * 2 + 1)
// and an inner local are worse (18 to 24); do-while, < 17 and a (u16) cast stay
// 6. Earlier: the pin in a one-pass block, the value in a one-pass block, an
// operand barrier, a memory clobber with an output (differ 6 to 8), four
// minutes of permute (6).
#include "types.h"
#include "io_reg.h"
#include "system.h"

void Func_08126944(void) {
  s32 step;
  asm("");

  do {
  REG_BLDCNT = 0x2044;
  } while (0);
  /* FAKEMATCH(barrier): an empty asm after the store; without it, 6 of 20
   * instructions differ in tla-en. */
  __asm__("" : : : "memory");
  for (step = 1; step <= 16; step += 2) {
    REG_BLDALPHA = 0x1010 - step;
    wait(1);
  }
}
