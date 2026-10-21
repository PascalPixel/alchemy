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
  u16 *source;    ///< the first colour of the run, in BG palette RAM
  u16 position;   ///< colours rotated so far, 0 to count - 1
  u16 timer;      ///< frames until the next rotation
  s16 delay;      ///< frames; the timer restarts from it
  u16 count;      ///< colours in the run
  u16 colors[16]; ///< the run's colours as saved when the cycle was added
};

/// The palette cycling queue (HEAP_SLOT_PALETTE_CYCLES): four cycles and how
/// many are in use.
struct PaletteSnapshotWork {
  struct PaletteSnapshot entries[4];
  /// Cycles in use, 0 to 4; PaletteQueue_StepCycles masks four to none.
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
