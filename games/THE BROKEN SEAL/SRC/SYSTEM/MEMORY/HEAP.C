#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "RAM_BUFFER.H"

extern u8 Data_03007800[];
extern u8 Data_03001e50[];

s32 Runtime_GetRemainingIwram(void)
{
    s32 state = ((u32)&Data_03001e50);

    return (s32)Data_03007800 - *(s32 *)(state + 4);
}

s32 Runtime_GetRemainingEwram(void)
{
    return 0x02040000 - *(s32 *)((u32)&Data_03001e50);
}

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

void *Runtime_AllocateBlock(s32 kind, s32 size)
{
    u32 *allocator_state;
    s32 kind_offset;
    u32 aligned_size;
    u32 next;
    u32 address;
    u32 next_address;
    u32 cached_address;

    allocator_state = (u32 *)((u32)&Data_03001e50);
    kind_offset = kind * 4;
    cached_address = *(u32 *)((u8 *)allocator_state + kind_offset);
    if (cached_address == 0) {
        address = allocator_state[0];
        aligned_size = (((u32)size + 3) >> 2) * 4;
        next = address + aligned_size;
        if (next >= (u32)(129 << 18)) {
            address = allocator_state[1];
            next_address = address + aligned_size;
            if (next_address >= (u32)Ram_IwramHeapEnd) {
                return NULL;
            }
            allocator_state[1] = next_address;
            *(u32 *)((u8 *)allocator_state + kind_offset) = address;
            return (void *)address;
        }
        allocator_state[0] = next;
        *(u32 *)((u8 *)allocator_state + kind_offset) = address;
        return (void *)address;
    }
    return (void *)cached_address;
}

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

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

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

void RuntimeMemory_ReservedNoOp(void)
{
}
