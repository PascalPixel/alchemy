/* Near miss, differ 15: the listing is a leaf that computes the equality
 * without a branch (eors; negs; orrs; lsrs #31; subs r0, 1 - ...); every
 * form tried compiles to a compare and branch with push {lr}: a == b,
 * 1 - (a != b), !(a ^ b), integer casts, a result variable. */
#include "types.h"
#include "irq.h"

void DisplayTransition_UpdateScanline(void);

s32 Func_080d072c(void)
{
    return gIrqHandlers[1] == DisplayTransition_UpdateScanline;
}
