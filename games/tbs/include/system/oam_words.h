#ifndef ALCHEMY_OAM_WORDS_H
#define ALCHEMY_OAM_WORDS_H

/// @file
/// OamWords, a sprite entry a routine builds by hand and DMA copies to OAM.

#include "types.h"

/// One sprite's two OAM words.
// volatile: the words are built in place for the DMA to copy to OAM.
struct OamWords {
  volatile u32 attr01; ///< attr0 | attr1 << 16
  volatile u32 attr2;  ///< attr2 | affine value << 16
};

#endif
