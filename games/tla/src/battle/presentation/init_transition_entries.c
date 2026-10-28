#include "types.h"
#include "transform.h"

struct BattleTransitionEntry {
  u32 value;
  u32 sum;
  u32 field8;
  u32 fieldc;
};

void BattleTransition_InitializeEntries(
    struct BattleTransitionEntry *entries) {
  u32 previous = entries[0].value;

  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_SetIdentity(entries);
  entries[0].sum = previous + entries[0].value;
}
