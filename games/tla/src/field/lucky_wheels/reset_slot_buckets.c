#include "lucky_wheels_lzpack.h"

void LzPack_ResetSlotBuckets(void) {
  s32 index;
  struct LzPackNode **bucket;

  for (index = 0; index < 1024; index++) {
    gFlash_Handler3->nodes[index].slot = index;
    gFlash_Handler3->nodes[index].prev = NULL;
  }
  bucket = gFlash_Handler3->bucket;
  for (index = 255; index >= 0; index--) {
    *bucket++ = NULL;
  }
}
