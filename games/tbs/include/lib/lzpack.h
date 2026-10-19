#ifndef ALCHEMY_LZPACK_H
#define ALCHEMY_LZPACK_H

/// @file
/// The LZ packer's work: hash chains over a 1024-byte window, one chain per
/// byte value, and the output it is building. It borrows gFlash_Handler3 as
/// its work pointer.

#include "edition.h"
#include "types.h"
#include "static_assert.h"
#include "audio_engine_symbols.h"

/// A position of the LZ packer's window in the chain of the byte value it
/// holds.
struct LzPackNode {
  struct LzPackNode *next; ///< Next node with the same value.
  struct LzPackNode *prev; ///< Previous node, or the bucket field at the head.
  s32 slot;
};

/// The LZ packer's work: a node per window position, a chain head per byte
/// value, the byte at each position, the best match and the output.
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

#endif
