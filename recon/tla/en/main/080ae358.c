// Near miss, differ 2 of 76 (one reordered pair), 2026-10-15: written from
// the listing: until the djinn counts of the active owners are within one,
// moves the last djinn of the owner holding most to the one holding least.
// The transfer's result reuses the sum (a fresh local is not copied to r2,
// differ 4), and the sum is cleared before the counts' address is formed
// (differ 4 after it). Remaining: after Unit_Get the native restores the
// lowest sum from the stack (ldr r4, [sp]) before it doubles the counts'
// offset (lsls r3, r3, #1); this doubles first (equal priority, the shift
// has a dependent). An asm tied to the sum lowers the restore's priority
// (asm links cost nothing); seven minutes of alchemy permute found
// nothing lower.
#include "types.h"
#include "party_state.h"
#include "owner_state.h"

s32 Func_080ae2fc(s32 unit, s32 *element_out, s32 *index_out);

void Func_080ae358(void) {
  s32 count = Party_CountActiveOwners();

  for (;;) {
    s32 lowNo = 0;
    s32 high = 0;
    s32 low = 999;
    s32 highNo = 0;
    s32 i;
    s32 sum;
    s32 element;
    s32 index;

    for (i = 0; i < count; i++) {
      s32 owner = gPartyState.activeOwners[i];
      struct OwnerDjinnState *unit = Unit_Get(owner);
      u8 *counts;
      s32 j;

      sum = 0;
      counts = unit->counts.banks.owned;
      j = 3;
      do {
        u8 value = *counts;
        counts++;
        sum += value;
        j--;
      } while (j >= 0);
      if (sum > high) {
        high = sum;
        highNo = owner;
      }
      if (sum < low) {
        low = sum;
        lowNo = owner;
      }
    }
    if (high - low <= 1)
      break;
    sum = Func_080ae2fc(highNo, &element, &index);
    if (sum != 0)
      break;
    Djinn_Transfer(highNo, element, index, lowNo);
  }
}
