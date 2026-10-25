// Draft, differ 24 of 97 (38 before, 2026-10-25): deletes a save record
// (SaveState_DeleteRecord) with interrupts masked and all four DMA channels
// stopped, queueing Func_08042dc4 as the serial command around it, the
// one-argument sibling of Func_08042f10. Written with irqlock.h's IRQ_LOCK
// and IRQ_UNLOCK and the saved IME and IE as volatile locals, which the
// native keeps in the frame and reaches through sp-based pointers (38 to 24).
// Left: the native builds the IME slot's pointer as movs r1, #4; add r1, sp;
// the IE clear's zero is a halfword constant here, so it is a pool load
// (the native's movs r3, #0 is a word constant), and the pool then sits in
// the DMA stops instead of at the end; a few loads and ALU rows of the stops
// sit one row apart.
#include "types.h"
#include "io_reg.h"
#include "irqlock.h"

void SerialRuntime_QueueCommand(u32 command);
s32 SaveState_DeleteRecord(s32 record);
void Func_08042dc4(s32 step);

#define DMA_STOP(n)                                                            \
  do {                                                                         \
    volatile u16 *dma = REG_DMA0 + (n) * 6;                                    \
    dma[5] &= 0xc5ff;                                                          \
    dma[5] &= 0x7fff;                                                          \
    dma[5];                                                                    \
  } while (0)

s32 Func_08042ff0(s32 record) {
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
  result = SaveState_DeleteRecord(record);
  SerialRuntime_QueueCommand(0);
  IRQ_LOCK(ime);
  REG_IE = ie;
  IRQ_UNLOCK(ime);
  return result;
}
