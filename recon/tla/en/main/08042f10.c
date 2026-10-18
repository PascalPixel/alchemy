// Draft, differ 112 of 102: runs Func_08015ca4 with interrupts masked and all
// four DMA channels stopped, queueing Func_08042dc4 as the serial command
// around it, then restores IE and IME. The native writes 0x208 to IME (bit 0
// clear, so it masks) from the low half of the register holding IME's own
// address 0x04000208; the draft's constant 0x208 is built apart and held in
// fp, which costs a fifth saved register and moves everything after it
// (register choice and frame). The native also addresses DMA0 to DMA2 at
// [rN, #10] from 0x040000b0 stepped by 12 where the draft folds the +10 into
// the base. The two saved registers are stack slots re-read for the first
// restore, so they are volatile locals here; the second IME restore in the
// native uses the value just read. Tried: the store written as the address's
// low half (folds to the same constant).
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
s32 Func_08015ca4(s32 a, s32 b);

s32 Func_08042f10(s32 a, s32 b) {
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
  result = Func_08015ca4(a, b);
  SerialRuntime_QueueCommand(NULL);
  ime = REG_IME_;
  REG_IME_ = 0x208;
  REG_IE_ = ie;
  REG_IME_ = ime;
  return result;
}
