#ifndef ALCHEMY_CELL_TARGET_H
#define ALCHEMY_CELL_TARGET_H

/// @file
/// The table of watched cells that the Jupiter and Mars Lighthouse overlays and
/// overlay 6B0 keep, and field/map/cell_sprite.c's sprite reads.

#include "types.h"

/// One watched cell, in map cells.
struct CellTargetEntry {
  s16 x;
  s16 z;
  s16 flag; ///< set the first time the leader steps on the cell
  s16 unk06;
};

/// A map's watched cells and the sprite drawn over the current one.
struct CellTarget {
  s16 firstFlag; ///< the first entry's flag; each later entry takes the next
  /// The first of two flag bytes that keep the leader's cell, x then z (at
  /// flagByte + 8).
  s16 flagByte;
  s16 kind; ///< the cell event marking a watched cell, kind or kind + 1
  s16 x;    ///< column of the copied cell, whose row is z + 1
  s16 z;
  s16 count;   ///< entries in use
  s16 current; ///< the entry the leader stands on, or -1
  s16 shown;   ///< nonzero draws the sprite
  s16 tile;    ///< resource entry of the sprite's tiles
  u16 unk12;
  u32 oam[3]; ///< the sprite's attributes, as Oam_AddSpriteAtDepth takes them
  struct CellTargetEntry entries[1];
};

#endif
