#ifndef ALCHEMY_PALETTE_SNAPSHOTS_H
#define ALCHEMY_PALETTE_SNAPSHOTS_H

/// @file
/// Palette cycles: runs of background palette colours that rotate one place
/// every few frames, up to four at once.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

/// A palette cycle: a run of up to 16 background palette colours, saved and
/// rotated by one colour every delay + 1 frames.
struct PaletteCycle {
  u16 *source;    ///< the first colour of the run, in BG palette RAM
  u16 position;   ///< colours rotated so far, 0 to count - 1
  u16 timer;      ///< frames until the next rotation
  s16 delay;      ///< frames; the timer restarts from it
  u16 count;      ///< colours in the run
  u16 colors[16]; ///< the run's colours as saved when the cycle was added
};

/// The palette cycling queue (HEAP_SLOT_PALETTE_CYCLES): four cycles and how
/// many are in use.
struct PaletteCycleState {
  struct PaletteCycle entries[4];
  /// Cycles in use, 0 to 4; PaletteCycle_Step runs none when all four are
  /// taken (a BUG there).
  u16 count;
};

STATIC_ASSERT(sizeof(struct PaletteCycle) == 44);
STATIC_ASSERT(offsetof(struct PaletteCycle, position) == 4);
STATIC_ASSERT(offsetof(struct PaletteCycle, timer) == 6);
STATIC_ASSERT(offsetof(struct PaletteCycle, delay) == 8);
STATIC_ASSERT(offsetof(struct PaletteCycle, colors) == 12);
STATIC_ASSERT(sizeof(struct PaletteCycleState) == 180);
STATIC_ASSERT(offsetof(struct PaletteCycleState, count) == 176);

/// The palette cycling queue, in its heap slot (HEAP_SLOT_PALETTE_CYCLES).
#define gPaletteState ((struct PaletteCycleState *)Ram_HeapSlots->paletteCycles)

void PaletteCycle_Initialize(void);
s32 PaletteCycle_Add(s16 paletteNumber, s16 firstColor, s16 delay,
                     s16 colorCount);
void PaletteQueue_Stop(void);
void PaletteCycle_Step(void);

#endif
