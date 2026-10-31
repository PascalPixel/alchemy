// Near miss, differ 4 of 73 (tla-en, 2026-10-31): only the outer
// preheader's order is left, a sched2 cost the C cannot move. Its seven
// insns and their dependences are the same as the native's: 4 * height - 2
// heads a chain of three (lsls r3, subs r3, mov lr; priority 3), the width's
// shift a chain of two (lsls r2 and its spill store, which costs 1 under
// the arm9tdmi model; priority 2), so sched2 issues 4 * height first and
// the spill after the copies into r8 and r9. The native order (lsls r2,
// lsls r3, str r2, subs r3) needs the store to cost 2; nothing in the block
// can depend on it. Permute 4 minutes on 2026-10-31: nothing below 4.
// 2026-10-25, with the Makefile's options (-mthumb-slow-loads): differ 4 of
// 73: the inner preheader now matches. A greedy device search found nothing
// lower.
// Earlier draft, differ 6 of 73: mirrors a width x height block of bytes into a
// 2*width x 2*height one, each byte written to all four quadrants (no
// twin: The Broken Seal has none). Left, all scheduling: in the outer
// preheader the native shifts the width (lsls r2, r6, #1) before
// 4 * height and stores it to the stack at once; here 4 * height comes
// first and the store last. In the inner preheader the native reloads src
// (ldr r1, [sp, #4]) before 2 * width; here after. Tried: the index forms
// (only y * width * 2 for the top-left and y * 2 * width for the top-right,
// as here, give the native's induction variables), row pointers (109), an
// explicit stride (118), unsigned counters or sizes, the byte as s32, a
// const source, alchemy permute 4 minutes (no candidate below 6). By hand
// through sched2's ranking, a store costing 2 cycles instead of 1 gives
// the outer preheader's native order (the spilled width then ties 4 *
// height and wins on insn order), and a load costing 3 the inner one's.
// 2026-10-18: the strides as (width * 2), a w2 local, 2 * width, the four
// stores reordered, unsigned sizes, y < height, alchemy permute 4 minutes
// (6 to 77; none below 6).
#include "types.h"

void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height) {
  s32 x;
  s32 y;

  for (y = 0; y != height; y++) {
    for (x = 0; x != width; x++) {
      u8 c = src[y * width + x];

      dst[y * width * 2 + x] = c;
      dst[y * 2 * width + (width * 2 - 1 - x)] = c;
      dst[(height * 2 - 1 - y) * 2 * width + x] = c;
      dst[(height * 2 - 1 - y) * 2 * width + (width * 2 - 1 - x)] = c;
    }
  }
}
