#include "types.h"
#include "io_reg.h"
#include "irqlock.h"

void Serial_QueueCommand(u32 command);
s32 Func_08015ca4(s32 slot, void *buffer);
void Func_08042dc4(s32 step);

#define DMA_STOP(n)                                                            \
  do {                                                                         \
    volatile u16 *dma = REG_DMA0 + (n) * 6;                                    \
    dma[5] &= 0xc5ff;                                                          \
    dma[5] &= 0x7fff;                                                          \
    dma[5];                                                                    \
  } while (0)

/// Runs Func_08015ca4 with interrupts masked and all four DMA channels
/// stopped, queueing Func_08042dc4 as the serial command around it, then
/// restores IE and IME. Func_08042ff0 does the same around
/// Save_DeletePart.
s32 Func_08042f10(s32 slot, void *buffer) {
  volatile u32 ime;
  volatile u32 ie;
  s32 result;
  u32 zero;

  /* FAKEMATCH(temporary): IRQ_LOCK written out with the saved copy's address
   * taken first; as IRQ_LOCK(ime), 4 of 102 instructions differ in tla-en. */
  do {
    volatile u32 *saved = &ime;
    volatile u16 *imeReg = &REG_IME;

    *saved = *imeReg;
    *imeReg = (u16)(u32)imeReg;
  } while (0);
  ie = REG_IE;
  /* FAKEMATCH(temporary): the zero in a word register; as REG_IE = 0 (a
   * pooled halfword constant, and no barrier), 9 of 102 instructions differ
   * in tla-en. */
  zero = 0;
  /* FAKEMATCH(barrier): the zero set after IE's register is copied; without
   * it, 2 of 102 instructions differ in tla-en. */
  asm("");
  REG_IE = zero;
  DMA_STOP(0);
  DMA_STOP(1);
  DMA_STOP(2);
  DMA_STOP(3);
  IRQ_UNLOCK(ime);
  Serial_QueueCommand((u32)Func_08042dc4);
  result = Func_08015ca4(slot, buffer);
  Serial_QueueCommand(0);
  IRQ_LOCK(ime);
  REG_IE = ie;
  IRQ_UNLOCK(ime);
  return result;
}
