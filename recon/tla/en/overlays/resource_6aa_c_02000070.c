/* Draft of overlay 6aa Func_02000070. differ 2 of 6: the listing builds the
 * value (r1 = 65) before the index (r0 = 0); this draft builds the index
 * first. An inline forwarding helper and a zero local shared with the return
 * value gave the same order. */
#include "types.h"

void Func_0200246c(s32 index, s32 value);

s32 Func_02000070(void)
{
    Func_0200246c(0, 65);
    return 0;
}
