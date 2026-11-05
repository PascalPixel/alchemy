// Under -Os, 5 November: differ 81 of 110 (tla-en); older figures below predate it.
// Draft, differ 61 of 110, skeleton gap 12 (tla-en, 2026-11-01): Garoh's
// scanline wave, building 160 BG offsets from two sines and starting an HBlank
// DMA. The gap is 7 listing instructions missing and 12 added, all from the
// allocation below; statements, loads, muls and the DMA tail are the listing's.
// Closed, and why:
// - The entry test `cmp r6, #160; beq` after `movs r6, #0` occurs nowhere else
//   in the game. thread_jumps runs before cse and again after cse2. Top and
//   bottom tests that differ at the first run (a register, a literal) stay
//   apart, so cse1 sees two paths into the top label and cannot fold; the last
//   run redirects the bottom test past the top one, and nothing folds after it.
//   lineCount is one-constant and gets the immediate from reload when it has no
//   register. goto and do { if break } while compare the same.
//   Review this: lineCount at the top and 160 at the bottom is the only form
//   found, and its only effect is to keep that test. It is a set-once local
//   (S2), but it may read as a disguise. Tests with one spelling at both ends,
//   and const lineCount, lose the test (differ 88, gap 17). No plain original
//   form is known.
// - Order sine, layer, line follows the listing's first block; line is set late
//   so layer is r1 and the base r2 (differ 95 to 72).
// - stepA and stepB set after the loads, side by side, give z r0, a tied r3
//   for the first mul and the copy for the second.
// - s2 * 7 first gives the listing's lsls and subs order.
// Left, allocation only:
// 1. s1 is single-block with one death, so local-alloc gives it r5 before
//    global-alloc runs. The listing has line r5, i r6, b r7, s1 r8, a r10,
//    x r9, sine r11. No form made s1 a global pseudo: declaration scope, order,
//    separate statements. Pinning s1 to r8 in scratch does not close it
//    (differ 77), so it is not the only cause.
// 2. line ranks low (7 refs over 94 insns) and takes r4 with a caller save
//    around the calls, adding sub sp, the str and ldr pair. lineCount then gets
//    r11 and shows as `cmp r6, fp` at the top. The listing needs line ranked
//    first and lineCount without a register.
//    A separate pointer after the loop leaves lineCount unallocated and removes
//    the spill (112 instructions) but gives differ 72 and puts it in r0, where
//    the listing has r5, so line is one variable there.
// Tried without gain: setting line inside the if (63, 67), Data_02006340 direct
// after the loop (72), line -= 160 (62), a and b swapped in the body (75), s2
// after s1 at declaration, i and lineCount scope and order, frame << 10 as a
// local or in two steps (worse), multiply operand order (no change).
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
  /// The veneer's target word: Math_Sine itself rather than its far call.
  s32 (*sine)(s32) = *(s32 (**)(s32))(((u32)Math_Sine & ~3) + 4);
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[1];
  u16 *line = Data_02006340;

  if (*(s8 *)((u8 *)Ram_HeapSlots->eventWork + 0xca4) == 0) {
    u32 frame;
    s32 x, z, a, b;
    s32 stepA, stepB;
    s32 i, lineCount;
    volatile u16 *dma;

    *(s32 *)&layer->unk08[20] = 0;
    frame = gFrameCount;
    z = *(s16 *)((u8 *)layer + 6);
    x = *(s16 *)((u8 *)layer + 2);
    stepA = 0x3300;
    stepB = 0x1320;
    a = z * stepA + (frame << 10);
    b = z * stepB - (frame << 10);
    lineCount = 160;
    i = 0;
  next_line:
    if (i == lineCount)
      goto lines_done;
    {
      s32 s2 = sine(b);
      s32 s1 = sine(a);

      *line = ((s2 * 7 + s1 * 3) >> 19) + x;
      i++;
      line++;
      a += stepA;
      b += stepB;
    }
    if (i != 160)
      goto next_line;
  lines_done:
    line = Data_02006340;
    *(volatile u16 *)0x04000018 = line[0];
    dma = (volatile u16 *)0x040000b0;
    dma[5] &= 0xc5ff;
    dma[5] &= 0x7fff;
    (void)dma[5];
    Dma_Set(line + 1, (void *)0x04000018, 0xa2600001, (volatile u32 *)dma);
  }
}
