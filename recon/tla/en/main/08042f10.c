// Draft, differ 42 of 102 (was 112 with volatile locals and IME written as
// 0x208): runs Func_08015ca4 with interrupts masked and all four DMA channels
// stopped, queueing Func_08042dc4 as the serial command around it, then
// restores IE and IME. IME is masked by writing back the low half of its own
// address, as irqlock.h's IME_DISABLE; the DMA controls are dma[5] from
// 0x040000b0 stepped by 6 halfwords, which gives the native's [rN, #10]. The
// two saved values live in a two-word stack array reached through pointers
// kept in r10 (sp + 4, IME) and r9 (sp, IE), and the second IME restore
// reuses the value just read, so they are not volatiles.
// Left: the native builds the IME slot's pointer as movs r1, #4; add r1, sp
// where the draft has add r1, sp, #4; the IE clear is movs r3, #0 there and a
// pool load here; buffer takes r9 where the native keeps it in r11; and
// several loads and ALU rows of the DMA stops sit one row apart. Measured:
// two separate locals (differ 44, the first IME store to [sp, #4]); the IME
// save in a one-pass block before the IE pointer is taken (differ 40 with
// separate locals); the saves in static inline helpers taking the addresses
// (differ 69, the slots purged into registers); the locals as volatiles
// (differ 112).
#include "types.h"
#include "io_reg.h"

void SerialRuntime_QueueCommand(u32 command);
s32 Func_08015ca4(s32 slot, void *buffer);
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

s32 Func_08042f10(s32 slot, void *buffer) {
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
  result = Func_08015ca4(slot, buffer);
  SerialRuntime_QueueCommand(0);
  *pIme = *ime;
  *ime = (u16)(u32)ime;
  REG_IE = *pIe;
  *ime = *pIme;
  return result;
}
