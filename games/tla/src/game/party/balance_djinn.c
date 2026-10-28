#include "types.h"
#include "sequence_parent_work.h"
#include "owner_state.h"

s32 Func_080ae2fc(s32 unit, s32 *element_out, s32 *index_out);

/// Until the Djinn counts of the active owners are within one, moves the last
/// Djinni of the owner holding most to the one holding least.
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
      s32 owner = gSequenceParentWork.activeOwners[i];
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
