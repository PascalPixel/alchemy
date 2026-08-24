#include "types.h"
#include "global_cells.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s16 *Runtime_BumpAllocateAlternatePool(s32 arg0)
{
    s32 allocator_state_address = ADDR_03001E50;
    u32 alternate_next_address;
    u32 temp_r0;
    u32 primary_next_address;
    u32 allocation_address;
    u32 aligned_words = ((u32)arg0 + 3) >> 2;

    allocation_address = M2C_FIELD((void *)allocator_state_address, u32 *, 0);
    arg0 = (s32)(aligned_words << 2);
    primary_next_address = allocation_address + (u32)arg0;
    if (primary_next_address >= 0x02040000U) {
        allocation_address = M2C_FIELD((void *)allocator_state_address, u32 *, 4);
        alternate_next_address = allocation_address + (u32)arg0;
        if (alternate_next_address > 0x030077FFU) {
            return NULL;
        }
        M2C_FIELD((void *)allocator_state_address, u32 *, 4) = alternate_next_address;
        goto done;
    }
    M2C_FIELD((void *)allocator_state_address, u32 *, 0) = primary_next_address;
done:
    return (s16 *)allocation_address;
}

void Func_080049a8(void)
{
}
