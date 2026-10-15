/* Near miss, differ 6 of 30, order only: the native loads the address of
 * gData_02006294 first and sets the call argument (movs r0, #0) between the
 * mask and the and; a one-pass block before the bit-field store leaves
 * differ 4. */
#include "../../../../games/tla/src/scene/kandorean_temple/local.h"

struct MapWork17 {
    u8 unk00[0x17];
    u8 bits0 : 2;
    u8 mode : 2;
};

void Func_02003a14(void)
{
    struct MapWork17 *map = (struct MapWork17 *)Ram_HeapSlots->mapWork;

    gData_02006294 = 0;
    map->mode = 1;
    Func_02005338(0);
    if (gPartyState.entrance == 1 || gPartyState.entrance == 2)
        GameFlag_ClearBit(0x12f);
}
