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
      value = input[readOffset];
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
