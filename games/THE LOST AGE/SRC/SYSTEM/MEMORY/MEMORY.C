#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "IWRAM_CALL.H"
#include "RUNTIME_MEM.H"

/* Hands out a block for the heap slot at byte offset kind once, from IWRAM
   while it lasts and then from EWRAM below 0x02040000, and keeps it in the
   slot; ⚓️ passes the offset where ☀️ passes the slot number. Slots 0 and
   1 hold the EWRAM and IWRAM bump pointers. Returns 0 when both are full. */
void *Runtime_AllocateHeapBlock(s32 kind, s32 size)
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
                return NULL;
            }
            allocator_state[0] = next_address;
            *(u32 *)((u8 *)allocator_state + kind_offset) = address;
            return (void *)address;
        }
        allocator_state[1] = next;
        *(u32 *)((u8 *)allocator_state + kind_offset) = cached_address;
        return (void *)cached_address;
    }
    return (void *)cached_address;
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

/* Fills the still-free portions of the two bump heaps with a word value. */
void Runtime_FillFreeHeapWords(u32 value)
{
    u32 *state = (u32 *)Ram_HeapSlots;
    u32 address;

    address = state[1];
    Iwram_FillWords((void *)address, (u32)Ram_IwramHeapEnd - address, value);
    address = state[0];
    Iwram_FillWords((void *)address, 0x02040000U - address, value);
}

/* IWRAM-first bump allocation. All-six trials: separate/reused rounding
   locals differed by 17 bytes; named words by 12; two order constraints by
   4; an unconstrained bound by 11; the final tied handoff matched. */
void *Runtime_BumpAllocate(s32 size)
{
    u32 *state = (u32 *)Ram_HeapSlots;
    u32 other;
    u32 next;
    u32 address;
    u32 words;
    u32 limit;
    /* FAKEMATCH: An unconstrained bound load changes the heap/head registers; capture the native bound in r4 and hand it to an ordinary local. */
    register u32 held asm("r4");

    /* FAKEMATCH: The best plain shape swaps the heap-base shift and rounding add; retain the native address-first order. */
    __asm__("" : : "r"(state), "r"(size));
    words = (u32)size + 3;
    address = state[1];
    /* FAKEMATCH: The best plain shape shifts before loading the heap head; retain the unshifted value and incoming size through that load. */
    __asm__("" : : "r"(address), "r"(words), "r"(size));
    words >>= 2;
    /* FAKEMATCH: After the head-order repair, the native bound load and final alignment shift remain swapped; capture the bound before that shift. */
    __asm__("" : "=r"(held), "+r"(words) : "0"((u32)(Ram_IwramHeapEnd - 1)));
    limit = held;
    size = (s32)(words << 2);
    next = address + (u32)size;
    if (next > limit) {
        address = state[0];
        other = address + (u32)size;
        if (other >= 0x02040000U) {
            return NULL;
        }
        state[0] = other;
        goto finish;
    }
    state[1] = next;
finish:
    return (void *)address;
}

/* EWRAM-first bump allocation. All-six trials: separate/reused rounding
   locals differed by 13 bytes; named words by 8; two order constraints
   matched the complete 52-byte owner, excluding the following raw tail. */
void *Runtime_BumpAllocateAlternatePool(s32 size)
{
    u32 *state = (u32 *)Ram_HeapSlots;
    u32 other;
    u32 next;
    u32 address;
    u32 words;

    /* FAKEMATCH: The best plain shape swaps the heap-base shift and rounding add; retain the native address-first order. */
    __asm__("" : : "r"(state), "r"(size));
    words = (u32)size + 3;
    address = state[0];
    /* FAKEMATCH: The best plain shape shifts before loading the heap head; retain the unshifted value and incoming size through that load. */
    __asm__("" : : "r"(address), "r"(words), "r"(size));
    words >>= 2;
    size = (s32)(words << 2);
    next = address + (u32)size;
    if (next >= 0x02040000U) {
        address = state[1];
        other = address + (u32)size;
        if (other > (u32)(Ram_IwramHeapEnd - 1)) {
            return NULL;
        }
        state[1] = other;
        goto finish;
    }
    state[0] = next;
finish:
    return (void *)address;
}
