#ifndef ALCHEMY_OAM_H
#define ALCHEMY_OAM_H

/// @file
/// OAM usage counts.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

/// How many OAM entries are in use, and the most that may be.
struct OamUsage {
  u16 used;  ///< not named by TBS code
  u16 limit; ///< not named by TBS code
};

STATIC_ASSERT(sizeof(struct OamUsage) == 4);
STATIC_ASSERT(offsetof(struct OamUsage, limit) == 2);

extern struct OamUsage gOamUsage;

#endif
