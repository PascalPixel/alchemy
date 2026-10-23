/// @file
/// The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3):
/// a helper for the race gauge that no map calls.
#include "types.h"
#include "heap.h"
#include "colosso_gauge.h"

/// Sets the race gauge's hold, which keeps the gauge on the screen.
static void UNUSED Colosso_SetGaugeHold(s16 v) {
  struct ColossoGaugeWork *gauge = HEAP_SLOT_BLOCK(HEAP_SLOT_COLOSSO_RACE);

  gauge->hold = v;
}
