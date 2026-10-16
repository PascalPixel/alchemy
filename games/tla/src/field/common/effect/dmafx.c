#include "types.h"
#include "callback_scheduler.h"
#include "io_reg.h"
#include "mem.h"
#include "dma.h"

void Func_080d5ff8(void);
void Func_080d607c(void);

struct DmaFxWork {
  u8 unk000[0xf01];
  u8 kind;
  u8 unkf02[6];
  s32 unkf08;
  s32 unkf0c;
  s32 unkf10;
  s32 unkf14;
  s32 unkf18;
  s32 unkf1c;
};

static __inline__ void WaitDma(struct DmaRegisters volatile *channel) {
  while ((channel->control & 0x80000000) != 0) {
  }
}

void Func_080d6408(s32 kind, s32 a, s32 c, s32 e, s32 b, s32 d, s32 f) {
  struct DmaFxWork *work = Runtime_AllocateBlock(136, sizeof(struct DmaFxWork));
  volatile u32 zero;

  zero = 0;
  Dma_Set((void *)&zero, work, 0x850003c8, REG_DMA3);
  WaitDma((volatile struct DmaRegisters *)0x040000d4);
  work->kind = kind;
  work->unkf08 = a;
  work->unkf0c = b;
  work->unkf18 = e;
  work->unkf1c = f;
  work->unkf10 = c;
  work->unkf14 = d;
  Scheduler_AddOrUpdateCallback(Func_080d607c, 0x480);
  Scheduler_AddOrUpdateCallback(Func_080d5ff8, 0xc80);
}

void Func_080d64b8(void) {
  Scheduler_RemoveCallback(Func_080d5ff8);
  Scheduler_RemoveCallback(Func_080d607c);
  {
    volatile u16 *dma = REG_DMA0;

    u16 control;

    /* FAKEMATCH: the one-pass block reads the control word before its mask is
     * built (one reordered pair, differ 2 without it). */
    do {
      control = dma[5];
    } while (0);
    dma[5] = control & 0xc5ff; // Clear start timing and repeat.
    dma[5] &= 0x7fff;          // Stop.
    (void)dma[5];
  }
}
