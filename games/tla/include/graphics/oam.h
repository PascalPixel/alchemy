#ifndef ALCHEMY_OAM_H
#define ALCHEMY_OAM_H

#include "edition.h"
#include "types.h"
#include "static_assert.h"

/// How many OAM entries are in use, and the most that may be.
struct OamUsage {
  u16 used;  ///< 32-bit words DMA copies to OAM each frame; 0 when reset
  u16 limit; ///< OAM_SIZE / 4, all of OAM, when reset
};

STATIC_ASSERT(sizeof(struct OamUsage) == 4);
STATIC_ASSERT(offsetof(struct OamUsage, limit) == 2);

/// The OAM shape and size a window sprite is created with
/// (WindowSprite_Create): attribute 0's shape in the low half, attribute 1's
/// size in the high half, as the hardware reads them.
#define OUTPUT_SIZE_16X16 0x40000000
#define OUTPUT_SIZE_32X32 0x80000000
#define OUTPUT_SIZE_32X8 0x40004000
#define OUTPUT_SIZE_32X16 0x80004000
#define OUTPUT_SIZE_16X32 0x80008000

/// The OAM entries the frame's list holds and the most it may; wait() empties
/// it each frame.
extern struct OamUsage gOamUsage;
/// Builds this frame's OAM list in work, a frame-scratch block of OAM_SIZE,
/// running its ARM kernel from a copy on the stack.
void Render_BuildOamList(void *work);

#endif
