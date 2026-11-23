#ifndef ALCHEMY_CELL_TARGET_H
#define ALCHEMY_CELL_TARGET_H

/// @file
/// The cracked floor cells that Jupiter Lighthouse, Mars Lighthouse and Anemos
/// Inner Sanctum watch (scene/shared/cracked_tile_*.h), and the sprite
/// field/map_cell_sprite.c draws over the one the leader stands on.

#include "fixed_math.h"
#include "types.h"
#include "field/field_coordinate.h"

/// One watched cell, in map cells.
struct CellTargetEntry {
  s16 x;
  s16 z;
  s16 flag;   ///< set the first time the leader steps on the cell
  s16 unused; ///< no code names it
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
  s16 count;           ///< entries in use
  s16 current;         ///< the entry the leader stands on, or -1
  s16 shown;           ///< nonzero draws the sprite
  s16 tile;            ///< resource entry of the sprite's tiles
  u16 unusedAfterTile; ///< no code names it
  /// The sprite as Oam_AddSpriteAtDepth takes it: the queue's link, then OAM
  /// attributes 0 and 1 in one word and attribute 2 in the next.
  unsigned long oam[3];
  /// Room for 128, as the ROM spaces Mars Lighthouse room F's table.
  struct CellTargetEntry entries[128];
};

/// The bytes of the hole mask's picture (a CellTarget's tile), the sprite drawn
/// in the OBJ window over the leader's cell as he sinks through the floor: 16
/// by 32 pixels, eight tiles of colour 1. TILE_SIZE_4BPP is system/io_reg.h's.
#define HOLE_MASK_BYTES (8 * TILE_SIZE_4BPP)

/// The map cell of a 16.16 coordinate with a watched cell's 64 bit ORed in,
/// as each scene's copy of the cracked tile handler builds its row.
static inline s32 CellTarget_CellOf(FIXED position, s16 bit) {
  return FIELD_CELL(position) | bit;
}

#endif
