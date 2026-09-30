#include "TYPES.H"
#include "PARTY_STATE.H"

/* Copies the active party's members into owners, 0xff-terminated, and
   returns how many there are. */
s32 Party_ListActiveOwners(s16 *owners)
{
    s32 count = 0;

    if (owners != NULL) {
        s32 index;

        count = Party_CountActiveOwners();
        index = 0;
        if (count != 0) {
            do {
                *owners++ = gPartyState.active_owners[index];
                index++;
            } while (index != count);
        }
        *owners = 0xff;
    }
    return count;
}
