/* Near miss, differ 26: the listing loads 0x02000000 from the literal pool
 * (this draft builds it as 128 << 18) and builds the 0xd4c offset after the
 * event work load. The rest of the function matches. */
#include "../../../../games/tla/src/field/overlay_659/ov659.h"

void Func_02000570(void)
{
    if (!FlagChk(0x8ab)) {
        FIELD_AT(Ram_HeapSlots->event_work, u16, 0xd4c) = 3;
        if (!FlagChk(0x911)) {
            Func_020007dc();
        } else {
            ActorPlace(9, 0, 0);
            ActorPlace(10, 0, 0);
        }
    } else if (FlagChk(0x8ff)) {
        ActorPlaceFacing(9, 0x2000000, 0x3490000, 0xe000);
        ActorPlaceFacing(10, 0x1d80000, 0x36d0000, 0xc000);
        ActorPlace(8, 0, 0);
    } else {
        ActorPlaceFacing(9, 0x2000000, 0x3490000, 0xa000);
        ActorPlaceFacing(10, 0x1d80000, 0x36d0000, 0xc000);
        ActorPlace(8, 0, 0);
    }
}
