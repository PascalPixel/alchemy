#ifndef ALCHEMY_LUCKY_WHEELS_LZPACK_H
#define ALCHEMY_LUCKY_WHEELS_LZPACK_H

/// @file
/// The LZ packer's work: hash chains over a 1024-byte window, one chain per
/// byte value, and the output it is building.

#include "edition.h"
#include "static_assert.h"
#include "types.h"

/// A position of the LZ packer's window in the chain of the byte value it
/// holds.
struct LzPackNode {
  struct LzPackNode *next; ///< Next node with the same value.
  struct LzPackNode
      *previous; ///< Previous node, or the bucket field at the head.
  s32 slot;      ///< its window position, 0 to 1023
};

/// The LZ packer's work: a node per window position, a chain head per byte
/// value, the byte at each position, the best match and the output.
struct LzPackWork {
  struct LzPackNode nodes[1024];  ///< one per window position
  struct LzPackNode *bucket[256]; ///< each byte value's chain, or null
  s32 flagMask;                   ///< the flag bit of the next item, 0x80 first
  s32 bucketBySlot[1024];         ///< each position's byte; -1 past the input
  s32 outCount;                   ///< bytes of outBuffer in use
  /// The group being built: its flag byte, then up to eight items.
  u8 outBuffer[36];
  s32 matchOffset; ///< the best match's distance back (LzPack_FindWindowMatch)
  s32 matchLength; ///< its length; 1 for none
  s32 position;    ///< the window position being packed
  u32 inputCursor; ///< input bytes read into the window
  s32 outTotal;    ///< bytes written to the output
  u32 inputLimit;  ///< the input's size
};

STATIC_ASSERT(sizeof(struct LzPackNode) == 12);
STATIC_ASSERT(sizeof(struct LzPackWork) == 0x4444);
STATIC_ASSERT(offsetof(struct LzPackWork, bucket) == 0x3000);
STATIC_ASSERT(offsetof(struct LzPackWork, inputCursor) == 0x4438);

/// The packer has no cell of its own for its work pointer: it borrows the flash
/// library's, which IdentifyFlash sets again before the flash code reads it.
extern struct LzPackWork *gWaitForFlashWrite;
#define gLzPackWork gWaitForFlashWrite

#endif
