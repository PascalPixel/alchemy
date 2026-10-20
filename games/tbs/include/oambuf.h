#ifndef ALCHEMY_OAMBUF_H
#define ALCHEMY_OAMBUF_H

/// @file
/// OamShadow, a sprite entry a routine builds by hand and DMA copies to OAM.

#include "types.h"

/// One sprite's two OAM words. Both are volatile: LuckyDice_Run,
/// ReelGame_RunFrame and Title_UpdateIntroSprites, which build lists of them,
/// each tag what that steers.
struct OamShadow {
  volatile u32 attr01; ///< attr0 | attr1 << 16
  volatile u32 attr2;  ///< attr2 | affine value << 16
};

#endif
