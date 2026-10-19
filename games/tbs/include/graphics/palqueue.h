#ifndef ALCHEMY_PALQUEUE_H
#define ALCHEMY_PALQUEUE_H

/// @file
/// Palette snapshots and their work.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

/// A palette cycle: a run of up to 16 background palette colours, saved and
/// rotated by one colour every delay + 1 frames.
struct PaletteSnapshot {
  u16 *source;
  u16 position;
  u16 timer; ///< frames
  s16 delay; ///< frames
  u16 count;
  u16 colors[16];
};

/// The palette cycling queue (heap slot 28): four cycles and how many are in
/// use.
struct PaletteSnapshotWork {
  struct PaletteSnapshot entries[4];
  u16 count;
};

STATIC_ASSERT(sizeof(struct PaletteSnapshot) == 0x2c);
STATIC_ASSERT(offsetof(struct PaletteSnapshot, position) == 4);
STATIC_ASSERT(offsetof(struct PaletteSnapshot, timer) == 6);
STATIC_ASSERT(offsetof(struct PaletteSnapshot, delay) == 8);
STATIC_ASSERT(offsetof(struct PaletteSnapshot, colors) == 0xc);
STATIC_ASSERT(sizeof(struct PaletteSnapshotWork) == 0xb4);
STATIC_ASSERT(offsetof(struct PaletteSnapshotWork, count) == 0xb0);

#endif
