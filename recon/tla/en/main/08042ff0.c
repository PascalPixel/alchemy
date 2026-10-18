// Draft, differ 38 of 97 (was 98 with volatile locals and IME written as
// 0x208): deletes a save record (SaveState_DeleteRecord) with interrupts
// masked and all four DMA channels stopped, queueing Func_08042dc4 as the
// serial command around it, the one-argument sibling of Func_08042f10 and
// written the same way (see its header). Left, as there: the native builds
// the IME slot's pointer as movs r1, #4; add r1, sp, the IE clear's zero is a
// pool load here, and loads and ALU rows of the DMA stops sit one row apart.
// Measured: two separate locals for the saved values (differ 40).
#include "types.h"
#include "io_reg.h"

void SerialRuntime_QueueCommand(u32 command);
s32 SaveState_DeleteRecord(s32 record);
void Func_08042dc4(s32 step);

// Stops DMA channel n: clears its start timing and repeat bits, then its
// enable bit, and reads the control back.
#define DMA_STOP(n)                                                            \
  do {                                                                         \
    volatile u16 *dma = REG_DMA0 + (n) * 6;                                    \
    dma[5] &= 0xc5ff;                                                          \
    dma[5] &= 0x7fff;                                                          \
    dma[5];                                                                    \
  } while (0)

s32 Func_08042ff0(s32 record) {
  volatile u16 *ime = &REG_IME;
  u32 saved[2];
  u32 *pIme = &saved[1];
  u32 *pIe = &saved[0];
  s32 result;

  *pIme = *ime;
  *ime = (u16)(u32)ime;
  *pIe = REG_IE;
  REG_IE = 0;
  DMA_STOP(0);
  DMA_STOP(1);
  DMA_STOP(2);
  DMA_STOP(3);
  *ime = *pIme;
  SerialRuntime_QueueCommand((u32)Func_08042dc4);
  result = SaveState_DeleteRecord(record);
  SerialRuntime_QueueCommand(0);
  *pIme = *ime;
  *ime = (u16)(u32)ime;
  REG_IE = *pIe;
  *ime = *pIme;
  return result;
}
