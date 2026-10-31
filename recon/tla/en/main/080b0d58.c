// Near miss, differ 2 of 85 (tla-en, 2026-10-30): scheduler order only. In
// the loop's second copy of the found branch the native stores the lowered
// count before it sets found to 1 in r2; this draft sets r2 first, as both
// copies do here (the first copy matches). Ported from The Broken Seal's
// DjinnRecovery_Remove.
// The sched2 dump says why: nothing later in that block reads the count
// store, so it ranks priority 1 against r2 = 1's 2 (r2 = 1 feeds r9). The
// native order needs the store level with it, as a store latency of 2 would
// give: the store then wins on its data dependence on the subtraction, while
// in the first copy r1 = 1 still wins on its two dependents. That is the
// cost model, which no source form reaches; only a barrier in the second copy
// alone would, and the loop's exit-test copy duplicates an empty asm into
// both. No empty asm before any one statement lowers it. found set before the
// count, a block between them, count - 1 written out, found as u32, u8 or
// s16, the match written as a continue, or the count lowered after the loop
// differ as much or more; permute (3 min, then 4 min on 2026-10-30) found
// nothing.
#include "types.h"
#include "owner_state.h"

struct TradeOfferList {
  struct TradeOffer offers[72];
  s32 count;
};

struct TradeOfferTable {
  u8 unk00[8];
  struct TradeOfferList list;
};

s32 DjinnRecovery_Remove(s32 owner, s32 index, s32 bit) {
  struct TradeOfferList *table;
  s32 found = 0;
  s32 i;

  table = &((struct TradeOfferTable *)Djinn_GetRecoveryTable(
                (owner < 0 || owner > 7)))
               ->list;
  for (i = 0; i < table->count; i++) {
    if (index == table->offers[i].element && bit == table->offers[i].djinn) {
      table->count--;
      found = 1;
      break;
    }
  }
  for (; i < table->count; i++)
    table->offers[i] = table->offers[i + 1];
  return found;
}
