#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "RAM_BUFFER.H"

extern u8 gWorkSlot[];
struct HeapState { void *next_ewram; void *next_iwram; u8 entries[248]; };

s32 Runtime_AllocateHeapBlock(s32 kind, s32 size)
{
    u32 *allocator_state;
    s32 kind_offset;
    s32 aligned_size;
    u32 address;
    u32 next_address;
    u32 next;
    u32 cached_address;

    allocator_state = (u32 *)((u32)&Data_03001e50);
    kind_offset = kind * 4;
    cached_address = *(u32 *)((u8 *)allocator_state + kind_offset);
    if (cached_address == 0) {
        cached_address = allocator_state[1];
        aligned_size = (((u32)size + 3) >> 2) * 4;
        next = cached_address + aligned_size;
        if (next >= (u32)Ram_IwramHeapEnd) {
            address = allocator_state[0];
            next_address = address + aligned_size;
            if (next_address >= 0x02040000U) {
                return 0;
            }
            allocator_state[0] = next_address;
            *(u32 *)((u8 *)allocator_state + kind_offset) = address;
            return (s32)address;
        }
        allocator_state[1] = next;
        *(u32 *)((u8 *)allocator_state + kind_offset) = cached_address;
        return (s32)cached_address;
    }
    return (s32)cached_address;
}
