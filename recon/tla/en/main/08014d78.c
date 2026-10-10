/* Pin-free trial (tla-en): listing 24 instructions, draft 24, common 13, differ 22.
 * Earlier register-allocation notes describe the prior pinned form.
 */
/* Trial: the search reached differ 4 by reusing address for the bound;
 * that overwrites the heap head and changes the returned pointer, so it is
 * rejected. This trial keeps the original allocation behavior (differ 22).
 * Staging permuter seed 257, two jobs, two minutes: 17,574 distinct
 * candidates. Its differ-18 candidate returns held (the limit) on the
 * successful IWRAM path and is rejected. A return-copy guard differs 34;
 * direct or const state pointers differ 36. A low-register held constraint
 * and an input-only bound operand retain differ 22. RTL allocates the
 * short-lived bound locally before the cross-branch state pointer.
 */
#include "types.h"
#include "ram_buffer.h"
#include "mem.h"

void *MemAlloc(s32 size)
{
    u32 *state = (u32 *)Ram_HeapSlots;
    u32 other;
    u32 next;
    u32 address;
    u32 words;
    u32 limit;
    u32 held;

    words = (u32)size + 3;
    address = state[1];
    /* FAKEMATCH: keep the unshifted value and incoming size live through the heap-head load. */
    __asm__("" : : "r"(address), "r"(words), "r"(size));
    words >>= 2;
    /* FAKEMATCH: capture the bound before the final alignment shift. */
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
