#include "types.h"
#include "io_reg.h"
#include "irq.h"

void Func_08045780(void) {
  u32 mask = 4;
  u32 value = REG_BG1CNT;

  mask = -mask;
  value &= mask;
  REG_BG1CNT = value;
}

void Func_08045794(void) {
  /* FAKEMATCH: a named zero is built with movs; as a literal the halfword store
   * loads it from the pool (differ 13). */
  u32 zero = 0;

  REG_BG0VOFS = zero;
  /* FAKEMATCH: a one-pass block loads the handler before the interrupt number
   * is built; called straight the two rows trade places (differ 2). */
  do {
    Irq_SetHandler(2, 136, Func_08045780);
  } while (0);
}
