#include "TYPES.H"
#include "PARTY_STATE.H"

s32 GameFlag_Test(s32 flag);

/* Counts the party members who have joined: flags 0 to 7. */
s32 Party_CountActiveOwners(void)
{
    s32 count;
    s32 i;

    count = 0;
    for (i = 0; i <= 7; i++) {
        if (GameFlag_Test(i))
            count++;
    }
    return count;
}
