#include "TYPES.H"
#include "RAM_BUFFER.H"

/* Hands out a block for the heap slot at byte offset kind once, from IWRAM
   while it lasts and then from EWRAM below 0x02040000, and keeps it in the
   slot; ⚓️ passes the offset where ☀️ passes the slot number. Slots 0 and
   1 hold the EWRAM and IWRAM bump pointers. Returns 0 when both are full. */
s32 Runtime_AllocateHeapBlock(s32 kind, s32 size)
{
    u32 *allocator_state;
    s32 kind_offset;
    s32 aligned_size;
    u32 address;
    u32 next_address;
    u32 next;
    u32 cached_address;

    allocator_state = (u32 *)Ram_HeapSlots;
    kind_offset = kind;
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

/* Hands out a block for a byte-offset slot once, using EWRAM first and
   IWRAM when EWRAM is full. The last usable IWRAM byte precedes the
   sound mixer's workspace, so the first byte after a block may not pass it. */
void *Runtime_AllocateBlock(s32 kind, s32 size)
{
    u32 *allocator_state;
    s32 kind_offset;
    u32 aligned_size;
    u32 next;
    u32 address;
    u32 next_address;
    u32 cached_address;

    allocator_state = (u32 *)Ram_HeapSlots;
    kind_offset = kind;
    cached_address = *(u32 *)((u8 *)allocator_state + kind_offset);
    if (cached_address == 0) {
        aligned_size = (u32)size + 3;
        address = allocator_state[0];
        /* FAKEMATCH: Three plain shapes shift alignment before loading the heap head; keep that load before the shift. */
        __asm__("" : : "r"(address), "r"(aligned_size));
        aligned_size = (aligned_size >> 2) * 4;
        next = address + aligned_size;
        if (next >= 0x02040000U) {
            address = allocator_state[1];
            next_address = address + aligned_size;
            if (next_address > (u32)(Ram_IwramHeapEnd - 1)) {
                return 0;
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
