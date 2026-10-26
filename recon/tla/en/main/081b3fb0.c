// 2026-10-25, with the Makefile's options (-mthumb-slow-loads): differ 2 of
// 31 as below. An empty asm after the byte store with the for loop's own
// step: the pointer step comes before the store (2); a do-while with the
// index stepped before the asm: 18. A greedy device search and six permuter
// minutes found nothing lower.
// Near miss: differ 2 of 31, The Broken Seal's LzPack_CopyBufferedBytes. In
// the loop the native steps the source pointer (adds r1, #1) before it reloads
// outTotal after the byte store; sched2 here reloads first, as the
// store-to-load dependence costs it one cycle. One-pass blocks or empty asm
// around either statement (16 combinations), an explicit source pointer (differ
// 21), the index stepped before the store with a barrier after it (differ 4)
// and three permuter minutes found nothing better. sched2's dump shows why:
// after the store the reload (priority 7) and the pointer step (priority 1)
// are both ready, so the reload goes first; the native's order needs the
// store-to-load dependence to cost more. Also tried: outBuf[index++] (2),
// work->outTotal++ (2), the reload into a temporary pinned to r3 (2), and an
// explicit source pointer pinned to r1 with a barrier (the loop matches, but
// the pointer is set up before the guard: differ 21; set up inside the
// guard, 31 to 45).
// 2026-10-24: a volatile empty asm after the byte store, the index stepped
// in the store (outBuf[index++]) or as its own statement before the asm,
// holds the reload behind the pointer step, but loop.c places the pointer
// step right after the ldrb, so in the group before the barrier it ties the
// index step (priority, class, dependents) and wins on insn order: adds r1,
// #1 before the strb and adds r0, #1 after it, where the native has the index
// step first (differ 4). An asm on the index kills the strength reduction
// (37); the byte read into a local first swaps r2 and r3 (8).
#include "lzpack.h"

void LzPack_CopyBufferedBytes(u8 *destination) {
  struct LzPackWork *work = gFlash_Handler3;
  s32 index;

  for (index = 0; index != work->outCnt; index++) {
    destination[work->outTotal] = work->outBuf[index];
    work->outTotal = work->outTotal + 1;
  }
}
