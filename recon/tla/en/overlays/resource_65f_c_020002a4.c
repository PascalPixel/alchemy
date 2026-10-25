// Draft, differ 126 of 110: Garoh's scanline wave, building 160 BG
// offsets from two sines and starting an HBlank DMA. Left: the native
// multiplies by constants with muls and keeps different registers.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "dma.h"
#include "io_reg.h"

extern u8 gFrameCount;
extern u16 Data_02006340[];

s32 Math_Sine(s32 angle);

void Func_020002a4(void) {
  /// The veneer's target word: Math_Sine itself rather than its far call.
  s32 (*sine)(s32) = *(s32 (**)(s32))(((u32)Math_Sine & ~3) + 4);
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[1];
  u16 *line = Data_02006340;

  if (*(s8 *)((u8 *)Ram_HeapSlots->eventWork + 0xca4) == 0) {
    u32 frame;
    s32 x, z, a, b;
    s32 i;
    volatile u16 *dma;

    *(s32 *)&layer->unk08[20] = 0;
    frame = gFrameCount;
    z = *(s16 *)((u8 *)layer + 6);
    x = *(s16 *)((u8 *)layer + 2);
    a = z * 0x3300 + (frame << 10);
    b = z * 0x1320 - (frame << 10);
    for (i = 0; i != 160; i++) {
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
