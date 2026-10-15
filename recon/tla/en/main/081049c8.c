/* Draft, differ 2 of 71 (2026-10-15), ported from The Broken Seal's Djinn_CountTurns: at the end
 * of the owner loop the native loads the owner count (ldrb r3) before it builds the 1 for the
 * high-register increment (movs r2, #1; add r8, r2); here the 1 comes first. The increment is
 * ahead of the test in RTL, so a barrier cannot order them. */
#include "types.h"
#include "ram_buffer.h"

struct DjinnMenuWork {
    u8 unknown_000[0x204];
    u16 owners[8];
    u8 unknown_214[1];
    u8 owner_count;
};

struct DjinnUnit {
    u8 unknown_000[0xf8];
    u32 djinn_available[4];
    u32 djinn_active[4];
};

struct DjinnUnit *UnitGet(s32 unit_id);

void Func_081049c8(u8 *counts)
{
    struct DjinnMenuWork *work;
    struct DjinnUnit *unit;
    s32 owner_index;
    s32 element;
    s32 bit;
    s32 count;

    work = Ram_HeapSlots->menu_runtime;
    for (owner_index = 0; owner_index < work->owner_count; owner_index++) {
        unit = UnitGet(work->owners[owner_index]);
        count = 0;
        for (element = 0; element <= 3; element++) {
            for (bit = 0; bit <= 19; bit++) {
                if (unit->djinn_active[element] & (1 << bit))
                    count++;
                else if (unit->djinn_available[element] & (1 << bit))
                    count++;
            }
        }
        counts[owner_index] = count;
    }
}
