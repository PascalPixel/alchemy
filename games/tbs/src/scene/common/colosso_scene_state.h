/// @file
/// The Colosso finals' three courses (Colosso Log, River and Wall): a helper
/// for the race gauge that no scene calls.
#include "types.h"

#define WORK_SLOT_STAGE 59

/// Set the race gauge's hold (ColossoGaugeWork's hold, at 0xdc in the work of
/// slot 59), which keeps the gauge on the screen.
static void UNUSED Colosso_SetGaugeHold(s16 v) {
  *(s16 *)(*(u8 **)((u8 *)gWorkSlot + WORK_SLOT_STAGE * 4) + 0xdc) = v;
}
