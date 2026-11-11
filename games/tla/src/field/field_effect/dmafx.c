#include "types.h"
#include "task.h"
#include "io_reg.h"
#include "heap_release.h"
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
  while ((channel->control & DMA_ENABLE << 16) != 0) {
  }
}

/// Starts the line scroll: its work (heap slot 136) holds the mode and, for x
/// and y, the wave's frequency, its step each frame and its amplitude; two
/// tasks, one in the vertical blank pass and one in the frame pass, build and
/// send each frame's line offsets.
void LineScroll_InitWave(s32 mode, s32 freqX, s32 stepX, s32 ampX, s32 freqY,
                         s32 stepY, s32 ampY) {
  struct DmaFxWork *work = HeapSlot_AllocEwram(136, sizeof(struct DmaFxWork));
  volatile u32 zero;

  zero = 0;
  Dma_Set((void *)&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (sizeof(*work) / 4),
          REG_DMA3);
  WaitDma((volatile struct DmaRegisters *)&REG_DMA3SAD);
  work->kind = mode;
  work->unkf08 = freqX;
  work->unkf0c = freqY;
  work->unkf18 = ampX;
  work->unkf1c = ampY;
  work->unkf10 = stepX;
  work->unkf14 = stepY;
  Task_Add(Func_080d607c, TASK_PASS_VBLANK);
  Task_Add(Func_080d5ff8, TASK_PASS_FRAME);
}

/// Stops the line scroll: removes its two tasks and stops DMA 0.
void LineScroll_Stop(void) {
  Task_Remove(Func_080d5ff8);
  Task_Remove(Func_080d607c);
  {
    volatile u16 *dma = REG_DMA0;

    u16 control;

    control = dma[5];
    dma[5] = control & 0xc5ff; // Clear start timing and repeat.
    dma[5] &= 0x7fff;          // Stop.
    (void)dma[5];
  }
}
