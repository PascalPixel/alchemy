// Draft, differ 98 of 97: deletes a save record (SaveState_DeleteRecord) with
// interrupts masked and all four DMA channels stopped, queueing
// Func_08042dc4 as the serial command around it, the one-argument sibling of
// Func_08042f10. Left: as there, the 0x208 written to IME comes in the
// native from the low half of the register holding 0x04000208, where the
// draft builds it apart; the IE clear's zero is a pool load in the draft; and
// the DMA controls are [rN, #10] in the native.
#include "types.h"
#include "io_reg.h"

#define REG_IE_ (*(volatile u16 *)0x04000200)
#define REG_IME_ (*(volatile u16 *)0x04000208)

// Stops a DMA channel: clears its start timing and repeat bits, then its
// enable bit, and reads the control back, as ☀️ MapAnimation_ApplyAffineFrame.
#define DMA_STOP(dma)                                                          \
  {                                                                            \
    (dma)[5] &= 0xc5ff;                                                        \
    (dma)[5] &= 0x7fff;                                                        \
    (void)(dma)[5];                                                            \
  }

#define DMA_STOP_ALL()                                                         \
  {                                                                            \
    DMA_STOP(REG_DMA0);                                                        \
    DMA_STOP(REG_DMA0 + 6);                                                    \
    DMA_STOP(REG_DMA0 + 12);                                                   \
    DMA_STOP((volatile u16 *)REG_DMA3);                                        \
  }

void SerialRuntime_QueueCommand(void (*command)(void));
void Func_08042dc4(void);
s32 SaveState_DeleteRecord(s32 record);

s32 Func_08042ff0(s32 record) {
  volatile u32 ime;
  volatile u32 ie;
  s32 result;

  ime = REG_IME_;
  REG_IME_ = 0x208;
  ie = REG_IE_;
  REG_IE_ = 0;
  DMA_STOP_ALL();
  REG_IME_ = ime;
  SerialRuntime_QueueCommand(Func_08042dc4);
  result = SaveState_DeleteRecord(record);
  SerialRuntime_QueueCommand(NULL);
  ime = REG_IME_;
  REG_IME_ = 0x208;
  REG_IE_ = ie;
  REG_IME_ = ime;
  return result;
}
