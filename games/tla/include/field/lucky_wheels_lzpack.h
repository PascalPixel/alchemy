#ifndef ALCHEMY_LUCKY_WHEELS_LZPACK_H
#define ALCHEMY_LUCKY_WHEELS_LZPACK_H

/// @file
/// The LZ packer's work: hash chains over a 1024-byte window, one chain per
/// byte value, and the output it is building. It borrows gFlash_Handler3 as
/// its work pointer.

#include "edition.h"
#include "types.h"
#include "static_assert.h"
#include "sound_driver_symbols.h"

struct LzPackNode {
  struct LzPackNode *next; ///< Next node with the same value.
  struct LzPackNode *prev; ///< Previous node, or the bucket field at the head.
  s32 slot;
};

struct LzPackWork {
  struct LzPackNode nodes[0x400];
  struct LzPackNode *bucket[0x100];
  s32 flagMask;
  s32 bucketBySlot[0x400];
  s32 outCnt;
  u8 outBuf[0x24];
  s32 matchOffset;
  s32 matchLen;
  s32 pos;
  u32 inputCursor;
  s32 outTotal;
  u32 inputLimit;
};

STATIC_ASSERT(sizeof(struct LzPackNode) == 12);
STATIC_ASSERT(sizeof(struct LzPackWork) == 0x4444);
STATIC_ASSERT(offsetof(struct LzPackWork, bucket) == 0x3000);
STATIC_ASSERT(offsetof(struct LzPackWork, inputCursor) == 0x4438);

void LzPack_ResetSlotBuckets(void);
void LzPack_InsertSlotNode(s32 slot);
void LzPack_RemoveSlotNode(s32 slot);
void LzPack_ConsumeSlotBytes(s32 start, s32 count, const u8 *input);
void LzPack_CopyBufferedBytes(u8 *destination);
s32 LzPack_PackStream(const u8 *input, u8 *destination, s32 size);

#endif
