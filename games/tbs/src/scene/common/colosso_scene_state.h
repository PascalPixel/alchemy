/// @file
/// The Colosso finals' three courses (Colosso Log, River and Wall): a helper
/// for the race gauge that no scene calls.
#include "types.h"
#include "heap_state.h"
#include "colosso_gauge.h"

/// Sets the race gauge's hold, which keeps the gauge on the screen.
static void UNUSED Colosso_SetGaugeHold(s16 v) {
  struct ColossoGaugeWork *gauge = WORK_SLOT(HEAP_SLOT_COLOSSO_RACE);

  gauge->hold = v;
}
