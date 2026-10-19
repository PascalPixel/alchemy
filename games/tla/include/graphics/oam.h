#ifndef ALCHEMY_OAM_H
#define ALCHEMY_OAM_H

#include "edition.h"
#include "types.h"
#include "static_assert.h"

/// How many OAM entries are in use, and the most that may be.
struct OamUsage {
  u16 used;  ///< 32-bit words DMA copies to OAM each frame; 0 when reset
  u16 limit; ///< 256 when reset
};

STATIC_ASSERT(sizeof(struct OamUsage) == 4);
STATIC_ASSERT(offsetof(struct OamUsage, limit) == 2);

extern struct OamUsage gOamUsage;
void Render_BuildOamList(void *work);

#endif
