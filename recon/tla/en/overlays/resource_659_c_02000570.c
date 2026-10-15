/* Near miss, differ 24 of 72: the listing loads x = 0x02000000 for actor 9 from the
 * literal pool in both branches, where this draft builds it as 128 << 18 (the
 * split constant; 0x1d80000 for actor 10 is built in both). No compiled ⚓️ code
 * pools 0x2000000. Measured: x and z in locals, 512 << 16, a direct call, a
 * pixel-coordinate inline that shifts (all 24 to 26). The 0xd4c store matches
 * through the one-pass block that Func_02000624 also takes. */
#include "../../../../games/tla/src/field/overlay_659/ov659.h"

void Func_02000570(void)
{
    void *event;

    if (!FlagCheck(0x8ab)) {
        do {
            event = Ram_HeapSlots->event_work;
        } while (0);
        FIELD_AT(event, u16, 0xd4c) = 3;
        if (!FlagCheck(0x911)) {
            Func_020007dc();
        } else {
            ActorPlace(9, 0, 0);
            ActorPlace(10, 0, 0);
        }
    } else if (FlagCheck(0x8ff)) {
        ActorPlaceFacing(9, 0x2000000, 0x3490000, 0xe000);
        ActorPlaceFacing(10, 0x1d80000, 0x36d0000, 0xc000);
        ActorPlace(8, 0, 0);
    } else {
        ActorPlaceFacing(9, 0x2000000, 0x3490000, 0xa000);
        ActorPlaceFacing(10, 0x1d80000, 0x36d0000, 0xc000);
        ActorPlace(8, 0, 0);
    }
}
