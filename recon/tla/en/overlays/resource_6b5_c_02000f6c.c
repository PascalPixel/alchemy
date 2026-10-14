/* Draft, differ 8 of 33: the native reloads the owner byte after the 0xff test and sets the
 * result 1 in r0 before comparing it with owner; here CSE reuses the first load. The goto loop
 * keeps loop.c from strength-reducing the index (a for loop differs by 27). Tried: the 0xff test
 * in the for condition, a result variable. */
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
    if (gPartyState.active_owners[i] == owner)
        return 1;
    i++;
    if (i < count)
        goto next;
none:
    return 0;
}
