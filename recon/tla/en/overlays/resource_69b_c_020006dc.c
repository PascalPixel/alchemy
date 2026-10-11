/* Draft of overlay 69b Func_020006dc. differ 2 of 17: the listing loads
 * gPartyState's address before it builds the field offset 0x212; this draft
 * builds the first half of the offset before the load. The twins in overlays
 * 6a2 and 66f hold the load first with an asm barrier. */
#include "types.h"
#include "party_state.h"

void Func_02002304(s32 x, s32 z, s32 a2, s32 a3);

void Func_020006dc(void)
{
    if (gPartyState.render_mode == 10)
        Func_02002304(0x1500000, 0x2500000, 0, 255);
}
