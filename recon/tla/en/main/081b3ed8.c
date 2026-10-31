// Near miss, differ 2 of 102 (tla-en, 2026-10-31): only the cursor's store,
// which the native issues before the limit offset's shift. In sched2 both are
// ready together on the dependences the native has, and the shift ranks first
// (priority 7, the store 6: shift, add, the limit's address, its load); only a
// dearer store or anti-dependence would put the store first, so the C cannot
// move it. Also tried for it, none lower: the cursor read as cursor++ or
// ++readOffset, the compare on a reread cursor or with swapped operands,
// *(input + readOffset), and a four-minute permuter run; a greedy device
// search earlier found nothing lower either. The second loop's removal call
// needs no one-pass block any more (differ 2 with or without it).
// The Broken Seal's LzPack_ConsumeSlotBytes. An earlier measurement differed
// in 6 with three such swaps (also the work pointer's pool load before 0x3ff's
// shift and the root's load inside the bucket offset's split constant); then
// the root pinned to r9 (16) or, declared before the mask, to r0 (29), and
// declared before the mask unpinned (16) did no better.
#include "lucky_wheels_lzpack.h"

void LzPack_ConsumeSlotBytes(s32 start, s32 count, const u8 *input) {
  s32 limit = count;
  s32 current = 0;
  u32 base = start;

  if (current < limit) {
    u32 mask = 0x3ff;
    u32 removal = base + 0x124;

    do {
      u32 slot = base + current;
      struct LzPackWork *state;
      u32 readOffset;
      u32 nextOffset;
      u8 value;

      LzPack_RemoveSlotNode(removal & mask);
      state = gFlash_Handler3;
      readOffset = state->inputCursor;
      /* FAKEMATCH(one-pass): the byte read in a once-only block; as a plain
         statement, 4 of 102 instructions differ in tla-en. */
      do {
        value = input[readOffset];
      } while (0);
      nextOffset = readOffset + 1;
      state->inputCursor = nextOffset;
      if (nextOffset == state->inputLimit) {
        state->bucketBySlot[slot & mask] = -1;
        break;
      }

      state->bucketBySlot[slot & mask] = value;
      current++;
      LzPack_InsertSlotNode(slot & mask);
      removal++;
    } while (current < limit);
  }

  current++;
  if (current < limit) {
    u32 mask = 0x3ff;
    struct LzPackWork **root = &gFlash_Handler3;
    s32 empty = -1;

    do {
      u32 slot = (base + current) & mask;
      struct LzPackWork *state;

      LzPack_RemoveSlotNode(slot);
      state = *root;
      current++;
      state->bucketBySlot[slot] = empty;
    } while (current < limit);
  }
}
