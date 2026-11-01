// Draft, differ 109 of 110, skeleton gap 30 (tla-en, 2026-11-01): Garoh's
// scanline wave, building 160 BG offsets from two sines and starting an HBlank
// DMA.
// Closed: the two muls (a constant reaches the multiply only through a local;
// written inline, expmed expands it into shifts); the constants stay rebuilt
// in the loop.
// Left, two causes:
// 1. The native keeps `cmp r6, #160; beq` after `movs r6, #0`, with a, b and
//    the muls before it. cse, gcse's cprop or the copied exit test fold it in
//    every plain form: for, while, one-pass block before the loop, nested
//    while and do-while, a goto top with a second entry to the body, a
//    condition with a label in it (i != 160 || zero, to defer the copy to the
//    jump pass after loop), the counter in a one-word struct. A goto loop keeps
//    the test but ends in `b` instead of `cmp; bne` (114). An empty asm with i
//    as its output keeps both tests in a for loop (gap 25, differ 127, since
//    i takes r4 and a stack slot); it did not reach 0, so it came back out.
// 2. Allocation. The native's order is line r5, i r6, b r7, s1 r8, a r10, x r9,
//    sine r11, which is global-alloc's order by priority. Here s1 is
//    single-block with one death, so local-alloc gives it r5 first and line
//    falls to a high register. For s1 to rank fourth it must be a global pseudo
//    with a long live length, which nothing in the listing shows: r8 is
//    referenced only by the three uses in the body. No form found gives that.
// Tried without effect (109 or 110): declaration orders in the block, i
// first, frame << 10 as a local, line[i] and Data_02006340[i]. Worse: i and z
// both stepped with the angles built in the loop (124; a multiply by a bare
// constant expands to shifts), a new pointer after the loop (117), no line
// before the if (125).
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "dma.h"
#include "io_reg.h"

extern u8 gFrameCount;
extern u16 Data_02006340[];

s32 Math_Sine(s32 angle);

void Func_020002a4(void) {
  u16 *line = Data_02006340;
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[1];
  /// The veneer's target word: Math_Sine itself rather than its far call.
  s32 (*sine)(s32) = *(s32 (**)(s32))(((u32)Math_Sine & ~3) + 4);

  if (*(s8 *)((u8 *)Ram_HeapSlots->eventWork + 0xca4) == 0) {
    u32 frame;
    s32 x, z, a, b;
    s32 stepA = 0x3300, stepB = 0x1320;
    s32 i;
    volatile u16 *dma;

    *(s32 *)&layer->unk08[20] = 0;
    frame = gFrameCount;
    z = *(s16 *)((u8 *)layer + 6);
    x = *(s16 *)((u8 *)layer + 2);
    a = z * stepA + (frame << 10);
    b = z * stepB - (frame << 10);
    for (i = 0; i != 160; i++) {
      s32 s1 = sine(a);
      s32 s2 = sine(b);

      *line++ = ((s1 * 3 + s2 * 7) >> 19) + x;
      a += stepA;
      b += stepB;
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
