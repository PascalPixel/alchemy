/* Near miss, differ 4: the native loads the menu work before it starts the 0x504 count
 * offset, and loads the multiplier table address one instruction earlier; this draft
 * starts the offset first. An empty asm barrier after the work load gives both orders
 * but then keeps sub sp, #4 above it (the native schedules it after the count test is
 * built); a one-pass block does the same. */
#include "types.h"
#include "ram_buffer.h"

struct InnWork {
    u8 unk000[0x4ca];
    s16 members[29];
    s8 memberCount;
};

void *UnitGet(s32 unitId);
extern const s8 gData_0810cf2c[];

s32 Func_0810b378(s32 mode)
{
    struct InnWork *work = Ram_HeapSlots->menuRuntime;
    s32 active = 0;
    s32 factor = gData_0810cf2c[mode];
    s32 index;

    for (index = 0; index < work->memberCount; index++) {
        if (*(s16 *)((u8 *)UnitGet(work->members[index]) + 56) != 0)
            active++;
    }

    return factor * active;
}
