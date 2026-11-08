// Matches: differ 0 (tla-en, 2026-11-08)
// The wavy scroll lines of the layer, read by a DMA each scanline. Differ 0 in all six editions (tla-en, tla-ja, tla-de, tla-es, tla-fr, tla-it; -Os, -mtune=gba), so ready to adopt into resource_65f's overlay in every edition once the full comparison agrees.
// 2026-11-08 (last): 2 to 0. The layer store now comes first and i = 0 with its barrier after frame is read: with i not yet 0 at the store, CSE stores the event flag's register (r3, known 0 on that path) as the native does, and i keeps its refs and live length, so it still ranks before b (r6, r7). The barrier before the store with i = 0 at the top 14 (i and b swap), i = 0 and the barrier just before the loop 10 (sine and x swap, r9 and fp). Re-measured in the matching form: without the barrier 56, without the p pin 64, without the s1 pin 36, without both pins 50; s1 (and s2) unpinned at the block's or the if's scope 36, p unpinned at the if's scope 64, writing through line itself 70. All three devices remain needed.
// 2026-11-08 (later): 48 to 2. The loop steps a before b (12 with the pins below; b first had suited only the old allocation), x is declared first so it wins its priority tie with sine (r9, sine fp), and two pins put the pointer in r5 and the first sine in r8, where local-alloc otherwise gives s1 the lowest free register. Re-measured: without the barrier 58, without the p pin 66, without the s1 pin 38, without both 52. The store before i = 0 (14: i and b then swap), the flag stored as itself or i = flag (2), the store in a do { } while (0) (4), the barrier after frame (2) or before the loop (12), x read later (46), i < 160 (30).
// 2026-11-08: the loop writes through a block-scoped pointer copied from line (54 to 48: line then takes a low register outside the loop, as the native's r5). Re-measured: without the barrier 78. Not better: p from Data_02006340 (48), line block-scoped at the reload (68), the store before i = 0 (60), line++ as its own statement (54), the steps in the for list (54), sine calls inline (54). 8 minutes of permute: nothing better.
// Before the barrier (2026-11-06), one cause: the native keeps a test at the loop's top (cmp r6, #160, beq) with i known to be 0; here CSE folds it, whatever the
// spelling (for, while, labels, if around a do). With those refs i ranks first in the native (r6), line r5, the first sine r8; here line r8, the first sine r5, i r10.
// 2026-11-06: jump.c copies the exit test before the loop and CSE folds it in every plain form (for, while, for(;;) with break, i set from the stored flag, a known-true inner if); a goto loop keeps the top test but is never rotated (83). In the native s1 also goes to r8, not local-alloc's r5, so it is live outside one block there. Indexing Data_02006340[i] 95, the comma increment 74, no step locals 114.
// 2026-11-06 (later): i set before the event test 72, u32 i 72, i < 160 85, s16 i 87, u8 i 84, i = 0 in the for 84; the permuter's best (63) passes a stale a to sine, so is wrong.
// 2026-11-06 (RTL dumps): a do { i = 0; } while (0) keeps the top test through cse1, but cse2 folds it (70). An asm("" : "+r"(i))
// after i = 0 (barrier, diagnostic only, not kept) keeps it to the end: cmp, beq at the top as in the native, differ 68, the same
// as a manually rotated goto loop with that barrier. What is left then is allocation alone: s1 is local (r5) and line ranks last
// (sl); the native has line r5, i r6, b r7, s1 r8, a sl. s1 and s2 at the block's scope, a separate post-loop pointer: no change.
// 12 minutes of permute on the barrier form: best 54, every candidate below 68 stores through a stale pointer (wrong program).
// 2026-11-07: the barrier taken (S2, now allowed in this game); placed after the layer store rather than before it 58, with b stepped before a 54. Without it the same form is 74. 5 minutes of permute: best 42 again stores through a stale pointer.
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
  // Declared first: x then wins its allocation tie with sine.
  s32 x;
  s32 (*sine)(s32) = *(s32 (**)(s32))(((u32)Math_Sine & ~3) + 4);
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[1];
  u16 *line = Data_02006340;

  if (*(s8 *)((u8 *)Ram_HeapSlots->eventWork + 0xca4) == 0) {
    u32 frame;
    s32 z, a, b;
    s32 stepA, stepB;
    s32 i;
    volatile u16 *dma;

    *(s32 *)&layer->unk08[20] = 0;
    frame = gFrameCount;
    i = 0;
    /* FAKEMATCH(barrier): asm("" : "+r"(i)) keeps the loop's top test; as i = 0 alone, 56 of 110 instructions differ in tla-en. */
    asm("" : "+r"(i));
    z = *(s16 *)((u8 *)layer + 6);
    x = *(s16 *)((u8 *)layer + 2);
    stepA = 0x3300;
    stepB = 0x1320;
    a = z * stepA + (frame << 10);
    b = z * stepB - (frame << 10);
    {
      /* FAKEMATCH(pin): p held in r5; as a plain local, 64 of 110 instructions differ in tla-en. */
      register u16 *p asm("r5") = line;

      for (; i != 160; i++) {
        /* FAKEMATCH(pin): s1 held in r8; as a plain local, 36 of 110 instructions differ in tla-en. */
        register s32 s1 asm("r8") = sine(a);
        s32 s2 = sine(b);
        *p++ = ((s1 * 3 + s2 * 7) >> 19) + x;
        a += 0x3300;
        b += 0x1320;
      }
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
