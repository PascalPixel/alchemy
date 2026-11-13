#ifndef ALCHEMY_CELL_WITH_BIT_H
#define ALCHEMY_CELL_WITH_BIT_H

/// @file
/// The cell a 16.16 position stands in with one bit of a row ORed in, as the
/// Jupiter and Mars Lighthouse rooms' and Anemos's cell targets compute it
/// at 18 sites. The name is ours.

#include "types.h"

/// The whole cell of position, a 16.16 coordinate, ORed with bit.
static inline s32 CellTarget_CellWithBit(s32 position, s16 bit) {
  return (position >> 20) | bit;
}

#endif
