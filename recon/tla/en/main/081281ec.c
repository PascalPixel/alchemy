#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"

void Func_081281ec(void)
{
    u8 *battle = (u8 *)Ram_HeapSlots->battle_work;
    s32 *totals = (s32 *)(battle + 0x550);
    u16 *slots;
    s32 i;

    gPartyState.tally_25c = 0;
    totals[0] = 0;
    totals[1] = 0;
    totals[2] = 0;
    slots = (u16 *)(battle + 0x55c);
    for (i = 3; i >= 0; i--)
        slots[i] = 0;
}
