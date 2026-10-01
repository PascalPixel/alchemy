#include "TYPES.H"
#include "PARTY_STATE.H"

void GameFlag_ClearBit(s32);

s32 Party_RemoveActiveOwner(s32 value)
{
    s32 count = Party_CountActiveOwners();
    s32 i;
    s32 j;

    GameFlag_ClearBit(value);
    for (i = 0; i < count; i++) {
        if (gPartyState.active_owners[i] == value)
            break;
    }
    for (j = i; j < count - 1; j++)
        gPartyState.active_owners[j] = gPartyState.active_owners[j + 1];
    return Party_CountActiveOwners();
}

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
