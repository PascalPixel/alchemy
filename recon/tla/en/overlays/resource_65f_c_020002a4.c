// Draft, differ 72 of 110 (tla-en, 5 November): the wavy scroll lines of the layer, read by a DMA each scanline.
// Left, one cause: the native keeps a test at the loop's top (cmp r6, #160, beq) with i known to be 0; here CSE folds it, whatever the
// spelling (for, while, labels, if around a do). With those refs i ranks first in the native (r6), line r5, the first sine r8; here line r8, the first sine r5, i r10.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "map.h"
#include "dma.h"
#include "io_reg.h"

extern u8 gFrameCount;
extern u16 Data_02006340[];

s32 Math_Sine(s32 angle);

void Func_020002a4(void) {
  s32 (*sine)(s32) = *(s32 (**)(s32))(((u32)Math_Sine & ~3) + 4);
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[1];
  u16 *line = Data_02006340;

  if (*(s8 *)((u8 *)Ram_HeapSlots->eventWork + 0xca4) == 0) {
    u32 frame;
    s32 x, z, a, b;
    s32 stepA, stepB;
    s32 i;
    volatile u16 *dma;

    *(s32 *)&layer->unk08[20] = 0;
    i = 0;
    frame = gFrameCount;
    z = *(s16 *)((u8 *)layer + 6);
    x = *(s16 *)((u8 *)layer + 2);
    stepA = 0x3300;
    stepB = 0x1320;
    a = z * stepA + (frame << 10);
    b = z * stepB - (frame << 10);
    for (; i != 160; i++) {
      s32 s1 = sine(a);
      s32 s2 = sine(b);
      *line++ = ((s1 * 3 + s2 * 7) >> 19) + x;
      a += 0x3300;
      b += 0x1320;
    }
    line = Data_02006340;
    *(volatile u16 *)0x04000018 = line[0];
    dma = (volatile u16 *)0x040000b0;
    dma[5] &= 0xc5ff;
    dma[5] &= 0x7fff;
    (void)dma[5];
    Dma_Set(line + 1, (void *)0x04000018, 0xa2600001, (volatile u32 *)dma);
  }
}
