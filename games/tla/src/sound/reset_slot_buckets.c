#include "lzpack.h"

void LzPack_ResetSlotBuckets(void) {
  /* FAKEMATCH: signed word stores preserve register allocation and the 60-byte
   * body; typed fields use 56. */
  s32 index;
  s32 limit;
  s32 zero;
  u8 *record;
  s32 *slot;

  index = 0;
  limit = 0x3FF;
  zero = 0;
  record = (u8 *)&gFlash_Handler3->nodes[0].prev;
  do {
    *(s32 *)(record + sizeof(struct LzPackNode *)) = index;
    index++;
    *(s32 *)record = zero;
    record += sizeof(struct LzPackNode);
  } while (index <= limit);
  slot = (s32 *)gFlash_Handler3->bucket;
  {
    s32 zero2 = 0;
    for (index = 0xFF; index >= 0; index--) {
      *slot++ = zero2;
    }
  }
}
