#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "RAM_BUFFER.H"

extern u8 gWorkSlot[];
struct HeapState { void *next_ewram; void *next_iwram; u8 entries[248]; };

s16 *Runtime_BumpAllocateAlternatePool(s32 arg0)
{
    s32 allocator_state_address = ((u32)&Data_03001e50);
    u32 alternate_next_address;
    u32 primary_next_address;
    u32 allocation_address;
    u32 aligned_words = ((u32)arg0 + 3) >> 2;

    allocation_address = FIELD_AT_OFFSET((void *)allocator_state_address, u32 *, 0);
    arg0 = (s32)(aligned_words << 2);
    primary_next_address = allocation_address + (u32)arg0;
    if (primary_next_address >= 0x02040000U) {
        allocation_address = FIELD_AT_OFFSET((void *)allocator_state_address, u32 *, 4);
        alternate_next_address = allocation_address + (u32)arg0;
        if (alternate_next_address >= (u32)Ram_IwramHeapEnd) {
            return NULL;
        }
        FIELD_AT_OFFSET((void *)allocator_state_address, u32 *, 4) = alternate_next_address;
        goto done;
    }
    FIELD_AT_OFFSET((void *)allocator_state_address, u32 *, 0) = primary_next_address;
done:
    return (s16 *)allocation_address;
}
