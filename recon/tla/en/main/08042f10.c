// Draft, differ 24 of 102 (42 before, 2026-10-25): runs Func_08015ca4 with
// interrupts masked and all four DMA channels stopped, queueing
// Func_08042dc4 as the serial command around it, then restores IE and IME.
// Written as Func_08042ff0 is, with irqlock.h's IRQ_LOCK and IRQ_UNLOCK and
// the saved IME and IE as volatile locals (42 to 24); buffer now takes fp,
// as the native's r11. Left, as there: the IME slot's pointer (movs r1, #4;
// add r1, sp in the native), the IE clear's zero as a pooled halfword
// constant, the pool's place, and a few rows of the DMA stops.
#include "types.h"
#include "io_reg.h"
#include "irqlock.h"

void SerialRuntime_QueueCommand(u32 command);
s32 Func_08015ca4(s32 slot, void *buffer);
void Func_08042dc4(s32 step);

#define DMA_STOP(n)                                                            \
  do {                                                                         \
    volatile u16 *dma = REG_DMA0 + (n) * 6;                                    \
    dma[5] &= 0xc5ff;                                                          \
    dma[5] &= 0x7fff;                                                          \
    dma[5];                                                                    \
  } while (0)

s32 Func_08042f10(s32 slot, void *buffer) {
  volatile u32 ime;
  volatile u32 ie;
  s32 result;

  IRQ_LOCK(ime);
  ie = REG_IE;
  REG_IE = 0;
  DMA_STOP(0);
  DMA_STOP(1);
  DMA_STOP(2);
  DMA_STOP(3);
  IRQ_UNLOCK(ime);
  SerialRuntime_QueueCommand((u32)Func_08042dc4);
  result = Func_08015ca4(slot, buffer);
  SerialRuntime_QueueCommand(0);
  IRQ_LOCK(ime);
  REG_IE = ie;
  IRQ_UNLOCK(ime);
  return result;
}
