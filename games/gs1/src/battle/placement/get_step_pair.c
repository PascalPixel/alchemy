#include "types.h"

/* Step table for battle placement: signed bytes in (x, y) pairs. */
extern const s8 Data_080c2a62[];

/*
 * Read one (x, y) pair from the step table.  Entries are pairs, so the index
 * is doubled and the second component read at index + 1.  A leaf with no
 * prologue; the owner includes the single pool word holding the table base.
 * No call site is known -- the address is reached by a computed value, or by
 * nothing at all.
 */
void BattlePlacement_GetStepPair(s32 index, s32 *x, s32 *y)
{
    index *= 2;
    *x = Data_080c2a62[index];
    *y = Data_080c2a62[index + 1];
}
