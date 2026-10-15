/* Draft, differ 2 of 33 (one reordered row): the native loads the party state
 * address at the loop top before it builds the 0x218 offset; here the offset's
 * movs comes first (sched2 priority 6 against the load's 5). The native reloads
 * the owner byte after the 0xff test and sets the result 1 before comparing it
 * with owner; the memory barrier reproduces both (without it, CSE reuses the
 * first load: differ 8). The goto loop keeps loop.c from strength-reducing the
 * index (a for loop differs by 27). Tried: the 0xff test in the for or while
 * condition (27), a switch (21, 25), a do-while (26), a result variable (11),
 * volatile reads (18, 22, 31), a party pointer, three minutes of permute. */
#include "types.h"
#include "party_state.h"

s32 GameFlag_Test(s32 flag);

s32 Func_02000f6c(s32 owner)
{
    s32 count = Party_CountActiveOwners();
    s32 limit = 3;
    s32 i;

    if (!GameFlag_Test(370))
        limit = 4;
    if (count > limit)
        count = limit;
    i = 0;
    if (i >= count)
        goto none;
next:
    if (gPartyState.active_owners[i] == 0xff)
        goto none;
    /* FAKEMATCH: the native reads the owner byte again for the second test (differ 8 without). */
    __asm__ volatile("" ::: "memory");
    if (gPartyState.active_owners[i] == owner)
        return 1;
    i++;
    if (i < count)
        goto next;
none:
    return 0;
}
