// Draft, differ 0 of 74 (matches): sets up the map animation work for
// map mode: its 92-byte work and an 8 KB buffer, both cleared by DMA from
// one zero word, and the animation callbacks (mode 4 runs its own; modes 3
// and 4 take Func_08023e18 at 0x480, the others Func_0802386c). No ☀️ twin
// found; ☀️'s field.c starts its map animations the same way. With
// dma.h's Dma_Fill32 twice (a zero per block), differ 52: the native
// shares one stack word.
#include "types.h"
#include "mem.h"
#include "dma.h"
#include "io_reg.h"
#include "callback_scheduler.h"

struct MapAnimationWork {
  u8 unk00[6];
  u8 unk06;
  u8 unk07;
  u8 unk08[84];
};

void Func_08022bd8(s32 mode);
void Func_0802493c(void);
void Func_08023f9c(void);
void Func_080246b8(void);
void Func_08023e18(void);
void Func_0802386c(void);

void Func_080230e0(s32 mode) {
  struct MapAnimationWork *work;
  void *buffer;
  volatile u32 zero;

  work = Runtime_AllocateBlock(24, sizeof(*work));
  buffer = Runtime_AllocateBlock(20, 0x2000);
  Func_08022bd8(mode);
  zero = 0;
  Dma_Set((const void *)&zero, buffer, 0x85000000 | (0x2000 / 4), REG_DMA3);
  zero = 0;
  Dma_Set((const void *)&zero, work, 0x85000000 | (sizeof(*work) / 4),
          REG_DMA3);
  if (mode == 4) {
    Scheduler_AddOrUpdateCallback(Func_0802493c, 0x48a);
  } else {
    Scheduler_AddOrUpdateCallback(Func_08023f9c, 0x48b);
    Scheduler_AddOrUpdateCallback(Func_080246b8, 0x48a);
  }
  if (mode == 3 || mode == 4)
    Scheduler_AddOrUpdateCallback(Func_08023e18, 0x480);
  else
    Scheduler_AddOrUpdateCallback(Func_0802386c, 0x480);
  work->unk06 = 15;
  work->unk07 = 0;
}
