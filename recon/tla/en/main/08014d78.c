#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "RAM_BUFFER.H"

extern u8 gWorkSlot[];
struct HeapState { void *next_ewram; void *next_iwram; u8 entries[248]; };

u32 Runtime_BumpAllocate(s32 size)
{
    u32 *allocator_state = (u32 *)((u32)&Data_03001e50);
    u32 next_address;
    u32 next;
    u32 allocation_address;
    u32 aligned_words = ((u32)size + 3) >> 2;

    allocation_address = allocator_state[1];
    size = (s32)(aligned_words << 2);
    next = allocation_address + (u32)size;
    if (next >= (u32)Ram_IwramHeapEnd) {
        allocation_address = allocator_state[0];
        next_address = allocation_address + (u32)size;
        if (next_address >= 0x02040000U) {
            return 0U;
        }
        allocator_state[0] = next_address;
        goto block_5;
    }
    allocator_state[1] = next;
block_5:
    return allocation_address;
}
